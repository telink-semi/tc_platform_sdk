/********************************************************************************************************
 * @file	pm_32k_xtal.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#if(__TL_LIB_8278__ || (MCU_CORE_TYPE == MCU_CORE_8278))

#include "compiler.h"


#include "bsp.h"
#include "compiler.h"
#include "register.h"
#include "analog.h"
#include "clock.h"
#include "irq.h"
#include "lib/include/pm.h"
#include "timer.h"
#include "watchdog.h"

//system timer clock source is constant 16M, never change
//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through the 750 and 751
//register, the conversion error(use 32k:16 cycle, count 16M sys tmr's ticks), at least the introduction of 64ppm.
#define CRYSTAL32768_TICK_PER_32CYCLE		15625  // 7812.5 * 2

#if PM_TIM_RECOVER_MODE
extern _attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

extern unsigned char pm_boost_dcdc_off_en;

extern unsigned int g_pm_rst_time;

static _attribute_no_inline_ void switch_ext32kpad_to_int32krc(unsigned char bit567_ana2c)
{
	analog_write(SYS_DEEP_ANA_REG,  analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear

	//switch 32k clk src: select internal 32k rc, if not do this, when deep+pad wakeup: there's no stable 32k clk(therefore, the pad wake-up time
	//is a bit long, the need for external 32k crystal vibration time) to count DCDC dly and XTAL dly. High temperatures even make it impossible
	//to vibrate, as the code for PWM excitation crystals has not yet been effectively executed. SO, we can switch 32k clk to the internal 32k rc.
	analog_write(0x2d, 0x15); //32k select:[7]:0 sel 32k rc,1:32k pad
	analog_write(0x05, 0x02); //[0]:32k rc;[1]:32k xtal (1->pwdn,0->pwup). Power up 32k rc.

	//deep + no tmr wakeup(we need  32k clk to count dcdc dly and xtal dly, but this case, ext 32k clk need close, here we use 32k rc instead)
	analog_write(0x2c, bit567_ana2c | 0x1f);
}


/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select,if only DEBOUNCE is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K watchdog will be turned off inside the function..
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
 * @return     indicate whether the cpu is wake up successful.
 */
_attribute_ram_code_sec_noinline_ int cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick)
{
	////////// disable IRQ //////////////////////////////////////////
	//If the time point of closing the total interrupt is later, the function may be interrupted by the interrupt,
	//cause the wake-up tick value to be calculated incorrectly, resulting in incorrect sleep time.
	//modify by weihua.zhang, confirmed by sihui.wang at 20220908.
	unsigned char r = irq_disable();

	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);

	//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through the 750 and 751 register, the conversion error, at least the introduction of 64ppm
	//tick_32k_calib = REG_ADDR32(0x754);
	//unsigned short  tick_32k_halfCalib = tick_32k_calib>>1;
	unsigned int span = (unsigned int)(wakeup_tick - clock_time ());

#if(PM_DEBUG)
	/******************************************debug_pm_info 1 **********************************************/
	debug_pm_info = 1;
	analog_write(0x3b, tick_32k_calib>>1);
	analog_write(0x3c, tick_32k_calib>>9);
#endif

#if(RAM_CRC_EN)
	unsigned char ram_crc_en = 0;
	unsigned short ram_crc_tick = 0;
#endif
	unsigned int emptyrun_time_deep = (pm_boost_dcdc_off_en ? (EMPTYRUN_TIME_US_DEEP_RET_NO_DCDC) : (EMPTYRUN_TIME_US_DEEP));
	if(timer_wakeup_enable){
		if (span > 0xE0000000)  //BIT(31)+BIT(30)+BIT(19)   7/8 cycle of 32bit
		{
			irq_restore(r);
			return  analog_read (0x44) & 0x1f;
		}
		else if (span < (((sleep_mode == SUSPEND_MODE)?(EMPTYRUN_TIME_US_SUSPEND):(emptyrun_time_deep)) * sys_tick_per_us)) // 0 us base
		{
			unsigned int t = clock_time ();
			analog_write (0x44, 0x1d);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x1d; ////clear the timer wake_up status(bit[1])before read 32k tick may cause get the error tick number from 32K.
			} while ( ((unsigned int)clock_time () - t < span) && !st);
			irq_restore(r);
			return st;

#if(PM_DEBUG)
	/******************************************debug_pm_info 2 **********************************************/
	debug_pm_info = 2;
#endif

		}
		else
		{
			//						thres			  16M		 24M		32M			48M
			//use 16 cycle   BIT(28) = 0x10000000   16.777 S   11.185 S   8.388 S      5.592 S
			//use 32 cycle   BIT(27) = 0x08000000    8.389 S 	5.592 S   4.194 S	   2.796 S
			//use 64 cycle   BIT(26) = 0x04000000 				2.796 S
			if(span >= 0x07FF0000){  //BIT(28) = 0x08000000   16M:8.389S
				pm_long_suspend = 1;
			}
			else{
				pm_long_suspend = 0;
			}
		}
	}else{
/*
 * When only DEBOUNCE is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K will always be in the sleep state.
 * If the 32K watchdog is enabled at this time, it will cause an abnormal reset.So if there is no Timer wake-up source, the 32K watchdog will be
 * turned off inside this function to prevent the application layer from misusing and causing an exception(changed by shuaixing at 20220722,confirm by weihua).
 */
		wd_32k_stop();
	}

#if(RAM_CRC_EN)
	if(wakeup_src == PM_WAKEUP_TIMER){
		if(((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW16K)&&(span > RAM_CRC_EN_16KRAM_TIME)) \
				|| ((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW32K)&&(span > RAM_CRC_EN_32KRAM_TIME))){
			analog_write(0x0b, 0x4c);    //ana_0b<3>: 1-enable retention LDO 0.6V.
			ram_crc_en = 1;
		}
	}
#endif

#if(PM_DEBUG)
	/******************************************debug_pm_info 3 **********************************************/
	debug_pm_info = 3;
#endif

	if(func_before_suspend){
		if (!func_before_suspend())
		{
			irq_restore(r);
			return WAKEUP_STATUS_PAD;
		}
	}

	tick_cur = clock_time () + 15 * sys_tick_per_us;  //cpu_get_32k_tick will cost 30~40 us
	reg_system_irq_mask &= (~BIT(2));   						//disable system timer irq
	//system_timer and 32kshould_cal should be disable before suspend(added by Yi Bao, confired by junwen and zhidong 20191015)
	//Otherwise, because the crystal oscillator is powered off during sleep,
	//there will be a period of time when the calibration value is inaccurate,
	//and this period of sleep will lead to crash, disconnection and other problems.(added by weihua.zhang, confired by peicai 20240207)
	reg_system_ctrl &= ~(FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN);
	tick_32k_cur = pm_get_32k_tick ();

#if (PM_TIM_RECOVER_MODE)
	pm_timRecover.recover_flag = 0;
	if(wakeup_src & PM_TIM_RECOVER_START){
		pm_timRecover.tick_sysClk = tick_cur;
		pm_timRecover.tick_32k = tick_32k_cur;
	}
	if(wakeup_src & PM_TIM_RECOVER_END){
		if(pm_timRecover.tick_sysClk && (unsigned int)(clock_time() - pm_timRecover.tick_sysClk) < BIT(26) ){  //BIT(26) = 2^26 = (16<<20)*4   about 4S
			pm_timRecover.recover_flag = 0x01;
		}
	}
#endif

#if(PM_DEBUG)
	/******************************************debug_pm_info 4 **********************************************/
	debug_pm_info = 4;
#endif

	unsigned int earlyWakeup_us = EARLYWAKEUP_TIME_US_SUSPEND;
	if(sleep_mode != SUSPEND_MODE){
		earlyWakeup_us = (pm_boost_dcdc_off_en ? (EARLYWAKEUP_TIME_US_DEEP_RET_NO_DCDC) : (EARLYWAKEUP_TIME_US_DEEP_RET));
	}
	unsigned int tick_wakeup_reset = wakeup_tick - earlyWakeup_us * sys_tick_per_us;

	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //[2] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter

	analog_write (0x44, 0x1f);				//clear all flag
	analog_write(0x2c, 0x5f);    //poweron_dft: 0x5f  <6> debounce reset

	///////////////////////// change to 24M RC clock before suspend /////////////
	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

#if(PM_DEBUG)
	/******************************************debug_pm_info 5 **********************************************/
	debug_pm_info = 5;
#endif

	unsigned char bit67_ana06;

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG) { //deepsleep with retention
		pm_cipher_key_save();

		bit67_ana06 = 0x40;  		//ana_06<7:6> -> poweron_dft: 11 -> 01 power up ret ldo and power down spd ldo
		tl_multi_addr_L = read_reg8(0x63e);
		if(0 == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0;
		}else if(0xf9 == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0x0;
		}else if(0xfa == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0x01;
		}else if(0xfc == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0x03;
		}
	}
	else if(sleep_mode){  //deepsleep no retention
		bit67_ana06  = 0xc0;  		//ana_06<7:6> -> poweron_dft: 11 -> 11 power down ret ldo and power down spd ldo
	}
	else{  //suspend
		bit67_ana06  = 0x00;  		//ana_06<7:6> -> poweron_dft: 10 -> 10 power down ret ldo and power up spd ldo
									//spd ldo setting at cpu_wakeup_init.

	}
	analog_write(0x7e, sleep_mode);
	analog_write(0x2b, (sleep_mode ? ((sleep_mode == SHUTDOWN_MODE)?0xff:0xfe) : 0x4c) );

#if(PM_DEBUG)
	/******************************************debug_pm_info 6 **********************************************/
	debug_pm_info = 6;
#endif

#if 1 //must
	if(sleep_mode == DEEPSLEEP_MODE && !timer_wakeup_enable){ //if deep mode and no tmr wakeup
		switch_ext32kpad_to_int32krc(0);
	}
	else if(sleep_mode == SUSPEND_MODE){ //suspend mode or deepretn mode or tmr wakeup (we don't pwdn ext 32k pad clk,even though three's no tmr wakeup src in susped or deepretn mode)
		analog_write(0x2c, (wakeup_src & PM_WAKEUP_DEBOUNCE)?0x1c:0x5c);//if use tmr wakeup, auto pad 32k pwdn should be disabled
	}
	else{
		analog_write(0x2c, (wakeup_src & PM_WAKEUP_DEBOUNCE)?0x1d:0x5d);
	}
#else
	analog_write(0x2c, 0x1d | (!timer_wakeup_enable)<<1);  //if use tmr wakeup, auto pad 32k pwdn should be disabled
#endif

	if(SHUTDOWN_MODE == sleep_mode)
	{
		analog_write(0x07, ((analog_read(0x07) & 0xc7) | 0x10));         //vbat lcldo_trim down 100(1.0v) -->  010(0.75v)
	}
	analog_write(0x06, (analog_read(0x06) & 0x3f) | bit67_ana06);

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG ) {
#if(RAM_CRC_EN)
		if(ram_crc_en)
		{
			if((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW16K)){//16k retention,check 16k sram crc
				write_reg8(0x40008,0x0f);					  // span > 2.5s or pad wakeup
				ram_crc_tick = (200 * 16);//170*16
			}
			else if((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW32K)){//32k retention,check 32k sram crc
				write_reg8(0x40008,0x1f);						   //span > 6s or pad wakeup
				ram_crc_tick = (350 * 16);
			}
			/*
			 * ram_crc_enable = ana_0x7f[1] & ram_crc
			 * ram_crc default 1, core_0x6f[4] write 1 to clear ram_crc, ram_crc set to 1 trigger by 0x6f = 0x81.
			 */
			write_reg8(0x6f, 0x10);
			/*
			 * ram_code_en = core_0x602[3] | ~crc_boot
			 * crc_boot = ana_0x7f[1]?crc_boot_o:ana_0x7f[0]
			 * crc_boot_o = ana_0x7f[0] | error
			 * error default 1, ram_crc_ok?error = 0:error = 1
			 * if deep_ret and ram_crc disable first, then deep_ret and ram_crc enable will cause ram_code disable, need to enable the core_602<3>
			 * modify by yi.bao, confirmed by jianzhi.chen at 20210708
			 */
			write_reg8(0x602, (read_reg8(0x602) | BIT(3)));
			if(0xff == g_chip_version)
			{
				analog_write(0x7f, 0x02);	//BIT<1> ram_crc_en
			}								//BIT<0> deep_ret not run boot_rom code while wakeup in A0 version
			else
			{
				analog_write(0x7f, 0x03);	//BIT<1> ram_crc_en,
											//BIT<0> boot_rom code disable will cause the otp_cmd invalid,so need to enable
											//boot_rom code and run a part of boot_rom code to ensure otp cmd valid in A1 version
			}
			tick_wakeup_reset -= ram_crc_tick;
		}
		else
#endif
		{
			if(0xff == g_chip_version)
			{
				analog_write(0x7f, 0x00);		//BIT<0> deep_ret not run boot_rom code while wakeup in A0 version
			}
			else
			{
				write_reg8(0x602, (read_reg8(0x602) | BIT(3)));
				analog_write(0x7f, 0x01);			//BIT<0> boot_rom code disable will cause the otp_cmd invalid,so need to enable
													//boot_rom code and run a part of boot_rom code to ensure otp cmd valid in A1 version
			}
		}
	}
	else{
		write_reg8(0x602, (read_reg8(0x602) | BIT(3)));      //~ana_7f<0> || core_602<3> = ram_code_en
		analog_write(0x7f, 0x01);
	}

#if(PM_DEBUG)
	/******************************************debug_pm_info 7 **********************************************/
	debug_pm_info = 7;
#endif

	//TODO:need change the xtal delay and dcdc delay after high-low temp test.
	//set DCDC delay and XTAL dealy durationadd xtal delay to cover the VDDDEC1V0 on to decrease the peak current when wakeup,
	//when ana_1f = 0x40, ana_20 = 0xa0, peak current about 50mA,ana_1f = ana_20 = 0xa0,peak current is about 20mA.
	//(0xff - n):  if timer wake up : ((n+1)*2) 32k cycle, use 32K/2 clock to count.
	//             else pad wake up: (((n+1)*2-1) ~ (n+1)*2 )32k cycle,pad wakeup has one cycle error at most,because the
	//             pad wakeup time is uncertain.
	//caution: actual XTAL delay time need <= DCDC delay,so the value of ana_1f <= the value of ana_20.

	unsigned int dcdc_dly = (SUSPEND_MODE == sleep_mode)?PM_DCDC_DELAY_DURATION_SUSPEND:PM_DCDC_DELAY_DURATION;
	unsigned int xtal_dly = (SUSPEND_MODE == sleep_mode)?PM_XTAL_DELAY_DURATION_SUSPEND:PM_XTAL_DELAY_DURATION;

	if(pm_boost_dcdc_off_en == 0){
		analog_write(0x05, analog_read(0x05) & ~(BIT(5)));		//poweron_boost_dcdc
		//set xtal delay duration
		span = (xtal_dly * (sys_tick_per_us>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
		unsigned char rst_cycle = 0xff - span;
		analog_write (0x20, rst_cycle);

		//set DCDC delay duration
		span = (dcdc_dly * (sys_tick_per_us>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
		rst_cycle = 0xff - span;
		analog_write (0x1f, rst_cycle);
	}
	else{
		analog_write(0x05, analog_read(0x05) | BIT(5));		//powerdown_boost_dcdc
		/*
		 * In order to reduce the startup time of the chip after turning off the boost dcdc function.
		 * modify by junhui.hu, confirmed by wenfeng.lou at 20220224.
		 */
		span = (g_pm_rst_time * (sys_tick_per_us>>1) * 3 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
		unsigned char rst_cycle = 0xff - span;
		analog_write (0x20, rst_cycle);
		analog_write (0x1f, rst_cycle);
	}

#if(PM_DEBUG)
	/******************************************debug_pm_info 8 **********************************************/
	debug_pm_info = 8;
#endif

	unsigned int tick_reset;


	if(pm_long_suspend){
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - tick_cur)/ CRYSTAL32768_TICK_PER_32CYCLE * 32;
	}
	else{
		tick_reset = tick_32k_cur + ((unsigned int)(tick_wakeup_reset - tick_cur) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1)) / CRYSTAL32768_TICK_PER_32CYCLE;
	}


#if (PM_TIM_RECOVER_MODE)
	unsigned int tick_reset_timRecover;
	if( (wakeup_src & PM_TIM_RECOVER_END) && pm_timRecover.recover_flag){


		if(pm_long_suspend){
			tick_reset_timRecover = pm_timRecover.tick_32k + (unsigned int)(tick_wakeup_reset - pm_timRecover.tick_sysClk)/ CRYSTAL32768_TICK_PER_32CYCLE * 32;
		}
		else{
			tick_reset_timRecover = pm_timRecover.tick_32k + ((unsigned int)(tick_wakeup_reset - pm_timRecover.tick_sysClk) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1)) / CRYSTAL32768_TICK_PER_32CYCLE;
		}


		int rest_32k_cycle = (int )(tick_reset_timRecover - tick_32k_cur);
		//if( rest_32k_cycle > 6  &&  abs(tick_reset - tick_reset_timRecover) < BIT(7)  ){  //128 * 30us(1/32k) = 3800us
		if( rest_32k_cycle > 6  &&  (unsigned int)(tick_reset + BIT(7) - tick_reset_timRecover) < BIT(8)  ){  //128 * 30us(1/32k) = 3800us
			pm_timRecover.recover_flag = 0x03;
			tick_reset = tick_reset_timRecover;
		}
		else{
			//debug
		}
	}
#endif

	// write 32k tick
	cpu_set_32k_tick(tick_reset);

#if(PM_DEBUG)
	analog_write(0x35, tick_32k_cur);
	analog_write(0x36, tick_32k_cur>>8);
	analog_write(0x37, tick_32k_cur>>16);
	analog_write(0x38, tick_32k_cur>>24);

	ana_32k_tick = 0;
	ana_32k_tick |= analog_read(0x4c);
	ana_32k_tick |= analog_read(0x4d)<<8;
	ana_32k_tick |= analog_read(0x4e)<<16;
	ana_32k_tick |= analog_read(0x4f)<<24;
	if(tick_reset != ana_32k_tick)
	{
		REG_ADDR8(0x74c) = 0x92;
		flash_write_page(0x10000, 4, (unsigned char *)&ana_32k_tick);
		gpio_write(GPIO_PD5, 0);
		while(1);
	}
	/******************************************debug_pm_info 9 **********************************************/
	debug_pm_info = 9;
#endif

	if(analog_read(0x44)&0x1f){

	}
	else{
		sleep_start();
	}



	if(sleep_mode == DEEPSLEEP_MODE){ //
		write_reg8 (0x6f, 0x20);  //reboot
	}

	unsigned int now_tick_32k = pm_get_32k_tick();

#if (PM_TIM_RECOVER_MODE)
	if(pm_timRecover.recover_flag == 0x03){

		if(pm_long_suspend){
			tick_cur = pm_timRecover.tick_sysClk+ (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			tick_cur = pm_timRecover.tick_sysClk + (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
	}
	else
#endif
	{
		if(pm_long_suspend){
			tick_cur += (unsigned int)(now_tick_32k - tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			tick_cur += (unsigned int)(now_tick_32k - tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
	}

	reg_system_tick = tick_cur + 20 * sys_tick_per_us;
	reg_system_ctrl |= FLD_SYSTEM_32K_CAL_EN|FLD_SYSTEM_TIMER_MANUAL_EN;
	reg_system_irq_mask |= BIT(2);   										//enable system timer irq

#if(PM_DEBUG)
	/******************************************debug_pm_info 10 **********************************************/
	debug_pm_info = 10;
#endif

	write_reg8 (0x066, reg66);		//restore system clock

#if(PM_DEBUG)
	/******************************************debug_pm_info 11 **********************************************/
	debug_pm_info = 11;
#endif

	unsigned char anareg44 = analog_read(0x44);

	if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}

	irq_restore(r);

#if(PM_DEBUG)
	/******************************************debug_pm_info 12 **********************************************/
	debug_pm_info = 12;
#endif
	/**
	 * Under normal circumstances, the wake up source cannot be zero. B85 had an exception that the wake up source was zero and never encountered it again.
	 * STATUS_GPIO_ERR_NO_ENTER_PM indicates this case where the wake up source is zero. The name of this flag is not quite appropriate, but it has been used for a long time, so it is still used today.
	 * added by bingyu.li, confirmed by sihui.wang at 20231018.
	 */
	return (anareg44 ? (anareg44 | STATUS_ENTER_SUSPEND) : STATUS_GPIO_ERR_NO_ENTER_PM );
}


_attribute_ram_code_sec_noinline_ unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k)
{
	unsigned int deepRet_tick;

#if (PM_TIM_RECOVER_MODE)
	if(pm_timRecover.recover_flag == 0x03){
		deepRet_tick = pm_timRecover.tick_sysClk + (unsigned int)(now_tick_32k - pm_timRecover.tick_32k) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
	}
	else
#endif
	{

#if PM_LONG_SUSPEND_EN  //do not consider  deepsleep retention  timer wake_up(bigger than 8S)
		if(pm_long_suspend){
			deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
#else
		deepRet_tick = tick_cur + (unsigned int)(now_tick_32k - tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
#endif
	}

	return deepRet_tick;
}



_attribute_data_retention_ static unsigned int tick_check32kPad = 0;


_attribute_no_inline_ void check_32k_clk_stable(void)
{
	if(clock_time_exceed(tick_check32kPad, 10000)){ //every 10ms, check if 32k pad clk is stable
		tick_check32kPad = clock_time();

		unsigned int last_32k_tick;
		unsigned int curr_32k_tick;

		//Check if 32k pad vibration and basically works stably
		last_32k_tick = pm_get_32k_tick();
		WaitUs(50); //for 32k tick accumulator, tick period: 30.5us, if stable: delta tick > 0
		curr_32k_tick = pm_get_32k_tick();

		if(last_32k_tick != curr_32k_tick){
			blt_miscParam.pm_enter_en = 1;//allow enter pm
			return;
		}
	}
	else{
		return;
	}

	// T > 2s , 32k pad clk still unstable: reboot MCU
	if(!blt_miscParam.pm_enter_en && clock_time_exceed(0, 2000000)){
		analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
		start_reboot(); //reboot the MCU
	}
}

#endif











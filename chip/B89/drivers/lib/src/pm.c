/********************************************************************************************************
 * @file	pm.c
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
#include "bsp.h"

#include "compiler.h"
#include "register.h"
#include "gpio.h"
#include "analog.h"
#include "clock.h"
#include "timer.h"
#include "irq.h"
#include "lib/include/rf_drv.h"
#include "lib/include/pm.h"
#include "adc.h"
#include "otp/otp.h"

unsigned char	otp_program_flag = 0;

//this value is used to control whether boost_dcdc is turned on after the next deep retention sleep wake-up, it is 0 to turn on and 1 to turn off.
_attribute_data_retention_ unsigned char pm_boost_dcdc_off_en = 0;

//The default value is xtal delay, if a customer's crystal vibrates for a long time, use the pm_set_rst_cycle() function to modify it.
_attribute_data_retention_ unsigned int g_pm_rst_time = 31.25*2*3;

#define FLASH_DEEP_EN				1   //if flash sleep,  when deep retention, must wakeup flash in cstartup

_attribute_data_retention_  unsigned char 		tl_multi_addr_L;
_attribute_data_retention_  unsigned char 		tl_multi_addr_H;
_attribute_data_retention_  unsigned char 		tl_24mrc_cal = 0x80;
_attribute_data_retention_  unsigned char       pm_long_suspend;
_attribute_data_retention_  unsigned char 		pm_1p2_power_sel_vbat = 0;    //  0 - supply from DCDC output;  1 - supply from VBAT

_attribute_data_retention_  static unsigned char cipher_key[6] = {0};

_attribute_data_retention_  _attribute_aligned_(4) misc_para_t 		blt_miscParam;
cpu_pm_handler_t 		  	cpu_sleep_wakeup;  //no need retention,  cause it will be set every wake_up
pm_tim_recover_handler_t    pm_tim_recover;
check_32k_clk_handler_t   	pm_check_32k_clk_stable = 0;

unsigned int g_pm_watchdog_enable = 0;
_attribute_data_retention_  unsigned int RAM_CRC_EN_16KRAM_TIME = 2500*16*1000;
_attribute_data_retention_  unsigned int RAM_CRC_EN_32KRAM_TIME = 6*16*1000*1000;

/* wakeup source :is_deepretn_back,is_pad_wakeup,wakeup_src*/
_attribute_aligned_(4) pm_para_t pmParam;
/* enable status :conn_mark,ext_cap_en,pad32k_en,pm_enter_en */
_attribute_data_retention_	unsigned int 		tick_32k_calib;
_attribute_data_retention_	unsigned int 		tick_cur;
_attribute_data_retention_	unsigned int 		tick_32k_cur;

_attribute_data_retention_	unsigned char 		g_chip_version;


/*32k rc calibrate configuration, the default value is set to require calibration*/
volatile static unsigned char g_32k_rc_calib_flag = 1;
/* tick :tick_sysClk,tick_32k,recover_flag */
#if (PM_TIM_RECOVER_MODE)
	_attribute_data_retention_	_attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

_attribute_data_retention_  static unsigned char       g_pm_suspend_power_cfg = 0x81;

/**
 * @brief		This function saves ram cipher encryption keys before deep retention.
 * @param[in]	none.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void pm_cipher_key_save(void)
{
	for(unsigned char i = 0; i < 5; i++)
	{
		cipher_key[i] = read_reg8(0x500+i);
	}
	cipher_key[5] = read_reg8(0x508);
}

/**
 * @brief		This function restores ram cipher encryption keys after deep retention.
 * @param[in]	none.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void pm_cipher_key_recover(void)
{
	for(unsigned char i = 0; i < 5; i++)
	{
		REG_ADDR8(0x500+i) = cipher_key[i];
	}
	read_reg8(0x508) = cipher_key[5];
}

/**
 * @brief		This function provides the function to turn off boost dcdc. When boost dcdc is turned off,
 * 				the voltage of the GPIO port on the chip's OTP module will drop to the VBAT voltage.
 * @param[in]	none.
 * @return		none.
 */
void pm_boost_dcdc_dis(void)
{
	pm_boost_dcdc_off_en = 1;
}

/**
 * @brief		This function provides the function to turn on boost dcdc.
 * @param[in]	none.
 * @return		none.
 */
void pm_boost_dcdc_en(void)
{
	pm_boost_dcdc_off_en = 0;
}

/**
 * @brief		This function serves to view the status of boost dcdc.
 * @return		status of boost dcdc, 1 - boost dcdc is open, 0 - boost dcdc is close.
 */
unsigned char pm_get_boost_dcdc_status(void)
{
	unsigned char boost_dcdc_status = 0;
	if(BIT(5) == (analog_read(0x05) & BIT(5))){
		boost_dcdc_status = 0;
	}
	else{
		boost_dcdc_status = 1;
	}
	return boost_dcdc_status;
}

/**
 * @brief		This function provides the function to turn on boost dcdc.
 * @param[in]	rst_time_value - set the value of reset time.
 * @return		none.
 */
void pm_set_rst_time(unsigned int rst_time_value)
{
	g_pm_rst_time = rst_time_value;
}

/**
 * @brief		This function serves to set baseband power on/off before suspend sleep,If power
 * 				on this module,the suspend current will increase;power down this module will save current,
 * 				but you need to re-init this module after suspend wakeup.All power down default to save
 * 				current.
 * @param[in]	value - weather to power on/off the baseband.
 * @param[in]	on_off - select power on or off, 0 - power off; other value - power on.
 * @return		none.
 */
void pm_set_suspend_power_cfg(pm_suspend_power_cfg_e value, unsigned char on_off)
{
	if(0 == on_off){
		g_pm_suspend_power_cfg |= (value);
	}
	else{
		g_pm_suspend_power_cfg &= ~(value);
	}
}

_attribute_data_retention_	suspend_handler_t	func_before_suspend = 0;

void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
	func_before_suspend = func;
}

/**
 * @brief		This function serves to update wakeup status.
 * @return		none.
 */
void pm_update_status_info(void)
{
	unsigned char analog_39 = analog_read(DEEP_ANA_REG10);
	unsigned char analog_3a = analog_read(DEEP_ANA_REG0);

	g_chip_version = read_reg8(0x7d);
	if(analog_39 & BIT(0))
	{
		if(analog_3a & BIT(0))
		{
			pmParam.mcu_status = MCU_STATUS_REBOOT_BACK;
			analog_write(DEEP_ANA_REG10, analog_39 & 0xfe);
		}else
		{
			pmParam.mcu_status = MCU_STATUS_POWER_ON;
			analog_write(DEEP_ANA_REG10,analog_39 & 0xfe);
			analog_write(DEEP_ANA_REG0, analog_3a | BIT(0));
		}
	}
	else
	{
		if(!((analog_read(0x06) & BIT(7))))
		{
			pmParam.mcu_status = MCU_STATUS_DEEPRET_BACK;
		}
		else
		{
			pmParam.mcu_status = MCU_STATUS_DEEP_BACK;
		}
	}
	pmParam.wakeup_src = analog_read(0x44);

	if (( pmParam.wakeup_src & WAKEUP_STATUS_TIMER_PAD ) == WAKEUP_STATUS_PAD)  //pad, no timer
	{
		pmParam.is_pad_wakeup = 1;
	}
}


/**
 * @brief     this function servers to manual set crystal.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void crystal_manual_settle(void)
{
	//xo_quict_settle with auto will cause the pragram crash in high temperature, must
	//be set to manual mode.(add by Yi Bao, confirmed by wenfeng 20190911)
	unsigned char  ana_2c = analog_read(0x2c);
	unsigned char  ana_05 = analog_read(0x05);
	analog_write(0x2c, ana_2c | 0x20);   //0x2c<5>: xo_quick_rst
	analog_write(0x2c, ana_2c & 0xdf);	//<5> 1b'1: xtal 24M quick settle count
	analog_write(0x05, ana_05 | 0x08);   //0x05<3>: 24M_xtl_pd
	analog_write(0x05, ana_05 & 0xf7);   //<3>1b'0: Power up 24MHz XTL oscillator
										 //<3>1b'1: Power down 24MHz XTL oscillator
}

/**
 * @brief     this function servers to start sleep mode.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_noinline_  void  sleep_start(void)
{
	analog_write(0x34, g_pm_suspend_power_cfg);
											//BIT<0> ZB  power down


	analog_write(0xe4,0x10);  //<4>: power down vmid(audio enable)  avoid current leakage, save 2uA
	//analog_write(0x82,0x85);  //power down signal of bbpll to avoid current leakage(confired by wenfeng 20191017)

	//If the ADC VBAT voltage divider mode is turned on, the sleep base current will increase by about 14uA, so it will be turned off before sleep,
	//and then restored after sleep. changed by junhui.hu at 20220302.confirmed by haitao.gu.
	unsigned char ana_01 = analog_read(0x01);
	analog_write(0x01, ana_01 & 0x7f); //<7>: detect vbat supply, 1: supply divide enable for vbat detection,
																//0:supply divide disable

	volatile unsigned int i;

/*************************************************************************
 * Sleep start and Flash Power down/on, can not insert any other code here
*************************************************************************/
	//if code run in otp,otp need enter deep to save current.
    if(otp_program_flag)
    {
    	otp_set_deep_standby_mode();
    }
    else{
	//the kestrel will power off the flash when it sleeps, so there is no need for the flash to enter the deep operation.
#if FLASH_DEEP_EN
    write_reg8(0x00d, 0);
    write_reg8(0x00c, 0xb9);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    write_reg8(0x00d, 1);

    write_reg8(0x5a1, 0x00); //MSPI ie disable
#endif
	}

#if(PM_DEBUG)
	/******************************************debug_info**********************************************/
	debug_pm_info = 20;
#endif

/*
 * Use for loop to delay may cause program crash,need use tnop to instead.the real reason is unknown.guangjun is suggested that all telink mcu use this solution.
 * changed by yi.bao at 20210628.confirmed by guangjun.
 */
    void start_suspend(void);
    start_suspend();              //sleep  core_6f = 0x81;

    //Attention: If Execute code from flash or otp, MCU will die because flash not alive now
    //if code run in otp,open otp pce and pdstb.
	if(otp_program_flag)
	{
		otp_set_active_mode();
	}
	else{
#if FLASH_DEEP_EN
    write_reg8(0x5a1, 0x0f); //MSPI ie enable

    write_reg8(0x00d, 0);
    write_reg8(0x00c, 0xab);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    write_reg8(0x00d, 1);
#endif
	}


/**************************************************************************/
    crystal_manual_settle();

    analog_write(0x01, ana_01); //<7>: detect vbat supply, 1: supply divide enable for vbat detection, 0:supply divide disable

    analog_write(0xe4,0x00);//<4>: power on vmid(audio enable)
    analog_write(0x34,0x80);  //BIT<2:0> audio/USB/ZB  power on

    analog_write(0x2c, 0x5f);    //poweron_dft: 0x5f  <6> debounce reset

#if(PM_XTAL_MANUAL_MODE_DELAY == 100)

#elif (PM_XTAL_MANUAL_MODE_DELAY == 200)
    for(i = 0; i < 300; i++){//200us
    	asm("tnop");
    }
#else

#endif


	//"PM_XTAL_MANUAL_MODE_DELAY" delay timing end

//    DBG_CHN1_LOW;

    //check 3 times for safety
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana

}

/**
 * @brief     this function servers to start reboot.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void start_reboot(void)
{
	if(blt_miscParam.pad32k_en){
		analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
	}

	irq_disable ();
	REG_ADDR8(0x6f) = 0x20;  //reboot
	while (1);
}

/**
 * Reading a 32k tick value using a digital register may cause a crash, so an analog register is used instead.
 * add by bingyu.li,confirmed by jianzhi.chen
 */
#if 1
/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */
_attribute_ram_code_sec_noinline_ unsigned int pm_get_32k_tick(void)
{
    unsigned int t0 = 0;
    unsigned int t1 = 0;
    unsigned int n = 0;

    while (1) {

        t0 = t1;
        t1 = analog_read(0x43);
        t1 <<= 8;
        t1 += analog_read(0x42);
        t1 <<= 8;
        t1 += analog_read(0x41);
        t1 <<= 8;
        t1 += analog_read(0x40);
        if (n)
        {
        	/**
			 * The 32k tick value is read byte by byte. If the read occurs during a jump on the edge, the result will be that several bytes are the value of the previous tick and several bytes are the
			 * value of the later tick, resulting in an incorrect tick value. Therefore, after reading the analog register, add a judgment: (t1-t0) is <2 (that is, t1-t0 is 0/1, 0/1 is normal).
			 * add by bingyu.li,confirmed by sihui.wang
			 */
            if ((unsigned int)(t1 - t0) < 2) {
                return t1;
            }
            /**
			 * After checking with BLE team sihui and sunwei, the code for the "else if" branch is redundant, but it has been used for a long time, so it is still used today.
			 * add by bingyu.li,confirmed by sihui.wang
			 */
            else if ( (t0^t1) == 1 ) {
                return t0;
            }
        }
        n++;
    }
    return t1;
}
#else
/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */
_attribute_ram_code_sec_noinline_ _attribute_no_inline_ unsigned int cpu_get_32k_tick (void)
{
//	analog_write(0x44, analog_read(0x44) | BIT(1));				//clear timer wakeup flag(can not clear when read 32k)
//	reg_system_irq_mask &= (~BIT(2));   						//disable system timer irq
	//system_timer and 32kshould_cal should be disable before suspend(added by Yi Bao, confired by junwen and zhidong 20191015)
	reg_system_ctrl &= (~(FLD_SYSTEM_32K_WR_EN|FLD_SYSTEM_TIMER_AUTO_EN));             // <0>: 1- write , 0- read
	while((reg_system_status & FLD_SYSTEM_32K_TIMER_UPDATE_RD));
	while(!(reg_system_status & FLD_SYSTEM_32K_TIMER_UPDATE_RD));
	reg_system_ctrl |= FLD_SYSTEM_TIMER_AUTO_EN;
 	return reg_system_32k_tick_rd;
}
#endif

/**
 * @brief  This function serves to set the 32k tick.
 * @param  tick - the tick need to set.
 * @return none.
 */
_attribute_ram_code_sec_noinline_ void cpu_set_32k_tick(unsigned int tick)
{

	reg_system_ctrl |= FLD_SYSTEM_32K_WR_EN;  	// <0>: 1- write , 0- read
	while(reg_system_status & FLD_SYSTEM_32K_TIMER_BUSY);

	reg_system_32k_tick_wt = tick;

	reg_system_status = FLD_SYSTEM_32K_TIMER_SYNC_TRIG;

	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	while(reg_system_status & FLD_SYSTEM_32K_TIMER_SYNC_TRIG);
}

/**
 * @brief      This function serves to get a valid level of 32k tick value, and there is a FIFO inside the chip with a size of 16 Half Words,
 * 			   and the highest bit of data in FIFO represents what level data is there. If the FIFO is full, the following data will be lost,
 * 			   and only the previously stored data will be retained.
 * @param[in]  none.
 * @return     data[14:0]: 32k tick value, ranges from 0 to 32767, which is 0~1023.979ms.
 * 			   data[15]: the data level polarity, 0 - low level, 1 - high level.
 */
unsigned short pm_debounce_read_data(void)
{
	unsigned short data;
	data = analog_read(0x45);			//ana_45<7:0> is the 7-0 bits of data read from the fifo.
	data |= (analog_read(0x46) << 8);	//ana_46<6:0> is the 14-8 bits of data read from the fifo.
										//ana_46<7> 1 indicates that the pen is high level data, 0 indicates that the pen is low level data.
	analog_write(0x46, analog_read(0x46) | BIT(7));	//ana_46<7> write 1 to read next 16bit data from fifo.
	return data;
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer0.
 * @param[in]   tick - capture value of timer0.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer0(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr0_tick = 0;

    reg_tmr0_capt = tick;
    reg_tmr_ctrl16 &= (~FLD_TMR0_MODE);
    reg_tmr_ctrl8 |= FLD_TMR0_EN;//enable TIMER0,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR0_EN;//timer0 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR0_EN;//disable TIMER0
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer1.
 * @param   tick - capture value of timer1.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer1(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr1_tick = 0;

    reg_tmr1_capt = tick;
    reg_tmr_ctrl16 &= (~FLD_TMR1_MODE);
    reg_tmr_ctrl8 |= FLD_TMR1_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR1_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR1_EN;//disable TIMER1
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer2.
 * @param[in]   tick - capture value of timer2.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer2(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr2_tick = 0;

    reg_tmr2_capt = tick;
    reg_tmr_ctrl16 &= (~FLD_TMR2_MODE);
    reg_tmr_ctrl8 |= FLD_TMR2_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR2_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR2_EN;//disable TIMER1
}

#if 0
/**
 * @brief   This function serves to wake up cpu from stall mode by timer1 or RF TX done irq.
 * @param   WakeupSrc  - timer1.
 * @param   IntervalUs - capture value of timer1.
 * @param   sysclktick - tick value of per us based on system clock.
 * @return  none.
 */
_attribute_ram_code_sec_noinline_ unsigned int cpu_stall(int WakeupSrc, unsigned int IntervalUs,unsigned int sysclktick)
{
    unsigned int wakeup_src = 0;

    if (IntervalUs) {
        reg_tmr1_tick = 0;
        reg_tmr1_capt = IntervalUs * sysclktick;
        reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
        reg_tmr_ctrl8 &= (~FLD_TMR1_MODE); //select mode0: using sclk
        reg_tmr_ctrl8 |= FLD_TMR1_EN; //enable TIMER1
    }
    REG_ADDR32(0x78) |= WakeupSrc;  //FLD_IRQ_TMR1_EN | FLD_IRQ_ZB_RT_EN

    reg_irq_mask &= (~FLD_IRQ_ZB_RT_EN);
    reg_irq_mask &= (~FLD_IRQ_TMR1_EN);

    write_reg8(0x6f, 0x80); //stall mcu
    asm("tnop");
    asm("tnop");

    if (IntervalUs) {
        reg_tmr1_tick = 0;
        reg_tmr_ctrl8 &= (~FLD_TMR1_EN); //disable TIMER1
    }

    //store the wakeup source
    wakeup_src = REG_ADDR32(0x40);

    //clear the source
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clear timer1 irq source
    write_reg16(0xf20, 0xffff); //clear rf irq status

    return wakeup_src;
}
#endif

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   kick_ms - duration of kick.
 * @return  	none.
 */
static _attribute_no_inline_ void pwm_kick_32k_pad(unsigned int kick_ms)
{
	//2.set pc2 as pwm output
	write_reg8(0x66,  0xe0); //sys clk use 16M crystal
	write_reg8(0x782, 0xf3); //pwm clk div  f3

	write_reg8(0x5b1, 0x04); //pc2 as pwm func
	write_reg8(0x596, 0xfb); //pc2 disable gpio
	write_reg32(0x794,0x20001); //pwm0's high time or low time && pwm1's cycle time
	write_reg8(0x781, 0x01);

	//3.wait for PWM wake up Xtal

	sleep_us(kick_ms*1000);

	//4.Recover PC2 as Xtal pin
	write_reg8(0x66,   0x06); //default value
	write_reg8(0x781, 0x00);  //default value
	write_reg32(0x794, 0x00); //default value
	write_reg8(0x596,  0xff); //default value
	write_reg8(0x5b1, 0x00);  //default value
	write_reg8(0x782,  0x00); //default value
}

/**
 * @brief   	This function serves to delay 13ms based on 24M rc clk.
 * @param[in]   none.
 * @return  	none.
 */
__attribute__((optimize("-O0")))
_attribute_no_inline_ void soft_reboot_dly13ms_use24mRC(void)
{
	for(int i = 0; i < 15500; i++); //about 13ms when 24M rc clk
}

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   times - the frequency of being kicked by pwm.
 * @return  	0-kick success; 1-kick fail.
 */
static _attribute_no_inline_ unsigned char pwm_kick_32k_pad_times(unsigned int times)
{
	if(times){

#if 0 //debug code
		write_reg8(0x73,0x07);
		write_reg8(0x586,0x1d);
#endif
		//1. select 32k xtal
		analog_write(0x2d, 0x95);//32k select:[7]:0 sel 32k rc,1:32k XTAL

		#if 0 //must close, reason is as follows:
			/*
			 * This problem occurs with suspend and deep and deep retention. When the power supply voltage is low, suspend/deep/deep retention cannot
			 * be reset within 12ms, otherwise softstart will work. However, there is not enough delay to wait for softstart to complete. This delay
			 * will be postponed to the code execution area and it will not be able to handle larger code, otherwise it will be dropped by 1.8v, load
			 * error, and finally stuck.(Root: DCDC dly depends on the 32k rc clock, so the 32k rc power supply can't be turned off here.)
			 */
			analog_write(0x05, 0x01);//Power down 32KHz RC,  Power up [32KHz crystal, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]
		#else
			analog_write(0x05, 0x00);//Power up 32KHz RC,  Power up 32KHz crystal
		#endif

		//analog_write(0x03,0x4f); //<7:6>current select(default value, not need)

		int last_32k_tick;
		int curr_32k_tick;
		int i = 0;
		for(i = 0; i< times; i++){

			//After 10ms, the external 32k crystal clk is considered stable(when using PWM to accelerate the oscillation process)
			pwm_kick_32k_pad(10);//PWM kick external 32k pad (duration 10ms)

			//Check if 32k pad vibration and basically works stably
			last_32k_tick = pm_get_32k_tick();

			sleep_us(305);//for 32k tick accumulator, tick period: 30.5us, dly 10 ticks

			curr_32k_tick = pm_get_32k_tick();

			if(last_32k_tick != curr_32k_tick){ //pwm kick 32k pad success
				return 0;
			}
		}

		#if (0) //blt_sdk_main_loop: check if 32k pad stable, if not, reboot MCU

			if(i >= times){
				analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
				start_reboot(); //reboot the MCU
			}
		#endif
	}
	return 1;
}

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  Pin - the pins can be set to all GPIO except PA2 port and GPIOC groups(PC groups for A0,A1 chip)
 * @param[in]  Pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup)
 * @param[in]  En  - enable or disable the wakeup function for the pan pin(1: Enable, 0: Disable)
 * @return     none
 */
void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en)
{
	///////////////////////////////////////////////////////////
	// 		  PA[7:0]	    PB[7:0]		PC[7:0]		PD[7:0]
	// en: 	ana_21<7:0>	 ana_22<7:0>  ana_23<7:0>  ana_24<7:0>
	// pol:	ana_27<7:0>	 ana_28<7:0>  ana_29<7:0>  ana_2a<7:0>
    unsigned char mask = pin & 0xff;
	unsigned char areg;
	unsigned char val;

	////////////////////////// polarity ////////////////////////

	areg = ((pin>>8) + 0x21);
	val = analog_read(areg);
	if (pol) {
		val &= ~mask;
	}
	else {
		val |= mask;
	}
	analog_write (areg, val);

	/////////////////////////// enable /////////////////////
	areg = ((pin>>8) + 0x27);
	val = analog_read(areg);
	if (en) {
		val |= mask;
	}
	else {
		val &= ~mask;
	}
	analog_write (areg, val);
}



/**
 * @brief   	This function serves to initiate the cpu after power on or deepsleep mode.
 * @param[in]   none.
 * @return  	none.
 */
static _attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
{

	//Set 32k clk src: external 32k crystal, only need init when deep+pad wakeup or 1st power on
	if(blt_miscParam.pad32k_en){

		//in this case: ext 32k clk was closed, here need re-init.
		//when deep_pad wakeup or first power on, it needs pwm acc 32k pad vibration time(dly 10ms)
		{
			pwm_kick_32k_pad_times(10);   //kick success if the return value is 0, return 1 mean kick fail;if kick failed , can call
										  //void check_32k_clk_stable(void)(the API used in ble sdk) to solve it or solve it by self.

		}
	}
	else{
		//default 32k clk src: internal 32k rc, here can be optimized
		//analog_write(0x2d, 0x15); //32k select:[7]:0 sel 32k rc,1:32k pad
		//analog_write(0x05, 0x02); //Power down 32k crystal,  Power up [32KHz RC, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]

		/*
			In some customer application scenarios, they want code execution time to be short and power consumption to be low.
			Meanwhile, they do not concerned about the accuracy of 32k RC or they want to control the calibration cycle themselves. 
			Therefore, here we provide a way for users to configure the calibration logic without affecting compatibility.
		*/
		if (g_32k_rc_calib_flag == 1)
		{
			rc_32k_cal();  //6.69 ms/6.7 ms
		}
	}
	//////////////////// calib adc vref ////////////////////
	/**
		 * change adc Vref calibrate operation, by call adc_update_1p2_vref_calib_value() in drivers/adc.c.
		 * changed by chaofan.20201029.
	*/
//	adc_update_1p2_vref_calib_value();
}

/**
 * @brief   This function serves to initialize MCU
 * @param   power mode- set the power mode(LOD mode, DCDC mode, DCDC_LDO mode)
 * @param   xtal- set this parameter based on external crystal
 * @return  none
 * @note	1. The 1p2 power(include RF_LDO, analog_ldo, digital_ldo and SRAM_ldo) default use dcdc mode to supply power.
 * 				Call the API of pm_set_1p2_power_sel() can change the power mode, but need to caution that:
 *				Application need to detect the battery voltage,if VBAT is bigger than 1.2V, you can use all two mode, but:
 *				Use DCDC mode:tx current will increase than VBAT mode.
 *				Use VBAT mode:the tx power will decreased with the VBAT voltage.
 *				if the VBAT voltage is less than 1.2V, must use DCDC mode, otherwise the chip function will abnormal.
 * 			2. When this function called after power on or deep sleep wakeup, it will cost about 6~7ms for perform 32k RC calibration. 
 * 				If do not want this logic, you can check the usage and precautions of cpu_wakeup_init_calib_32k_rc_cfg().
 */
_attribute_ram_code_sec_noinline_
void cpu_wakeup_init(XTAL_TypeDef xtal)    //must on ramcode
{
	write_reg8(0x60, 0x00); 	//poweron_dft: 0x7c -> 0x00
	write_reg8(0x61, 0x00); 	//poweron_dft: 0xff -> 0x00
	write_reg8(0x62, 0x08); 	//poweron_dft: 0xcf -> 0xc8
								//<7>:mcic1,R  	default:1
								//<6>:risc1,R  	default:1
								//<3>:trng  	default:1,

	write_reg8(0x63, 0xff); 	//poweron_dft: 0x83 -> 0xff
	write_reg8(0x64, 0xff); 	//poweron_dft: 0x00 -> 0x9f
								//<5>/<6> undefined ,default:0

	write_reg8(0x65, 0xf7); 	//poweron_dft: 0x30 -> 0x37
								//<0>:aif -> disable 32k for qdec
								//<5/6>:undefined,default:0

	//when load code twice without power down dut, dut will use crystal clock in here,
	//xo_quick_settle manual mode need to use in RC colck.
	write_reg8(0x66, 0x06);		//poweron_dft :set RC clock as system clock.

	crystal_manual_settle();

	analog_write(0x01, 0x44);       //bit<2> pm_pd_osc32k_i50n_vbat, save about 50nA current.

	analog_write(0x04, 0x92);       //bit<1:0> rc_osc_32k_trim_vbat,dft 2b'01  -->  2b'10
									//ana_04<7:5> dig suspend LDO output voltage trim  default:100-1.0v
									//ana_04<4:2> ana suspend LDO output voltage trim  default:100-1.0v

	analog_write(0x8c, 0x02);    //enable crystal clock from crystal to analog.


	analog_write(0x82, 0x20);  	//poweron_dft: 0x00
								//<4:0>:rsvd
								//<5>: enable 24M clock to digital core
								//<6>: enable signal of 24M clock to sar

	analog_write(0x34, 0x80);	//poweron_dft: 0x07 -> 0x80
								//<0>: power on baseband
								//<6:1>: rsvd
								//<7>: enable change power sequence clk


	if(pm_boost_dcdc_off_en == 0){
		analog_write(0x07, 0xa4);	//select supply from DCDC output.
									//bit<6> vdd1p8v_supply_sel: 0: open switch ---->select supply from DCDC output; 1: close switch
									//bit<7> vbat_supply_sel: 0: open switch ----->select supply from VBAT; 1: close switch
	}
	else{
		analog_write(0x07, 0x64);	//select supply from VBAT.
	}

	/*
	 *   ana_02<2:0>          000     001     010     011     100     101     110     111
	 *   ana_0b<3> = 0b'0     1.00V   1.05V   1.10V   1.15V   1.20V   1.25V   1.30V   1.35V
	 *   ana_0b<3> = 0b'1     0.574V  0.601V  0.632V  0.659V  0.687V  0.717V  0.743V  0.774V
	 */
	//confirm with wenfeng(20201126).
	//modify 0.6V to 1.0V.(changed by yi.bao at 20210324,confirm by wenfeng and guangjun).0.6V may crash in high temp.
	analog_write(0x0b, 0x04);   //poweron_dft: 0x44   <3> en_vo_op6, retention LDO 0.6V mode output disable
													//<6> enable_b signal of 1M pullup resistor for mscn PAD, 1: disable 0: enable
	analog_write(0x02, 0x20);  	//poweron_dft: 0x24,  <2:0> ret_ldo_trim,  set 0x00: 1.0V


	if(xtal == EXTERNAL_XTAL_48M){		//use external 48M crystal instead of external 24M crystal
		analog_write(0x2d, analog_read(0x2d) | BIT(6));  	//poweron_dft: 0x15, <6>: 0-24M XTAL , 1- 48M XTAL
	}

#if	PM_OTP_SET_DIF_VERSION
	//if code run in flash,otp need to enter deep to save current. only need in A0.
	if(0 == (read_reg8(0x602) & BIT(4)))
	{
		write_reg8(0x10+0x00, read_reg8(0x10) & ~BIT(1));              //pce=0
		write_reg8(0x10+0x01, read_reg8(0x11) & ~(BIT_RNG(4,5)));      //pdstb=0
	}
#endif
	/**
	 * 1.whether the program is burned to otp or flash, when using the read interface to operate otp, it needs to wait for a ready signal,
	 *   so it needs to set the tcs time according to the system clock. since 24rc is not accurate, the time set here is a bit larger, about 15.75us;
	 * 2.If pce_auto_mode is enabled, there must be a relationship: tcs> timeout,the timeout_config defaults to 0;
	 */
	reg_otp_offsetl =(reg_otp_offsetl &(~FLD_OTP_PCE_TCS_SEL))|((0x05)<<5);
	if(read_reg8(0x602) & BIT(4))
	{
		analog_write(0x06, analog_read(0x06) | BIT(1));	//<1>:Power down of Flash LDO, 1: Power down  0: Power up
		otp_program_flag = 1;
		//if the pce be pulled all the time,it will consumes power,to save power consumption,
		//when fetch instructions from the otp,the hardware can automatically pull up the pce,when mcu is not in use,Pull it down.
#if OTP_PCE_AUTO_MODE_EN
		otp_auto_pce_enable();
		//to save power consumption, pull the pce down. when the otp is loaded, the hardware pulls the pce up by using the pce_auto_mode function
		reg_otp_ctrl0 &= ~(FLD_OTP_PCE);//pce=0

#endif
	}


	analog_write(0x27, 0x00);   //poweron_dft: 0x00 PA gpio wakeup disable
	analog_write(0x28, 0x00);   //poweron_dft: 0x00 PB gpio wakeup disable
	analog_write(0x29, 0x00);   //poweron_dft: 0x00	PC gpio wakeup disable
	analog_write(0x2a, 0x00);   //poweron_dft: 0x00 PD gpio wakeup disable

	write_reg32(0xc40, 0x04040404); //poweron_dft: 0x00000000 -> 0x04040404
	write_reg32(0xc44, 0x04040404); //poweron_dft: 0x00000000 -> 0x04040404
	write_reg8(0xc48, 0x04);        //poweron_dft: 0x00 -> 0x04

	pm_update_status_info();

	if(MCU_STATUS_DEEPRET_BACK == pmParam.mcu_status){
		unsigned int deepRet_32k_tick = pm_get_32k_tick();

		unsigned int deepRet_tick = pm_tim_recover(deepRet_32k_tick);
		reg_system_tick = deepRet_tick;

		reg_system_ctrl |= FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN;
	}
	else
	{
		reg_system_ctrl |= FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN;

		cpu_wakeup_no_deepretn_back_init(); // to save ramcode
	}


	reg_system_irq_mask |= BIT(2);   //enable system timer irq
	//core_c20/c21 power on default all enable, so we disable them first,  then if use, enable them by manual
	//note that: PWM/RF Tx/RF Rx/AES code/AES dcode dma may be affected by this, you must handle them when initialization
	reg_dma_chn_en = 0;
	reg_dma_chn_irq_msk = 0;
	reg_gpio_wakeup_irq |= (FLD_GPIO_CORE_WAKEUP_EN | FLD_GPIO_CORE_INTERRUPT_EN);

	//wait for XTAL ready
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana

}

/**
 * @brief 	  This function performs to configure whether to calibrate the 32k rc in the cpu_wakeup_init() when power-on or wakeup from deep sleep mode.If wakeup from deep retention sleep mode will not calibrate.
 * @param[in] calib_flag - Choose whether to calibrate the 32k rc or not.
 * 						1 - calibrate; 0 - not calibrate
 * @return	  none
 * @note	  This function will not take effect until it is called before cpu_wakeup_init(). 
 */
void cpu_wakeup_init_calib_32k_rc_cfg(char calib_flag)
{
	g_32k_rc_calib_flag = calib_flag;
}

/**
 * @brief      This function serves to switch dcdc.
 * @param[in]  dcdc_output type
 * @return     none
 */
void pm_set_dcdc_output(DcdcOutput_TypeDef volt)
{
	// normal read and write, VDD and VDD2 voltage chips can be met.
	// OTP marginal read, VDD2 requires power supply of 3.3V,DCDC_OUTPUT needs to be switched to 3.0V.
	// Switching time:2.5V->3.0V, about 40uS, in the code, set the time to 200uS;
	//3.0V->2.5V, about 1ms, 3.0V to 2.5V, no delay time added,in this voltage range, OTP Normal Read/Write can work.
	if(volt == DCDC_2P5V)
	{
		analog_write(0x08,(analog_read(0x08)&0x9f)|0x20);//switch dcdc to 2.5V
	}
	else if(volt ==DCDC_3P0V)
	{
		analog_write(0x08,(analog_read(0x08)&0x9f)|0x40);//switch dcdc to 3.0V
		sleep_us(200);
	}
}

/**
 * @brief		This function serves to set flash voltage vdd_f.TO ensure the vdd_f is enough to supply the flash,need to calibration the vdd_f.
 * @param[in]	voltage - the vdd_f need to set.The voltage is range from 0 to 0x07.
 * @return		none.
 */
void pm_set_vdd_f(Flash_VoltageDef voltage)
{
	analog_write(0x0a, ((analog_read(0x0a) & 0xf8)  | (voltage & 0x07) ));
}

/**
 * @brief		This function serves to set the voltage of DCDC 2V5.DCDC 2V5 need to calibration to about 2.5V.
 * @param[in]	vol - the dcdc 2p5 need to set.the vol is range from 0 to 0x07.
 * @return		none.
 */
void pm_set_dcdc_2p5(unsigned char vol)
{
	analog_write(0x09, ((analog_read(0x09) & 0x8f)  | ( (vol & 0x07) << 4) ));
}

/**
 * @brief		This function serves to set the power sel of 1p2,include RF_LDO, analog_ldo, digital_ldo and SRAM_ldo.
 * @param[in]	power_sel - 0:DCDC_OUTPUT_SEL, 1:VBAT_SEL.
 * @return		none.
 * @note		When the VBAT voltage > 1.2V,we can use the VBAT to supply the power for 1p2,in this mode,the rf power will
 * 				decrease as the VBAT voltage decrease.
 * 				When the VBAT voltage < 1.2V,we can use the DCDC mode to supply the power for 1p2,but the rf tx current will increase.
 *
 *				Application need to detect the battery voltage,if VBAT is bigger than 1.2V, you can use all two mode,but:
 *					Use DCDC mode:tx current will increase than VBAT mode.
 *					Use VBAT mode:the tx power will decrease with the VBAT voltage.
 *				if the VBAT voltage is less than 1.2V,must use DCDC mode,otherwise the chip function will abnormal.
 *				if call this API to change the 1p2 power mode,you should to re_call the rf_set_power_level_index() to set the rf power.
 */
void pm_set_1p2_power_sel(PowerSel_TypeDef power_sel)
{
	if(DCDC_OUTPUT_SEL == power_sel)
	{
		analog_write(0x07, (analog_read(0x07) & 0x3f) | 0x80);
		pm_1p2_power_sel_vbat = 0;
	}
	else if(VBAT_SEL == power_sel)
	{
		analog_write(0x07, (analog_read(0x07) & 0x3f) | 0x40);
		pm_1p2_power_sel_vbat = 1;
	}
}

#if PM_LONG_SLEEP_WAKEUP_EN
//definitions for U64 operations
typedef struct {            /*The result of the multiplication with 64 bits.*/
    unsigned int High32;    /*The high 32 bits.*/
    unsigned int Low32;     /*The low 32 bits.*/
} U64_VALUE_t;

/** Get the high word and low word of a variable with 32 bits.*/
#define LOWORD(p)           ((p) & 0x0000FFFFU)
#define HIWORD(p)           (((p) >> 16) & 0x0000FFFFU)

#define MAXVALUEOF32BITS    (0xFFFFFFFFU)

/**
* @brief Calculate the multiplication between two factors,
*        and one factor is the high 32 bit of one 64 bit unsigned integer and the other factor is 32 bit unsigned integer.
* @param Factor64H           [IN]The high 32 bit of one 64 bit unsigned integer.
*                               - Type: unsigned int  \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Factor32            [IN]A 32 bit unsigned integer.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Result          [OUT]A pointer to the production between the two factors.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the multiplication.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_Cal64Multi32_H(
    const unsigned int Factor64H,                /*The high 32 bit of one 64 bit unsigned integer.*/
    const unsigned int Factor32,               /*A 32 bit unsigned integer.*/
    U64_VALUE_t * const Result)       /*A pointer to the production between the two factors.*/

{
    unsigned char IsOverFlow = 0;   /*The overflow state of the product.*/
    if (0 != Result) {    /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;

        if ((0U == Factor64H) || (0U == Factor32)) {/*Check whether one of the multiplier factor is zero.*/
            /*Set the product to zero when one of the multiplier factor is zero.*/
            Result->High32 = 0U;
            Result->Low32 = 0U;
        }
        else {
            if ((Factor64H * Factor32) / Factor32 == Factor64H) {/*Check whether the product is not overflow.*/
                /*Calculate the product when the product is not overflow*/
                Result->High32 = 0U;
                Result->Low32 = Factor64H * Factor32;
            }
            else {
                /*Set the overflow flag of the product and print the overflow message for debug.*/
                IsOverFlow = 1;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the multiplication between two factors,
*        and one factor is the low 32 bit of one 64 bit unsigned integer and the other factor is 32 bit unsigned integer.
* @param Factor64L             [IN]The low 32 bit of one 64 bit unsigned integer
*                               - Type: unsigned int  \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Factor32               [IN]A 32 bit unsigned integer.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Result          [OUT]A pointer to the production between the two factors.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return none
*/
__attribute__((section(".ram_code"))) static void TN_Cal64Multi32_L(
    const unsigned int Factor64L,            /*The low 32 bit of one 64 bit unsigned integer.*/
    const unsigned int Factor32,              /*An 32 bit unsigned integer.*/
    U64_VALUE_t * const Result)   /*A pointer to the production between the two factors.*/

    {
    unsigned int  A0    = 0U;   /*The low 16 bits of the multiplier factor with 64 bits.*/
    unsigned int  A1    = 0U;   /*The 16~31 bits of the multiplier factor with 64 bits.*/
    unsigned int  B0    = 0U;   /*The low 16 bits of the multiplier factor with 32 bits.*/
    unsigned int  B1    = 0U;   /*The 16~31 bits of the multiplier factor with 32 bits.*/
    unsigned int  A0B0  = 0U;   /*The product between A0 and B0.*/
    unsigned int  A1B0  = 0U;   /*The product between A1 and B0.*/
    unsigned int  A0B1  = 0U;   /*The product between A0 and B1.*/
    unsigned int  A1B1  = 0U;   /*The product between A1 and B1.*/
    if (0 != Result)   /*Check the output buffer.*/
    {
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if((0U == Factor64L) || (0U == Factor32)) /*Check whether one of the multiplier factor with low 32 bits is zero.*/
        {
            /*Set the product to zero when one of the multiplier factor with 32 bits is zero.*/
            Result->High32 = 0U;
            Result->Low32 = 0U;
        }
        else
        {
            if((Factor32 * Factor64L )/Factor32   ==   Factor64L )/*Check whether the product between two multiplier factor with 32 bits is not oveeflow.*/
            {
                /*Calculate the product when the product is not overflow.*/
                Result->High32 = 0U;
                Result->Low32 = Factor64L  * Factor32;
            }
            else
            {
                /*Calculate the product when the product between two multiplier factor with 32 bits is not overflow.*/
                A0 = LOWORD( Factor64L );    /*Get the low 16 bits of the multiplier factor with 64 bits.*/
                A1 = HIWORD( Factor64L );    /*Get the 16~31 bits of the multiplier factor with 64 bits.*/
                B0 = LOWORD( Factor32 );       /*Get the low 16 bits of the multiplier factor with 32 bits.*/
                B1 = HIWORD( Factor32 );       /*Get the 16~31 bits of the multiplier factor with 32 bits.*/
                /*Calculate the product high 32 bit.*/
                /* Factor64L * Factor32 = (A0+A1*2^16)*(B0+B1*2^16) = A0*B0 + (A0*B1+A1*B0)*2^16 + A1*B1*2^32 */
                A0B0 = A0 * B0;
                A1B0 = A1 * B0 + HIWORD( A0B0 );
                A0B1 = A0 * B1 + LOWORD( A1B0 );
                A1B1 = A1 * B1 + HIWORD( A1B0 ) + HIWORD( A0B1 );
                /*Calculate the product low 32 bit.*/
                Result->High32 = A1B1;
                Result->Low32 = Factor64L * Factor32;
            }
        }
    }

}

/**
* @brief   Calculate the addition between the two 64 bit factors.
* @param Factor32             [IN]A pointer to one factor of the addition.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Factor32             [IN]A pointer to another factor of the addition.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param Result              [OUT]A pointer to the result of the addition.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the addition.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_Cal64Add(
    const U64_VALUE_t * const Factor1,    /*One factor of the addition.*/
    const U64_VALUE_t * const Factor2,    /*Another factor of the addition.*/
    U64_VALUE_t * const Result)           /*The result of the addition.*/

{
    unsigned char IsOverFlow = 0;   /*The overflow state of the product.*/
    if (0 != Result) {  /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if ((0 != Factor1) && (0 != Factor2)) {  /*Check the input buffer.*/
            if( (Factor1->Low32) > ((unsigned int)(0xFFFFFFFFu) - (Factor2->High32)) ) { /*Check whether the high 32 bits of the product is overflow.*/
                /*Set the overflow flag of the product and print the overflow message for debug.*/
                IsOverFlow = 1;
            }
            else {
                /*Calculate the final product and reset the overflow state.*/
                Result->High32 = Factor2->High32 + Factor1->Low32;
                Result->Low32 = Factor2->Low32;
                IsOverFlow = 0;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the multiplication between two factors, and one factor is 64 bit unsigned integer while the other factor 32 bit.
* @param U64Factor           [IN]One of the multiplier factor with 64 bits.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param U32Factor           [IN]One of the multiplier factor with 32 bits.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF]\n
* @param Result              [OUT]A pointer to the multiplication result with 64 bits.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the multiplication.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_UINT64Multiply(
    const U64_VALUE_t * const U64Factor,      /*One of the multiplier factor with 64 bits.*/
    const unsigned int U32Factor,             /*One of the multiplier factor with 32 bits.*/
    U64_VALUE_t * const Result)               /*The multiplication result with 64 bits.*/

{
    unsigned char IsOverFlow = 0;       /*The overflow state of the U64Factor.*/
    U64_VALUE_t Product64H_32 = {0};    /*The U64Factor between the high 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
    U64_VALUE_t Product64L_32 = {0};    /*The U64Factor between the low 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
    /*Initiate the output buffer.*/
    if (0 != Result) { /*Check the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if (0 != U64Factor) {  /*Check the input buffer.*/
            if ((0U == U32Factor)||((0U == U64Factor->High32) && (0U == U64Factor->Low32))) { /*Check whether one of the multiplier factor is zero.*/
                /*The result is zero when one of the multiplier factor is zero.*/
                Result->High32 = 0U;
                Result->Low32 = 0U;
                IsOverFlow = 0;
            }
            else {
                /*Calculate the multiplication between the high 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
                IsOverFlow = TN_Cal64Multi32_H(U64Factor->High32,U32Factor,&Product64H_32);
                if (0 == IsOverFlow) {/*Check whether the multiplication between the high 32 bits of the 64 bits multiplier factor and the 32 bits multiplier factor is not overflow.*/
                    /*Calculate multiplication between the low 32 bits of the two multiplier factors.*/
                    TN_Cal64Multi32_L(U64Factor->Low32,U32Factor,&Product64L_32);
                    /*Calculate the addition between the two multiplication results calculating by the upper two functions.*/
                    IsOverFlow = TN_Cal64Add(&Product64H_32,&Product64L_32,Result);
                }
            }
            /*The U64Factor is zero when it is overflow.*/
            if(1 == IsOverFlow) {
                Result->High32 = 0U;
                Result->Low32 = 0U;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the remainder with 64 bits.
* @param MulRes             [IN]The multiplication result.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Dividend           [IN]The dividend.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
*
* @return The remainder
*                               - Type: U64_VALUE_t \n
*                               - Range: N/A.\n
*
*/
__attribute__((section(".ram_code"))) static U64_VALUE_t TN_CalRemaider(const U64_VALUE_t MulRes,const U64_VALUE_t Dividend)
{
    U64_VALUE_t Remaider = {0};                  /*The division remainder with 64 bit.*/
    if( MulRes.Low32 <= Dividend.Low32)/*Do not need to carry calculation*/
    {
        Remaider.Low32 = Dividend.Low32 - MulRes.Low32;
        Remaider.High32 = Dividend.High32 - MulRes.High32;
    }
    else    /*Need to carry calculation*/
    {
        Remaider.Low32 = MAXVALUEOF32BITS - MulRes.Low32 + 1U + Dividend.Low32;
        Remaider.High32 = Dividend.High32 - MulRes.High32 - 1U;
    }
    return Remaider;
}

/**
* @brief Two numbers with 64 bit are divided by 2 respectively until the high 32 bit of the second number is zero.
* @param Number1             [IN]A pointer to the first number with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Number2             [IN]A pointer to the second number with 64 bit.
*                               - Type: unsigned char *\n
*                               - Range: N/A.\n
* @param Number1Div          [OUT]A pointer to the first divided number which has been divided by 2.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Number2Div          [OUT]A pointer to the second number which has been divided by 2.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return none
*
*
*/
__attribute__((section(".ram_code"))) static void TN_Div2NumBy2(
    const U64_VALUE_t * const Number1,    /*A pointer to the first number with 64 bit.*/
    const U64_VALUE_t * const Number2,    /*A pointer to the second number with 64 bit.*/
    U64_VALUE_t * const Number1Div,       /*A pointer to the first number which has been divided by 2.*/
    U64_VALUE_t * const Number2Div)       /*A pointer to the second number which has been divided by 2.*/
{
    if ((0 != Number1Div) && (0 != Number2Div) && (0 != Number1) && (0 != Number2)) {/*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Number1Div->Low32 = Number1->Low32;
        Number1Div->High32 = Number1->High32;
        Number2Div->Low32 = Number2->Low32;
        Number2Div->High32 = Number2->High32;
        while (0U != Number2Div->High32) {
                /*The high 32 bit and low 32 bit of the first number are divided by 2.*/
                Number1Div->Low32 >>= 1U;
                if ((Number1Div->High32 % 2U) > 0U) {
                    Number1Div->Low32 += 0x80000000U;
                }
                Number1Div->High32 >>= 1U;
                /*The high 32 bit and low 32 bit of the second number are divided by 2.*/
                Number2Div->Low32 >>= 1U;
                if((Number2Div->High32 %2U) > 0U) {
                    Number2Div->Low32 += 0x80000000U;
                }
                Number2Div->High32 >>= 1U;
        }
    }
}

/**
* @brief Calculate the quotient between the 64 bit dividend and 32 bit divisor. The value of the dividend and divisor are treated as unsigned integer.
*        The division used in this function is the standard algorithm used for pen-and-paper division of multidigit numbers expressed in decimal notation.
* @param U64Dividend         [IN]The dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param U32Divisor          [IN]The divisor with 32 bit.
*                               - Type: unsigned int \n
*                               - Range: N/A.\n
*
* @return none
*
*/
__attribute__((section(".ram_code"))) static unsigned int TN_CalQuto(
    const U64_VALUE_t * const U64Dividend,    /*The dividend with 64 bit.*/
    const unsigned int U32Divisor)                 /*The divisor with 32 bit.*/
{
    unsigned int i = 0U; /*The index for the FOR loop.*/
    unsigned int l_uFlag=0U;  /*The most significant bit of the dividend which has been shifted.*/
    U64_VALUE_t l_DivdShift={0};   /*The dividend which has been shifted.*/
    unsigned int l_Quto = 0U;         /*The quotient.*/
    if (0 != U64Dividend) {/*Check the input buffer.*/
        l_DivdShift.High32 = U64Dividend->High32;
        l_DivdShift.Low32 = U64Dividend->Low32;
        for (i = 0U; i < 32U; i++) {
            l_uFlag = (unsigned int)l_DivdShift.High32 >> 31U;    /*The 'uFlag' is 0xffffffff when the most significant bit of the 'l_DivdShift.High32' is one, otherwise 0x00000000.*/
            /*Left-shift dividend by 1 bit.*/
            l_DivdShift.High32 = (l_DivdShift.High32 << 1)|(l_DivdShift.Low32 >> 31);
            l_DivdShift.Low32 <<= 1;
            /*Left-shift quotient by 1 bit.*/
            l_Quto = l_Quto<<1;
            if((l_DivdShift.High32 >= U32Divisor) || (l_uFlag >0U)) { /*Check whether the dividend high 32 bit is greater than the divisor low 32 bit after left shifting.*/
                /*Calculate the quotient and remainder between the dividend high 32 bit and divisor low 32 bit.*/
                l_DivdShift.High32 -= U32Divisor;
                l_Quto++;
            }
        }
    }
    return l_Quto;
}

/**
* @brief Check the validation of the function input and output.
* @param i_pDividend            [IN]A pointer to the dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param i_pDivisor             [IN]A pointer to the divisor with 64 bit.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param o_pQuto                [OUT]A pointer to the division quotient with 32 bit.
*                               - Type: unsigned int  *\n
*                               - Range: N/A.\n
* @param ErrorCode           [OUT]A pointer to the division error code.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
* @param DivStatus           [OUT]A pointer to the division successful status.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
*
* @return none
*
*/
__attribute__((section(".ram_code"))) static void TN_CheckUINT64AccurateDiv(
    const U64_VALUE_t * const i_pDividend,               /*A pointer to the dividend with 64 bit.*/
    const U64_VALUE_t * const i_pDivisor,                /*A pointer to the divisor with 64 bit.*/
    unsigned int * const o_pQuto,                             /*A pointer to the division quotient with 32 bit.*/
    unsigned char * const ErrorCode,    /*A pointer to the division error code.*/
    unsigned char * const o_pDivStatus)                     /*A pointer to the division successful status.*/
{
    if ((0 != o_pQuto) && (0 != ErrorCode) && (0 != o_pDivStatus)) { /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        *ErrorCode = 0;
        *o_pQuto = 0U;
        *o_pDivStatus = 1;
        if ((0 == i_pDividend) || (0 == i_pDivisor)) { /*Check the input buffer.*/
            *ErrorCode = 0xff;
            *o_pQuto = 0U;
            *o_pDivStatus = 0;
        }
    }
}

/**
* @brief Calculate the 64 bit accurate division, and the quotient is rounded.
* @param i_pDividend            [IN]A pointer to the dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param i_pDivisor             [IN]A pointer to the divisor with 64 bit.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param o_pQuto                [OUT]A pointer to the division quotient with 32 bit.
*                               - Type: unsigned int  *\n
*                               - Range: N/A.\n
* @param ErrorCode           [OUT]A pointer to the division error code.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
* @param o_pDivStatus           [OUT]A pointer to the division successful status.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
*
* @return N/A
*
*/
__attribute__((section(".ram_code"))) static void TN_U64AccurateDiv(
    const U64_VALUE_t * const Dividend,               /*A pointer to the dividend with 64 bit.*/
    const U64_VALUE_t * const Divisor,                /*A pointer to the divisor with 64 bit.*/
    unsigned int * const Quto,
    U64_VALUE_t * const Remainder,                            /*A pointer to the division quotient with 32 bit.*/
    unsigned char * const ErrorCode,    /*A pointer to the division error code.*/
    unsigned char * const DivStatus)                      /*A pointer to the division successful status.*/
{
    U64_VALUE_t l_DividendDiv2 = {0};        /*The temporary buffer.*/
    U64_VALUE_t l_DivisorDiv2 = {0};         /*The temporary buffer.*/
    unsigned int l_QutoTemp = 0U;             /*The quotient has not been rounded.*/
    TN_CheckUINT64AccurateDiv(Dividend, Divisor, Quto, ErrorCode, DivStatus);
    if (0 == *ErrorCode) {
        if ((0U == Divisor->High32) && (0U == Divisor->Low32)) {/*Check whether the divisor is zero.*/
            /*The division result is zero and division status is not successful when the divisor is zero.*/
            *Quto = 0;
            *ErrorCode  = 1;
            *DivStatus =  0;
        }
        else {
            if ((0U == Divisor->High32) && (Dividend->High32 >= Divisor->Low32)) {/*Check whether the dividend high 32 bit is greater than the divisor low 32 bit.*/
                /*The division result is overflow when the dividend high 32 bit is greater than the divisor low 32 bit.*/
                *Quto = 0;
                *ErrorCode = 2;
                *DivStatus =  0;
            }
            else {
                if ((0U == Divisor->High32) && (0U == Dividend->High32)) { /*Check whether the high 32 bit of the divisor and dividend is zero.*/
                    /*Get the division result directly the high 32 bit of the divisor and dividend is zero.*/
                    l_QutoTemp = Dividend->Low32 / Divisor->Low32;
                }
                else {
                    /*The dividend and divisor are divided by 2 respectively until the high 32 bit of the divisor is zero.*/
                    TN_Div2NumBy2(Dividend,Divisor,& l_DividendDiv2,& l_DivisorDiv2);
                    /*Calculate the quotient and remainder of the 64 bit division by subtracting with borrow.*/
                    l_QutoTemp = TN_CalQuto(&l_DividendDiv2,l_DivisorDiv2.Low32);
                }

                U64_VALUE_t U64MulRes = {0};
                TN_UINT64Multiply(Divisor, l_QutoTemp, &U64MulRes);/*Calculate the multiplication result between the 'Divisor' and 'QutoNotRound'.*/
                /*Calculate the remainder.*/
                *Remainder = TN_CalRemaider(U64MulRes, *Dividend);
                *Quto = l_QutoTemp;
            }
        }
    }
}




/**
 * @brief      This function serves to set the working mode of MCU,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  SleepDurationUs - the time of long sleep, which means MCU can sleep for up to one hour.
 * @return     indicate whether the cpu is wake up successful.
 */
_attribute_ram_code_sec_noinline_ int pm_long_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int SleepDurationUs)
{
	unsigned int   t0 = clock_time();
	unsigned int   tl_tick_cur;
	unsigned int   tl_tick_32k_cur;
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);
#if(RAM_CRC_EN)
	unsigned char ram_crc_en = 0;
	unsigned short ram_crc_tick = 0;
#endif
	//NOTICE:We think that the external 32k crystal clock is very accurate, does not need to read 2 bytes through reg_750
	//the conversion offset is less than 64ppm
#if (!CLK_32K_XTAL_EN)
	unsigned short  tl_tick_32k_calib = REG_ADDR16(0x754);
	unsigned short  tick_32k_halfCalib = tl_tick_32k_calib>>1;
#endif

	unsigned int span = SleepDurationUs;


	if(timer_wakeup_enable){
		if (span < EMPTYRUN_TIME_US) 		// 0 us base
		{
			analog_write (0x44, 0x1d);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x1d;
			} while ( ((unsigned int)clock_time () - t0 < (span * sys_tick_per_us )) && !st);
			return st;
		}
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

	////////// disable IRQ //////////////////////////////////////////
	unsigned char r = irq_disable ();

	//set 32k wakeup tick
	U64_VALUE_t TmpVal[4] = {
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0}
	};
	unsigned char ErrorCode = 0;
	unsigned char DivStatus = 0;
	//1. calculate the absolute System wakeup tick
	TmpVal[0].Low32 = SleepDurationUs;
	TN_UINT64Multiply(&TmpVal[0], sys_tick_per_us, &TmpVal[1]);
	TmpVal[2].Low32 = t0;
	TN_Cal64Add(&TmpVal[1], &TmpVal[2], &TmpVal[3]);
	unsigned int wakeup_tick = TmpVal[3].Low32;
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TmpVal[1].Low32 = 0;
	TmpVal[1].High32 = 0;
	TmpVal[2].Low32 = 0;
	TmpVal[2].High32 = 0;
	TmpVal[3].Low32 = 0;
	TmpVal[3].High32 = 0;

	//2. calculate and set the 32K timer wakeup tick
#if(!CLK_32K_XTAL_EN)
	TmpVal[2].Low32 = tl_tick_32k_calib;
#else
	TmpVal[2].Low32 = TICK_16M_PER_32CYC_XTAL32P768K;
#endif
	tl_tick_cur = clock_time ();
	reg_system_irq_mask &= (~BIT(2));   						//disable system timer irq
	reg_system_ctrl &= ~(FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN);
	tl_tick_32k_cur = pm_get_32k_tick ();
    unsigned int M = 0;
    unsigned int m = 0;
	unsigned int EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_SUSPEND;
	if(sleep_mode != SUSPEND_MODE){
		EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_DEEP_RET;
	}
	TmpVal[0].Low32 = SleepDurationUs - (((unsigned int)(tl_tick_cur - t0)) / sys_tick_per_us) - EARLYWAKEUP_TIME_US;
	TN_UINT64Multiply(&TmpVal[0], sys_tick_per_us, &TmpVal[1]);
	TN_U64AccurateDiv(&TmpVal[1], &TmpVal[2], &M, &TmpVal[3], &ErrorCode, &DivStatus);
#if(!CLK_32K_XTAL_EN)
	m = M*16 + (TmpVal[3].Low32)/(tl_tick_32k_calib/16);
#else
	m = M*32 + (TmpVal[3].Low32)/(TICK_16M_PER_32CYC_XTAL32P768K/32);
#endif


	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //@@@@@@@ [3] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter
	write_reg8(0x6e, 0x00);

	analog_write (0x44, 0x1f);				//clear all flag
	analog_write(0x2c, 0x5f);    //poweron_dft: 0x5f  <6> debounce reset

	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

	unsigned char bit67_ana06;

	if(sleep_mode & 0x7f) { //deepsleep with retention
		bit67_ana06 = 0x40;  //ana_2c<7:5>: 010
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
	unsigned char shutdown = sleep_mode + 1;
	analog_write(0x2b, (sleep_mode ? ((sleep_mode == SHUTDOWN_MODE)?0xff:0xfe) : 0x4c)|((shutdown<sleep_mode)? 1:0));// turn off soft start switch
#if (!CLK_32K_XTAL_EN)
		analog_write(0x2c, ((wakeup_src & PM_WAKEUP_DEBOUNCE)?0x1e:0x5e) | (!timer_wakeup_enable));
#else
		analog_write(0x2c, ((wakeup_src & PM_WAKEUP_DEBOUNCE)?0x1c:0x5c) | (!timer_wakeup_enable)<<1);
#endif

	if(SHUTDOWN_MODE == sleep_mode)
	{
		analog_write(0x07, ((analog_read(0x07) & 0xc7) | 0x10));         //vbat lcldo_trim down 100(1.0v) -->  010(0.75v)
	}
	analog_write(0x06, (analog_read(0x06) & 0x3f) | bit67_ana06);

	if(sleep_mode & 0x7f ) {
#if(RAM_CRC_EN)
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
		if(g_chip_version == 0xff)
		{
			analog_write(0x7f, 0x02);	//BIT<1> ram_crc_en
		}								//BIT<0> deep_ret not run boot_rom code while wakeup in A0 version
		else
		{
			analog_write(0x7f, 0x03);	//BIT<1> ram_crc_en
										//BIT<0> boot_rom code disable will cause the otp_cmd invalid,so need to enable
										//boot_rom code and run a part of boot_rom code to ensure otp cmd valid in A1 version
		}
		m -= ram_crc_tick;
#else
		if(g_chip_version == 0xff)
		{
			analog_write(0x7f, 0x00);		//BIT<0> deep_ret not run boot_rom code while wakeup in A0 version
		}
		else
		{
			write_reg8(0x602, (read_reg8(0x602) | BIT(3)));
			analog_write(0x7f, 0x01);	//BIT<0> boot_rom code disable will cause the otp_cmd invalid,so need to enable
										//boot_rom code and run a part of boot_rom code to ensure otp cmd valid in A1 version
		}
#endif
	}
	else{
		write_reg8(0x602, (read_reg8(0x602) | BIT(3)));      //~ana_7f<0> || core_602<3> = ram_code_en
		analog_write(0x7f, 0x01);
	}

	//TODO:need change the xtal delay and dcdc delay after high-low temp test.
	//set DCDC delay and XTAL dealy durationadd xtal delay to cover the VDDDEC1V0 on to decrease the peak current when wakeup,
	//when ana_1f = 0x40, ana_20 = 0xa0, peak current about 50mA,ana_1f = ana_20 = 0xa0,peak current is about 20mA.
	//(0xff - n):  if timer wake up : ((n+1)*2) 32k cycle, use 32K/2 clock to count.
	//             else pad wake up: (((n+1)*2-1) ~ (n+1)*2 )32k cycle,pad wakeup has one cycle error at most,because the
	//             pad wakeup time is uncertain.
	//caution: actual XTAL delay time need <= DCDC delay,so the value of ana_1f <= the value of ana_20.

	unsigned int dcdc_dly = (SUSPEND_MODE == sleep_mode)?PM_DCDC_DELAY_DURATION_SUSPEND:PM_DCDC_DELAY_DURATION;
	unsigned int xtal_dly = (SUSPEND_MODE == sleep_mode)?PM_XTAL_DELAY_DURATION_SUSPEND:PM_XTAL_DELAY_DURATION;
#if(!CLK_32K_XTAL_EN)
	//set xtal delay duration
	span = (xtal_dly * (sys_tick_per_us>>1) * 16 + tick_32k_halfCalib)/ tick_32k_calib;
	unsigned char rst_cycle = 0xff - span;
	analog_write (0x20, rst_cycle);

	//set DCDC delay duration
	span = (dcdc_dly * (sys_tick_per_us>>1) * 16 + tick_32k_halfCalib)/ tick_32k_calib;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#else
	//set xtal delay duration
	span = (xtal_dly * (sys_tick_per_us>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
	unsigned char rst_cycle = 0xff - span;
	analog_write (0x20, rst_cycle);

	//set DCDC delay duration
	span = (dcdc_dly * (sys_tick_per_us>>1) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1))/ CRYSTAL32768_TICK_PER_32CYCLE;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#endif
	unsigned int tick_reset;
	if(pm_long_suspend){
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - tick_cur)/ tick_32k_calib * 16;
	}
	else{
		tick_reset = tick_32k_cur + ((unsigned int)(tick_wakeup_reset - tick_cur) * 16 + tick_32k_halfCalib) / tick_32k_calib;
	}
	cpu_set_32k_tick(tl_tick_32k_cur + m);

	write_reg8(0x74c, 0x20);
	if(analog_read(0x44)&0x1f){

	}
	else{
		sleep_start();
	}

	/* long press pad to wake up from deep */  //reboot can not use at A0
	if(sleep_mode == DEEPSLEEP_MODE){ //
	   write_reg8 (0x6f, 0x20);  //reboot
	}

	else if(sleep_mode&0x7f)
	{
		write_reg8 (0x602, 0x88);  //mcu run from the beginning
	}

  //recover system clock from 32k clock
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TmpVal[1].Low32 = 0;
	TmpVal[1].High32 = 0;
	TmpVal[2].Low32 = 0;
	TmpVal[2].High32 = 0;
	TmpVal[3].Low32 = 0;
	TmpVal[3].High32 = 0;
	span = (unsigned int)(pm_get_32k_tick () - tl_tick_32k_cur);
#if(!CLK_32K_XTAL_EN)
	M = span / 16;
	m = span % 16;
	TmpVal[0].Low32 = M;
	TN_UINT64Multiply(&TmpVal[0], tl_tick_32k_calib, &TmpVal[1]);
	TmpVal[0].Low32 = m*tl_tick_32k_calib/16;
#else
	M = span / 32;
	m = span % 32;
	TmpVal[0].Low32 = M;
	TN_UINT64Multiply(&TmpVal[0], TICK_16M_PER_32CYC_XTAL32P768K, &TmpVal[1]);
	TmpVal[0].Low32 = m*TICK_16M_PER_32CYC_XTAL32P768K/32;
#endif
	TN_Cal64Add(&TmpVal[1], &TmpVal[0], &TmpVal[2]);
	TmpVal[3].Low32 = tl_tick_cur;
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TN_Cal64Add(&TmpVal[2], &TmpVal[3], &TmpVal[0]);
	tl_tick_cur = TmpVal[0].Low32;

	reg_system_tick = tl_tick_cur + 20*sys_tick_per_us;  //20us compensation
	reg_system_ctrl |= FLD_SYSTEM_32K_CAL_EN|FLD_SYSTEM_TIMER_MANUAL_EN;

	reg_system_irq_mask |= BIT(2);   //enable system timer irq

	write_reg8 (0x066, reg66);		//restore system clock

	unsigned char anareg44 = analog_read(0x44);
	if(!anareg44){
		anareg44 = STATUS_GPIO_ERR_NO_ENTER_PM;
	}
	else if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}

	irq_restore(r);

	return anareg44;
}
#endif





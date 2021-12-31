/********************************************************************************************************
 * @file	otp.c
 *
 * @brief	This is the source file for b80
 *
 * @author	Driver Group
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "otp.h"
#include "timer.h"
#include "irq.h"
extern unsigned char	otp_program_flag;
unsigned char g_pce_timeout=6;
unsigned char g_irq_en=0;

/**
 * @brief     This function servers to enable pce auto mode,after enable pce auto mode,
 *            If the time from the last instruction fetching to the next instruction fetching exceeds the set timeout time,
 *            the pce will be pulled down. If the pce is pulled down, the hardware will automatically pull up when the instruction fetching,
 *            and clk will not work until the tsc time. This mechanism can save power consumption but reduce efficiency.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_auto_pce_enable(){
	reg_otp_ctrl2 |= FLD_OTP_AUTO_PCE_MODE;
}

/**
 * @brief     This function servers to waiting for pce timeout.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_pce_timeout_exceed(){
	 unsigned long t = clock_time();
	 /*
	 * 1.If the pce auto mode is enabled, there will be a timeout mechanism. when the software manually operates the pce within the timeout time,
	 *   if the pce is manually lowered, timeout will not be invalid, However, if the time exceeds Tsas, the hardware will automatically raise the pce,
	 *   if the software is manually raised, the timeout will become invalid. Therefore, in order to prevent the interaction between software and hardware
	 *   and disrupt the timing sequence, it is necessary to wait for the pce hardware to pull down automatically before performing software processing,add by shuaixing.zhai, confirmed by weixin 20211015
	 * 2.when pce_auto_mode is enabled, the pce hardware will automatically pull down, which will be triggered only after mcu fetches the instruction from otp.
	 *   If the previous function (to pull up the pce) moves to the next function (to wait for the pce hardware to pull down automatically),
	 *   the prefetch instruction (cache:4 word) causes no fetch from otp,then it gets stuck, so add timeout_exceed to handle this exception,
	 *   timeout = 24M/SYS_CLK*((timeout_config(bit[0:3]) + 1)*1.344 us),in here, use the minimum system clock defined timeout time.
	 */
	 while((reg_otp_ctrl0&FLD_OTP_PCE)&&(!clock_time_exceed(t, g_pce_timeout)));
}

/**
 * @brief     This function servers to disable pce auto mode,if disable pce auto mode,
 *            the pce will always be stretched, which increases efficiency but consumes power.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_auto_pce_disable(){
/*
 * Interrupt protection needs to be added because the read and write function interfaces require the use of delay functions and time on the sequence;
 */
	g_irq_en = irq_disable();
	unsigned char pce_flag=0;
#if	ONLY_SUPPORT_OTP_PROGRAM
	if(reg_otp_ctrl2 & FLD_OTP_AUTO_PCE_MODE){
#else
	if((reg_otp_ctrl2 & FLD_OTP_AUTO_PCE_MODE)&&(otp_program_flag==1)){
#endif
		 pce_flag=1;
		 otp_pce_timeout_exceed();
		 reg_otp_ctrl2 &= ~FLD_OTP_AUTO_PCE_MODE;
	}
	return pce_flag;
}

#if OTP_ALL_FUNCTION_EN
/**
 * @brief     This function servers to set otp pce timeout time, the optimal time cannot be set according to the system clock,
 *            because the time between two otp fetch commands cannot be determined,otp fetch commands will only be sent when the cache miss occurs,
 *            but when the miss is not known,so this time is self-determining based on power consumption and efficiency.
 * @param[in] timeout_config -range:0-15,timeout_time = 24M/SYS_CLK*((timeout_config(bit[0:3]) + 1)*1.344 us),timeout_config(bit[0:3]):the default value is 0x01.
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_timeout(unsigned char timeout_config){
	reg_otp_ctrl2 =(reg_otp_ctrl2 &(~FLD_OTP_TIMEOUT_CONFIG))|timeout_config;
	/*
	 * g_pce_timeout = 24M/SYS_CLK*((timeout_config(bit[0:3]) + 1)*1.344 us)(in here, use the minimum system clock(12M) defined timeout time)
	 *               = 24M/12M*((timeout_config(bit[0:3]) + 1)*1.344 us)
	 *               because the decimal point is omitted, add 1 to avoid time being smaller than the hardware time;
	 *               = (timeout_config(bit[0:3]) + 1)*2.688+1
	 */
	g_pce_timeout = (timeout_config+1)*2688/1000+1;

}
#endif



/**
 * @brief      This function serves to enable the otp test area,only for internal testing,there are two operating units, each of which is 16 words,one start at address 0x00,
 *             the other the otp start at address 0x4000, test area address is independent of the real otp address,if use the otp test area,need to use this function.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_en(void){
	 reg_otp_ctrl0 |= FLD_OTP_PTR;
}
/**
 * @brief      This function serves to disable the otp test area,only for internal testing,if do not use the otp test area,use this function,
 *             will transition from test area to operating on real the otp.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_dis(void){
	reg_otp_ctrl0 &= ~(FLD_OTP_PTR);
}

/**
 * @brief      This function serves to wait until the operation of OTP is done.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_ static inline void otp_wait_done(void)
{
	while(reg_otp_dat & FLD_OTP_BUSY);
}

/*
 * @brief     This function is a common sequence used by these interfaces:otp_write32/otp_read_cycle/otp_set_active_mode.
 * @param[in] ptm_mode - ptm type.
 * @return    none
 */
_attribute_ram_code_sec_ static  void  otp_start(OTP_PtmTypeDef ptm_mode)
{
	reg_otp_ctrl0 &= ~(FLD_OTP_PCLK_EN | FLD_OTP_PCE);
	//ptm
	reg_otp_ctrl1 &= (~FLD_OTP_PTM);
	reg_otp_ctrl1 |= ptm_mode;
	//Tms >= 1(ns)
	//pce
	reg_otp_ctrl0 |= (FLD_OTP_PCE);
}


/**
 * @brief      This function serves to write data to OTP memory,4 bytes one time.
 * @param[in]  ptm_mode - write mode.
 * @param[in]  addr  - the address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  data  - the data need to be write,4 bytes.
 * @return     none
 */
_attribute_ram_code_sec_ static void otp_write32(OTP_PtmTypeDef ptm_mode,unsigned int addr, unsigned int data)
{
	//if code run in otp,when switching to ram_code,prog/pwe must be pulled down,pce/pclk may be pulled up.
	//when write to otp, ptm needs to be configured,prog/pwe/pce need to pull down.
	otp_start(ptm_mode);
	//tcsp
	sleep_us(Tcsp);
	//prog pas addr data
	reg_otp_ctrl0 |= FLD_OTP_PPROG ;
	reg_otp_ctrl0 |=FLD_OTP_PAS;
	reg_otp_pa = addr;
	reg_otp_wr_dat = data;
	reg_otp_paio = 0;
	//Tpps
	sleep_us(Tpps);
	//redundancy programming  38*2
	for(unsigned char i = 1; i <= 76; i++){
		reg_otp_ctrl0 |= FLD_OTP_PWE;
		sleep_us(Tpw);
		reg_otp_ctrl0 &= ~(FLD_OTP_PWE);
		if(i<38){
			 reg_otp_paio = i;
		}
		else if(i==38){
			reg_otp_ctrl0&=(~FLD_OTP_PAS);
			reg_otp_pa = addr;
			reg_otp_wr_dat = data;
			reg_otp_paio = 0;
		}
		else if((i>38)&&(i<76))
		{
			reg_otp_paio = i-38;
		}
		else if(i==76)
		{
			break;
		}
		//because the for loop and the if judge the time,choose to use Tpwi/2.
		 sleep_us(Tpwi/2);
	}
	sleep_us(Tpph);
	//prog =0
	reg_otp_ctrl0 &= ~(FLD_OTP_PPROG);
	sleep_us(5);
	//pce=0
	reg_otp_ctrl0 &= ~(FLD_OTP_PCE);
	//Tms >= 1(ns)
	reg_otp_ctrl1 = ~(FLD_OTP_PTM);
}


/**
 * @brief      This function serves to read data from OTP memory.
 * @param[in]  ptm_mode - read mode.
 * @param[in]  addr - the address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_ static void otp_read_cycle(OTP_PtmTypeDef ptm_mode,unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	//if code run in otp,when switching to ram_code,pce/pclk may be pulled up,when read to otp,ptm needs to be configured,prog/pce need to pull down.
	otp_start(ptm_mode);
	sleep_us(Tcs);
	reg_otp_pa = addr;
	reg_otp_ctrl0 |= FLD_OTP_PCLK_EN;
	/*
	 * if this bit is enabled, the address is automatically added,if the bit is not enabled,need to manually add the address,
	 * determine which scenario based on the actual situation.
	 */
	reg_otp_paio |=FLD_OTP_PADR_AUTO;
	reg_otp_rd_dat;
	for(unsigned int i = 0; i < word_len; i++){
		otp_wait_done();
		buff[i] = reg_otp_rd_dat;
	}

      reg_otp_ctrl0 &= ~(FLD_OTP_PCLK_EN | FLD_OTP_PCE);
}

/**
 * @brief      This function serves to otp set deep standby mode,if code run in flash,otp is idle,can enter deep to save current,
 *             if code run in otp,otp_set_deep_standby_mode and otp_set_active_mode are restricted to use in cpu_sleep_wakeup.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_deep_standby_mode(void)
{
	/*
	 * 1.Tash has no maximum limit and does not require interrupt protection;
	 * 2.this function is limited to the cpu_sleep_wakeup function, earlyWakeup_us=90us, and the maximum pce_timeout is about 22us,
	 * so there is no need to wait for a pce timeout;
	 */
	reg_otp_ctrl0 &= ~(FLD_OTP_PCE);//pce=0
	//Tash >10(ns)
	reg_otp_ctrl1 &=~(FLD_OTP_PLDO|FLD_OTP_PDSTD);    //pdstb=0  pldo=0  Tplh >= 0(ns)
}
/**
 * @brief      This function serves to otp set active mode,if otp is in deep mode,need to operate on the otp,set active mode,
 *             if code run in otp,otp_set_deep_standby_mode and otp_set_active_mode are restricted to use in cpu_sleep_wakeup.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_active_mode(void)
{
	/*
	 *Tpls/Tsas has no maximum limit and does not require interrupt protection;
	 */
	reg_otp_ctrl1 |= FLD_OTP_PLDO; //pldo=1
	for(unsigned char i = 0; i < 100; i++){//Tpls:the requirement is greater than 10us, set to 14us according to 48M,if is 24RC,the corresponding time is 29us.
		asm("tnop");
	}
	reg_otp_ctrl1 |= FLD_OTP_PDSTD;//pdstb=1
    for(unsigned char i = 0; i < 35; i++){//Tsas:the requirement is greater than 2us, set to 5us according to 48M,if is 24RC,the corresponding time is 10us.
		asm("tnop");
	}
	reg_otp_ctrl0 |= FLD_OTP_PCE;//pce=1
  /*
   * the following code is for the otp process:
   * first, if otp_clk given by mcu is in working state before active mode(such as suspend), clk_en needs to be closed and reconfigured.
   * second,otp active and correct read timing need to be set separately,and after software is out,given the correct read timing,add by shuaixing.zhai, confirmed by weixin 20211015.
   */
   otp_start(OTP_PTM_READ);
  /*
   * The time here does not need to be compared with the time set by the hardware, the given time only needs to meet the time of the datasheet,
   * Given the correct timing, after software is out, the pce is in the state of lifting, the hardware will not lift the pce, and the Tcs will not be timed,
   * therefore, there is no need to meet a time greater than or equal to the hardware setting,add by shuaixing.zhai, confirmed by weixin 20211015.
   */
   for(unsigned char i = 0; i < 100; i++){//Tcs:the requirement is greater than 10us, set to 14us according to 48M,if is 24RC,the corresponding time is 29us.
	  asm("tnop");
   }
   reg_otp_ctrl0 |= FLD_OTP_PCLK_EN;
}


/**
 * @brief      This function serves to preparations after otp software operation.
 * @param[in]  pce_flag - pce auto mode flag,from the return value of otp_auto_pce_disable function.
 * @return     none
 */
_attribute_ram_code_sec_ static void otp_auto_pce_restore(unsigned char pce_flag){
	/*
	 * If bin is downloaded to ram or flash, can disable pce and clk_en to save power consumption.
	 * if bin is downloaded to otp,after the write process is complete, pce and clk_en need to be turned on in order to get instructions from otp normally.
	 * If clk_en is turned on manually, the clock will work immediately. In the read time sequence, pce and clk_en need to meet
	 * a certain time sequence (tcs_time) regardless of whether pce auto mode is enabled or not, in order to otp instruction
	 * fetching correctly can be satisfied and clk hardware mechanism prevente conflict when pce auto mode is enabled,add by shuaixing.zhai, confirmed by weixin 20211015.
	 */
#if	ONLY_SUPPORT_OTP_PROGRAM
	if(otp_program_flag==1)
#endif
	{
		//ptm
	  reg_otp_ctrl1 &= (~FLD_OTP_PTM);
	  reg_otp_ctrl1 |= OTP_PTM_READ;
	  reg_otp_ctrl0 |= FLD_OTP_PCE;
   /*
	* The time here does not need to be compared with the time set by the hardware, the given time only needs to meet the time of the datasheet,
	* Given the correct timing, after software is out, the pce is in the state of lifting, the hardware will not lift the pce, and the Tcs will not be timed,
	* therefore, there is no need to meet a time greater than or equal to the hardware setting,add by shuaixing.zhai, confirmed by weixin 20211015.
	*/
	   sleep_us(Tcs);
	   reg_otp_ctrl0 |= FLD_OTP_PCLK_EN;

	}
	 if(pce_flag==1){
		 otp_auto_pce_enable();
	}
	irq_restore(g_irq_en);
}
/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 * @param[in]  addr - the address of the data,the OTP memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     0 :it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             1 :it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 2.25V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_write(unsigned int addr,unsigned int word_len,unsigned int *buff)
{
	unsigned char pce_flag=otp_auto_pce_disable();
	for(unsigned int i=0;i<word_len;i++)
	{
		unsigned int temp = 0;
		otp_read_cycle(OTP_PTM_READ,addr + i*4, 1, (unsigned int *)&temp);
		if(temp == 0xffffffff){
			otp_write32(OTP_PTM_PROG,addr + i*4,buff[i]);
	    }else if(temp != buff[i]){
	    	otp_auto_pce_restore(pce_flag);
		    return 1;
	    }

	}
	otp_auto_pce_restore(pce_flag);
	return 0;
}
/**
 * @brief      This function serves to read data from OTP memory,belong to otp normal read.
 *             otp has three kinds of read mode,in general,just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,can use pm_set_dcdc_output(DCDC_2P5V) to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,can use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 1.62V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ void otp_read(unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	unsigned char pce_flag=otp_auto_pce_disable();
	otp_read_cycle(OTP_PTM_READ,addr,word_len,buff);
	otp_auto_pce_restore(pce_flag);
}

#if OTP_ALL_FUNCTION_EN
/**
 * @brief      This function serves to read data from OTP memory,belong to otp pgm marginread.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,can use pm_set_dcdc_output(DCDC_2P5V) to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,can use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 2.8V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ void otp_pgm_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	unsigned char pce_flag=otp_auto_pce_disable();
	otp_read_cycle(OTP_PTM_INIT_MARGIN_READ,addr,word_len,buff);
	otp_auto_pce_restore(pce_flag);
}
/**
 * @brief      This function serves to read data from OTP memory,belong to otp initial margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,can use pm_set_dcdc_output(DCDC_2P5V) to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,can use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 2.8V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ void otp_initial_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	unsigned char pce_flag=otp_auto_pce_disable();
	otp_read_cycle(OTP_PTM_PGM_MARGIN_READ,addr,word_len,buff);
	otp_auto_pce_restore(pce_flag);
}
#endif

/*******************************************************************************************************************
 *												The internal interface
 ******************************************************************************************************************/

/**
 * @brief     This function servers to set tcs time,the minimum value of Tcs_time is 10us,
 *            to improve efficiency, the Tcs_time is set to the minimum value corresponding to the system clock.
 * @param[in] SYS_CLK - system clock,Tcs_time=24M/SYS_CLK*((tcs_config(bit5-7) + 1)*2.688 us).
 * | :-------------- | :--------------- | :------------ |
 * |    SYS_CLK      |    tcs_config    |   tcs_time    |
 * |   12M_Crystal   |     0x01         |   10.752us    |
 * |   16M_Crystal   |     0x02         |   12.096us    |
 * |   24M_Crystal   |     0x03         |   10.752us    |
 * |   32M_Crystal   |     0x05         |   12.096us    |
 * |   48M_Crystal   |     0x07         |   10.752us    |
 * |   24M_RC        |     0x03         |   10.752us    |
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_tcs(SYS_CLK_TypeDef SYS_CLK){
    unsigned char tcs_config= (SYS_CLK>>8) &0x07;
	reg_otp_ctrl2 =(reg_otp_ctrl2 &(~FLD_OTP_TCS_CONFIG))|(tcs_config<<5);
}


/**
 * @brief     This function servers to set otp clk,hardware settings otp maximum clk is 12M,
 *            in this function, otp_clk is set to the highest based on the sys_clk passed in.
 * @param[in] SYS_CLK - system clock,reg_otp_paio_bit6: 0 - two frequency,1 - four frequency.
 * | :-------------- | :---------------  | :------------ |
 * |    SYS_CLK      |    tcs_config     |    otp_clk    |
 * |   12M_Crystal   |     0x00          |      6M       |
 * |   16M_Crystal   |     0x00          |      8M       |
 * |   24M_Crystal   |     0x00          |      12M      |
 * |   32M_Crystal   |     0x01          |      8M       |
 * |   48M_Crystal   |     0x01          |      12M      |
 * |   24M_RC        |     0x00          |      12M      |
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_clk(SYS_CLK_TypeDef SYS_CLK)
{
	unsigned char div_flag = (SYS_CLK>>11)&0x01;
	if(div_flag){
		reg_otp_paio |= FLD_OTP_CLK_DIV;
	}else{
		reg_otp_paio &=~FLD_OTP_CLK_DIV;
	}
}

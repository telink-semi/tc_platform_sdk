/********************************************************************************************************
 * @file	otp_base.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "otp/otp_base.h"
extern unsigned char	otp_program_flag;
/**
 * @brief     This function servers to enable pce auto mode,after enable pce auto mode,
 *            If the time from the last instruction fetching to the next instruction fetching exceeds the set timeout time,
 *            the pce will be pulled down. If the pce is pulled down, the hardware will automatically pull up when the instruction fetching,
 *            and clk will not work until the tsc time. This mechanism can save power consumption but reduce efficiency.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_auto_pce_enable(){
	reg_otp_offsetl |= (FLD_OTP_PCE_AUTO_MODE|FLD_OTP_PCE_TCS_SEL_MODE);

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
    reg_otp_ctrl1 &= ~(FLD_OTP_PDSTD); /* pdstb=0, when power down OTP, PDSTD must be cleared before PLDO is cleared. */
    /* Tplh >= 0(ns) */
    reg_otp_ctrl1 &= ~(FLD_OTP_PLDO); /* pldo=0 */
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
    /*
	 * if enable the pce_auto_mode function, after coming out of the software interface,
	 * the hardware will automatically give the correct timing when taking instructions, without manual configuration;
	 * if no enable the pce_auto_mode function,after coming out of the software interface,
	 * when the program is running in otp, the otp read sequence needs to be restored;
	 */

#if !OTP_PCE_AUTO_MODE_EN
#if !ONLY_SUPPORT_OTP_PROGRAM
    if(otp_program_flag==1)
#endif
    {
    	reg_otp_ctrl0 |= FLD_OTP_PCE;
        while(!(reg_otp_dat|FLD_OTP_READY));
    }
#endif
}

/**
 * @brief     This function servers to set tcs time,the minimum value of Tcs_time is 10us,
 *            to improve efficiency, the Tcs_time is set to the minimum value corresponding to the system clock.
 * @param[in] SYS_CLK - system clock,Tcs_time=(tcs_config(bit5-7)+1)*0x3f(system clock cycle).
 * | :-------------- | :--------------- | :------------ |
 * |    SYS_CLK      |    tcs_config    |   tcs_time    |
 * |   4M_Crystal    |     0x01         |   31.5us      |
 * |   6M_Crystal    |     0x01         |   21us        |
 * |   8M_Crystal    |     0x01         |   15.75us     |
 * |   12M_Crystal   |     0x02         |   15.75us     |
 * |   16M_Crystal   |     0x02         |   11.81us     |
 * |   24M_Crystal   |     0x04         |   13.12us     |
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_tcs(SYS_CLK_TypeDef SYS_CLK){
	unsigned char tcs_config= (SYS_CLK>>8) &0x07;
	reg_otp_offsetl =(reg_otp_offsetl &(~FLD_OTP_PCE_TCS_SEL))|(tcs_config<<5);
}

/********************************************************************************************************
 * @file    watchdog.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page WATCHDOG
 *
 *	Introduction
 *	===============
 *	 -----------------------------------------------------------------------------------------------------------------------------------------------
 *	|  watchdog mode | timer source |            Usage scenarios                         |                          note                            |
 *  |-----------------------------------------------------------------------------------------------------------------------------------------------
 *  |                |              |                                                    |the timer watchdog does not work while sleep              |
 *  | timer watchdog | system clock | only reset exceptions that occur during active     |(because its clock source is no longer available)         |
 *  |                |              |                                                    |                                                          |
 *  |-----------------------------------------------------------------------------------------------------------------------------------------------
 *  |                |              |                                                    |1.If want to use 32K watchdog to handle sleep exceptions, |
 *  |                |              |                                                    |need to pay attention:if there is no timer as the wake-up |
 *  |                |              |                                                    |source in the sleep state,32K watchdog cannot be enabled. |
 *  |                |              |                                                    |                                                          |
 *  |                |              | 1. reset exceptions that occur during active       |                                                          |                                                         |
 *  | 32k watchdog   | 32k timer    | 2. reset exceptions that occur during sleep wakeup |2.Because the 32K clock source may also be used by other  |
 *  |                |              |                                                    |modules,the 32K watchdog has no action to clear watchdog, |
 *  |                |              |                                                    |and can only feed the dog by resetting the capture value. |
 *  |                |              |                                                    |The correct operation process is:                         |
 *  |                |              |                                                    |wd_32k_stop->wd_32k_set_interval_ms->wd_32k_start;        |
 *  |                |              |                                                    |(If set the capture value without stopping,there will be  |
 *  |                |              |                                                    |some intermediate values that can cause an abnormal reset)|
 *   ------------------------------------------------------------------------------------------------------------------------------------------------
 *	API Reference
 *	===============
 *	Header File: watchdog.h
 */
#pragma once


#include "register.h"

/**
 * @brief     This function set the feed dog capture value (capture_tick), the clock source is the system clock.
 * 			  When this capture value is reached, the chip will restart. The actual capture value is only high 14Bits will work,
 * 			  so there will be some deviation from the set value, the deviation can be calculated by the following principle,
 * 			  the actual capture value is: capture_tick = (period_ms*tick_per_ms) & 0xfffc0000.
 * @param[in] period_ms - feeding period time, the unit is ms
 * @param[in] tick_per_ms - tick value required for 1ms under system clock timing
 * @return    none
 */
static inline void wd_set_interval_ms(unsigned int period_ms,unsigned long int tick_per_ms)
{
	static unsigned short tmp_period_ms = 0;
	tmp_period_ms = (period_ms*tick_per_ms>>18);
	reg_tmr2_tick = 0x00000000;    //reset tick register
	reg_tmr_ctrl=(((reg_tmr_ctrl&(~FLD_TMR_WD_CAPT))|((tmp_period_ms<<9)&FLD_TMR_WD_CAPT))&0x00ffffff);//set the capture register
}

/**
 * @brief     start watchdog. ie enable watchdog
 * @param[in] none
 * @return    none
 */
static inline void wd_start(void){
	BM_SET(reg_tmr_ctrl8, FLD_TMR2_EN);
	BM_SET(reg_wd_ctrl1, FLD_WD_EN);
}
/**
 * @brief     stop watchdog. ie disable watchdog
 * @param[in] none
 * @return    none
 */
static inline void wd_stop(void){
	BM_CLR(reg_wd_ctrl1, FLD_WD_EN);
}

/**
 * @brief     clear watchdog.
 * @param[in] none
 * @return    none
 */
static inline void wd_clear(void)
{
	reg_tmr_sta = FLD_TMR_STA_WD;
}

/**
 * @brief     start 32k watchdog.
 * @return    none
 * @note      For otp products, if all codes cannot be executed in ram code, there will be a risk of crash,
 *            so 32K watchdog needs to be enabled to reduce the risk (this interface must be put in ram code to reduce the risk, if put in text segment, there will be a risk of error).
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void);

/**
 * @brief     stop 32k watchdog.
 * @return    none
 * @note      For otp products, if all codes cannot be executed in ram code, there will be a risk of crash,
 *            so 32K watchdog needs to be enabled to reduce the risk (this interface must be put in ram code to reduce the risk, if put in text segment, there will be a risk of error).
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void);

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is  millisecond, ranges from 1~134,217,720ms.
 * @return    none
 * @note      For otp products, if all codes cannot be executed in ram code, there will be a risk of crash,
 *            so 32K watchdog needs to be enabled to reduce the risk (this interface must be put in ram code to reduce the risk, if put in text segment, there will be a risk of error).
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms);

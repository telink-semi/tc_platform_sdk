/********************************************************************************************************
 * @file    watchdog.h
 *
 * @brief   This is the header file for TC321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/**
 * @brief     get 32k watchdog overflow status.
 * @return    watchdog overflow status.
 * @note      -# After the 32k watchdog reboot returns, the status is set to 1,before the mcu enters the next state, wd_32k_clear_status() must be used to clear the status,
 *               otherwise, the next status judgment of the mcu will be affected;
 *            -# When the status is set to 1, if it is not cleared by calling wd_32k_clear_status():
 *                - software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the interface status remains;
 *                - power cyele/reset pin/vbus detect come back, the status of the interface is lost;
 */
_attribute_ram_code_sec_noinline_ unsigned char wd_32k_get_status(void);

/**
 * @brief     Clear the reset state caused by the 32k watchdog overflow.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_clear_status(void);

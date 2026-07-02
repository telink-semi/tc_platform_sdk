/********************************************************************************************************
 * @file    watchdog.h
 *
 * @brief   This is the header file for TC123X
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page WATCHDOG
 *
 *  Introduction
 *  ===============
 *   -----------------------------------------------------------------------------------------------------------------------------------------------
 *  |  watchdog mode | timer source |            Usage scenarios                         |                          note                            |
 *   -----------------------------------------------------------------------------------------------------------------------------------------------
 *  |                |              |                                                    |If want to use 32K watchdog to handle sleep exceptions,   |
 *  | 32k watchdog   |  32k timer   | 1. reset exceptions that occur during active       |need to pay attention:if there is no timer as the wake-up |
 *  |                |              | 2. reset exceptions that occur during sleep wakeup |source in the sleep state,32K watchdog cannot be enabled. |
 *  |                |              |                                                    |                                                          |
 *   -----------------------------------------------------------------------------------------------------------------------------------------------
 *  API Reference
 *  ===============
 *  Header File: watchdog.h
 */
#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "register.h"

/**
 * @brief   wakeup tick type definition
 */
typedef enum
{
    WK_32K_CLK_WIDTH_256MS      = 0x00,
    WK_32K_CLK_WIDTH_8192MS     = 0x20,
    WK_32K_CLK_WIDTH_262144MS   = 0x40,
    WK_32K_CLK_WIDTH_8388608MS  = 0x60,
} wd_32k_clk_sel_e;

/**
 * @brief     start 32k watchdog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void);

/**
 * @brief     stop 32k watchdog.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 *            -# after calling this interface, wd_32k_get_status() will be cleared.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void);

/**
 * @brief     feed 32k watch dog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_feed(void);

/**
 * @brief     get 32k watch dog count.
 * @return    32k watch dog count.
 */
_attribute_ram_code_sec_noinline_ unsigned int wd_32k_get_count_ms(void);

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

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is  millisecond.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_target_value(wd_32k_clk_sel_e wd_32k_clk_sel, unsigned char wd_32k_target);

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is millisecond.
 * @return    none.
 * @note      | period(ms)                   | 256-8192 | 8192-262144 |  262144-8388608 | 8388608-268435456 |
 *            | One wd clock cycle width(ms) |   256    |    8192     |     262144      |       8388608     |
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms);

#endif

/********************************************************************************************************
 * @file	watchdog.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "register.h"
#include "lib/include/pm.h"
#include "analog.h"

/**
 * @brief     start 32k watchdog.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void){

	analog_write(0x70, analog_read(0x70) | 0x01);
}

/**
 * @brief     stop 32k watchdog.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void){

	analog_write(0x70, analog_read(0x70) & 0xfe);
}

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is  millisecond, ranges from 1~134,217,720ms.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms)
{
	unsigned int tmp_period_ms = 0;

	tmp_period_ms = pm_get_32k_tick() + 32 * period_ms;

	analog_write(0x73, tmp_period_ms >> 24);
	analog_write(0x72, tmp_period_ms >> 16);
	analog_write(0x71, tmp_period_ms >> 8);
	analog_write(0x70,(analog_read(0x70)&(~BIT_RNG(5,7)))|(tmp_period_ms&0xe0));
}

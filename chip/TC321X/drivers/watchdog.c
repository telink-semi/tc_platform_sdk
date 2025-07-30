/********************************************************************************************************
 * @file    watchdog.c
 *
 * @brief   This is the source file for TC321X
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
#include "register.h"
#include "lib/include/pm/pm.h"
#include "clock.h"
#include "analog.h"

/**
 * @brief     start 32k watchdog.
 * @return    none
 * @note      For otp products, if all codes cannot be executed in ram code, there will be a risk of crash,
 *            so 32K watchdog needs to be enabled to reduce the risk (this interface must be put in ram code to reduce the risk, if put in text segment, there will be a risk of error).
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void){

	analog_write(0x79, analog_read(0x79) | 0x01);
}

/**
 * @brief     stop 32k watchdog.
 * @return    none
 * @note      For otp products, if all codes cannot be executed in ram code, there will be a risk of crash,
 *            so 32K watchdog needs to be enabled to reduce the risk (this interface must be put in ram code to reduce the risk, if put in text segment, there will be a risk of error).
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void){

        analog_write(0x79, analog_read(0x79) & 0xfe);
}
/**
 * @brief     get 32k watchdog overflow status.
 * @return    watchdog overflow status.
 * @note      -# After the 32k watchdog reboot returns, the status is set to 1,before the mcu enters the next state, wd_32k_clear_status() must be used to clear the status,
 *               otherwise, the next status judgment of the mcu will be affected;
 *            -# When the status is set to 1, if it is not cleared by calling wd_32k_clear_status():
 *                - software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the interface status remains;
 *                - power cyele/reset pin/vbus detect come back, the status of the interface is lost;
 */
_attribute_ram_code_sec_noinline_ unsigned char wd_32k_get_status(void)
{
    return (analog_read(areg_aon_0x64) & FLD_32K_WD_OVERFLOW_STATUS);
}

/**
 * @brief     Clear the reset state caused by the 32k watchdog overflow.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_clear_status(void)
{
    analog_write(areg_aon_0x64, FLD_32K_WD_OVERFLOW_STATUS);
}
/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is  millisecond, ranges from 1~134,217,720ms.
 * @return    none
 * @note      For otp products, if all codes cannot be executed in ram code, there will be a risk of crash,
 *            so 32K watchdog needs to be enabled to reduce the risk (this interface must be put in ram code to reduce the risk, if put in text segment, there will be a risk of error).
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms)
{
	unsigned int tmp_period_ms = 0;

	tmp_period_ms = pm_get_32k_tick() + 32 * period_ms;

	analog_write(0x7c, tmp_period_ms >> 24);//ltimer_watchdog_v[31:24]
	analog_write(0x7b, tmp_period_ms >> 16);//ltimer_watchdog_v[23:16]
	analog_write(0x7a, tmp_period_ms >> 8);//ltimer_watchdog_v[15:8]
	//ltimer_watchdog_v[7:0] is fixed at 0x0. Watch dog is 8ms resolutions
}

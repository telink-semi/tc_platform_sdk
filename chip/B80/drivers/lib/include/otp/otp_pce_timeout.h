/********************************************************************************************************
 * @file    otp_pce_timeout.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2022
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
#ifndef _OTP_PCE_TIMEOUT_H_
#define _OTP_PCE_TIMEOUT_H_

#include "bsp.h"
#if (MCU_CORE_B80)
/**
 * @brief     This function servers to set otp pce timeout time, the optimal time cannot be set according to the system clock,
 *            because the time between two otp fetch commands cannot be determined,otp fetch commands will only be sent when the cache miss occurs,
 *            but when the miss is not known,so this time is self-determining based on power consumption and efficiency.
 * @param[in] timeout_config -range:0-15,timeout_time = 24M/SYS_CLK*((timeout_config(bit[0:3]) + 1)*1.344 us),timeout_config(bit[0:3]):the default value is 0x07.
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_timeout(unsigned char timeout_config);
#endif

#endif /* _OTP_H_ */

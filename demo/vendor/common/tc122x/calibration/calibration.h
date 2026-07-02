/********************************************************************************************************
 * @file    calibration.h
 *
 * @brief   This is the source file for TC1211
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
#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

/**
 * @brief        This function servers to get data from EFUSE. EFUSE default value is 0. The EFUSE address range is 0~32 bytes.
 * @param[in]    addr - the start address of the EFUSE location.
 * @param[in]    buf  - the start address of the buffer.
 * @param[in]    len  - the length(in byte) of content needs to read out from EFUSE.
 * @return       1: operation completed.
 *               0: operation fail.
 */
unsigned char efuse_read(unsigned char addr, unsigned char *buff, unsigned char len);

/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in efuse,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * @return      none
 */
void user_read_efuse_value_calib(void);

#endif


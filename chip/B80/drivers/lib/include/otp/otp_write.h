/********************************************************************************************************
 * @file    otp_write.h
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
#ifndef _OTP_WRITE_H_
#define _OTP_WRITE_H_

#include "bsp.h"

/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 * @param[in]  addr - the address of the data,it has to be a multiple of 4,the OTP memory that can access is from 0x0000-0x3ffc,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     0 :it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             1 :it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 *
 * Attention:
 * -# When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 *    then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 *    the voltage value is at least 2.8V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 * -# Chip version A0/A1/A2/A3 can't read/write OTP in RF RX state, A4 chip doesn't have this limitation.
 *    The chip version number can be read in the following way: after calling cpu_wakeup_init(), read the value of the g_chip_version variable,
 *    which corresponds to the chip version as follows:
 *    A0: g_chip_version = 0xff
 *    A1: g_chip_version = 0xfe
 *    A2: g_chip_version = 0xfc
 *    A3: g_chip_version = 0xf8
 *    A4: g_chip_version = 0xfa
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_write(unsigned int addr,unsigned int word_len,unsigned int *buff);


#endif /* _OTP_H_ */

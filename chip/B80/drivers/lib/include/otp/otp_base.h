/********************************************************************************************************
 * @file	otp_base.h
 *
 * @brief	This is the header file for B80
 *
 * @author	Driver Group
 * @date	2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#ifndef OTP_BASE_H_
#define OTP_BASE_H_
#include "clock.h"
/**
 * @brief     This function servers to enable pce auto mode,after enable pce auto mode,
 *            If the time from the last instruction fetching to the next instruction fetching exceeds the set timeout time,
 *            the pce will be pulled down. If the pce is pulled down, the hardware will automatically pull up when the instruction fetching,
 *            and clk will not work until the tsc time. This mechanism can save power consumption but reduce efficiency.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_auto_pce_enable();

/**
 * @brief      This function serves to otp set deep standby mode,if code run in flash,otp is idle,can enter deep to save current.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_deep_standby_mode(void);
/**
 * @brief      This function serves to otp set active mode,if otp is in deep mode,need to operate on the otp,set active mode.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_active_mode(void);

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
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_tcs(SYS_CLK_TypeDef SYS_CLK);

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
_attribute_ram_code_sec_noinline_ void otp_set_clk(SYS_CLK_TypeDef SYS_CLK);

#endif /* OTP_BASE_H_ */

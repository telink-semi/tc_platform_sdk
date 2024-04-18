/********************************************************************************************************
 * @file    mdec.h
 *
 * @brief   This is the header file for B87
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once
#include "bsp.h"
#include "analog.h"
#include "register.h"
#include "gpio.h"
#include "clock.h"


typedef  enum{
	GPIO_PA0_MDEC,
	GPIO_PB7_MDEC,
	GPIO_PC4_MDEC,
	GPIO_PD0_MDEC
}MDEC_PinTypeDef;

/**
 * @brief     This function servers to reset the MDEC module.When the system is wakeup by MDEC, you should
 * 			  to reset the MDEC module to clear the flag bit of MDEC wakeup.
 * @param[in] none
 * @return    none
 */
static inline void mdec_reset(void)
{
	analog_write(mdec_rst_addr,analog_read(mdec_rst_addr) | FLD_RST_MDEC);
	analog_write(mdec_rst_addr,analog_read(mdec_rst_addr) & (~FLD_RST_MDEC));
}

/**
 * @brief This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]    pin - MDEC_PinTypeDef
 * @return none
 */
void mdec_init(MDEC_PinTypeDef pin);

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat);



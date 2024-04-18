/********************************************************************************************************
 * @file    ir_learn.c
 *
 * @brief   This is the source file for B80
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
#include "ir_learn.h"

/**
 *@brief	this function is used to set rx pin of the ir_learn
 *@param	ir_rx_pin - the rx pin of ir_learn
 *@return	none
 *@note		In the mcu of ir learn, there is a set of values corresponding to gpio, and the relevant mapping relationship needs to be configured (PA0-0x00,PB2-0x0a,PD7-0x20)
 */
void ir_learn_set_pin(GPIO_PinTypeDef ir_rx_pin)
{
	reg_il_cfg0 = ((reg_il_cfg0 &(~FLD_IL_INPUT_SEL))|(BIT_LOW_BIT(ir_rx_pin&0xFF)+((ir_rx_pin>>8)*8)));
	gpio_set_output_en(ir_rx_pin,0);
	gpio_set_input_en(ir_rx_pin,1);
	gpio_set_func(ir_rx_pin,AS_GPIO);
}

/**
 * @brief	This function servers to set the tick counter four modes.
 * @param	cnt_mode - variable to select tick count modes.
 * @return	none.
 */
void ir_learn_set_mode(il_mode_e cnt_mode)
{
	reg_il_cfg0 = ((reg_il_cfg0 & (~FLD_IL_MODE))|(cnt_mode << 5));
}


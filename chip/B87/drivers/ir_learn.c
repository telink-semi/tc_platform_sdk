/********************************************************************************************************
 * @file	ir_learn.c
 *
 * @brief	This is the source file for b87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	reg_il_cfg0 = ((reg_il_cfg0 & (~FLD_IL_INPUT_SEL)) | ( BIT_LOW_BIT(ir_rx_pin&0xFF)+((ir_rx_pin>>8)*8)));

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
	reg_il_cfg0 = ((reg_il_cfg0 & (~FLD_IL_MODE)) | (cnt_mode << 5));
}


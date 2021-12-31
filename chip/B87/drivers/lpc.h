/********************************************************************************************************
 * @file	lpc.h
 *
 * @brief	This is the header file for b87
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
#ifndef LPC_H_
#define LPC_H_

#include "bsp.h"
#include "analog.h"
#include "gpio.h"

/**
 *  @brief  Define the mode for Low power comparator
 */
typedef enum{
	LPC_NORMAL=0,
	LPC_LOWPOWER,
}LPC_Mode_TypeDef;

/**
 *  @brief  Define the input channel for Low power comparator
 */
typedef enum{
	LPC_INPUT_RSVD =0     ,
	LPC_INPUT_PB1=1   ,
	LPC_INPUT_PB2=2   ,
	LPC_INPUT_PB3=3   ,
	LPC_INPUT_PB4=4   ,
	LPC_INPUT_PB5=5   ,
	LPC_INPUT_PB6=6   ,
	LPC_INPUT_PB7=7   ,
}LPC_Input_Channel_TypeDef;

/**
 *  @brief  Define the reference voltage for Low power comparator
 */
typedef enum{
	//normal
	LPC_NORMAL_REF_972MV    = 1,
	LPC_NORMAL_REF_921MV    = 2,
	LPC_NORMAL_REF_870MV    = 3,
	LPC_NORMAL_REF_819MV    = 4,
	LPC_NORMAL_REF_PB0      = 5,
	LPC_NORMAL_REF_PB3      = 6,

	//low power
	LPC_LOWPOWER_REF_964MV  = 1,
	LPC_LOWPOWER_REF_913MV  = 2,
	LPC_LOWPOWER_REF_862MV  = 3,
	LPC_LOWPOWER_REF_810MV  = 4,
	LPC_LOWPOWER_REF_PB0    = 5,
	LPC_LOWPOWER_REF_PB3    = 6,
	//
	LPC_AVDD3               = 7,
}LPC_Reference_TypeDef;

/**
 *  @brief  Define the scaling coefficient for Low power comparator
 */
typedef enum{
	LPC_SCALING_PER25 =0 ,
	LPC_SCALING_PER50 =1 ,
	LPC_SCALING_PER75 =2 ,
	LPC_SCALING_PER100=3 ,
}LPC_Scaling_TypeDef;

/**
 * @brief This function powers down low power comparator.
 * @param[in] none
 * @return none
 */
extern void lpc_power_down(void);

/**
 * @brief This function provides power for low power comparator.
 * @param[in] none
 * @return none
 */
extern void lpc_power_on(void);

/**
 * @brief This function selects input channel for low power comparator .
 * @param[in] pin-selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return none
 */
extern void lpc_set_input_chn(LPC_Input_Channel_TypeDef pin);

/**
 * @brief 		This function selects input reference voltage for low power comparator .
 * @param[in] 	mode - lower power comparator working mode includes normal mode and low power mode.
 * @param[in] 	ref	 - selected input reference voltage.
 * @return 		none
 */
extern void lpc_set_input_ref(LPC_Mode_TypeDef, LPC_Reference_TypeDef ref);

/**
 * @brief 		This function serves to set scaling_coefficient for low power comparator .
 * @param[in] 	divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return 		none
 */
extern void lpc_set_scaling_coeff(LPC_Scaling_TypeDef divider);

/**
 * @brief 		This function serves to get the comparison results.
 * @param[in] 	divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return 		none
 */
static inline unsigned char lpc_get_result(void)
{
	return ((analog_read(0x88)&0x40)>>6);
}
#endif /* LPC_H_ */


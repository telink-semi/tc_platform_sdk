/********************************************************************************************************
 * @file    lpc.h
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


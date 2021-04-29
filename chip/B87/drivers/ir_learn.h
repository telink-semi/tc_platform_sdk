/********************************************************************************************************
 * @file	ir_learn.h
 *
 * @brief	This is the header file for b87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef IR_LEARN_H_
#define IR_LEARN_H_

#include "gpio.h"
#include "register.h"
#include "irq.h"

/**
 *   ir learn interrupt
 */
typedef enum{
	FALLING_EDGE_IRQ       = BIT(0),
	RISING_EDGE_IRQ        = BIT(1),
	TIMEOUT_IRQ    	   	   = BIT(2),
}il_irq_e;

/**
 * IR LEARN target tick trig level,
 * note: This tick value is converted to time according to the system clock
 * example: (system clock is 16M) TARGET_TRIG_LEVEL_6 : 0x002000 -> (DEC:8192 / 16 = 512 us)
 */
typedef enum{
	TICK_VALUE_2,			//tick value: 0x000002
	TICK_VALUE_8,			//tick value: 0x000008
	TICK_VALUE_32,			//tick value: 0x000020
	TICK_VALUE_128,			//tick value: 0x000080
	TICK_VALUE_512,		    //tick value: 0x000200
	TICK_VALUE_2048,		//tick value: 0x000800
	TICK_VALUE_8192,		//tick value: 0x002000
	TICK_VALUE_32768,		//tick value: 0x008000
	TICK_VALUE_131072,		//tick value: 0x020000
	TICK_VALUE_524288,		//tick value: 0x080000
	TICK_VALUE_2097152,		//tick value: 0x200000
}il_target_tick_e;

/**
 * ir learn timeout tick trig level,
 * note: This tick value is converted to time according to the system clock
 * example: (system clock is 16M) TIMEOUT_TRIG_LEVEL_6 : 0x003fff -> (DEC:16383 / 16 =  1023 us)
 */
typedef enum{
	TICK_VALUE_3,			// tick value: 0x000003
	TICK_VALUE_15,			// tick value: 0x00000f
	TICK_VALUE_63,			// tick value: 0x00003f
	TICK_VALUE_255,			// tick value: 0x0000ff
	TICK_VALUE_1023,		// tick value: 0x0003ff
	TICK_VALUE_4095,		// tick value: 0x000fff
	TICK_VALUE_16383,		// tick value: 0x003fff
	TICK_VALUE_65535,		// tick value: 0x00ffff
	TICK_VALUE_262143,		// tick value: 0x03ffff
	TICK_VALUE_1048575,		// tick value: 0x0fffff
	TICK_VALUE_4194303,		// tick value: 0x3fffff
}il_timeout_tick_e;

/**
 * ir learn tick count mode:
 * IL_EN_START_CNT_HIGH_STATE :	After the lr module is enabled(ir_learn_en), it enters a high logic state and starts counting immediately.
 * RISING_EDGE_START_CNT 	  :	After enabling the ir learning module(ir_learn_en), wait for the first rising edge tick to start counting
 * IL_EN_START_CNT_LOW_STATE  :	After the lr module is enabled(ir_learn_en), it enters a low logic state and starts counting immediately.
 * FALLING_EDGE_START_CNT	  :	After enabling the ir learning module(ir_learn_en), wait for the first falling edge tick to start counting
 */
typedef enum{
	RISING_EDGE_START_CNT,			//ir_mode[6,5]: 00,
	IL_EN_HIGH_STATUS_START_CNT,	//ir_mode[6,5]: 01,		This mode can be used as a gpio falling edge detection function
	FALLING_EDGE_START_CNT,			//ir_mode[6,5]: 10,
	IL_EN_LOW_STATUS_START_CNT,		//ir_mode[6,5]: 11,		This mode can be used as a gpio rising edge detection function
}il_mode_e;

/**
 *@brief	this function is used to enable the ir_learn function
 *@param	none
 *@return	none
 */
static inline void ir_learn_en(void)
{
	reg_il_run |= BIT(0);
}

/**
 *@brief	this function is used to disable the ir_learn function
 *@param	none
 *@return	none
 *@note		After the module is disable, the tick count value, high register, cycle register will be reset
 */
static inline void ir_learn_dis(void)
{
	reg_il_run &= ~BIT(0);
}

/**
 *@brief	This function servers to set trigger level of the timeout.
 *@param	timeout_value - select timeout value of the il_timeout_tick_e
 *@return	none
 *@note		When there is no rising edge to clear the tick value, the tick value will keep increasing, and finally reach the set timeout value
 */
static inline void ir_learn_set_timeout(il_timeout_tick_e timeout_value)
{
	reg_il_cfg1 &= ~FLD_IL_TIMEOUT;
	reg_il_cfg1 |= (FLD_IL_TIMEOUT & (timeout_value << 4));
}

/**
 *@brief	This function serves to set the mask of ir learn irq
 *@param	mask - variable of enum to select the mask source.
 *@return	none
 */
static inline void ir_learn_set_irq_mask(il_irq_e mask)
{
	BM_SET(reg_il_mask_0,mask);
}

/**
 *@brief	This function serves to clr the mask of ir learn irq
 *@param	mask - variable of enum to select the mask source.
 *@return	none
 */
static inline void ir_learn_clr_irq_mask(il_irq_e mask)
{
	BM_CLR(reg_il_mask_0,mask);
}

/**
 * @brief     This function servers to get the IRLEARN interrupt status.
 * @param[in] status - variable of enum to select the IRLEARN interrupt source.
 * @return	  ir learn irq status.
 */
static inline unsigned char ir_learn_get_irq_status(il_irq_e status)
{
	return (reg_il_mask_1 & status);
}

/**
 * @brief     This function servers to clear the ir learn interrupt.When a ir learn interrupt occurs, the corresponding interrupt flag bit needs to be cleared manually.
 * @param[in] status  - variable of enum to select the ir learn interrupt source.
 * @return	  none.
 */
static inline void ir_learn_clr_irq_status(il_irq_e status)
{
	reg_il_mask_1 = status;
}

/**
 * @brief	This function servers to read the tick value of the high register
 * @param	none
 * @return	The tick value of the high level duration stored in high register.
 */
static inline unsigned int ir_learn_get_high(void)
{
	return reg_il_high_0;
}

/**
 * @brief	This function servers to read the tick value of the cycle register
 * @param	none
 * @return	The tick value of the cycle in the cycle register.
 */
static inline unsigned int ir_learn_get_cycle(void)
{
	return reg_il_cycle_0;
}

/**
 *@brief	this function is used to set rx pin of the ir_learn
 *@param	ir_rx_pin - the rx pin of ir_learn
 *@return	none
 */
void ir_learn_set_pin(GPIO_PinTypeDef ir_rx_pin);


/**
 * @brief	This function servers to set the tick counter four modes.
 * @param	cnt_mode - variable to select tick count modes.
 * @return	none.
 */
void ir_learn_set_mode(il_mode_e cnt_mode);


#endif /* IR_LEARN_H_ */

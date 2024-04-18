/********************************************************************************************************
 * @file    stimer.h
 *
 * @brief   This is the header file for B80
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
#ifndef STIMER_H_
#define STIMER_H_
#include "register.h"


#define stimer_set_capture_tick systimer_set_capture_tick

/**
 * @brief     This function performs to set the system timer irq capture tick.
 * @param[in] none.
 * @return    timer0 address.
 */
static inline void stimer_set_capture_tick(unsigned long tick)
{
	//0x744 bit[2:0] is always 0, to ensure system timer can be trigger correctly, bit[2:0] of the setting tick should
	//set to 0 to match the value of 0x744
	reg_system_tick_irq_level = (tick & (~((unsigned long)0x07)));
}

/**
 * @brief This function servers to set stimer irq mask.
 * @param[in]	mask - the irq mask.
 * @return  	none.
 */
static inline void stimer_set_irq_mask(void)
{
	reg_irq_mask |= FLD_IRQ_SYSTEM_TIMER;
}

/**
 * @brief This function servers to clear stimer irq mask.
 * @param[in] 	mask - the irq mask.
 * @return  	none.
 */
static inline void stimer_clr_irq_mask(void)
{
	reg_irq_mask &= (~FLD_IRQ_SYSTEM_TIMER);
}

/**
 * @brief This function servers to clear stimer irq status.
 * @param[in] 	status - the irq status.
 * @return  	none.
 */
static inline void stimer_clr_irq_status(void)
{
	reg_irq_src = (FLD_IRQ_SYSTEM_TIMER);
}

/**
 * @brief This function servers to get stimer irq status.
 * @param[in] 	status - the irq status.
 * @return      none.
 */
static inline unsigned char stimer_get_irq_status(void)
{
	return (reg_irq_src & FLD_IRQ_SYSTEM_TIMER) == FLD_IRQ_SYSTEM_TIMER;
}

/**
 * @brief This function servers to enable stimer.
 * @return  none.
 */
static inline void stimer_enable(void)
{
	reg_system_irq_mask |= FLD_SYSTEM_TIMER_AUTO_EN;
}

/**
 * @brief This function servers to disable stimer.
 * @return  none.
 */
static inline void stimer_disable(void)
{
	reg_system_irq_mask &= ~(FLD_SYSTEM_TIMER_AUTO_EN);
}

#endif /* STIMER_H_ */

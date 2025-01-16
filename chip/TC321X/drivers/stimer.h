/********************************************************************************************************
 * @file    stimer.h
 *
 * @brief   This is the header file for TC321X
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
 * @brief       This function is used to set the tick value of the system timer.
 * @param[in]   tick - the value of system timer tick.
 * @return      none.
 */
static _always_inline void stimer_set_tick(unsigned int tick)
{
    reg_system_tick = (tick);
}

/**
 * @brief     This function performs to get system timer tick.
 * @return    system timer tick value.
 */
static _always_inline unsigned int stimer_get_tick(void)
{
    return reg_system_tick;
}

/**
 * @brief   This function is used to enable system timer in manual mode.
 * @return  none.
 */
static _always_inline void stimer_enable_in_manual_mode(void)
{
    reg_system_ctrl |= FLD_SYSTEM_TIMER_MANUAL_EN;
}

/**
 * @brief This function servers to enable stimer.
 * @return  none.
 */
static _always_inline void stimer_enable(unsigned int tick)
{
	stimer_set_tick(tick);
	stimer_enable_in_manual_mode();
}

/**
 * @brief This function servers to disable stimer.
 * @return  none.
 */
static _always_inline void stimer_disable(void)
{
	reg_system_ctrl &= ~(FLD_SYSTEM_TIMER_MANUAL_EN);
}

/**
 * @brief   This function is used to enable 32k tracing.
 * @return  none.
 */
static _always_inline void stimer_32k_tracking_enable(void)
{
    reg_system_ctrl |= FLD_SYSTEM_32K_CAL_EN;
}

/**
 * @brief   This function is used to disable 32k tracing.
 * @return  none.
 */
static _always_inline void stimer_32k_tracking_disable(void)
{
    reg_system_ctrl &= ~(FLD_SYSTEM_32K_CAL_EN);
}

#endif /* STIMER_H_ */

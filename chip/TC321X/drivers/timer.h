/********************************************************************************************************
 * @file    timer.h
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
#ifndef TIMER_H_
#define TIMER_H_
#include "compiler.h"
#include "register.h"
#include "analog.h"
#include "gpio.h"

/**
 * @brief   Type of Timer
 */
typedef enum{
	TIMER0		=0,
	TIMER1		=1,
	TIMER2		=2,
}TIMER_TypeDef;

/**
 * @brief   Mode of Timer
 */
typedef enum{
	TIMER_MODE_SYSCLK		=0,
	TIMER_MODE_GPIO_TRIGGER	=1,
	TIMER_MODE_GPIO_WIDTH	=2,
	TIMER_MODE_TICK			=3,
}TIMER_ModeTypeDef;

typedef enum{
	TMR_STA_TMR0 =			BIT(0),
	TMR_STA_TMR1 =			BIT(1),
	TMR_STA_TMR2 =			BIT(2),
	TMR_STA_WD =			BIT(3),
}tmr_sta_e;

/**
 * @brief   system Timer : 32Khz, Constant
 */
enum{
	CLOCK_32K_SYS_TIMER_CLK_1S  = 32*1000,
	CLOCK_32K_SYS_TIMER_CLK_1MS = 32,
};


/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer0.
 * @param[in] pin - select pin for timer0.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
extern void timer0_gpio_init(GPIO_PinTypeDef pin, GPIO_PolTypeDef pol);

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer1.
 * @param[in] pin - select pin for timer1.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
extern void timer1_gpio_init(GPIO_PinTypeDef pin, GPIO_PolTypeDef pol);

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer2.
 * @param[in] pin - select pin for timer2.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
extern void timer2_gpio_init(GPIO_PinTypeDef pin,GPIO_PolTypeDef pol);

/**
 * @brief     set mode, initial tick and capture of timer0.
 * @param[in] mode - select mode for timer0.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
extern void timer0_set_mode(TIMER_ModeTypeDef mode,unsigned int init_tick, unsigned int cap_tick);

/**
 * @brief     set mode, initial tick and capture of timer1.
 * @param[in] mode - select mode for timer1.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
extern void timer1_set_mode(TIMER_ModeTypeDef mode,unsigned int init_tick, unsigned int cap_tick);

/**
 * @brief     set mode, initial tick and capture of timer2.
 * @param[in] mode - select mode for timer2.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
extern void timer2_set_mode(TIMER_ModeTypeDef mode,unsigned int init_tick, unsigned int cap_tick);

/**
 * @brief     the specified timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
extern void timer_start(TIMER_TypeDef type);

/**
 * @brief     the specified timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
extern void timer_stop(TIMER_TypeDef type);

/**
 * @brief		This function servers to set timer irq mask.
 * @param[in]	mask - variable of enum to select the timer interrupt status source.
 * @return		none.
 */
static inline void timer_set_irq_mask(tmr_sta_e mask)
{
	reg_irq_mask |= mask;
}

/**
 * @brief		This function servers to clear timer irq mask.
 * @param[in]	mask - variable of enum to select the timer interrupt status source.
 * @return		none.
 */
static inline void timer_clr_irq_mask(tmr_sta_e mask)
{
	reg_irq_mask &= (~mask);
}

/**
 * @brief     This function servers to clear the timer interrupt status.
 * @param[in] status  - variable of enum to select the timer interrupt status source.
 * @return	  none.
 */
static inline void timer_clear_interrupt_status(tmr_sta_e status)
{
	   reg_tmr_sta = status;
}

/**
 * @brief     This function servers to get the timer interrupt status.
 * @param[in] status - variable of enum to select the timer interrupt status source.
 * @return	  none.
 */
static inline unsigned char timer_get_interrupt_status(tmr_sta_e status){
	 return reg_tmr_sta & status;
}
#endif /* TIMER_H_ */

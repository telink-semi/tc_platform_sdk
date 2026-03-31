/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "app_config.h"
#include "calibration.h"

extern void user_init(void);
extern void main_loop (void);

int timer0_irq_cnt = 0;
int timer1_irq_cnt = 0;
int timer2_irq_cnt = 0;
int stimer_irq_cnt = 0;
unsigned int gpio_width =0;

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if(TIMER_MODE == TIMER_SYS_CLOCK_MODE)
	if(timer_get_interrupt_status(TMR_STA_TMR0))
	{
		timer_clear_interrupt_status(TMR_STA_TMR0); //clear irq status
		timer0_irq_cnt ++;
		gpio_toggle(LED2);
	}
#elif(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	if(timer_get_interrupt_status(TMR_STA_TMR0))
	{
		timer_clear_interrupt_status(TMR_STA_TMR0); //clear irq status
		timer0_irq_cnt ++;
		gpio_toggle(LED2);
	}

#elif(TIMER_MODE == TIMER_GPIO_WIDTH_MODE)
	
	if(timer_get_interrupt_status(TMR_STA_TMR0))
	{
		timer_clear_interrupt_status(TMR_STA_TMR0); //clear irq status
		gpio_toggle(LED2);
#if (!(MCU_CORE_TC1211 || MCU_CORE_TC122X))
		gpio_width = reg_tmr0_tick;
		reg_tmr0_tick = 0;
#else
		gpio_width = reg_tmr0_tick0 | (reg_tmr0_tick1<<8) | (reg_tmr0_tick2<<16);
		reg_tmr0_tick0 = 0;
		reg_tmr0_tick1 = 0;
		reg_tmr0_tick2 = 0;
#endif
	}
#elif(TIMER_MODE == STIMER_MODE)
#if (MCU_CORE_TC321X || MCU_CORE_TC122X)
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ))
#else
	if(stimer_get_irq_status())
#endif
	{
#if (MCU_CORE_TC321X || MCU_CORE_TC122X)
		stimer_clr_irq_status(FLD_SYSTEM_IRQ);            			//clear irq status
#else
		stimer_clr_irq_status();
#endif
		stimer_set_capture_tick(clock_time() + CLOCK_SYS_TIMER_CLK_1S);
		stimer_irq_cnt++;
		gpio_toggle(LED2);
	}
#endif
	
}
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
#if(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
	blc_pm_select_internal_32k_crystal();
#endif

    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop();
	}
	return 0;
}


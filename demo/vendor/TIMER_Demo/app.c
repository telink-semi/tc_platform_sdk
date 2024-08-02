/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for B85m
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

volatile unsigned int t0;

void user_init(void)
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On
	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2 ,0);			//disable input
	gpio_write(LED2, 0);
	gpio_set_func(LED3 ,AS_GPIO);
	gpio_set_output_en(LED3, 1); 		//enable output
	gpio_set_input_en(LED3 ,0);			//disable input
	gpio_write(LED3, 0);
	gpio_set_func(LED4 ,AS_GPIO);
	gpio_set_output_en(LED4, 1); 		//enable output
	gpio_set_input_en(LED4 ,0);			//disable input
	gpio_write(LED4, 0);


#if (TIMER_MODE==TIMER_SYS_CLOCK_MODE)
	timer0_set_mode(TIMER_MODE_SYSCLK,0,1000 * CLOCK_SYS_CLOCK_1MS);
	timer_start(TIMER0);
	irq_enable();
#elif(TIMER_MODE==TIMER_GPIO_TRIGGER_MODE)
	/****  timer0 POL_FALLING  TIMER_GPIO link LED3  **/
	timer0_gpio_init(TIMER_GPIO, POL_FALLING);
	irq_enable();
	timer0_set_mode(TIMER_MODE_GPIO_TRIGGER,0,3);
	timer_start(TIMER0);
#elif(TIMER_MODE==TIMER_GPIO_WIDTH_MODE)
	timer0_gpio_init(TIMER_GPIO, POL_FALLING);
	irq_enable();
	timer0_set_mode(TIMER_MODE_GPIO_WIDTH,0,0);
	timer_start(TIMER0);
	gpio_write(LED3, 0);
	sleep_ms(250);
	gpio_write(LED3, 1);
#elif(TIMER_MODE==TIMER_TICK_MODE)
	timer0_set_mode(TIMER_MODE_TICK,0,0);
	timer_start(TIMER0);
#elif(TIMER_MODE==TIMER_WATCHDOG_MODE)
	wd_set_interval_ms(1000,CLOCK_SYS_CLOCK_1MS);
	wd_start();
#elif(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
	gpio_write(LED2, 1);
	blc_pm_select_internal_32k_crystal();
	wd_32k_set_interval_ms(1000);
	wd_32k_start();
#elif(TIMER_MODE==STIMER_MODE && (MCU_CORE_B80 || MCU_CORE_B80B))
	stimer_set_irq_mask();
	stimer_set_capture_tick(clock_time() + CLOCK_16M_SYS_TIMER_CLK_1S);
	stimer_enable();
	irq_enable();
#endif

}

void main_loop (void)
{
#if(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	gpio_toggle(LED3);
	gpio_toggle(LED4);

#elif(TIMER_MODE == TIMER_TICK_MODE)

	if(reg_tmr0_tick > 500 * CLOCK_SYS_CLOCK_1US*1000)
	{
		reg_tmr0_tick = 0;
		gpio_toggle(LED3);
		gpio_toggle(LED4);
	}

#endif


#if(TIMER_MODE==TIMER_WATCHDOG_MODE)
	t0= clock_time();
	while(!clock_time_exceed(t0,990000));
	wd_clear();
	gpio_toggle(LED1);

#elif(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
#if(MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89)
	sleep_ms(990);
	//32K watchdog capture time settings: program run time and sleep time to leave some margin.
	wd_32k_stop();

	wd_32k_set_interval_ms(1000);

	wd_32k_start();
	sleep_ms(200);
	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD|PM_WAKEUP_TIMER, (clock_time() + 500*CLOCK_16M_SYS_TIMER_CLK_1MS));
	wd_32k_stop();
	gpio_toggle(LED1);
#endif
#else

	sleep_ms(500);
	gpio_toggle(LED1);

#endif
}


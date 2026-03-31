/********************************************************************************************************
 * @file    app.c
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

volatile unsigned int t0;

#if (MCU_CORE_TC1211)
#define LED2 GPIO_PA2
#define LED3 GPIO_PA2
#define LED4 GPIO_PA2
#endif

void user_init(void)
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

#if !(MCU_CORE_TC1211)
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
#endif

#if (TIMER_MODE==TIMER_SYS_CLOCK_MODE)
	timer0_set_mode(TIMER_MODE_SYSCLK,0,500 * CLOCK_SYS_CLOCK_1MS);
	timer_start(TIMER0);
	irq_enable();
#elif(TIMER_MODE==TIMER_GPIO_TRIGGER_MODE)
	/****  timer0 POL_FALLING  TIMER_GPIO link LED3  **/
	timer0_gpio_init(TIMER_GPIO, POL_FALLING);
	irq_enable();
	timer0_set_mode(TIMER_MODE_GPIO_TRIGGER,0,3);
	timer_start(TIMER0);
#elif(TIMER_MODE==TIMER_GPIO_WIDTH_MODE)
	/****  timer0 POL_FALLING  TIMER_GPIO link LED3  **/
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
#elif(TIMER_MODE==TIMER_WATCHDOG_MODE)//TC1211/TC122X NOT SUPPORT
	/**
	 * Since the SW reset of Raptor A0 cannot be used, it also leads to the inability to use the Timer watchdog.
	 */
	wd_set_interval_ms(1000,CLOCK_SYS_CLOCK_1MS);
	wd_start();
#elif(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
    sleep_ms(500);
    gpio_write(LED1, 1);

#if !(MCU_CORE_B80 || MCU_CORE_B80B)
    //Remove the stop 32k watchdog operation in main, otherwise this state cannot be read.
    if (wd_32k_get_status())
    {
    	for(int i=0; i<8; i++)
    	{
			gpio_write(LED3, 1);
			sleep_ms(60);
			gpio_write(LED3, 0);
			sleep_ms(60);
    	}
    	gpio_write(LED3, 1);
        wd_32k_clear_status();
        if (wd_32k_get_status())
        {
        	gpio_write(LED4, 1);
        	while(1){}
        }
    }
#endif

    #if defined(MCU_CORE_TC122X)
    wd_32k_set_interval_ms(1000);
    #elif defined(MCU_CORE_TC1211)
	/**
	 * For TC1211, period_ms must be set to a multiple of 2048ms
	 * and the 32k watch dog reboot may take place at any time between “ period_ms ~ (period_ms+2048ms) ".
	 */
    wd_32k_stop();
    wd_32k_set_interval_ms(1000 + 2048);
    wd_32k_start();
    #else
    wd_32k_stop();
    wd_32k_set_interval_ms(1000);
    wd_32k_start();
    #endif

#elif(TIMER_MODE==STIMER_MODE)
	stimer_set_capture_tick(clock_time() + CLOCK_16M_SYS_TIMER_CLK_1S);
#if(MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B85)
	stimer_set_irq_mask();
#elif(MCU_CORE_TC321X)
	stimer_set_irq_mask(FLD_SYSTEM_IRQ_MASK);
	irq_set_mask(FLD_IRQ_SYSTEM_TIMER);
#endif
	irq_enable();
#endif

}

void main_loop (void)
{
#if(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	gpio_toggle(LED3);
	gpio_toggle(LED4);

#elif(TIMER_MODE == TIMER_TICK_MODE)

#if (!(MCU_CORE_TC1211 || MCU_CORE_TC122X))
	if(reg_tmr0_tick > 500 * CLOCK_SYS_CLOCK_1US*1000)
	{
		reg_tmr0_tick = 0;
		gpio_toggle(LED3);
		gpio_toggle(LED4);
	}
#else
	unsigned int tmr0_tick = reg_tmr0_tick0 | (reg_tmr0_tick1<<8) | (reg_tmr0_tick2<<16);
	if(tmr0_tick > 500 * CLOCK_SYS_CLOCK_1US*1000)
	{
		reg_tmr0_tick0 = 0;
		reg_tmr0_tick1 = 0;
		reg_tmr0_tick2 = 0;
		gpio_toggle(LED3);
		gpio_toggle(LED4);
	}
#endif

#endif


#if(TIMER_MODE==TIMER_WATCHDOG_MODE)//TC1211 NOT SUPPORT
	t0= clock_time();
	while(!clock_time_exceed(t0,990000));
	wd_clear();
	gpio_toggle(LED1);

#elif(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
#if(MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89 || MCU_CORE_TC321X || MCU_CORE_TC1211 || MCU_CORE_TC122X)
    //800ms<1000ms, watchdog does not overflow and the program continues to run.
    for(int i=0; i<2; i++)
    {
    	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 200 * CLOCK_SYS_TIMER_CLK_1MS);
        gpio_write(LED2, 1);
        sleep_ms(200);
        gpio_write(LED2, 0);
    }

	#if defined(MCU_CORE_TC122X)
    wd_32k_feed();
    #elif defined(MCU_CORE_TC1211)
	/**
	 * For TC1211, period_ms must be set to a multiple of 2048ms
	 * and the 32k watch dog reboot may take place at any time between “ period_ms ~ (period_ms+2048ms) ".
	 */
    wd_32k_stop();
    wd_32k_set_interval_ms(2000 + 2048);
    wd_32k_start();
    #else
    wd_32k_stop();
    wd_32k_set_interval_ms(2000);
    wd_32k_start();
    #endif

    for(int i=0; i<4; i++)
    {
    	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 200 * CLOCK_SYS_TIMER_CLK_1MS);
        gpio_write(LED2, 1);
        sleep_ms(200);
        gpio_write(LED2, 0);
    }
#if (MCU_CORE_TC1211)
    //4400ms>2048ms, watchdog overflows, program restarts.
    cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 2800 * CLOCK_SYS_TIMER_CLK_1MS);
#else
    //2400ms>2000ms, watchdog overflows, program restarts.
    cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 800 * CLOCK_SYS_TIMER_CLK_1MS);
#endif
    gpio_write(LED4, 1);
    while(1){}
#endif
#else

	sleep_ms(500);
	gpio_toggle(LED1);

#endif
}


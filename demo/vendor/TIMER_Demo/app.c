/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2018
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
#include "app_config.h"

volatile unsigned int t0;

void user_init()
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
	timer0_gpio_init(SW1, POL_FALLING);
	irq_enable();
	timer0_set_mode(TIMER_MODE_GPIO_TRIGGER,0,3);
	timer_start(TIMER0);
#elif(TIMER_MODE==TIMER_GPIO_WIDTH_MODE)
	timer0_gpio_init(SW1, POL_FALLING);
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
#elif(TIMER_MODE==STIMER_MODE)
	stimer_set_irq_mask();
	stimer_set_capture_tick(clock_time() + CLOCK_16M_SYS_TIMER_CLK_1S);
	stimer_enable();
	irq_enable();
#endif

}

void main_loop (void)
{
#if(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	gpio_toggle(LED3|LED4);

#elif(TIMER_MODE == TIMER_TICK_MODE)

	if(reg_tmr0_tick > 500 * CLOCK_SYS_CLOCK_1US*1000)
	{
		reg_tmr0_tick = 0;
		gpio_toggle(LED3|LED4);
	}

#endif


#if(TIMER_MODE==TIMER_WATCHDOG_MODE)
	t0= clock_time();
	while(!clock_time_exceed(t0,990000));
	wd_clear();
	gpio_toggle(LED1);
#else

	sleep_ms(500);
	gpio_toggle(LED1);

#endif
}


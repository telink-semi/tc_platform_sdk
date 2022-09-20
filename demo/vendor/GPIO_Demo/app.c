/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

void user_init()
{
	sleep_ms(2000);

#if(GPIO_MODE == GPIO_HIGH_RESISTOR)
	gpio_shutdown(GPIO_ALL);				//set all gpio as high resistor except sws and mspi

#else
	//1.init the LED pin,for indication
	gpio_set_func(LED1,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1,0);			//disable input
	gpio_write(LED1, 0);              	//LED Off
	gpio_set_func(LED2,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2,0);			//disable input
	gpio_write(LED2, 0);              	//LED Off
	gpio_set_func(LED3,AS_GPIO);
	gpio_set_output_en(LED3, 1); 		//enable output
	gpio_set_input_en(LED3,0);			//disable input
	gpio_write(LED3, 0);              	//LED Off
	gpio_set_func(LED4,AS_GPIO);
	gpio_set_output_en(LED4, 1); 		//enable output
	gpio_set_input_en(LED4,0);			//disable input
	gpio_write(LED4, 0);              	//LED Off

#if (GPIO_DEMO_MODE == GPIO_DEMO_KEY)
	/*
	 * Button matrix table:
	 * 			KEY3	KEY4
	 * 	KEY1	SW2		SW3
	 * 	KEY2	SW4		SW5
	 */
	gpio_set_func(KEY3 ,AS_GPIO);
	gpio_set_output_en(KEY3, 1); 		//enable output
	gpio_set_input_en(KEY3 ,0);			//disable input
	gpio_write(KEY3, 0);

	//2.init the KEY1 pin,for trigger interrupt
	gpio_set_func(KEY1 ,AS_GPIO);
	gpio_set_output_en(KEY1, 0); 			// disable output
	gpio_set_input_en(KEY1 ,1);				// enable input

	/* Taking the GPIO_IRQ interrupt configuration as an example,
	 * other GPIO_IRQ_RSIC0, GPIO_IRQ_RSIC1, GPIO_SEL_IRQ_SRC interrupt configurations are similar. ***/
#if (GPIO_MODE == GPIO_IRQ )
	gpio_setup_up_down_resistor(KEY1, PM_PIN_PULLUP_10K);
	/****GPIO_IRQ POL_FALLING   Press SW2 to connect KEY1 and KEY3 to trigger an interrupt. **/
	gpio_set_interrupt(KEY1, POL_FALLING);	//When SW2 is pressed, the falling edge triggers the interrupt.
	irq_enable();
#endif

#elif (GPIO_DEMO_MODE == GPIO_DEMO_SQUARE_WAVE)
	/****  IRQ pin link PA0, PA0 produces a square wave.  **/
	gpio_set_func(GPIO_PA0 ,AS_GPIO);
	gpio_set_output_en(GPIO_PA0, 1); 		//enable output
	gpio_set_input_en(GPIO_PA0 ,0);			//disable input
	gpio_write(GPIO_PA0, 0);

	//2.init the IRQ pin, for trigger interrupt
	gpio_set_func(IRQ_PIN ,AS_GPIO);
	gpio_set_output_en(IRQ_PIN, 0); 			// disable output
	gpio_set_input_en(IRQ_PIN ,1);				// enable input

	/* Taking the GPIO_IRQ_RSIC0, GPIO_IRQ_RSIC1, GPIO_SEL_IRQ_SRC interrupt configuration as an example,
	 * other GPIO_IRQ interrupt configurations are similar. ***/
#if(GPIO_MODE == GPIO_IRQ_RSIC0)
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLDOWN_100K);
	/****GPIO_IRQ_RSIC0  POL_RISING   toggle PA0 to trigger an interrupt. **/
	gpio_set_interrupt_risc0(IRQ_PIN, POL_RISING);	//When switching PA0, the rising edge triggers an interrupt.
	irq_enable();

#elif(GPIO_MODE == GPIO_IRQ_RSIC1)
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
	/****GPIO_IRQ_RSIC1  POL_FALLING   toggle PA0 to trigger an interrupt. **/
	gpio_set_interrupt_risc1(IRQ_PIN, POL_FALLING);	//When switching PA0, the falling edge triggers an interrupt.
	irq_enable();

#elif((GPIO_MODE == GPIO_SEL_IRQ_SRC)&&(MCU_CORE_B80))
	/*Note : the use method of 8 new GPIO irq source :
	 * First ,you can choose a gpio group,like gpio GPIO_GROUP_A,8 GPIO in all.
	 * Second,the 8 gpio you choose corresponding to the 8 gpio irq source respectively.for example,irq source0-GPIO_PA0,irq source1-GPIO-PA1......
	 * Attention:Once you choose a gpio-group,you can only use the gpio as irq_source in this group.And you should obey the rule of correspondence.
	 */
	gpio_setup_up_down_resistor(SET_GROUP_GPIO, PM_PIN_PULLUP_10K);
	gpio_set_src_irq_group(SET_GROUP);
	gpio_set_src_irq(SET_GROUP_GPIO, SRC_IRQ_FALLING_EDGE);	//When switching PA0, the falling edge triggers an interrupt.
	irq_enable();
#endif
#endif
#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
   sleep_ms(10);
#if (GPIO_DEMO_MODE == GPIO_DEMO_SQUARE_WAVE)
   gpio_toggle(GPIO_PA0);
#endif
}





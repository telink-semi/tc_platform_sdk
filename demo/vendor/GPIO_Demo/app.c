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
	//1.init the LED pin,for indication
	gpio_set_func(LED1,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	gpio_set_func(GPIO_PA0 ,AS_GPIO);
	gpio_set_output_en(GPIO_PA0, 1); 		//enable output
	gpio_set_input_en(GPIO_PA0 ,0);			//disable input
	gpio_write(GPIO_PA0, 0);
	//2.init the SW1 pin,for trigger interrupt
#if (GPIO_MODE == GPIO_IRQ )
	gpio_set_func(SW1 ,AS_GPIO);
	gpio_set_output_en(SW1, 0); 			// disable output
	gpio_set_input_en(SW1 ,1);				// enable input
	gpio_setup_up_down_resistor(SW1, PM_PIN_PULLUP_10K);
	gpio_set_interrupt(SW1, POL_FALLING);
	irq_enable();
#elif(GPIO_MODE == GPIO_IRQ_RSIC0)
	gpio_set_func(SW1 ,AS_GPIO);
	gpio_set_output_en(SW1, 0); 			// disable output
	gpio_set_input_en(SW1 ,1);				// enable input
	gpio_setup_up_down_resistor(SW1, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc0(SW1, POL_FALLING);
	irq_enable();

#elif(GPIO_MODE == GPIO_IRQ_RSIC1)
	gpio_set_func(SW1 ,AS_GPIO);
	gpio_set_output_en(SW1, 0); 			// disable output
	gpio_set_input_en(SW1 ,1);				// enable input
	gpio_setup_up_down_resistor(SW1, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc1(SW1, POL_FALLING);
	irq_enable();

#elif(GPIO_MODE == GPIO_TOGGLE)
	gpio_write(LED1, !gpio_read(LED1));
	gpio_toggle(LED1);

#elif(GPIO_MODE == GPIO_HIGH_RESISTOR)
	gpio_shutdown(GPIO_ALL);				//set all gpio as high resistor except sws and mspi
#elif((GPIO_MODE == GPIO_SEL_IRQ_SRC)&&(MCU_CORE_B80))
	/*Note : the use method of 8 new GPIO irq source :
	 * First ,you can choose a gpio group,like gpio GPIO_GROUP_A,8 GPIO in all.
	 * Second,the 8 gpio you choose corresponding to the 8 gpio irq source respectively.for example,irq source0-GPIO_PA0,irq source1-GPIO-PA1......
	 * Attention:Once you choose a gpio-group,you can only use the gpio as irq_source in this group.And you should obey the rule of correspondence.
	 */
	gpio_set_src_irq_group(SET_GROUP);
	gpio_set_func(SET_GROUP_GPIO ,AS_GPIO);
	gpio_set_output_en(SET_GROUP_GPIO, 0); 			// disable output
	gpio_set_input_en(SET_GROUP_GPIO ,1);			// enable  input
	gpio_setup_up_down_resistor(SET_GROUP_GPIO, PM_PIN_PULLUP_10K);
	gpio_set_src_irq(SET_GROUP_GPIO, SRC_IRQ_FALLING_EDGE);
	irq_enable();
#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
   sleep_ms(10);
   gpio_toggle(GPIO_PA0);
}





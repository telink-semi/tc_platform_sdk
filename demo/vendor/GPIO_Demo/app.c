/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
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

void user_init()
{
	sleep_ms(2000);

#if(GPIO_MODE == GPIO_HIGH_RESISTOR)
	gpio_shutdown(GPIO_ALL);				//set all gpio as high resistor except sws and mspi

#else
	//0.init the LED pin,for indication
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
	/*
	 * Button matrix table:
	 * 			KEY3	KEY4
	 * 	KEY1	SW2		SW3
	 * 	KEY2	SW4		SW5
	 */
	//1.Initialization signal generation output pin
	gpio_set_func(KEY3 ,AS_GPIO);
	gpio_set_output_en(KEY3, 1); 		//enable output
	gpio_set_input_en(KEY3 ,0);			//disable input
	//2.Initialize interrupt detection input pin
	gpio_set_func(IRQ_PIN ,AS_GPIO);
	gpio_set_output_en(IRQ_PIN, 0); 	// disable output
	gpio_set_input_en(IRQ_PIN ,1);	    // enable input
	/*
	 * When IRQ_PIN' interrupt type is set to falling edge, set the IRQ_PIN pull-up   resistor and KEY3 outputs low level.
	 * When IRQ_PIN' interrupt type is set to rising  edge, set the IRQ_PIN pull-down resistor and KEY3 outputs high level.
	 * */
#if (GPIO_MODE == GPIO_IRQ )
	gpio_write(KEY3, 0);
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
	gpio_set_interrupt(IRQ_PIN, POL_FALLING);
	irq_enable();
#elif(GPIO_MODE == GPIO_IRQ_RSIC0)
	gpio_write(KEY3, 1);
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLDOWN_100K);
	gpio_set_interrupt_risc0(IRQ_PIN, POL_RISING);
	irq_enable();
#elif(GPIO_MODE == GPIO_IRQ_RSIC1)
	gpio_write(KEY3, 0);
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc1(IRQ_PIN, POL_FALLING);
	irq_enable();
#elif (GPIO_MODE == GPIO_IRQ_RSIC2)
	gpio_write(KEY3, 0);
    gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
    gpio_set_interrupt_risc2(IRQ_PIN, POL_FALLING);
    irq_enable();
#elif(GPIO_MODE == GPIO_SEL_IRQ_SRC)
	/*Note : the use method of 8 new GPIO irq source :
	 * First ,you can choose a gpio group,like gpio GPIO_GROUP_A,8 GPIO in all.
	 * Second,the 8 gpio you choose corresponding to the 8 gpio irq source respectively.for example,irq source0-GPIO_PA0,irq source1-GPIO-PA1......
	 * Attention:Once you choose a gpio-group,you can only use the gpio as irq_source in this group.And you should obey the rule of correspondence.
	 */
    gpio_write(KEY3, 0);
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
	gpio_set_src_irq_group(IRQ_PIN>>8);             /*Set the group corresponding to IRQ_PIN*/
	gpio_set_src_irq(IRQ_PIN, SRC_IRQ_FALLING_EDGE);/*IRQ_PIN is used as an example to configure up to 8 gpio.*/
	irq_enable();
#endif

#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
   sleep_ms(50);
#if (GPIO_DEMO_MODE == GPIO_DEMO_SQUARE_WAVE)
   gpio_toggle(KEY3);
#endif
}





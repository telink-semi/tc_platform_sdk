/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
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

void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	//2.init the SW1 pin,for trigger interrupt
#if (GPIO_MODE == GPIO_IRQ )
	gpio_set_func(SW1 ,AS_GPIO);
	gpio_set_output_en(SW1, 0); 			//enable output
	gpio_set_input_en(SW1 ,1);				//disable input
	gpio_setup_up_down_resistor(SW1, PM_PIN_PULLUP_10K);
	gpio_set_interrupt(SW1, POL_FALLING);
	irq_enable();

#elif(GPIO_MODE == GPIO_IRQ_RSIC0)
	gpio_set_func(SW2 ,AS_GPIO);
	gpio_set_output_en(SW2, 0); 			//enable output
	gpio_set_input_en(SW2 ,1);				//disable input
	gpio_setup_up_down_resistor(SW2, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc0(SW2, POL_FALLING);
	irq_enable();

#elif(GPIO_MODE == GPIO_IRQ_RSIC1)
	gpio_set_func(SW2 ,AS_GPIO);
	gpio_set_output_en(SW2, 0); 			//enable output
	gpio_set_input_en(SW2 ,1);				//disable input
	gpio_setup_up_down_resistor(SW2, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc1(SW2, POL_FALLING);
	irq_enable();

#elif(GPIO_MODE == GPIO_TOGGLE)
	gpio_write(LED1, !gpio_read(LED1)); 
	gpio_toggle(LED1);

#elif(GPIO_MODE == GPIO_HIGH_RESISTOR)
	gpio_shutdown(GPIO_ALL);				//set all gpio as high resistor except sws and mspi
	
#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{

}





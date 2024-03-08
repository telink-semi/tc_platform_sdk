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
#if (GPIO_MODE != AUTO_TEST_MODE)
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
#elif (GPIO_MODE == GPIO_IRQ_RSIC2)//only B80_A and B80_B support
	gpio_write(KEY3, 0);
    gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
    gpio_set_interrupt_risc2(IRQ_PIN, POL_FALLING);
    irq_enable();
#elif(GPIO_MODE == GPIO_SEL_IRQ_GROUP)//only B80_A support
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
#elif (GPIO_MODE == GPIO_SEL_IRQ_NEW_RISC)//only B80_B support
	/*Note:
	 * An IRQ_PIN  can be mapped to an NEW_RISK, and up to 5 new_risks(RISC3~RISC7) can be set up at the same time.
	 */
	gpio_write(KEY3, 0);
	gpio_setup_up_down_resistor(IRQ_PIN, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_new_risc(IRQ_PIN, SRC_IRQ_RISING_EDGE, GPIO_IRQ_NEW_RISC_NUM);
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

volatile unsigned int gpio_irq_cnt;
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
    sleep_ms(1000);
#if (GPIO_MODE == GPIO_IRQ )

	if((reg_irq_src & FLD_IRQ_GPIO_EN)==FLD_IRQ_GPIO_EN){
		reg_irq_src = FLD_IRQ_GPIO_EN; // clear the relevant irq
			gpio_irq_cnt++;
			gpio_toggle(LED1);
	}

#elif(GPIO_MODE == GPIO_IRQ_RSIC0)

	if((reg_irq_src & FLD_IRQ_GPIO_RISC0_EN)==FLD_IRQ_GPIO_RISC0_EN){
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN; // clear the relevant irq
			gpio_irq_cnt++;
			gpio_toggle(LED2);
	}

#elif(GPIO_MODE == GPIO_IRQ_RSIC1)

	if((reg_irq_src & FLD_IRQ_GPIO_RISC1_EN)==FLD_IRQ_GPIO_RISC1_EN){
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EN; // clear the relevant irq

			gpio_irq_cnt++;
			gpio_toggle(LED3);
	}
#elif (GPIO_MODE == GPIO_IRQ_RSIC2)//only B80_A and B80_B support
    if((reg_irq_src & FLD_IRQ_GPIO_RISC2_EN)==FLD_IRQ_GPIO_RISC2_EN){
        reg_irq_src = FLD_IRQ_GPIO_RISC2_EN; // clear the relevant irq
        gpio_irq_cnt++;
    	gpio_toggle(LED4);
    }

#elif(GPIO_MODE == GPIO_SEL_IRQ_GROUP)//only B80_A support
	volatile unsigned char gpio_irqsrc;
	gpio_irqsrc = (reg_gpio_irq_from_pad & IRQ_PIN);
	if(gpio_irqsrc)
	{
		reg_gpio_irq_from_pad = IRQ_PIN;
		gpio_irq_cnt++;
		gpio_toggle(LED1);
	}
#elif(GPIO_MODE == GPIO_SEL_IRQ_NEW_RISC)//only B80_B support
	/*Note:
	  * When multiple new risc are set up at the same time, the corresponding bit in reg_irq_src (FLD_IRQ_GPIO_NEW_EN) is automatically cleared to 0 after all irq states are cleared.
	 */
	if(gpio_get_new_risk_irq_status(BIT(GPIO_IRQ_NEW_RISC_NUM)))
	{
		gpio_clr_new_risk_irq_status(BIT(GPIO_IRQ_NEW_RISC_NUM));
	}

#endif

}
#endif

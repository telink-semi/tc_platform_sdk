/********************************************************************************************************
 * @file	app_bqb.c
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
#include "BQB/bqb.h"
#if(TEST_DEMO==BQB_DEMO)

#define BQB_PA_TX_PORT     RFFE_TX_PD1
#define BQB_PA_RX_PORT     RFFE_RX_PD0

#define BQB_UART_BUAD	   115200

void user_init(void)
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);	 //disable input
	gpio_write(LED1, 0);         //LED On

	uart_gpio_set(BQB_UART_TX_PORT, BQB_UART_RX_PORT);// uart tx/rx pin set
	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
	uart_init_baudrate(BQB_UART_BUAD,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);
	uart_dma_enable(0, 0);
	irq_disable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);
	uart_irq_enable(0,0);   //uart RX irq enable
	uart_ndma_irq_triglevel(0,0);	//set the trig level. 1 indicate one byte will occur interrupt
	bqbtest_init();
	//SET PA PORT
#if USER_REDEFINE_PA
	gpio_set_func(BQB_PA_TX_PORT ,AS_GPIO);
	gpio_set_output_en(BQB_PA_TX_PORT, 1); //enable output
	gpio_set_input_en(BQB_PA_TX_PORT ,0);	 //disable input
	gpio_write(BQB_PA_TX_PORT, 0);         //LED On
	gpio_set_func(BQB_PA_RX_PORT ,AS_GPIO);
	gpio_set_output_en(BQB_PA_RX_PORT, 1); //enable output
	gpio_set_input_en(BQB_PA_RX_PORT ,0);	 //disable input
	gpio_write(BQB_PA_RX_PORT, 0);         //LED On
#else
	rf_rffe_set_pin(BQB_PA_TX_PORT,BQB_PA_RX_PORT);
#endif
}

void main_loop(void)
{
	bqb_serviceloop();
}
#endif

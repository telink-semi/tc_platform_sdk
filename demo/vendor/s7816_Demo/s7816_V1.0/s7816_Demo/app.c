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

volatile __attribute__((aligned(4))) unsigned char s7816_tx_buff_byte[5] = {0x00,0x84,0x00,0x00,0x04};

void user_init(void)
{
    s7816_set_pin(S7816_RST_PIN,S7816_VCC_PIN,S7816_CLK_PIN,S7816_TRX_PIN);
    s7816_init(S7816_4MHZ,F,D);

    irq_enable();

    uart_irq_enable(1,0);   //uart RX irq enable

    uart_ndma_irq_triglevel(1,0);	//set the trig level. 1 indicate one byte will occur interrupt

    s7816_mode(S7816_RX);
}

void main_loop (void)
{
	/*********************activate and coldReset and set trx pin***************/
	s7816_coldreset();// the coldreset accompanied by IC-CARD activate.
	sleep_ms(50);//wait for the return atr.
	//s7816_warmreset(); //the warmreset is required after the IC-CARD activate.
	/*******************************TX*****************************/
	s7816_mode(S7816_TX);
	for(int i=0;i<5;i++)
	{
		uart_ndma_send_byte(s7816_tx_buff_byte[i]);
	    //delay_ms(0.5);//extra protect time
	}
	s7816_mode(S7816_RX);
	/******************************RX****************************/
}





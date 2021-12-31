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

volatile __attribute__((aligned(4))) unsigned char s7816_tx_buff_byte[5] = {0x00,0x84,0x00,0x00,0x04};

void user_init()
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





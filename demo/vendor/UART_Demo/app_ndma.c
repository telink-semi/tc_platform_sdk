/********************************************************************************************************
 * @file	app_ndma.c
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

#if (UART_MODE==UART_NDMA)

#define rec_buff_Len    16
#define trans_buff_Len  16

volatile unsigned char uart_rx_flag;
volatile unsigned int  uart_ndmairq_cnt;
volatile unsigned char uart_ndmairq_index;
volatile unsigned char uart_cts_count=0;
extern volatile unsigned char rts_count;
__attribute__((aligned(4))) unsigned char rec_buff[rec_buff_Len]={0};
__attribute__((aligned(4))) unsigned char trans_buff[trans_buff_Len] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on

	uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set

	uart_reset();  //uart module power-on again.
	//baud rate: 115200
	uart_init_baudrate(115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

	uart_dma_enable(0, 0);

	irq_disable_type(FLD_IRQ_DMA_EN);

	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);

	uart_irq_enable(1,0);   //uart RX irq enable

	uart_ndma_irq_triglevel(1,0);	//set the trig level. 1 indicate one byte will occur interrupt

	uart_mask_error_irq_enable();// open uart_error_mask,when stop bit error or parity error,it will enter error_interrupt.

	irq_enable();

#if( FLOW_CTR ==  USE_CTS )
	uart_set_cts(1, STOP_VOLT, UART_CTS_PIN);
#elif( FLOW_CTR ==   USE_RTS )
	uart_set_rts(1, RTS_MODE, RTS_THRESH, RTS_INVERT,UART_RTS_PIN);
#endif
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	sleep_ms(1000);
#if( FLOW_CTR == NONE)

	for(unsigned char i=0;i<trans_buff_Len;i++){
		uart_ndma_send_byte(trans_buff[i]);
	}
	if(uart_rx_flag>0){
		uart_ndmairq_cnt=0; //Clear uart_ndmairq_cnt
		uart_rx_flag=0;
		for(unsigned char i=0;i<rec_buff_Len;i++){
			uart_ndma_send_byte(rec_buff[i]);
		}
	}
#elif( FLOW_CTR ==  USE_CTS )
	uart_ndma_send_byte(trans_buff[uart_cts_count]);
	uart_cts_count++;
	if(uart_cts_count == 16)
	{
		uart_cts_count=0;
	}

#elif( FLOW_CTR ==  USE_RTS )
	while(rts_count==1)//to make the rts obvious,design the program to trigger rts one time.
	{
		uart_irq_enable(0,0);//dis rhe rx,then the data will store in the rx buff until the amount of data trigger rts pin.
		if((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT)>(RTS_THRESH-1))//trigger rts pin
		{
			sleep_ms(1000);
			uart_irq_enable(1,0);//enable rx,then the data sending and receiving will proceed normally,
			rts_count=0;//trigger only once.
			break;
		}
	}
#endif
}

#endif
















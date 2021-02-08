/********************************************************************************************************
 * @file	app_dma.c
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

#if (UART_MODE==UART_DMA)

/**
 * rec_buff_Len Set up rules:(Both conditions have to be met)
 * 1.The rec_buff_Len value is a multiple of 16 	(16 * n)
 * 2.The length of the receive data is less than (16 * n -4)
 */
#define rec_buff_Len    32
#define trans_buff_Len  16

__attribute__((aligned(4))) unsigned char rec_buff[rec_buff_Len]={0};
__attribute__((aligned(4))) unsigned char trans_buff[trans_buff_Len] = {0x0c,0x00,0x00,0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc};
volatile unsigned char uart_dmairq_tx_cnt;
volatile unsigned char uart_dmairq_rx_cnt;
extern volatile unsigned char rts_count;
volatile unsigned char uart_dma_send_flag = 1;

void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on

	//note: dma addr must be set first before any other uart initialization! (confirmed by sihui)
	uart_recbuff_init( (unsigned char *)rec_buff, sizeof(rec_buff));

	uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set

	uart_reset();  //uart module power-on again.

	//baud rate: 115200
	uart_init_baudrate(115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

	uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first

	irq_enable_type(FLD_IRQ_DMA_EN);// uart_rx use dma_rx irq

	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX, 1);   	//uart Rx dma irq enable

	irq_enable_type(FLD_IRQ_UART_EN);// uart_tx use uart_txdone irq

	uart_mask_tx_done_irq_enable();

	uart_mask_error_irq_enable();// open uart_error_mask,when stop bit error or parity error,it will enter error_interrupt.

	irq_enable();

#if( FLOW_CTR ==  USE_CTS )
	uart_set_cts(1, STOP_VOLT, UART_CTS_PIN);
#elif( FLOW_CTR ==   USE_RTS )
	uart_set_rts(1, RTS_MODE, RTS_THRESH, RTS_INVERT,UART_RTS_PIN);
#endif
}

void main_loop (void)
{
#if( (FLOW_CTR == NONE)||(FLOW_CTR ==  USE_CTS))

	sleep_ms(100);
	if(uart_dma_send_flag == 1)
	{
		uart_dma_send((unsigned char *)trans_buff);
		trans_buff[4] ++;
		uart_dma_send_flag = 0;
	}

#elif( FLOW_CTR ==  USE_RTS )
	while(rts_count==1) //to make the rts obvious,design the program to trigger rts one time.
	{
		uart_dma_enable(0,1); //dis rhe rx,then the data will store in the rx buff until the amount of data trigger rts pin.
		if((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT)>(RTS_THRESH-1))  //trigger rts pin
		{
			sleep_ms(1000);
			uart_dma_enable(1,1);//enable rx,then the data sending and receiving will proceed normally,
			rts_count=0;//trigger only once.
			break;
		}
	}
#endif
}
#endif















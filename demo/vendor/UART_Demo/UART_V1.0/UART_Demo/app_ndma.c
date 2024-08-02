/********************************************************************************************************
 * @file    app_ndma.c
 *
 * @brief   This is the source file for B85m
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

#if (UART_MODE==UART_NDMA)

#define rec_buff_Len    16
#define trans_buff_Len  16

volatile unsigned char uart_rx_flag;
volatile unsigned char uart_rx_done_flag;
volatile unsigned int  uart_ndmairq_cnt;
volatile unsigned char uart_ndmairq_index;
volatile unsigned char uart_cts_count=0;
extern volatile unsigned char rts_count;
volatile  unsigned char uart_rx_trig_level=1;//B85/B87/B89 can only be 1,B80 can be 1 or 4.
__attribute__((aligned(4))) unsigned char rec_buff[rec_buff_Len]={0};
__attribute__((aligned(4))) unsigned char trans_buff[trans_buff_Len] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

void user_init(void)
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on

#if( UART_WIRE_MODE == UART_1WIRE_MODE)
	uart_set_rtx_pin(UART_RTX_PIN);// the status of rtx line will be rx by default,if there is a send-action,the status of rtx-line will changed to tx,and changed to rx immediately if send over.
	uart_rtx_en();
#elif(( UART_WIRE_MODE == UART_2WIRE_MODE))
	uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

	uart_reset();  //uart module power-on again.
	//baud rate: 115200
	uart_init_baudrate(115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

	uart_dma_enable(0, 0);

	irq_disable_type(FLD_IRQ_DMA_EN);

	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);

	uart_irq_enable(1,0);   //uart rx_buff irq enable

	uart_ndma_irq_triglevel(uart_rx_trig_level,0);	//set the trig level. 1 indicate one byte will occur interrupt
#if(MCU_CORE_B80)
	uart_rxdone_irq_en();  //mask rx_done irq
#endif
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
	gpio_toggle(LED1);
#if(FLOW_CTR == NONE)
#if(MCU_CORE_B80)
	if(uart_rx_done_flag>0)
#else
	if(uart_rx_flag>0)
#endif
	{
		uart_ndmairq_cnt=0; //Clear uart_ndmairq_cnt
		uart_rx_flag=0;
		uart_rx_done_flag=0;
		for(unsigned char i=0;i<rec_buff_Len;i++)
		{
			uart_ndma_send_byte(rec_buff[i]);
#if( UART_WIRE_MODE == UART_1WIRE_MODE)
			uart_rtx_pin_tx_trig();
#endif
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
		uart_irq_enable(0,0);//dis irq rx,then the data will store in the rx buff until the amount of data trigger rts pin.
#if(MCU_CORE_B80)
		uart_rxdone_irq_dis();
#endif
		if((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT)>(RTS_THRESH-1))//trigger rts pin
		{
			sleep_ms(1000);
			uart_irq_enable(1,0);//enable rx,then the data sending and receiving will proceed normally,
#if(MCU_CORE_B80)
			uart_rxdone_irq_en();
#endif
			rts_count=0;//trigger only once.
			break;
		}
	}
#endif
}

#endif
















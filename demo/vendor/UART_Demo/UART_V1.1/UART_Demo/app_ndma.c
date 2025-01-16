/********************************************************************************************************
 * @file    app_ndma.c
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
	gpio_write(LED4, 0);
#if( UART_WIRE_MODE == UART_1WIRE_MODE)
	uart_set_rtx_pin(UART_MODULE_SEL,UART_RTX_PIN);// the status of rtx line will be rx by default,if there is a send-action,the status of rtx-line will changed to tx,and changed to rx immediately if send over.
	uart_rtx_en(UART_MODULE_SEL);
#elif(( UART_WIRE_MODE == UART_2WIRE_MODE))
	uart_gpio_set(UART_MODULE_SEL,UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

	uart_reset(UART_MODULE_SEL);  //uart module power-on again.
	//baud rate: 115200
	uart_init_baudrate(UART_MODULE_SEL,115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

	uart_dma_enable(UART_MODULE_SEL,0, 0);

	irq_disable_type(FLD_IRQ_DMA_EN);

#if (UART_MODULE_SEL == UART0_MODULE)
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);
#elif (UART_MODULE_SEL == UART1_MODULE)
#ifndef MCU_CORE_TC321X /* TC321X only UART0 support DMA. */
	dma_chn_irq_enable(FLD_DMA_CHN_UART1_RX | FLD_DMA_CHN_UART1_TX, 0);
#endif
#endif

	uart_irq_enable(UART_MODULE_SEL,1,0);   //uart rx_buff irq enable

	uart_ndma_irq_triglevel(UART_MODULE_SEL,uart_rx_trig_level,0);	//set the trig level. 1 indicate one byte will occur interrupt

	uart_rxdone_irq_en(UART_MODULE_SEL);  //mask rx_done irq

	uart_mask_error_irq_enable(UART_MODULE_SEL);// open uart_error_mask,when stop bit error or parity error,it will enter error_interrupt.

	irq_enable();

#if( FLOW_CTR ==  USE_CTS )
	uart_set_cts(UART_MODULE_SEL,1, STOP_VOLT, UART_CTS_PIN);
#elif( FLOW_CTR ==   USE_RTS )
	uart_set_rts(UART_MODULE_SEL,1, RTS_MODE, RTS_THRESH, RTS_INVERT,UART_RTS_PIN);
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
	if(uart_rx_done_flag>0)
	{
		uart_ndmairq_cnt=0; //Clear uart_ndmairq_cnt
		uart_rx_flag=0;
		uart_rx_done_flag=0;
		for(unsigned char i=0;i<rec_buff_Len;i++)
		{
			uart_ndma_send_byte(UART_MODULE_SEL,rec_buff[i]);
#if( UART_WIRE_MODE == UART_1WIRE_MODE)
			uart_rtx_pin_tx_trig(UART_MODULE_SEL);
#endif
		}
	}
#elif( FLOW_CTR ==  USE_CTS )
	uart_ndma_send_byte(UART_MODULE_SEL,trans_buff[uart_cts_count]);
	uart_cts_count++;
	if(uart_cts_count == 16)
	{
		uart_cts_count=0;
	}

#elif( FLOW_CTR ==  USE_RTS )
	while(rts_count==1)//to make the rts obvious,design the program to trigger rts one time.
	{
		uart_irq_enable(UART_MODULE_SEL,0,0);//dis irq rx,then the data will store in the rx buff until the amount of data trigger rts pin.
		uart_rxdone_irq_dis(UART_MODULE_SEL);
		if((reg_uart_buf_cnt(UART_MODULE_SEL)&FLD_UART_RX_BUF_CNT)>(RTS_THRESH-1))//trigger rts pin
		{
			sleep_ms(1000);
			uart_irq_enable(UART_MODULE_SEL,1,0);//enable rx,then the data sending and receiving will proceed normally,
			uart_rxdone_irq_en(UART_MODULE_SEL);

			rts_count=0;//trigger only once.
			break;
		}
	}
#endif
}
#endif
















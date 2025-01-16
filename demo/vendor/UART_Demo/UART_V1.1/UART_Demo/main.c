/********************************************************************************************************
 * @file    main.c
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
#include "calibration.h"

extern void user_init(void);
extern void main_loop (void);

/**
 * rec_buff_Len Set up rules:(Both conditions have to be met)
 * 1.The rec_buff_Len value is a multiple of 16 	(16 * n)
 * 2.The length of the receive data is less than (16 * n -4)
 */
#define rec_buff_Len    	32
#define trans_buff_Len    	16
volatile unsigned char rts_count=1;


#if(UART_MODE==UART_NDMA)
extern volatile unsigned char uart_rx_flag;
extern volatile unsigned char uart_rx_done_flag;
extern volatile unsigned int  uart_ndmairq_cnt;
extern volatile unsigned char uart_ndmairq_index;
extern volatile  unsigned char uart_rx_trig_level;

#elif(UART_MODE==UART_DMA)
extern volatile unsigned char uart_dmairq_tx_cnt;
extern volatile unsigned char uart_dmairq_rx_cnt;
extern volatile unsigned char uart_dma_send_flag;
extern volatile unsigned char uart_dmairq_err_cnt;
#endif

extern unsigned char rec_buff[rec_buff_Len];
extern unsigned char trans_buff[trans_buff_Len];
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
#if (UART_MODE!=UART_SOFTWARE_RX)
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{

#if (UART_MODE==UART_DMA)


	if(reg_uart_status1(UART_MODULE_SEL) & FLD_UART_TX_DONE)
	{
		uart_dmairq_tx_cnt++;
		uart_dma_send_flag = 1;
		gpio_toggle(LED2);
		uart_clr_tx_done(UART_MODULE_SEL);
	}
#if(UART_DMA_INT_TYPE == UART_RXDMA_IRQ)

#if (UART_MODULE_SEL == UART0_MODULE)
	if(dma_chn_irq_status_get(FLD_DMA_CHN_UART_RX))
#elif (UART_MODULE_SEL == UART1_MODULE)
	if(dma_chn_irq_status_get(FLD_DMA_CHN_UART1_RX))
#endif

	{
        uart_send_dma(UART_MODULE_SEL,(unsigned char *)rec_buff);

#if (UART_MODULE_SEL == UART0_MODULE)
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
#elif (UART_MODULE_SEL == UART1_MODULE)
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART1_RX);
#endif

		uart_dmairq_rx_cnt++;
	}
#elif(UART_DMA_INT_TYPE == UART_RXDONE_IRQ)
	if(reg_uart_status1(UART_MODULE_SEL)&FLD_UART_RXDONE_IRQ)
	{
		uart_send_dma(UART_MODULE_SEL,(unsigned char *)rec_buff);
		 reg_uart_status0(UART_MODULE_SEL) =FLD_URAT_CLEAR_RXDONE_FLAG;
		uart_dmairq_rx_cnt++;
	}
#endif
	if(uart_is_parity_error(UART_MODULE_SEL))//when stop bit error or parity error.
	{
		uart_clear_parity_error(UART_MODULE_SEL);
		gpio_write(LED4,1);
		uart_dmairq_err_cnt++;
	}
#elif(UART_MODE==UART_NDMA)
	static unsigned char uart_ndma_irqsrc;
	uart_ndma_irqsrc = uart_ndmairq_get(UART_MODULE_SEL);
	if(uart_ndma_irqsrc)
	{
	 if(reg_uart_status1(UART_MODULE_SEL)&FLD_UART_RX_BUF_IRQ)
	 {
		 if(uart_rx_flag==0)
		 {
			 unsigned char fifo_cnt=reg_uart_buf_cnt(UART_MODULE_SEL)&FLD_UART_RX_BUF_CNT;
			 for(int i=0;i<fifo_cnt;i++){

				 if((uart_ndmairq_cnt%trans_buff_Len==0)&&(uart_ndmairq_cnt!=0))
				 {
					 uart_ndma_read_byte(UART_MODULE_SEL);
				 }else{
					 rec_buff[uart_ndmairq_cnt++] = uart_ndma_read_byte(UART_MODULE_SEL);
				 }
			 }
			 if((uart_ndmairq_cnt%trans_buff_Len==0)&&(uart_ndmairq_cnt!=0))
			 {
				 uart_rx_flag=1;
			 }

		 }
		 else
		 {
			 unsigned char uart_fifo_cnt =0;
			 uart_fifo_cnt= reg_uart_buf_cnt(UART_MODULE_SEL)&FLD_UART_RX_BUF_CNT;
			 if(uart_fifo_cnt !=0)
			 {
				 for(unsigned char j=0;j<uart_fifo_cnt;j++){
					 uart_ndma_read_byte(UART_MODULE_SEL);
				 }
			 }
		 }

	 }
	 if(reg_uart_status1(UART_MODULE_SEL)&FLD_UART_RXDONE_IRQ)
	 {
         unsigned char uart_rxbuff_cnt=0;
		 uart_rxbuff_cnt= reg_uart_buf_cnt(UART_MODULE_SEL)&FLD_UART_RX_BUF_CNT;
		 if(uart_rxbuff_cnt>0)
		 {
			 for(int j=0;j<uart_rxbuff_cnt;j++)
			{
				 if(uart_rx_flag==0)
				 {
					 rec_buff[uart_ndmairq_cnt++] = uart_ndma_read_byte(UART_MODULE_SEL);
				     if((uart_ndmairq_cnt%trans_buff_Len==0)&&(uart_ndmairq_cnt!=0))
				     {
				    	 uart_rx_flag=1;
				    	 uart_rx_done_flag=1;
				    	 break;
				     }
				 }

			}


		 }
		 if(uart_rx_flag==1)
		 {
			 uart_rx_done_flag=1;

		 }
		 reg_uart_status0(UART_MODULE_SEL) =FLD_URAT_CLEAR_RXDONE_FLAG;
		 uart_reset(UART_MODULE_SEL);//clearing the rx_buff does not clear the rx_fifo and requires uart_reset.
		 uart_ndma_clear_rx_index(UART_MODULE_SEL);
		 reg_uart_status0(UART_MODULE_SEL)=FLD_UART_CLEAR_RX_FLAG;
	 }

     if(uart_is_parity_error(UART_MODULE_SEL))
     {
    	 uart_reset(UART_MODULE_SEL);
    	 uart_ndma_clear_rx_index(UART_MODULE_SEL);
    	 uart_ndmairq_cnt=0;
    	 uart_rx_flag=0;
    	 for(int i=0;i<rec_buff_Len;i++)
		 {
    		 rec_buff[i]=0;
		 }

     }
}
#endif

}
#endif
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}



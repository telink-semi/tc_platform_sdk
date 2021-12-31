/********************************************************************************************************
 * @file	main.c
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
#include "calibration.h"

extern void user_init();
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

_attribute_ram_code_sec_noinline_ void irq_handler(void)
{

#if (UART_MODE==UART_DMA)


	if(reg_uart_status1 & FLD_UART_TX_DONE)
	{
		uart_dmairq_tx_cnt++;
		uart_dma_send_flag = 1;
		gpio_toggle(LED2);
		uart_clr_tx_done();
	}
#if(UART_DMA_INT_TYPE == UART_RXDMA_IRQ)
	if(dma_chn_irq_status_get() & FLD_DMA_CHN_UART_RX)
	{
        uart_send_dma((unsigned char *)rec_buff);
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
		uart_dmairq_rx_cnt++;
	}
#elif(UART_DMA_INT_TYPE == UART_RXDONE_IRQ&&MCU_CORE_B80)
	if(reg_uart_status1&FLD_UART_RXDONE_IRQ)
	{
		uart_send_dma((unsigned char *)rec_buff);
		 reg_uart_status0 =FLD_URAT_CLEAR_RXDONE_FLAG;
		uart_dmairq_rx_cnt++;
	}
#endif
	if(uart_is_parity_error())//when stop bit error or parity error.
	{
		uart_clear_parity_error();
		gpio_write(LED4,1);
		uart_dmairq_err_cnt++;
	}
#elif(UART_MODE==UART_NDMA)
	static unsigned char uart_ndma_irqsrc;
	uart_ndma_irqsrc = uart_ndmairq_get();
	if(uart_ndma_irqsrc)
	{
	 if(reg_uart_status1&FLD_UART_RX_BUF_IRQ)
	 {
		 if(uart_rx_flag==0)
		 {
			 for(int i=0;i<uart_rx_trig_level;i++){
				 rec_buff[uart_ndmairq_cnt++] = uart_ndma_read_byte();
				 if((uart_ndmairq_cnt%trans_buff_Len==0)&&(uart_ndmairq_cnt!=0))
				 {
					 uart_rx_flag=1;
				 }
			 }
		 }
		 else
		 {
			 unsigned char uart_fifo_cnt =0;
			 uart_fifo_cnt= reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT;
			 if(uart_fifo_cnt !=0)
			 {
				 for(unsigned char j=0;j<uart_fifo_cnt;j++){
					 uart_ndma_read_byte();
				 }
			 }
		 }

	 }
#if(MCU_CORE_B80)
	 if(reg_uart_status1&FLD_UART_RXDONE_IRQ)
	 {
         unsigned char uart_rxbuff_cnt=0;
		 uart_rxbuff_cnt= reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT;
		 if(uart_rxbuff_cnt>0)
		 {
			 for(int j=0;j<uart_rxbuff_cnt;j++)
			{
				 if(uart_rx_flag==0)
				 {
					 rec_buff[uart_ndmairq_cnt++] = uart_ndma_read_byte();
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
		 reg_uart_status0 =FLD_URAT_CLEAR_RXDONE_FLAG;
		 uart_reset();//clearing the rx_buff does not clear the rx_fifo and requires uart_reset.
		 uart_ndma_clear_rx_index();
		 reg_uart_status0=FLD_UART_CLEAR_RX_FLAG;
	 }

#endif
     if(uart_is_parity_error())
     {
    	 uart_reset();
    	 uart_ndma_clear_rx_index();
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

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{

#if(MCU_CORE_B80||MCU_CORE_B89)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#endif
#if (MCU_CORE_B85) || (MCU_CORE_B87)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif (MCU_CORE_B89)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_otp_value_calib();
#endif

	clock_init(SYS_CLK);

	gpio_init(0);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}



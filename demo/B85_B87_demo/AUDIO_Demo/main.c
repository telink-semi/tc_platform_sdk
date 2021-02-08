/********************************************************************************************************
 * @file	main.c
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

extern void user_init();
extern void main_loop (void);
# if(AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ)
extern const short *buffer ;
extern volatile unsigned int remaining;
extern unsigned short half_buff_length;
#endif
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{
	/**
	 * @brief  This function servers to  write data to fifo by timer  interrupt.
	 * step 0:set timer interval,it  equals approximately the time of sdm play half depth of fifo.
	 * step 1: circulate write the half number of dfifo0 depth ,if fifo will full,write zero.
	 * step 2: if sample data finish,stop timer*/
# if(AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ)
	if(reg_tmr_sta & FLD_TMR_STA_TMR1)
		{
			reg_tmr_sta |= FLD_TMR_STA_TMR1; //clear irq status
			if (remaining)
			{
				unsigned int block_len_temp=((reg_dfifo0_wptr+1+half_buff_length)==reg_dfifo0_rptr)? 0:(half_buff_length>>1);
	        	unsigned int  block_len = (remaining <block_len_temp) ? remaining :block_len_temp;
	            audio_rx_data_from_sample_buff(buffer, block_len);
				buffer += block_len;
				remaining -= block_len;
			}
			else
			{
				timer_stop(TIMER1);
			}
		}
#endif
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {

#if (MCU_CORE_B89)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B85)
	cpu_wakeup_init();
#endif

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}






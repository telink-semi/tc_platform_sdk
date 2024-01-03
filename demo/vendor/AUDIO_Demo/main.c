/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
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

extern void user_init();
extern void main_loop (void);
# if(AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ)
extern const short *buffer ;
extern volatile unsigned int remaining;
extern unsigned short half_buff_length;
#elif (AUDIO_MODE == AUDIO_SINE_WAVE_TO_SDM)
extern  volatile  unsigned  int  block_16;
extern const short *buffer ;
extern  volatile  unsigned  int  sample_num ;
#endif
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
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
#elif (AUDIO_MODE == AUDIO_SINE_WAVE_TO_SDM)
	if(reg_tmr_sta & FLD_TMR_STA_TMR1)
			{
			   reg_tmr_sta |= FLD_TMR_STA_TMR1; //clear irq status
			   audio_rx_data_from_sample_buff(buffer+block_16, 16);
			   block_16=(block_16+16);
			   if( block_16>( sample_num-16))
			   {
				 block_16=0;
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

#if (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B89)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
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

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(0);

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}






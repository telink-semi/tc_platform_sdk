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
extern unsigned char sd_adc_rx_done_flag;

volatile unsigned char sample_cnt=0;
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if (ADC_MODE == ADC_DMA_MODE)
	if(dma_chn_irq_status_get(FLD_DMA_CHN_SAR_ADC))
	{
		sample_cnt++;
		sd_adc_rx_done_flag=1;
		adc_stop_sample_dma();
		dma_chn_irq_status_clr(FLD_DMA_CHN_SAR_ADC);
	}
#endif

}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {

    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}


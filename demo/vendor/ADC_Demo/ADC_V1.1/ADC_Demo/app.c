/********************************************************************************************************
 * @file    app.c
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

adc_gpio_cfg_t adc_gpio_cfg_m =
{
        .v_ref       = ADC_VREF_1P2V,
        .pre_scale   = ADC_PRESCALE_1F4,
        .sample_freq = ADC_SAMPLE_FREQ_96K,
        .pin         = GPIO_M_CHN_SAMPLE_PIN,
};


volatile unsigned short adc_m_chn_val;
volatile unsigned int adc_data = 0;
unsigned char sd_adc_rx_done_flag = 0;


#if (ADC_MODE == ADC_DMA_MODE)
/**
 * The reason for "ADC_SAMPLE_GROUP_CNT + 2" is as follows:
 * Since the write-back feature of ADC's DMA5 is enabled by default and cannot be disabled, and the length information occupies 1 word,
 * the buffer must be allocated with 1 additional word of space.
 */
unsigned short adc_sample_buffer[(ADC_SAMPLE_GROUP_CNT + 2) * ADC_SAMPLE_CHN_CNT] __attribute__((aligned(4))) = {0};
#elif (ADC_MODE == ADC_NDMA_MODE)
unsigned short adc_sample_buffer[ADC_SAMPLE_GROUP_CNT * ADC_SAMPLE_CHN_CNT] __attribute__((aligned(4))) = {0};
#endif
unsigned short channel_buffers[3][ADC_SAMPLE_GROUP_CNT] __attribute__((aligned(4))) = {{0}, {0}};

unsigned short adc_sort_and_get_average_code(unsigned short *channel_sample_buffer);
unsigned short adc_get_result(adc_transfer_mode_e transfer_mode, adc_sample_chn_e chn);

#if (ADC_MODE == ADC_DMA_MODE)

void adc_code_split_dma(unsigned short *sample_buffer, unsigned int sample_num, unsigned char chn_cnt, unsigned short buffers[chn_cnt][sample_num]);
#endif

void user_init(void)
{
	gpio_set_func(LED1,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1,0);			//disable input
	gpio_write(LED1, 0);

#if (ADC_MODE == ADC_NDMA_MODE)
    adc_init(NDMA_M_CHN);
	#if (ADC_SAMPLE_MODE == ADC_GPIO_SAMPLE)
	adc_gpio_sample_init(ADC_M_CHANNEL, adc_gpio_cfg_m);
	#elif (ADC_SAMPLE_MODE == ADC_VBAT_SAMPLE)
	adc_vbat_sample_init(ADC_M_CHANNEL);
	#endif

#elif (ADC_MODE == ADC_DMA_MODE)
	adc_rx_buff_init((unsigned int *)adc_sample_buffer,ADC_SAMPLE_GROUP_CNT);
	adc_set_irq_mask();
    irq_enable_type(FLD_IRQ_DMA_EN);
	irq_enable();
    adc_init(DMA_M_CHN);
	#if (ADC_M_CHN_SAMPLE_MODE == ADC_GPIO_SAMPLE)
	adc_gpio_sample_init(ADC_M_CHANNEL, adc_gpio_cfg_m);
	#elif (ADC_M_CHN_SAMPLE_MODE == ADC_VBAT_SAMPLE)
	adc_vbat_sample_init(ADC_M_CHANNEL);
	#endif
#endif

    adc_power_on();
#if (ADC_MODE == ADC_NDMA_MODE)
    adc_start_sample_nodma();
#elif (ADC_MODE == ADC_DMA_MODE)
    adc_start_sample_dma();
#endif
}

void main_loop(void)
{
#if (ADC_MODE == ADC_NDMA_MODE)
    adc_m_chn_val = adc_get_result(NDMA, ADC_M_CHANNEL);
    sleep_ms(500);
    gpio_toggle(LED1);
#elif (ADC_MODE == ADC_DMA_MODE)
    if(sd_adc_rx_done_flag == 1)
    {
        /******get adc sample data and sort these data ********/
    	/**
    	 * Note: During the FPGA stage, the following code should not be used yet, otherwise all data will become 0.
    	 * It can only be used on a real chip.
    	 */
//        for (int i = 1; i < ((ADC_SAMPLE_GROUP_CNT+2) * ADC_SAMPLE_CHN_CNT); i++) {
//            if (adc_sample_buffer[i] & BIT(9)) {  //10 bit resolution, BIT(9) is sign bit, 1 means negative voltage in differential_mode
//                adc_sample_buffer[i] = 0;
//            } else {
//                adc_sample_buffer[i] = (adc_sample_buffer[i] & 0x1ff); //BIT(8..0) is valid adc code
//            }
//        }
    	adc_code_split_dma((unsigned short *)(adc_sample_buffer+2), ADC_SAMPLE_GROUP_CNT, ADC_SAMPLE_CHN_CNT, channel_buffers);
    	adc_m_chn_val = adc_get_result(DMA, ADC_M_CHANNEL);
        sd_adc_rx_done_flag=0;
        sleep_ms(500);
        gpio_toggle(LED1);
        adc_start_sample_dma();
    }

#endif
}


/**
 * @brief This function serves to sort adc sample code and get average value.
 * @param[in]   channel_sample_buffer - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *              and the actual buffer size defined by the user needs to be not smaller than the sample_num, otherwise there may be an out-of-bounds problem.
 * @return      adc_code_average    - the average value of adc sample code.
 */
unsigned short adc_sort_and_get_average_code(unsigned short *channel_sample_buffer)
{
    int            i, j;
    unsigned short adc_code_average = 0;
    unsigned short temp;

    /**** insert Sort and get average value ******/
    for (i = 1; i < ADC_SAMPLE_GROUP_CNT; i++) {
        if (channel_sample_buffer[i] < channel_sample_buffer[i - 1]) {
            temp                     = channel_sample_buffer[i];
            channel_sample_buffer[i] = channel_sample_buffer[i - 1];
            for (j = i - 1; j >= 0 && channel_sample_buffer[j] > temp; j--) {
                channel_sample_buffer[j + 1] = channel_sample_buffer[j];
            }
            channel_sample_buffer[j + 1] = temp;
        }
    }
    //get average value from raw data(abandon 1/4 small and 1/4 big data)
    for (i = ADC_SAMPLE_GROUP_CNT >> 2; i < (ADC_SAMPLE_GROUP_CNT - (ADC_SAMPLE_GROUP_CNT >> 2)); i++) {
        adc_code_average += channel_sample_buffer[i] / (ADC_SAMPLE_GROUP_CNT >> 1);
    }
    return adc_code_average;
}

/**
 * @brief      This function serves to convert to voltage value and temperature value.
 * @param[in]  transfer_mode -enum variable of adc code transfer mode.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @return     adc_result   - adc voltage value or temperature value.
 */
unsigned short adc_get_result(adc_transfer_mode_e transfer_mode, adc_sample_chn_e chn)
{
    unsigned short code_average;
    unsigned short adc_result;
    unsigned int   cnt = 0;

    if (transfer_mode == NDMA) {
        adc_start_sample_nodma();

        while (cnt < ADC_SAMPLE_GROUP_CNT) {
            int sample_cnt = adc_get_rxfifo_cnt();
            if (sample_cnt > 0) {

                adc_data =  adc_get_raw_code();
                channel_buffers[chn][cnt]= adc_data & 0xffff;
                channel_buffers[chn][cnt+1]= adc_data >> 16;
            	/**
            	 * Note: During the FPGA stage, the following code should not be used yet, otherwise all data will become 0.
            	 * It can only be used on a real chip.
            	 */
//                if (channel_buffers[chn][cnt] & BIT(9)) { //10 bit resolution, BIT(9) is sign bit, 1 means negative voltage in differential_mode
//                    channel_buffers[chn][cnt] = 0;
//                } else {
//                    channel_buffers[chn][cnt] &= 0x1FF;    //BIT(8..0) is valid adc code
//                }
                cnt +=2;
            }
        }
    }
    code_average = adc_sort_and_get_average_code(channel_buffers[chn]);
    return adc_result = adc_calculate_voltage(chn, code_average);
}


#if (ADC_MODE == ADC_DMA_MODE)
/**
 * @brief       This function serves to split the data from all channels in the sample buffer into different channels.
 * @param[in]   sample_buffer - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                              and the actual buffer size defined by the user needs to be not smaller than the sample_num, otherwise there may be an out-of-bounds problem.
 * @param[in]   sample_num    - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @param[in]   chn_cnt -number of channels used.
 * @param[in]   buffers -This parameter is the first address of ADC sample channel buffers, which must be 4 bytes aligned, otherwise the program will enter an exception.
 * @return      none
 * @note        Since the write-back feature of ADC's DMA5 is enabled by default and cannot be disabled, and the length information occupies 1 word,
 *              the address of sample_buffer needs to be offset by 1 word.
 */
void adc_code_split_dma(unsigned short *sample_buffer, unsigned int sample_num, unsigned char chn_cnt, unsigned short buffers[chn_cnt][sample_num])
{
    unsigned int i, j;
    for (i = 0; i < chn_cnt; i++) {
        for (j = 0; j < sample_num; j++) {
            buffers[i][j] = sample_buffer[j * chn_cnt + i];
        }
    }
}
#endif


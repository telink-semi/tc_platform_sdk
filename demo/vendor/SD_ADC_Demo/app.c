/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#if (DEMO_MODE == NORMAL_MODE)
sd_adc_gpio_cfg_t sd_adc_gpio_cfg =
{
     .input_p          = SD_ADC_GPIO_PB0P,
     .input_n          = SD_ADC_GNDN,
     .p_div            = SD_ADC_GPIO_P_CHN_DIV_1F4,
     .n_div            = SD_ADC_GPIO_N_CHN_DIV_1F4,
	 .clk_div          = SD_ADC_SAMPLE_CLOCK_DIV,
     .downsample_rate  = SD_ADC_DOWN_SAMPLE_RATE,
};

volatile unsigned char flag=0;
volatile unsigned int sample_times=0;
signed int sd_adc_vol_10x = 0;
volatile signed int sd_adc_vol = 0;
volatile signed int temp_value = 0;
/*
 *  The length of sd_adc_sample_buffer must be >= SD_ADC_FIFO_DEPTH, otherwise there is a risk of array overflow.
 */
signed int sd_adc_sample_buffer[SD_ADC_FIFO_DEPTH] __attribute__((aligned(4))) = {0};

signed int sd_adc_sort_and_get_average_code(signed int *sample_buffer);
signed int sd_adc_get_result(sd_adc_result_type_e result_type);

void user_init(void)
{
	gpio_set_func(LED3,AS_GPIO);
	gpio_set_output_en(LED3, 1);
	gpio_set_input_en(LED3,0);
	gpio_write(LED3, 0);

    sd_adc_init();
#if(SD_ADC_MODE==SD_ADC_GPIO_MODE)
    sd_adc_gpio_sample_init(&sd_adc_gpio_cfg);
#elif(SD_ADC_MODE==SD_ADC_VBAT_MODE)
    sd_adc_vbat_sample_init(SD_ADC_SAMPLE_CLOCK_DIV, SD_ADC_VBAT_DIV_1F4, SD_ADC_DOWN_SAMPLE_RATE);
#elif(SD_ADC_MODE==SD_ADC_TEMP_MODE)
    sd_adc_temp_init(SD_ADC_SAMPLE_CLOCK_DIV, SD_ADC_DOWN_SAMPLE_RATE );
#endif

    sd_adc_set_rx_fifo((unsigned int *)sd_adc_sample_buffer, SD_ADC_FIFO_DEPTH);
    sd_adc_set_irq_trig_thres(SD_ADC_SAMPLE_CNT);

#if(SAMPLE_MODE==INTERRUPT_MODE)
    sd_adc_set_irq_mask();
    irq_set_mask(FLD_IRQ_DAM_FIFO_EN);
    irq_enable();
#endif
    /**
     * @attention
     * -# After sd_adc_power_on(SD_ADC_SAMPLE_MODE), must wait >160us(when the C10 capacitor on the development board is 10nF) for VMID power to stabilize, otherwise there will be hundreds of sample data will be abnormal.
     * -# After each call to sd_adc_power_on(SD_ADC_SAMPLE_MODE) and wait >160us(when the C10 capacitor on the development board is 10nF), the first 4 codes sampled by sd_adc may be abnormal and need to be discarded.
     */
    sd_adc_power_on(SD_ADC_SAMPLE_MODE);
    sleep_us(160);
    sd_adc_sample_start();
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
#if(SAMPLE_MODE == POLLING_MODE)

	while(sd_adc_get_irq_status())
	{
		sd_adc_sample_stop();//Stop sampling to prevent sd_adc_sample_buffer data from being overwritten while data is being processed.
#if(SD_ADC_MODE==SD_ADC_GPIO_MODE || SD_ADC_MODE==SD_ADC_VBAT_MODE)
        sd_adc_vol_10x = sd_adc_get_result(SD_ADC_VOLTAGE_10X_MV);
        sd_adc_vol = sd_adc_vol_10x / 10;
        printf("vol = %d.%d mv \n",(sd_adc_vol_10x / 10),((unsigned int)sd_adc_vol_10x % 10));
#elif(SD_ADC_MODE==SD_ADC_TEMP_MODE)
        temp_value = sd_adc_get_result(TEMP_VALUE);
        printf("temp = %d \n",temp_value);
#endif
        sleep_ms(500);
		sd_adc_sample_start();
	}

#elif(SAMPLE_MODE == INTERRUPT_MODE)
    if(flag == 1)
    {
#if(SD_ADC_MODE==SD_ADC_GPIO_MODE || SD_ADC_MODE==SD_ADC_VBAT_MODE)
        sd_adc_vol_10x = sd_adc_get_result(SD_ADC_VOLTAGE_10X_MV);
        sd_adc_vol = sd_adc_vol_10x / 10;
        printf("vol = %d.%d mv\n",(sd_adc_vol_10x / 10),((unsigned int)sd_adc_vol_10x % 10));
#elif(SD_ADC_MODE==SD_ADC_TEMP_MODE)
        temp_value = sd_adc_get_result(TEMP_VALUE);
        printf("temp = %d \n",temp_value);
#endif
        flag=0;
        sample_times++;

        sleep_ms(500);
        sd_adc_sample_start();
    }

#endif


}

/**
 * @brief       This function serves to sort and get average code.
 * @param[in]   sd_adc_data_buf -Pointer to sd_adc_data_buf
 * @return      average code
 * @note        If sd_adc_sort_and_get_average_code() interface is called, SD_ADC_SAMPLE_CNT must be a multiple of 4.
 */
signed int sd_adc_sort_and_get_average_code(signed int *sample_buffer)
{
    int i, j;
    signed int sd_adc_code_average = 0;
    signed int temp;

    /**** insert Sort and get average value ******/
    for(i = 1 ;i < SD_ADC_SAMPLE_CNT; i++)
    {
        if(sample_buffer[i] < sample_buffer[i-1])
        {
            temp = sample_buffer[i];
            sample_buffer[i] = sample_buffer[i-1];
            for(j=i-1; j>=0 && sample_buffer[j] > temp;j--)
            {
                sample_buffer[j+1] = sample_buffer[j];
            }
            sample_buffer[j+1] = temp;
        }
    }
    //get average value from raw data(abandon 1/4 small and 1/4 big data)
    for (i = SD_ADC_SAMPLE_CNT>>2; i < (SD_ADC_SAMPLE_CNT - (SD_ADC_SAMPLE_CNT>>2)); i++)
    {
        sd_adc_code_average += (float)sample_buffer[i]/(float)(SD_ADC_SAMPLE_CNT>>1);
    }
    return sd_adc_code_average;
}

/**
 * @brief       This function serves to get voltage or temperature value.
 * @param[in]   result_type -sd_adc_result_type_e
 * @return      voltage or temperature value
 * @note        The A0 version of the chip temperature sensor is no longer open for use.
 */
signed int sd_adc_get_result(sd_adc_result_type_e result_type)
{
    signed int code_average;
    signed int sd_adc_result;

    code_average = sd_adc_sort_and_get_average_code(sd_adc_sample_buffer);

    if(result_type == SD_ADC_VOLTAGE_10X_MV)
    {
        return sd_adc_result = sd_adc_calculate_voltage(code_average,result_type);
    }
    else if(result_type == SD_ADC_VOLTAGE_MV)
    {
        return sd_adc_result = sd_adc_calculate_voltage(code_average,result_type);
    }
    else if((result_type == TEMP_VALUE))
    {
        return sd_adc_result = sd_adc_calculate_temperature(code_average);
    }
    else
    {
        return 0;
    }
}
#endif

/********************************************************************************************************
 * @file    app_audio_and_adc.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#if (AUDIO_MODE <= AMIC_INPUT_TO_BUF) && (AUDIO_AND_ADC_SAMPLE_MODE == AUDIO_DEMO_MODE)

#define AUDIO_BUFF_SIZE 512*4
#define ADC_SAMPLE_GROUP_CNT   8
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1] __attribute__((aligned(4)));

signed short adc_sample_buf[ADC_SAMPLE_GROUP_CNT] __attribute__((aligned(4)));

#define SAMPLE_RATE AUDIO_16K

volatile unsigned int adc_l_chn_val = 0;
/*
ATTENTION:
#1.In the concurrent use mode of audio and ADC, the audio must use PB0 and PB1. Therefore, the ADC can only use the remaining PA4, PA5, and PA6 for GPIO sampling. 
#2.The ADC sample frequency must using ADC_SAMPLE_FREQ_96K_FOR_AUDIO_AND_ADC when using the concurrent use mode of audio and ADC.
*/
adc_gpio_cfg_t adc_gpio_cfg_l =
{
    .v_ref       = ADC_VREF_1P2V,
    .pre_scale   = ADC_PRESCALE_1F4,
    .sample_freq = ADC_SAMPLE_FREQ_96K_FOR_AUDIO_AND_ADC,
    .pin         = GPIO_L_CHN_SAMPLE_PIN,
};

unsigned short adc_get_result_for_audio_and_adc_mode(void)
{
    uint32_t adc_sum = 0, adc_average= 0;
    unsigned short temp;
    int i,j;
    signed short adc_sample[ADC_SAMPLE_GROUP_CNT];
    /* dfifo disable, and clear buf */
    adc_disable_dfifo1();
    memset(adc_sample_buf, 0, sizeof(adc_sample_buf));
    /* dfifo setting will lose in suspend/deep, so we need config it every time size: ADC_SAMPLE_GROUP_CNT*2*/
    adc_config_misc_channel_buf((unsigned short *)adc_sample_buf, ADC_SAMPLE_GROUP_CNT<<1);
    adc_enable_dfifo1();
    unsigned int t0 = clock_time();
    /* wait at least 2 sample cycle(f = 96K, T = 10.4us) */
    while(!clock_time_exceed(t0, 23));
    /* get adc sample data and sort these data , DFIFO transfers 32-bit data each time, corresponding to 2 sets of ADC data.*/
    for(i=0;i<ADC_SAMPLE_GROUP_CNT;i += 2){
        /*wait at least 2 sample cycle(f = 96K, T = 10.4us),then get 2 sets of ADC data*/
        while(!clock_time_exceed(t0, 23));
        t0 = clock_time();
        /* 10 bit resolution, BIT(9) is sign bit, 1 means negative voltage in differential_mode */
        if(adc_sample_buf[i] & BIT(9)){  
            adc_sample[i] = 0;
        }
        else{
            /* BIT(9..0) is valid adc result */
            adc_sample[i] = ((unsigned short)adc_sample_buf[i] & 0x3FF);
        }
        if(adc_sample_buf[i+1] & BIT(9)){  
            adc_sample[i+1] = 0;
        }
        else{
            /* BIT(9..0) is valid adc result */
            adc_sample[i+1] = ((unsigned short)adc_sample_buf[i+1] & 0x3FF);
        }
    }
    /* disable adc fifo */
    adc_disable_dfifo1();
    /* get average value from raw data(abandon some small and big data ), then filter with history data. */
    for (i = 1; i < ADC_SAMPLE_GROUP_CNT; i++) {
        if (adc_sample[i] < adc_sample[i - 1]) {
            temp = adc_sample[i];
            adc_sample[i] = adc_sample[i - 1];
            for (j = i - 1; j >= 0 && adc_sample[j] > temp; j--) {
                adc_sample[j + 1] = adc_sample[j];
            }
            adc_sample[j + 1] = temp;
        }
     }
     /* get average value from raw data(abandon 1/4 small and 1/4 big data) */
     for (i = ADC_SAMPLE_GROUP_CNT >> 2; i < (ADC_SAMPLE_GROUP_CNT - (ADC_SAMPLE_GROUP_CNT >> 2)); i++) {
         adc_sum += (uint32_t)adc_sample[i];
     }
     adc_average = adc_sum / (ADC_SAMPLE_GROUP_CNT >> 1);

    /*When the code value is 0, the returned voltage value should be 0.*/
    if(adc_average == 0){
        return 0;
    }
    unsigned short adc_voltage = adc_calculate_voltage(ADC_L_CHANNEL, adc_average);
    if(adc_voltage & BIT(15)) {
        /*When the adc_voltage < 0, the returned voltage value should be 0.*/
        return 0;
    }else {
        return adc_voltage;
    }
}

#define DATA_WIDTH CODEC_BIT_16_DATA

#if (AUDIO_MODE == LINE_INPUT_TO_BUF)
#define INPUT_SRC LINE_STREAM0_MONO_L

#elif (AUDIO_MODE == AMIC_INPUT_TO_BUF)
#define INPUT_SRC AMIC_STREAM0_MONO_L
#endif

audio_codec_stream0_input_t audio_codec_stream0_input =
{
    .input_src = INPUT_SRC,
    .sample_rate = SAMPLE_RATE,
    .data_width = DATA_WIDTH,
    .fifo_chn = FIFO0,
    .data_buf = AUDIO_BUFF,
    .data_buf_size = sizeof(AUDIO_BUFF),
};

void user_init(void)
{
    audio_set_amic_pga_pin();
#if ((AUDIO_MODE == LINE_INPUT_TO_BUF) || (AUDIO_MODE == AMIC_INPUT_TO_BUF))
#if (AUDIO_MODE == AMIC_INPUT_TO_BUF)
    audio_set_amic_bias_pin(GPIO_PA6);
#endif
    audio_dfifo_config(FIFO0,(unsigned short* )AUDIO_BUFF,sizeof(AUDIO_BUFF));
    /****stream0 line in/amic init****/
    audio_codec_stream0_input_init(&audio_codec_stream0_input);
#endif

    /* set audio m chn sample rate */
    audio_set_sample_rate_audio_and_adc_mode();

#if (ADC_SAMPLE_MODE == ADC_VBAT_SAMPLE)
    /* config adc vbat mode, must use ADC_L_CHANNEL */
    adc_vbat_sample_init_for_audio_and_adc_mode(ADC_L_CHANNEL);
#elif (ADC_SAMPLE_MODE == ADC_GPIO_SAMPLE)
    /* config adc gpio mode, must use ADC_L_CHANNEL */
    adc_gpio_sample_init(ADC_L_CHANNEL, adc_gpio_cfg_l);
#endif
    adc_dfifo1_config((unsigned short* )adc_sample_buf,sizeof(adc_sample_buf));

    /* power on adc and audio */
    audio_and_adc_power_on();
}

void main_loop(void)
{
    //audio_and_adc_power_on();
    adc_l_chn_val =adc_get_result_for_audio_and_adc_mode();
    printf("adc_l_chn_val %d\r\n", adc_l_chn_val);
    sleep_ms(500);
    //audio_and_adc_power_down();

}

#endif

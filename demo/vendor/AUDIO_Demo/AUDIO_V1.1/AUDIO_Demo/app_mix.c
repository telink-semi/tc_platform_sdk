/********************************************************************************************************
 * @file    app_mix.c
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
#if((AUDIO_MODE == LINE_INPUT_TO_BUF_TO_I2S) || (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_I2S) || (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_I2S))
#include "audio_common.h"
#define    AUDIO_BUFF_SIZE  4096
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE>>1] __attribute__((aligned(4)));

/**
 *                                       i2s_clk_config[2]-->lrclk_adc(sampling rate)
 *                                              ||                  ||
 *  pll(48M default)------->div---->i2s_clk--->blck--------------->div(32)
 *                           ||
 *           i2s_clk_config[0]/i2s_clk_config[1]
 * For example:sampling rate=48K,i2s_clk_config[3]={1, 48, 0},sampling rate=240M*(4/1875)/32=16K
 *
 */

unsigned short  audio_i2s_48k_config[3]   ={16, 250, 0}; // 48M * 16 / 250 / (2 * (0 + 1)) / 32 = 48K
unsigned short  audio_i2s_44P1k_config[3] ={10, 170, 0}; // 48M * 10 / 170 / (2 * (0 + 1)) / 32 = 44.118K
unsigned short  audio_i2s_32k_config[3]   ={32, 250, 2}; // 48M * 32 / 250 / (2 * (2 + 1)) / 32 = 32K
unsigned short  audio_i2s_24k_config[3]   ={16, 250, 1}; // 48M * 16 / 250 / (2 * (1 + 1)) / 32 = 24K
unsigned short  audio_i2s_22P0k_config[3] ={10, 170, 1}; // 48M * 10 / 170 / (2 * (1 + 1)) / 32 = 22.0588K
unsigned short  audio_i2s_16k_config[3]   ={16, 250, 2}; // 48M * 16 / 250 / (2 * (2 + 1)) / 32 = 16K
unsigned short  audio_i2s_12k_config[3]   ={16, 250, 3}; // 48M * 16 / 250 / (2 * (3 + 1)) / 32 = 12K
unsigned short  audio_i2s_11P0k_config[3] ={10, 170, 3}; // 48M * 10 / 170 / (2 * (3 + 1)) / 32 = 11.0294K
unsigned short  audio_i2s_8k_config[3]    ={16, 250, 5}; // 48M * 16 / 250 / (2 * (5 + 1)) / 32 = 8K


#define SAMPLE_RATE AUDIO_48K
#define DATA_WIDTH CODEC_BIT_16_DATA
#define RX_FIFO_NUM FIFO0
#define TX_FIFO_NUM FIFO0// TX Hardware is fixed to FIFO0 and cannot be modified.

#if (AUDIO_MODE == LINE_INPUT_TO_BUF_TO_I2S)
#define INPUT_SRC LINE_STREAM0_MONO_L
#define OUTPUT_SRC I2S_MONO_L
#elif (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_I2S)
#define INPUT_SRC AMIC_STREAM0_MONO_L
#define OUTPUT_SRC I2S_MONO_L
#elif (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_I2S)
#define INPUT_SRC DMIC_STREAM0_STEREO
#define OUTPUT_SRC I2S_STEREO
#endif

audio_codec_stream0_input_t audio_codec_stream0_input =
{
    .input_src = INPUT_SRC,
    .sample_rate = SAMPLE_RATE,
    .data_width = DATA_WIDTH,
    .fifo_chn = RX_FIFO_NUM,
    .data_buf = AUDIO_BUFF,
    .data_buf_size = sizeof(AUDIO_BUFF),
};

void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_output_en(LED1, 1);
    gpio_set_input_en(LED1, 0);
    audio_init();
#if((AUDIO_MODE == LINE_INPUT_TO_BUF_TO_I2S) || (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_I2S) || (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_I2S))
    i2s_pin_config_t i2s_pin_config = {
        .bclk_pin           = GPIO_PD2,
        .adc_dac_lr_clk_pin = GPIO_PD1,
        .adc_dat_pin        = GPIO_NONE_PIN,
        .dac_dat_pin        = GPIO_PE0,
    };
    audio_i2s_config_t audio_i2s_config =
    {
        .pin_config = &i2s_pin_config,
        .data_width = I2S_BIT_16_DATA,
        .sample_rate = (unsigned short *)&audio_i2s_48k_config,
    };
    audio_i2s_input_output_t audio_i2s_output =
    {
        .data_width = audio_i2s_config.data_width,
        .fifo_chn = TX_FIFO_NUM, //only support fifo0
        .i2s_ch = OUTPUT_SRC,
        .data_buf = AUDIO_BUFF,
        .data_buf_size = sizeof(AUDIO_BUFF),
    };
    audio_dfifo_config(FIFO0,(unsigned short* )AUDIO_BUFF,sizeof(AUDIO_BUFF));
    /****stream0 line in/amic/dmic init****/
    audio_codec_stream0_input_init(&audio_codec_stream0_input);
    audio_set_codec_en(1);
#if (AUDIO_MODE== DMIC_INPUT_TO_BUF_TO_I2S)
    audio_set_stream0_dmic_pin(DMIC_DATA_PA5,DMIC_CLK_PA6,DMIC_CLK_PB5);
    audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_m6_DB);
#elif (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_I2S)
    audio_set_amic_bias_pin(GPIO_PC5);
#endif
    /**** i2s config init ****/
    audio_i2s_config_init(&audio_i2s_config);
    /*** i2s output init ***/
    audio_i2s_output_init(&audio_i2s_output);
#endif
}

void main_loop (void)
{
    gpio_toggle(LED1);
    sleep_ms(200);
}

#endif

/********************************************************************************************************
 * @file    app_codec.c
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
#if (AUDIO_MODE < I2S_TO_I2S)
#include "audio_common.h"

#define AUDIO_BUFF_SIZE 480*8
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1] __attribute__((aligned(4)));
sdm_pin_config_t sdm_pin_config = {
    .sdm0_p_pin = GPIO_PD7,
    .sdm0_n_pin = GPIO_PD6,
    .sdm1_p_pin = GPIO_PD5,
    .sdm1_n_pin = GPIO_PD3,
};

#if ((AUDIO_MODE == LINE_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_LINEOUT))

#define SAMPLE_RATE AUDIO_48K
#define DATA_WIDTH CODEC_BIT_16_DATA
#define RX_FIFO_NUM FIFO0
#define TX_FIFO_NUM FIFO0// TX Hardware is fixed to FIFO0 and cannot be modified.

#if (AUDIO_MODE == LINE_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC LINE_STREAM0_MONO_L
#define OUTPUT_SRC SDM_MONO

#elif (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC AMIC_STREAM0_MONO_L
#define OUTPUT_SRC SDM_MONO

#elif (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC DMIC_STREAM0_STEREO
#define OUTPUT_SRC SDM_STEREO
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
audio_codec_output_t audio_stream_output =
{
    .output_src = OUTPUT_SRC,
    .sample_rate = SAMPLE_RATE,
    .data_buf = AUDIO_BUFF,
    .data_buf_size = sizeof(AUDIO_BUFF),
};
#elif (AUDIO_MODE == BUFF_TO_LINEOUT)
#define INPUT_SRC DMIC_STREAM0_STEREO
#define OUTPUT_SRC SDM_MONO
#define SAMPLE_RATE AUDIO_48K
#define DATA_WIDTH CODEC_BIT_16_DATA
#define RX_FIFO_NUM FIFO0
#define TX_FIFO_NUM FIFO0// TX Hardware is fixed to FIFO0 and cannot be modified.
void* buff;
audio_codec_output_t audio_stream_output =
{
    .output_src = OUTPUT_SRC,
    .sample_rate = SAMPLE_RATE,
    .data_buf = AUDIO_BUFF,
    .data_buf_size = sizeof(AUDIO_BUFF),
};
#endif

void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_output_en(LED1, 1);
    gpio_set_input_en(LED1, 0);

    audio_init();
#if ((AUDIO_MODE == LINE_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_LINEOUT))
#if (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT)
    audio_set_amic_bias_pin(GPIO_PC5);
#elif (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_LINEOUT)
    /****setting up the dmic's multiplexed pins****/
    audio_set_stream0_dmic_pin(DMIC_DATA_PA5,DMIC_CLK_PA6,DMIC_CLK_PB5);
#endif
    audio_dfifo_config(FIFO0,(unsigned short* )AUDIO_BUFF,sizeof(AUDIO_BUFF));
    /****stream0 line in/amic/dmic init****/
    audio_codec_stream0_input_init(&audio_codec_stream0_input);
    /****line output init****/
    audio_codec_stream_output_init(&audio_stream_output);

    /****audio starts run****/
    #if (AUDIO_CLR_CODEC_POP == 1)
    audio_set_mute_mic(1);                                             /* Step1 - mute audio*/
    #endif
    #if (AUDIO_CLR_CODEC_POP == 1)
    audio_codec_clr_input_pop(20);                                     /* Step2 - Clear codec input pop and dis mute audio */
    #endif
    audio_set_codec_en(1);                                             /* Step3 - enable codec, codec data come in */

    #if (AUDIO_CODEC_FADE_IN == 1)
    /* Collect enough codec data and make it fade in.
     * A delay of 5ms is used here in order to allow the codec to generate enough data for the fade-in process.
     * (t_end - t_start) : fade-in time.
    */
    /* start end: fade-in process */
    unsigned short t_start = 0;
    unsigned short t_end = 5;
    sleep_ms(t_end);
    audio_linear_fade_in_config(AUDIO_MONO, audio_codec_stream0_input.data_width, audio_codec_stream0_input.sample_rate, (char *)audio_codec_stream0_input.data_buf, t_start, t_end);
    #endif

#if (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT)
    audio_stream0_fade_dig_gain(CODEC_IN_D_GAIN_12_DB);
#endif
    audio_set_sdm_pin(&sdm_pin_config);

#elif (AUDIO_MODE == BUFF_TO_LINEOUT)
    irq_enable();//enable global interrupt
    timer1_set_mode(TIMER_MODE_SYSCLK,0,CLOCK_SYS_CLOCK_1MS);
    audio_dfifo_config(FIFO0,(unsigned short* )AUDIO_BUFF,sizeof(AUDIO_BUFF));
    /****line output init****/
    audio_set_sdm_pin(&sdm_pin_config);
    audio_codec_stream_output_init(&audio_stream_output);
    buff = (unsigned short* )sin_48k_d1;
    timer_start(TIMER1);//trigger timer.
#endif
}

void main_loop(void)
{
    gpio_toggle(LED1);
    sleep_ms(200);
#if defined(AUDIO_CODEC_POWER_TEST)
    sleep_ms(5000);
    {/* audio off */
        audio_stream0_fade_dig_gain(CODEC_IN_D_GAIN_m48_DB);
        audio_unset_sdm_pin(&sdm_pin_config);
        audio_set_codec_en(0);
#if (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT)
        audio_codec_adc_power_down();
#endif
        audio_power_down();
    }
    sleep_ms(2000);
    {/* audio power on and reinit */
        audio_init();
        audio_dfifo_config(FIFO0,(unsigned short* )AUDIO_BUFF,sizeof(AUDIO_BUFF));
        /****stream0 line in/amic/dmic init****/
        audio_codec_stream0_input_init(&audio_codec_stream0_input);
        audio_set_mute_mic(1);                                             /* Step1 - mute audio*/
        audio_codec_clr_input_pop(20);                                     /* Step2 - Clear codec input pop and dis mute audio */
        audio_set_codec_en(1);                                             /* Step3 - enable codec, codec data come in */
#if (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT)
        audio_stream0_fade_dig_gain(CODEC_IN_D_GAIN_12_DB);
#endif
        audio_set_sdm_pin(&sdm_pin_config);
        /****line output init****/
        audio_codec_stream_output_init(&audio_stream_output);
    }
#endif
}

#endif

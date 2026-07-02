/********************************************************************************************************
 * @file    app.c
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
#if (AUDIO_MODE <=  AMIC_INPUT_TO_BUF)

#define AUDIO_BUFF_SIZE 512*4
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1] __attribute__((aligned(4)));

#define SAMPLE_RATE AUDIO_16K
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
}

void main_loop(void)
{
}

#endif

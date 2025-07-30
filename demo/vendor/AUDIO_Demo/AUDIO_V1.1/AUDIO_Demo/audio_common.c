/********************************************************************************************************
 * @file    audio_common.c
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

extern unsigned int audio_sample_rate_value[AUDIO_RATE_SIZE];

/**
 * @brief       This function serves to configure audio data fade in
 * @param[in]   source          - audio channel select.
 * @param[in]   data_width      - data bit width.
 * @param[in]   rate            - select channel sample rate.
 * @param[in]   audio_data      - audio data buff.
 * @param[in]   t_start         - start time of audio fade in.
 * @param[in]   t_end           - end time of audio fade in.
 * @return      none.
 */
void audio_linear_fade_in_config(audio_channel_select_e source, audio_codec_wl_mode_e data_width, audio_sample_rate_e rate, char *audio_data, unsigned short t_start, unsigned short t_end)
{
    /* Do not change these variables to unsigned int or an arithmetic error will occur
     * because signed short data will expand to an unsigned int type, which will produce undesired value when that data is negative.
     */
    short index;
    short audio_start_idx = audio_sample_rate_value[rate] / 1000 * t_start;
    short audio_end_idx   = audio_sample_rate_value[rate] / 1000 * t_end;

    if (source == AUDIO_STEREO && data_width == CODEC_BIT_16_DATA) /* stereo 16bit*/
    {
        for (index = audio_start_idx; index < audio_end_idx; index++) {
            *(short *)(audio_data + index * 4)     = *(short *)(audio_data + index * 4) * (index - audio_start_idx) / (audio_end_idx - audio_start_idx);
            *(short *)(audio_data + index * 4 + 2) = *(short *)(audio_data + index * 4);
        }
    } else if (source == AUDIO_STEREO && data_width == CODEC_BIT_20_DATA) /* stereo 20bit*/
    {
        for (index = audio_start_idx; index < audio_end_idx; index++) {
            *(int *)(audio_data + index * 8)     = *(int *)(audio_data + index * 8) * (index - audio_start_idx) / (audio_end_idx - audio_start_idx);
            *(int *)(audio_data + index * 8 + 4) = *(int *)(audio_data + index * 8);
        }
    } else if (data_width == CODEC_BIT_16_DATA) /* mono 16bit*/
    {
        for (index = audio_start_idx; index < audio_end_idx; index++) {
            *(short *)(audio_data + index * 2) = *(short *)(audio_data + index * 2) * (index - audio_start_idx) / (audio_end_idx - audio_start_idx);
        }
    } else /* mono 20bit*/
    {
        for (index = audio_start_idx; index < audio_end_idx; index++) {
            *(int *)(audio_data + index * 4) = *(int *)(audio_data + index * 4) * (index - audio_start_idx) / (audio_end_idx - audio_start_idx);
        }
    }
}

/**
 * @brief       This function serves to fade audio pga gain
 * @param[in]   gain          - audio channel select.
 * @return      none.
 */
void audio_fade_pga_gain(codec_in_pga_gain_e gain)
{
    codec_in_pga_gain_e value = audio_get_adc_pga_gain();

    if (gain > value) {
        for (value++; (signed int)value != (signed int)gain; value++) {
            sleep_ms(1);
            audio_set_adc_pga_gain(value);
        }
    } else if (gain < value) {
        for (value--; (signed int)value != (signed int)gain; value--) {
            sleep_ms(1);
            audio_set_adc_pga_gain(value);
        }
    }
}

/**
 * @brief       This function serves to fade stream digital gain
 * @param[in]   gain          - audio channel select.
 * @return      none.
 */
void audio_stream0_fade_dig_gain(codec_in_path_digital_gain_e gain)
{
    codec_in_path_digital_gain_e value = audio_get_stream0_dig_gain();

    if (gain > value) {
        for (value += 4; (signed int)value != (signed int)gain; value += 4) {
            sleep_ms(1);
            audio_set_stream0_dig_gain(value);
        }
    } else if (gain < value) {
        for (value -= 4; (signed int)value != (signed int)gain; value -= 4) {
            sleep_ms(1);
            audio_set_stream0_dig_gain(value);
        }
    }
}

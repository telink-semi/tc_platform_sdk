/********************************************************************************************************
 * @file    audio.c
 *
 * @brief   This is the source file for TC123X
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
#include "audio.h"

/**
 * @brief     This function serves to configure amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_set_amic_bias_pin(GPIO_PinTypeDef amic_bias)
{
    gpio_set_func(amic_bias, AS_GPIO);
    gpio_set_output_en(amic_bias, 1);
    gpio_set_input_en(amic_bias, 0);
    gpio_write(amic_bias, 1);
}

/**
 * @brief     This function serves to unset amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_unset_amic_bias_pin(GPIO_PinTypeDef amic_bias)
{
    gpio_set_func(amic_bias, AS_GPIO);
    gpio_set_output_en(amic_bias, 1);
    gpio_set_input_en(amic_bias, 0);
    gpio_write(amic_bias, 0);
}

/**
 * @brief     This function serves to configure amic input pins.
 * @return    none.
 * @note      This function only supports PB0 (positive) and PB1 (negative).
 */
void audio_set_amic_pga_pin(void)
{
    write_reg8(ADC_BASE_ADDR, 0x66);
}

/**
 * @brief      This function serves to configure DFIFO0 for audio data input.
 * @param[in]  fifo_chn  - fifo channel.
 * @param[in]  pbuff     - address in DFIFO0 or DFIFO1.
 * @param[in]  size_buff - depth of DFIFO0 or DFIFO1.
 * @return     none.
 */
void audio_dfifo_config(audio_fifo_chn_e fifo_chn, unsigned short* pbuff, unsigned int size_buff)
{
    if (fifo_chn == FIFO0)
    {
        reg_dfifo0_addr = (unsigned short)((unsigned int)pbuff);
        reg_dfifo0_size = (size_buff >> 4) - 1;
    }
}

/**
 * @brief      This function serves to set adc power mode.
 * @param[in]  mode - power mode. bit[0] for PGA. The default value is 0 for optimal performance and high power consumption.
 *                    0: high power; 1: low power.
 * @return     none.
 */
void audio_codec_set_adc_power_mode(unsigned char mode)
{
    analog_write(areg_pga_itrim, (analog_read(areg_pga_itrim) & ~FLD_PGA_CAPTRIM) | MASK_VAL(FLD_PGA_CAPTRIM, mode));
}

/**
 * @brief     This function serves to set codec_adc 4M clock.
 * @return    none.
 */
void audio_codec_set_adc_clock(void)
{
    adc_set_clk();
    adc_clk_en();
    reg_adc_config2 |= FLD_CLK_EN;
}

/**
 * @brief      This function serves to power on codec_adc.
 * @return     none.
 */
void audio_codec_adc_power_on(void)
{
    analog_write(areg_adc_pga_ctrl, (analog_read(areg_adc_pga_ctrl) & (~FLD_SAR_ADC_POWER_DOWN)));
}

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none.
 */
void audio_codec_adc_power_down(void)
{
    analog_write(areg_adc_pga_ctrl, (analog_read(areg_adc_pga_ctrl) | (FLD_SAR_ADC_POWER_DOWN)));
}

/**
 * @brief      This function serves to power on codec pga.
 * @return     none.
 */
void audio_codec_pga_power_on(void)
{
    analog_write(areg_adc_pga_ctrl, (analog_read(areg_adc_pga_ctrl) & (~FLD_PGA_POWER_DOWN)));
}

/**
 * @brief     This function serves to power down codec pga.
 * @return    none.
 */
void audio_codec_pga_power_down(void)
{
    analog_write(areg_adc_pga_ctrl, (analog_read(areg_adc_pga_ctrl) | (FLD_PGA_POWER_DOWN)));
}

/**
 * @brief      This function serves to set stream0 sample rate for M channel.
 * @param[in]  source - audio input source select.
 * @param[in]  rate   - sample rate.
 * @return     none.
 */
void audio_set_codec_stream0_sample_rate(codec_stream0_input_src_e source, audio_sample_rate_e rate)
{
    (void)source;
    if (rate == AUDIO_8K)
    {
        adc_set_state_length(ADC_M_CHANNEL, 490, 10);
    }
    else if (rate == AUDIO_16K)
    {
        adc_set_state_length(ADC_M_CHANNEL, 240, 10);
    }

    if (rate == AUDIO_16K)
    {
        adc_set_tsample_cycle(ADC_M_CHANNEL, ADC_SAMPLE_CYC_27);
    }
    else
    {
        adc_set_tsample_cycle(ADC_M_CHANNEL, ADC_SAMPLE_CYC_48);
    }
}

/**
 * @brief      This function serves to set codec stream0 input configuration.
 * @param[in]  source - audio input source select.
 * @param[in]  rate   - sample rate.
 * @return     none.
 */
void audio_codec_stream0_input_config(codec_stream0_input_src_e source, audio_sample_rate_e rate)
{
    audio_codec_set_adc_clock();
    adc_set_resolution(ADC_RES10);
    adc_set_adc_sample_mode(SAR_ADC_DIFFERENTIAL);
    reg_adc_config2 |= FLD_M_CHANNEL_EN;
    reg_adc_config0 |= MASK_VAL(FLD_SCANT_MAX, 2);

    audio_set_codec_stream0_sample_rate(source, rate);

    audio_set_adc_pga_gain(CODEC_IN_A_GAIN_0_DB);
    audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_0_DB);
    audio_bypass_hpf3(0);
    audio_codec_adc_power_on();
    audio_codec_pga_power_on();
}

/**
 * @brief      This function serves to set stream0 codec input fifo data format.
 * @param[in]  fifo_chn   - fifo channel.
 * @param[in]  ain_mode   - number of channels.
 * @param[in]  data_width - data bit width.
 * @return     none.
 */
void audio_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_codec_wl_mode_e data_width)
{
    switch (fifo_chn)
    {
    case FIFO0:
        reg_dfifo_ain = (reg_dfifo_ain & (~FLD_AUD_FIFO0_INPUT_SELECT)) | MASK_VAL(FLD_AUD_FIFO0_INPUT_SELECT, data_width);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec stream0 input data bit width for fifo0/fifo1.
 * @param[in]  fifo_chn   - fifo0/fifo1.
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - input data bit width selection.
 * @return     none.
 */
void audio_codec_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width)
{
    audio_set_stream0_fifo_input_mode(fifo_chn, AUDIO_MONO, data_width);
}

/**
 * @brief      This function serves to configure audio stream0 fifo input.
 * @param[in]  fifo_chn   - select channel fifo.
 * @param[in]  source     - codec input source.
 * @param[in]  data_width - fifo data bit width.
 * @return     none.
 */
void audio_codec_stream0_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width)
{
    audio_rxfifo_en(fifo_chn);
    audio_codec_set_stream0_fifo_input_mode(fifo_chn, source, data_width);
}

/**
 * @brief      This function serves to set codec stream0 input initialization.
 * @param[in]  audio_codec - audio_codec_stream0_input_t pointer.
 * @return     none.
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec)
{
    audio_codec_stream0_input_config(audio_codec->input_src, audio_codec->sample_rate);
    audio_codec_stream0_fifo_input_config(audio_codec->fifo_chn, audio_codec->input_src, audio_codec->data_width);
}

/**
 * @brief     This function serves to write data to dfifo.
 * @param[in] buf - the buffer in which the data need to write.
 * @param[in] len - the length of the buffer by short.
 * @return    none.
 */
void audio_dfifo_write_data(const short *buf, unsigned int len)
{
    for (unsigned int i = 0; i < len; i += 2)
    {
        reg_usb_mic_dat1 = (unsigned short)buf[i];
        reg_usb_mic_dat0 = (unsigned short)buf[i + 1];
    }
}
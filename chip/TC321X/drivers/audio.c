/********************************************************************************************************
 * @file    audio.c
 *
 * @brief   This is the source file for TC321X
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
#include "audio.h"

static unsigned char audio_codec_rate[AUDIO_RATE_SIZE + 0x03] = {
    0x06, /*8k*/              // 12Mhz/1500=8K
    0x19, /*11.0259k*/        // 12Mhz/1088=11.0259K
    0x08, /*12k*/             // 12Mhz/1000=12K
    0x0a, /*16k*/             // 12Mhz/750=16K
    0x1b, /*22.0588k*/        // 12Mhz/544=22.0588K
    0x1c, /*24k*/             // 12Mhz/500=24K
    0x0c, /*32k*/             // 12Mhz/375=32K 4mhz
    0x11, /*44.118k*/         // 12Mhz/272=44.118K 6mhz
    0x00, /*48k*/             // 12Mhz/250=48k_clk 6mhz
    0x0d,                     // 32k_clk2mhz           only used in dmic
    0x1f,                     // 44.118k_clk3mhz       only used in dmic
    0x1e                      // 48k_clk3mhz           only used in dmic
};

static unsigned int audio_stream_output_step[AUDIO_RATE_SIZE] = {
    0x00831001, /*8k*/
    0x00b4a001, /*11.0259k*/
    0x00c4a001, /*12k*/
    0x01062001, /*16k*/
    0x01696001, /*22.0588k*/
    0x01893001, /*24k*/
    0x020c5001, /*32k*/
    0x02d28001, /*44.1k*/
    0x03127001, /*48k*/
};

unsigned int audio_sample_rate_value[AUDIO_RATE_SIZE] = {
    8000,   /*8k*/
    11025,  /*11.0259k*/
    12000,  /*12k*/
    16000,  /*16k*/
    22058,  /*22.0588k*/
    24000,  /*24k*/
    32000,  /*32k*/
    44100,  /*44.1k*/
    48000,  /*48k*/
};

/**
 * @brief     This function configures amic bias pin.
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
 * @brief     This function unset amic bias pin.
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
 * @brief     This function configures stream0 dmic pin.
 * @param[in] dmic_data - the data of dmic pin
 * @param[in] dmic_clk1 - the clk1 of dmic pin
 * @param[in] dmic_clk2 - the clk2 of dmic pin, if need not set clk2, please set GPIO_NONE_PIN
 * @return    none
 */
void audio_set_stream0_dmic_pin(audio_dmic_data_pin_e dmic_data, audio_dmic_clk_pin_e dmic_clk1, audio_dmic_clk_pin_e dmic_clk2)
{
    gpio_set_input_en((GPIO_PinTypeDef)dmic_data, 1);
    if (dmic_data == DMIC_DATA_PE0)
    {
        gpio_set_func((GPIO_PinTypeDef)dmic_data, 5);
    }
    else
    {
        gpio_set_func((GPIO_PinTypeDef)dmic_data, 0);
    }

    if ((GPIO_PinTypeDef)dmic_clk1 != GPIO_NONE_PIN)
    {
        if (dmic_clk1 == DMIC_CLK_PE1)
        {
            gpio_set_func((GPIO_PinTypeDef)dmic_clk1, 5);
        }
        else
        {
            gpio_set_func((GPIO_PinTypeDef)dmic_clk1, 0);
        }
    }

    if ((GPIO_PinTypeDef)dmic_clk2 != GPIO_NONE_PIN)
    {
        if (dmic_clk2 == DMIC_CLK_PE1)
        {
            gpio_set_func((GPIO_PinTypeDef)dmic_clk2, 5);
        }
        else
        {
            gpio_set_func((GPIO_PinTypeDef)dmic_clk2, 0);
        }
    }
}

/**
 * @brief     This function configures sdm pin.
 * @param[in] config - sdm config pin struct.
 * @return    none.
 */
void audio_set_sdm_pin(sdm_pin_config_t *config)
{
    if (config->sdm0_p_pin != GPIO_NONE_PIN)
    {
        gpio_set_func(config->sdm0_p_pin, SDM_P0);
    }

    if (config->sdm0_n_pin != GPIO_NONE_PIN)
    {
        gpio_set_func(config->sdm0_n_pin, SDM_N0);
    }

    if (config->sdm1_p_pin != GPIO_NONE_PIN)
    {
        gpio_set_func(config->sdm1_p_pin, SDM_P1);
    }

    if (config->sdm1_n_pin != GPIO_NONE_PIN)
    {
        gpio_set_func(config->sdm1_n_pin, SDM_N1);
    }
}

/**
 * @brief     This function unset sdm pin.
 * @param[in] config - sdm config pin struct.
 * @return    none.
 */
void audio_unset_sdm_pin(sdm_pin_config_t *config)
{
    if (config->sdm0_p_pin != GPIO_NONE_PIN) {
        gpio_set_func(config->sdm0_p_pin, AS_GPIO);
    }

    if (config->sdm0_n_pin != GPIO_NONE_PIN) {
        gpio_set_func(config->sdm0_n_pin, AS_GPIO);
    }

    if (config->sdm1_p_pin != GPIO_NONE_PIN) {
        gpio_set_func(config->sdm1_p_pin,AS_GPIO);
    }

    if (config->sdm1_n_pin != GPIO_NONE_PIN) {
        gpio_set_func(config->sdm1_n_pin, AS_GPIO);
    }
}

/**
 * @brief      This function performs to start w/r data into/from DFIFO0 or DFIFO1.
 * @param[in]  fifo_chn  - fifo channel.
 * @param[in]  pbuff     - address in DFIFO0 or DFIFO1.
 * @param[in]  size_buff - depth of DFIFO0 or DFIFO1.
 * @return     none.
 */
void audio_dfifo_config(audio_fifo_chn_e fifo_chn, unsigned short* pbuff,unsigned int size_buff)
{
    if (fifo_chn == FIFO0)
    {
        reg_dfifo0_addr = (unsigned short)((unsigned int)pbuff);
        reg_dfifo0_size = (size_buff>>4)-1;
    }
    else
    {
        reg_dfifo1_addr = (unsigned short)((unsigned int)pbuff);
        reg_dfifo1_size = (size_buff>>4)-1;
    }
}
/**
 * @brief     This function serves to power on audio.
 * @return    none.
 */
void audio_power_on(void)
{
    pm_set_dig_module_power_switch(PM_POWER_AUDIO, PM_POWER_UP);
    BM_SET(reg_rst2, FLD_RST2_AUD);
}

/**
 * @brief      This function serves to power down all audio power.
 * @return     none.
 */
void audio_power_down(void)
{
    BM_CLR(reg_rst2, FLD_RST2_AUD);
    pm_set_dig_module_power_switch(PM_POWER_AUDIO, PM_POWER_DOWN);
}

/**
 * @brief      This function serves to set adc power_mode.
 * @param[in]  mode - power mode. bit[3]for PGA. The default value is 0 for optimal performance and high power consumption.
 *                    0:high power;1:low power
 * @return     none.
 */
void audio_codec_set_adc_power_mode(unsigned char mode)
{
    analog_write(areg_0x8f, (analog_read(areg_0x8f) & ~FLD_PGA_LOWPOWER) | MASK_VAL(FLD_PGA_LOWPOWER,mode));
}

/**
 *  @brief      This function serves to set the clock of audio
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (7bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(8bits valid).
 *  @return     none
 *  @attention  The default is from pll 48M(default). If the pll is changed, the clk will be changed accordingly.
 */
void audio_set_audio_clk(unsigned short div_numerator, unsigned short div_denominator)
{
    reg_codec_clk_step = (div_numerator & FLD_DMIC_STEP) | FLD_DMIC_SEL;
    reg_codec_clk_mod = div_denominator;
}

/**
 * @brief     This function serves to initial audio.
 * @return    none.
 */
void audio_init(void)
{
    audio_power_on();
    audio_set_audio_clk(1, 2); // audio mclk=48M*(div_numerator/div_denominator):48*(1/2)=24M
}

/**
 * @brief     This function serves to initial audio codec.
 * @return    none
 */
void audio_codec_init(void)
{
    reg_dfifo_dec_ratio |= MASK_VAL(FLD_CLK_USB, 0x01, FLD_CLK_DIV2, 0x01); //codec MCLK 24M, Mclk_real 12M
}

/**
 * @brief     This function serves to set codec_adc 2M clock.
 * @return    none.
 */
void audio_codec_set_adc_clock(void)
{
    reg_codec_config = (reg_codec_config & ~FLD_CK_SEL) | FLD_R_SEL_2M; // 1'b1:dmic clk;1'b0: according to r_sel_2m;
}

/**
 * @brief      This function serves to power on codec_adc.
 * @return     none.
 */
void audio_codec_adc_power_on(void)
{
    sd_adc_power_on(SD_ADC_AUDIO_MODE);
}

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none
 */
void audio_codec_adc_power_down(void)
{
    sd_adc_power_off(SD_ADC_AUDIO_MODE);
}

/**
 *  @brief      This function serves to set stream0 input mode.
 *  @param[in]  input_mode - 0:line_in or amic 1:dmic.
 *  @return     none.
 */
void audio_set_codec_stream0_input_mode(audio_codec_input_mode_e input_mode)
{
    reg_mic_ctrl = (reg_mic_ctrl & (~FLD_AUD_AMIC_DMIC_SELECT)) | MASK_VAL(FLD_AUD_AMIC_DMIC_SELECT, input_mode);
}

/**
 *  @brief      This function serves to set stream0 sample_rate.
 *  @param[in]  source - audio input source select.
 *  @param[in]  rate   - sample_rate.
 *  @return     none.
 */
void audio_set_codec_stream0_sample_rate(codec_stream0_input_src_e source, audio_sample_rate_e rate)
{
    if (source & BIT(3))
    {
        if (rate >= AUDIO_32K)
        {
            rate += 3;
        }
    }
    reg_dfifo_dec_ratio = (reg_dfifo_dec_ratio & (~FLD_CLK_SR)) | (audio_codec_rate[rate] << 0x01) | FLD_CLK_EN;
}

/**
 *  @brief      This function serves to set stream0 input path.
 *  @param[in]  che_en  - channel selection.
 *  @return     none.
 */
void audio_set_codec_stream0_path(audio_chn_sel_e che_en)
{
    reg_dfifo_ain = (reg_dfifo_ain & (~(FLD_AUD_MIC_LEFT_CHN_SELECT | FLD_AUD_MIC_RIGHT_CHN_SELECT))) | MASK_VAL(FLD_AUD_MIC_LEFT_CHN_SELECT, (che_en & CHANNEL_LEFT) ? (0x01) : (0x00),
            FLD_AUD_MIC_RIGHT_CHN_SELECT, (che_en & CHANNEL_RIGHT) ? (0x01) : (0x00));
}

/**
 *  @brief      This function serves to set codec configuration.
 *  @param[in]  source - audio input source select.
 *  @param[in]  rate   - sample_rate.
 *  @return     none.
 */
void audio_codec_stream0_input_config(codec_stream0_input_src_e source, audio_sample_rate_e rate)
{
    if (!(source & BIT(3)))
    {
        audio_codec_set_adc_clock();
        audio_codec_adc_power_on();
        audio_set_adc_pga_gain(CODEC_IN_GAIN_0_DB);
        sd_adc_set_op_mode(AUDIO_ONLY);
    }
    audio_set_codec_stream0_input_mode(source >> 3);
    audio_set_codec_stream0_path(source & 3);
    audio_set_codec_stream0_sample_rate(source, rate);
    audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_m24_DB);
}

/**
 *  @brief      This function serves to set stream0 codec input fifo data format.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 */
void audio_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_codec_wl_mode_e data_width)
{

    if (ain_mode == AUDIO_STEREO)
    {
        reg_mic_ctrl &= ~FLD_AUD_MIC_MONO_EN;
    }
    else
    {
        reg_mic_ctrl |= FLD_AUD_MIC_MONO_EN;
    }

    switch (fifo_chn)
    {
    case FIFO0:
        reg_dfifo_ain = (reg_dfifo_ain & (~FLD_AUD_FIFO0_INPUT_SELECT)) | MASK_VAL(FLD_AUD_FIFO0_INPUT_SELECT, data_width + 2);
        break;
    case FIFO1:
        reg_dfifo_ain = (reg_dfifo_ain & (~FLD_AUD_FIFO1_INPUT_SELECT)) | MASK_VAL(FLD_AUD_FIFO1_INPUT_SELECT, data_width + 2);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec stream0 input data bit width for fifo0/fifo1 .
 * @param[in]  fifo_chn   - fifo0/fifo1
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - input data bit width selection
 * @return     none.
 */
void audio_codec_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width)
{
    unsigned char ain_mode = 0;
    if ((source & CHANNEL_STEREO) == CHANNEL_STEREO)
    {
        ain_mode = AUDIO_STEREO;
    }
    else
    {
        ain_mode = AUDIO_MONO;
    }
    audio_set_stream0_fifo_input_mode(fifo_chn, ain_mode, data_width);
}

/**
 * @brief This function serves to configure audio stream0 fifo input.
 * @param[in]  fifo_chn        - select channel fifo
 * @param[in]  source          - codec input source
 * @param[in]  data_width      - fifo data bit width.
 * @return     none.
 */
void audio_codec_stream0_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width)
{
    audio_rxfifo_en(fifo_chn);
    audio_codec_set_stream0_fifo_input_mode(fifo_chn, source, data_width);
}

/**
 *  @brief      This function serves to set codec initialization.
 *  @param[in]  audio_codec - audio_codec_stream0_input_t pointer.
 *  @return     none.
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec)
{
    audio_codec_init();
    audio_codec_stream0_input_config(audio_codec->input_src, audio_codec->sample_rate);
    audio_codec_stream0_fifo_input_config(audio_codec->fifo_chn, audio_codec->input_src, audio_codec->data_width);
}

/**
 *  @brief      This function serves to set sdm sample rate.
 *  @param[in]  output_src - sdm channel.
 *  @param[in]  rate       - sdm sample rate.
 *  @return     none.
 */
void audio_set_stream_output_sample_rate(audio_stream_output_src_e output_src, audio_sample_rate_e rate)
{
    if (output_src == SDM_MONO)
    {
        reg_audio_ctrl |= FLD_AUDIO_MONO_MODE;
    }
    else
    {
        reg_audio_ctrl &= ~FLD_AUDIO_MONO_MODE;
    }
    reg_ascl_tune = audio_stream_output_step[rate];

}

/**
 * @brief      This function serves to configure audio stream0 output.
 * @param[in]  chn        - audio output source select.
 * @param[in]  rate       - select channel sample rate.
 * @return     none.
 */
void audio_stream_output_config(audio_stream_output_src_e chn, audio_sample_rate_e rate)
{
    /***sdm and i2s0 share a common clock***/
    audio_set_i2s_clk(1, 24); // sdm clk is currently configured for 2m.
    audio_set_stream_output_sample_rate(chn, rate);
}

/**
 *  @brief      This function serves to set sdm path.
 *  @param[in]  chn  - channel selection.
 *  @return     none.
 */
void audio_set_sdm_path(audio_stream_output_src_e chn)
{
    unsigned char value = 0x00;
    if (chn == SDM_MONO)
    {
        value = 0x01;
    }
    reg_audio_ctrl = (reg_audio_ctrl & (~FLD_AUDIO_MONO_MODE)) | MASK_VAL(FLD_AUDIO_MONO_MODE, value);
}

/**
 * @brief This function serves to configure audio stream0 fifo output.
 * @param[in]  chn              - audio output source select.
 * @return    none
 */
void audio_codec_stream_fifo_output_config(audio_stream_output_src_e chn)
{
    audio_txfifo_en();
    audio_set_sdm_path(chn);
}

/**
 * @brief      This function serves to set sdm initialization.
 * @param[in]  audio_stream_output   - audio_codec_output_t pointer.
 * @return     none.
 * @note       only one tx path, FIFO0.
 */
void audio_codec_stream_output_init(audio_codec_output_t *audio_stream_output)
{
    audio_stream_output_config(audio_stream_output->output_src, audio_stream_output->sample_rate);
    audio_codec_stream_fifo_output_config(audio_stream_output->output_src);
    audio_sdm_en();
}
/**
 * @brief     This function configures i2s pin.
 * @param[in] config           - i2s config pin struct.
 * @return    none.
 */
void i2s_set_pin(i2s_pin_config_t *config)
{
    gpio_set_func(config->bclk_pin, I2S_CK);
    gpio_set_func(config->adc_dac_lr_clk_pin, I2S_LR);

    if (config->adc_dat_pin != GPIO_NONE_PIN)
    {
        gpio_set_input_en(config->adc_dat_pin, 1);
        gpio_set_func(config->adc_dat_pin, i2S_DI_I);
    }

    if (config->dac_dat_pin != GPIO_NONE_PIN)
    {
        gpio_set_func(config->dac_dat_pin, I2S_DO);
    }
}

/**
 * @brief     This function serves to set sampling rate when i2s as master.
 * @param[in] i2s_clk_config                i2s_clk_config[2]--->lrclk_adc(sampling rate)
                                                ||                  ||
 *  pll(48M default)------->div---->i2s_clk--->blck--------------->div(32)
 *                           ||
 *           i2s_clk_config[0]/i2s_clk_config[1]
 * For example:sampling rate=48K,i2s_clk_config[3]={1, 48, 0},sampling rate=240M*(4/1875)/32=16K

 * @return    none
 * @attention The default is from pll 240M(default). If the pll is changed, the clk will be changed accordingly.
 */
void audio_set_i2s_clock(unsigned short *i2s_clk_config)
{
    audio_set_i2s_clk(i2s_clk_config[0], i2s_clk_config[1]);
    audio_set_i2s_bclk(i2s_clk_config[2]);
}

/**
 * @brief This function serves to initialize configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer
 * @return    none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config)
{
    i2s_set_pin(i2s_config->pin_config);
    audio_set_i2s_clock(i2s_config->sample_rate);
    reg_audio_ctrl |= FLD_AUDIO_I2S_INTERFACE_EN;
}

/**
 * @brief This function serves to initialize input i2s.
 * @param[in] audio_i2s_input - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_input_init(audio_i2s_input_output_t *audio_i2s_input)
{
    audio_rxfifo_en(audio_i2s_input->fifo_chn);
    if (audio_i2s_input->fifo_chn == FIFO0)
    {
        reg_dfifo_ain = (reg_dfifo_ain & ~FLD_AUD_FIFO0_INPUT_SELECT) | MASK_VAL(FLD_AUD_FIFO0_INPUT_SELECT, AUDIO_FIFO_INPUT_SELECT_I2S);
    }
    else
    {
        reg_dfifo_ain = (reg_dfifo_ain & ~FLD_AUD_FIFO1_INPUT_SELECT) | MASK_VAL(FLD_AUD_FIFO1_INPUT_SELECT, AUDIO_FIFO_INPUT_SELECT_I2S);
    }
    reg_audio_ctrl |= FLD_AUDIO_I2S_RECORDER_EN;
}

/**
 * @brief This function serves to initialize output i2s.
 * @param[in] audio_i2s_output - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_output_init(audio_i2s_input_output_t *audio_i2s_output)
{
    if (audio_i2s_output->i2s_ch == I2S_MONO_L)
    {
        reg_audio_ctrl = (reg_audio_ctrl | FLD_AUDIO_I2S_PLAYER_EN | FLD_AUDIO_MONO_MODE) & (~FLD_AUDIO_SDM_PLAYER_EN);
    }
    else
    {
        reg_audio_ctrl = (reg_audio_ctrl | FLD_AUDIO_I2S_PLAYER_EN) & (~FLD_AUDIO_SDM_PLAYER_EN) & (~FLD_AUDIO_MONO_MODE);
    }
}

/**
 * @brief     This function servers to write data to dfifo.
 * @param[in] buf - the buffer in which the data need to write
 * @param[in] len - the length of the buffer by short.
 * @return    none.
 */
void audio_dfifo_write_data(const short *buf, unsigned int len)
{
    for (unsigned int  i = 0; i < len; i += 2)
    {
        reg_usb_mic_dat1 = (unsigned short)buf[i];
        reg_usb_mic_dat0 = (unsigned short)buf[i + 1];
    }
}

/**
 *  @brief      This function serves to clear codec's input pop.
 *  @param[in]  t_ms        - cut input codec's data.
 *  @return     none.
 */
void audio_codec_clr_input_pop(unsigned char t_ms)
{
    sleep_ms(t_ms);
    audio_set_mute_mic(0);
}

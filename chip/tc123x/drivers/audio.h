/********************************************************************************************************
 * @file    audio.h
 *
 * @brief   This is the header file for TC123X
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
#ifndef audio_H
#define audio_H

#include "driver.h"

/**
 *  @brief  Define fifo type
 */
typedef enum
{
    FIFO0 = 0,
} audio_fifo_chn_e;

/**
 *  @brief  Define the codec input mode selection.
 */
typedef enum
{
    AMIC_OR_LINE_INPUT = 0,
} audio_codec_input_mode_e;

/**
 *  @brief  Define the audio fifo Interrupt type.
 */
typedef enum
{
    AUDIO_RX_FIFO0_IRQ = BIT(0),
} audio_fifo_irq_type_e;

/**
 *  @brief  Define the audio code data bit width.
 *  @note   0: {data_new[15:0], data_old[15:0]};
 *          1: {{16{data[15]}}, data[15:0]}
 */
typedef enum
{
    CODEC_BIT_16_DATA,
    CODEC_BIT_32_DATA,
} audio_codec_wl_mode_e;

/**
 *  @brief  Define the audio channel selection.
 */
typedef enum
{
    AUDIO_MONO,
} audio_channel_select_e;

/**
 *  @brief  Define the audio channel selection.
 */
typedef enum
{
    CHANNEL_LEFT = BIT(0),
} audio_chn_sel_e;

/**
 *  @brief  Define audio codec stream0 input enum
 */
typedef enum
{
    LINE_STREAM0_MONO_L  = BIT(0),
    AMIC_STREAM0_MONO_L  = BIT(2) | BIT(0),
} codec_stream0_input_src_e;

/**
 *  @brief  Define audio codec stream0 input struct
 */
typedef struct
{
    unsigned char           sample_rate;
    unsigned char           data_width;
    void                   *data_buf;
    unsigned int            data_buf_size;
    codec_stream0_input_src_e input_src;
    audio_fifo_chn_e        fifo_chn;
} audio_codec_stream0_input_t;

/**
 *  @brief  Define the audio codec sample rate.
 */
typedef enum
{
    AUDIO_8K,
    AUDIO_16K,
    AUDIO_RATE_SIZE,
} audio_sample_rate_e;

/**
 *  @brief  Define codec input pga gain type
 *  @note   (-10dB) (0x0, default) - (14dB) (0x30) step 0.5dB
 */
typedef enum
{
    CODEC_IN_A_GAIN_m10_DB,    /**< default -10dB */
    CODEC_IN_A_GAIN_m9P5_DB,   /**< -9.5dB */
    CODEC_IN_A_GAIN_m9_DB,     /**< -9dB */
    CODEC_IN_A_GAIN_m8P5_DB,   /**< -8.5dB */
    CODEC_IN_A_GAIN_m8_DB,     /**< -8dB */
    CODEC_IN_A_GAIN_m7P5_DB,   /**< -7.5dB */
    CODEC_IN_A_GAIN_m7_DB,     /**< -7dB */
    CODEC_IN_A_GAIN_m6P5_DB,   /**< -6.5dB */
    CODEC_IN_A_GAIN_m6_DB,     /**< -6dB */
    CODEC_IN_A_GAIN_m5P5_DB,   /**< -5.5dB */
    CODEC_IN_A_GAIN_m5_DB,     /**< -5dB */
    CODEC_IN_A_GAIN_m4P5_DB,   /**< -4.5dB */
    CODEC_IN_A_GAIN_m4_DB,     /**< -4dB */
    CODEC_IN_A_GAIN_m3P5_DB,   /**< -3.5dB */
    CODEC_IN_A_GAIN_m3_DB,     /**< -3dB */
    CODEC_IN_A_GAIN_m2P5_DB,   /**< -2.5dB */
    CODEC_IN_A_GAIN_m2_DB,     /**< -2dB */
    CODEC_IN_A_GAIN_m1P5_DB,   /**< -1.5dB */
    CODEC_IN_A_GAIN_m1_DB,     /**< -1dB */
    CODEC_IN_A_GAIN_m0P5_DB,   /**< -0.5dB */
    CODEC_IN_A_GAIN_0_DB,      /**< 0dB */
    CODEC_IN_A_GAIN_0P5_DB,    /**< 0.5dB */
    CODEC_IN_A_GAIN_1_DB,      /**< 1dB */
    CODEC_IN_A_GAIN_1P5_DB,    /**< 1.5dB */
    CODEC_IN_A_GAIN_2_DB,      /**< 2dB */
    CODEC_IN_A_GAIN_2P5_DB,    /**< 2.5dB */
    CODEC_IN_A_GAIN_3_DB,      /**< 3dB */
    CODEC_IN_A_GAIN_3P5_DB,    /**< 3.5dB */
    CODEC_IN_A_GAIN_4_DB,      /**< 4dB */
    CODEC_IN_A_GAIN_4P5_DB,    /**< 4.5dB */
    CODEC_IN_A_GAIN_5_DB,      /**< 5dB */
    CODEC_IN_A_GAIN_5P5_DB,    /**< 5.5dB */
    CODEC_IN_A_GAIN_6_DB,      /**< 6dB */
    CODEC_IN_A_GAIN_6P5_DB,    /**< 6.5dB */
    CODEC_IN_A_GAIN_7_DB,      /**< 7dB */
    CODEC_IN_A_GAIN_7P5_DB,    /**< 7.5dB */
    CODEC_IN_A_GAIN_8_DB,      /**< 8dB */
    CODEC_IN_A_GAIN_8P5_DB,    /**< 8.5dB */
    CODEC_IN_A_GAIN_9_DB,      /**< 9dB */
    CODEC_IN_A_GAIN_9P5_DB,    /**< 9.5dB */
    CODEC_IN_A_GAIN_10_DB,     /**< 10dB */
    CODEC_IN_A_GAIN_10P5_DB,   /**< 10.5dB */
    CODEC_IN_A_GAIN_11_DB,     /**< 11dB */
    CODEC_IN_A_GAIN_11P5_DB,   /**< 11.5dB */
    CODEC_IN_A_GAIN_12_DB,     /**< 12dB */
    CODEC_IN_A_GAIN_12P5_DB,   /**< 12.5dB */
    CODEC_IN_A_GAIN_13_DB,     /**< 13dB */
    CODEC_IN_A_GAIN_13P5_DB,   /**< 13.5dB */
    CODEC_IN_A_GAIN_14_DB,     /**< 14dB */
} codec_in_pga_gain_e;

/**
 *  @brief  Define codec input path digital gain type
 */
typedef enum
{
    CODEC_IN_D_GAIN_m48_DB = 0,  /**< -48dB */
    CODEC_IN_D_GAIN_m42_DB = 4,  /**< -42dB */
    CODEC_IN_D_GAIN_m36_DB = 8,  /**< -36dB */
    CODEC_IN_D_GAIN_m30_DB = 12, /**< -30dB */
    CODEC_IN_D_GAIN_m24_DB = 16, /**< -24dB */
    CODEC_IN_D_GAIN_m18_DB = 20, /**< -18dB */
    CODEC_IN_D_GAIN_m12_DB = 24, /**< -12dB */
    CODEC_IN_D_GAIN_m6_DB  = 28, /**< -6dB */
    CODEC_IN_D_GAIN_0_DB   = 32, /**<  0dB  default */
    CODEC_IN_D_GAIN_6_DB   = 36, /**<  6dB */
    CODEC_IN_D_GAIN_12_DB  = 40, /**<  12dB */
    CODEC_IN_D_GAIN_18_DB  = 44, /**<  18dB */
    CODEC_IN_D_GAIN_24_DB  = 48, /**<  24dB */
    CODEC_IN_D_GAIN_30_DB  = 52, /**<  30dB */
    CODEC_IN_D_GAIN_36_DB  = 56, /**<  36dB */
    CODEC_IN_D_GAIN_42_DB  = 60, /**<  42dB */
} codec_in_path_digital_gain_e;

/**
 * @brief      This function serves to rxfifo enable.
 * @param[in]  fifo_chn - fifo channel.
 * @return     none.
 */
static inline void audio_rxfifo_en(audio_fifo_chn_e fifo_chn)
{
    reg_dfifo_mode |= BIT(fifo_chn);
}

/**
 * @brief      This function serves to rxfifo disable.
 * @param[in]  fifo_chn - fifo channel.
 * @return     none.
 */
static inline void audio_rxfifo_dis(audio_fifo_chn_e fifo_chn)
{
    reg_dfifo_mode &= ~BIT(fifo_chn);
}

/**
 * @brief      This function serves to set adc pga gain.
 * @param[in]  pga_gain - gain value, post-amplifier (Gain-stage).
 * @return     none.
 */
static inline void audio_set_adc_pga_gain(codec_in_pga_gain_e pga_gain)
{
    reg_pga_m_config = (reg_pga_m_config & (~FLD_PGA_GAIN)) | MASK_VAL(FLD_PGA_GAIN, pga_gain);
}

/**
 * @brief      This function serves to set stream0 digital gain.
 * @param[in]  d_gain - gain value (Both left and right channels are in effect).
 * @return     none.
 */
static inline void audio_set_stream0_dig_gain(codec_in_path_digital_gain_e d_gain)
{
    reg_mic_ctrl = ((reg_mic_ctrl & (~FLD_AUD_MIC_VOL_CONTROL)) | d_gain);
}

/**
 * @brief      This function serves to read pointer/write pointer location.
 * @return     the address of read/write.
 */
static inline unsigned short get_mic_wr_ptr(void)
{
    return reg_audio_wptr >> 1;
}

/**
 * @brief      This function serves to set MUTE PGA gain.
 * @param[in]  enable - enable or disable MUTE PGA gain.
 * @note       Bypass pga gain, but PGA boost still work.
 */
static inline void audio_set_mute_pga(unsigned char enable)
{
    if (enable)
    {
        analog_write(areg_pga_trigger, FLD_PGA_MUTE_TRIG);
    }
    else
    {
        analog_write(areg_pga_trigger, FLD_PGA_MUTE_RELEASE_TRIG);
    }
}

/**
 * @brief      This function serves to enable or bypass audio high pass filter 3.
 * @param[in]  enable - 0: bypass hpf3, 1: enable hpf3.
 * @return     none.
 * @note       If hpf3 filter is enabled, the data will be down-sampled by a factor of 6, for example, from 96K to 16K.
 */
static inline void audio_bypass_hpf3(unsigned char enable)
{
    if (enable)
    {
        reg_dfifo_manual_mode |= FLD_DFIFO_LPF3_BYPASS;
    }
    else
    {
        reg_dfifo_manual_mode &= ~FLD_DFIFO_LPF3_BYPASS;
    }
}

/**
 * @brief      This function serves to clear dfifo mask and status.
 * @return     none.
 */
static inline void audio_clr_dfifo_mask_and_status(void)
{
    reg_dfifo_mode &= (~(FLD_AUD_DFIFO0_L_INT | FLD_AUD_DFIFO0_H_INT | FLD_AUD_DFIFO1_H_INT));
    reg_dfifo_irq_status = (FLD_AUD_DFIFO0_IRQ0 | FLD_AUD_DFIFO0_IRQ1 | FLD_AUD_DFIFO1_IRQ2);
}

/**
 * @brief     This function serves to configure amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_set_amic_bias_pin(GPIO_PinTypeDef amic_bias);

/**
 * @brief     This function serves to unset amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_unset_amic_bias_pin(GPIO_PinTypeDef amic_bias);

/**
 * @brief     This function serves to configure amic input pins.
 * @return    none.
 * @note      This function only supports PB0 (positive) and PB1 (negative).
 */
void audio_set_amic_pga_pin(void);

/**
 * @brief      This function serves to configure DFIFO0 for audio data input.
 * @param[in]  fifo_chn  - fifo channel.
 * @param[in]  pbuff     - address in DFIFO0 or DFIFO1.
 * @param[in]  size_buff - depth of DFIFO0 or DFIFO1.
 * @return     none.
 */
void audio_dfifo_config(audio_fifo_chn_e fifo_chn, unsigned short *pbuff, unsigned int size_buff);

/**
 * @brief      This function serves to set adc power mode.
 * @param[in]  mode - power mode. bit[0] for PGA. The default value is 0 for optimal performance and high power consumption.
 *                    0: high power; 1: low power.
 * @return     none.
 */
void audio_codec_set_adc_power_mode(unsigned char mode);

/**
 * @brief     This function serves to set codec_adc 4M clock.
 * @return    none.
 */
void audio_codec_set_adc_clock(void);

/**
 * @brief      This function serves to power on codec_adc.
 * @return     none.
 */
void audio_codec_adc_power_on(void);

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none.
 */
void audio_codec_adc_power_down(void);

/**
 * @brief      This function serves to power on codec pga.
 * @return     none.
 */
void audio_codec_pga_power_on(void);

/**
 * @brief     This function serves to power down codec pga.
 * @return    none.
 */
void audio_codec_pga_power_down(void);

/**
 * @brief      This function serves to set stream0 sample rate for M channel.
 * @param[in]  source - audio input source select.
 * @param[in]  rate   - sample rate.
 * @return     none.
 */
void audio_set_codec_stream0_sample_rate(codec_stream0_input_src_e source, audio_sample_rate_e rate);

/**
 * @brief      This function serves to set codec stream0 input configuration.
 * @param[in]  source - audio input source select.
 * @param[in]  rate   - sample rate.
 * @return     none.
 */
void audio_codec_stream0_input_config(codec_stream0_input_src_e source, audio_sample_rate_e rate);

/**
 * @brief      This function serves to set stream0 codec input fifo data format.
 * @param[in]  fifo_chn   - fifo channel.
 * @param[in]  ain_mode   - number of channels.
 * @param[in]  data_width - data bit width.
 * @return     none.
 */
void audio_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_codec_wl_mode_e data_width);

/**
 * @brief      This function serves to set codec stream0 input data bit width for fifo0/fifo1.
 * @param[in]  fifo_chn   - fifo0/fifo1.
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - input data bit width selection.
 * @return     none.
 */
void audio_codec_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief      This function serves to configure audio stream0 fifo input.
 * @param[in]  fifo_chn   - select channel fifo.
 * @param[in]  source     - codec input source.
 * @param[in]  data_width - fifo data bit width.
 * @return     none.
 */
void audio_codec_stream0_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief      This function serves to set codec stream0 input initialization.
 * @param[in]  audio_codec - audio_codec_stream0_input_t pointer.
 * @return     none.
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec);

/**
 * @brief     This function serves to write data to dfifo.
 * @param[in] buf - the buffer in which the data need to write.
 * @param[in] len - the length of the buffer by short.
 * @return    none.
 */
void audio_dfifo_write_data(const short *buf, unsigned int len);

#endif
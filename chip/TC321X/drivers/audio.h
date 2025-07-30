/********************************************************************************************************
 * @file    audio.h
 *
 * @brief   This is the header file for TC321X
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
#ifndef audio_H
#define audio_H

#include "driver.h"

/**
 *  @brief  Define fifo type
 */
typedef enum
{
    FIFO0 = 0,
    FIFO1,
} audio_fifo_chn_e;

/**
 *  @brief  Define the i2s source selection.
 */
typedef enum
{
    I2S_STEREO = 0,
    I2S_MONO_L = 1,
} audio_i2s_ch_e;

/**
 *  @brief  Define the codec input mode selection.
 */
typedef enum
{
    AMIC_OR_LINE_INPUT = 0,
    DMIC_INPUT,
} audio_codec_input_mode_e;

/**
 *  @brief  Define the audio fifo Interrupt type.
 */
typedef enum
{
    AUDIO_RX_FIFO0_IRQ = BIT(0),
    AUDIO_RX_FIFO1_IRQ = BIT(1),
    AUDIO_TX_FIFO0_IRQ = BIT(3),
} audio_fifo_irq_type_e;

/**
 *  @brief  Define the i2s data bit width selection.
 */
typedef enum
{
    I2S_BIT_16_DATA,
} audio_i2s_wl_mode_e;

/**
 *  @brief  Define the audio code data bit width.
 */
typedef enum
{
    CODEC_BIT_16_DATA,
    CODEC_BIT_20_DATA,
} audio_codec_wl_mode_e;

/**
 *  @brief  Define the audio channel selection.
 */
typedef enum
{
    AUDIO_MONO,
    AUDIO_STEREO,
} audio_channel_select_e;

/**
 *  @brief  Define the audio output source selection.
 */
typedef enum
{
    SDM_MONO,
    SDM_STEREO,
} audio_stream_output_src_e;

/**
 *  @brief  Define the audio channel selection.
 */
typedef enum
{
    CHANNEL_LEFT = BIT(0),
    CHANNEL_RIGHT = BIT(1),
    CHANNEL_STEREO = BIT(0) | BIT(1),
} audio_chn_sel_e;

/**
 * @brief  Define the sdm pin definition structure.
 */
typedef struct
{
    GPIO_PinTypeDef sdm0_p_pin;
    GPIO_PinTypeDef sdm0_n_pin;
    GPIO_PinTypeDef sdm1_p_pin;
    GPIO_PinTypeDef sdm1_n_pin;
} sdm_pin_config_t;

/**
 * @brief  Define the i2s pin definition structure.
 */
typedef struct
{
    GPIO_PinTypeDef bclk_pin;
    GPIO_PinTypeDef adc_dac_lr_clk_pin;
    GPIO_PinTypeDef adc_dat_pin;
    GPIO_PinTypeDef dac_dat_pin;
} i2s_pin_config_t;

/**
 *  @brief  Define audio codec stream0 input enum
 */
typedef enum
{
    LINE_STREAM0_MONO_L = BIT(0),
    LINE_STREAM0_STEREO = BIT(1) | BIT(0),
    AMIC_STREAM0_MONO_L = BIT(2) | BIT(0),
    AMIC_STREAM0_STEREO = BIT(2) | BIT(1) | BIT(0),
    DMIC_STREAM0_MONO_L = BIT(3) | BIT(0),
    DMIC_STREAM0_MONO_R = BIT(3) | BIT(1),
    DMIC_STREAM0_STEREO = BIT(3) | BIT(1) | BIT(0),
} codec_stream0_input_src_e;

/**
 *  @brief  Define audio codec stream0 input struct
 */
typedef struct
{
    unsigned char sample_rate;
    unsigned char data_width;
    void *data_buf;
    unsigned int data_buf_size;
    codec_stream0_input_src_e input_src;
    audio_fifo_chn_e fifo_chn;
} audio_codec_stream0_input_t;

/**
 *  @brief  Define audio codec output struct
 */
typedef struct
{
    unsigned char sample_rate;
    void *data_buf;
    unsigned int data_buf_size;
    audio_stream_output_src_e output_src;
    audio_fifo_chn_e fifo_chn;
} audio_codec_output_t;

/**
 *  @brief  Define audio i2s general configuration struct
 */
typedef struct
{
    unsigned short *sample_rate;
    i2s_pin_config_t *pin_config;
    audio_i2s_wl_mode_e data_width;
} audio_i2s_config_t;

/**
 *  @brief  Define audio i2s input output struct
 */
typedef struct
{
    void *data_buf;
    unsigned int data_buf_size;
    audio_i2s_wl_mode_e data_width;
    audio_fifo_chn_e fifo_chn;
    audio_i2s_ch_e i2s_ch;
} audio_i2s_input_output_t;

/**
 *  @brief  Define audio dmic data pin enum
 */
typedef enum
{
    DMIC_DATA_PA5 = GPIO_PA5,
    DMIC_DATA_PB4 = GPIO_PB4,
    DMIC_DATA_PB6 = GPIO_PB6,
    DMIC_DATA_PC0 = GPIO_PC0,
    DMIC_DATA_PC3 = GPIO_PC3,
    DMIC_DATA_PC7 = GPIO_PC7,
    DMIC_DATA_PE0 = GPIO_PE0,
} audio_dmic_data_pin_e;

/**
 *  @brief  Define audio dmic clock pin enum
 */
typedef enum
{
    DMIC_CLK_PA6 = GPIO_PA6,
    DMIC_CLK_PB5 = GPIO_PB5,
    DMIC_CLK_PB7 = GPIO_PB7,
    DMIC_CLK_PC1 = GPIO_PC1,
    DMIC_CLK_PC4 = GPIO_PC4,
    DMIC_CLK_PD0 = GPIO_PD0,
    DMIC_CLK_PE1 = GPIO_PE1,
} audio_dmic_clk_pin_e;

/**
 *  @brief  Define the audio codec/sdm sample rate.
 */
typedef enum
{
    AUDIO_8K,       /**< 8K=12Mhz/1500 */
    AUDIO_11P0K,    /**< 11.0259K=12Mhz/1088 */
    AUDIO_12K,      /**< 12K=12Mhz/1000 */
    AUDIO_16K,      /**< 16K=12Mhz/750 */
    AUDIO_22P0588K, /**< 22.0588K=12Mhz/544 */
    AUDIO_24K,      /**< 24K=12Mhz/500 */
    AUDIO_32K,      /**< 32K=12Mhz/375 */
    AUDIO_44P1K,    /**< 44.118K=12Mhz/272 */
    AUDIO_48K,      /**< 48k=12Mhz/250 */
    AUDIO_RATE_SIZE,
} audio_sample_rate_e;

/**
 *  @brief  Define codec input pga gain type
 */
typedef enum
{
    CODEC_IN_A_GAIN_45P2_DB, /**< 45.2dB */
    CODEC_IN_GAIN_43P5_DB,   /**< 43.5dB */
    CODEC_IN_GAIN_42P1_DB,   /**< 42.1dB */
    CODEC_IN_GAIN_40P5_DB,   /**< 40.5dB */
    CODEC_IN_GAIN_39P1_DB,   /**< 39.1dB */
    CODEC_IN_GAIN_37P4_DB,   /**< 37.4dB */
    CODEC_IN_GAIN_36P0_DB,   /**< 36.0dB */
    CODEC_IN_GAIN_34P6_DB,   /**< 34.6dB */
    CODEC_IN_GAIN_33P0_DB,   /**< 33.0dB default */
    CODEC_IN_GAIN_30P1_DB,   /**< 30.1dB */
    CODEC_IN_GAIN_27P0_DB,   /**< 27.0dB */
    CODEC_IN_GAIN_24P0_DB,   /**< 24.0dB */
    CODEC_IN_GAIN_21P0_DB,   /**< 21.0dB */
    CODEC_IN_GAIN_15P0_DB,   /**< 15.0dB */
    CODEC_IN_GAIN_9P0_DB,    /**< 9.0dB */
    CODEC_IN_GAIN_0_DB,      /**< 0dB */
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
    CODEC_IN_D_GAIN_m6_DB = 28,  /**< -6dB */
    CODEC_IN_D_GAIN_0_DB = 32,   /**<  0dB  default */
    CODEC_IN_D_GAIN_6_DB = 36,   /**<  6dB */
    CODEC_IN_D_GAIN_12_DB = 40,  /**<  12dB */
    CODEC_IN_D_GAIN_18_DB = 44,  /**<  18dB */
    CODEC_IN_D_GAIN_24_DB = 48,  /**<  24dB */
    CODEC_IN_D_GAIN_30_DB = 52,  /**<  30dB */
    CODEC_IN_D_GAIN_36_DB = 56,  /**<  36dB */
    CODEC_IN_D_GAIN_42_DB = 60,  /**<  42dB */
} codec_in_path_digital_gain_e;

/**
 *  @brief  Define codec output path digital gain type
 */
typedef enum
{
    ASCL_OUT_D_GAIN_m48_DB = 0x20, /**<-48dB */
    ASCL_OUT_D_GAIN_m42_DB = 0x24, /**<-42dB */
    ASCL_OUT_D_GAIN_m36_DB = 0x28, /**<-36dB */
    ASCL_OUT_D_GAIN_m30_DB = 0x2C, /**<-30dB */
    ASCL_OUT_D_GAIN_m24_DB = 0x30, /**<-24dB */
    ASCL_OUT_D_GAIN_m18_DB = 0x34, /**<-18dB */
    ASCL_OUT_D_GAIN_m12_DB = 0x38, /**<-12dB */
    ASCL_OUT_D_GAIN_m6_DB  = 0x3C, /**<-6dB */
    ASCL_OUT_D_GAIN_0_DB   = 0x40, /**< 0dB  default  */
    ASCL_OUT_D_GAIN_6_DB   = 0x44, /**< 6dB */
    ASCL_OUT_D_GAIN_12_DB  = 0x48, /**< 12dB */
    ASCL_OUT_D_GAIN_18_DB  = 0x4C, /**< 18dB */
    ASCL_OUT_D_GAIN_24_DB  = 0x50, /**< 24dB */
    ASCL_OUT_D_GAIN_30_DB  = 0x54, /**< 30dB */
    ASCL_OUT_D_GAIN_36_DB  = 0x58, /**< 36dB */
    ASCL_OUT_D_GAIN_42_DB  = 0x5C, /**< 42dB */
} ascl_out_path_digital_gain_e;

/**
 *  @brief      This function serves to set codec enable.
 *  @param[in]  val - 0:disable, 1:enable.
 *  @return     none.
 */
static inline void audio_set_codec_en(unsigned char val)
{
    reg_codec_dec_en = MASK_VAL(FLD_AUD_CODEC_DEC_EN, val);
}

/**
 *  @brief      This function serves to rxfifo enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_en(audio_fifo_chn_e fifo_chn)
{
    reg_dfifo_mode |= BIT(fifo_chn);
}

/**
 *  @brief      This function serves to rxfifo disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_dis(audio_fifo_chn_e fifo_chn)
{
    reg_dfifo_mode &= ~BIT(fifo_chn);
}

/**
 *  @brief      This function serves to txfifo enable.
 *  @return     none.
 */
static inline void audio_txfifo_en()
{
    reg_dfifo_mode |= FLD_AUD_DFIFO0_OUT;
}

/**
 *  @brief      This function serves to txfifo disable.
 *  @return     none.
 */
static inline void audio_txfifo_dis()
{
    reg_dfifo_mode &= ~FLD_AUD_DFIFO0_OUT;
}

/**
 *  @brief      This function serves to sdm enable.
 *  @return     none.
 */
static inline void audio_sdm_en(void)
{
    BM_SET(reg_audio_ctrl, FLD_AUDIO_SDM_PLAYER_EN);
}

/**
 *  @brief      This function serves to sdm disable.
 *  @return     none.
 */
static inline void audio_sdm_dis(void)
{
    BM_CLR(reg_audio_ctrl, FLD_AUDIO_SDM_PLAYER_EN);
}

/**
 *  @brief      This function serves to set adc pga gain.
 *  @param[in]  pga_gain - gain value.
 *  @return     none.
 */
static inline void audio_set_adc_pga_gain(codec_in_pga_gain_e pga_gain)
{
    analog_write(areg_0x8d, ((analog_read(areg_0x8d) & (~FLD_AUDIO_PGAVOL_IN)) | MASK_VAL(FLD_AUDIO_PGAVOL_IN, pga_gain)));
}

/**
 *  @brief      This function serves to get adc pga gain.
 *  @param[in]  pga_gain - gain value.
 *  @return     none.
 */
static inline codec_in_pga_gain_e audio_get_adc_pga_gain(void)
{
    codec_in_pga_gain_e val;
    val = (analog_read(areg_0x8d) & FLD_AUDIO_PGAVOL_IN) >> 4;
    return val;
}

/**
 *  @brief      This function serves to stream0 digital gain.
 *  @param[in]  d_gain - gain value(Both left and right channels are in effect).
 *  @return     none.
 */
static inline void audio_set_stream0_dig_gain(codec_in_path_digital_gain_e d_gain)
{
    reg_mic_ctrl = ((reg_mic_ctrl & (~FLD_AUD_MIC_VOL_CONTROL)) | d_gain);
}

/**
 *  @brief      This function serves to get stream0 digital gain.
 *  @return     gain value.
 */
static inline codec_in_path_digital_gain_e audio_get_stream0_dig_gain(void)
{
    codec_in_path_digital_gain_e val;
    val = reg_mic_ctrl & FLD_AUD_MIC_VOL_CONTROL;
    return val;
}

/**
 *  @brief      This function serves to ascl gain.
 *  @param[in]  gain        - ascl gain value.
 *  @return     none.
 */
static inline void audio_set_ascl_gain(ascl_out_path_digital_gain_e gain)
{
	reg_audio_vol = ((reg_audio_vol & (~FLD_AUDIO_VOL)) | MASK_VAL(FLD_AUDIO_VOL, gain));
}

/**
 * @brief     This function serves to reset audio Module
 * @param[in] none.
 * @return    none
 */
static inline void audio_reset(void)
{
    BM_SET(reg_rst2, FLD_RST2_AUD);
    BM_CLR(reg_rst2, FLD_RST2_AUD);
}

/**
 *  @brief      This function serves to set the clock of i2s
 *  @param[in]  step - the dividing factor of step.
 *  @param[in]  mod - the dividing factor of mod.
 *  @return     none
 */
static inline void audio_set_i2s_clk(unsigned char step,unsigned char mod)
{
    reg_i2s_step = step|FLD_I2S_CLK_EN;
    reg_i2s_mod = mod;
}

/**
 *  @brief      This function serves to set the bclk.
 *  @param[in]  div  - bclk = i2s_clk / ((div+1)*2)
 *  @return     none.
 */
static inline void audio_set_i2s_bclk(unsigned char div)
{
    reg_ascl_tune1 = MASK_VAL(FACTOR_TO_GN_I2S_CLK, div) | MASK_VAL(STEPI_LOW_BITS, 0x00);
    reg_ascl_tune2 = MASK_VAL(STEPI_MID_BITS, 0x00);
    reg_ascl_tune3 = MASK_VAL(STEPI_HIGH_BITS, 0x80);
}

/**
 * @brief      This function performs to read pointer/write pointer location.
 * @return     the address of read/write.
 */
static inline unsigned short get_mic_wr_ptr (void)
{
    return reg_audio_wptr >>1;
}

/**
 *  @brief     This function serves to set MUTE mic.
 *  @param[in] ebable - enable or disable MUTE mic
 *  @return    none.
 */
static inline void audio_set_mute_mic(unsigned char enable)
{
    if(enable)
    {
        reg_codec_config |= FLD_DATI_SOTE_MUTE;
    }
    else
    {
        reg_codec_config &= ~FLD_DATI_SOTE_MUTE;
    }
}

/**
 *  @brief     This function serves to set MUTE PGA.
 *  @param[in] enable - enable or disable MUTE PGA
 *  @return    none.
 */
static inline void audio_set_mute_pga(unsigned char enable)
{
    if(enable)
    {
        analog_write(areg_0x8d,analog_read(areg_0x8d) | FLD_AUDIO_MUTE_PGA);
    }
    else
    {
        analog_write(areg_0x8d,analog_read(areg_0x8d) & ~FLD_AUDIO_MUTE_PGA);
    }
}

/**
 *  @brief     This function serves to enable audio high pass filter.
 *  @return    none.
 */
static inline void audio_en_hpf(void)
{
    reg_audio_ctrl |= FLD_AUDIO_HPF_EN;
}

/**
 *  @brief     This function serves to clear dfifo mask and status.
 *  @return    none.
 */
static inline void audio_clr_dfifo_mask_and_status(void)
{
	reg_dfifo_mode &= (~(FLD_AUD_DFIFO0_L_INT | FLD_AUD_DFIFO0_H_INT | FLD_AUD_DFIFO1_H_INT));
	reg_dfifo_irq_status = (FLD_AUD_DFIFO0_IRQ0 | FLD_AUD_DFIFO0_IRQ1 | FLD_AUD_DFIFO1_IRQ2);
}
/**
 * @brief     This function configures amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_set_amic_bias_pin(GPIO_PinTypeDef amic_bias);

/**
 * @brief     This function unset amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_unset_amic_bias_pin(GPIO_PinTypeDef amic_bias);

/**
 * @brief     This function configures stream0 dmic pin.
 * @param[in] dmic_data - the data of dmic pin
 * @param[in] dmic_clk1 - the clk1 of dmic pin
 * @param[in] dmic_clk2 - the clk2 of dmic pin, if need not set clk2, please set GPIO_NONE_PIN
 * @return    none
 */
void audio_set_stream0_dmic_pin(audio_dmic_data_pin_e dmic_data, audio_dmic_clk_pin_e dmic_clk1, audio_dmic_clk_pin_e dmic_clk2);

/**
 * @brief     This function configures sdm pin.
 * @param[in] config - sdm config pin struct.
 * @return    none.
 */
void audio_set_sdm_pin(sdm_pin_config_t *config);

/**
 * @brief     This function unset sdm pin.
 * @param[in] config - sdm config pin struct.
 * @return    none.
 */
void audio_unset_sdm_pin(sdm_pin_config_t *config);

/**
 * @brief      This function performs to start w/r data into/from DFIFO0 or DFIFO1.
 * @param[in]  fifo_chn  - fifo channel.
 * @param[in]  pbuff     - address in DFIFO0 or DFIFO1.
 * @param[in]  size_buff - depth of DFIFO0 or DFIFO1.
 * @return     none.
 */
void audio_dfifo_config(audio_fifo_chn_e fifo_chn, unsigned short* pbuff,unsigned int size_buff);

/**
 * @brief     This function serves to power on audio.
 * @return    none.
 */
void audio_power_on(void);

/**
 * @brief      This function serves to power down all audio power.
 * @return     none.
 */
void audio_power_down(void);

/**
 * @brief      This function serves to set adc power_mode.
 * @param[in]  mode - power mode. bit[3]for PGA. The default value is 0 for optimal performance and high power consumption.
 *                    0:high power;1:low power
 * @return     none.
 */
void audio_codec_set_adc_power_mode(unsigned char mode);

/**
 * @brief     This function serves to initial audio.
 * @return    none.
 */
void audio_init(void);

/**
 * @brief      This function serves to power on codec_adc.
 * @return     none.
 */
void audio_codec_adc_power_on(void);

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none
 */
void audio_codec_adc_power_down(void);

/**
 *  @brief      This function serves to set stream0 input mode.
 *  @param[in]  input_mode - 0:line_in or amic 1:dmic.
 *  @return     none.
 */
void audio_set_codec_stream0_input_mode(audio_codec_input_mode_e input_mode);

/**
 *  @brief      This function serves to set codec initialization.
 *  @param[in]  audio_codec - audio_codec_stream0_input_t pointer.
 *  @return     none.
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec);

/**
 * @brief      This function serves to set sdm initialization.
 * @param[in]  audio_stream_output   - audio_codec_output_t pointer.
 * @return     none.
 */
void audio_codec_stream_output_init(audio_codec_output_t *audio_stream_output);

/**
 * @brief This function serves to initialize configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer
 * @return    none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config);

/**
 * @brief This function serves to initialize input i2s.
 * @param[in] audio_i2s_input - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_input_init(audio_i2s_input_output_t *audio_i2s_input);

/**
 * @brief This function serves to initialize output i2s.
 * @param[in] audio_i2s_output - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_output_init(audio_i2s_input_output_t *audio_i2s_output);

/**
 * @brief     This function servers to write data to dfifo.
 * @param[in] buf - the buffer in which the data need to write
 * @param[in] len - the length of the buffer by short.
 * @return    none.
 */
void audio_dfifo_write_data(const short *buf, unsigned int len);

/**
 *  @brief      This function serves to clear codec's input pop.
 *  @param[in]  t_ms        - cut input codec's data.
 *  @return     none.
 */
void audio_codec_clr_input_pop(unsigned char t_ms);
#endif

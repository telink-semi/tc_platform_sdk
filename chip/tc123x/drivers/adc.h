/********************************************************************************************************
 * @file    adc.h
 *
 * @brief   This is the header file for tc123x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page ADC
 *
 *  Introduction
 *  ===============
 *  TC123X supports hardware ADC function.
 *
 *  API Reference
 *  ===============
 *  Header File: adc.h
 */
#pragma once

#include "dma.h"
#include "compiler.h"
#include "gpio.h"
#include "register.h"

#ifndef INTERNAL_TEST_FUNC_EN
    #define INTERNAL_TEST_FUNC_EN 0 //only for internal test
#endif



typedef enum
{
    ADC_VREF_0P9V    = 0x01,
    ADC_VREF_1P2V    = 0x02,
} adc_ref_vol_e;

typedef enum
{
    ADC_VBAT_DIV_OFF = 0,
    ADC_VBAT_DIV_1F4 = 0x01,
} adc_vbat_div_e;

typedef enum {
    NOINPUTN           = 0,
    ADC_GPIO_PB0N      = 0x01,
    ADC_GPIO_PB1N      = 0x02,
    ADC_GPIO_PB2N      = 0x03,
    ADC_GPIO_PA4N      = 0x04,
    ADC_GPIO_PA5N      = 0x05,
    GND                = 0x07,

} adc_input_nch_e;

typedef enum {
    NOINPUTP           = 0,
    ADC_GPIO_PB0P      = 0x01,
    ADC_GPIO_PB1P      = 0x02,
    ADC_GPIO_PB2P      = 0x03,
    ADC_GPIO_PA4P      = 0x04,
    ADC_GPIO_PA5P      = 0x05,
    ADC_VBAT           = 0x07,
}adc_input_pch_e;

/**
 * @brief adc input pin type
 * |           |              |
 * | :-------- | :----------- |
 * |   <15:12> |    <11:0>    |
 * |adc channel|    gpio pin  |
 */
typedef enum{
    ADC_GPIO_PB0 = GPIO_PB0 | (0x1 << 12),
    ADC_GPIO_PB1 = GPIO_PB1 | (0x2 << 12),
    ADC_GPIO_PB2 = GPIO_PB2 | (0x3 << 12),
    ADC_GPIO_PA4 = GPIO_PA4 | (0x4 << 12),
    ADC_GPIO_PA5 = GPIO_PA5 | (0x5 << 12),
} adc_input_pin_def_e;

typedef enum
{
    ADC_GPIO_MODE,
    ADC_VBAT_MODE,
} adc_input_pin_mode_e;

/**
 * @brief adc sample channel type.
 * @attention
 *  #1 Only M channel can be used in NDMA mode, multiple channels can be used in DMA mode.
 *  #2 When using one channels in DMA mode, only M channel can be selected.
 *  #3 When using two channels in DMA mode, only M and L channels can be selected.
 */
typedef enum
{
    ADC_M_CHANNEL,
    ADC_L_CHANNEL,
    ADC_R_CHANNEL,
} adc_sample_chn_e;

/**
 * @brief adc channel cnt type.
*|             |             |
*| :--------   | :-----------|
*|   <7:4>     |    <3:0>    |
*|transfer_mode| channel_cnt |
**/
typedef enum
{
    NDMA_M_CHN    = 1 | (0 << 4),
    DMA_M_CHN     = 1 | (1 << 4),
    DMA_M_L_CHN   = 2 | (1 << 4),
    DMA_M_L_R_CHN = 3 | (1 << 4),
} adc_chn_cnt_e;

/**
 * @brief enumeration of adc resolutions.
 * @note  #1 The adc performs best when set to ADC_RES10 by default, and it is not recommended to change this configuration.
 *        #2 The sign and valid bits corresponding to adc code for setting different resolutions are as follows:
 *           If ADC_RES8 is set, the sign bit of adc code is bit7 and bit0-6 are valid bits.
 *           If ADC_RES10 is set, the sign bit of adc code is bit9 and bit0-8 are valid bits.(default)
 *        #3 Regardless of the set resolution, the final output code value is always 16bit.
 */
typedef enum
{
    ADC_RES8  = 0,
    ADC_RES10 = 1,
} adc_res_e;


typedef enum
{
    ADC_SAMPLE_CYC_3,
    ADC_SAMPLE_CYC_6,
    ADC_SAMPLE_CYC_9,
    ADC_SAMPLE_CYC_12,
    ADC_SAMPLE_CYC_15,
    ADC_SAMPLE_CYC_18,
    ADC_SAMPLE_CYC_21,
    ADC_SAMPLE_CYC_24,
    ADC_SAMPLE_CYC_27,
    ADC_SAMPLE_CYC_30,
    ADC_SAMPLE_CYC_33,
    ADC_SAMPLE_CYC_36,
    ADC_SAMPLE_CYC_39,
    ADC_SAMPLE_CYC_42,
    ADC_SAMPLE_CYC_45,
    ADC_SAMPLE_CYC_48,
} adc_sample_cycle_e;

/**
 * @brief adc sample frequency type.
 * |         |           |             |             |
 * | :-------|  :--------| :--------   | :-----------|
 * | <25:24> |  <23:8>   |   <7:4>     |    <3:0>    |
 * |  flag   |  capture  | sample_cycle|     set     |
 *
 */
typedef enum
{
    ADC_SAMPLE_FREQ_23K  = (0 << 24) | (1023 << 8) | (ADC_SAMPLE_CYC_48 << 4) | 15,
    ADC_SAMPLE_FREQ_48K  = (1 << 24) | (490 << 8) | (ADC_SAMPLE_CYC_48 << 4) | 10,
    ADC_SAMPLE_FREQ_96K  = (2 << 24) | (240 << 8) | (ADC_SAMPLE_CYC_27 << 4) | 10,
    ADC_SAMPLE_FREQ_192K = (3 << 24) | (115 << 8) | (ADC_SAMPLE_CYC_6 << 4) | 10,
} adc_sample_freq_e;

typedef enum
{
    ADC_PRESCALE_1 = 0x00,
    ADC_PRESCALE_1F4 = 0x02,
} adc_pre_scale_e;

typedef struct
{
    adc_vbat_div_e    divider;
    adc_ref_vol_e     v_ref;
    adc_pre_scale_e   pre_scale;
    adc_sample_freq_e sample_freq;
    adc_input_pch_e   input_p;
    adc_input_nch_e   input_n;
} adc_chn_cfg_t;

typedef struct
{
    adc_ref_vol_e       v_ref;
    adc_pre_scale_e     pre_scale;
    adc_sample_freq_e   sample_freq;
    adc_input_pin_def_e pin;
} adc_gpio_cfg_t;

typedef enum
{
    DMA,
    NDMA,
} adc_transfer_mode_e;

/**
 * @brief sar adc mode select.
 *
 */
typedef enum
{
    SAR_ADC_SINGLE_ENDED,
    SAR_ADC_DIFFERENTIAL,
} sar_adc_mode_e;


/**********************************************************************************************************************
 *                                         SAR ADC global interface                                              *
 **********************************************************************************************************************/

/**
 * @brief      This function sets adc digital clock and analog clock.
 * @return     none
 * @attention  ADC digital clock from crystal(24MHz), analog clock from digital clock.
 *             The ADC digital clock and analog clock are fixed at 24 MHz and 4 MHz, respectively, and should not be modified.
 */
void adc_set_clk(void);

/**
 * @brief This function serves to set resolution.
 * @param[in]  res - enum variable of ADC resolution.
 * @return none
 */
void adc_set_resolution(adc_res_e res);

/**
 * @brief This function serves to set sar adc sample mode.
 * @param[in]  sar_adc_mode_e - enum variable of sar adc mode.
 * @return none
 */
void adc_set_adc_sample_mode(sar_adc_mode_e mode);

/**
 * @brief      This function serves to set ADC sample time(the number of adc clocks for sample cycles) for M channel,L channel or R channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  sample_cycle - enum variable of adc sample cycles.
 * @return     none
 */
void adc_set_tsample_cycle(adc_sample_chn_e chn, adc_sample_cycle_e sample_cycle);

/**
 * @brief This function serves to set state and capture_state length.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   r_max_mc - Value of length of "capture" state for channel.
 * @param[in]   r_max_s - Value of length of "set" state for channel.
 * @return none
 */
void adc_set_state_length(adc_sample_chn_e chn, unsigned short r_max_mc, unsigned char r_max_s);

/**********************************************************************************************************************
 *                                         DMA and NDMA common interface                                              *
 **********************************************************************************************************************/
/**
 * @brief      This function enable adc source clock: xtal_24M
 * @return     none
 */
static inline void adc_clk_en(void)
{
    reg_rst1 |= FLD_RST1_SAR;
}
/**
 * @brief      This function disable adc source clock: xtal 24M
 * @return     none
 * @note       This API can stop adc sampling and clear rx fifo.
 */
static inline void adc_clk_dis(void)
{
    reg_rst1 &= ~FLD_RST1_SAR;
}


/**
 * @brief    This function is used to power on sar_adc.
 * @return   none.
 * @note     -# User no longer need to wait after adc_power_on() for ADC to be stable.(confirmed by peng.liu,added by xiaobin.huang on 20250714)
 *           -# ADC and audio cannot use the m channel simultaneously.
 */
void adc_power_on(void);
/**
 * @brief      This function is used to power off sar_adc.
 * @return     none
 */
void adc_power_off(void);


/**
 * @brief This function serves to set input channel in differential_mode.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  p_ain - enum variable of ADC analog positive input pin.
 * @param[in]  n_ain - enum variable of ADC analog negative input pin.
 * @return none
 */
static inline void adc_set_diff_input(adc_sample_chn_e chn, adc_input_pch_e p_ain, adc_input_nch_e n_ain)
{
    analog_write(areg_adc_res_m, analog_read(areg_adc_res_m) | FLD_ADC_EN_DIFF_CHN_M);
    reg_adc_r_mux(chn) = (p_ain | (n_ain << 4));
}

/**
 * @brief      This function serves to enable adc's interrupt mask in NDMA INTERRUPT MODE and DMA MODE.
 * @param[in]  none
 * @return     none
 */
static inline void adc_set_irq_mask(void)
{
    reg_adc_config2 |= FLD_RX_IRQ_ENABLE;//ndma and dma
}

/**
 * @brief This function is used to set IO port for ADC supply or ADC IO port voltage sampling.
 * @param[in]  mode - ADC gpio pin sample mode
 * @param[in]  pin - adc_input_pin_def_e ADC input gpio pin
 * @return none
 */
void adc_pin_config(adc_input_pin_mode_e mode, adc_input_pin_def_e pin);
/**
 * @brief This function is used to set two IO port configuration and set it as input channel of ADC difference IO port voltage sampling.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  p_pin - enum variable of ADC analog positive input IO.
 * @param[in]  n_pin - enum variable of ADC analog negative input IO.
 * @return none
 */
void adc_set_diff_pin(adc_sample_chn_e chn, adc_input_pin_def_e p_pin, adc_input_pin_def_e n_pin);

/**
 * @brief      This function serves to select Vbat voltage division factor.
 * @param[in]  chn - enum variable of ADC sample channel
 * @param[in]  vbat_div - enum variable of Vbat division factor.
 * @return     none
 */
void adc_set_vbat_divider(adc_sample_chn_e chn, adc_vbat_div_e vbat_div);

/**
 * @brief This function is used to initialize the ADC.
 * @param[in]  channel_cnt - transfer_mode and the number of channels used.
 * @return none
 * @attention Many features are configured in adc_init function. But some features
 *      such as adc digital clk, adc analog clk, resolution, we think better to set as default value,
 *      and user don't need to change them in most use cases.
 */
void adc_init(adc_chn_cnt_e channel_cnt);
/**
 * @brief This function is used to initialize the ADC for gpio sampling.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  cfg -structure for configuring ADC channel.
 * @return none
 */
void adc_gpio_sample_init(adc_sample_chn_e chn, adc_gpio_cfg_t cfg);
/**
 * @brief This function is used to initialize the ADC for vbat sampling.
 * @param[in]  chn -structure for configuring ADC channel.
 * @return none
 */
void adc_vbat_sample_init(adc_sample_chn_e chn);

/**
 * @brief This function serves to calculate voltage from adc sample code.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   adc_code    - the adc sample code(should be positive value.)
 * @return      adc_vol_mv  - the average value of adc voltage value(adc voltage value >= 0).
 */
unsigned short adc_calculate_voltage(adc_sample_chn_e chn, unsigned short adc_code);


/**
 * @brief This function is used to calib ADC 1.2V vref for GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_gpio_calib_vref(unsigned short vref, signed char offset);

/**
 * @brief This function is used to calib ADC 1.2V vref for Vbat.
 * @param[in] vref - Vbat channel sampling calibration value.
 * @param[in] offset - Vbat channel sampling two-point calibration value offset.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short vref, signed char offset);

/**********************************************************************************************************************
 *                                                NDMA only interface                                                 *
 **********************************************************************************************************************/

/**
 * @brief     This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @param[in] none.
 * @return    none
 */
void adc_start_sample_nodma(void);

/**
 * @brief     This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @param[in] none.
 * @return    none
 */
void adc_stop_sample_nodma(void);


/**
 * @brief      This function performs to enable audio input of DFIFO1.
 * @param[in]  none.
 * @return     none.
 */
void adc_enable_dfifo1(void);
/**
 * @brief      This function performs to disable input of DFIFO1.
 * @param[in]  none.
 * @return     none.
 */
void adc_disable_dfifo1(void);

/**
 * @brief      This function performs to set MISC channel.
 * @param[in]  pbuff - address in FIFO1.
 * @param[in]  size_buff - depth of FIFO1.
 * @return     none.
 */
void adc_config_misc_channel_buf(unsigned short* pbuff,unsigned int size_buff);

/**
 * @brief This function serves to set adc sampling and get results.
 * @param[in]  none.
 * @return the result of sampling.
 */
unsigned short adc_sample_and_get_result(void);

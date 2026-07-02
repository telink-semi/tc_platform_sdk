/********************************************************************************************************
 * @file    adc.h
 *
 * @brief   This is the header file for tc122x
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
 *  TC122X supports hardware ADC function.
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

extern unsigned char g_adc_rx_fifo_index;

typedef enum
{
	ADC_VREF_0P9V    = 0x01,
    ADC_VREF_1P2V    = 0x00,
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
	reg_adc_config2 = (reg_adc_config2 & (~FLD_SAR_IRQ_RX_STATUS)) | FLD_RX_IRQ_ENABLE;//ndma and dma
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
 * @brief       This function sets the threshold that triggers the DMA IRQ or SAR ADC RX IRQ.
 * @param[in]   trig_num -trigger threshold.
 * @return      none
 * @note        -# The DMA IRQ or SAR ADC RX IRQ is triggered when fifo_cnt> trig_cnt.
 *              -# unit: word, range:0-7(the actual fifo depth is 8 words, but the register is only 3 bits, so it is limited to 0~7).
 *              -# The data of rx fifo is stored according to HALF word.
 *              -# In DMA mode, DMA is configured to transmit by word, so the trigger cnt must be set to an odd number, otherwise there will be incorrect data to the buffer.
 *              -# In DMA mode, it has been configured by default to 1 (greater than 1 half word, i.e., 1 word triggers a DMA request to the DMA once ).
 *              -# In NDMA mode, the threshold can be configured according to the usage situation.
 *              -# The threshold is recalculated after the dma has finished moving.
 *              -# trig_num must be 0 in dma mode or it will cause problems.
 */
static inline void adc_set_rx_fifo_trig_cnt(unsigned char trig_num)
{
	reg_adc_rxfifo_trig_num = ((reg_adc_rxfifo_trig_num & (~FLD_RXFIFO_TRIG_NUM)) | (trig_num << 4));
}

/**
 * @brief       This function sets the rx length of dma,.
 * @param[in]   length -dma irq trigger length. (length max is 0x3fa)
 * @return      none
 * @note        -# When the DMA reaches the configured transfer length, it will automatically trigger a DMA interrupt and stop transferring data.
 *                 However, the ADC will continue running. Therefore, after entering the interrupt, you must call adc_clk_dis() to disable the ADC clock,
 *                 which stops the ADC and simultaneously clears the FIFO data.
 *              -# For the next sampling cycle, simply call adc_clk_en(), the ADC will resume sampling, and the DMA will automatically restart
 *                 without requiring reconfiguration or re-enabling.
 */
static inline void adc_set_dma_rx_length(unsigned short length)
{
	reg_dma_length = length;
}

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
 *                                                DMA only interface                                                  *
 **********************************************************************************************************************/
/**
 * @brief     The adc starts sampling in DMA mode.
 * @param[in] none
 * @return    none
 */
void adc_start_sample_dma(void);

/**
 * @brief     This stops sampling in DMA mode.
 * @param[in] none.
 * @return    none
 */
void adc_stop_sample_dma(void);

/**
 * @brief     This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo and clear rx fifo cnt and g_adc_rx_fifo_index.
 * @param[in] rx_buffer - Pointer to data buffer, it must be 4-bytes aligned address
 * @param[in] sample_num -The total number of sampling code
 * @return    none
 * @note      -# Since the write-back feature of ADC's DMA5 is enabled by default and cannot be disabled, and the length information occupies 1 word,
 *             the buffer must be allocated with 1 additional word of space.
 */
void adc_rx_buff_init(unsigned int *rx_buffer, unsigned int sample_num);
/**********************************************************************************************************************
 *                                                NDMA only interface                                                 *
 **********************************************************************************************************************/
/**
 * @brief     This function serves to directly get an adc sample code from fifo.
 * @param[in] none.
 * @return    adc_code    - the adc sample code.
 */
unsigned int adc_get_raw_code(void);

/**
 * @brief     Get the irq status of ADC.
 * @param[in] none.
 * @retval    non-zero      -  the interrupt occurred.
 * @retval    zero  -  the interrupt did not occur.
 */
static inline unsigned char adc_get_irq_status(void)
{
    return reg_adc_config2 & FLD_SAR_IRQ_RX_STATUS;
}

/**
 * @brief     Clear the irq status of ADC.
 * @return    none
 * @note      The irq status of ADC will not be cleared automatically after reading out the data from the rxfifo, you need to clear manually.
 */
static inline void adc_clr_irq_status(void)
{
	reg_adc_config2 |= FLD_SAR_IRQ_RX_STATUS;
}

/**
 * @brief     This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @param[in] none.
 * @return    none
 */
void adc_start_sample_nodma(void);

/**
 * @brief     This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo and clear rx fifo cnt and g_adc_rx_fifo_index.
 * @param[in] none.
 * @return    none
 */
void adc_stop_sample_nodma(void);


/**
 * @brief     Get the rxfifo cnt,when data enters rxfifo, the rxfifo cnt increases; when reading data from rx_fifo, rxfifo cnt decays.
 * @param[in] none.
 * @return    none
 * @note      After waking up from suspend mode,rxfifo will not be cleared.(confirmed by qiangkai.xu, added by xiaobin.huang at 20240903)
 */
static inline unsigned char adc_get_rxfifo_cnt(void)
{
    return ((reg_adc_rxfifo_trig_num & FLD_RX_FIFO_NUM));
}

/**
 * @brief     Set the 'g_adc_rx_fifo_index' to 0,'g_adc_rx_fifo_index' is used to synchronize the rxfifo hardware pointer in NDMA mode.
 * @param[in] none.
 * @return    none.
 */
static inline void adc_clr_rx_index(void)
{
    g_adc_rx_fifo_index = 0;
}

/**
 * @brief     Clear the ADC rx fifo.
 * @param[in] none.
 * @return    none
 */
static inline void adc_clr_rx_fifo_cnt(void)
{
    reg_adc_r_tsamp |= FLD_RX_FIFO_CLR;
    adc_clr_rx_index();
}



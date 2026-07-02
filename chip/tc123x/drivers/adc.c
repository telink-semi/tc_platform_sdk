/********************************************************************************************************
 * @file    adc.c
 *
 * @brief   This is the source file for tc123x
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
#include "adc.h"
#include "compiler.h"
#include "lib/include/stimer.h"
#include "dma.h"
#define ADC_CHN_CNT                        3

//The default value is provided by ATE based on a large batch of test data.
#define ADC_GPIO_VREF_DEFAULT_VALUE        1202
#define ADC_GPIO_VREF_OFFSET_DEFAULT_VALUE 0

#define ADC_VBAT_VREF_DEFAULT_VALUE        1210
#define ADC_VBAT_VREF_OFFSET_DEFAULT_VALUE 0

_attribute_data_retention_ unsigned short g_adc_vref[ADC_CHN_CNT] = {ADC_GPIO_VREF_DEFAULT_VALUE, ADC_VBAT_VREF_DEFAULT_VALUE, ADC_GPIO_VREF_DEFAULT_VALUE}; //default ADC ref voltage (unit:mV)
_attribute_data_retention_ signed char    g_adc_vref_offset[ADC_CHN_CNT];                                                                                    //ADC calibration value voltage offset (unit:mV).
_attribute_data_retention_ unsigned short g_adc_gpio_calib_vref        = ADC_GPIO_VREF_DEFAULT_VALUE;                                                        //ADC gpio calibration value voltage (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_ signed char    g_adc_gpio_calib_vref_offset = ADC_GPIO_VREF_OFFSET_DEFAULT_VALUE;                                                 //ADC gpio calibration value voltage offset (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_ unsigned short g_adc_vbat_calib_vref        = ADC_VBAT_VREF_DEFAULT_VALUE;                                                        //ADC vbat calibration value voltage (unit:mV)(used for vbat voltage sample).
_attribute_data_retention_ signed char    g_adc_vbat_calib_vref_offset = ADC_VBAT_VREF_OFFSET_DEFAULT_VALUE;                                                 //ADC vbat calibration value voltage offset (unit:mV)(used for vbat voltage sample).


volatile unsigned char g_adc_pre_scale[ADC_CHN_CNT];
volatile unsigned char g_adc_vbat_divider[ADC_CHN_CNT];
unsigned char          g_channel_cnt       = 0;
adc_sample_freq_e      g_sample_freq       = 0;
volatile unsigned short adc_code;

/**********************************************************************************************************************
 *                                         DMA and NDMA common interface                                              *
 **********************************************************************************************************************/
/**
 * @brief      This function is used to reset sar_adc module.
 * @return     none
 */
static inline void adc_reset(void)
{
	reg_rst1 &= ~FLD_RST1_SAR;
    reg_rst1 |= FLD_RST1_SAR;
}


/**
 * @brief      This function sets adc digital clock and analog clock.
 * @return     none
 * @attention  ADC digital clock from crystal(24MHz), analog clock from digital clock.
 *             The ADC digital clock and analog clock are fixed at 24 MHz and 4 MHz, respectively, and should not be modified.
 */
void adc_set_clk(void)
{
    reg_adc_config1 = ((reg_adc_config1 & FLD_SAR_ADC_CLK_DIV) | 1); //div=1, adc digital clk = 24MHz/div.(crystal = 24MHz)
    analog_write(areg_adc_sample_clk_div, 5);                   //div=5, adc analog clk = 24MHz/(1+div) = 4M.
}

/**
 * @brief      This function sets ADC input channel as M channel,L channel or R channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @return     none
 */
static inline void adc_set_chn_en(adc_sample_chn_e chn)
{
    reg_adc_config2 |= BIT(chn);
}

/**
 * @brief This function serves to set resolution.
 * @param[in]  res - enum variable of ADC resolution.
 * @return none
 */
void adc_set_resolution(adc_res_e res)
{
    analog_write(areg_adc_res_m, (analog_read(areg_adc_res_m) & (~FLD_ADC_RES_M)) | res);
}

/**
 * @brief This function serves to set sar adc sample mode.
 * @param[in]  sar_adc_mode_e - enum variable of sar adc mode.
 * @return none
 */
void adc_set_adc_sample_mode(sar_adc_mode_e mode)
{
    analog_write(areg_adc_res_m, (analog_read(areg_adc_res_m) & (~FLD_ADC_EN_DIFF_CHN_M)) | MASK_VAL(FLD_ADC_EN_DIFF_CHN_M, mode));
}

/**
 * @brief      This function serves to set ADC sample time(the number of adc clocks for sample cycles) for M channel,L channel or R channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  sample_cycle - enum variable of adc sample cycles.
 * @return     none
 */
void adc_set_tsample_cycle(adc_sample_chn_e chn, adc_sample_cycle_e sample_cycle)
{
    switch (chn) {
    case ADC_M_CHANNEL:
        reg_adc_tsamp = ((reg_adc_tsamp & (~FLD_M_TSAMP)) | sample_cycle);
        break;
    case ADC_L_CHANNEL:
        reg_adc_tsamp = ((reg_adc_tsamp & (~FLD_L_TSAMP)) | (sample_cycle << 4));
        break;
    case ADC_R_CHANNEL:
        reg_adc_r_tsamp = ((reg_adc_r_tsamp & (~FLD_R_TSAMP)) | sample_cycle);
        break;
    default:
        break;
    }
}

/**
 * @brief This function serves to set state and capture_state length.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   r_max_mc - Value of length of "capture" state for channel.
 * @param[in]   r_max_s - Value of length of "set" state for channel.
 * @return none
 */
void adc_set_state_length(adc_sample_chn_e chn, unsigned short r_max_mc, unsigned char r_max_s)
{
    reg_adc_capture_state(chn)     = r_max_mc;
    reg_adc_channel_set_state(chn) = ((reg_adc_channel_set_state(chn) & (~FLD_R_MAX_S)) | r_max_s);
}

/**
 * @brief   This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @return  none
 * @note    -# In DMA mode, must enable this function.
 *          -# In NDMA mode, if this function is not enabled, the adc code can only be read from the registers, but it will lead to the problem of repeatedly getting the same adc code when calling adc_get_raw_code() several times,
 *             if this function is enabled, the adc code can be read from the rx fifo, and this problem can be avoided, so it is also must enable it.
 */
static inline void adc_all_chn_data_to_fifo_en(void)
{
    analog_write(areg_adc_data_transfer_control, analog_read(areg_adc_data_transfer_control) & (~FLD_AUTO_NOT_EN)); //Used to enable the transfer of data from the adc's M-channel, L-channel, and R-channel to the sar adc rxfifo.
}

/**
 * @brief   This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @return  none
 */
static inline void adc_all_chn_data_to_fifo_dis(void)
{
    analog_write(areg_adc_data_transfer_control, FLD_AUTO_NOT_EN);
}

/**
 * @brief       This function is used to set the scan channel cnt.
 * @param[in]   chn_cnt - range:1-3
 * @return      none
 */
static inline void adc_set_scan_chn_cnt(unsigned char chn_cnt)
{
    reg_adc_config0 = ((reg_adc_config0 & (~FLD_SCANT_MAX)) | ((chn_cnt * 2) << 4)); //scan_cnt = chn_cnt*2
}


/**
 * @brief      This function disable adc digital clock.
 * @return     none
 * @note       -# After disabling the digital clock, rxfifo will not be updated.
 *             -# When you don't need to keep the adc sampling, you can disable the digital clock for power saving purposes.
 */
static inline void adc_dig_clk_dis(void)
{
    reg_adc_config2 &= (~FLD_CLK_EN);
}

/**
 * @brief      This function enable adc digital clock.
 * @return     none
 */
static inline void adc_dig_clk_en(void)
{
    reg_adc_config2 |= FLD_CLK_EN;
}

/**
 * @brief      This function serves to stop the state machine at the beginning of the M channel and not start sampling.
 * @return     none
 */
static inline void adc_set_scan_chn_dis(void)
{
    /**
     * FLD_SCANT_MAX cannot be configured to 0, because 0 is the RNG channel, if RNG is not enabled (default is not enabled), the state machine will not stop at the RNG channel after setting 0, it will not stop.
     */
    reg_adc_config0 = ((reg_adc_config0 & (~FLD_SCANT_MAX)) | (1 << 4));
}

/**
 * @brief    This function is used to power on sar_adc.
 * @return   none.
 * @note     -# User no longer need to wait after adc_power_on() for ADC to be stable.(confirmed by peng.liu,added by xiaobin.huang on 20250714)
 *           -# ADC and audio cannot use the m channel simultaneously.
 */
void adc_power_on(void)
{
    /**
     * adc_set_scan_chn_dis() must be called to stop the state machine at the beginning of the M channel and not start sampling.
     */
    adc_set_scan_chn_dis();
    analog_write(areg_adc_pga_ctrl, (analog_read(areg_adc_pga_ctrl) & (~FLD_SAR_ADC_POWER_DOWN)));
    adc_reset();                   //reset whole digital adc module
    adc_dig_clk_en();
    reg_adc_config2 |= FLD_R_SOFT_EN; //Both adc and audio can use the m channel, When using ADC, the R_SOFT_EN bit needs to be set to 1
}

/**
 * @brief      This function is used to power off sar_adc.
 * @return     none
 */
void adc_power_off(void)
{
    analog_write(areg_adc_pga_ctrl, (analog_read(areg_adc_pga_ctrl) | FLD_SAR_ADC_POWER_DOWN));
    adc_dig_clk_dis();
    reg_adc_config2 &= (~FLD_R_SOFT_EN); //Both adc and audio can use the m channel, When not using ADC, the R_SOFT_EN bit needs to be set to 0
}

/**
 * @brief This function is used to set IO port for ADC supply or ADC IO port voltage sampling.
 * @param[in]  mode - ADC gpio pin sample mode
 * @param[in]  pin - adc_input_pin_def_e ADC input gpio pin
 * @return none
 */
void adc_pin_config(adc_input_pin_mode_e mode, adc_input_pin_def_e pin)
{
    unsigned short adc_input_pin = pin & 0xfff;
    switch (mode) {
    case ADC_GPIO_MODE:
        gpio_set_func(adc_input_pin,AS_GPIO);
        gpio_set_output_en(adc_input_pin, 0);
        gpio_set_input_en(adc_input_pin,0);
        gpio_write(adc_input_pin, 0);
        break;
    case ADC_VBAT_MODE:
        gpio_set_func(adc_input_pin,AS_GPIO);
        gpio_set_output_en(adc_input_pin, 1);
        gpio_set_input_en(adc_input_pin,0);
        gpio_write(adc_input_pin, 1);
        break;
    }
}

/**
 * @brief This function is used to set two IO port configuration and set it as input channel of ADC difference IO port voltage sampling.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  p_pin - enum variable of ADC analog positive input IO.
 * @param[in]  n_pin - enum variable of ADC analog negative input IO.
 * @return none
 */
void adc_set_diff_pin(adc_sample_chn_e chn, adc_input_pin_def_e p_pin, adc_input_pin_def_e n_pin)
{
    adc_pin_config(ADC_GPIO_MODE, p_pin);
    adc_pin_config(ADC_GPIO_MODE, n_pin);
    adc_set_diff_input(chn, p_pin >> 12, n_pin >> 12);
}

/**
 * @brief This function serves to set the reference voltage of the channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @return none
 * @note       adc_set_ref_voltage does not take effect immediately after configuration, it needs to be delayed 100us after calling adc_dig_clk_en().
 */
static void adc_set_ref_voltage(adc_sample_chn_e chn,adc_ref_vol_e v_ref)
{
    reg_adc_channel_set_state(chn) = (reg_adc_channel_set_state(chn) & (~FLD_SEL_VREF)) | (v_ref << 6);
    //For TC122x, it is best to configure areg_ain_scale as 0x15, which is different from that of TL751X.(confirmed by peng.liu,added by xiaobin.huang on 20250714)
    analog_write(areg_ain_scale, (analog_read(areg_ain_scale) & (0xC0)) | 0x15);
}
/**
 * @brief This function serves to set the sample frequency.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  sample_freq - enum variable of ADC sample frequency.
 * @return none
 */
static void adc_set_sample_rate(adc_sample_chn_e chn, adc_sample_freq_e sample_freq)
{
    unsigned char  adc_set          = sample_freq & 0xf;
    unsigned char  adc_sample_cycle = (sample_freq >> 4) & 0xf;
    unsigned short adc_capture      = (sample_freq >> 8) & 0xffff;
    adc_set_state_length(chn, adc_capture, adc_set);
    adc_set_tsample_cycle(chn, adc_sample_cycle);
}

/**
 * @brief This function serves to set pre_scaling factor.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  pre_scale - enum variable of ADC pre_scaling factor.
 * @return none
 */
static inline void adc_set_scale_factor(adc_sample_chn_e chn, adc_pre_scale_e pre_scale)
{
    reg_adc_channel_set_state(chn) = ((reg_adc_channel_set_state(chn) & (~FLD_SEL_AI_SCALE)) | (pre_scale << 4));
    g_adc_pre_scale[chn]           = 1 << pre_scale;
}

/**
 * @brief      This function serves to select Vbat voltage division factor.
 * @param[in]  chn - enum variable of ADC sample channel
 * @param[in]  vbat_div - enum variable of Vbat division factor.
 * @return     none
 * @note       adc_set_vbat_divider() does not take effect immediately after configuration, it needs to be delayed 100us after calling adc_dig_clk_en().
 */
void adc_set_vbat_divider(adc_sample_chn_e chn, adc_vbat_div_e vbat_div)
{
    unsigned char offset = (chn == 0) ? 0 : (1 << chn);
    reg_adc_vbat_div = (reg_adc_vbat_div & (~BIT_RNG(offset,offset+1)))| ((vbat_div) << offset);
    g_adc_vbat_divider[chn] = vbat_div ? (5 - vbat_div) : 1;
}


/**
 * @brief This function is used to initialize the ADC.
 * @param[in]  channel_cnt - transfer_mode and the number of channels used.
 * @return none
 * @attention Many features are configured in adc_init function. But some features
 *      such as adc digital clk, adc analog clk, resolution, we think better to set as default value,
 *      and user don't need to change them in most use cases.
 */
void adc_init(adc_chn_cnt_e channel_cnt)
{
    adc_power_off();               //power off sar adc
    adc_reset();                   //reset whole digital adc module
    adc_set_clk();                 //set adc digital clk to 24MHz and adc analog clk to 4MHz
    adc_set_resolution(ADC_RES10); //default adc_resolution set as 10bit ,BIT(9) is sign bit
    adc_all_chn_data_to_fifo_en();
    g_channel_cnt = channel_cnt;
}

/**
 * @brief This function is used to configure the channel of the ADC.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  adc_cfg -structure for configuring ADC channel.
 * @return none
 */
void adc_chn_config(adc_sample_chn_e chn, adc_chn_cfg_t adc_cfg)
{
    adc_set_diff_input(chn, adc_cfg.input_p, adc_cfg.input_n);
    adc_set_vbat_divider(chn, adc_cfg.divider);
    adc_set_ref_voltage(chn, adc_cfg.v_ref);
    adc_set_scale_factor(chn, adc_cfg.pre_scale);
    adc_set_sample_rate(chn, adc_cfg.sample_freq);
    adc_set_chn_en(chn);
    g_sample_freq = adc_cfg.sample_freq;
}

/**
 * @brief This function is used to initialize the ADC for gpio sampling.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  cfg -structure for configuring ADC channel.
 * @return none
 */
void adc_gpio_sample_init(adc_sample_chn_e chn, adc_gpio_cfg_t cfg)
{
    adc_pin_config(ADC_GPIO_MODE, cfg.pin);
    adc_chn_cfg_t chn_cfg =
    {
            .divider     = ADC_VBAT_DIV_OFF,
            .v_ref       = cfg.v_ref,
            .pre_scale   = cfg.pre_scale,
            .sample_freq = cfg.sample_freq,
            .input_p     = cfg.pin >> 12,
            .input_n     = GND,
    };
    adc_chn_config(chn, chn_cfg);

    g_adc_vref[chn]        = g_adc_gpio_calib_vref;        //set gpio sample calib vref
    g_adc_vref_offset[chn] = g_adc_gpio_calib_vref_offset; //set adc_vref_offset as adc_gpio_calib_vref_offset
}

/**
 * @brief This function is used to initialize the ADC for vbat sampling.
 * @param[in]  chn -structure for configuring ADC channel.
 * @return none
 */
void adc_vbat_sample_init(adc_sample_chn_e chn)
{
    adc_chn_cfg_t chn_cfg =
    {
        /**
         * For TC123X, divider=ADC_VBAT_DIV_1F4 / pre_scale=ADC_PRESCALE_1 are the best and do not need to be configured as
         * divider=ADC_VBAT_DIV_1F4 / pre_scale=ADC_PRESCALE_1F2 as same as TL751X.(confirmed by peng.liu,added by xiaobin.huang on 20250714)
         */
        .divider     = ADC_VBAT_DIV_1F4,
        .v_ref       = ADC_VREF_1P2V,
        .pre_scale   = ADC_PRESCALE_1,
        .sample_freq = ADC_SAMPLE_FREQ_96K,
        .input_p     = ADC_VBAT,
        .input_n     = GND,

    };
    adc_chn_config(chn, chn_cfg);

    g_adc_vref[chn]        = g_adc_vbat_calib_vref;        //set vbat sample calib vref
    g_adc_vref_offset[chn] = g_adc_vbat_calib_vref_offset; //set g_adc_vref_offset as g_adc_vbat_calib_vref_offset
}

#if 0
void adc_temp_sample_init(adc_sample_chn_e chn)
{
    adc_chn_cfg_t chn_cfg =
    {
        /**
         * For TC123X, divider=ADC_VBAT_DIV_1F4 / pre_scale=ADC_PRESCALE_1 are the best and do not need to be configured as
         * divider=ADC_VBAT_DIV_1F4 / pre_scale=ADC_PRESCALE_1F2 as same as TL751X.(confirmed by peng.liu,added by xiaobin.huang on 20250714)
         */
        .divider     = ADC_VBAT_DIV_1F4,
        .v_ref       = ADC_VREF_1P2V,
        .pre_scale   = ADC_PRESCALE_1,
        .sample_freq = ADC_SAMPLE_FREQ_96K,
        .input_p     = ADC_VBAT,
        .input_n     = GND,

    };
    adc_chn_config(chn, chn_cfg);

    g_adc_vref[chn]        = g_adc_vbat_calib_vref;        //set vbat sample calib vref
    g_adc_vref_offset[chn] = g_adc_vbat_calib_vref_offset; //set g_adc_vref_offset as g_adc_vbat_calib_vref_offset
}
#endif
/**
 * @brief This function serves to calculate voltage from adc sample code.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   adc_code    - the adc sample code(should be positive value.)
 * @return      adc_vol_mv  - the average value of adc voltage value(adc voltage value >= 0).
 */
unsigned short adc_calculate_voltage(adc_sample_chn_e chn, unsigned short adc_code)
{
    /**
     *  adc sample code convert to voltage(mv):
     *  (adc code BIT<8~0> is valid data)
     *  adc_voltage  =  (adc_code * Vref * adc_pre_scale / 0x200) + offset
     *               =  (adc_code * Vref * adc_pre_scale >>9) + offset
     */
    unsigned short adc_voltage = (((adc_code * g_adc_vbat_divider[chn] * g_adc_pre_scale[chn] * g_adc_vref[chn]) >> 9) + g_adc_vref_offset[chn]);
    if(adc_voltage & BIT(15))
    {
        return 0;//When the adc_voltage < 0, the returned voltage value should be 0.
    }else
    {
        return adc_voltage;
    }
}
unsigned short adc_calculate_temp(adc_sample_chn_e chn, unsigned short adc_code)
{
    return 564 - ((adc_code * 819) >> 11);
}

/**
 * @brief This function is used to calib ADC 1.2V vref for GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_gpio_calib_vref(unsigned short vref, signed char offset)
{
    g_adc_gpio_calib_vref        = vref;
    g_adc_gpio_calib_vref_offset = offset;
}

/**
 * @brief This function is used to calib ADC 1.2V vref for Vbat.
 * @param[in] vref - Vbat channel sampling calibration value.
 * @param[in] offset - Vbat channel sampling two-point calibration value offset.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short vref, signed char offset)
{
    g_adc_vbat_calib_vref        = vref;
    g_adc_vbat_calib_vref_offset = offset;
}

/**********************************************************************************************************************
 *                                                NDMA only interface                                                 *
 **********************************************************************************************************************/
/**
 * @brief       This function is used to get the status of the valid adc code for the m channel.
 * @param[in]   none.
 * @return      0:Before sampling one adc_code at each time,m_chn_valid_status is automatically set to 0.
 *              !0:After sampling one adc_code at each time,m_chn_valid_status is automatically set to 1.
 * @attention   Must call adc_ana_read_en()API first to get the status of the valid adc code for the m channel.
 */
static inline unsigned char adc_get_m_chn_valid_status(void)
{
    return (analog_read(areg_m_chn_data_valid_status) & FLD_M_CHN_DATA_VALID_STATUS);
}

/**
 * @brief      This function performs to enable adc input of DFIFO1.
 * @param[in]  none.
 * @return     none.
 */
void adc_enable_dfifo1(void)
{
    reg_dfifo1_wptr = 0;  //clear dfifo1 write pointer
    reg_dfifo_manual_mode |= FLD_DFIFO_LPF3_BYPASS;
    reg_dfifo_mode |= FLD_AUD_DFIFO1_IN ;
}

/**
 * @brief      This function performs to disable input of DFIFO1.
 * @param[in]  none.
 * @return     none.
 */
void adc_disable_dfifo1(void)
{
    reg_dfifo_mode &= ~FLD_AUD_DFIFO1_IN;
}

/**
 * @brief      This function performs to set MISC channel.
 * @param[in]  pbuff - address in FIFO1.
 * @param[in]  size_buff - depth of FIFO1.
 * @return     none.
 */
void adc_config_misc_channel_buf(unsigned short* pbuff,unsigned int size_buff)
{
    reg_dfifo1_addr = (unsigned short)((unsigned int)pbuff);
    reg_dfifo1_size = (size_buff>>4)-1;
}

/**
 * @brief     This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel.
 * @param[in] none.
 * @return    none
 */
void adc_start_sample_nodma(void)
{
    adc_set_scan_chn_cnt(1);
    adc_clk_en();
}

/**
 * @brief     This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel.
 * @param[in] none.
 * @return    none
 */
void adc_stop_sample_nodma(void)
{
    adc_set_scan_chn_dis();
    adc_clk_dis();
}

#define ADC_SAMPLE_NUM      8 //8 /4

/**
 * @brief This function serves to set adc sampling and get results.
 * @param[in]  none.
 * @return the result of sampling.
 */
unsigned short adc_sample_and_get_result(void)
{
    unsigned short temp;
    volatile signed short adc_data_buf[ADC_SAMPLE_NUM];  //size must 16 byte aligned(16/32/64...)

    int i,j;
    unsigned short adc_sample[ADC_SAMPLE_NUM] = {0};
    unsigned short  adc_result=0;

    adc_reset();

    for(i=0;i<ADC_SAMPLE_NUM;i++){      //dfifo data clear
        adc_data_buf[i] = 0;
    }
    //dfifo setting will lose in suspend/deep, so we need config it every time
    adc_config_misc_channel_buf((unsigned short *)adc_data_buf, ADC_SAMPLE_NUM<<1);  //size: ADC_SAMPLE_NUM*2
    adc_enable_dfifo1();
    unsigned int t0 = clock_time();
    if(g_sample_freq==ADC_SAMPLE_FREQ_23K)
    {
        while(!clock_time_exceed(t0, 90));  //wait at least 2 sample cycle(f = 23K, T = 43.4us)
    }
    else if(g_sample_freq==ADC_SAMPLE_FREQ_48K)
    {
        while(!clock_time_exceed(t0, 43));  //wait at least 2 sample cycle(f = 48K, T = 20.8us)
    }
    else if(g_sample_freq==ADC_SAMPLE_FREQ_96K)
    {
        while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
    }
    else if(g_sample_freq==ADC_SAMPLE_FREQ_192K)
    {
        while(!clock_time_exceed(t0, 11));  //wait at least 2 sample cycle(f = 192K, T = 5.2us)
    }
//////////////// get adc sample data and sort these data ////////////////
    for(i=0;i<ADC_SAMPLE_NUM;i++){
        /*wait for new adc sample data, When the data is not zero and more than 1.5 sampling times (when the data is zero),
     The default data is already ready.*/
        if(g_sample_freq==ADC_SAMPLE_FREQ_23K)
        {
            while(!clock_time_exceed(t0, 90));  //wait at least 2 sample cycle(f = 23K, T = 43.4us)
        }
        else if(g_sample_freq==ADC_SAMPLE_FREQ_48K)
        {
            while(!clock_time_exceed(t0, 43));  //wait at least 2 sample cycle(f = 48K, T = 20.8us)
        }
        else if(g_sample_freq==ADC_SAMPLE_FREQ_96K)
        {
            while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
        }
        //Add 192K sample rate by jiarong.ji at 20220621
        else if(g_sample_freq==ADC_SAMPLE_FREQ_192K)
        {
            while(!clock_time_exceed(t0, 11));  //wait at least 2 sample cycle(f = 192K, T = 5.2us)
        }
        t0 = clock_time();
        if(adc_data_buf[i] & BIT(9)){  //10 bit resolution, BIT(9) is sign bit, 1 means negative voltage in differential_mode
            adc_sample[i] = 0;
        }
        else{
            adc_sample[i] = ((unsigned short)adc_data_buf[i] & 0x3FF);  //BIT(9..0) is valid adc result
        }

        //insert sort
        if(i){
            if(adc_sample[i] < adc_sample[i-1]){
                temp = adc_sample[i];
                adc_sample[i] = adc_sample[i-1];
                for(j=i-1;j>=0 && adc_sample[j] > temp;j--){
                    adc_sample[j+1] = adc_sample[j];
                }
                adc_sample[j+1] = temp;
            }
        }
    }


//////////////////////////////////////////////////////////////////////////////
     adc_disable_dfifo1();   //misc channel data dfifo disable
///// get average value from raw data(abandon some small and big data ), then filter with history data //////
#if (ADC_SAMPLE_NUM == 4)   //use middle 2 data (index: 1,2)
    unsigned short adc_average = (adc_sample[1] + adc_sample[2])/2;
#elif(ADC_SAMPLE_NUM == 8)  //use middle 4 data (index: 2,3,4,5)
    unsigned short adc_average = (adc_sample[2] + adc_sample[3] + adc_sample[4] + adc_sample[5])/4;
#endif
    adc_code=adc_result = adc_average;


    //When the code value is 0, the returned voltage value should be 0.
    if(adc_result == 0){
        return 0;
    }
    /**
     *  adc sample code convert to voltage(mv):
     *  (adc code BIT<8~0> is valid data)
     *  adc_voltage  =  (adc_code * Vref * adc_pre_scale / 0x200) + offset
     *               =  (adc_code * Vref * adc_pre_scale >>9) + offset
     */
    unsigned short adc_voltage = (((adc_code * g_adc_vbat_divider[0] * g_adc_pre_scale[0] * g_adc_vref[0]) >> 9) + g_adc_vref_offset[0]);
    if(adc_voltage & BIT(15))
    {
        return 0;//When the adc_voltage < 0, the returned voltage value should be 0.
    }else
    {
        return adc_voltage;
    }
}




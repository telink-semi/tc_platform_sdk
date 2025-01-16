/********************************************************************************************************
 * @file    sd_adc.c
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
#include "sd_adc.h"

/**
 * @brief      This function serves to set adc vmid.
 * @param[in]  en - SD_ADC_VMID_POWER_DOWN or SD_ADC_VMID_POWER_ON.
 * @return     none.
 */
void sd_adc_set_vmid(sd_adc_vmid_power_switch_e en)
{
    if (en)
    {
        /***enable vmid voltage and set vmid driving ability***/
        analog_write(areg_0x8e, (analog_read(areg_0x8e) & (~FLD_AUDIO_PD_VMID)) | (FLD_AUDIO_PD_VMIDSEL));
    }
    else
    {
        /***disable vmid voltage***/
        analog_write(areg_0x8e, (analog_read(areg_0x8f) | FLD_AUDIO_PD_VMID));
    }
}

/**
 * @brief     This function servers to power on SD_ADC.
 * @param[in] mode -sd_adc_mode_e
 * @return    none
 * @note      -# After sd_adc_power_on(SD_ADC_SAMPLE_MODE), must wait >160us(when the C10 capacitor on the development board is 10nF) for VMID power to stabilize, otherwise there will be hundreds of sample data will be abnormal.
 *            -# After each call to sd_adc_power_on(SD_ADC_SAMPLE_MODE) and wait >160us(when the C10 capacitor on the development board is 10nF), the first 4 codes sampled by sd_adc may be abnormal and need to be discarded.
 *
 */
void sd_adc_power_on(sd_adc_mode_e mode)
{
	sd_adc_set_vmid(SD_ADC_VMID_POWER_ON);
	switch (mode)
	{
		case SD_ADC_SAMPLE_MODE:
		    /**
		     * SD_ADC can not be used with audio amic at the same time, so dmic must be selected.
		     */
		    audio_set_codec_stream0_input_mode(DMIC_INPUT);
			BM_SET(reg_rst2, FLD_RST2_AIF);
			analog_write(areg_0x8c, analog_read(areg_0x8c) & (~FLD_L_PD_BUFFER));//power on two sd_adc buffer at the positive and negative.
			analog_write(areg_0x8e, (analog_read(areg_0x8e) & (~(FLD_AUDIO_PD_BIAS|FLD_AUDIO_PD_ADC))));
			break;
		case SD_ADC_AUDIO_MODE:
		    sd_adc_data_weighted_average_en();
		    analog_write(areg_0x8d, (analog_read(areg_0x8d) & (~(FLD_AUDIO_MUTE_PGA | FLD_AUDIO_PGA_DIS))));
		    analog_write(areg_0x8e, (analog_read(areg_0x8e) & (~(FLD_AUDIO_PD_PGA | FLD_AUDIO_PD_BIAS | FLD_AUDIO_PD_ADC))));
			break;
		default:
			break;
	}
}

/**
 * @brief     This function servers to power off SD_ADC.
 * @param[in] mode -sd_adc_mode_e
 * @return    none
 */
void sd_adc_power_off(sd_adc_mode_e mode)
{
	sd_adc_set_vmid(SD_ADC_VMID_POWER_DOWN);
	switch (mode)
	{
		case SD_ADC_SAMPLE_MODE:
			BM_CLR(reg_rst2, FLD_RST2_AIF);
		    analog_write(areg_0x8c, analog_read(areg_0x8c) | FLD_L_PD_BUFFER);//power down two sd_adc buffer at the positive and negative.
		    analog_write(areg_0x8e, (analog_read(areg_0x8e) | (FLD_AUDIO_PD_BIAS|FLD_AUDIO_PD_ADC)));
		    break;
		case SD_ADC_AUDIO_MODE:
		    analog_write(areg_0x8d, (analog_read(areg_0x8d) | FLD_AUDIO_MUTE_PGA | FLD_AUDIO_PGA_DIS));
		    analog_write(areg_0x8e, (analog_read(areg_0x8e) | FLD_AUDIO_PD_PGA | FLD_AUDIO_PD_BIAS | FLD_AUDIO_PD_ADC));
			break;
		default:
			break;
	}
}

/**
 * @brief      This function is used to initialize the positive and negative channels for gpio sampling.
 * @param[in]  p_pin - sd_adc_p_input_pin_def_e
 * @param[in]  n_pin - sd_adc_n_input_pin_def_e
 * @return     none
 */
void sd_adc_gpio_pin_init(sd_adc_p_input_pin_def_e p_pin,sd_adc_n_input_pin_def_e n_pin)
{
    //SD_ADC GPIO Init
    gpio_set_input_en(p_pin&0xfff,0);
    gpio_set_output_en(p_pin&0xfff,0);

    if(n_pin != SD_ADC_GNDN)
    {
        gpio_set_input_en(n_pin&0xfff,0);
        gpio_set_output_en(n_pin&0xfff,0);
    }

    sd_adc_set_diff_input(p_pin, n_pin);
}

/**
 * @brief      This function serves to select Vbat voltage division.
 * @param[in]  div - sd_adc_vbat_div_e.
 * @return     none
*/
void sd_adc_set_vbat_divider(sd_adc_vbat_div_e div)
{
    analog_write(areg_0x8d, (analog_read(areg_0x8d) & (~FLD_SEL_VBAT_DIV)) | div );
    g_sd_adc_divider = 8>>div;
}

/**
 * @brief      This function serves to select the positive and negative voltage divider of the gpio.
 * @param[in]  div - sd_adc_vbat_div_e.
 * @return     none
*/
void sd_adc_set_gpio_divider(sd_adc_gpio_p_chn_div_e p_div,sd_adc_gpio_n_chn_div_e n_div)
{
    analog_write(areg_sel_ana_input_div, (analog_read(areg_sel_ana_input_div)&(~(FLD_SEL_ANA_INPUT_P_DIV | FLD_SEL_ANA_INPUT_N_DIV))) | (n_div | (p_div<<2)) );
    g_sd_adc_divider = 8>>p_div;
}

/**
 * @brief      This function serves to enable vbat function.
 * @param[in]  none
 * @return     none
 */
void sd_adc_vbat_mode_en(void)
{
    analog_write(areg_0x8c, analog_read(areg_0x8c) | FLD_L_SEL_DIVOUT_P);
    analog_write(areg_0x8f, analog_read(areg_0x8f) | FLD_EN_VBAT);
}

/**
 * @brief      This function serves to disable vbat function.
 * @param[in]  none
 * @return     none
 */
void sd_adc_vbat_mode_dis(void)
{
    analog_write(areg_0x8c, analog_read(areg_0x8c) & (~FLD_L_SEL_DIVOUT_P));
    analog_write(areg_0x8f, analog_read(areg_0x8f) & (~FLD_EN_VBAT));
}

/**
 * @brief      This function is used to initialize the SD_ADC.
 * @param[in]  none
 * @return     none
 */
void sd_adc_init(void)
{
    sd_adc_data_to_sram_en();
    sd_adc_data_weighted_average_en();
}

/**
 * @brief      This function is used to initialize the SD_ADC for gpio sampling.
 * @param[in]  cfg -Pointer to configure the GPIO channel structure.
 * @return     none
 * @note       If you switch to gpio mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_gpio_sample_init(sd_adc_gpio_cfg_t *cfg)
{
	sd_adc_temp_sensor_power_off();//Turn off to reduce power
    sd_adc_vbat_mode_dis();
    sd_adc_set_clk(cfg->clk_div);
    sd_adc_gpio_pin_init(cfg->input_p,cfg->input_n);
    sd_adc_set_gpio_divider(cfg->p_div,cfg->n_div);
    sd_adc_set_downsample_rate(cfg->downsample_rate);
}

/**
 * @brief      This function is used to initialize the SD_ADC for vbat sampling.
 * @param[in]  clk_div       - system clock/sd_adc sample clock scale
 * @param[in]  div             - sd_adc_vbat_div_e.
 * @param[in]  downsample_rate - sd_adc_downsample_rate_e
 * @return     none
 * @note       If you switch to vbat mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_vbat_sample_init(unsigned char clk_div, sd_adc_vbat_div_e div, sd_adc_downsample_rate_e downsample_rate)
{
	sd_adc_temp_sensor_power_off();//Turn off to reduce power
    sd_adc_vbat_mode_en();
    sd_adc_set_clk(clk_div);
    sd_adc_set_vbat_divider(div);
    sd_adc_set_downsample_rate(downsample_rate);
}

/**
 * @brief      This function is used to initialize the SD_ADC for Temperature Sensor sampling.
 * @param[in]  clk_div       - system clock/sd_adc sample clock scale
 * @param[in]  downsample_rate - sd_adc_downsample_rate_e
 * @return     none
 * @note       If you switch to temp mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_temp_init(unsigned char clk_div, sd_adc_downsample_rate_e downsample_rate)
{
    sd_adc_vbat_mode_dis();
    sd_adc_set_clk(clk_div);
    sd_adc_set_diff_input(SD_ADC_TEMP_SENSOR_P, SD_ADC_GNDN);
    sd_adc_set_downsample_rate(downsample_rate);
    sd_adc_temp_sensor_power_on();
}

/**
 * @brief       This function is used to convert SD_ADC sample code to a voltage.
 * @param[in]   sd_adc_code    - the sd_adc sample code.
 * @param[in]   type       - sd_adc_result_type_e.
 * @return      the average value of adc voltage value.
 */
signed int sd_adc_calculate_voltage(signed int sd_adc_code,sd_adc_result_type_e type)
{
    //When the code value is 0, the returned voltage value should be 0.
    if(sd_adc_code == 0)
    {
        return 0;
    }
    else
    {
        //code to vol: vol = (code*1000/(OSR^3)/2)*diviver (unit:mv)
        if(type == SD_ADC_VOLTAGE_10X_MV)
        {
            return  ((signed long long)((float)sd_adc_code*(float)10/(float)g_sd_adc_downsample_rate*(float)100/(float)g_sd_adc_downsample_rate*(float)10/(float)g_sd_adc_downsample_rate/(float)2*(float)g_sd_adc_divider));
        }else if(type == SD_ADC_VOLTAGE_MV)
        {
            return  ((signed long long)((float)sd_adc_code*(float)10/(float)g_sd_adc_downsample_rate*(float)100/(float)g_sd_adc_downsample_rate/(float)g_sd_adc_downsample_rate/(float)2*(float)g_sd_adc_divider));
        }else
        {
            return 0;
        }
    }

}

/**
 * @brief       This function is used to convert SD_ADC sample code to celsius value.
 * @param[in]   sd_adc_code    - the adc sample code.
 * @return      Celsius value.
 */
signed short sd_adc_calculate_temperature(signed int sd_adc_code)
{
	//sd_adc sample voltage convert to temperature: T = ((884 - V) / 1.4286) - 40 (unit:Celsius)
     return ((signed long long)(((float)884 - ((float)sd_adc_code*(float)10/(float)g_sd_adc_downsample_rate*(float)10/(float)g_sd_adc_downsample_rate*(float)10/(float)g_sd_adc_downsample_rate/(float)2)) * (float)10000/(float)14286 ) - 40 );
}



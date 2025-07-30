/********************************************************************************************************
 * @file    calibration.c
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
#include "calibration.h"
#include "driver.h"
#include "types.h"

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  addr - the frequency offset value address of flash.
 * @return 	   1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(unsigned int addr)
{
	unsigned char frequency_offset_value = 0xff;
	flash_read_page(addr, 1, &frequency_offset_value);
	if(0xff != (0xff&frequency_offset_value))
	{
		rf_update_internal_cap(frequency_offset_value);
		return 1;
	}
	return 0;
}

/**
 * @brief       This function is used to tighten the judgment of illegal values for gpio calibration and vbat calibration in the flash.
 * @param[in]   gain - the value of single_gpio_gain_10000x ,diff_gpio_gain_10000x and vbat_gain_10000x
 *              offset - the value of single_gpio_offset_10x ,diff_gpio_offset_10x and vbat_offset_10x
 *              calib_func - Function pointer to gpio_calibration or vbat_calibration.
 * @return      1:the calibration function is invalid; 0:the calibration function is valid.
 */
unsigned char flash_set_adc_calib_value(unsigned short gain, signed short offset, void (*calib_func)(unsigned short, signed short))
{
    /**
     * The legal range of gain for single_gpio/diff_gpio and vbat in flash is [9000,11000],
     * and the legal range of offset for single_gpio/diff_gpio and vbat is [-1000,1000].
     */
    if ((gain >= 9000) && (gain <= 11000) && (offset >= -1000) && (offset <= 1000)) {
        (*calib_func)(gain, offset);
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief      This function servers to get calibration value from flash.
 * @param[in]  addr - the  calibration value address of flash.
 * @return     true - the calibration value update, false - the calibration value is not update.
 */
bool user_calib_sd_adc(unsigned int addr)
{
	sd_adc_calib_t calib_value;
	flash_read_page(addr, 12,  (unsigned char*)&calib_value);

    if (flash_set_adc_calib_value(calib_value.single_gpio_gain_10000x, calib_value.single_gpio_offset_10x, adc_set_single_gpio_calib_vref) ||
        flash_set_adc_calib_value(calib_value.vbat_gain_10000x, calib_value.vbat_offset_10x, adc_set_vbat_calib_vref) ||
		flash_set_adc_calib_value(calib_value.diff_gpio_gain_10000x, calib_value.diff_gpio_offset_10x, adc_set_diff_gpio_calib_vref) )
    {
        return false;
    }else
    {
    	return true;
    }
}
/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in flash,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * 				(reduce adc measurement error, reduce frequency offset, etc.)
 * @return		none.
 */
void user_read_flash_value_calib(void)
{
	unsigned char flash_mid[4];
	unsigned char flash_uid[16];
	unsigned char flash_mid_sure = 0;
	/******check for flash mid********/
	flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);
	if (1 == flash_mid_sure)
	{
		switch (flash_mid[2])
		{
			case FLASH_SIZE_64K:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_64K);
				user_calib_sd_adc(FLASH_ADC_VREF_CALIB_ADDR_64K);
				break;
			case FLASH_SIZE_128K:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_128K);
				user_calib_sd_adc(FLASH_ADC_VREF_CALIB_ADDR_128K);
				break;
			case FLASH_SIZE_512K:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_512K);
				user_calib_sd_adc(FLASH_ADC_VREF_CALIB_ADDR_512K);
				break;
			case FLASH_SIZE_1M:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_1M);
				user_calib_sd_adc(FLASH_ADC_VREF_CALIB_ADDR_1M);
				break;
			case FLASH_SIZE_2M:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_2M);
				user_calib_sd_adc(FLASH_ADC_VREF_CALIB_ADDR_2M);
				break;
			case FLASH_SIZE_4M:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_4M);
				user_calib_sd_adc(FLASH_ADC_VREF_CALIB_ADDR_4M);
				break;
			default:
				break;
		}
	}
}


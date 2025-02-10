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
 * @brief		This function is used to Tighten the judgment of illegal values for gpio calibration and vbat calibration in the otp.
 *              The ADC vref gain calibtation should range from 1100mV to 1300mV, the ADC vref offset calibration should range from -40mV to 100mV.
 * @param[in]   gain - the value of gpio_calib_vref_gain or vbat_calib_vref_gain
 *              offset - the value of gpio_calib_vref_offset or vbat_calib_vref_offset
 *              calib_func - Function pointer to gpio_calibration or vbat_calibration.
 * @return		false:the calibration function is invalid; true:the calibration function is valid.
 */
bool adc_update_vref_calib_value_ft_cp(unsigned char gain, signed char offset, void (*calib_func)(unsigned short, signed char))
{
	/**
	 * The stored offset value is not of "signed" type, and the ATE writes the offset value with the following rules:
	 * Bit[7] = 1 for negative value, Bit[7] = 0 for positive value, and the absolute value of Bit[0:6] indicates the absolute value of the offset.
	 * Therefore, after taking out the offset value, it needs to be converted to "signed" type.
	 */
	offset = (offset & BIT(7)) ? ((-1) * (offset & 0x7f)) : offset;
	if((gain >= 100) && (offset >= -20) && (offset <= 120))
	{
		(*calib_func)(gain+1000, offset-20);
		return true;
	}
	else
	{
		return false;
	}
}
/**
 * @brief		This function is used to Tighten the judgment of illegal values for gpio calibration and vbat calibration in the flash.
 *              The ADC vref gain calibtation should range from 1047mV to 1300mV, the ADC vref offset calibration should range from 0mV to 127mV.
 * @param[in]   gain - the value of gpio_calib_vref_gain or vbat_calib_vref_gain
 *              offset - the value of gpio_calib_vref_offset or vbat_calib_vref_offset
 *              calib_func - Function pointer to gpio_calibration or vbat_calibration.
 * @return		false:the calibration function is invalid; true:the calibration function is valid.
 */
bool adc_update_vref_calib_value_flash(unsigned char gain, signed char offset, void (*calib_func)(unsigned short, signed char))
{
	/**
	 * The stored offset value with the following rules:
	 * Bit[7] = 1 for negative value, Bit[7] = 0 for positive value, and the absolute value of Bit[0:6] indicates the absolute value of the offset.
	 * Therefore, after taking out the offset value, it needs to be converted to "signed" type.
	 */
	offset = (offset & BIT(7)) ? ((-1) * (offset & 0x7f)) : offset;
	if((gain >= 47) && (offset >= 20) && (offset <= 147))
	{
		(*calib_func)(gain+1000, offset-20);
		return true;
	}
	else
	{
		return false;
	}
}
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
				break;
			case FLASH_SIZE_128K:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_128K);
				break;
			case FLASH_SIZE_512K:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_512K);
				break;
			case FLASH_SIZE_1M:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_1M);
				break;
			case FLASH_SIZE_2M:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_2M);
				break;
			case FLASH_SIZE_4M:
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_4M);
				break;
			default:
				break;
		}
	}
}


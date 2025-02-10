/********************************************************************************************************
 * @file    calibration.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
extern unsigned char	otp_program_flag;
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
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the frequency offset value address of flash or otp.
 * @return 	   1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(unsigned int addr)
{
#if(PACKAGE_TYPE == FLASH_PACKAGE)
	unsigned char frequency_offset_value = 0xff;
	flash_read_page(addr, 1, &frequency_offset_value);
#elif(PACKAGE_TYPE == OTP_PACKAGE)
	unsigned int frequency_offset_value = 0xff;
	otp_read(addr,1,&frequency_offset_value);
#endif
	if(0xff != (0xff&frequency_offset_value))
	{
		rf_update_internal_cap(frequency_offset_value);
		return 1;
	}
	return 0;
}
/**
 * @brief      This function is used to calib ADC 1.2V vref.
 * @return 	   1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char user_calib_adc_vref(unsigned int addr)
{

	unsigned char adc_vref_ft_calib_value[4] = {0};
	unsigned char adc_vref_cp_calib_value[4] = {0};
	//It will be automatically active during OTP programming. This operation is to activate OTP during SRAM or FLASH programming.
	if(otp_program_flag==0){
		otp_set_active_mode();
	}
	/********************************************************************************************
		The ADC calibration value priority of B80 is FT > CP.
		The GPIO calibration value and the VBAT calibration value do not necessarily exist at the same time.
	********************************************************************************************/
	otp_read(OTP_ADC_VREF_FT_CALIB_ADDR,1,(unsigned int *)adc_vref_ft_calib_value);
	otp_read(OTP_ADC_VREF_CP_CALIB_ADDR,1,(unsigned int *)adc_vref_cp_calib_value);
	//must
	if(otp_program_flag==0){
		otp_set_deep_standby_mode();
	}
#if(PACKAGE_TYPE == OTP_PACKAGE)
	//Determine if the stored vref gain value and stored vref offset matches the range, if it is, update the value.
	if(!adc_update_vref_calib_value_ft_cp(adc_vref_ft_calib_value[0],(signed char)adc_vref_ft_calib_value[1],adc_set_gpio_calib_vref))//gpio_ft
	{
		adc_update_vref_calib_value_ft_cp(adc_vref_cp_calib_value[0],(signed char)adc_vref_cp_calib_value[1],adc_set_gpio_calib_vref);//gpio_cp
	}
	if(!adc_update_vref_calib_value_ft_cp(adc_vref_ft_calib_value[2],(signed char)adc_vref_ft_calib_value[3],adc_set_vbat_calib_vref))//vbat_ft
	{
		adc_update_vref_calib_value_ft_cp(adc_vref_cp_calib_value[2],(signed char)adc_vref_cp_calib_value[3],adc_set_vbat_calib_vref);//vbat_cp
	}
#elif(PACKAGE_TYPE == FLASH_PACKAGE)
	//ADC calibration rule for flash has not been determined yet. Therefore, follow ADC calibration rule of OTP temporarily.
	if(!adc_update_vref_calib_value_ft_cp(adc_vref_ft_calib_value[0],(signed char)adc_vref_ft_calib_value[1],adc_set_gpio_calib_vref))//gpio_ft
	{
		adc_update_vref_calib_value_ft_cp(adc_vref_cp_calib_value[0],(signed char)adc_vref_cp_calib_value[1],adc_set_gpio_calib_vref);//gpio_cp
	}
	if(!adc_update_vref_calib_value_ft_cp(adc_vref_ft_calib_value[2],(signed char)adc_vref_ft_calib_value[3],adc_set_vbat_calib_vref))//vbat_ft
	{
		adc_update_vref_calib_value_ft_cp(adc_vref_cp_calib_value[2],(signed char)adc_vref_cp_calib_value[3],adc_set_vbat_calib_vref);//vbat_cp
	}
#endif
	return 1;
}
/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in otp,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * 				(reduce adc measurement error, reduce frequency offset, etc.)
 * @return		none.
 */
void user_read_otp_value_calib(void)
{
	user_calib_adc_vref(0);
	user_calib_freq_offset(OTP_CAP_VALUE_ADDR);
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
	unsigned int *flash_mid_check;
	/******check for flash mid********/
	flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);
	if (1 == flash_mid_sure)
	{
		switch (flash_mid[2])
		{
			case FLASH_SIZE_64K:
				user_calib_adc_vref(FLASH_ADC_VREF_CALIB_ADDR_64K);
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_64K);
				break;
			case FLASH_SIZE_128K:
				user_calib_adc_vref(FLASH_ADC_VREF_CALIB_ADDR_128K);
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_128K);
				break;
			case FLASH_SIZE_512K:
				user_calib_adc_vref(FLASH_ADC_VREF_CALIB_ADDR_512K);
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_512K);
				break;
			case FLASH_SIZE_1M:
				user_calib_adc_vref(FLASH_ADC_VREF_CALIB_ADDR_1M);
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_1M);
				break;
			case FLASH_SIZE_2M:
				user_calib_adc_vref(FLASH_ADC_VREF_CALIB_ADDR_2M);
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_2M);
				break;
			case FLASH_SIZE_4M:
				user_calib_adc_vref(FLASH_ADC_VREF_CALIB_ADDR_4M);
				user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_4M);
				break;
			default:
				user_calib_adc_vref(0);
				break;
		}
#if (MCU_CORE_B80)
/* If the flash is ZG25WD40B, the flash voltage(VDD_F) will be trim to 2.25V(2b'111 the max).
 * Reason for trim VDD_F voltage: BLE group requires 16byte write time to be less than 200us, the test found that flash voltage is maintained above 1.85V during flash write (VDD_F voltage with load 10mA max), and 16bytes write time is below 200us.
 * Reason for not trimming voltage for all ZB flash: For the subsequent addition of ZB flash, flash vendors may fix the problem of slow flash write speed, so currently only for this flash ZG25WD40B trim VDD_F voltage.(added by xiaobin.huang 20240802)
 */
    flash_mid_check = (unsigned int *)flash_mid;
	if(*flash_mid_check == 0x13325e)
	{
		pm_set_vdd_f(FLASH_VOLTAGE_2V25);
	}
#endif
	}
	else{
		user_calib_adc_vref(0);
	}
}

/********************************************************************************************************
 * @file	calibration.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
extern unsigned char	otp_program_flag;

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the frequency offset value address of flash or otp.
 * @return 	   1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(unsigned int addr)
{
#if(PACKAGE_TYPE == FLASH_PACKAGE)
	unsigned char freqency_offset_value = 0xff;
	flash_read_page(addr, 1, &freqency_offset_value);
#elif(PACKAGE_TYPE == OTP_PACKAGE)
	unsigned int freqency_offset_value = 0xff;
	otp_read(addr,1,&freqency_offset_value);
#endif

	if(0xff != (0xff&freqency_offset_value))
	{
		rf_update_internal_cap(freqency_offset_value);
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
#if(PACKAGE_TYPE == FLASH_PACKAGE)
	unsigned char adc_vref_calib_value[4] = {0};
#elif(PACKAGE_TYPE == OTP_PACKAGE)
	unsigned char adc_vref_ft_calib_value[4] = {0};
#endif
	unsigned char adc_vref_cp_calib_value[4] = {0};
	unsigned short gpio_calib_vref = 1175;
	signed char gpio_calib_vref_offset = 0;
	unsigned short vbat_calib_vref = 1175;
	signed char vbat_calib_vref_offset = 0;
	//It will be automatically active during OTP programming. This operation is to activate OTP during SRAM or FLASH programming.
	if(otp_program_flag==0){
		otp_set_active_mode();
	}
#if(PACKAGE_TYPE == OTP_PACKAGE)
	/********************************************************************************************
		The ADC calibration value priority of B80 is FT > CP.
		The GPIO calibration value and the VBAT calibration value do not necessarily exist at the same time.
	********************************************************************************************/
	otp_read(OTP_ADC_VREF_FT_CALIB_ADDR,1,(unsigned int *)adc_vref_ft_calib_value);
#endif
	otp_read(OTP_ADC_VREF_CP_CALIB_ADDR,1,(unsigned int *)adc_vref_cp_calib_value);
	//must
	if(otp_program_flag==0){
		otp_set_deep_standby_mode();
	}
#if(PACKAGE_TYPE == OTP_PACKAGE)
	//The stored vref value may be 0xff, but the offset value of 0xff must be uncalibrated.
	if((adc_vref_ft_calib_value[1]!=0xff)||(adc_vref_cp_calib_value[1]!=0xff))
	{
		if(adc_vref_ft_calib_value[1]!=0xff)
		{
			gpio_calib_vref = adc_vref_ft_calib_value[0] + 1000;
			gpio_calib_vref_offset = adc_vref_ft_calib_value[1] - 20;
		}
		else
		{
			gpio_calib_vref = adc_vref_cp_calib_value[0] + 1000;
			gpio_calib_vref_offset = adc_vref_cp_calib_value[1] - 20;
		}
		adc_set_gpio_calib_vref(gpio_calib_vref, gpio_calib_vref_offset);
	}
	if((adc_vref_ft_calib_value[3]!=0xff)||(adc_vref_cp_calib_value[3]!=0xff))
	{
		if(adc_vref_ft_calib_value[3]!=0xff)
		{
			vbat_calib_vref = adc_vref_ft_calib_value[2] + 1000;
			vbat_calib_vref_offset = adc_vref_ft_calib_value[3] - 20;
		}
		else
		{
			vbat_calib_vref = adc_vref_cp_calib_value[2] + 1000;
			vbat_calib_vref_offset = adc_vref_cp_calib_value[3] - 20;
		}
		adc_set_vbat_calib_vref(vbat_calib_vref, vbat_calib_vref_offset);
	}
#elif(PACKAGE_TYPE == FLASH_PACKAGE)
	if(addr == 0)
	{
		//The stored vref value may be 0xff, but the offset value of 0xff must be uncalibrated.
		if(adc_vref_cp_calib_value[1]!=0xff)
		{
			gpio_calib_vref = adc_vref_cp_calib_value[0] + 1000;
			gpio_calib_vref_offset = adc_vref_cp_calib_value[1] - 20;
			adc_set_gpio_calib_vref(gpio_calib_vref, gpio_calib_vref_offset);
		}
		if(adc_vref_cp_calib_value[3]!=0xff)
		{
			vbat_calib_vref = adc_vref_cp_calib_value[2] + 1000;
			vbat_calib_vref_offset = adc_vref_cp_calib_value[3] - 20;
			adc_set_vbat_calib_vref(vbat_calib_vref, vbat_calib_vref_offset);
		}
	}
	else
	{
		/********************************************************************************************
			The ADC calibration value priority of B80 is FLASH > CP.
			The GPIO calibration value and the VBAT calibration value do not necessarily exist at the same time.
		********************************************************************************************/
		flash_read_page(addr, 4, adc_vref_calib_value);
		if((adc_vref_calib_value[1]!=0xff)||(adc_vref_cp_calib_value[1]!=0xff))
		{
			if(adc_vref_calib_value[1]!=0xff)
			{
				gpio_calib_vref = adc_vref_calib_value[0] + 1000;
				gpio_calib_vref_offset = adc_vref_calib_value[1] - 20;
			}
			else
			{
				gpio_calib_vref = adc_vref_cp_calib_value[0] + 1000;
				gpio_calib_vref_offset = adc_vref_cp_calib_value[1] - 20;
			}
			adc_set_gpio_calib_vref(gpio_calib_vref, gpio_calib_vref_offset);
		}
		if((adc_vref_calib_value[3]!=0xff)||(adc_vref_cp_calib_value[3]!=0xff))
		{
			if(adc_vref_calib_value[3]!=0xff)
			{
				vbat_calib_vref = adc_vref_calib_value[2] + 1000;
				vbat_calib_vref_offset = adc_vref_calib_value[3] - 20;
			}
			else
			{
				vbat_calib_vref = adc_vref_cp_calib_value[2] + 1000;
				vbat_calib_vref_offset = adc_vref_cp_calib_value[3] - 20;
			}
			adc_set_vbat_calib_vref(vbat_calib_vref, vbat_calib_vref_offset);
		}
	}
#endif
	return 1;
}


/**
 * @brief      This function is used to calib vdd_1v2.
 * @return 	   1 - the calibration value update, 0 - the calibration value is not update.
 * @note       if rf rx_mode is turned on, otp has read error problem, if running otp program, do to avoid the solution,
 *             trim core voltage, and use 32K watchdog (enable in the front position).
 */
unsigned char user_calib_vdd_1v2(unsigned int addr){
/*
 * When the bit[7] of 0x3fc0 is 0, it indicates that the lower 3Bit is the calibration value and read 0x3fc0 bit<2:0> to afe3V_reg03<5:3>;
 * if 0x3fc0 bit[7] is 1, it indicates that there is no calibration value, and the value 0x02(1.1V) is assigned to afe3V_reg03<5:3>.
 */
	unsigned int vdd1v2_trim_value = 0xff;
	otp_read(addr,1,&vdd1v2_trim_value);

	if(0x00 == (vdd1v2_trim_value&BIT(7)))
	{
		pm_set_vdd_1v2(vdd1v2_trim_value&0x07);
		return 1;
	}
	else{
		pm_set_vdd_1v2(VDD_1V2_1V1);
	}

	return 0;
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
#if(!OTP_ALL_SRAM_CODE)
	user_calib_vdd_1v2(OTP_VDD_1V2_CALIB_ADDR);
#endif
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
	}
	else{
		user_calib_adc_vref(0);
	}
}

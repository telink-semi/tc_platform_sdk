/********************************************************************************************************
 * @file    calibration.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "driver.h"


#ifndef FLASH_ADC_VREF_CALIB_ADDR_64K
#define FLASH_ADC_VREF_CALIB_ADDR_64K	0xe0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_128K
#define FLASH_ADC_VREF_CALIB_ADDR_128K	0x1e0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_512K
#define FLASH_ADC_VREF_CALIB_ADDR_512K	0x770c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_1M
#define FLASH_ADC_VREF_CALIB_ADDR_1M	0xfe0c0
#endif

#ifndef FLASH_ADC_VREF_CALIB_ADDR_2M
#define FLASH_ADC_VREF_CALIB_ADDR_2M	0x1fe0c0
#endif

#ifndef FLASH_CAP_VALUE_ADDR_64K
#define FLASH_CAP_VALUE_ADDR_64K		0xe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_128K
#define FLASH_CAP_VALUE_ADDR_128K		0x1e000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_512K
#define FLASH_CAP_VALUE_ADDR_512K		0x77000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_1M
#define FLASH_CAP_VALUE_ADDR_1M			0xfe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_2M
#define FLASH_CAP_VALUE_ADDR_2M			0x1fe000
#endif

#ifndef FLASH_VDDF_CALIB_ADDR_64K
#define FLASH_VDDF_CALIB_ADDR_64K		0xe1c0
#endif

#ifndef FLASH_VDDF_CALIB_ADDR_128K
#define FLASH_VDDF_CALIB_ADDR_128K		0x1e1c0
#endif

#ifndef FLASH_VDDF_CALIB_ADDR_512K
#define FLASH_VDDF_CALIB_ADDR_512K		0x771c0
#endif

#ifndef FLASH_VDDF_CALIB_ADDR_1M
#define FLASH_VDDF_CALIB_ADDR_1M		0xfe1c0
#endif

#ifndef FLASH_VDDF_CALIB_ADDR_2M
#define FLASH_VDDF_CALIB_ADDR_2M		0x1fe1c0
#endif


/**
 * @brief	flash voltage definition
 */
typedef enum {
    USER_CALIB_FROM_FLASH	= 0x01,
    USER_CALIB_FROM_OTP		= 0x02,
}user_calib_from_e;


/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in flash,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * 				(reduce adc measurement error, reduce frequency offset, etc.)
 * @return		none.
 */
void user_read_flash_value_calib(void);

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the frequency offset value address of flash or otp.
 * @return 	   1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(user_calib_from_e velfrom, unsigned int addr);

/**
 * @brief      This function is used to calib ADC 1.2V vref.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the calibration value address of flash or otp.
 * @return 	   1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char user_calib_adc_vref(user_calib_from_e velfrom, unsigned int addr);

#endif


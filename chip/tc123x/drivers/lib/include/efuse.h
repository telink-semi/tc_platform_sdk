/********************************************************************************************************
 * @file    efuse.h
 *
 * @brief   This is the header file for TC123X
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
#ifndef EFUSE_H_
#define EFUSE_H_

typedef struct
{
    unsigned char gpio_gain;
    unsigned char gpio_offset;
    unsigned char vbat_gain;
    unsigned char vbat_offset;
} adc_calib_t;

/**
 * @brief        This function servers to get data from EFUSE. EFUSE default value is 0. The EFUSE address range is 0~32 bytes.
 * @param[in]    addr - the start address of the EFUSE location.
 * @param[in]    buf  - the start address of the buffer.
 * @param[in]    len  - the length(in byte) of content needs to read out from EFUSE.
 * @return       1: operation completed.
 *               0: operation fail.
 */
unsigned char efuse_read(unsigned char addr, unsigned char *buff, unsigned char len);

/**
 * @brief        This function servers to set data to EFUSE. The efuse address range is 0~32 bytes.
 * @param[in]    addr - the start address of the efuse location.
 * @param[in]    buf  - the start address of the buffer.
 * @param[in]    len  - the length(in byte) of content needs to be written to efuse.
 * @return       1: operation completed. It does not mean that the written data is correct and requires read back verification.
 *               0: operation timeout.
 */
unsigned char efuse_write(unsigned char addr, unsigned char *buff, unsigned char len);


/**
 * @brief       This function is used to Tighten the judgment of illegal values for gpio calibration and vbat calibration in the efuse.
 *              The ADC vref gain calibtation should range from 1100mV to 1300mV, the ADC vref offset calibration should range from -100mV to 100mV.
 * @param[in]   gain - the value of gpio_calib_vref_gain or vbat_calib_vref_gain
 *              offset - the value of gpio_calib_vref_offset or vbat_calib_vref_offset
 *              calib_func - Function pointer to gpio_calibration or vbat_calibration.
 * @return      1:the calibration function is invalid; 0:the calibration function is valid.
 */
unsigned char efuse_set_adc_calib_value(unsigned char gain, signed char offset, void (*calib_func)(unsigned short, signed char));

/**
 * @brief      This function servers to get calibration value from EFUSE.
 * @param[in]  none
 * @return     0 - the calibration value update, 1 - the calibration value is not update.
 */
unsigned char efuse_calib_adc_vref(void);

/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in efuse,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * 				(reduce frequency offset.)
 * @return      1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char efuse_calib_rf_freq(void);

#endif

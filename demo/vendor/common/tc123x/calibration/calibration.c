/********************************************************************************************************
 * @file    calibration.c
 *
 * @brief   This is the source file for TC1211
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
#include "lib/include/efuse.h"
/**
 * @brief      This function serves to read IEEE address from flash
 * @param[out] buf  - Pointer to IEEE address buffer(IEEE address is 6bytes)
 * @return     none
 */
void flash_get_ieee_addr(unsigned char *buf)
{
    if (buf == NULL) return;
    unsigned char flash_mid[4];
    unsigned char flash_uid[16];
    unsigned char flash_mid_sure = 0;
    unsigned int ieee_value_addr = 0;

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);
    if (1 == flash_mid_sure) {
        switch (flash_mid[2]) {
            case FLASH_SIZE_64K:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_64K;
                break;
            case FLASH_SIZE_128K:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_128K;
                break;
            case FLASH_SIZE_512K:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_512K;
                break;
            case FLASH_SIZE_1M:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_1M;
                break;
            case FLASH_SIZE_2M:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_2M;
                break;
            case FLASH_SIZE_4M:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_4M;
                break;
            case FLASH_SIZE_8M:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_8M;
                break;
            case FLASH_SIZE_16M:
                ieee_value_addr = FLASH_IEEE_VALUE_ADDR_16M;
                break;
        }
    }
    if (ieee_value_addr) {
        flash_read_page(ieee_value_addr, 6, buf);
    }
}

/**
 * @brief      This function servers to get calibration value from flash.
 * @param[in]  none
 * @return     0 - the calibration value update, 1 - the calibration value is not update.
 */
unsigned char flash_calib_adc_vref(unsigned int addr)
{
    //the data order is gpio_gain, gpio_offset, rsvd, vbat_gain, vbat_offset
    unsigned char  adc_calib_value[5] = {0};
    flash_read_page(addr, 5, adc_calib_value);
    /**
     * The legal range of gain for both gpio and vbat in efuse is [0,250],
     * and the legal range of offset for both gpio and vbat is [-100,100].
     */
    if (adc_calib_value[0] <= 250 && (signed char)adc_calib_value[1] <= 100 && (signed char)adc_calib_value[1] >= -100 && adc_calib_value[3] <= 250 && (signed char)adc_calib_value[4] <= 100 && (signed char)adc_calib_value[4] >= -100) {
        adc_set_gpio_calib_vref(1100 + adc_calib_value[0], (signed char)adc_calib_value[1]);
        adc_set_vbat_calib_vref(1100 + adc_calib_value[3], (signed char)adc_calib_value[4]);
        return 0;
    }
    return 1;
}

/**
 * @brief      This function servers to get calibration value from flash or efuse.
 * @param[in]  addr - the addr of calib value in flash
 * @return     0 - the calibration value update, 1 - the calibration value is not update.
 */
void user_calib_adc_vref(unsigned int addr)
{
    unsigned char ret = 1; // 1 fail 0 success
    if (addr != 0) {
        ret = flash_calib_adc_vref(addr);
    }
    //The register used for distinguishing the A0 judgment is 0x7D, and the A0 version value is 0.
    if (ret == 1 && 0 != read_reg8(0x7d)) { // A0 version not use the calib value in efuse
        efuse_calib_adc_vref();
    }
}
/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  addr - the frequency offset value address of flash.
 * @return        1 - the frequency offset update, 0 - the frequency offset is not update.
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
 * @brief        This function is used to calibrate the user's parameters.
 *                 This function is to read the calibration value stored in flash or in efuse,
 *                 and use the calibration value to configure the chip to improve chip performance.
 *                 (reduce adc measurement error, reduce frequency offset, etc.)
 * @return        none.
 */
void user_calibration_func(void)
{
    unsigned char flash_mid[4];
    unsigned char flash_uid[16];
    unsigned char flash_mid_sure = 0;
    unsigned int adc_value_addr = 0;

    /******check for flash mid********/
    flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);
    if (1 == flash_mid_sure)
    {
        switch (flash_mid[2])
        {
            case FLASH_SIZE_64K:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_64K);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_64K;
                break;
            case FLASH_SIZE_128K:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_128K);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_128K;
                break;
            case FLASH_SIZE_512K:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_512K);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_512K;
                break;
            case FLASH_SIZE_1M:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_1M);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_1M;
                break;
            case FLASH_SIZE_2M:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_2M);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_2M;
                break;
            case FLASH_SIZE_4M:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_4M);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_4M;
                break;
            case FLASH_SIZE_8M:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_8M);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_8M;
                break;
            case FLASH_SIZE_16M:
                user_calib_freq_offset(FLASH_CAP_VALUE_ADDR_16M);
                adc_value_addr = FLASH_ADC_CALI_VALUE_ADDR_16M;
                break;
            default:
                adc_value_addr = 0;
                break;
        }
    }
    user_calib_adc_vref(adc_value_addr);
}

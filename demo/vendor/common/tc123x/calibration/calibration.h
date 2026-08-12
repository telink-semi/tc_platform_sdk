/********************************************************************************************************
 * @file    calibration.h
 *
 * @brief   This is the header file for tc123x
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
#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#ifndef FLASH_CAP_VALUE_ADDR_64K
#define FLASH_CAP_VALUE_ADDR_64K        0xe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_128K
#define FLASH_CAP_VALUE_ADDR_128K        0x1e000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_512K
#define FLASH_CAP_VALUE_ADDR_512K        0x7e000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_1M
#define FLASH_CAP_VALUE_ADDR_1M            0xfe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_2M
#define FLASH_CAP_VALUE_ADDR_2M            0x1fe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_4M
#define FLASH_CAP_VALUE_ADDR_4M            0x3fe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_8M
#define FLASH_CAP_VALUE_ADDR_8M            0x7fe000
#endif

#ifndef FLASH_CAP_VALUE_ADDR_16M
#define FLASH_CAP_VALUE_ADDR_16M            0xffe000
#endif


#ifndef FLASH_ADC_CALI_VALUE_ADDR_OFFSET
    #define FLASH_ADC_CALI_VALUE_ADDR_OFFSET    (0xc4)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_64K
    #define FLASH_ADC_CALI_VALUE_ADDR_64K       (0xe000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_128K
    #define FLASH_ADC_CALI_VALUE_ADDR_128K      (0x1e000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_512K
    #define FLASH_ADC_CALI_VALUE_ADDR_512K      (0x7e000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_1M
    #define FLASH_ADC_CALI_VALUE_ADDR_1M        (0xfe000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_2M
    #define FLASH_ADC_CALI_VALUE_ADDR_2M        (0x1fe000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_4M
    #define FLASH_ADC_CALI_VALUE_ADDR_4M        (0x3fe000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_8M
    #define FLASH_ADC_CALI_VALUE_ADDR_8M        (0x7fe000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif

#ifndef FLASH_ADC_CALI_VALUE_ADDR_16M
    #define FLASH_ADC_CALI_VALUE_ADDR_16M       (0xffe000 + FLASH_ADC_CALI_VALUE_ADDR_OFFSET)
#endif


#ifndef FLASH_IEEE_VALUE_ADDR_64K
#define FLASH_IEEE_VALUE_ADDR_64K        0xF000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_128K
#define FLASH_IEEE_VALUE_ADDR_128K        0x1F000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_512K
#define FLASH_IEEE_VALUE_ADDR_512K        0x7F000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_1M
#define FLASH_IEEE_VALUE_ADDR_1M            0xFF000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_2M
#define FLASH_IEEE_VALUE_ADDR_2M            0x1FF000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_4M
#define FLASH_IEEE_VALUE_ADDR_4M            0x3FF000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_8M
#define FLASH_IEEE_VALUE_ADDR_8M            0x7FF000
#endif

#ifndef FLASH_IEEE_VALUE_ADDR_16M
#define FLASH_IEEE_VALUE_ADDR_16M            0xFFF000
#endif


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
 * @brief      This function serves to read IEEE address from flash
 * @param[out] buf  - Pointer to IEEE address buffer(IEEE address is 6bytes)
 * @return     none
 */
void flash_get_ieee_addr(unsigned char *buf);

/**
 * @brief       This function is used to calibrate the user's parameters.
 *              This function is to read the calibration value stored in efuse and flash,
 *              and use the calibration value to configure the chip to improve chip performance.
 *              (reduce adc measurement error, reduce frequency offset, etc.)
 * @param[in]   gpio_type - enum variable of the GPIO voltage.
 * @return      none.
 */
void user_calibration_func(void);
#endif


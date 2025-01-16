/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for Telink MCU
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
#pragma once
#include "driver.h"
#include "common.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Define system clock
 */
#define CLOCK_SYS_CLOCK_HZ 24000000

/**
 * @note The sys_clock.h file depends on the definition of CLOCK_SYS_CLOCK_HZ so CLOCK_SYS_CLOCK_HZ must be placed before #include "sys_clock.h" without reversing the order.
 */
#include "sys_clock.h"

/**********************************************************************************************************************
 *                                         Users do not need to modify                                                *
 *********************************************************************************************************************/
#define SD_ADC_GPIO_MODE        1
#define SD_ADC_VBAT_MODE        2
#define SD_ADC_TEMP_MODE        3

#define POLLING_MODE        1
#define INTERRUPT_MODE      2

/**********************************************************************************************************************
 *                                         Users can modify macros                                                    *
 *********************************************************************************************************************/

/**
 * @attention SD_ADC_SAMPLE_CLOCK only supports 1M/2M.
 */
#define SD_ADC_SAMPLE_CLOCK_DIV     SD_ADC_SAPMPLE_CLK_2M_DIV(CLOCK_SYS_CLOCK_HZ)
#define SD_ADC_DOWN_SAMPLE_RATE     SD_ADC_DOWNSAMPLE_RATE_128

/**
 * -# SD_ADC_FIFO_DEPTH must be 2^n (n ranges from 3 to 10): 8/16/32/64/128/256/512/1024,
 * -# SD_ADC_SAMPLE_CNT must be multiple of 4 and < SD_ADC_FIFO_DEPTH and max is 1020 .
 * -# Due to the first 4 sample data anomaly after each call to dc_sample_start()
 *    and the call to dc_sort_and_get_average_code() for filtering,
 *    it is recommended that SD_ADC_SAMPLE_CNT be set to multiple of 4 and >= 16,
 *    SD_ADC_FIFO_DEPTH to 32/64/128/256/512/1024.
 */
#define SD_ADC_FIFO_DEPTH           32
#define SD_ADC_SAMPLE_CNT           16

/**
 * @attention SD_ADC can be used with audio dmic at the same time, but not with audio amic.
 *
 */
#define SD_ADC_MODE        SD_ADC_GPIO_MODE

#define SAMPLE_MODE        POLLING_MODE


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

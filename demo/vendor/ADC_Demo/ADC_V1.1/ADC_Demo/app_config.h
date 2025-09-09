/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
 * @date    2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "driver.h"

    /**********************************************************************************************************************
 *                                         Users do not need to modify                                                *
 *********************************************************************************************************************/
#define ADC_DMA_MODE         1
#define ADC_NDMA_MODE        2

#define ADC_GPIO_SAMPLE      1
#define ADC_VBAT_SAMPLE      2

//In NDMA mode, only M channel can be used.
#define NDMA_M_1_CHN_EN 1
#define DMA_M_1_CHN_EN  1


/**********************************************************************************************************************
 *                                         Users can modify macros                                                    *
 *********************************************************************************************************************/
/**
 *@attention  -# In NDMA/DMA mode, ADC_SAMPLE_GROUP_CNT must be multiple of 8.
 */
#define ADC_SAMPLE_GROUP_CNT        16 //Number of adc sample codes per channel.

#define ADC_MODE                    ADC_NDMA_MODE

#if (ADC_MODE == ADC_DMA_MODE)
    #define ADC_SAMPLE_CHN_CNT      DMA_M_1_CHN_EN //Number of channels enabled
    #define ADC_M_CHN_SAMPLE_MODE   ADC_GPIO_SAMPLE
#else
    #define ADC_SAMPLE_CHN_CNT      NDMA_M_1_CHN_EN
    #define ADC_SAMPLE_MODE         ADC_GPIO_SAMPLE
#endif

#define GPIO_M_CHN_SAMPLE_PIN       ADC_GPIO_PB0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

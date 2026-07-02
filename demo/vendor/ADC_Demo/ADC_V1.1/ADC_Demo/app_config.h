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
#if !defined(MCU_CORE_TC123X)
#define ADC_DMA_MODE         1
#endif
#define ADC_NDMA_MODE        2

#define ADC_GPIO_SAMPLE      1
#define ADC_VBAT_SAMPLE      2
#define ADC_TEMP_SENSOR_SAMPLE 3 //internal test, user don't need care.
//In NDMA mode, only M channel can be used.
#define NDMA_M_1_CHN_EN 1
#define DMA_M_1_CHN_EN  1


/**********************************************************************************************************************
 *                                         Users can modify macros                                                    *
 *********************************************************************************************************************/
/**
 *@attention  -# In NDMA/DMA mode, ADC_SAMPLE_GROUP_CNT must be multiple of 8.
 *@attention  -# For tc123x, this macro configuration is defined in adc.c and is called ADC_SAMPLE_NUM.
 */
#if !defined(MCU_CORE_TC123X)
#define ADC_SAMPLE_GROUP_CNT        8 //Number of adc sample codes per channel.
#endif
/**
 *@attention  -# For TC123X, adc just support NDMA_MODE.
 */
#define ADC_MODE                    ADC_NDMA_MODE


#if (ADC_MODE == ADC_DMA_MODE)
    #define ADC_SAMPLE_CHN_CNT      DMA_M_1_CHN_EN //Number of channels enabled
    #define ADC_M_CHN_SAMPLE_MODE   ADC_GPIO_SAMPLE
#else
    #define ADC_SAMPLE_CHN_CNT      NDMA_M_1_CHN_EN
    #define ADC_SAMPLE_MODE         ADC_GPIO_SAMPLE
#endif

/**
 * @attention:
 * For TC321X:
 * -# The maximum sampling voltage range of PB5-6 and PD0-1 is 0 ~ (vbat*60%).
 *    e.g., if vbat is supplied with 3.3V, then the sampling range is 0 ~ 1.98V).
 * -# If external voltage division is required, we recommend using PB5-6 and PD0-1,
 *    which can prevent the influence of internal resistance and provide more accurate sampling voltage.  
 * -# The maximum sampling voltage range of PB0-4 is 0 ~ 3.3V.
 */
#define GPIO_M_CHN_SAMPLE_PIN       ADC_GPIO_PB0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

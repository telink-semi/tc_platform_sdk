/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for B85m
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

#if (MCU_CORE_B80)
#define I2C_GPIO_SDA            GPIO_PB5
#define I2C_GPIO_SCL            GPIO_PB4
#elif (MCU_CORE_B80B)
#define I2C_GPIO_SDA            GPIO_PB5
#define I2C_GPIO_SCL            GPIO_PB4
#elif (MCU_CORE_B85)
#define I2C_GPIO_SDA_SCL        I2C_GPIO_GROUP_A3A4
#elif (MCU_CORE_B87)
#define I2C_GPIO_SDA            I2C_GPIO_SDA_A3
#define I2C_GPIO_SCL            I2C_GPIO_SCL_A4
#elif (MCU_CORE_B89)
#define I2C_GPIO_SDA            I2C_GPIO_SDA_A0
#define I2C_GPIO_SCL            I2C_GPIO_SCL_A1
#endif



#define I2C_DMA_MODE                1
#define I2C_MAPPING_MODE            2

#define I2C_MODE                	1

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

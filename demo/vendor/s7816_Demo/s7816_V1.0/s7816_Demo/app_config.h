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

#if (MCU_CORE_B80)
#define S7816_TRX_PIN			GPIO_PB2
#define S7816_VCC_PIN			GPIO_PB3
#define S7816_RST_PIN			GPIO_PB4
#define S7816_CLK_PIN			GPIO_PB5
#elif (MCU_CORE_B80B)
#define S7816_TRX_PIN           GPIO_PB2
#define S7816_VCC_PIN           GPIO_PB3
#define S7816_RST_PIN           GPIO_PB4
#define S7816_CLK_PIN           GPIO_PB5
#elif (MCU_CORE_B85)
#define S7816_TRX_PIN			S7816_TRX_D0
#define S7816_VCC_PIN			GPIO_PB0
#define S7816_RST_PIN			GPIO_PB1
#define S7816_CLK_PIN			S7817_CLK_PA1 //the clk-source of s7816 is 24M.
#elif (MCU_CORE_B87)
#define S7816_TRX_PIN			S7816_TRX_D0
#define S7816_VCC_PIN			GPIO_PB0
#define S7816_RST_PIN			GPIO_PB1
#define S7816_CLK_PIN			S7817_CLK_PA1
#elif (MCU_CORE_B89)
#define S7816_TRX_PIN			S7816_TRX_PD3
#define S7816_VCC_PIN			GPIO_PB5
#define S7816_RST_PIN			GPIO_PB6
#define S7816_CLK_PIN			S7817_CLK_PB2
#endif



#define F   372      //clock frequency regulator ,372 is the initialized  value.
#define D    1       //bitrate regulator,1 is the initialized value.


#define S7816_RX_BUFF_LEN   48

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

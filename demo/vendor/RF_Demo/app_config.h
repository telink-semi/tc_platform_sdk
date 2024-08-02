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

#if (MCU_CORE_B85)
#define RF_POWER			RF_POWER_P10p46dBm
#elif(MCU_CORE_B87)
#define RF_POWER			RF_POWER_P11p26dBm
#elif(MCU_CORE_B89)
#define RF_POWER			RF_POWER_INDEX_P4p98dBm
#elif(MCU_CORE_B80)
#define RF_POWER			RF_POWER_P11p46dBm
#elif(MCU_CORE_B80B)
#define RF_POWER            RF_POWER_P11p46dBm
#endif

#define RF_BLE_2M				1
#define RF_BLE_1M				2
#define RF_BLE_1M_NO_PN			3
#define RF_ZIGBEE_250K			4
#define RF_LR_S2_500K			5
#define RF_LR_S8_125K			6
#define RF_PRIVATE_250K			7
#define RF_PRIVATE_500K			8
#define RF_PRIVATE_1M			9
#define RF_PRIVATE_2M			10
#define RF_ANT					11

#if (MCU_CORE_B87)
#define RF_HYBEE_1M				12
#define RF_HYBEE_2M				13
#define RF_HYBEE_500K			14
#define RF_BLE_SDK_TEST			20
#endif

#define RF_BLE_1M_STX2RX		15



#define RF_MODE					RF_LR_S8_125K

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

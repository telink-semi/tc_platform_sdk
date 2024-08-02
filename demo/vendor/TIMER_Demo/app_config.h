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
#define TIMER_GPIO      		GPIO_PB3
#elif (MCU_CORE_B80B)
#define TIMER_GPIO              GPIO_PB3
#elif (MCU_CORE_B85)
#define TIMER_GPIO      		GPIO_PD0
#elif (MCU_CORE_B87)
#define TIMER_GPIO      		GPIO_PD0
#elif (MCU_CORE_B89)
#define TIMER_GPIO      		GPIO_PD4
#endif

#define TIMER_SYS_CLOCK_MODE 	1
#define TIMER_GPIO_TRIGGER_MODE 2
#define TIMER_GPIO_WIDTH_MODE 	3
#define TIMER_TICK_MODE 		4
#define TIMER_WATCHDOG_MODE 	5  /* ONLY TIMER2 SUPPORT THIS MODE*/
#if (MCU_CORE_B80 || MCU_CORE_B80B)
#define STIMER_MODE				6
#endif
#if(MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89)
#define TIMER_32K_WATCHDOG_MODE 7
#endif

#define TIMER_MODE				5

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

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

/* IDLE MODE */
#if(MCU_CORE_B80 || MCU_CORE_B80B)
#define IDLE_TIMER_WAKEUP				1
#define IDLE_STIMER_WAKEUP				2
#define IDLE_RF_WAKEUP					3
#define IDLE_PAD_WAKEUP					4
#elif(MCU_CORE_B85||MCU_CORE_B87||MCU_CORE_B89)
#define IDLE_TIMER0_WAKEUP				1
#define IDLE_TIMER1_WAKEUP				2
#define IDLE_TIMER2_WAKEUP				3
#endif


/* SUSPEND MODE */
#define SUSPEND_PAD_WAKEUP   			11
#define SUSPEND_32K_RC_WAKEUP   		12
#define SUSPEND_32K_XTAL_WAKEUP			13
#if(MCU_CORE_B80 || MCU_CORE_B80B)
#define SUSPEND_LONG_32K_RC_WAKEUP   	14
#define SUSPEND_LONG_32K_XTAL_WAKEUP   	15
#elif(MCU_CORE_B87)
#define SUSPEND_MDEC_WAKEUP				14
#define SUSPEND_CORE_WAKEUP				15
#endif
#if(MCU_CORE_B85|MCU_CORE_B87)
#define SUSPEND_COMPARATOR_WAKEUP		16
#endif

/* DEEPSLEEP MODE */
#define DEEP_PAD_WAKEUP		 			21
#define DEEP_32K_RC_WAKEUP      		22
#define DEEP_32K_XTAL_WAKEUP      		23
#if(MCU_CORE_B80 || MCU_CORE_B80B)
#define DEEP_LONG_32K_RC_WAKEUP   		24
#define DEEP_LONG_32K_XTAL_WAKEUP   	25
#elif(MCU_CORE_B89)
#define DEEP_DEBOUNCE_WAKEUP      		24
#elif(MCU_CORE_B87)
#define DEEP_MDEC_WAKEUP      			24
#endif
#if(MCU_CORE_B85|MCU_CORE_B87)
#define DEEP_COMPARATOR_WAKEUP			26
#endif

/* DEEPSLEEP WITH RETENTION MODE */
#define DEEP_RET_PAD_WAKEUP     		31
#define DEEP_RET_32K_RC_WAKEUP     		32
#define DEEP_RET_32K_XTAL_WAKEUP     	33
#if(MCU_CORE_B80 || MCU_CORE_B80B)
#define DEEP_RET_LONG_32K_RC_WAKEUP   	34
#define DEEP_RET_LONG_32K_XTAL_WAKEUP   35
#elif(MCU_CORE_B89)
#define DEEP_RET_DEBOUNCE_WAKEUP      	34
#elif(MCU_CORE_B87)
#define DEEP_RET_MDEC_WAKEUP      		34
#endif
#if(MCU_CORE_B85|MCU_CORE_B87)
#define DEEP_RET_COMPARATOR_WAKEUP		36
#endif

#if(MCU_CORE_B89)
/* SHUTDOWN_MODE */
#define	SHUTDOWN_PAD_WAKEUP				41
#endif

#define PM_MODE			     			SUSPEND_PAD_WAKEUP

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

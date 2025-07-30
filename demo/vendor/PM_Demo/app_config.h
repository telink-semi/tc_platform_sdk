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

/* IDLE MODE */
#if(MCU_CORE_B80 || MCU_CORE_B80B||MCU_CORE_TC321X)
#define IDLE_TIMER_WAKEUP				1
#define IDLE_STIMER_WAKEUP				2
#define IDLE_RF_WAKEUP					3
#define IDLE_PAD_WAKEUP					4
#elif(MCU_CORE_B85||MCU_CORE_B87||MCU_CORE_B89)
#define IDLE_TIMER0_WAKEUP				1
#define IDLE_TIMER1_WAKEUP				2
#define IDLE_TIMER2_WAKEUP				3
#endif

#if(MCU_CORE_B89||MCU_CORE_TC321X)
/* SHUTDOWN_MODE */
#define	SHUTDOWN_PAD_WAKEUP				41
#endif

/* SUSPEND MODE */
#define SUSPEND_PAD   					11
#define SUSPEND_32K_RC   				12
#define SUSPEND_32K_XTAL				13
#define SUSPEND_LONG_32K_RC   			14
#define SUSPEND_LONG_32K_XTAL   		15
#define SUSPEND_MDEC					16
#define SUSPEND_CORE					17
#define SUSPEND_COMPARATOR				18

/* DEEPSLEEP MODE */
#define DEEP_PAD	 					21
#define DEEP_32K_RC      				22
#define DEEP_32K_XTAL     				23
#define DEEP_LONG_32K_RC  				24
#define DEEP_LONG_32K_XTAL  			25
#define DEEP_DEBOUNCE     				26
#define DEEP_MDEC      					27
#define DEEP_COMPARATOR					28

/* DEEPSLEEP WITH RETENTION MODE */
#define DEEP_RET_PAD     				31
#define DEEP_RET_32K_RC    				32
#define DEEP_RET_32K_XTAL     			33
#define DEEP_RET_LONG_32K_RC   			34
#define DEEP_RET_LONG_32K_XTAL   		35
#define DEEP_RET_DEBOUNCE      			36
#define DEEP_RET_MDEC      				37
#define DEEP_RET_COMPARATOR				38


#if(MCU_CORE_B85)
#define SUSPEND_PAD_WAKEUP   			SUSPEND_PAD
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC
#define SUSPEND_32K_XTAL_WAKEUP			SUSPEND_32K_XTAL
#define SUSPEND_COMPARATOR_WAKEUP		SUSPEND_COMPARATOR
#define DEEP_PAD_WAKEUP		 			DEEP_PAD
#define DEEP_32K_RC_WAKEUP      		DEEP_32K_RC
#define DEEP_32K_XTAL_WAKEUP      		DEEP_32K_XTAL
#define DEEP_COMPARATOR_WAKEUP			DEEP_COMPARATOR
#define DEEP_RET_PAD_WAKEUP		 		DEEP_RET_PAD
#define DEEP_RET_32K_RC_WAKEUP      	DEEP_RET_32K_RC
#define DEEP_RET_32K_XTAL_WAKEUP      	DEEP_RET_32K_XTAL
#define DEEP_RET_COMPARATOR_WAKEUP		DEEP_RET_COMPARATOR

#elif(MCU_CORE_B87)
#define SUSPEND_PAD_WAKEUP   			SUSPEND_PAD
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC
#define SUSPEND_32K_XTAL_WAKEUP			SUSPEND_32K_XTAL
#define SUSPEND_MDEC_WAKEUP				SUSPEND_MDEC
#define SUSPEND_CORE_WAKEUP				SUSPEND_CORE
#define SUSPEND_COMPARATOR_WAKEUP		SUSPEND_COMPARATOR
#define DEEP_PAD_WAKEUP		 			DEEP_PAD
#define DEEP_32K_RC_WAKEUP      		DEEP_32K_RC
#define DEEP_32K_XTAL_WAKEUP      		DEEP_32K_XTAL
#define DEEP_MDEC_WAKEUP      			DEEP_MDEC
#define DEEP_COMPARATOR_WAKEUP			DEEP_COMPARATOR
#define DEEP_RET_PAD_WAKEUP		 		DEEP_RET_PAD
#define DEEP_RET_32K_RC_WAKEUP      	DEEP_RET_32K_RC
#define DEEP_RET_32K_XTAL_WAKEUP      	DEEP_RET_32K_XTAL
#define DEEP_RET_MDEC_WAKEUP      		DEEP_RET_MDEC
#define DEEP_RET_COMPARATOR_WAKEUP		DEEP_RET_COMPARATOR

#elif(MCU_CORE_B80 || MCU_CORE_B80B)
#define SUSPEND_PAD_WAKEUP   			SUSPEND_PAD
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC
#define SUSPEND_32K_XTAL_WAKEUP			SUSPEND_32K_XTAL
#define SUSPEND_LONG_32K_RC_WAKEUP   	SUSPEND_LONG_32K_RC
#define SUSPEND_LONG_32K_XTAL_WAKEUP   	SUSPEND_LONG_32K_XTAL
#define DEEP_PAD_WAKEUP		 			DEEP_PAD
#define DEEP_32K_RC_WAKEUP      		DEEP_32K_RC
#define DEEP_32K_XTAL_WAKEUP      		DEEP_32K_XTAL
#define DEEP_LONG_32K_RC_WAKEUP   		DEEP_LONG_32K_RC
#define DEEP_LONG_32K_XTAL_WAKEUP   	DEEP_LONG_32K_XTAL
#define DEEP_RET_PAD_WAKEUP		 		DEEP_RET_PAD
#define DEEP_RET_32K_RC_WAKEUP      	DEEP_RET_32K_RC
#define DEEP_RET_32K_XTAL_WAKEUP      	DEEP_RET_32K_XTAL
#define DEEP_RET_LONG_32K_RC_WAKEUP   	DEEP_RET_LONG_32K_RC
#define DEEP_RET_LONG_32K_XTAL_WAKEUP   DEEP_RET_LONG_32K_XTAL

#elif(MCU_CORE_B89)
#define SUSPEND_PAD_WAKEUP   			SUSPEND_PAD
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC
#define SUSPEND_32K_XTAL_WAKEUP			SUSPEND_32K_XTAL
#define DEEP_PAD_WAKEUP		 			DEEP_PAD
#define DEEP_32K_RC_WAKEUP      		DEEP_32K_RC
#define DEEP_32K_XTAL_WAKEUP      		DEEP_32K_XTAL
#define DEEP_DEBOUNCE_WAKEUP      		DEEP_DEBOUNCE
#define DEEP_RET_PAD_WAKEUP		 		DEEP_RET_PAD
#define DEEP_RET_32K_RC_WAKEUP      	DEEP_RET_32K_RC
#define DEEP_RET_32K_XTAL_WAKEUP      	DEEP_RET_32K_XTAL
#define DEEP_RET_DEBOUNCE_WAKEUP      	DEEP_RET_DEBOUNCE

#elif(MCU_CORE_TC321X)
#define SUSPEND_PAD_WAKEUP   			SUSPEND_PAD
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC
#define DEEP_PAD_WAKEUP		 			DEEP_PAD
#define DEEP_32K_RC_WAKEUP      		DEEP_32K_RC
#define DEEP_RET_PAD_WAKEUP		 		DEEP_RET_PAD
#define DEEP_RET_32K_RC_WAKEUP      	DEEP_RET_32K_RC

#elif(MCU_CORE_TC1211)
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC

#elif(MCU_CORE_TC122X)
#define SUSPEND_PAD_WAKEUP   			SUSPEND_PAD
#define SUSPEND_32K_RC_WAKEUP   		SUSPEND_32K_RC
#define DEEP_PAD_WAKEUP		 			DEEP_PAD
#define DEEP_32K_RC_WAKEUP      		DEEP_32K_RC
#define DEEP_RET_PAD_WAKEUP		 		DEEP_RET_PAD
#define DEEP_RET_32K_RC_WAKEUP      	DEEP_RET_32K_RC

#endif


#define PM_MODE			     			SUSPEND_32K_RC_WAKEUP

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

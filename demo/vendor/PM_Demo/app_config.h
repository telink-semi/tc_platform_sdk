/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#if (MCU_CORE_B80)
#define LED1     		        GPIO_PB3
#define LED2     		        GPIO_PB4
#define LED3     		        GPIO_PB5
#define LED4     		        GPIO_PB6
#elif (MCU_CORE_B89)
#define LED1     		        GPIO_PD0
#define LED2     		        GPIO_PD1
#define LED3     		        GPIO_PD2
#define LED4     		        GPIO_PD3
#elif (MCU_CORE_B87)
#define LED1     		        GPIO_PD2
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5

#define SW1      		        GPIO_PD1
#define SW2      		        GPIO_PD2

#elif (MCU_CORE_B85)
#define LED1     		        GPIO_PD0
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5

#define SW1      		        GPIO_PD1
#define SW2      		        GPIO_PD2

#endif


/* IDLE MODE */
#define IDLE_TIMER_WAKEUP				1
#define IDLE_STIMER_WAKEUP				2
#define IDLE_RF_WAKEUP					3
#define IDLE_PAD_WAKEUP					4

/* SUSPEND MODE */
#define SUSPEND_PAD_WAKEUP   			11
#define SUSPEND_32K_RC_WAKEUP   		12
#define SUSPEND_32K_XTAL_WAKEUP			13
#if(MCU_CORE_B87)
#define SUSPEND_MDEC_WAKEUP				14
#define SUSPEND_CORE_WAKEUP				15
#endif

/* DEEPSLEEP MODE */
#define DEEP_PAD_WAKEUP		 			21
#define DEEP_32K_RC_WAKEUP      		22
#define DEEP_32K_XTAL_WAKEUP      		23
#if(MCU_CORE_B89)
#define DEEP_DEBOUNCE_WAKEUP      		24
#elif(MCU_CORE_B87)
#define DEEP_MDEC_WAKEUP      			24
#endif

/* DEEPSLEEP WITH RETENTION MODE */
#define DEEP_RET_PAD_WAKEUP     		31
#define DEEP_RET_32K_RC_WAKEUP     		32
#define DEEP_RET_32K_XTAL_WAKEUP     	33
#if(MCU_CORE_B89)
#define DEEP_RET_DEBOUNCE_WAKEUP      	34
#elif(MCU_CORE_B87)
#define DEEP_RET_MDEC_WAKEUP      		34
#endif

#if(MCU_CORE_B89)
/* SHUTDOWN_MODE */
#define	SHUTDOWN_PAD_WAKEUP				41
#endif

#define PM_MODE			     			SUSPEND_PAD_WAKEUP


/* Define system clock */
#define CLOCK_SYS_CLOCK_HZ  	24000000

#if(MCU_CORE_B89)
#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#endif
#else
#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==32000000)
	#define SYS_CLK  	SYS_CLK_32M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==48000000)
	#define SYS_CLK  	SYS_CLK_48M_Crystal
#endif
#endif

/* List tick per second/millisecond/microsecond */
enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,				///< system tick per 1 second
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),		///< system tick per 1 millisecond
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),   ///< system tick per 1 microsecond
};












/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

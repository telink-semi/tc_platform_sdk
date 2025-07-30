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

#define IRQ_PIN                 KEY1

#define GPIO_DEMO_KEY			1	 //Short press SW2 intermittent trigger interrupts,short press KEY3 to generate an edge signal.
#define GPIO_DEMO_SQUARE_WAVE	2	 //long presses SW2 to continuously trigger interrupts,IRQ_PIN connects to KEY3, toggle KEY3 to generate a square wave signal.
#define GPIO_DEMO_MODE 			GPIO_DEMO_KEY

#define GPIO_HIGH_RESISTOR		   1
#if((!MCU_CORE_TC1211) && (!MCU_CORE_TC122X))
#define GPIO_IRQ				   2
#endif
#define GPIO_IRQ_RSIC0			   3
#define GPIO_IRQ_RSIC1			   4

#if(MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC1211 || MCU_CORE_TC122X)
#define GPIO_IRQ_RSIC2             5//only B80_A / B80_B / TC321X / TC1211 support
#if (MCU_CORE_TC321X)
#define GPIO_IRQ_RSIC3             6//only B80_A / B80_B / TC321X support
#endif
#if (MCU_CORE_B80)
#define GPIO_SEL_IRQ_GROUP         6//only  B80_A support
#elif (MCU_CORE_B80B)
#define GPIO_SEL_IRQ_NEW_RISC      6//only  B80_B support
#define GPIO_IRQ_NEW_RISC_NUM      RISC3
#endif
#endif

#define AUTO_TEST_MODE 			   10 // For internal testing, users need not care

#if (!MCU_CORE_TC321X) && (!MCU_CORE_TC1211) && (!MCU_CORE_TC122X)
#define GPIO_MODE 				GPIO_IRQ
#else
#define GPIO_MODE 				GPIO_IRQ_RSIC0
#endif


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

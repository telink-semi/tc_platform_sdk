/********************************************************************************************************
 * @file    common.h
 *
 * @brief    This is the header file for B85m
 *
 * @author    Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#ifndef __COMMON_H_
#define __COMMON_H_

#if (MCU_CORE_B85 || MCU_CORE_B87)
#include "zb_flash_ctrl.h"
#endif
#include "calibration.h"
#include "hal_driver/flash/hal_flash.h"
#include "gpio.h"

/**
 *   ===============================================================================
 *                        ##### LED and KEY #####
 *   ===============================================================================
 *   The default configuration of LEDs and KEYs match the following hardware revisions:
 *   B80  C1T261A30_V1_1
 *   B80B C1T321A30_V1_0
 *   B85  C1T139A30_V1_2
 *   B87  C1T197A30_V1_2
 */
#if (MCU_CORE_B89)
#define LED1 GPIO_PD0
#define LED2 GPIO_PD1
#define LED3 GPIO_PD2
#define LED4 GPIO_PD3
/*
 * Button matrix table:
 *             KEY3       KEY4
 *     KEY1    SW2        SW3
 *     KEY2    SW4        SW5
 */
#define KEY1 GPIO_PD4
#define KEY2 GPIO_PD5
#define KEY3 GPIO_PD6
#define KEY4 GPIO_PD7
#elif (MCU_CORE_B87)
#define LED1 GPIO_PD2
#define LED2 GPIO_PD3
#define LED3 GPIO_PD4
#define LED4 GPIO_PD5
/*
 * Button matrix table:
 *             KEY3       KEY4
 *     KEY1    SW2        SW3
 *     KEY2    SW4        SW5
 */
#define KEY1 GPIO_PB2
#define KEY2 GPIO_PB3
#define KEY3 GPIO_PB4
#define KEY4 GPIO_PB5
#elif (MCU_CORE_B85)
#define LED1 GPIO_PD2
#define LED2 GPIO_PD3
#define LED3 GPIO_PD4
#define LED4 GPIO_PD5
/*
 * Button matrix table:
 *             KEY3       KEY4
 *     KEY1    SW2        SW3
 *     KEY2    SW4        SW5
 */
#define KEY1 GPIO_PB2
#define KEY2 GPIO_PB3
#define KEY3 GPIO_PB4
#define KEY4 GPIO_PB5
#elif (MCU_CORE_B80)
#define LED1 GPIO_PA4
#define LED2 GPIO_PA5
#define LED3 GPIO_PA6
#define LED4 GPIO_PA7
/*
 * Button matrix table:
 *             KEY3       KEY4
 *     KEY1    SW2        SW3
 *     KEY2    SW4        SW5
 */
#define KEY1 GPIO_PA0
#define KEY2 GPIO_PD4
#define KEY3 GPIO_PF0
#define KEY4 GPIO_PF1
#elif (MCU_CORE_B80B)
#define LED1 GPIO_PA4
#define LED2 GPIO_PA5
#define LED3 GPIO_PA6
#define LED4 GPIO_PA7
/*
 * Button matrix table:
 *             KEY3       KEY4
 *     KEY1    SW2        SW3
 *     KEY2    SW4        SW5
 */
#define KEY1 GPIO_PA0
#define KEY2 GPIO_PD4
#define KEY3 GPIO_PF0
#define KEY4 GPIO_PF1
#endif

void flash_init(unsigned char flash_protect_en);

void platform_init(unsigned char flash_protect_en);

#define PLATFORM_INIT platform_init(1)
#define CLOCK_INIT clock_init(SYS_CLK)

#endif /* end of __COMMON_H_ */

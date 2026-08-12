/********************************************************************************************************
 * @file    common.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/*
 * common.h
 *
 *  Created on: Sep 5, 2024
 *      Author: Admin
 */

#ifndef VENDOR_MULTIBIN_DEMO_COMMON_COMMON_H_
#define VENDOR_MULTIBIN_DEMO_COMMON_COMMON_H_

#include "driver.h"
typedef void  (* func_ptr_args_int)(unsigned int);
typedef void (* func_ptr_args_GPIO_PinTypeDef_int)(GPIO_PinTypeDef,unsigned int);
typedef void (* func_ptr_args_GPIO_PinTypeDef)(GPIO_PinTypeDef);
typedef void (* func_ptr_args_GPIO_PinTypeDef_gpio_func_e)(GPIO_PinTypeDef,gpio_func_e);
typedef void (* func_ptr_args_GPIO_PinTypeDef_GPIO_PolTypeDef)(GPIO_PinTypeDef,GPIO_PolTypeDef);
typedef void(* func_ptr_args_void)(void);
typedef unsigned char (* func_ptr_void_ret_char)(void);
typedef void(* func_ptr_args_cap_typedef_e)(cap_typedef_e);
typedef void(* func_ptr_args_SYS_CLK_TypeDef)(SYS_CLK_TypeDef);
typedef int(* func_ptr_args_SleepMode_TypeDef_SleepWakeupSrc_TypeDef_pm_wakeup_tick_type_e_int)(SleepMode_TypeDef,SleepWakeupSrc_TypeDef,pm_wakeup_tick_type_e,unsigned int);

// No need to list interfaces that are never called by upper layers; internal calls within the interface are fine
typedef struct app_spi_s {
//init
	func_ptr_args_cap_typedef_e cpu_wakeup_init;
	func_ptr_args_void wd_32k_stop;
	func_ptr_args_SYS_CLK_TypeDef clock_init;
//gpio
	func_ptr_args_GPIO_PinTypeDef_int gpio_set_output_en;
	func_ptr_args_GPIO_PinTypeDef_int gpio_set_input_en;
	func_ptr_args_GPIO_PinTypeDef gpio_toggle;
	func_ptr_args_GPIO_PinTypeDef_gpio_func_e gpio_set_func;
	func_ptr_args_GPIO_PinTypeDef_int gpio_write;
//stimer
	func_ptr_args_int sleep_us;

//suspend
	func_ptr_args_void blc_pm_select_internal_32k_crystal;
	func_ptr_args_SleepMode_TypeDef_SleepWakeupSrc_TypeDef_pm_wakeup_tick_type_e_int cpu_sleep_wakeup_32k_rc;

};

#endif /* VENDOR_MULTIBIN_DEMO_COMMON_COMMON_H_ */

/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
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
#pragma once
#include "driver.h"
#include "common.h"
/* Enable C linkage for C++ compilers: */
#if defined(__cplusplus)
extern "C"{
#endif

/**
 * @brief Define system clock
 */
#define CLOCK_SYS_CLOCK_HZ 16000000

/**
 * @note The sys_clock.h file depends on the definition of CLOCK_SYS_CLOCK_HZ so CLOCK_SYS_CLOCK_HZ must be placed before #include "sys_clock.h" without reversing the order.
 */
#include "sys_clock.h"

#define IR_DIGITAL_TX_MODE 0 /* The digital pins output PWM waveforms just for sending to the IR. */
#define IR_DIGITAL_RX_MODE 1
#define IR_ANALOG_TX_MODE  2
#define IR_ANALOG_RX_MODE  3
#define IR_LEARN_MODE      IR_ANALOG_TX_MODE

#define IR_LEARN_PM_FUNCTION_EN  0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

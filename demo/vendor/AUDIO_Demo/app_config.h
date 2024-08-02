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

#define AUDIO_AMIC_TO_SDM					1
#define AUDIO_DMIC_TO_SDM					2
#define AUDIO_AMIC_TO_CODEC					3
#define AUDIO_BUF_TO_SDM					4
#define AUDIO_AMIC_TO_BUF					5
#define AUDIO_AMIC_TO_BUF_TO_SDM			6			/* MUST enable AUDIO_DBL_BUF_ENABLE in audio.h */
#define AUDIO_AMIC_TO_SDM_BIDIR_SIGFIFO		7
#define AUDIO_AMIC_TO_SDM_BIDIR_BIDFIFO		8			/* MUST enable AUDIO_DBL_BUF_ENABLE in audio.h */
#define AUDIO_BUF_TO_SDM_BY_TIMER_IRQ		9
#if(MCU_CORE_B85)
#define AUDIO_CODEC_TO_CODEC				10
#endif
#define AUDIO_SINE_WAVE_TO_SDM		          11      /*The capacitance between SDMP and SDMN(C21 220nF default)needs to be set to 1uF */
#define AUDIO_MODE							AUDIO_AMIC_TO_SDM

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

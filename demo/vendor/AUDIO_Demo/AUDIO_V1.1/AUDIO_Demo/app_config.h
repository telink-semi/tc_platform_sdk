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

/**
 *  TC321X
 *  codec support   input: line_in / amic_in / dmic_in
 *                  output: support SDM out
 *  I2S support 1 I2S (I2S0)
 */
/* audio codec case */
#define    LINE_INPUT_TO_BUF_TO_LINEOUT        1  //line_in->buff->SDM out
#define    AMIC_INPUT_TO_BUF_TO_LINEOUT        2  //amic_in->buff->SDM out
#define    DMIC_INPUT_TO_BUF_TO_LINEOUT        3  //dmic_in->buff->SDM out
#define    BUFF_TO_LINEOUT                     4  //buff->SDM out
/* audio i2s case */
#define    I2S_TO_I2S                          10  //I2S input->buff->I2S out
/* audio mix case */
#define    LINE_INPUT_TO_BUF_TO_I2S            20 //line_in->buff->I2S out
#define    AMIC_INPUT_TO_BUF_TO_I2S            21 //amic_in->buff->I2S out
#define    DMIC_INPUT_TO_BUF_TO_I2S            22 //dmic_in->buff->I2S out

#define    AUDIO_MODE                          AMIC_INPUT_TO_BUF_TO_LINEOUT

#if ((AUDIO_MODE == LINE_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_LINEOUT))
/* Comment this macro if not used. */
//#define    AUDIO_CODEC_POWER_TEST              1 //codec power off, power on and reinit loop test
#endif

/* There are 3 steps in order to clear codec pop:
 * Step1 - mute audio;
 * Step2 - Clear codec input pop and dis mute audio;
 * Step3 - enable codec, codec data come in.
 */
#define AUDIO_CLR_CODEC_POP 1

/* Collect enough codec data and make it fade in. */
#define AUDIO_CODEC_FADE_IN 1

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

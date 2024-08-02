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

#define	 USB_MOUSE          1
#define	 USB_KEYBOARD		2
#define	 USB_CDC	        3
#if (!MCU_CORE_B80 && !MCU_CORE_B80B)
#define	 USB_MICROPHONE		4
#define	 USB_SPEAKER		5
#endif

#define	 USB_DEMO_TYPE		USB_MOUSE


#if	(USB_DEMO_TYPE == USB_MOUSE)
	#define	USB_MOUSE_ENABLE 		1
#elif (USB_DEMO_TYPE == USB_KEYBOARD)
	#define	USB_KEYBOARD_ENABLE 	1
#elif (USB_DEMO_TYPE == USB_MICROPHONE)
	#define	  USB_MIC_ENABLE 		1
#elif (USB_DEMO_TYPE == USB_SPEAKER)
	#define	USB_SPEAKER_ENABLE 		1
#elif (USB_DEMO_TYPE == USB_CDC)
	#define USB_CDC_ENABLE          1
#endif




#define ON            				1
#define OFF           				0


/*config for spk */
#define AUDIO_USB_TO_SDM			1

#define  AUDIO_SPK_MODE				1

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

/********************************************************************************************************
 * @file    usb_default.h
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

#define	AUTO_TEST_SWITCH		0

#if(1 == AUTO_TEST_SWITCH)
#define	FLOW_NO_OS				1
#define	USB_PRINTER_ENABLE 		1
#define	USB_MOUSE_ENABLE 		1
#define ID_VENDOR				0x248a		// for report
#define ID_VERSION              0x0100
#define STRING_VENDOR			L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT			L"Telink Mouse"
#define STRING_SERIAL			L"Mouse demo"

#define CHIP_U26				1
#define CHIP_U27				2
#define CHIP_LABEL				CHIP_U27

#if(CHIP_LABEL == CHIP_U26)
#define ID_PRODUCT	   			0x8726
#elif(CHIP_LABEL == CHIP_U27)
#define ID_PRODUCT	   			0x8727
#endif

#if (MCU_CORE_B80B)
/* control endpoint size config. */
#define USB_CTR_ENDPOINT_SIZE       8 /* 8/16/32/64 */
#define USB_CTR_SIZE                (USB_CTR_ENDPOINT_SIZE == 64) ? SIZE_64_BYTE :                 \
                                    ((USB_CTR_ENDPOINT_SIZE == 32) ? SIZE_32_BYTE :                \
                                    ((USB_CTR_ENDPOINT_SIZE == 16) ? SIZE_16_BYTE :                \
                                    ((USB_CTR_ENDPOINT_SIZE == 8) ? SIZE_8_BYTE : SIZE_64_BYTE)))
#endif

/* control endpoint size default is 8 bytes. */
#ifndef USB_CTR_ENDPOINT_SIZE
#define USB_CTR_ENDPOINT_SIZE       8
#endif

#else
	#include "app_config.h"
//////////// product  Information  //////////////////////////////
#define	FLOW_NO_OS				1
#define APPLICATION_DONGLE							1
#if(APPLICATION_DONGLE)
#ifndef USB_PRINTER_ENABLE
		#define	USB_PRINTER_ENABLE 		0
	#endif

	#ifndef	USB_MOUSE_ENABLE
		#define	USB_MOUSE_ENABLE 		0
	#endif

	#ifndef USB_KEYBOARD_ENABLE
		#define	USB_KEYBOARD_ENABLE 	0
	#endif

	#ifndef USB_MIC_ENABLE
		#define	USB_MIC_ENABLE 			0
	#endif

	#ifndef USB_SPEAKER_ENABLE
		#define	USB_SPEAKER_ENABLE 		0
	#endif

	#ifndef USB_CDC_ENABLE
		#define USB_CDC_ENABLE          0
	#endif

	#ifndef USB_SOMATIC_ENABLE
		#define	USB_SOMATIC_ENABLE      0   //  when USB_SOMATIC_ENABLE, USB_EDP_PRINTER_OUT disable
	#endif

	#ifndef USB_CUSTOM_HID_REPORT
		#define USB_CUSTOM_HID_REPORT	0
	#endif
#endif

#if (MCU_CORE_B80 || MCU_CORE_B85 || MCU_CORE_B87 || MCU_CORE_B89)
#define USB_PHYSICAL_EDP_CDC_IN     USB_EDP_CDC_IN  /* physical in endpoint */
#define USB_PHYSICAL_EDP_CDC_OUT    USB_EDP_CDC_OUT /* physical out endpoint */
#elif (MCU_CORE_B80B)
/* control endpoint size config. */
#define USB_CTR_ENDPOINT_SIZE       8 /* 8/16/32/64 */
#define USB_CTR_SIZE                (USB_CTR_ENDPOINT_SIZE == 64) ? SIZE_64_BYTE :                 \
                                    ((USB_CTR_ENDPOINT_SIZE == 32) ? SIZE_32_BYTE :                \
                                    ((USB_CTR_ENDPOINT_SIZE == 16) ? SIZE_16_BYTE :                \
                                    ((USB_CTR_ENDPOINT_SIZE == 8) ? SIZE_8_BYTE : SIZE_64_BYTE)))

#define USB_MAP_EN                  0 /* 1:usb map function enable, 0:usb map function disable. */

#define USB_PHYSICAL_EDP_CDC_IN     USB_EDP_CDC_IN  /* physical in endpoint */
#define USB_PHYSICAL_EDP_CDC_OUT    USB_EDP_CDC_OUT /* physical out endpoint */

#if (USB_MAP_EN == 1)
#define CDC_RX_EPNUM                USB_EDP_CDC_OUT /* logical in endpoint */
#define CDC_TX_EPNUM                USB_EDP_CDC_OUT /* logical out endpoint */
#else
#define CDC_RX_EPNUM                USB_PHYSICAL_EDP_CDC_OUT /* USB_MAP_EN = 0, logical endpoint is the same as the physical endpoint */
#define CDC_TX_EPNUM                USB_PHYSICAL_EDP_CDC_IN /* USB_MAP_EN = 0, logical endpoint is the same as the physical endpoint*/
#endif

#endif

/* control endpoint size default is 8 bytes. */
#ifndef USB_CTR_ENDPOINT_SIZE
#define USB_CTR_ENDPOINT_SIZE       8
#endif

//////////////////// Audio /////////////////////////////////////
#define MIC_RESOLUTION_BIT		16
#define MIC_SAMPLE_RATE			16000//set sample for mic and spk
#define MIC_CHANNEL_COUNT		1
#define	MIC_ENCODER_ENABLE		0

#define SPK_RESOLUTION_BIT		16
#define SPEAKER_SAMPLE_RATE     16000
#define   SPK_CHANNEL_COUNT     1
#if(USB_MIC_ENABLE||USB_SPEAKER_ENABLE)
	#define USB_MODE_AUDIO_EN				1
#endif


#define ID_VENDOR				0x248a			// for report

#if(USB_CDC_ENABLE)
#define ID_PRODUCT			    0x8002
#else
#define ID_PRODUCT			    0x8006
#endif

#define  ID_VERSION             0x0100

#if(USB_MODE_CDC_EN)
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Telink CDC"
#define STRING_SERIAL				L"CDC demo "
#endif

#if(USB_MOUSE_ENABLE&(!USB_KEYBOARD_ENABLE))
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Telink Mouse"
#define STRING_SERIAL				L"Mouse demo"
#endif

#if((!USB_MOUSE_ENABLE)&USB_KEYBOARD_ENABLE)
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Tek Keyboard"
#define STRING_SERIAL				L"Keyboard demo"
#endif

#if((USB_MOUSE_ENABLE)&USB_KEYBOARD_ENABLE)
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Telink KM"
#define STRING_SERIAL				L"KM demo"
#endif

#if(USB_CDC_ENABLE)
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Telink CDC"
#define STRING_SERIAL				L"CDC demo "
#endif


#if(USB_MODE_AUDIO_EN)
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Telink Audio16"
#define STRING_SERIAL				L"Audio16 demo"
#endif

#if((!USB_MODE_AUDIO_EN)&&(!USB_KEYBOARD_ENABLE)&&(!USB_MOUSE_ENABLE)&&(!USB_MODE_CDC_EN)&&(!USB_CDC_ENABLE))
#define STRING_VENDOR				L"Telink Semi-conductor Ltd, Co"
#define STRING_PRODUCT				L"Telink No Product"
#define STRING_SERIAL				L"USB demo"
#endif
#endif


///////////////////  USB   /////////////////////////////////
#ifndef IRQ_USB_PWDN_ENABLE
#define	IRQ_USB_PWDN_ENABLE  	0
#endif

#ifndef USB_PRINTER_ENABLE
#define	USB_PRINTER_ENABLE 		0
#endif
#ifndef USB_SPEAKER_ENABLE
#define	USB_SPEAKER_ENABLE 		0
#endif
#ifndef USB_MIC_ENABLE
#define	USB_MIC_ENABLE 			0
#endif
#ifndef USB_MOUSE_ENABLE
#define	USB_MOUSE_ENABLE 			0
#endif
#ifndef USB_KEYBOARD_ENABLE
#define	USB_KEYBOARD_ENABLE 		0
#endif
#ifndef USB_SOMATIC_ENABLE
#define	USB_SOMATIC_ENABLE 		0
#endif
#ifndef USB_CUSTOM_HID_REPORT
#define	USB_CUSTOM_HID_REPORT 		0
#endif
#ifndef USB_AUDIO_441K_ENABLE
#define USB_AUDIO_441K_ENABLE  	0
#endif
#ifndef USB_MASS_STORAGE_ENABLE
#define USB_MASS_STORAGE_ENABLE  	0
#endif

#ifndef MIC_CHANNEL_COUNT
#define MIC_CHANNEL_COUNT  			1
#endif

#ifndef USB_DESCRIPTOR_CONFIGURATION_FOR_KM_DONGLE
#define USB_DESCRIPTOR_CONFIGURATION_FOR_KM_DONGLE  			0
#endif

#ifndef USB_ID_AND_STRING_CUSTOM
#define USB_ID_AND_STRING_CUSTOM  								0
#endif

#define KEYBOARD_RESENT_MAX_CNT			3
#define KEYBOARD_REPEAT_CHECK_TIME		300000	// in us
#define KEYBOARD_REPEAT_INTERVAL		100000	// in us
#define KEYBOARD_SCAN_INTERVAL			16000	// in us
#define MOUSE_SCAN_INTERVAL				8000	// in us
#define SOMATIC_SCAN_INTERVAL     		8000

#define USB_KEYBOARD_POLL_INTERVAL		10		// in ms	USB_KEYBOARD_POLL_INTERVAL < KEYBOARD_SCAN_INTERVAL to ensure PC no missing key
#define USB_MOUSE_POLL_INTERVAL			4		// in ms
#define USB_SOMATIC_POLL_INTERVAL     	8		// in ms

#define USB_KEYBOARD_RELEASE_TIMEOUT    (450000) // in us
#define USB_MOUSE_RELEASE_TIMEOUT       (200000) // in us
#define USB_SOMATIC_RELEASE_TIMEOUT     (200000) // in us

/*config for swire through USB DP, it requires a call to usb_set_pin_en() at the same time to enable this function*/
#ifndef SWIRE_THROUGH_USB_DP_ENABLE
#define SWIRE_THROUGH_USB_DP_ENABLE     1
#endif




/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif


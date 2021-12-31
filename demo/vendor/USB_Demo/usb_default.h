/********************************************************************************************************
 * @file	usb_default.h
 *
 * @brief	This is the header file for b85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#pragma once
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


//////////////////// Audio /////////////////////////////////////
#define MIC_RESOLUTION_BIT		16
#define MIC_SAMPLE_RATE			16000//set sample for mic and spk
#define MIC_CHANNLE_COUNT		1
#define	MIC_ENOCDER_ENABLE		0

#define SPK_RESOLUTION_BIT		16
#define SPEAKER_SAMPLE_RATE     16000
#define   SPK_CHANNLE_COUNT     1
#if(USB_MIC_ENABLE||USB_SPEAKER_ENABLE)
	#define USB_MODE_AUDIO_EN				1
#endif


#define ID_VENDOR				0x248a			// for report

#if(USB_CDC_ENABLE)
#define ID_PRODUCT			    0x8002
#else
#define ID_PRODUCT			    0x8006
#endif

#define  ID_VERDION             0x0100

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

#ifndef MIC_CHANNLE_COUNT
#define MIC_CHANNLE_COUNT  			1
#endif

#ifndef USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE
#define USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE  			0
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






/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif


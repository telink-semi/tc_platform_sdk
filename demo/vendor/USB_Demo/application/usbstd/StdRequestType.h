/********************************************************************************************************
 * @file	StdRequestType.h
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
/* Includes: */
#include "types.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif


#define CONTROL_REQTYPE_TYPE       		0x60
#define CONTROL_REQTYPE_DIRECTION  		0x80
#define CONTROL_REQTYPE_RECIPIENT  		0x1F
#define REQDIR_HOSTTODEVICE        		(0<<7)
#define REQDIR_DEVICETOHOST        		(1<<7)
#define REQTYPE_STANDARD           		(0<<5)
#define REQTYPE_CLASS              		(1<<5)
#define REQTYPE_VENDOR             		(2<<5)
#define REQREC_DEVICE              		(0<<0)
#define REQREC_INTERFACE           		(1<<0)
#define REQREC_ENDPOINT            		(2<<0)
#define REQREC_OTHER               		(3<<0)
#define FEATURE_SELFPOWERED_ENABLED     (1<<0)
#define FEATURE_REMOTE_WAKEUP_ENABLED   (1<<1)

typedef struct
{
	u8  RequestType;
	u8  Request;
	u16 Value;
	u16 Index;
	u16 Length;
}USB_Request_Hdr_t;

enum USB_Control_Request_t
{
	REQ_GetStatus,
	REQ_ClearFeature,
	REQ_SetFeature = 3,
	REQ_SetAddress = 5,
	REQ_GetDescriptor,
	REQ_SetDescriptor,
	REQ_GetConfiguration,
	REQ_SetConfiguration,
	REQ_GetInterface,
	REQ_SetInterface,
	REQ_SynchFrame,
};


enum USB_Feature_Selectors_t
{
	FEATURE_SEL_EndpointHalt,
	FEATURE_SEL_DeviceRemoteWakeup,
	FEATURE_SEL_TestMode,
};

#if defined(__cplusplus)
	}
#endif




/********************************************************************************************************
 * @file	PrinterClassCommon.h
 *
 * @brief	This is the header file for B85m
 *
 * @author	Driver Group
 * @date	2018
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
	/* Includes: */
#include "types.h"
	
	/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif
	

#define PRNT_PORTSTATUS_NOTERROR	BIT(3)
#define PRNT_PORTSTATUS_SELECT		BIT(4)
#define PRNT_PORTSTATUS_PAPEREMPTY	BIT(5)

enum PRNT_Descriptor_ClassSubclassProtocol_t
{
	PRNT_CSCP_PrinterClass = 0x07,
	PRNT_CSCP_PrinterSubclass = 0x01,
	PRNT_CSCP_BidirectionalProtocol = 0x02,
};

enum PRNT_ClassRequests_t
{
	PRNT_REQ_GetDeviceID,
	PRNT_REQ_GetPortStatus,
	PRNT_REQ_SoftReset,
};

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif


/********************************************************************************************************
 * @file	usbcdc.h
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
#include "../../usb_default.h"
#if USB_CDC_ENABLE
#include "usbhw.h"
/** Endpoint number of the CDC device-to-host notification IN endpoint. */
#define CDC_NOTIFICATION_EPNUM         2

/** Endpoint number of the CDC device-to-host data IN endpoint. */
#ifndef		CDC_TX_EPNUM
#define     CDC_TX_EPNUM               4
#endif

/** Endpoint number of the CDC host-to-device data OUT endpoint. */
#define CDC_RX_EPNUM                   5

/** Size in bytes of the CDC device-to-host notification IN endpoint. */
#define CDC_NOTIFICATION_EPSIZE        8

/** Size in bytes of the CDC data IN and OUT endpoints.*/
#define CDC_TXRX_EPSIZE                64

extern unsigned char  usb_cdc_data[CDC_TXRX_EPSIZE];
extern unsigned short usb_cdc_data_len;

extern unsigned int usb_cdc_tx_cnt;
extern unsigned char LineCoding[7];

/**
 * @brief		This function serves to send data to USB host in CDC device
 * @param[in] 	data_ptr -  the pointer of data, which need to be sent.
 * @param[in] 	data_len -  the length of data, which need to be sent.
 * @return 		none
 */
 void usb_cdc_tx_data_to_host(unsigned char * data_ptr, unsigned short data_len);
 /**
  * @brief		This function serves to receive data from
  * @param[in] 	rx_buff -  the pointer of data, which need to receive.
  * @return 		none
  */
void usb_cdc_rx_data_from_host(unsigned char* rx_buff);

#endif

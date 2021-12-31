/********************************************************************************************************
 * @file	usbcdc.c
 *
 * @brief	This is the source file for b85m
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
#include "usbcdc.h"
#if(USB_CDC_ENABLE)
#include "../usbstd/usb.h"
unsigned char  usb_cdc_data[CDC_TXRX_EPSIZE];
unsigned short usb_cdc_data_len;
unsigned int usb_cdc_tx_cnt;
unsigned char LineCoding[7]={0x00,0xC2,0x01,0x00,0x00,0x00,0x08};
unsigned char cdc_lenth=0;
/**
 * @brief		This function serves to send data to USB host in CDC device
 * @param[in] 	data_ptr -  the pointer of data, which need to be sent.
 * @param[in] 	data_len -  the length of data, which need to be sent.
 * @return 		none
 */
void usb_cdc_tx_data_to_host(unsigned char * data_ptr, unsigned short data_len)
{

	if(usbhw_is_ep_busy(USB_EDP_CDC_IN))
	{
		return;
	}

	if(data_len >CDC_TXRX_EPSIZE)
	{
		data_len = CDC_TXRX_EPSIZE;

	}
	cdc_lenth=data_len;
	usbhw_reset_ep_ptr(USB_EDP_CDC_IN);

	while(data_len-- > 0) {
		reg_usb_ep_dat(USB_EDP_CDC_IN) = (*data_ptr);
		++data_ptr;
	}

	usbhw_data_ep_ack(USB_EDP_CDC_IN);
	/*If the length of the data sent is equal to the wMaxPacketSize (CDC_TXRX_EPSIZE),
		 the device must return a zero-length packet to indicate the end of the data stage,
		 The following is the process of sending zero-length packet*/
	if(cdc_lenth%CDC_TXRX_EPSIZE==0)
	{
		sleep_us(64);//delay for the actual data transfer to complete
		usbhw_reset_ep_ptr(USB_EDP_CDC_IN);
		usbhw_data_ep_ack(USB_EDP_CDC_IN);
	}

}

/**
 * @brief		This function serves to receive data from host.
 * @param[in] 	rx_buff -  the pointer of data, which need to receive.
 * @return 		none
 */
void usb_cdc_rx_data_from_host(unsigned char* rx_buff)
{
	unsigned char rx_from_usbhost_len = reg_usb_ep_ptr(USB_EDP_CDC_OUT);
	usbhw_reset_ep_ptr(USB_EDP_CDC_OUT);
	if(rx_from_usbhost_len > 0 && rx_from_usbhost_len <= CDC_TXRX_EPSIZE)
	{
		for(int i = 0; i < rx_from_usbhost_len; ++i)
		{
			rx_buff[i] = reg_usb_ep_dat(USB_EDP_CDC_OUT);
		}
		usb_cdc_data_len = rx_from_usbhost_len&0xff;
	}
}
#endif

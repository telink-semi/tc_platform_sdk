/********************************************************************************************************
 * @file    usbhw.h
 *
 * @brief   This is the header file for B85
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
#include "register.h"
#include "types.h"
enum {
	USB_EDP_PRINTER_IN = 8, // endpoint 8 is alias of endpoint 0,  becareful.  // default hw buf len = 64
	USB_EDP_MOUSE = 2,			// default hw buf len = 8
	USB_EDP_KEYBOARD_IN = 1,	// default hw buf len = 8
	USB_EDP_IN = 3,	            // default hw buf len = 16
	USB_EDP_AUDIO_IN = 4,		// default hw buf len = 64
	USB_EDP_PRINTER_OUT = 5,	// default hw buf len = 64
	USB_EDP_SPEAKER = 6,		// default hw buf len = 16
	USB_EDP_MIC = 7,			// default hw buf len = 16
	USB_EDP_MS_IN = USB_EDP_PRINTER_IN,		// mass storage
	USB_EDP_MS_OUT = USB_EDP_PRINTER_OUT,
	USB_EDP_SOMATIC_IN = USB_EDP_AUDIO_IN,		//  when USB_SOMATIC_ENABLE, USB_EDP_PRINTER_OUT disable
	USB_EDP_SOMATIC_OUT = USB_EDP_PRINTER_OUT,
    USB_EDP_CDC_IN = 4,
    USB_EDP_CDC_OUT = 5,
};

// #defined in the standard spec
enum {
	USB_HID_AUDIO       	= 2,
	USB_HID_MOUSE       	= 1,
	USB_HID_KB_MEDIA    	= 3,// media
	USB_HID_KB_SYS      	= 4,// system : power,sleep,wakeup
	USB_HID_SOMATIC			= 5,// somatic sensor,  may have many report ids
};

static inline void usbhw_set_printer_threshold(unsigned char th) {
	reg_usb_ep8_send_thres = th;
}

/**
 * @brief     This function servers to reset the pointer of control Endpoint.
 * @param[in] data - the value of control Endpoint
 * @return    none.
 */
static inline void usbhw_reset_ctrl_ep_ptr(void) {
	reg_ctrl_ep_ptr = 0;
}

/**
 * @brief     This function servers to get the irq status of control Endpoint.
 * @param[in] none.
 * @return    none.
 */
static inline unsigned int usbhw_get_ctrl_ep_irq(void) {
	return reg_ctrl_ep_irq_sta;
}

/**
 * @brief     This function servers to clear the irq status of control Endpoint.
 * @param[in] ep - select the Endpoint
 * @return    none.
 */
static inline void usbhw_clr_ctrl_ep_irq(int ep) {
	reg_ctrl_ep_irq_sta = ep;
}

/**
 * @brief     This function servers to set the value of control Endpoint.
 * @param[in] data - the value of control Endpoint
 * @return    none.
 */
static inline void usbhw_write_ctrl_ep_ctrl(unsigned char data) {
	reg_ctrl_ep_ctrl = data;
}

/**
 * @brief     This function servers to read the data of control Endpoint.
 * @param[in] none.
 * @return    the value of control Endpoint
 */
static inline unsigned char usbhw_read_ctrl_ep_data(void) {
	return reg_ctrl_ep_dat;
}

/**
 * @brief     This function servers to write the data of control Endpoint.
 * @param[in] data -  the value of control Endpoint
 * @return    none
 */
static inline void usbhw_write_ctrl_ep_data(unsigned char data) {
	reg_ctrl_ep_dat = data;
}

/**
 * @brief     This function servers to determine whether control Endpoint is busy.
 * @param[in] none.
 * @return    1: busy; 0: not busy.
 */
static inline int usbhw_is_ctrl_ep_busy(void) {
	return reg_ctrl_ep_irq_sta & FLD_USB_EP_BUSY;
}


/**
 * @brief     This function servers to reset the pointer of Endpoint.
 * @param[in] data - the value of control Endpoint
 * @return    none.
 */
static inline void usbhw_reset_ep_ptr(unsigned int ep) {
	reg_usb_ep_ptr(ep & 0x07) = 0;
}



/**
 * @brief     This function servers to get the irq status of Endpoint.
 * @param[in] none.
 * @return    none.
 */
static inline unsigned int usbhw_get_eps_irq(void) {
	return reg_usb_irq;
}
/**
 * @brief     This function servers to clear the irq status of Endpoint.
 * @param[in] ep - select the Endpoint
 * @return    none.
 */
static inline void usbhw_clr_eps_irq(int ep) {
	reg_usb_irq = ep;
}

/**
 * @brief     This function servers to read the data of Endpoint.
 * @param[in] none.
 * @return    the value of Endpoint
 */
static inline unsigned char usbhw_read_ep_data(unsigned int ep) {
	return reg_usb_ep_dat(ep & 0x07);
}

/**
 * @brief     This function servers to write the data of Endpoint.
 * @param[in] data -  the value of Endpoint
 * @return    none
 */
static inline void usbhw_write_ep_data(unsigned int ep, unsigned char data) {
	reg_usb_ep_dat(ep & 0x07) = data;
}

/**
 * @brief     This function servers to determine whether Endpoint is busy.
 * @param[in] none.
 * @return    1: busy; 0: not busy.
 */
static inline unsigned int usbhw_is_ep_busy(unsigned int ep) {
	return reg_usb_ep_ctrl(ep & 0x07) & FLD_USB_EP_BUSY;
}

/**
 * @brief     This function servers to set the specified data EndPoint to ack.
 * @param[in] ep -  select the data EndPoint.
 * @return    none.
 */
static inline void usbhw_data_ep_ack(unsigned int ep) {
	reg_usb_ep_ctrl(ep & 0x07) = FLD_USB_EP_BUSY;
}

/**
 * @brief     This function servers to set the specified data EndPoint to stall.
 * @param[in] ep -  select the data EndPoint.
 * @return    none.
 */
static inline void usbhw_data_ep_stall(unsigned int ep) {
	reg_usb_ep_ctrl(ep & 0x07) = FLD_USB_EP_STALL;
}

void usbhw_disable_manual_interrupt(int m);

/**
 * @brief      This function enable the manual interrupt
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_enable_manual_interrupt(int m);

/**
 * @brief      This function sends a bulk of data to host via the specified endpoint
 * @param[in]  ep - the number of the endpoint
 * @param[in]  data - pointer to the data need to send
 * @param[in]  len - length in byte of the data need to send
 * @return     none
 */
void usbhw_write_ep(unsigned int ep, unsigned char * data, int len);

/**
 * @brief      This function sends two bytes data to host via the control endpoint
 *             (handy help function)
 * @param[in]  v - the two bytes data need to send
 * @return     none
 */
void usbhw_write_ctrl_ep_u16(unsigned short v);

/**
 * @brief   This function reads two bytes data from host via the control endpoint
 * @param   none
 * @return  the two bytes data read from the control endpoint
 */
unsigned short usbhw_read_ctrl_ep_u16(void);

/**
 * @brief      This function serves to set dp_through_swire function.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 */
void dp_through_swire_en(bool dp_through_swire);

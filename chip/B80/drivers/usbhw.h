/********************************************************************************************************
 * @file    usbhw.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif

typedef enum
{
    USB_EDP8_IN         = 8,
    USB_EDP1_IN         = 1,
    USB_EDP2_IN         = 2,
    USB_EDP3_IN         = 3,
    USB_EDP4_IN         = 4,
    USB_EDP5_OUT        = 5,
    USB_EDP6_OUT        = 6,
    USB_EDP7_IN         = 7,

    USB_EDP_PRINTER_IN  = USB_EDP8_IN,
    USB_EDP_KEYBOARD_IN = USB_EDP1_IN,
    USB_EDP_MOUSE       = USB_EDP2_IN,
    USB_EDP3_UNUSED_IN  = USB_EDP3_IN,
    USB_EDP_AUDIO_IN    = USB_EDP4_IN,
    USB_EDP_PRINTER_OUT = USB_EDP5_OUT,
    USB_EDP_SPEAKER     = USB_EDP6_OUT,
    USB_EDP_MIC         = USB_EDP7_IN,
    USB_EDP_MS_IN       = USB_EDP8_IN,
    USB_EDP_MS_OUT      = USB_EDP5_OUT,
    USB_EDP_SOMATIC_IN  = USB_EDP4_IN,
    USB_EDP_SOMATIC_OUT = USB_EDP5_OUT,
    USB_EDP_CDC_IN      = USB_EDP4_IN,
    USB_EDP_CDC_OUT     = USB_EDP5_OUT,
} usb_ep_index;

// #defined in the standard spec
enum {
	USB_HID_AUDIO       	= 2,
	USB_HID_MOUSE       	= 1,
	USB_HID_KB_MEDIA    	= 3,// media
	USB_HID_KB_SYS      	= 4,// system : power,sleep,wakeup
	USB_HID_SOMATIC			= 5,// somatic sensor,  may have many report ids
};

#if (MCU_CORE_B80B)
typedef enum
{
    USB_IRQ_RESET_STATUS    = BIT(0),
    USB_IRQ_250US_STATUS    = BIT(1),
    USB_IRQ_SUSPEND_STATUS  = BIT(2),
    USB_IRQ_SOF_STATUS      = BIT(3),
    USB_IRQ_EP_SETUP_STATUS = BIT(4),
    USB_IRQ_EP_DATA_STATUS  = BIT(5),
    USB_IRQ_EP_STA_STATUS   = BIT(6),
    USB_IRQ_EP_INTF_STATUS  = BIT(7),
} usb_irq_status_e;

typedef enum
{
    EP_MAP_DIS,
    EP_MAP_AUTO_EN,
} usb_ep_map_sel_e;

typedef enum
{
    SIZE_8_BYTE,
    SIZE_16_BYTE,
    SIZE_32_BYTE,
    SIZE_64_BYTE,
} usb_ctrl_ep_sel_e;

/**
 * @brief     This function servers to change usb control endpoint reset, 250us and sof trigger type level to edge.
 * @return    none.
 * @note
 *            - This function must be called when the usb is initialized, otherwise the corresponding flags will not be set.
 */
static inline void usbhw_set_irq_edge(void)
{
    reg_usb_edp0_size &= ~(FLD_USB_IRQ_RESET_LVL | FLD_USB_IRQ_250US_LVL | FLD_USB_IRQ_SOF_LVL);
}

/**
 * @brief     This function servers to set the size of control endpoint.
 * @param[in] size - the size of control endpoint.
 * @return    none.
 */
static inline void usbhw_set_ctrl_ep_size(usb_ctrl_ep_sel_e size)
{
    reg_usb_edp0_size = ((reg_usb_edp0_size & ~(FLD_USB_CTR_EP_SIZE)) | size);
}

/**
 * @brief     This function servers to set the buffer address of data endpoint.
 * @param[in] ep - select the data endpoint.
 * @param[in] addr - data endpoint buffer address.
 * @return    none.
 */
static inline void usbhw_set_ep_addr(usb_ep_index ep, unsigned short addr)
{
    reg_usb_ep_buf_addr(ep) = addr & 0xff;
}

/**
 * @brief     This function servers to set usb irq mask.
 * @param[in]  mask -the irq mask of usb.
 * @return    none.
 */
static inline void usbhw_set_irq_mask(usb_irq_mask mask)
{
    reg_usb_irq_mask |= mask;
}

/**
 * @brief     This function servers to set the irq mask of data endpoint.
 * @param[in]  mask -the irq mask of data endpoint.
 * @return    none.
 */
static inline void usbhw_set_eps_irq_mask(usb_eps_irq_e mask)
{
    reg_usb_mask |= mask;
}

/**
 * @brief     This function servers to clr the irq mask of data endpoint.
 * @param[in]  mask -the irq mask of data endpoint.
 * @return    none.
 */
static inline void usbhw_clr_eps_irq_mask(usb_eps_irq_e mask)
{
    reg_usb_mask &= (~mask);
}

/**
 * @brief     This function servers to get usb irq status.
 * @param[in]  status -the irq status of usb.
 * @return    The status of irq.
 */
static inline unsigned char usbhw_get_irq_status(usb_irq_status_e status)
{
    return reg_ctrl_ep_irq_sta & status;
}

/**
 * @brief     This function servers to clr usb irq status.
 * @param[in]  status -the irq status of usb.
 * @return    none.
 */
static inline void usbhw_clr_irq_status(usb_irq_status_e status)
{
    reg_ctrl_ep_irq_sta = status;
}

/**
 * @brief     This function servers to get time stamp.
 * @return    the time stamp.
 */
static inline unsigned int usbhw_get_timer_stamp(void)
{
    return (reg_usb_tstamp0 | (reg_usb_tstamp1 << 8) | (reg_usb_tstamp2 << 16) | (reg_usb_tstamp3 << 24));
}

/**
 * @brief     This function servers to set in endpoint buffer max size(except 7 aiso mode).
 * @param[in] max_size - in endpoint max size.
 * @return    none.
 */
static inline void usbhw_set_eps_max_size(unsigned int max_size)
{
    reg_usb_ep_max_size = max_size >> 3;
}

/**
 * @brief     This function servers to enable mapping logical data endpoint.
 * @param[in] ep - select the data endpoint mask.
 * @return    none.
 */
static inline void usbhw_set_eps_map_en(usb_ep_en_e ep)
{
    reg_usb_edps_map_en = ep;
}

/**
 * @brief   This function serves to set data endpoint mapping.
 * @param   source_ep - The source endpoint of the mapping.
 * @param   target_ep - The target endpoint of the mapping.
 * @return    none.
 */
void usbhw_set_ep_map(usb_ep_index source_ep, usb_ep_index target_ep);

/**
 * @brief   This function serves to set data endpoint mapping.
 * @param   map_en - mapping enable or disable.
 * @return    none.
 */
void usbhw_ep_map_en(usb_ep_map_sel_e map_en);
#endif

/**
 * @brief     This function servers to set the threshold of printer.
 * @param[in] th - set the threshold for printer.
 * @return    none.
 */
static inline void usbhw_set_printer_threshold(unsigned char th) {
	reg_usb_ep8_send_thres = th;
}

#if (MCU_CORE_B80)
/**
 * @brief     This function servers to enable Endpoint.
 * @param[in] ep - selected  the Endpoint Bit.
 * @return    none.
 */
static inline void  usbhw_set_eps_en(usb_ep_en_e ep)
{
	reg_usb_ep_en= ep ;
}
#else
/**
 * @brief     This function servers to enable data endpoint.
 * @param[in] ep - select the data endpoint.
 * @return    none.
 * @note
 *            - when usb map en, reg_usb_edps_logic_en reg enable data endpoint.
 *            - when usb map dis, reg_usb_edp_en reg enable data endpoint.
 */
static inline void usbhw_set_eps_en(usb_ep_en_e ep)
{
    if (reg_usb_map & FLD_USB_EDPS_SM_MAP_EN)
    {
        reg_usb_edps_logic_en = ep;
    }
    else
    {
        reg_usb_ep_en = ep;
    }
}
#endif

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
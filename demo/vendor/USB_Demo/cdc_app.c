/********************************************************************************************************
 * @file	cdc_app.c
 *
 * @brief	This is the source file for B85m
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
#include "app_config.h"
#if(USB_DEMO_TYPE==USB_CDC)
#include "application/usb_app/usbcdc.h"
#include "application/usbstd/usb.h"

void user_init(void)
{
    usb_init();
#if (MCU_CORE_B80B)

#if (USB_MAP_EN == 1)
    /* When map enable, for the device side, there are three physical endpoints: notify endpoint 2, IN endpoint 4 and OUT endpoint 5. */
    usbhw_ep_map_en(EP_MAP_AUTO_EN);
    usbhw_set_eps_map_en(BIT(USB_PHYSICAL_EDP_CDC_OUT));
    /* enable data endpoint CDC_NOTIFICATION_EPNUM and USB_PHYSICAL_EDP_CDC_OUT. */
    usbhw_set_eps_en(BIT(CDC_NOTIFICATION_EPNUM) | BIT(USB_PHYSICAL_EDP_CDC_OUT));
    /* host sends data to logical endpoint 5, because our physical endpoint supports OUT, the physical endpoint and logical endpoint are the same, no mapping is needed. \n
     * host get data from logical endpoint 5, because our physical endpoints only support OUT, then you need to logical endpoint 5 mapping to any physical endpoint that supports IN,
     * the software only needs to configure the mapping relationship, the hardware will parse itself!
     */
    usbhw_set_ep_map(USB_PHYSICAL_EDP_CDC_OUT, USB_PHYSICAL_EDP_CDC_IN);
#else
    /* enable data endpoint CDC_NOTIFICATION_EPNUM, USB_PHYSICAL_EDP_CDC_IN and USB_PHYSICAL_EDP_CDC_OUT. */
    usbhw_set_eps_en(BIT(CDC_NOTIFICATION_EPNUM) | BIT(USB_PHYSICAL_EDP_CDC_IN) | BIT(USB_PHYSICAL_EDP_CDC_OUT));
#endif

    /* usb mask on */
    usbhw_set_eps_irq_mask(FLD_USB_EDP4_IRQ);
    irq_enable_type(FLD_IRQ_EP_DATA_EN);

#if (USB_ENUM_IN_INTERRUPT == 1)
    /* enable sof irq mask */
    usbhw_set_irq_mask(FLD_USB_IRQ_SOF_MASK | FLD_USB_IRQ_SETINF_MASK);
    irq_enable_type(FLD_IRQ_USB_250US_OR_SOF_EN | FLD_IRQ_SET_INTF_EN);
#endif
    irq_enable();

#elif (MCU_CORE_B87 || MCU_CORE_B80)
    /* enable data endpoint CDC_NOTIFICATION_EPNUM, USB_PHYSICAL_EDP_CDC_IN and USB_PHYSICAL_EDP_CDC_OUT. */
    usbhw_set_eps_en(BIT(CDC_NOTIFICATION_EPNUM) | BIT(USB_PHYSICAL_EDP_CDC_IN) | BIT(USB_PHYSICAL_EDP_CDC_OUT));
#endif

    //enable USB DP pull up 1.5k
    usb_set_pin_en();
}



void main_loop (void)
{
	usb_handle_irq();

	if(usb_cdc_data_len!=0)
	{
		usb_cdc_tx_data_to_host(usb_cdc_data,usb_cdc_data_len);
		usb_cdc_data_len = 0;
	}

}

#endif


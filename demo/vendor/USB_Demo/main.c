/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for B85m
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
#include "app_config.h"
#include "calibration.h"



extern void user_init(void);
extern void main_loop (void);

#if (MCU_CORE_B80B)
#include "application/usbstd/usb.h"
volatile unsigned int sof_cnt = 0;
volatile unsigned int set_intf_cnt;
volatile unsigned int sof_frame[4] = {0};
volatile unsigned int sys_tick[4] = {0};
volatile unsigned char usb_edps_irq_flag = 0;
#endif

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if (MCU_CORE_B80B)

#if (USB_ENUM_IN_INTERRUPT == 1)
    /* sof interrupt. */
    if (usbhw_get_irq_status(USB_IRQ_SOF_STATUS))
    {
        usbhw_clr_irq_status(USB_IRQ_SOF_STATUS);
        sof_cnt++;
        sof_frame[sof_cnt & 3] = (reg_usb_sof_frame1 << 8) | reg_usb_sof_frame0;
        sys_tick[sof_cnt & 3] = usbhw_get_timer_stamp();
    }
    /* set interface interrupt. */
    if (usbhw_get_irq_status(USB_IRQ_EP_INTF_STATUS))
    {
        usbhw_clr_irq_status(USB_IRQ_EP_INTF_STATUS);
        set_intf_cnt++;
    }
#endif

    /* edps irq */
    if (usbhw_get_eps_irq() & FLD_USB_EDP4_IRQ)
    {
    	usb_edps_irq_flag = usbhw_get_eps_irq();
        /* clear interrupt flag of endpoint 4 */
        usbhw_clr_eps_irq(FLD_USB_EDP4_IRQ);
    }
#endif
}
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}


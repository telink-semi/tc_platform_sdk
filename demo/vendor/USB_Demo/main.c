/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for Telink MCU
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

#if (MCU_CORE_B80B || MCU_CORE_TC1211)
#include "application/usbstd/usb.h"
volatile unsigned int sof_cnt = 0;
volatile unsigned int set_intf_cnt;
volatile unsigned int sof_frame[4] = {0};
volatile unsigned int sys_tick[4] = {0};
volatile unsigned char usb_edps_irq_flag = 0;
volatile unsigned int set_addr_cnt = 0;
#endif

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if (MCU_CORE_B80B || MCU_CORE_TC1211)

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

#if (MCU_CORE_TC1211)
    if (usbhw_get_set_addr_irq_status())
    {
        usbhw_clr_set_addr_irq_status();
        set_addr_cnt++;
        if (usbhw_get_set_addr_error_status())
        {
            usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
        }
    }
#endif /* MCU_CORE_TC1211 */
#endif /* USB_ENUM_IN_INTERRUPT */

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
#if(USB_DEMO_TYPE == USB_MOUSE_SLEEP)
	blc_pm_select_internal_32k_crystal();
#endif

    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}


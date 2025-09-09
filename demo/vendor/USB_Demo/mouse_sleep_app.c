/********************************************************************************************************
 * @file    mouse_sleep_app.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#if(USB_DEMO_TYPE == USB_MOUSE_SLEEP)

#include "application/usbstd/usb.h"
#include "application/usb_app/usbmouse.h"

char  mouse[4];
 
unsigned int tick_suspend_interval = 0;
unsigned int tick_suspend_interval2 = 0;
unsigned int tick_suspend_interval3 = 0;
extern unsigned char usb_g_config_value;

void user_init(void)
{
	//1.enable global interrupt
	irq_enable();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
    usb_init();
#if (MCU_CORE_B87 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC1211)
    usbhw_set_eps_en(BIT(USB_EDP_MOUSE)); /* enable endpoint. */
#endif
	//3.enable USB DP pull up 1.5k
	 usb_set_pin(1);

	gpio_set_output_en(LED1,1);
	gpio_set_func(LED1,AS_GPIO);
}

/* enum to USB input device and simulate the left click and right click of mouse */
void main_loop (void)
{
	usb_handle_irq();

	if((usb_g_config_value)&&(usb_has_suspend_irq == 1)){
		if(clock_time_exceed (tick_suspend_interval, 6000))
		{
			reg_wakeup_en &= (~FLD_WAKEUP_SRC_GPIO);
			cpu_sleep_wakeup (SUSPEND_MODE, PM_WAKEUP_CORE, 0);
			tick_suspend_interval = clock_time()|1;
			tick_suspend_interval2 = clock_time()|1;
			tick_suspend_interval3 = clock_time()|1;
		}
	}

	if(clock_time_exceed (tick_suspend_interval2, 500000))
	{
		gpio_write(LED1, 1);
	}
	if(clock_time_exceed (tick_suspend_interval3, 600000))
	{
		gpio_write(LED1, 0);
		tick_suspend_interval2 = clock_time()|1;
		tick_suspend_interval3 = clock_time()|1;
	}
}
#endif

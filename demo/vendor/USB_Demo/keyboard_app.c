/********************************************************************************************************
 * @file	keyboard_app.c
 *
 * @brief	This is the source file for B85m
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
#include "app_config.h"
#if(USB_DEMO_TYPE==USB_KEYBOARD)
#include "application/usb_app/usbkb.h"
#include "application/usbstd/usb.h"
unsigned char  kb_data[6];

// BYTE0: special key(Ctrl/shift ...);
// BYTE1: reserved;
// BYTE2~BYTE7: normal key

void user_init()
{
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
	//initiate LED for indication
	gpio_set_output_en(LED1,1);
	gpio_set_func(LED1,AS_GPIO);
	gpio_set_output_en(LED2,1);
	gpio_set_func(LED2,AS_GPIO);
	gpio_set_output_en(LED3,1);
	gpio_set_func(LED3,AS_GPIO);
	gpio_set_output_en(LED4,1);
	gpio_set_func(LED4,AS_GPIO);
	//initiate Button for keyboard input
	gpio_set_func(GPIO_PD0, AS_GPIO);
	gpio_set_input_en(GPIO_PD0,1);
	gpio_set_output_en(GPIO_PD0,0);
	gpio_setup_up_down_resistor(GPIO_PD0, PM_PIN_PULLUP_10K);

	gpio_set_func(GPIO_PD1, AS_GPIO);
	gpio_set_input_en(GPIO_PD1,1);
	gpio_set_output_en(GPIO_PD1,0);
	gpio_setup_up_down_resistor(GPIO_PD1, PM_PIN_PULLUP_10K);
}


void main_loop (void)
{
	usb_handle_irq();
	if(usb_g_config != 0 )
	{
		if(gpio_read(GPIO_PD0)==0)
		{
			sleep_us(10000);
			if(gpio_read(GPIO_PD0)==0)
			{
				while(gpio_read(GPIO_PD0)==0);
				gpio_write(LED1,ON);
				// normal key: data[0]~data[5]
			    kb_data[0] = 0;
				kb_data[1] = 0;
				kb_data[2] = 0x59;	// number key: 1
				kb_data[3] = 0x5a;  // number key: 2
				kb_data[4] = 0;
				kb_data[5] = 0;

				usbkb_hid_report_normal(0x00,kb_data);

			}
		}

		if(gpio_read(GPIO_PD1)==0)
		{
			sleep_us(10000);
			if(gpio_read(GPIO_PD1)==0)
			{
				while(gpio_read(GPIO_PD1)==0);
				gpio_write(LED1,OFF);
				{
					for(int i=0;i<6;i++)
					{
						kb_data[i]=0;
					}
				}

			     usbkb_hid_report_normal(0x00,kb_data);
			}
		}
	}
}
#endif

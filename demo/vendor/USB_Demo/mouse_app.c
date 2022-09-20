/********************************************************************************************************
 * @file	mouse_app.c
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
#if(USB_DEMO_TYPE==USB_MOUSE)
#include "application/usbstd/usb.h"
#include "application/usb_app/usbmouse.h"

 char  mouse[4];

void user_init(void)
{
	//1.enable global interrupt
	irq_enable();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	 usb_init_interrupt();
	//3.enable USB DP pull up 1.5k
	 usb_set_pin_en();

	//initiate LED for indication
	gpio_set_output_en(LED1,1);
	gpio_set_func(LED1,AS_GPIO);
	gpio_set_output_en(LED2,1);
	gpio_set_func(LED2,AS_GPIO);
	gpio_set_output_en(LED3,1);
	gpio_set_func(LED3,AS_GPIO);
	gpio_set_output_en(LED4,1);
	gpio_set_func(LED4,AS_GPIO);
	//initiate Button for Mouse input

	gpio_set_input_en(GPIO_PD0,1);
	gpio_set_output_en(GPIO_PD0,0);
	gpio_setup_up_down_resistor(GPIO_PD0, PM_PIN_PULLUP_10K);
	gpio_set_func(GPIO_PD0, AS_GPIO);

	gpio_set_input_en(GPIO_PD1,1);
	gpio_set_output_en(GPIO_PD1,0);
	gpio_setup_up_down_resistor(GPIO_PD1, PM_PIN_PULLUP_10K);
	gpio_set_func(GPIO_PD1, AS_GPIO);
	gpio_write(GPIO_PD0|GPIO_PD1,1);

}

/* enum to USB input device and simulate the left click and right click of mouse */
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
				printf("Key:Mouse  Click ! \r\n");
				mouse[0] = BIT(1);// BIT(0) - left key; BIT(1) - right key; BIT(2) - middle key; BIT(3) - side key; BIT(4) - external key
				mouse[1] = -2;	  // Displacement relative to x coordinate
				mouse[2] = 2;	  // Displacement relative to y coordinate
				mouse[3] = 0;     // Displacement relative to the roller
				usbmouse_hid_report(USB_HID_MOUSE,(unsigned char*)mouse,4);
			}
		}

		if(gpio_read(GPIO_PD1)==0)
		{
			sleep_us(10000);
			if(gpio_read(GPIO_PD1)==0)
			{
				while(gpio_read(GPIO_PD1)==0);
				gpio_write(LED1,OFF);
				printf("Key:release \r\n");
				mouse[0] = 0;
				mouse[1] = 0;
				mouse[2] = 0;
				mouse[3] = 0;
				usbmouse_hid_report(USB_HID_MOUSE,(unsigned char*)mouse,4);
			}
		}
	}
}
#endif

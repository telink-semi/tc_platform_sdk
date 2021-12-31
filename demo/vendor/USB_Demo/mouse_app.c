/********************************************************************************************************
 * @file	mouse_app.c
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
	gpio_set_output_en(LED1|LED2|LED3|LED4,1);
	gpio_set_func(LED1|LED2|LED3|LED4,AS_GPIO);
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

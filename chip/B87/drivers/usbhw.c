/********************************************************************************************************
 * @file    usbhw.c
 *
 * @brief   This is the source file for B87
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
//#include "types.h"
//#include "drivers.h"


#include "usbhw.h"
// Endpoint8 is the alias of endpoint0
void usbhw_disable_manual_interrupt(int m) {
	BM_SET(reg_ctrl_ep_irq_mode, m);
}

void usbhw_enable_manual_interrupt(int m) {
	BM_CLR(reg_ctrl_ep_irq_mode, m);
}

void usbhw_write_ep(unsigned int ep, unsigned char * data, int len) {
	//assert(ep < 8);
	reg_usb_ep_ptr(ep) = 0;

	//foreach(i,len)
	for(int i=0;i<len;++i)
	{
		reg_usb_ep_dat(ep) = data[i];
	}
	reg_usb_ep_ctrl(ep) = FLD_EP_DAT_ACK;		// ACK
}

// handy help function
void usbhw_write_ctrl_ep_u16(unsigned short v){
	usbhw_write_ctrl_ep_data(v & 0xff);
	usbhw_write_ctrl_ep_data(v >> 8);
}

unsigned short usbhw_read_ctrl_ep_u16(void){
	unsigned short v = usbhw_read_ctrl_ep_data();
	return (usbhw_read_ctrl_ep_data() << 8) | v;
} 

/**
 * @brief      This function serves to set dp_through_swire function.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 */
void dp_through_swire_en(bool dp_through_swire)
{
    if(dp_through_swire)
    {
        write_reg8(0xb1, (read_reg8(0xb1) | BIT(7))); // BIT(7) = 1 : swire_usb_en
    }
    else
    {
        write_reg8(0xb1, (read_reg8(0xb1) & (~BIT(7)))); // BIT(7) = 0 : swire_usb_dis
    }

}
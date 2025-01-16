/********************************************************************************************************
 * @file    usbhw.c
 *
 * @brief   This is the source file for B80
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

#if (MCU_CORE_B80B)
/**
 * @brief   This function serves to set data endpoint mapping.
 * @param   source_ep - The source endpoint of the mapping.
 * @param   target_ep - The target endpoint of the mapping.
 * @return    none.
 */
void usbhw_set_ep_map(usb_ep_index source_ep, usb_ep_index target_ep)
{
    reg_usb_rdps_map(source_ep) = (source_ep & 1) == 0 ? (reg_usb_rdps_map(source_ep) & (~BIT_RNG(0, 3))) | (target_ep) : (reg_usb_rdps_map(source_ep) & (~BIT_RNG(4, 7))) | ((target_ep) << 4);
}

/**
 * @brief   This function serves to set data endpoint mapping.
 * @param   map_en - mapping enable or disable.
 * @return    none.
 */
void usbhw_ep_map_en(usb_ep_map_sel_e map_en)
{
    if (map_en == EP_MAP_AUTO_EN)
    {
        reg_usb_map |= FLD_USB_EDP_MAP_AUTO_EN | FLD_USB_EDPS_SM_MAP_EN | FLD_USB_EDPS_MAP_TGL_EN | FLD_USB_GET_STA_MAP_EN;
    }
    else
    {
        reg_usb_map &= ~(FLD_USB_EDP_MAP_AUTO_EN | FLD_USB_EDP_MAP_MANUAL_EN);
    }
}
#endif

/**
 * @brief      This function serves to set dp_through_swire function.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 */
void dp_through_swire_en(bool dp_through_swire)
{
#if(MCU_CORE_B80B)
    if(dp_through_swire)
    {
        if(read_reg8(0x7d) == 0xf0) // chip version A0
        {
            write_reg8(0xb1, (read_reg8(0xb1) | BIT(7))); // BIT(7) = 1 : swire_usb_en
        }
        else // chip version A1 and later
        {
            write_reg8(0xb1, (read_reg8(0xb1) & (~BIT(7)))); // BIT(7) = 0 : swire_usb_en
        }
    }
    else
    {
        if(read_reg8(0x7d) == 0xf0) // chip version A0
        {
            write_reg8(0xb1, (read_reg8(0xb1) & (~BIT(7)))); // BIT(7) = 0 : swire_usb_dis
        }
        else // chip version A1 and later
        {
            write_reg8(0xb1, (read_reg8(0xb1) | BIT(7))); // BIT(7) = 1 : swire_usb_dis
        }
    }
#else
    if(dp_through_swire)
    {
        write_reg8(0xb1, (read_reg8(0xb1) | BIT(7))); // BIT(7) = 1 : swire_usb_en
    }
    else
    {
        write_reg8(0xb1, (read_reg8(0xb1) & (~BIT(7)))); // BIT(7) = 0 : swire_usb_dis
    }
#endif
}

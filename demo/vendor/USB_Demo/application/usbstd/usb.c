/********************************************************************************************************
 * @file	usb.c
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
#include "driver.h"
#include "../../usb_default.h"
#define MODULE_USB_ENABLE  1

#ifndef USB_CUSTOM_HID_REPORT_REG_ACCESS
#define	USB_CUSTOM_HID_REPORT_REG_ACCESS		1
#endif

#if (MODULE_USB_ENABLE)//if usb not use

#include "usb.h"
#include "usbdesc.h"
#include "../usbstd/StdRequestType.h"

#if(USB_CDC_ENABLE)
#include "../usb_app/usbcdc.h"
#endif


#if (USB_MOUSE_ENABLE)
#include "../usb_app/usbmouse_i.h"
#endif

#if (USB_KEYBOARD_ENABLE)
#include "../usb_app/usbkb_i.h"
#endif

#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE)
#include "../usb_app/usbaud_i.h"

#endif

#if (USB_CDC_ENABLE)
#include "../usb_app/usbcdc.h"
#endif

unsigned char usb_g_config = 0;
volatile unsigned char usb_g_feature=0;
extern u8 keyboard_interface_number, mouse_interface_number;

u8		host_keyboard_status;
u8		host_cmd[8];
u8		host_cmd_paring_ok = 0;
static USB_Request_Hdr_t control_request;
static u8 * g_response = 0;
static u16 g_response_len = 0;
static int g_stall = 0;
u8 usb_mouse_report_proto = 0; //default 1 for report proto
u8 g_rate = 0; //default 0 for all report
#if(MCU_CORE_B80)
static unsigned short usb_len_idx_0;
static unsigned short usb_len_idx_s;
static unsigned short usb_len_idx_h;
#endif
#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE)
u8 usb_alt_intf[USB_INTF_MAX];

#endif

/**
 * @brief		This function serves to send response to USB host
 * @param[in] 	none
 * @return 		none
 */
void usb_send_response(void) {
	u16 n;
	if (g_response_len < 8) {
		n = g_response_len;
	} else {
		n = 8;
	}
	g_response_len -= n;
#if(MCU_CORE_B80)
	usb_len_idx_0=n;
#endif
	usbhw_reset_ctrl_ep_ptr();
	while (n-- > 0) {
		usbhw_write_ctrl_ep_data(*g_response);
		++g_response;
	}
}



void usb_prepare_desc_data(void) {
	u8 value_l = (control_request.Value) & 0xff;
	u8 value_h = (control_request.Value >> 8) & 0xff;
	g_response = 0;
	g_response_len = 0;

	switch (value_h) {

	case DTYPE_Device:
#if(USB_ID_AND_STRING_CUSTOM)
		g_response = (u8*) (&device_desc_km);
#else
		g_response = usbdesc_get_device();
#endif
		g_response_len = sizeof(USB_Descriptor_Device_t);
		break;

	case DTYPE_Configuration:
#if(USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE)
		g_response = (u8*) (&configuration_km_desc);
		g_response_len = configuration_km_desc[2];  //the third element is the len
#else
		g_response = usbdesc_get_configuration();
		g_response_len = sizeof(USB_Descriptor_Configuration_t);
#endif
		break;

	case DTYPE_String:
#if(USB_ID_AND_STRING_CUSTOM)
		if (USB_STRING_LANGUAGE == value_l) {
			g_response = usbdesc_get_language();
			g_response_len = sizeof(LANGUAGE_ID_ENG);
		} else if (USB_STRING_VENDOR == value_l) {
			g_response = (u8*) (&vendor_desc_km);
			g_response_len = vendor_desc_km.Size;
		} else if (USB_STRING_PRODUCT == value_l) {
			g_response = (u8*) (&prodct_desc_km);
			g_response_len = prodct_desc_km.Size;
		} else if (USB_STRING_SERIAL == value_l) {
			g_response = (u8*) (&serial_desc_km);
			g_response_len = serial_desc_km.Size;
#else
		if (USB_STRING_LANGUAGE == value_l) {
			g_response = usbdesc_get_language();
			g_response_len = sizeof(LANGUAGE_ID_ENG);
		} else if (USB_STRING_VENDOR == value_l) {
			g_response = usbdesc_get_vendor();
			g_response_len = sizeof(STRING_VENDOR);
		} else if (USB_STRING_PRODUCT == value_l) {
			g_response = usbdesc_get_product();
			g_response_len = sizeof(STRING_PRODUCT);
		} else if (USB_STRING_SERIAL == value_l) {
			g_response = usbdesc_get_serial();
			g_response_len = sizeof(STRING_SERIAL);
#endif

#if (MS_OS_DESCRIPTOR_ENABLE)
		} else if (USB_STRING_MS_OS == value_l) {
			g_response = usbdesc_get_OS_descriptor();
			g_response_len = sizeof(STRING_MSFT);
#endif

		}
		else {
			g_stall = 1;
		}
		break;

	default:
		g_stall = 1;
		break;

	}
#if(MCU_CORE_B80)	
	usb_len_idx_s = g_response_len;
#endif
	if (control_request.Length < g_response_len) {
		g_response_len = control_request.Length;
	}

	return;
}

//standard interface request handle

void usb_handle_std_intf_req() {
	u8 value_h = (control_request.Value >> 8) & 0xff;
#if( USB_MOUSE_ENABLE || USB_KEYBOARD_ENABLE || USB_SOMATIC_ENABLE)
	unsigned char index_l = (control_request.Index) & 0xff;
#endif
	switch (value_h) {
	case HID_DTYPE_HID:// HID Descriptor
#if(0)
		if (index_l == USB_INTF_AUDIO_HID) {
			//audio hid
			g_response = usbdesc_get_audio();
			g_response_len = sizeof(USB_HID_Descriptor_HID_Audio_t);
		}
#endif
  #if(USB_MOUSE_ENABLE)
   #if(USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE)
		if (index_l == mouse_interface_number)
		{
			g_response = (u8*) (&configuration_desc_mouse[9]);
			g_response_len = USB_HID_DESCRIPTOR_LENGTH;
		}
  #else
		if (index_l == USB_INTF_MOUSE) //index_l is the interface number
		{
			//mouse
			g_response = usbdesc_get_mouse();
			g_response_len = sizeof(USB_HID_Descriptor_HID_Mouse_t);
		}
  #endif
#endif
#if(USB_KEYBOARD_ENABLE)
#if(USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE)
		if (index_l == keyboard_interface_number)
		{
			g_response = (u8*) (&configuration_desc_keyboard[9]);
			g_response_len = USB_HID_DESCRIPTOR_LENGTH;
		}
#else
		if (index_l == USB_INTF_KEYBOARD) {
			//keyboard
			g_response = usbdesc_get_keyboard();
			g_response_len = sizeof(USB_HID_Descriptor_HID_Keyboard_t);
		}
#endif
#endif
#if(USB_SOMATIC_ENABLE )
		if (index_l == USB_INTF_SOMATIC) //index_l is the interface number
		{
			//SOMATIC
			g_response = usbdesc_get_somatic();
			g_response_len = sizeof(USB_HID_Descriptor_HID_Somatic_t);
		}
#endif
		break;
	case HID_DTYPE_Report://Report Descriptor
	 if(0)
   {
   }
#if (0)
		if (index_l == USB_INTF_AUDIO_HID) {
			//audio hid
			g_response = usbaud_get_report_desc();
			g_response_len = usbaud_get_report_desc_size();
		}
#endif
#if(USB_KEYBOARD_ENABLE)
	else if (index_l == (USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE ? keyboard_interface_number : USB_INTF_KEYBOARD)) {
			//keyboard
			g_response = (u8*) usbkb_get_report_desc();
			g_response_len = usbkb_get_report_desc_size();
		}
#endif
#if(USB_MOUSE_ENABLE)
	else if (index_l == (USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE ? mouse_interface_number : USB_INTF_MOUSE)) {
			//mouse
			g_response = (u8*) usbmouse_get_report_desc();
			g_response_len = usbmouse_get_report_desc_size();
		}
#endif
#if(USB_SOMATIC_ENABLE)
		else if (index_l == USB_INTF_SOMATIC) {
			//somatic sensor
			g_response = (u8*) usbsomatic_get_report_desc();
			g_response_len = usbsomatic_get_report_desc_size();
		}
#endif
		else{
			g_stall = 1;
		}
		break;
	case 0x23:// Phisical Descriptor
		// TODO
		break;

	default:// other condition
		break;
	}

	if (control_request.Length < g_response_len) {
		g_response_len = control_request.Length;
	}

	return;
}

u32			custom_read_dat;
u32			custom_reg_cmd;


void usb_handle_out_class_intf_req(int data_request) {
	u8 property = control_request.Request;
	u8 value_l = (control_request.Value) & 0xff;
	u8 value_h = (control_request.Value >> 8) & 0xff;
#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
	u8 Entity = (control_request.Index >> 8) & 0xff;
#endif

	switch (property) {
    #if(USB_CDC_ENABLE)//for cdc
	   case CDC_REQ_SetControlLine_State:

		break;
		case CDC_REQ_SetLine_Encoding:			// set serial parameter:baudrate,check,stop and data

                  if(USB_IRQ_DATA_REQ==data_request)
                  {
					usbhw_reset_ctrl_ep_ptr();
					for(int i = 0; i < 7; ++i)
					{
					  LineCoding[i] = usbhw_read_ctrl_ep_data();
					}
					//-todo- according to LineCoding from USB HOST, user can set baud rate, stop bits, parity, data bit, etc.
                  }
				 break;



    #endif
	case HID_REQ_SetReport:
		switch (value_h) {
		case HID_REPORT_ITEM_In:
			break;
		case HID_REPORT_ITEM_Out:
			// usb_hid_set_report_ouput();
			break;
		case HID_REPORT_ITEM_Feature:
			if (data_request) {
				host_keyboard_status = usbhw_read_ctrl_ep_data();
			}
#if(USB_SET_REPORT_FEATURE_SUPPORT)
		{
			usb_set_report_t rpt;
			rpt.report_id = value_l;
			rpt.len = control_request.Index;
			ev_emit_event_syn(EV_USB_SET_REPORT, (void*)(&rpt));	// send in report id
		}
#endif
			break;
		case HID_REPORT_CUSTOM:
#if (USB_CUSTOM_HID_REPORT)
		{	//Paring, EMI-TX, EMI-RX
			if (data_request) {
				int i=0;
				usbhw_reset_ctrl_ep_ptr (); //address
				for(i=0;i<8;i++) {
					host_cmd[i] = usbhw_read_ctrl_ep_data();
				}
#if (USB_CUSTOM_HID_REPORT_REG_ACCESS)
				custom_reg_cmd = (host_cmd[1] & 0xf0) == 0xc0;
				if (custom_reg_cmd) {
					host_cmd[0] = 0;
					int adr = *((u16 *)(host_cmd + 2));
					int len = host_cmd[1] & 3;
					if (host_cmd[1] == 0xcc && adr == 0x5af0) { //re-enumerate device
						usb_dp_pullup_en (0);			//disable device
						sleep_us (300000);
						reg_ctrl_ep_irq_mode = 0xff;	//hardware mode
						usb_dp_pullup_en (1);			//enable device
					}
					else {
						adr += 0x800000;
					}

					if ((host_cmd[1] & 0x0c)==0) {	//write core register
						if (len == 0) {
							for (int k=0; k<4; k++) {
								custom_read_dat = (custom_read_dat >> 8) | (read_reg8 (adr++) << 24);
							}
						}
						else if (len == 1) {
							write_reg8 (adr, host_cmd[4]);
						}
						else if (len == 2) {
							write_reg16 (adr, *((u16 *)(host_cmd + 4)));
						}
						else {
							write_reg32 (adr, *((u32 *)(host_cmd + 4)));
						}
					}
					else {	//read core register
						if (len == 0) {
							custom_read_dat = analog_read (host_cmd[2]);
						}
						else {
							analog_write (host_cmd[2], host_cmd[4]);
						}
					}
				}
#endif
			}
			break;
		}
#endif
		default:
			g_stall = 1;
			break;
		}
		break;

	case HID_REQ_SetIdle:
		if (data_request) {
			g_rate = usbhw_read_ctrl_ep_data();
		}
		g_rate = value_h;
		break;

	case HID_REQ_SetProtocol:
		if (data_request) {
			usb_mouse_report_proto = usbhw_read_ctrl_ep_data();
		}
		usb_mouse_report_proto = value_l;
		reg_usb_ep_ctrl(USB_EDP_MOUSE) = 0;
		break;
#if (0)
	case CDC_REQ_SetLine_Encoding:
	case CDC_REQ_SetControlLine_State:
		CDC_Device_ProcessControlRequest(control_request.Request, control_request.Value,
			control_request.Index, control_request.Length);
		break;
#endif
	default:
		g_stall = 1;
		break;
	}




#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
	if(0 == g_stall){		// already handled
		return;
	}
	g_stall = 0;
	switch(Entity){
		case USB_SPEAKER_FEATURE_UNIT_ID:
			usbaud_handle_set_speaker_cmd(value_h);
			break;
		case USB_MIC_FEATURE_UNIT_ID:
			usbaud_handle_set_mic_cmd(value_h);
			break;
		default:
			g_stall = 1;
			break;
	}
#endif
}



void usb_handle_in_class_intf_req() {
	u8 property = control_request.Request;
#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
	u8 value_h = (control_request.Value >> 8);
	u8 Entity = (control_request.Index >> 8);
#endif
	switch (property) {
		case 0x00:
			usbhw_write_ctrl_ep_data(0x00);
			break;

#if(USB_CDC_ENABLE)
//#if(1)
			case CDC_REQ_GetLine_Encoding:

				usbhw_write_ctrl_ep_data(LineCoding[0]);
				usbhw_write_ctrl_ep_data(LineCoding[1]);
				usbhw_write_ctrl_ep_data(LineCoding[2]);
				usbhw_write_ctrl_ep_data(LineCoding[3]);
				usbhw_write_ctrl_ep_data(LineCoding[4]);
				usbhw_write_ctrl_ep_data(LineCoding[5]);
				usbhw_write_ctrl_ep_data(LineCoding[6]);
			break;

			case CDC_NOTIF_Serial_State:

			break;
#endif
		case HID_REQ_GetReport:
#if(USB_SOMATIC_ENABLE)
			if(usbsomatic_hid_report_type((control_request.Value & 0xff))){
			}
			else
#elif (USB_CUSTOM_HID_REPORT)
			if( control_request.Value==0x0305 ) {
				if (USB_CUSTOM_HID_REPORT_REG_ACCESS && custom_reg_cmd) {
					usbhw_write_ctrl_ep_data (custom_read_dat);
					usbhw_write_ctrl_ep_data (custom_read_dat>>8);
					usbhw_write_ctrl_ep_data (custom_read_dat>>16);
					usbhw_write_ctrl_ep_data (custom_read_dat>>24);
					usbhw_write_ctrl_ep_data (0x10);
					usbhw_write_ctrl_ep_data (0x20);
					usbhw_write_ctrl_ep_data (0x40);
					usbhw_write_ctrl_ep_data (0x80);
				}
				else {
					usbhw_write_ctrl_ep_data (0x04);
					usbhw_write_ctrl_ep_data (0x58);
					usbhw_write_ctrl_ep_data (0x00);
					usbhw_write_ctrl_ep_data (host_cmd_paring_ok ? 0xa1 : 0x00);  //For binding OK
					usbhw_write_ctrl_ep_data (0x00);
					usbhw_write_ctrl_ep_data (0x00);
					usbhw_write_ctrl_ep_data (0x08);
					usbhw_write_ctrl_ep_data (0x00);
				}
			}
			else
#endif
			break;
		case HID_REQ_GetIdle:
			usbhw_write_ctrl_ep_data(g_rate);
			break;
		case HID_REQ_GetProtocol:
			usbhw_write_ctrl_ep_data(usb_mouse_report_proto);
			break;


		default:
			g_stall = 1;
			break;
	}
#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
	if(0 == g_stall){		// already handled
		return;
	}
	g_stall = 0;
	switch(Entity){
		case USB_SPEAKER_FEATURE_UNIT_ID:
			if(usbaud_handle_get_speaker_cmd(property, value_h)){
				g_stall = 1;
			}
			break;
		case USB_MIC_FEATURE_UNIT_ID:
			if(usbaud_handle_get_mic_cmd(property, value_h)){
				g_stall = 1;
			}
			break;
		default:
			g_stall = 1;
			break;
	}
#endif

}


void usb_handle_in_class_endp_req() {
#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
	unsigned char property = control_request.Request;
	unsigned char ep_ctrl = control_request.Value >> 8;
	//unsigned char addr = (control_request.Index >> 8);

	if(ep_ctrl == AUDIO_EPCONTROL_SamplingFreq){
		switch(property){
		case AUDIO_REQ_GetCurrent:
			usbhw_write_ctrl_ep_data(MIC_SAMPLE_RATE & 0xff);
			usbhw_write_ctrl_ep_data(MIC_SAMPLE_RATE >> 8);
			usbhw_write_ctrl_ep_data(MIC_SAMPLE_RATE >> 16);
			break;
		default:
			break;
		}
	}
#endif
}

void usb_handle_out_class_endp_req() {
	return;
#if 0
	u8 property = control_request.Request;
	u8 ep_ctrl = control_request.Value & 0xff;
#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
	u8 addr = (control_request.Index >> 8);
#endif
#endif
}


void usb_handle_set_intf() {
#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE)
	u8 value_l = (control_request.Value) & 0xff;
	u8 intf_index = (control_request.Index) & 0x07;

	usb_alt_intf[intf_index] = value_l;

#if (USB_MIC_ENABLE)
	if(USB_INTF_MIC == intf_index && value_l){
//		usbhw_reset_ep_ptr(USB_EDP_MIC);
//		reg_usb_ep_ptr(USB_EDP_MIC) = USB_MIC_CHANNELS_LEN;
//		reg_usb_ep_ctrl(USB_EDP_MIC) = (MIC_CHANNLE_COUNT == 2 ? 0x81 : 0xc1);
		reg_usb_ep_ptr(USB_EDP_MIC) = 0;
		reg_usb_ep_ctrl(USB_EDP_MIC) = BIT(0);		//ACK first packet
	}
#endif

#if (USB_SPEAKER_ENABLE)
	if(USB_INTF_SPEAKER == intf_index && value_l){
//		usbhw_reset_ep_ptr(USB_EDP_MIC);
//		reg_usb_ep_ptr(USB_EDP_MIC) = USB_MIC_CHANNELS_LEN;
//		reg_usb_ep_ctrl(USB_EDP_MIC) = (MIC_CHANNLE_COUNT == 2 ? 0x81 : 0xc1);
		reg_usb_ep_ptr(USB_EDP_SPEAKER) = 0;
		reg_usb_ep_ctrl(USB_EDP_SPEAKER) = BIT(0);		//ACK first packet
	}
#endif
#endif
	return;
}

#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE)
void usb_handle_get_intf() {
	u8 intf_index = (control_request.Index) & 0x07;

	usbhw_write_ctrl_ep_data(usb_alt_intf[intf_index]);

	return;
}
#endif


void usb_handle_request(u8 data_request) {
	u8 RequestType = control_request.RequestType;
	u8 Request = control_request.Request;

	usbhw_reset_ctrl_ep_ptr();
	switch (RequestType) {
	case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE)://0x80
		if (REQ_GetDescriptor == Request) {
			if (USB_IRQ_SETUP_REQ == data_request) {
				usb_prepare_desc_data();
			}
			usb_send_response();
		}
		break;

	case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_INTERFACE)://0x81
		if (REQ_GetDescriptor == Request) {
			if (USB_IRQ_SETUP_REQ == data_request) {
				usb_handle_std_intf_req();
			}
			usb_send_response();
		}
		#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE)
		else if (REQ_GetInterface == Request) {
			usb_handle_get_intf();
		}
		#endif
		break;

	#if (MS_OS_DESCRIPTOR_ENABLE)
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE):
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_INTERFACE):
		 if ((Request ==  MS_VENDORCODE)) {//Retrieve an OS Feature Descriptor
			unsigned char index_l = control_request.Index&0xff;
			if (USB_IRQ_SETUP_REQ == data_request) {
				//usb_indexl==0x04 for Extended compat ID
				//usb_indexl==0x05 for Extended properties
				if(index_l==0x04 )
				{
					g_response = usbdesc_get_compatID(&g_response_len);
				}
				else if(index_l==0x05)
				{
					g_response = usbdesc_get_OSFeature(&g_response_len);
				}
				else
					g_stall = 1;
				if (control_request.Length < g_response_len) {
					g_response_len = control_request.Length;
				}
			}

			usb_send_response();
		}
		break;
	#endif

	case (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)://21
		usb_handle_out_class_intf_req(data_request);
		break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_ENDPOINT)://
		usb_handle_out_class_endp_req();
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)://a1
		usb_handle_in_class_intf_req();
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_ENDPOINT)://a2,
		usb_handle_in_class_endp_req();
		break;

	case (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_INTERFACE)://01
		if (REQ_SetInterface == Request) {
			usb_handle_set_intf();
		}
		else if(REQ_SetFeature == Request) {
			g_stall = 1;
		    usb_g_feature = 1;
				}
		break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE)://00
		if(control_request.Value&0xff)
		{
			usb_g_config=1;
		}

		break;
	default:
		g_stall = 1;
		break;
	}

	return;
}


void usb_handle_ctl_ep_setup(void)
{
#if(MCU_CORE_B80)
	reg_usb_sups_cyc_cali=0x38;
#endif
	usbhw_reset_ctrl_ep_ptr();
	control_request.RequestType = usbhw_read_ctrl_ep_data();
	control_request.Request = usbhw_read_ctrl_ep_data();
	control_request.Value = usbhw_read_ctrl_ep_u16();
	control_request.Index = usbhw_read_ctrl_ep_u16();
	control_request.Length = usbhw_read_ctrl_ep_u16();
#if(MCU_CORE_B80)
	usb_len_idx_h = control_request.Length;
#endif
	g_stall = 0;
	usb_handle_request(USB_IRQ_SETUP_REQ);
	if (g_stall)
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	}
	else
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
	}
}


void usb_handle_ctl_ep_data(void) {
#if(MCU_CORE_B80)
	reg_usb_sups_cyc_cali=0x38;
#endif
	usbhw_reset_ctrl_ep_ptr();
	g_stall = 0;
	usb_handle_request(USB_IRQ_DATA_REQ);
	if (g_stall)
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	}
#if(MCU_CORE_B80)
	else if((usb_len_idx_s % 8 == 0) && (usb_len_idx_0 == 0) && (usb_len_idx_s != usb_len_idx_h))
	{
		reg_usb_sups_cyc_cali=0x18;
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
	}
#endif
	else
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
	}

}



void usb_handle_ctl_ep_status() {
#if(MCU_CORE_B80)
	reg_usb_sups_cyc_cali=0x38;
#endif
	if (g_stall)
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_STALL);
	}
	else
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_ACK);
	}

}

u8 usb_has_suspend_irq = 0;
u8  usb_just_wakeup_from_suspend = 1;
extern u8 rf_channel;
int usb_suspend_check(void){
	return 0;
}

#if(0)
void usb_resume_host(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_3520)
#else
	reg_wakeup_en = FLD_WAKEUP_SRC_USB_RESM;
	reg_wakeup_en = 0;
#endif
	sleep_us(6000);
}
#endif
u8 edp_toggle[8];


void usb_handle_irq(void) {
	u32 irq = usbhw_get_ctrl_ep_irq();
	if (irq & FLD_CTRL_EP_IRQ_SETUP) {
		usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_SETUP);
		usb_handle_ctl_ep_setup();
	}
	if (irq & FLD_CTRL_EP_IRQ_DATA) {
		usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_DATA);
		usb_handle_ctl_ep_data();
	}
	if(usb_g_feature==1){ sleep_us(1000);usb_g_feature=0;}//for Chapter 8 test
	if (irq & FLD_CTRL_EP_IRQ_STA) {
		usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_STA);
		usb_handle_ctl_ep_status();
	}

#if(USB_CDC_ENABLE)
	usb_cdc_irq_data_process();
#endif

#if(USB_MIC_ENABLE||USB_SPEAKER_ENABLE)
	usb_audio_irq_data_process();
#endif


#if(USB_SOMATIC_ENABLE)
	if(irq & BIT((USB_EDP_SOMATIC_OUT & 0x07))){
		reg_usb_irq = BIT((USB_EDP_SOMATIC_OUT & 0x07));		// clear ime
		usbhw_reset_ep_ptr(USB_EDP_SOMATIC_OUT);

		ev_emit_event_syn(EV_USB_OUT_DATA, (void*)irq);

		usbhw_data_ep_ack(USB_EDP_SOMATIC_OUT);
	}
#endif

	if (reg_irq_src & FLD_IRQ_USB_RST_EN)
	{		//USB reset
			usb_mouse_report_proto = 1;                   //1: report protocol; 0: start protocol
			reg_irq_src |= FLD_IRQ_USB_RST_EN;					//Clear USB reset flag
			for (int i=0; i<8; i++) {
				reg_usb_ep_ctrl(i) = 0;
				edp_toggle[i]=0;
	}

    #if (USB_CDC_ENABLE)
    //must add ,if endpoint is reset and ack is not set,CDC out_irq will not be generated.
    //The packet capture phenomenon of the USB analyzer is: device does not return ack.(kaixin modify,2020-01-15)
    usbhw_data_ep_ack(USB_EDP_CDC_OUT);
    #endif
    }

   if(IRQ_USB_PWDN_ENABLE && (reg_irq_src & FLD_IRQ_USB_PWDN_EN)){
		usb_has_suspend_irq = 1;
	}else{
		usb_has_suspend_irq = 0;
	}

#if (!USB_DESCRIPTER_CONFIGURATION_FOR_KM_DONGLE)
	if ((reg_irq_src & FLD_IRQ_USB_PWDN_EN))
	{
		return;
	}

#endif



}

void usb_init_interrupt(void)
{
	usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC|FLD_CTRL_EP_AUTO_CFG);
#if(MCU_CORE_B87||MCU_CORE_B80)
	usbhw_set_eps_en(FLD_USB_EDP8_EN|FLD_USB_EDP1_EN|FLD_USB_EDP2_EN|FLD_USB_EDP3_EN|FLD_USB_EDP4_EN|FLD_USB_EDP5_EN|FLD_USB_EDP6_EN|FLD_USB_EDP7_EN);
#endif

}

void usb_init() {

	usb_init_interrupt();
#if FLOW_NO_OS
#else
	usb_handle_irq();
#endif
}


#if(USB_CDC_ENABLE)

void usb_cdc_irq_data_process(void)
{
	unsigned char irq = usbhw_get_eps_irq();

	if(irq & FLD_USB_EDP4_IRQ ){
		usbhw_clr_eps_irq(FLD_USB_EDP4_IRQ);
		usbhw_reset_ep_ptr(USB_EDP_CDC_IN);
		usb_cdc_tx_cnt++;
	}
	//CDC host-to-device data Output IRQ
	if(irq & FLD_USB_EDP5_IRQ){
		reg_usb_irq=FLD_USB_EDP5_IRQ;
		g_stall = 0;
		//receive data from usb host
		usb_cdc_rx_data_from_host(usb_cdc_data);
		if(g_stall)
		{
			usbhw_data_ep_stall(USB_EDP_CDC_OUT);
		}
		else
		{
			usbhw_data_ep_ack(USB_EDP_CDC_OUT);

		}
	}
}
#endif

#endif

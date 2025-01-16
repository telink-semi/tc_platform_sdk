/********************************************************************************************************
 * @file    spk_app.c
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
#if (!MCU_CORE_B80 && !MCU_CORE_B80B  && !MCU_CORE_TC1211)
#include "app_config.h"
#if(USB_DEMO_TYPE==USB_SPEAKER)
#include "application/usb_app/usbaud.h"
#include "application/usbstd/usb.h"
extern unsigned char usb_audio_speaker_cnt;
#define	MIC_BUFFER_SIZE			4096
volatile signed short MicBuf[MIC_BUFFER_SIZE>>1];

#define AUDIO_RATE_VAL			AUDIO_16K

void user_init(void)
{
#if(AUDIO_SPK_MODE == AUDIO_USB_TO_SDM)
    usb_init();
#if (MCU_CORE_B87)
    usbhw_set_eps_en(BIT(USB_EDP_SPEAKER)); /* enable endpoint. */
#endif
	//enable USB DP pull up 1.5k
	usb_set_pin(1);
	//3.enable global interrupt
	irq_enable();

	reg_usb_ep6_buf_addr =0x80;//ep7 192bytes
	reg_usb_ep7_buf_addr =0xc0;//ep7 64bytes
	reg_usb_ep_max_size  =192>>3;
	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_usb_init(AUDIO_RATE_VAL);
#if(MCU_CORE_B85)
	audio_set_sdm_output(USB_IN,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,USB_IN,AUDIO_RATE_VAL,1);
#endif
	audio_set_usb_output();

#endif

}


void main_loop (void)
{
#if (AUDIO_SPK_MODE == AUDIO_USB_TO_SDM)
	usb_handle_irq();

	if(usb_audio_speaker_cnt)
	{
		audio_rx_data_from_usb();
		usb_audio_speaker_cnt=0;
	}

#endif
}
#endif
#endif


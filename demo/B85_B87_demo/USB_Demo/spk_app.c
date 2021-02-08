/********************************************************************************************************
 * @file	spk_app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
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
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
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



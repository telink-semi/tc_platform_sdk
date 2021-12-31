/********************************************************************************************************
 * @file	mic_app.c
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
#if (!MCU_CORE_B80)
#include "app_config.h"
#if(USB_DEMO_TYPE==USB_MICROPHONE)
#include "application/usb_app/usbaud.h"
#include "application/usbstd/usb.h"
#define AUDIO_AMIC_TO_USB		1
#define AUDIO_DMIC_TO_USB		2
#define AUDIO_CODEC_TO_USB		3

#define AUDIO_MIC_MODE				AUDIO_CODEC_TO_USB

extern unsigned char usb_audio_mic_cnt;
#define	MIC_BUFFER_SIZE			4096
volatile signed short MicBuf[MIC_BUFFER_SIZE>>1];
#define AMIC_BIAS_PIN			     GPIO_PC4 //need check the evk board
void user_init(void)
{
	 gpio_set_output_en(LED1|LED2|LED3|LED4,1);
	 gpio_set_func(LED1|LED2|LED3|LED4,AS_GPIO);
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
	//3.enable global interrupt
	irq_enable();

	reg_usb_ep6_buf_addr =0x80;
	reg_usb_ep7_buf_addr =0xc0;
	reg_usb_ep_max_size  =(192 >> 3);

	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);

#if (AUDIO_MIC_MODE==AUDIO_AMIC_TO_USB)
	/*when test audio performance  we need  disable BIAS pin*/
	gpio_set_func(AMIC_BIAS_PIN, AS_GPIO);
	gpio_set_output_en(AMIC_BIAS_PIN, 1); 		//enable output
	gpio_set_input_en(AMIC_BIAS_PIN ,0);			//disable input
	gpio_write(AMIC_BIAS_PIN, 1);              	//BIAS OUTPUT 1

	audio_amic_init(AUDIO_8K);
#elif(AUDIO_MIC_MODE==AUDIO_DMIC_TO_USB)
	gpio_set_func(GPIO_PA0, AS_DMIC);
	gpio_set_func(GPIO_PA1, AS_DMIC);
	gpio_set_input_en(GPIO_PA0, 1);

	audio_dmic_init(AUDIO_32K);

#elif (AUDIO_MIC_MODE == AUDIO_CODEC_TO_USB)
#if(MCU_CORE_B85)
	audio_set_codec(I2C_GPIO_GROUP_A3A4, CODEC_MODE_LINE_TO_HEADPHONE_LINEOUT_I2S,CLOCK_SYS_CLOCK_HZ);
#elif(MCU_CORE_B85)
	audio_set_codec(I2C_GPIO_SDA_A3, I2C_GPIO_SCL_A4,CODEC_MODE_LINE_TO_HEADPHONE_LINEOUT_I2S,CLOCK_SYS_CLOCK_HZ);
#endif
	audio_i2s_init();
#endif
	audio_set_usb_output();
}

void main_loop (void)
{
	usb_handle_irq();

	if(usb_audio_mic_cnt)
	{
#if(AUDIO_MIC_MODE==AUDIO_AMIC_TO_USB)
		audio_tx_data_to_usb(AMIC, AUDIO_16K);
#elif(AUDIO_MIC_MODE==AUDIO_DMIC_TO_USB)
		audio_tx_data_to_usb(DMIC, AUDIO_16K);
#elif(AUDIO_MIC_MODE==AUDIO_CODEC_TO_USB)
		audio_tx_data_to_usb(I2S_IN, AUDIO_16K);
#endif
		usb_audio_mic_cnt=0;
	}

}
#endif
#endif


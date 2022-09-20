/********************************************************************************************************
 * @file	mic_app.c
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
	gpio_set_output_en(LED1,1);
	gpio_set_func(LED1,AS_GPIO);
	gpio_set_output_en(LED2,1);
	gpio_set_func(LED2,AS_GPIO);
	gpio_set_output_en(LED3,1);
	gpio_set_func(LED3,AS_GPIO);
	gpio_set_output_en(LED4,1);
	gpio_set_func(LED4,AS_GPIO);
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


/********************************************************************************************************
 * @file    usbaud.h
 *
 * @brief   This is the header file for B85m
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
#if (!MCU_CORE_B80 && !MCU_CORE_B80B)
#pragma once
#include "driver.h"
#include "../usbstd/HIDClassCommon.h"
#include "../usbstd/HIDReportData.h"
#include "types.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

// telink usb report ctrl command. used mixed with USB_REPORT_NO_EVENT
enum {
	USB_AUD_PLAY_PAUSE		= 0,
	USB_AUD_NEXT_TRACK		= 1,
	USB_AUD_PREV_TRACK		= 2,
	USB_AUD_STOP			= 3,
	USB_AUD_PLAY			= 4,
	USB_AUD_PAUSE			= 5,
	USB_AUD_FAST_FWD		= 6,
	USB_AUD_REWIND			= 7,
	USB_AUD_VOL_INC			= 0x10,
	USB_AUD_VOL_DEC			= 0x11,
	USB_AUD_VOL_MUTE 		= 0x12,
};

#define AUDIO_FEATURE_ID_SPEAKER	0x02
#define AUDIO_FEATURE_ID_MIC		0x05

typedef struct{
	s16 speaker_vol;
	s16 mic_vol;
	s8 speaker_mute;
	s8 mic_mute;
	s8 change;
}usb_audio_status_t;


typedef struct {
	u16 vol_cur;
	u16	vol_step;
	u8 	mute;
}speaker_setting_t;


typedef struct {
	u16 vol_cur;
	u16	vol_step;
	u8 	mute;
}mic_setting_t;

#define AUDIO_VOLUME_STEP_MAX  11

#define MIC_VOL_MIN			((s16) 0x0000)     /* Volume Minimum Value */
#define MIC_VOL_MAX			((s16) 0x1e00)     /* Volume Maximum Value */
#define	MIC_VOL_RES			0x0180     /* Volume Resolution */
#define MIC_VOL_DEF			0x1800     /* Volume default */
#define	MIC_MAX_STEP		(MIC_VOL_MAX / MIC_VOL_RES)

#define	SPEAKER_VOL_MIN		((s16) 0xa000)     /* Volume Minimum Value */
#define	SPEAKER_VOL_MAX		((s16) 0x0300)     /* Volume Maximum Value */
#define SPEAKER_VOL_RES		0x0180     /* Volume Resolution */
#define SPEAKER_VOL_DEF		0x8000     /* Volume default */
#define SPEAKER_VOL_STEP	400

extern int usbaud_handle_set_speaker_cmd(int type);
extern int usbaud_handle_set_mic_cmd(int type);
int usbaud_handle_get_speaker_cmd(int req, int type);
int usbaud_handle_get_mic_cmd(int req, int type);
void usbaud_init(void);
u8 usbaud_speaker_vol_get(void);
u8 usbaud_mic_vol_get(void);
void usbaud_mic_en(int en);

/**
 * @brief     This function serves to send data to USB.
 * @param[in] Input_Type - audio input type.
 * @param[in] Audio_Rate - audio rate. This value is matched with usb_desc.c :USB_RATE.
 * @return    none.
 */
void audio_tx_data_to_usb(AudioInput_Typedef Input_Type,AudioRate_Typedef Audio_Rate);
void audio_rx_data_from_usb(void);
/**
 * @brief		This function serves to send data to USB Host or receive data from USB Host
 * @param[in] 	none
 * @return 		none
 */
void usb_audio_irq_data_process(void);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

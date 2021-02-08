/********************************************************************************************************
 * @file	app.c
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
#include "compiler.h"


#define FLASH_ADDR				0x00d000
#define FLASH_SECURITY_ADDR		0x001000
#define FLASH_BUFF_LEN			256
#define FLASH_OTP_LOCK			0

volatile unsigned char Flash_Read_Buff[FLASH_BUFF_LEN]={0};
volatile unsigned char Flash_Write_Buff[FLASH_BUFF_LEN]=
{		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
		0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
		0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
		0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
		0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,


		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
		0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
		0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
		0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
		0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
		0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
		0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,

		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
		0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
		0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
		0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,

		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
		0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
		0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
		0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
		0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
		0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,
		0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
};

typedef struct{
	unsigned char erase_err:1;
	unsigned char write_err:1;
	unsigned char lock_err:1;
	unsigned char unlock_err:1;
	unsigned char otp_erase_err:1;
	unsigned char otp_write_err:1;
	unsigned char otp_lock_err:1;
	unsigned char check_umid:1;
}err_status_t;
volatile err_status_t err_status;
volatile unsigned short status1=0;
volatile unsigned short status2=0;
volatile unsigned short status3=0;
volatile unsigned short status4=0;
volatile unsigned short status5=0;
volatile unsigned int  mid=0;
unsigned char uid[16]={0};

#if (MCU_CORE_B85)
_attribute_ram_code_ void flash_mid1060c8_test(void)
{
	int i;

	status1 = flash_read_status_mid1060c8();
	flash_lock_mid1060c8(FLASH_LOCK_LOW_56K_MID1060C8);
	status2 = flash_read_status_mid1060c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid1060c8();
	status3 = flash_read_status_mid1060c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
}

_attribute_ram_code_ void flash_mid13325e_test(void)
{
	int i;

	status1 = flash_read_status_mid13325e();
	flash_lock_mid13325e(FLASH_LOCK_LOW_256K_MID13325E);
	status2 = flash_read_status_mid13325e();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid13325e();
	status3 = flash_read_status_mid13325e();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
}

_attribute_ram_code_ void flash_mid134051_test(void)
{
	int i;

	status1 = flash_read_status_mid134051();
	flash_lock_mid134051(FLASH_LOCK_LOW_256K_MID134051);
	status2 = flash_read_status_mid134051();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid134051();
	status3 = flash_read_status_mid134051();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
}

_attribute_ram_code_ void flash_mid136085_test(void)
{
	int i;

	status1 = flash_read_status_mid136085();
	flash_lock_mid136085(FLASH_LOCK_LOW_64K_MID136085);
	status2 = flash_read_status_mid136085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid136085();
	status3 = flash_read_status_mid136085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}

	flash_erase_otp_mid136085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid136085(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}

	flash_write_otp_mid136085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid136085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid136085();
	flash_lock_otp_mid136085(FLASH_LOCK_OTP_0x001000_512K_MID136085);
	status5 = flash_read_status_mid136085();
	flash_erase_otp_mid136085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid136085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
#endif
}

_attribute_ram_code_ void flash_mid1360c8_test(void)
{
	int i;

	status1 = flash_read_status_mid1360c8();
	flash_lock_mid1360c8(FLASH_LOCK_LOW_256K_MID1360C8);
	status2 = flash_read_status_mid1360c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid1360c8();
	status3 = flash_read_status_mid1360c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
}

_attribute_ram_code_ void flash_mid1360eb_test(void)
{
	int i;

	status1 = flash_read_status_mid1360eb();
	flash_lock_mid1360eb(FLASH_LOCK_LOW_64K_MID1360EB);
	status2 = flash_read_status_mid1360eb();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid1360eb();
	status3 = flash_read_status_mid1360eb();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}

	flash_erase_otp_mid1360eb(FLASH_SECURITY_ADDR);
	flash_read_otp_mid1360eb(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}

	flash_write_otp_mid1360eb(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid1360eb(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid1360eb();
	flash_lock_otp_mid1360eb(FLASH_LOCK_OTP_0x001000_512K_MID1360EB);
	status5 = flash_read_status_mid1360eb();
	flash_erase_otp_mid1360eb(FLASH_SECURITY_ADDR);
	flash_read_otp_mid1360eb(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
#endif
}

_attribute_ram_code_ void flash_mid14325e_test(void)
{
	int i;

	status1 = flash_read_status_mid14325e();
	flash_lock_mid14325e(FLASH_LOCK_LOW_768K_MID14325E);
	status2 = flash_read_status_mid14325e();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid14325e();
	status3 = flash_read_status_mid14325e();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
}

_attribute_ram_code_ void flash_mid001460c8_test(void)
{
	int i;

	status1 = flash_read_status_mid001460c8();
	flash_lock_mid001460c8(FLASH_LOCK_LOW_768K_MID001460C8);
	status2 = flash_read_status_mid001460c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid001460c8();
	status3 = flash_read_status_mid001460c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
}

_attribute_ram_code_ void flash_mid011460c8_test(void)
{
	int i;

	status1 = flash_read_status_mid011460c8();
	flash_lock_mid011460c8(FLASH_LOCK_LOW_64K_MID011460C8);
	status2 = flash_read_status_mid011460c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}

	flash_unlock_mid011460c8();
	status3 = flash_read_status_mid011460c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
	flash_write_status_mid011460c8(status1, FLASH_WRITE_STATUS_BP_MID011460C8);

	flash_erase_otp_mid011460c8(FLASH_SECURITY_ADDR);
	flash_read_otp_mid011460c8(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}

	flash_write_otp_mid011460c8(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid011460c8(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid011460c8();
	flash_lock_otp_mid011460c8(FLASH_LOCK_OTP_0x001000_512K_MID011460C8);
	status5 = flash_read_status_mid011460c8();
	flash_erase_otp_mid011460c8(FLASH_SECURITY_ADDR);
	flash_read_otp_mid011460c8(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
#endif
}

void user_init()
{
	int i;

	sleep_ms(1000);

	mid = flash_read_mid();

	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.erase_err = 1;
			while(1);
		}
	}

	flash_write_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.write_err = 1;
			while(1);
		}
	}

	switch(mid)
	{
	case 0x1060c8:
		flash_mid1060c8_test();
		break;
	case 0x13325e:
		flash_mid13325e_test();
		break;
	case 0x134051:
		flash_mid134051_test();
		break;
	case 0x136085:
		flash_mid136085_test();
		break;
	case 0x1360c8:
		flash_mid1360c8_test();
		break;
	case 0x1360eb:
		flash_mid1360eb_test();
		break;
	case 0x14325e:
		flash_mid14325e_test();
		break;
	case 0x001460c8:
		flash_mid001460c8_test();
		break;
	case 0x011460c8:
		flash_mid011460c8_test();
		break;
	default:
		break;
	}

	err_status.check_umid = flash_read_mid_uid_with_check((unsigned int *)&mid, uid);
}


#else
void user_init()
{
	int i;

	sleep_ms(1000);

	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.erase_err = 1;
			while(1);
		}
	}

	flash_write_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.write_err = 1;
			while(1);
		}
	}

}
#endif


void main_loop (void)
{
	sleep_ms(1000);
}



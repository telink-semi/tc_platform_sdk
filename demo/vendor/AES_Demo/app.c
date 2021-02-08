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


#if(AES_MODE == RISC_MODE)

unsigned char sPlainText[16] 	= {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
unsigned char sKey[16] 			= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char EncryptResult[16] = {};
unsigned char DecryptResult[16] = {};
unsigned char CipherStd[16]		= {0x69,0xc4,0xe0,0xd8,0x6a,0x7b,0x04,0x30,0xd8,0xcd,0xb7,0x80,0x70,0xb4,0xc5,0x5a};

#elif(AES_MODE == DMA_MODE)

unsigned int sPlainText[64]={0x00000040,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc};
unsigned char sKey[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned int EncryptResult[64];
unsigned int CipherStd[64] = {0x00000040,
							 0xd8e0c469,0x30047b6a,0x80b7cdd8,0x5ac5b470,
							 0xd8e0c469,0x30047b6a,0x80b7cdd8,0x5ac5b470,
							 0xd8e0c469,0x30047b6a,0x80b7cdd8,0x5ac5b470,
							 0xd8e0c469,0x30047b6a,0x80b7cdd8,0x5ac5b470};
unsigned int DecryptResult[64];

#elif(AES_MODE == CBC_MODE)

unsigned char EncryptResult1[16] = {};
unsigned char EncryptResult2[16] = {};
unsigned char DecryptResult1[16] = {};
unsigned char DecryptResult2[16] = {};
unsigned char temp[16] = {};
unsigned char EncryptStd1[16] = {0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d};
unsigned char EncryptStd2[16] = {0x50,0x86,0xcb,0x9b,0x50,0x72,0x19,0xee,0x95,0xdb,0x11,0x3a,0x91,0x76,0x78,0xb2};
unsigned char init[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

unsigned char sPlainText1[16]={0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
unsigned char sPlainText2[16]={0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51};
unsigned char sKey[16]={0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
//unsigned char counter1[16]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
//unsigned char counter2[16]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xff,0x00};

#endif
void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1|LED2|LED3|LED4 ,AS_GPIO);       //set as GPIO
	gpio_set_output_en(LED1|LED2|LED3|LED4, 1); 		//enable output
	gpio_set_input_en(LED1|LED2|LED3|LED4 ,0);			//disable input
	gpio_write(LED1|LED2|LED3|LED4, 0);              	//LED OFF

}

void main_loop (void)
{
#if(AES_MODE == RISC_MODE)
	//encrypt the plain text
	aes_encrypt(sKey, sPlainText, EncryptResult);

	//check the encrypt result with std result.
	for(unsigned char i = 0; i < 16; i++)
	{
		if(EncryptResult[i] != CipherStd[i])
		{
			gpio_toggle(LED1);
			while(1);
		}
	}

	//decrypt the result being encrypted and save result to DecryptResult[]
	aes_decrypt(sKey, EncryptResult, DecryptResult);

	//check the dncrypt result with sPlainText.
	for(unsigned char i = 0; i < 16; i++)
	{
		if(DecryptResult[i] != sPlainText[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}
	sleep_ms(500);
	gpio_toggle(LED1|LED2|LED3|LED4);

#elif(AES_MODE == DMA_MODE)
	//encrypt the plain text
	aes_dma_encrypt(sKey,sPlainText,sizeof(sPlainText),EncryptResult,sizeof(EncryptResult) );

	//check the encrypt result with std result.
	for(unsigned char i = 0; i < (sizeof(sPlainText)/sizeof(unsigned int)); i++)
	{
		if(EncryptResult[i] != CipherStd[i])
		{
			gpio_toggle(LED1);
			while(1);
		}
	}

	//decrypt the result being encrypted and save result to decrypt_result[]
	aes_dma_decrypt(sKey,EncryptResult,sizeof(EncryptResult),DecryptResult,sizeof(DecryptResult) );

	//check the encrypt result with std result.
	for(unsigned char i = 0; i < (sizeof(sPlainText)/sizeof(unsigned int)); i++)
	{
		if(DecryptResult[i] != sPlainText[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}
	sleep_ms(500);
	gpio_toggle(LED1|LED2|LED3|LED4);
#elif(AES_MODE == CBC_MODE)

	//encrypt 1
	for(int i = 0;i<16;i++)
	{
		temp[i] = sPlainText1[i]^init[i];
	}
	aes_encrypt(sKey, temp, EncryptResult1);

	//chech result
	for(int i = 0;i<16;i++)
	{
		if(EncryptResult1[i] != EncryptStd1[i])
		{
			gpio_toggle(LED1);
			while(1);
		}
	}

	//encrypt 2
	for(int i = 0;i<16;i++)
	{
		temp[i] = sPlainText2[i]^EncryptResult1[i];
	}
	aes_encrypt(sKey, temp, EncryptResult2);

	//chech result
	for(int i = 0;i<16;i++)
	{
		if(EncryptResult2[i] != EncryptStd2[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}

	//decrypt 1
	aes_decrypt(sKey, EncryptResult1, DecryptResult1);
	for(int i = 0;i<16;i++)
	{
		DecryptResult1[i] = DecryptResult1[i]^init[i];
	}

	//chech result
	for(int i = 0;i<16;i++)
	{
		if(DecryptResult1[i] != sPlainText1[i])
		{
			gpio_toggle(LED3);
			while(1);
		}
	}

	//decrypt 2
	aes_decrypt(sKey, EncryptResult2, DecryptResult2);
	for(int i = 0;i<16;i++)
	{
		DecryptResult2[i] = DecryptResult2[i]^EncryptResult1[i];
	}

	//chech result
	for(int i = 0;i<16;i++)
	{
		if(DecryptResult2[i] != sPlainText2[i])
		{
			gpio_toggle(LED4);
			while(1);
		}
	}

	sleep_ms(500);
	gpio_toggle(LED1|LED2|LED3|LED4);
#endif
}





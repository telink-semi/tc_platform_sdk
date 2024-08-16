/********************************************************************************************************
 * @file    app_x25519.c
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
/********* pke version:1.0 *********/
#include "app_config.h"

#if (PKE_TEST_MODE == PKE_X25519)


unsigned char x25519_std_test()
{
	//the test vectors are from RFC7748
	unsigned char A_prikey[] = {
		0x77,0x07,0x6D,0x0A,0x73,0x18,0xA5,0x7D,0x3C,0x16,0xC1,0x72,0x51,0xB2,0x66,0x45,
		0xDF,0x4C,0x2F,0x87,0xEB,0xC0,0x99,0x2A,0xB1,0x77,0xFB,0xA5,0x1D,0xB9,0x2C,0x2A};
	unsigned char A_pubkey[] = {
		0x85,0x20,0xF0,0x09,0x89,0x30,0xA7,0x54,0x74,0x8B,0x7D,0xDC,0xB4,0x3E,0xF7,0x5A,
		0x0D,0xBF,0x3A,0x0D,0x26,0x38,0x1A,0xF4,0xEB,0xA4,0xA9,0x8E,0xAA,0x9B,0x4E,0x6A};
	unsigned char B_prikey[] = {
		0x5D,0xAB,0x08,0x7E,0x62,0x4A,0x8A,0x4B,0x79,0xE1,0x7F,0x8B,0x83,0x80,0x0E,0xE6,
		0x6F,0x3B,0xB1,0x29,0x26,0x18,0xB6,0xFD,0x1C,0x2F,0x8B,0x27,0xFF,0x88,0xE0,0xEB};
	unsigned char B_pubkey[] = {
		0xDE,0x9E,0xDB,0x7D,0x7B,0x7D,0xC1,0xB4,0xD3,0x5B,0x61,0xC2,0xEC,0xE4,0x35,0x37,
		0x3F,0x83,0x43,0xC8,0x5B,0x78,0x67,0x4D,0xAD,0xFC,0x7E,0x14,0x6F,0x88,0x2B,0x4F};
	unsigned char std_key[] = {
		0x4A,0x5D,0x9D,0x5B,0xA4,0xCE,0x2D,0xE1,0x72,0x8E,0x3B,0xF4,0x80,0x35,0x0F,0x25,
		0xE0,0x7E,0x21,0xC9,0x47,0xD1,0x9E,0x33,0x76,0xF0,0x9B,0x3C,0x1E,0x16,0x17,0x42};
	unsigned char A_key[32], B_key[32];
	unsigned char ret;

	ret = x25519_get_pubkey_from_prikey(A_prikey, A_key);
	if(X25519_SUCCESS != ret || memcmp(A_key, A_pubkey, 32))
	{
		printf("\r\n X25519 A get key error, ret=%d", ret);
		return 1;
	}

	ret = x25519_get_pubkey_from_prikey(B_prikey, B_key);
	if(X25519_SUCCESS != ret || memcmp(B_key, B_pubkey, 32))
	{
		printf("\r\n X25519 B get key error, ret=%d", ret);
		return 1;
	}

	ret = x25519_compute_key(A_prikey, B_pubkey, A_key, 32, NULL);
	if(X25519_SUCCESS != ret)
	{
		printf("\r\n X25519 A calc error, ret=%d", ret);
		return 1;
	}

	ret = x25519_compute_key(B_prikey, A_pubkey, B_key, 32, NULL);
	if(X25519_SUCCESS != ret)
	{
		printf("\r\n X25519 B calc error, ret=%d", ret);
		return 1;
	}

	if(memcmp(A_key, B_key, 32) || memcmp(A_key, std_key, 32))
	{
		printf("\r\n X25519 key agreement error ");
		return 1;
	}
	else
	{
		printf("\r\n X25519 key agreement success ");
	}

	return 0;
}

/**
 * @brief		x25519 rand test.
 * @return		X25519_SUCCESS(success); other(error).
 */
unsigned char x25519_rand_test()
{
	unsigned char A_prikey[32];
	unsigned char A_pubkey[32];
	unsigned char B_prikey[32];
	unsigned char B_pubkey[32];
	unsigned char A_key[200], B_key[200];
	unsigned int keyByteLen, i,j;
	unsigned char ret;
	KDF_FUNC kdf=NULL;

	for(i=0;i<10;i++)
	{
		ret = x25519_getkey(A_prikey, A_pubkey);
		if(X25519_SUCCESS != ret)
		{
			printf("\r\n X25519 A get key error, ret=%d", ret);
			return 1;
		}
		ret = x25519_getkey(B_prikey, B_pubkey);
		if(X25519_SUCCESS != ret)
		{
			printf("\r\n X25519 B get key error, ret=%d", ret);
			return 1;
		}

		for(j=0;j<50;j++)
		{
			printf("\r\n X25519 round1=%d, round2=%d", i+1, j+1);

			keyByteLen = A_prikey[0]+B_prikey[0];
			if(NULL == kdf)
			{
				keyByteLen %= 32;
			}
			else
			{
				keyByteLen %= 200;
			}
			if(0 == keyByteLen)
			{
				keyByteLen+=1;
			}

			ret = x25519_compute_key(A_prikey, B_pubkey, A_key, keyByteLen, kdf);
			if(X25519_SUCCESS != ret)
			{
				printf("\r\n X25519 A calc error, ret=%d", ret);
				return 1;
			}

			ret = x25519_compute_key(B_prikey, A_pubkey, B_key, keyByteLen, kdf);
			if(X25519_SUCCESS != ret)
			{
				printf("\r\n X25519 B calc error, ret=%d", ret);
				return 1;
			}

			if(memcmp(A_key, B_key, keyByteLen))
			{
				printf("\r\n X25519 key agreement error ");
				return 1;
			}
		}
	}

	return 0;
}

/**
 * @brief		x25519 speed test.
 * @return		x25519_SUCCESS(success); other(error).
 */
unsigned char x25519_speed_test()
{
	unsigned char A_prikey[32];
	unsigned char A_pubkey[32];
	unsigned char B_prikey[32];
	unsigned char B_pubkey[32];
	unsigned char A_key[200], B_key[200];
	unsigned int i, keyByteLen=32;
	unsigned char ret;

printf("\r\n begin");
for(i=0;i<80;i++)
	ret = x25519_getkey(A_prikey, A_pubkey);
printf("\r\n finished");
	if(X25519_SUCCESS != ret)
	{
		printf("\r\n X25519 A get key error, ret=%d", ret);
		return 1;
	}
	ret = x25519_getkey(B_prikey, B_pubkey);
	if(X25519_SUCCESS != ret)
	{
		printf("\r\n X25519 B get key error, ret=%d", ret);
		return 1;
	}
printf("\r\n begin");
for(i=0;i<80;i++)
	ret = x25519_compute_key(A_prikey, B_pubkey, A_key, 32, NULL);
printf("\r\n finished");
	if(ret)
	{
		printf("\r\n X25519 A calc error, ret=%d", ret);
		return 1;
	}

	ret = x25519_compute_key(B_prikey, A_pubkey, B_key, 32, NULL);
	if(ret)
	{
		printf("\r\n X25519 B calc error, ret=%d", ret);
		return 1;
	}

	if(memcmp(A_key, B_key, keyByteLen))
	{
		printf("\r\n X25519 key agreement error ");
		return 1;
	}
	else
	{
		printf("\r\n X25519 key agreement success ");
	}

	return 0;
}


unsigned char X25519_all_test()
{
	printf("\r\n\r\n =================== x25519 test =================== \r\n");

#if 0
	sleep_ms(5);
	if(x25519_speed_test())
		return 1;
#endif

	if(x25519_std_test())
		return 1;

	if(x25519_rand_test())
		return 1;

	return 0;
}




void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);
    gpio_set_func(LED3, AS_GPIO);
    gpio_set_func(LED4, AS_GPIO);

    gpio_set_output_en(LED1, 1);
    gpio_set_output_en(LED2, 1);
    gpio_set_output_en(LED3, 1);
    gpio_set_output_en(LED4, 1);

    gpio_set_input_en(LED1, 0);
    gpio_set_input_en(LED2, 0);
    gpio_set_input_en(LED3, 0);
    gpio_set_input_en(LED4, 0);

    gpio_write(LED1, 1);
    gpio_write(LED2, 1);
    gpio_write(LED3, 1);
    gpio_write(LED4, 1);

	X25519_all_test();
}



void main_loop (void)
{
	sleep_ms(500);

    gpio_toggle(LED1);
    gpio_toggle(LED2);
    gpio_toggle(LED3);
    gpio_toggle(LED4);
}

#endif




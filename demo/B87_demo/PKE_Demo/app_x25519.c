/********************************************************************************************************
 * @file	app_x25519.c
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
#include "PKE_COMMON/app_pke.h"

#if (PKE_TEST_MODE == PKE_X25519)


/********* oprand length *********/
#define PKE_OPERAND_MAX_WORD_LEN      (0x08)
#define PKE_OPERAND_MAX_BIT_LEN       (0x100)
#define ECC_MAX_WORD_LEN              PKE_OPERAND_MAX_WORD_LEN
#define ECC_MAX_BIT_LEN               PKE_OPERAND_MAX_BIT_LEN
#define C25519_WORD_LEN               ECC_MAX_WORD_LEN
#define C25519_BYTE_LEN               (C25519_WORD_LEN<<2)
#define Ed25519_WORD_LEN              C25519_WORD_LEN
#define Ed25519_BYTE_LEN              C25519_BYTE_LEN

//X25519 return code
enum {
	X25519_SUCCESS = PKE_SUCCESS,
	X25519_POINTOR_NULL = PKE_SUCCESS+0x70,
	X25519_INVALID_INPUT,
	X25519_INVALID_OUTPUT,
};

//Curve25519 parameters
unsigned int curve25519_p[8]    = {0xFFFFFFED,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x7FFFFFFF,};
unsigned int curve25519_p_h[8]  = {0x000005A4,0,0,0,0,0,0,0,};
unsigned int curve25519_p_n1[1] = {0x286BCA1B};
unsigned int curve25519_a24[8]  = {0x0001DB41,0,0,0,0,0,0,0,};
//unsigned int curve25519_B[]   = {0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,};
unsigned int curve25519_u[]     = {0x00000009,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,};
unsigned int curve25519_v[]     = {0x7ECED3D9,0x29E9C5A2,0x6D7C61B2,0x923D4D7E,0x7748D14C,0xE01EDD2C,0xB8A086B4,0x20AE19A1,};
unsigned int curve25519_n[]     = {0x5CF5D3ED,0x5812631A,0xA2F79CD6,0x14DEF9DE,0x00000000,0x00000000,0x00000000,0x10000000,};
unsigned int curve25519_n_h[8]  = {0x449C0F01,0xA40611E3,0x68859347,0xD00E1BA7,0x17F5BE65,0xCEEC73D2,0x7C309A3D,0x0399411B,};
unsigned int curve25519_n_n1[1] = {0x12547E1B};
unsigned int curve25519_h       = 8;


mont_curve_t c25519[1]={
{	255,
	curve25519_p,
	curve25519_p_h,
	curve25519_p_n1,
	curve25519_a24,
	curve25519_u,
	curve25519_v,
	curve25519_n,
	curve25519_n_h,
	curve25519_n_n1,
	&curve25519_h,}
};


/**
 * @brief		get X25519 public key from private key
 * @param[in]	priKey		- private key, 32 bytes, little-endian
 * @param[out]	pubKey		- public key, 32 bytes, little-endian
 * @return		X25519_SUCCESS(success); other(error)
 */
unsigned char x25519_get_pubkey_from_prikey(unsigned char prikey[32], unsigned char pubkey[32])
{
	unsigned int t[C25519_WORD_LEN];
	unsigned char ret;

	if(NULL == prikey || NULL == pubkey)
	{
		return X25519_POINTOR_NULL;
	}

	//decode scalar
	memcpy(t, prikey, C25519_BYTE_LEN);
	*(((unsigned char *)t)+0) &= 0xF8;
	*(((unsigned char *)t)+31) &= 0x7F;
	*(((unsigned char *)t)+31) |= 0x40;

	//it could be proved that here t is not multiple of c25519->n, so no need to compare
	//(t mod c25519->n) with c25519->n

	ret = pke_x25519_point_mul(c25519, t, c25519->mont_u, t);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	memcpy(pubkey, t, C25519_BYTE_LEN);

	return X25519_SUCCESS;
}
/**
 * @brief		get x25519 random key pair
 * @param[out]	priKey		- private key, 32 bytes, little-endian
 * @param[out]	pubKey		- public key, 32 bytes, little-endian
 * @return		X25519_SUCCESS(success); other(error)
 */
unsigned char x25519_getkey(unsigned char prikey[32], unsigned char pubkey[32])
{
	unsigned char ret;

	if(NULL == prikey || NULL == pubkey)
	{
		return X25519_POINTOR_NULL;
	}

	ret = rand_get(prikey, C25519_BYTE_LEN);
	if(TRNG_SUCCESS != ret)
	{
		return ret;
	}

	return x25519_get_pubkey_from_prikey(prikey, pubkey);
}
/**
 * @brief		X25519 key agreement
 * @param[in]	local_prikey	- local private key, 32 bytes, little-endian
 * @param[in]	peer_pubkey		- peer Public key, 32 bytes, little-endian
 * @param[in]	keyByteLen		- byte length of output key
 * @param[in]	kdf				- KDF function
 * @param[out]	key				- derived key
 * @return		X25519_SUCCESS(success); other(error)
 * @Caution		if no KDF function, please set kdf to be NULL
 */
unsigned char x25519_compute_key(unsigned char local_prikey[32], unsigned char peer_pubkey[32], unsigned char *key, unsigned int keyByteLen,
						   KDF_FUNC kdf)
{
	unsigned int k[C25519_WORD_LEN], u[C25519_WORD_LEN];
	unsigned char ret;

	if(NULL == local_prikey || NULL == peer_pubkey || NULL == key)
	{
		return X25519_POINTOR_NULL;
	}

	if(0 == keyByteLen)
	{
		return X25519_INVALID_INPUT;
	}

	//decode u
	memcpy(u, peer_pubkey, C25519_BYTE_LEN);
	*(((unsigned char *)u)+31) &= 0x7F;
	if(big_integer_compare(u, C25519_WORD_LEN, c25519->mont_p, C25519_WORD_LEN) >= 0)
	{
		sub_u32(u, c25519->mont_p, k, C25519_WORD_LEN);
		memcpy(u, k, C25519_WORD_LEN<<2);
	}

	//decode scalar
	memcpy(k, local_prikey, C25519_BYTE_LEN);
	*(((unsigned char *)k)+0) &= 0xF8;
	*(((unsigned char *)k)+31) &= 0x7F;
	*(((unsigned char *)k)+31) |= 0x40;

	ret = pke_x25519_point_mul(c25519, k, u, u);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//make sure u is not zero
	k[0]=0;
	for(ret=0; ret<C25519_WORD_LEN; ret++)
	{
		k[0] |= u[ret];
	}
	if(0 == k[0])
	{
		return X25519_INVALID_OUTPUT;
	}

	if(kdf)
	{
		kdf(u, C25519_BYTE_LEN, key, keyByteLen);
	}
	else
	{
		if(keyByteLen > C25519_BYTE_LEN)
		{
			keyByteLen = C25519_BYTE_LEN;
		}
		memcpy(key, u, keyByteLen);
	}

	return X25519_SUCCESS;
}
/**
 * @brief		X25519 std test
 * @return		X25519_SUCCESS(success); other(error)
 */
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
 * @brief		X25519 rand test
 * @return		X25519_SUCCESS(success); other(error)
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
			else
			{
				printf("\r\n X25519 key agreement success! ");
			}
		}
	}

	return 0;
}
/**
 * @brief		X25519 speed test
 * @return		X25519_SUCCESS(success); other(error)
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
	for(i=0;i<1000;i++)
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

	ret = x25519_compute_key(A_prikey, B_pubkey, A_key, 32, NULL);
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
/**
 * @brief		X25519 all test
 * @return		X25519_SUCCESS(success); other(error)
 */
unsigned char X25519_all_test()
{
#if 0
	if(x25519_speed_test())
		return 1;
#endif

	if(x25519_std_test())
		return 1;

	if(x25519_rand_test())
		return 1;

	return 0;
}



void user_init()
{
//	uart_gpio_set(UART_TX_PA2, UART_RX_PA0);// uart tx/rx pin set
//
//	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
//
//	uart_init_baudrate(115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

	X25519_all_test();
}



void main_loop (void)
{

}

#endif




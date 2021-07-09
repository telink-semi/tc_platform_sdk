/********************************************************************************************************
 * @file	app_ecdh.c
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


#if (PKE_TEST_MODE == PKE_ECDH)

//ECDH return code
enum ECDH_RET_CODE
{
	ECDH_SUCCESS = PKE_SUCCESS,
	ECDH_POINTOR_NULL = PKE_SUCCESS+0x60,
	ECDH_INVALID_INPUT,
};


unsigned int secp256r1_p[8]    = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int secp256r1_p_h[8]  = {0x00000003,0x00000000,0xFFFFFFFF,0xFFFFFFFB,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFD,0x00000004};
unsigned int secp256r1_p_n1[1] = {1};
unsigned int secp256r1_a[8]    = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int secp256r1_b[8]    = {0x27D2604B,0x3BCE3C3E,0xCC53B0F6,0x651D06B0,0x769886BC,0xB3EBBD55,0xAA3A93E7,0x5AC635D8};
unsigned int secp256r1_Gx[8]   = {0xD898C296,0xF4A13945,0x2DEB33A0,0x77037D81,0x63A440F2,0xF8BCE6E5,0xE12C4247,0x6B17D1F2};
unsigned int secp256r1_Gy[8]   = {0x37BF51F5,0xCBB64068,0x6B315ECE,0x2BCE3357,0x7C0F9E16,0x8EE7EB4A,0xFE1A7F9B,0x4FE342E2};
unsigned int secp256r1_n[8]    = {0xFC632551,0xF3B9CAC2,0xA7179E84,0xBCE6FAAD,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF};
unsigned int secp256r1_n_h[8]  = {0xBE79EEA2,0x83244C95,0x49BD6FA6,0x4699799C,0x2B6BEC59,0x2845B239,0xF3D95620,0x66E12D94};
unsigned int secp256r1_n_n1[1] = {0xEE00BC4F};


eccp_curve_t secp256r1[1]={
{	256,
	256,
	secp256r1_p,
	secp256r1_p_h,
	secp256r1_p_n1,
	secp256r1_a,
	secp256r1_b,
	secp256r1_Gx,
	secp256r1_Gy,
	secp256r1_n,
	secp256r1_n_h,
	secp256r1_n_n1,}
};


/**
 * @brief		ECDH compute key
 * @param[in]	local_prikey	- local private key, big-endian
 * @param[in]	peer_pubkey		- peer public key, big-endian
 * @param[out]	key				- output key
 * @param[in]	keyByteLen		- byte length of output key
 * @param[in]	KDF				- KDF function to get key
 * @Return		0(success); other(error)
 */
unsigned char ecdh_compute_key(eccp_curve_t *curve, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key,
						 unsigned int keyByteLen, KDF_FUNC kdf)
{
	unsigned int k[ECC_MAX_WORD_LEN] = {0};
	unsigned int Px[ECC_MAX_WORD_LEN] = {0};
	unsigned int Py[ECC_MAX_WORD_LEN] = {0};
	unsigned int byteLen, wordLen;
	unsigned char ret;

	if(0 == curve || 0 == local_prikey || 0 == peer_pubkey || 0 == key)
	{
		return ECDH_POINTOR_NULL;
	}
	if(0 == keyByteLen)
	{
		return ECDH_INVALID_INPUT;
	}

	byteLen = (curve->eccp_n_bitLen+7)/8;
	wordLen = (curve->eccp_n_bitLen+31)/32;

	//make sure private key is in [1, n-1]
	memcpy((unsigned char *)k, (unsigned char *)local_prikey, byteLen);
	if(ismemzero4(k, wordLen<<2))
	{
		return ECDH_INVALID_INPUT;
	}
	if(big_integer_compare(k, wordLen, curve->eccp_n, wordLen) >= 0)
	{
		return ECDH_INVALID_INPUT;
	}

	//check public key
	memcpy((unsigned char *)Px, peer_pubkey, byteLen);
	memcpy((unsigned char *)Py, peer_pubkey+byteLen, byteLen);
	ret = pke_eccp_point_verify(curve, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	ret = pke_eccp_point_mul(curve, k, Px, Py, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	memcpy((unsigned char *)Px,(unsigned char *)Px,  byteLen);

	if(kdf)
	{
		kdf(Px, byteLen, key, keyByteLen);
	}
	else
	{
		if(keyByteLen > byteLen)
		{
			keyByteLen = byteLen;
		}
		memcpy(key, Px, keyByteLen);
	}

	return ECDH_SUCCESS;
}
/**
 * @brief		ecdh rand test
 * @return		ECDH_SUCCESS(success); other(error)
 */
unsigned char ecdh_rand_test(eccp_curve_t *curve)
{
	unsigned char local_prikey[32];
	unsigned char peer_prikey[32];
	unsigned char local_pubkey[64];
	unsigned char peer_pubkey[64];
	unsigned char local_key[200];
	unsigned char peer_key[200];
	unsigned int i,j, keyByteLen;
	KDF_FUNC kdf=NULL;
	unsigned char ret;

	for(i=0;i<10;i++)
	{
		ret = eccp_getkey(curve, local_prikey, local_pubkey);
		if(PKE_SUCCESS != ret)
		{
			printf("\r\n ECDH A get key error, ret=%d", ret);
			return 1;
		}
		ret = eccp_getkey(curve, peer_prikey, peer_pubkey);
		if(PKE_SUCCESS != ret)
		{
			printf("\r\n ECDH B get key error, ret=%d", ret);
			return 1;
		}
		for(j=0;j<50;j++)
		{
			printf("\r\n ECDH round1=%d, round2=%d", i+1, j+1);

			keyByteLen = local_prikey[0]+local_prikey[0];
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

			ret = ecdh_compute_key(curve, local_prikey, peer_pubkey, local_key,  keyByteLen, kdf);
			if(ECDH_SUCCESS != ret)
			{
				printf("\r\n ecdh test error 1, ret = %02x", ret);
				return 1;
			}

			ret = ecdh_compute_key(curve, peer_prikey, local_pubkey, peer_key,  keyByteLen, kdf);
			if(ECDH_SUCCESS != ret || memcmp(local_key, peer_key,keyByteLen))
			{
				printf("\r\n ecdh test error 2, ret = %02x", ret);
				return 1;
			}
			else
			{
				printf("\r\n ecdh test success! ");
			}
		}

	}
	return 0;
}



void user_init()
{
//	uart_gpio_set(UART_TX_PA2, UART_RX_PA0);// uart tx/rx pin set
//
//	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
//
//	uart_init_baudrate(115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

	ecdh_rand_test(secp256r1);
	gpio_write(GPIO_PD1,1);
}




void main_loop (void)
{

}

#endif




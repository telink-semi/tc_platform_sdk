/********************************************************************************************************
 * @file	x25519.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
/********* pke version:1.0 *********/
#include "lib/include/pke/x25519.h"


//Curve25519 parameters
unsigned int curve25519_p[8]    = {0xFFFFFFED,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x7FFFFFFF,};
unsigned int curve25519_p_h[8]  = {0x000005A4,0,0,0,0,0,0,0,};
unsigned int curve25519_p_n1[1] = {0x286BCA1B};
unsigned int curve25519_a24[8]  = {0x0001DB41,0,0,0,0,0,0,0,};
//unsigned int const curve25519_B[]   = {0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,};
unsigned int curve25519_u[]     = {0x00000009,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,};
unsigned int curve25519_v[]     = {0x7ECED3D9,0x29E9C5A2,0x6D7C61B2,0x923D4D7E,0x7748D14C,0xE01EDD2C,0xB8A086B4,0x20AE19A1,};
unsigned int curve25519_n[]     = {0x5CF5D3ED,0x5812631A,0xA2F79CD6,0x14DEF9DE,0x00000000,0x00000000,0x00000000,0x10000000,};
unsigned int curve25519_n_h[8]  = {0x449C0F01,0xA40611E3,0x68859347,0xD00E1BA7,0x17F5BE65,0xCEEC73D2,0x7C309A3D,0x0399411B,};
unsigned int curve25519_n_n1[1] = {0x12547E1B};
unsigned int curve25519_h       = 8;

mont_curve_t c25519[1]={
	{
		255,
		curve25519_p,
		curve25519_p_h,
		curve25519_p_n1,
		curve25519_a24,
		curve25519_u,
		curve25519_v,
		curve25519_n,
		curve25519_n_h,
		curve25519_n_n1,
		&curve25519_h,
	}
};


/**
 * @brief		get X25519 public key from private key.
 * @param[in]	priKey		- private key, 32 bytes, little-endian.
 * @param[out]	pubKey		- public key, 32 bytes, little-endian.
 * @Caution		X25519_SUCCESS(success), other(error).
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

	ret = x25519_pointMul(c25519, t, c25519->u, t);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	memcpy(pubkey, t, C25519_BYTE_LEN);

	return X25519_SUCCESS;
}

/**
 * @brief		get x25519 random key pair.
 * @param[out]	priKey		- private key, 32 bytes, little-endian.
 * @param[out]	pubKey		- public key, 32 bytes, little-endian.
 * @Caution		X25519_SUCCESS(success), other(error).
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
 * @brief		X25519 key agreement.
 * @param[in]	local_prikey	- local private key, 32 bytes, little-endian.
 * @param[in]	peer_pubkey		- peer Public key, 32 bytes, little-endian.
 * @param[in]	keyByteLen		- byte length of output key.
 * @param[in]	kdf				- KDF function.
 * @param[out]	key				- derived key.
 * @return		X25519_SUCCESS(success), other(error).
 * @Caution		if no KDF function, please set kdf to be NULL.
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
	if(uint32_BigNumCmp(u, C25519_WORD_LEN, c25519->p, C25519_WORD_LEN) >= 0)
	{
		pke_sub(u, c25519->p, k, C25519_WORD_LEN);
		uint32_copy(u, k, C25519_WORD_LEN);
	}

	//decode scalar
	memcpy(k, local_prikey, C25519_BYTE_LEN);
	*(((unsigned char *)k)+0) &= 0xF8;
	*(((unsigned char *)k)+31) &= 0x7F;
	*(((unsigned char *)k)+31) |= 0x40;

	ret = x25519_pointMul(c25519, k, u, u);
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


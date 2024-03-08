/********************************************************************************************************
 * @file	ecdh.c
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
/********* pke version:1.1 *********/
#include "lib/include/pke/ecdh.h"


/**
 * @brief		ECDH compute key.
 * @param[in]   curve			- eccp curve struct pointer.
 * @param[in]	local_prikey	- local private key, big-endian.
 * @param[in]	peer_pubkey		- peer public key, big-endian.
 * @param[out]	key				- output key.
 * @param[in]	keyByteLen		- byte length of output key.
 * @param[in]	KDF				- KDF function to get key.
 * @Return		0(success), other(error).
 */
unsigned int ecdh_compute_key(eccp_curve_t *curve, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key,
		unsigned int keyByteLen, KDF_FUNC kdf)
{
	unsigned int k[ECCP_MAX_WORD_LEN] = {0};
	unsigned int Px[ECCP_MAX_WORD_LEN] = {0};
	unsigned int Py[ECCP_MAX_WORD_LEN] = {0};
	unsigned int byteLen, wordLen;
	unsigned int ret;

	if(NULL == curve || NULL == local_prikey || NULL == peer_pubkey || NULL == key)
	{
		return ECDH_POINTOR_NULL;
	}
	else if(0 == keyByteLen)
	{
		return ECDH_INVALID_INPUT;
	}
	else
	{;}

	byteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	wordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

	//make sure private key is in [1, n-1]
	reverse_byte_array((unsigned char *)local_prikey, (unsigned char *)k, byteLen);
	if(uint32_BigNum_Check_Zero(k, wordLen))
	{
		return ECDH_INVALID_INPUT;
	}
	else if(uint32_BigNumCmp(k, wordLen, curve->eccp_n, wordLen) >= 0)
	{
		return ECDH_INVALID_INPUT;
	}
	else
	{;}

	//check public key
	byteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
	reverse_byte_array(peer_pubkey, (unsigned char *)Px, byteLen);
	reverse_byte_array(peer_pubkey+byteLen, (unsigned char *)Py, byteLen);
	ret = eccp_pointVerify(curve, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	ret = eccp_pointMul(curve, k, Px, Py, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	reverse_byte_array((unsigned char *)Px, (unsigned char *)Px, byteLen);

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
		else
		{;}

		memcpy(key, Px, keyByteLen);
	}

	return ECDH_SUCCESS;
}





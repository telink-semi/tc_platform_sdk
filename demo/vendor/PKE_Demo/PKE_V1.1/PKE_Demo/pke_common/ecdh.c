/********************************************************************************************************
 * @file	ecdh.c
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
#include "ecdh.h"

/**
 * @brief       ECDH compute key.
 * @param[in]   local_prikey - local private key, big-endian.
 * @param[in]   peer_pubkey  - peer public key, big-endian.
 * @param[in]   keyByteLen   - byte length of output key.
 * @param[out]	key          - output key.
 * @return   	PKE_SUCCESS(success), other(inverse not exists or error).
 */
unsigned int ecdh_compute_key(eccp_curve_t *curve, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key,
		unsigned int keyByteLen)
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

//	if(kdf)
//	{
//		kdf(Px, byteLen, key, keyByteLen);
//	}
//	else
//	{
		if(keyByteLen > byteLen)
		{
			keyByteLen = byteLen;
		}
		else
		{;}

		memcpy_(key, Px, keyByteLen);
//	}

	return ECDH_SUCCESS;
}


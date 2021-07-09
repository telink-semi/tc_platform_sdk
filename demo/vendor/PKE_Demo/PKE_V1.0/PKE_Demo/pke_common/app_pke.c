/********************************************************************************************************
 * @file	app_pke.c
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
#include "app_pke.h"


extern unsigned int rnd_m_w;
/**
 * @brief		get rand
 * @param[in] 	rand	- the buffer rand
 * @param[in] 	byteLen	- the length of rand
 * @return		0:TRNG_SUCCESS,   1:TRNG_ERROR
 */
unsigned char rand_get(unsigned char *rand, unsigned int byteLen)
{
	unsigned int word_len;
    unsigned char left_len;

    if(0 == rand)
    {
    	return TRNG_ERROR;
    }

    word_len = byteLen >> 2;
    left_len = byteLen & 0x3;

    // obtain the data by word
    while (word_len--)
    {
		random_generator_init();
	    *((unsigned int *)rand) = rnd_m_w;
        rand += 4;
    }

    // if the byteLen is not aligned by word
    if (left_len)
    {
    	random_generator_init();
        memcpy(rand, &rnd_m_w, left_len);
    }

	return TRNG_SUCCESS;
}
/**
 * @brief		get ECCP key pair(the key pair could be used in ECDSA/ECDH)
 * @param[in]	eccp_curve	- eccp_curve_t struct pointer
 * @param[in]	priKey		- private key, big-endian
 * @param[in]	pubKey		- public key, big-endian
 * @return		PKE_SUCCESS(success), other(error)
 */
unsigned char eccp_getkey(eccp_curve_t *curve, unsigned char *priKey, unsigned char *pubKey)
{
	unsigned int tmpLen;
 	unsigned int nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
 	unsigned int nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
 	unsigned int pByteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
 	unsigned int k[PKE_OPERAND_MAX_WORD_LEN] = {0};
 	unsigned int x[PKE_OPERAND_MAX_WORD_LEN];
 	unsigned int y[PKE_OPERAND_MAX_WORD_LEN];
 	unsigned char ret;


ECCP_GETKEY_LOOP:
	ret = rand_get((unsigned char *)k, nByteLen);
 	if(TRNG_SUCCESS != ret)
 	{
 		return ret;
 	}

 	//make sure k has the same bit length as n
 	tmpLen = (curve->eccp_n_bitLen)&0x1F;
 	if(tmpLen)
 	{
 		k[nWordLen-1] &= (1<<(tmpLen))-1;
 	}

 	//make sure k in [1, n-1]
 	if(ismemzero4(k, nWordLen<<2))
 	{
 		goto ECCP_GETKEY_LOOP;
 	}
 	if(big_integer_compare(k, nWordLen, curve->eccp_n, nWordLen) >= 0)
 	{
 		goto ECCP_GETKEY_LOOP;
 	}
 	//get pubKey
 	ret = pke_eccp_point_mul(curve, k, curve->eccp_Gx, curve->eccp_Gy, x, y);
 	if(PKE_SUCCESS != ret)
 	{
 		return ret;
 	}
 	memcpy(priKey, (unsigned char *)k, nByteLen);
 	memcpy(pubKey, (unsigned char *)x, pByteLen);
 	memcpy(pubKey+pByteLen, (unsigned char *)y, pByteLen);

 	return PKE_SUCCESS;
}



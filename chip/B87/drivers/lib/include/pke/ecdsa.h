/********************************************************************************************************
 * @file	ecdsa.h
 *
 * @brief	This is the header file for b87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/********* pke version:1.0 *********/
#ifndef _ECDSA_H_
#define _ECDSA_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "pke.h"



//ECDSA return code
enum ECDSA_RET_CODE
{
	ECDSA_SUCCESS = PKE_SUCCESS,
	ECDSA_POINTOR_NULL = PKE_SUCCESS+0x50,
	ECDSA_INVALID_INPUT,
	ECDSA_ZERO_ALL,
	ECDSA_INTEGER_TOO_BIG,
	ECDSA_VERIFY_FAILED,
};


//APIs

/**
 * @brief		Generate ECDSA Signature in octet string style.
 * @param[in]	curve		- ecc curve struct pointer, please make sure it is valid.
 * @param[in]	E			- hash value, big-endian.
 * @param[in]	EByteLen	- byte length of E.
 * @param[in]	priKey		- private key, big-endian.
 * @param[out]	signature	- signature r and s, big-endian.
 * @return		ECDSA_SUCCESS(success), other(error).
 */
unsigned char ecdsa_sign(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *rand_k, unsigned char *priKey,
				   unsigned char *signature);

/**
 * @brief		Verify ECDSA Signature in octet string style.
 * @param[in]	curve		- ecc curve struct pointer, please make sure it is valid.
 * @param[in]	E			- hash value, big-endian.
 * @param[in]	EByteLen	- byte length of E.
 * @param[in]	pubKey		- public key, big-endian.
 * @param[in]	signature	- signature r and s, big-endian.
 * @return		ECDSA_SUCCESS(success), other(error).
 */
unsigned char ecdsa_verify(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *pubKey, unsigned char *signature);



#ifdef __cplusplus
}
#endif

#endif

/********************************************************************************************************
 * @file	x25519.h
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
#ifndef _X25519_H_
#define _X25519_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "pke.h"



extern mont_curve_t c25519[1];



//X25519 return code
enum {
	X25519_SUCCESS = PKE_SUCCESS,
	X25519_POINTOR_NULL = PKE_SUCCESS+0x70,
	X25519_INVALID_INPUT,
	X25519_INVALID_OUTPUT,
};


//APIs

/**
 * @brief		get X25519 public key from private key.
 * @param[in]	priKey		- private key, 32 bytes, little-endian.
 * @param[out]	pubKey		- public key, 32 bytes, little-endian.
 * @return		X25519_SUCCESS(success), other(error).
 */
unsigned char x25519_get_pubkey_from_prikey(unsigned char prikey[32], unsigned char pubkey[32]);

/**
 * @brief		get x25519 random key pair.
 * @param[out]	priKey		- private key, 32 bytes, little-endian.
 * @param[out]	pubKey		- public key, 32 bytes, little-endian.
 * @return		X25519_SUCCESS(success), other(error).
 */
unsigned char x25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

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
						   KDF_FUNC kdf);


#ifdef __cplusplus
}
#endif

#endif

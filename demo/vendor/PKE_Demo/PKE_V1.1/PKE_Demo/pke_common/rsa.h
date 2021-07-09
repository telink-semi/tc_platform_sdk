/********************************************************************************************************
 * @file	rsa.h
 *
 * @brief	This is the header file for b85m
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
#ifndef PKE_DEMO_RSA_H_
#define PKE_DEMO_RSA_H_

#include "../app_config.h"
#include "types.h"

//RSA return code
enum RSA_RET_CODE
{
    RSA_SUCCESS = PKE_SUCCESS,
    RSA_BUFFER_NULL = PKE_SUCCESS+0x30,
    RSA_INPUT_TOO_LONG,
    RSA_INPUT_INVALID,
};

/**
 * @brief       out = a^e mod n.
 * @param[in]   a       - unsigned int big integer a, base number, make sure a < n.
 * @param[in]   e       - unsigned int big integer e, exeponent, make sure e < n.
 * @param[in]   n       - unsigned int big integer n, modulus, make sure n is odd.
 * @param[in]   eBitLen - real bit length of unsigned int big integer e.
 * @param[in]   nBitLen - real bit length of unsigned int big integer n.
 * @param[out]	out     - out = a^e mod n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @note        a, n, and out have the same word length:((nBitLen+31)>>5); and e word length is (eBitLen+31)>>5.
 */
unsigned int RSA_ModExp(unsigned int *a, unsigned int *e, unsigned int *n, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen);

/**
 * @brief       out = a^d mod n, here d represents RSA CRT private key (p,q,dp,dq,u).
 * @param[in]   a       - unsigned int big integer a, base number, make sure a < n=pq.
 * @param[in]   p       - unsigned int big integer p, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]   q       - unsigned int big integer q, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]   dp      - unsigned int big integer dp = e^(-1) mod (p-1), one part of private key (p,q,dp,dq,u).
 * @param[in]   dq      - unsigned int big integer dq = e^(-1) mod (q-1), one part of private key (p,q,dp,dq,u).
 * @param[in]   u       - unsigned int big integer u = q^(-1) mod p, one part of private key (p,q,dp,dq,u).
 * @param[in]   nBitLen - real bit length of unsigned int big integer n=pq.
 * @param[out]	out     - out = a^d mod n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @note        a and out have the same word length:((nBitLen+31)>>5); and p,p_h,q,q_h,dp,dq,u have the same word length:((nBitLen/2+31)>>5).
 */
unsigned int RSA_CRTModExp(unsigned int *a, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int*dq,unsigned int *u, unsigned int *out,  unsigned int nBitLen);

/**
 * @brief       generate RSA key (e,d,n).
 * @param[in]   eBitLen - real bit length of e.
 * @param[in]   nBitLen - real bit length of n.
 * @param[out]	e       - unsigned int big integer, RSA public key e.
 * @param[out]	d       - unsigned int big integer, RSA private key d.
 * @param[out]	n       - unsigned int big integer, RSA public module n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @note        1. nBitLen can not be even.
 *              2. eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
unsigned int RSA_GetKey(unsigned int *e, unsigned int *d, unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);

/**
 * @brief       generate RSA-CRT key (e,p,q,dp,dq,u,n).
 * @param[in]   eBitLen - real bit length of e.
 * @param[in]   nBitLen - real bit length of n.
 * @param[out]	e       - unsigned int big integer, RSA public key e.
 * @param[out]	p       - unsigned int big integer, RSA private key p.
 * @param[out]	q       - unsigned int big integer, RSA private key q.
 * @param[out]	dp      - unsigned int big integer, RSA private key dp.
 * @param[out]	dq      - unsigned int big integer, RSA private key dq.
 * @param[out]	u       - unsigned int big integer, RSA private key u = q^(-1) mod p.
 * @param[out]	n       - unsigned int big integer, RSA public module n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @note        1. nBitLen can not be even.
 *              2. eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
unsigned int RSA_GetCRTKey(unsigned int *e, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u,unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);


#endif

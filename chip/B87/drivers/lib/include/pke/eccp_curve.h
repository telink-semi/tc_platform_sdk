/********************************************************************************************************
 * @file	eccp_curve.h
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
/********* pke version:1.1 *********/
#ifndef ECCP_CURVE_H
#define ECCP_CURVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"

//sample ecc curve
#define SUPPORT_BRAINPOOLP160R1
#define SUPPORT_SECP192R1
#define SUPPORT_SECP224R1
#define SUPPORT_SECP256R1
//#define SUPPORT_SECP384R1
//#define SUPPORT_BRAINPOOLP512R1
//#define SUPPORT_SECP521R1


//#define PKE_HP


#define POINT_NOT_COMPRESSED      (0x04)


// eccp curve struct
#ifdef PKE_HP
typedef struct
{
    unsigned int eccp_p_bitLen;        //bit length of prime p
    unsigned int eccp_n_bitLen;        //bit length of order n
    unsigned int *eccp_p;
    unsigned int *eccp_p_h;
    unsigned int *eccp_a;
    unsigned int *eccp_b;
    unsigned int *eccp_Gx;
    unsigned int *eccp_Gy;
    unsigned int *eccp_n;
    unsigned int *eccp_n_h;
    unsigned int *eccp_half_Gx;
    unsigned int *eccp_half_Gy;
} eccp_curve_t;
#else
typedef struct
{
    unsigned int eccp_p_bitLen;        //bit length of prime p
    unsigned int eccp_n_bitLen;        //bit length of order n
    unsigned int *eccp_p;              //prime p
    unsigned int *eccp_p_h;
    unsigned int *eccp_p_n1;
    unsigned int *eccp_a;
    unsigned int *eccp_b;
    unsigned int *eccp_Gx;
    unsigned int *eccp_Gy;
    unsigned int *eccp_n;              //order of curve or point(Gx,Gy)
    unsigned int *eccp_n_h;
    unsigned int *eccp_n_n1;
} eccp_curve_t;
#endif



#ifdef SUPPORT_BRAINPOOLP160R1
extern eccp_curve_t brainpoolp160r1[1];
#endif

#ifdef SUPPORT_SECP192R1
extern eccp_curve_t secp192r1[1];
#endif

#ifdef SUPPORT_SECP224R1
extern eccp_curve_t secp224r1[1];
#endif

#ifdef SUPPORT_SECP256R1
extern eccp_curve_t secp256r1[1];
#endif

#ifdef SUPPORT_SECP384R1
extern eccp_curve_t secp384r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP512R1
extern eccp_curve_t brainpoolp512r1[1];
#endif

#ifdef SUPPORT_SECP521R1
extern eccp_curve_t secp521r1[1];
#endif



/********* Curve25519 struct *********/
typedef struct
{
    unsigned int p_bitLen;        //bit length of prime p
    unsigned int *p;
    unsigned int *p_h;
    unsigned int *p_n1;
    unsigned int *a24;            //(A-2)/4
    unsigned int *u;
    unsigned int *v;
    unsigned int *n;              //order of curve or point(Gx,Gy)
    unsigned int *n_h;
    unsigned int *n_n1;
    unsigned int *h;
} mont_curve_t;


/********* Edward Curve 25519 struct *********/
typedef struct
{
    unsigned int p_bitLen;        //bit length of prime p
    unsigned int *p;
    unsigned int *p_h;
    unsigned int *p_n1;
    unsigned int *d;
    unsigned int *Gx;
    unsigned int *Gy;
    unsigned int *n;              //order of curve or point(Gx,Gy)
    unsigned int *n_h;
    unsigned int *n_n1;
    unsigned int *h;
} edward_curve_t;


#ifdef __cplusplus
}
#endif

#endif

/********************************************************************************************************
 * @file	app_ecdsa.c
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

#if (PKE_TEST_MODE == PKE_ECDSA)


//ECDSA return code
enum ECDSA_RET_CODE
{
	ECDSA_SUCCESS = PKE_SUCCESS,
	ECDSA_POINTOR_NULL = PKE_SUCCESS+0x50,
	ECDSA_INVALID_INPUT,
	ECDSA_ZERO,
	ECDSA_LARGE_INTEGER,
	ECDSA_VERIFY_FAIL,
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
 * @brief		Generate ECDSA Signature in U32 little-endian big integer style
 * @param[in]	curve	- ecc curve struct pointer, please make sure it is valid
 * @param[in]	e		- derived from hash value
 * @param[in]	k		- internal random integer k
 * @param[in]	dA		- private key
 * @param[out]	r		- signature r
 * @param[out]	s		- signature s
 * @return		ECDSA_SUCCESS(success); other(error)
 */
unsigned char ecdsa_sign4(eccp_curve_t *curve, unsigned int *e, unsigned int *k, unsigned int *dA, unsigned int *r, unsigned int *s)
{
	unsigned char ret;
	unsigned int nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
	unsigned int pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
	unsigned int tmp1[ECC_MAX_WORD_LEN];

	if(0 == curve || 0 == e || 0 == k || 0 == dA || 0 == r || 0 == s)
	{
		return ECDSA_POINTOR_NULL;
	}

	if(curve->eccp_p_bitLen > ECC_MAX_BIT_LEN)
	{
		return ECDSA_INVALID_INPUT;
	}

	//make sure k in [1, n-1]
	if(ismemzero4(k, nWordLen<<2))
	{
		return ECDSA_ZERO;
	}
	if(big_integer_compare(k, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_LARGE_INTEGER;
	}

	//get x1
	ret = pke_eccp_point_mul(curve, k, curve->eccp_Gx, curve->eccp_Gy, tmp1, 0);  //y coordinate is not needed
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//r = x1 mod n
	ret = pke_mod(tmp1, pWordLen, curve->eccp_n, curve->eccp_n_h, curve->eccp_n_n1, nWordLen, r);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//make sure r is not zero
	if(ismemzero4(r, nWordLen<<2))
	{
		return ECDSA_ZERO;
	}

	//tmp1 =  r*dA mod n
	if(curve->eccp_n_h && curve->eccp_n_n1)
	{
		pke_load_pre_calc_mont(curve->eccp_n_h, curve->eccp_n_n1, nWordLen);
		ret = pke_mod_mul(curve->eccp_n, r, dA, tmp1, nWordLen);
	}
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//tmp1 = e + r*dA mod n
	ret = pke_mod_add(curve->eccp_n, e, tmp1, tmp1, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//s = k^(-1) mod n
	ret = pke_mod_inv(curve->eccp_n, k, s, nWordLen, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//s = (k^(-1))*(e + r*dA) mod n
	ret = pke_mod_mul(curve->eccp_n, s, tmp1, s, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//make sure s is not zero
	if(ismemzero4(s, nWordLen<<2))
	{
		return ECDSA_ZERO;
	}

	return ECDSA_SUCCESS;
}
/**
 * @brief		Generate ECDSA Signature in octet string style
 * @param[in]	curve		- ecc curve struct pointer, please make sure it is valid
 * @param[in]	E			- hash value, big-endian
 * @param[in]	EByteLen	- byte length of E
 * @param[in]	priKey		- private key, big-endian
 * @param[out]	signature	- signature r and s, big-endian
 * @return		ECDSA_SUCCESS(success); other(error)
 */
unsigned char ecdsa_sign(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *rand_k, unsigned char *priKey,
				   unsigned char *signature)
{
	unsigned int tmpLen;
	unsigned int nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	unsigned int nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
	unsigned int e[ECC_MAX_WORD_LEN], k[ECC_MAX_WORD_LEN], dA[ECC_MAX_WORD_LEN];
	unsigned int r[ECC_MAX_WORD_LEN], s[ECC_MAX_WORD_LEN];
	unsigned char ret;

	if(0 == curve || 0 == E || 0 == priKey || 0 == signature)
	{
		return ECDSA_POINTOR_NULL;
	}

	if(0 == EByteLen)
	{
		return ECDSA_INVALID_INPUT;
	}

	if(curve->eccp_p_bitLen > ECC_MAX_BIT_LEN)
	{
		return ECDSA_INVALID_INPUT;
	}

    //get integer e from hash value E(according to SEC1-V2 2009)
	memset(e, 0, nWordLen<<2);
	if(curve->eccp_n_bitLen >= (EByteLen<<3)) //in this case, make E as e directly
	{
		memcpy((unsigned char *)e, (unsigned char *)E, EByteLen);
	}
	else                                     //in this case, make left eccp_n_bitLen bits of E as e
	{
		memcpy((unsigned char *)e, (unsigned char *)E, nByteLen);
		tmpLen = (curve->eccp_n_bitLen)&7;
		if(tmpLen)
		{
			div2n_u32(e, nWordLen, 8-tmpLen);
		}
	}

	//get e = e mod n, i.e., make sure e in [0, n-1]
	if(big_integer_compare(e, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		sub_u32(e, curve->eccp_n, k, nWordLen);
		memcpy(e, k, nWordLen<<2);
	}

	//make sure priKey in [1, n-1]
	memset(dA, 0, (nWordLen<<2)-nByteLen);
	memcpy((unsigned char *)dA, (unsigned char *)priKey, nByteLen);
	if(ismemzero4(dA, nWordLen<<2))
	{
		return ECDSA_ZERO;
	}
	if(big_integer_compare(dA, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_LARGE_INTEGER;
	}

	//get k
	memset(k, 0, (nWordLen<<2)-nByteLen);
	if(rand_k)
	{
		memcpy((unsigned char *)k, rand_k, nByteLen);
	}
	else
	{
ECDSA_SIGN_LOOP:

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
	}

	//sign
	ret = ecdsa_sign4(curve, e, k, dA, r, s);
	if(ECDSA_ZERO == ret || ECDSA_LARGE_INTEGER == ret)
	{
		if(rand_k)
		{
			return ret;
		}
		else
		{
			goto ECDSA_SIGN_LOOP;
		}
	}
	else if(ECDSA_SUCCESS != ret)
	{
		return ret;
	}

	memcpy(signature, (unsigned char *)r, nByteLen);
	memcpy(signature+nByteLen, (unsigned char *)s, nByteLen);

	return ECDSA_SUCCESS;
}
/**
 * @brief		Verify ECDSA Signature in octet string style
 * @param[in]	curve		- ecc curve struct pointer, please make sure it is valid
 * @param[in]	E			- hash value, big-endian
 * @param[in]	EByteLen	- byte length of E
 * @param[in]	pubKey		- public key, big-endian
 * @param[in]	signature	- signature r and s, big-endian
 * @return		ECDSA_SUCCESS(success); other(error)
 */
unsigned char ecdsa_verify(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *pubKey, unsigned char *signature)
{
	unsigned int tmpLen;
	unsigned int nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	unsigned int nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
	unsigned int pByteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
	unsigned int pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
	unsigned int maxWordLen = GET_MAX_LEN(nWordLen,pWordLen);
	unsigned int e[ECC_MAX_WORD_LEN], r[ECC_MAX_WORD_LEN], s[ECC_MAX_WORD_LEN];
	unsigned int tmp[ECC_MAX_WORD_LEN], x[ECC_MAX_WORD_LEN];
	unsigned char ret;

	if(0 == curve || 0 == E || 0 == pubKey || 0 == signature)
	{
		return ECDSA_POINTOR_NULL;
	}

	if(0 == EByteLen)
	{
		return ECDSA_INVALID_INPUT;
	}

	if(curve->eccp_p_bitLen > ECC_MAX_BIT_LEN)
	{
		return ECDSA_INVALID_INPUT;
	}

	//make sure r in [1, n-1]
	memset(r, 0, (nWordLen<<2)-nByteLen);
	memcpy((unsigned char *)r, signature, nByteLen);
	if(ismemzero4(r, nWordLen<<2))
	{
		return ECDSA_ZERO;
	}
	if(big_integer_compare(r, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_LARGE_INTEGER;
	}

	//make sure s in [1, n-1]
	memset(s, 0, (nWordLen<<2)-nByteLen);
	memcpy((unsigned char *)s, signature+nByteLen, nByteLen);
	if(ismemzero4(s, nWordLen<<2))
	{
		return ECDSA_ZERO;
	}
	if(big_integer_compare(s, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_LARGE_INTEGER;
	}

	//tmp = s^(-1) mod n
	ret = pke_mod_inv(curve->eccp_n, s, tmp, nWordLen, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

    //get integer e from hash value E(according to SEC1-V2 2009)
	memset(e, 0, nWordLen<<2);
	if(curve->eccp_n_bitLen >= (EByteLen<<3)) //in this case, make E as e directly
	{
		memcpy((unsigned char *)e, (unsigned char *)E, EByteLen);
	}
	else                                     //in this case, make left eccp_n_bitLen bits of E as e
	{
		memcpy(e, E, nByteLen);
		memcpy((unsigned char *)e, (unsigned char *)E, nByteLen);
		tmpLen = (curve->eccp_n_bitLen)&7;
		if(tmpLen)
		{
			div2n_u32(e, nWordLen, 8-tmpLen);
		}
	}

	//get e = e mod n, i.e., make sure e in [0, n-1]
	if(big_integer_compare(e, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		sub_u32(e, curve->eccp_n, x, nWordLen);
		memcpy(e, x, nWordLen<<2);
	}

	//x =  e*(s^(-1)) mod n
	if(curve->eccp_n_h && curve->eccp_n_n1)
	{
		pke_load_pre_calc_mont(curve->eccp_n_h, curve->eccp_n_n1, nWordLen);
		ret = pke_mod_mul(curve->eccp_n, e, tmp, x, nWordLen);
	}
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//tmp =  r*(s^(-1)) mod n
	ret = pke_mod_mul(curve->eccp_n, r, tmp, tmp, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	//check public key
	memset(e, 0, (maxWordLen<<2)-pByteLen);
	memset(s, 0, (maxWordLen<<2)-pByteLen);
	memcpy((unsigned char *)e, pubKey, pByteLen);
	memcpy((unsigned char *)s, pubKey+pByteLen, pByteLen);
	ret = pke_eccp_point_verify(curve, e, s);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	ret = pke_eccp_point_mul(curve, tmp, e, s, e, s);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	if(!ismemzero4(x, nWordLen<<2))
	{
		ret = pke_eccp_point_mul(curve, x, curve->eccp_Gx, curve->eccp_Gy, x, tmp);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}

		ret = pke_eccp_point_add(curve, e, s, x, tmp, e, s);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
	}

	//x = x1 mod n
	ret = pke_mod(e, pWordLen, curve->eccp_n, curve->eccp_n_h, curve->eccp_n_n1, nWordLen, tmp);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}

	if(big_integer_compare(tmp, nWordLen, r, nWordLen))
	{
		return ECDSA_VERIFY_FAIL;
	}

	return ECDSA_SUCCESS;
}
/**
 * @brief		ecdsa rand test
 * @return		ECDSA_SUCCESS(success); other(error)
 */
unsigned char ecdsa_rand_test(eccp_curve_t *curve)
{
	unsigned char priKey[32];
	unsigned char pubKey[64];
	unsigned char rand_k[32];
	unsigned char *k;
	unsigned char E[64];
	unsigned char signature[64];
	unsigned int i,j, EByteLen, pByteLen, nByteLen, signatureByteLen;
	unsigned char ret;

	pByteLen = (curve->eccp_p_bitLen+7)/8;
	nByteLen = (curve->eccp_n_bitLen+7)/8;
	signatureByteLen = nByteLen*2;

	for(i=0;i<10;i++)
	{
		ret = rand_get(signature, 4);
		if(TRNG_SUCCESS != ret)
		{
			printf("\r\n get rand error, ret = %02x", ret);
			return 1;
		}

		//random E of random length
		EByteLen = signature[0]%64+1;
		ret = rand_get(E, EByteLen);
		if(TRNG_SUCCESS != ret)
		{
			printf("\r\n get rand error, ret = %02x", ret);
			return 1;
		}

		//internal k
		if(signature[1]&1)
		{
			k = NULL;
		}
		else
		{
			k = rand_k;
			ret = rand_get(k, nByteLen);
			if(TRNG_SUCCESS != ret)
			{
				printf("\r\n get rand error, ret = %02x", ret);
				return 1;
			}
			k[nByteLen-1] = 0;
		}

		//get random key pair
		ret = eccp_getkey(curve, priKey, pubKey);
		if(ECDSA_SUCCESS != ret)
		{
			printf("\r\n ECDSA get key error, ret = %02x", ret);
			return 1;
		}

		for(j=0;j<50;j++)
		{
			printf("\r\n ECDSA round1=%d, round2=%d", i+1, j+1);

			ret = ecdsa_sign(curve, E, EByteLen, k, priKey, signature);
			if(ECDSA_SUCCESS != ret)
			{
				printf("\r\n ECDSA sign error, ret = %02x", ret);
				return 1;
			}
			else
			{
				printf("\r\n ECDSA sign success ");
			}



			ret = ecdsa_verify(curve, E, EByteLen, pubKey, signature);
			if(ECDSA_SUCCESS != ret)
			{
				printf("\r\n ECDSA verify error, ret = %02x", ret);
				return 1;
			}
			else
			{
				printf("\r\n ECDSA verify success ");
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

	ecdsa_rand_test(secp256r1);
}




void main_loop (void)
{

}

#endif




/********************************************************************************************************
 * @file	ecdsa.c
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
#include "lib/include/pke/ecdsa.h"


extern void pke_set_exe_cfg(unsigned int cfg);

/**
 * @brief		Generate ECDSA Signature in U32 little-endian big integer style.
 * @param[in]	curve	- eccp curve struct pointer, please make sure it is valid.
 * @param[in]	e		- derived from hash value.
 * @param[in]	k		- internal random integer k.
 * @param[in]	dA		- private key.
 * @param[out]	r		- signature r.
 * @param[out]	s		- signature s.
 * @return		ECDSA_SUCCESS(success), other(error).
 */
unsigned int ecdsa_sign_uint32(eccp_curve_t *curve, unsigned int *e, unsigned int *k, unsigned int *dA, unsigned int *r, unsigned int *s)
{
	unsigned int nWordLen;
	unsigned int pWordLen;
	unsigned int tmp1[ECCP_MAX_WORD_LEN];
	unsigned int ret;

	if(NULL == curve || NULL == e || NULL == k || NULL == dA || NULL == r || NULL == s)
	{
		return ECDSA_POINTOR_NULL;
	}
	else if(curve->eccp_p_bitLen > ECCP_MAX_BIT_LEN)
	{
		return ECDSA_INVALID_INPUT;
	}
	else
	{;}

	nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
	pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);

	//make sure k in [1, n-1]
	if(uint32_BigNum_Check_Zero(k, nWordLen))
	{
		return ECDSA_ZERO_ALL;
	}
	else if(uint32_BigNumCmp(k, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_INTEGER_TOO_BIG;
	}
	else
	{;}

	//get x1
	ret = eccp_pointMul(curve, k, curve->eccp_Gx, curve->eccp_Gy, tmp1, NULL);  //y coordinate is not needed
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//r = x1 mod n
	ret = pke_mod(tmp1, pWordLen, curve->eccp_n, curve->eccp_n_h, curve->eccp_n_n1, nWordLen, r);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else if(uint32_BigNum_Check_Zero(r, nWordLen))//make sure r is not zero
	{
		return ECDSA_ZERO_ALL;
	}
	else
	{;}

	//tmp1 =  r*dA mod n
	if((NULL == curve->eccp_n_h) || (NULL == curve->eccp_n_n1))
	{
		ret = pke_modmul(curve->eccp_n, r, dA, tmp1, nWordLen);
	}
	else
	{
		pke_load_pre_calc_mont(curve->eccp_n_h, curve->eccp_n_n1, nWordLen);
		pke_set_exe_cfg(PKE_EXE_CFG_ALL_NON_MONT);
		ret = pke_modmul_internal(curve->eccp_n, r, dA, tmp1, nWordLen);
	}
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//tmp1 = e + r*dA mod n
	ret = pke_modadd(curve->eccp_n, e, tmp1, tmp1, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//s = k^(-1) mod n
	ret = pke_modinv(curve->eccp_n, k, s, nWordLen, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//s = (k^(-1))*(e + r*dA) mod n
	ret = pke_modmul_internal(curve->eccp_n, s, tmp1, s, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//make sure s is not zero
	if(uint32_BigNum_Check_Zero(s, nWordLen))
	{
		return ECDSA_ZERO_ALL;
	}
	else
	{
		return ECDSA_SUCCESS;
	}
}

/**
 * @brief		Generate ECDSA Signature in octet string style.
 * @param[in]	curve		- eccp curve struct pointer, please make sure it is valid.
 * @param[in]	E			- hash value, big-endian.
 * @param[in]	EByteLen	- byte length of E.
 * @param[in]	rand_k		- random number k.
 * @param[in]	priKey		- private key, big-endian.
 * @param[out]	signature	- signature r and s, big-endian.
 * @return		ECDSA_SUCCESS(success), other(error).
 */
unsigned int ecdsa_sign(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *rand_k, unsigned char *priKey,
		unsigned char *signature)
{
	unsigned int tmpLen;
	unsigned int nByteLen;
	unsigned int nWordLen;
	unsigned int e[ECCP_MAX_WORD_LEN], k[ECCP_MAX_WORD_LEN], dA[ECCP_MAX_WORD_LEN];
	unsigned int r[ECCP_MAX_WORD_LEN], s[ECCP_MAX_WORD_LEN];
	unsigned int ret;

	if(NULL == curve || NULL == priKey || NULL == signature)
	{
		return ECDSA_POINTOR_NULL;
	}
	else if(curve->eccp_p_bitLen > ECCP_MAX_BIT_LEN)
	{
		return ECDSA_INVALID_INPUT;
	}
	else
	{;}

	//E could be zero
	if(NULL == E)
	{
		EByteLen = 0;
	}
	else
	{;}

	nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

	//get integer e from hash value E(according to SEC1-V2 2009)
	uint32_clear(e, nWordLen);
	if(curve->eccp_n_bitLen >= (EByteLen<<3)) //in this case, make E as e directly
	{
		reverse_byte_array((unsigned char *)E, (unsigned char *)e, EByteLen);
	}
	else                                      //in this case, make left eccp_n_bitLen bits of E as e
	{
		reverse_byte_array((unsigned char *)E, (unsigned char *)e, nByteLen);
		tmpLen = (curve->eccp_n_bitLen)&7;
		if(tmpLen)
		{
			Big_Div2n(e, nWordLen, 8-tmpLen);
		}
		else
		{;}
	}

	//get e = e mod n, i.e., make sure e in [0, n-1]
	if(uint32_BigNumCmp(e, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		ret = pke_sub(e, curve->eccp_n, e, nWordLen);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}
	}
	else
	{;}

	//make sure priKey in [1, n-1]
	memset(((unsigned char *)dA)+nByteLen, 0, (nWordLen<<2)-nByteLen);
	reverse_byte_array((unsigned char *)priKey, (unsigned char *)dA, nByteLen);
	if(uint32_BigNum_Check_Zero(dA, nWordLen))
	{
		return ECDSA_ZERO_ALL;
	}
	else if(uint32_BigNumCmp(dA, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_INTEGER_TOO_BIG;
	}
	else
	{;}

	//get k
	memset(((unsigned char *)k)+nByteLen, 0, (nWordLen<<2)-nByteLen);
	if(rand_k)
	{
		reverse_byte_array(rand_k, (unsigned char *)k, nByteLen);
	}
	else
	{
ECDSA_SIGN_LOOP:

		ret = rand_get((unsigned char *)k, nByteLen);
		if(TRNG_SUCCESS != ret)
		{
			return ret;
		}
		else
		{
			//make sure k has the same bit length as n
			tmpLen = (curve->eccp_n_bitLen)&0x1F;
			if(tmpLen)
			{
				k[nWordLen-1] &= (1<<(tmpLen))-1;
			}
			else
			{;}
		}
	}

	//sign
	ret = ecdsa_sign_uint32(curve, e, k, dA, r, s);
	if((ECDSA_ZERO_ALL == ret || ECDSA_INTEGER_TOO_BIG == ret) && (NULL == rand_k))
	{
		goto ECDSA_SIGN_LOOP;
	}
	else
	{;}

	if(ECDSA_SUCCESS != ret)
	{
		return ret;
	}
	else
	{
		reverse_byte_array((unsigned char *)r, signature, nByteLen);
		reverse_byte_array((unsigned char *)s, signature+nByteLen, nByteLen);

		return ECDSA_SUCCESS;
	}
}

/**
 * @brief		Verify ECDSA Signature in octet string style.
 * @param[in]	curve		- eccp curve struct pointer, please make sure it is valid.
 * @param[in]	E			- hash value, big-endian.
 * @param[in]	EByteLen	- byte length of E.
 * @param[in]	pubKey		- public key, big-endian.
 * @param[in]	signature	- signature r and s, big-endian.
 * @return		ECDSA_SUCCESS(success), other(error).
 */
unsigned int ecdsa_verify(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *pubKey, unsigned char *signature)
{
	unsigned int tmpLen;
	unsigned int nByteLen;
	unsigned int nWordLen;
	unsigned int pByteLen;
	unsigned int pWordLen;
	unsigned int maxWordLen;
	unsigned int e[ECCP_MAX_WORD_LEN], r[ECCP_MAX_WORD_LEN], s[ECCP_MAX_WORD_LEN];
	unsigned int tmp[ECCP_MAX_WORD_LEN], x[ECCP_MAX_WORD_LEN];
	unsigned int ret;

	if(NULL == curve || NULL == pubKey || NULL == signature)
	{
		return ECDSA_POINTOR_NULL;
	}
	else if(curve->eccp_p_bitLen > ECCP_MAX_BIT_LEN)
	{
		return ECDSA_INVALID_INPUT;
	}
	else
	{;}

	//E could be zero
	if(NULL == E)
	{
		EByteLen = 0;
	}
	else
	{;}

	nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
	pByteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
	pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
	maxWordLen = GET_MAX_LEN(nWordLen,pWordLen);

	//make sure r in [1, n-1]
	memset(((unsigned char *)r)+nByteLen, 0, (nWordLen<<2)-nByteLen);
	reverse_byte_array(signature, (unsigned char *)r, nByteLen);
	if(uint32_BigNum_Check_Zero(r, nWordLen))
	{
		return ECDSA_ZERO_ALL;
	}
	else if(uint32_BigNumCmp(r, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_INTEGER_TOO_BIG;
	}
	else
	{;}

	//make sure s in [1, n-1]
	memset(((unsigned char *)s)+nByteLen, 0, (nWordLen<<2)-nByteLen);
	reverse_byte_array(signature+nByteLen, (unsigned char *)s, nByteLen);
	if(uint32_BigNum_Check_Zero(s, nWordLen))
	{
		return ECDSA_ZERO_ALL;
	}
	else if(uint32_BigNumCmp(s, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		return ECDSA_INTEGER_TOO_BIG;
	}
	else
	{;}

	//tmp = s^(-1) mod n
	ret = pke_modinv(curve->eccp_n, s, tmp, nWordLen, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

    //get integer e from hash value E(according to SEC1-V2 2009)
	uint32_clear(e, nWordLen);
	if(curve->eccp_n_bitLen >= (EByteLen<<3)) //in this case, make E as e directly
	{
		reverse_byte_array((unsigned char *)E, (unsigned char *)e, EByteLen);
	}
	else                                      //in this case, make left eccp_n_bitLen bits of E as e
	{
		memcpy(e, E, nByteLen);
		reverse_byte_array((unsigned char *)E, (unsigned char *)e, nByteLen);
		tmpLen = (curve->eccp_n_bitLen)&7;
		if(tmpLen)
		{
			Big_Div2n(e, nWordLen, 8-tmpLen);
		}
		else
		{;}
	}

	//get e = e mod n, i.e., make sure e in [0, n-1]
	if(uint32_BigNumCmp(e, nWordLen, curve->eccp_n, nWordLen) >= 0)
	{
		ret = pke_sub(e, curve->eccp_n, e, nWordLen);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}
	}
	else
	{;}

	//x =  e*(s^(-1)) mod n
	if((NULL == curve->eccp_n_h) || (NULL == curve->eccp_n_n1))
	{
		ret = pke_modmul(curve->eccp_n, e, tmp, x, nWordLen);
	}
	else
	{
		pke_load_pre_calc_mont(curve->eccp_n_h, curve->eccp_n_n1, nWordLen);
		pke_set_exe_cfg(PKE_EXE_CFG_ALL_NON_MONT);
		ret = pke_modmul_internal(curve->eccp_n, e, tmp, x, nWordLen);
	}
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//tmp =  r*(s^(-1)) mod n
	ret = pke_modmul_internal(curve->eccp_n, r, tmp, tmp, nWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//check public key
	memset(e, 0, (maxWordLen<<2)-pByteLen);
	memset(s, 0, (maxWordLen<<2)-pByteLen);
	reverse_byte_array(pubKey, (unsigned char *)e, pByteLen);
	reverse_byte_array(pubKey+pByteLen, (unsigned char *)s, pByteLen);
	ret = eccp_pointVerify(curve, e, s);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	ret = eccp_pointMul(curve, tmp, e, s, e, s);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	if(!uint32_BigNum_Check_Zero(x, nWordLen))
	{
		ret = eccp_pointMul(curve, x, curve->eccp_Gx, curve->eccp_Gy, x, tmp);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}

		ret = eccp_pointAdd(curve, e, s, x, tmp, e, s);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}
	}
	else
	{;}

	//x = x1 mod n
	ret = pke_mod(e, pWordLen, curve->eccp_n, curve->eccp_n_h, curve->eccp_n_n1, nWordLen, tmp);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	if(uint32_BigNumCmp(tmp, nWordLen, r, nWordLen))
	{
		return ECDSA_VERIFY_FAILED;
	}
	else
	{
		return ECDSA_SUCCESS;
	}
}


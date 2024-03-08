/********************************************************************************************************
 * @file	rsa.c
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
#include "lib/include/pke/pke_prime.h"
#include "lib/include/pke/rsa.h"


extern void pke_set_operand_width(unsigned int bitLen);
extern unsigned int pke_get_operand_bytes(void);
extern unsigned int pke_pre_calc_mont(const unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n1);

/**
 * @brief		out = a^e mod n.
 * @param[in]	a		- unsigned int big integer a, base number, make sure a < n.
 * @param[in]	e		- unsigned int big integer e, exponent, make sure e < n.
 * @param[in]	n		- unsigned int big integer n, modulus, make sure n is odd.
 * @param[out]	out		- out = a^e mod n.
 * @param[in]	eBitLen	- real bit length of unsigned int big integer e.
 * @param[in]	nBitLen	- real bit length of unsigned int big integer n.
 * @return		RSA_SUCCESS(success), other(error).
 * @caution		a, n, and out have the same word length:((nBitLen+31)>>5); and e word length is (eBitLen+31)>>5
 */
unsigned int RSA_ModExp(unsigned int *a, unsigned int *e, unsigned int *n, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen)
{
	unsigned int eWordLen = GET_WORD_LEN(eBitLen);
	unsigned int nWordLen = GET_WORD_LEN(nBitLen);
	unsigned int ret;

	if(NULL == a || NULL == e || NULL == n || NULL == out)
	{
		return RSA_BUFFER_NULL;
	}
	else if(nBitLen > MAX_RSA_BIT_LEN || eBitLen > nBitLen)
	{
		return RSA_INPUT_TOO_LONG;
	}
	else if(!(n[0] & 1))
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	//a should be less than n
	if(uint32_BigNumCmp(a, nWordLen, n, nWordLen) >= 0)
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	//0^0 mod n
	if(uint32_BigNum_Check_Zero(a, nWordLen) && uint32_BigNum_Check_Zero(e, eWordLen))
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	ret = pke_pre_calc_mont(n, nBitLen, NULL, NULL);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	return pke_modexp(n, e, a, out, nWordLen, eWordLen);
}

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
 * @caution        a and out have the same word length:((nBitLen+31)>>5); and p,p_h,q,q_h,dp,dq,u have the same word length:((nBitLen/2+31)>>5).
 */
unsigned int RSA_CRTModExp(unsigned int *a, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int*dq, unsigned int *u,
		unsigned int *out,  unsigned int nBitLen)
{
	unsigned int buf[MAX_RSA_WORD_LEN];
	unsigned int *m1 = buf;
	unsigned int *m2 = buf+(MAX_RSA_WORD_LEN/2);

	unsigned int tmp_step;
	unsigned int nWordLen = GET_WORD_LEN(nBitLen);
	unsigned int pBitLen = nBitLen/2;
	unsigned int pWordLen = GET_WORD_LEN(pBitLen);
	unsigned int ret;

	if(NULL == a || NULL == p || NULL == q || NULL == dp || NULL == dq
	   || NULL == u || NULL == out)
	{
		return RSA_BUFFER_NULL;
	}
	else if(nBitLen > MAX_RSA_BIT_LEN)
	{
		return RSA_INPUT_TOO_LONG;
	}
	else
	{;}

	//a should be less than n
	ret = pke_mul(p, q, buf, pWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else if(uint32_BigNumCmp(a, nWordLen, buf, nWordLen) >= 0)
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	//store the nBitLen step
	pke_set_operand_width(pBitLen);
	tmp_step = pke_get_operand_bytes();

	//m2 = (a) mod q
	ret = pke_pre_calc_mont(q, pBitLen, NULL, NULL);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	ret = pke_mod(a, nWordLen, q, (unsigned int *)(PKE_A(3,tmp_step)), (unsigned int *)(PKE_B(4,tmp_step)), pWordLen, m2);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//m2 = (a)^dq mod q
	ret = pke_modexp(q, dq, m2, m2, pWordLen, pWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//m1 = (a) mod p
	ret = pke_pre_calc_mont(p, pBitLen, NULL, NULL);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	ret = pke_mod(a, nWordLen, p, (unsigned int *)(PKE_A(3,tmp_step)), (unsigned int *)(PKE_B(4,tmp_step)), pWordLen, m1);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//m1 = (a)^dp mod p
	ret = pke_modexp(p, dp, m1, m1, pWordLen, pWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//m2 = m2 mod p
	if(uint32_BigNumCmp(m2, pWordLen, p, pWordLen) > 0)
	{
		ret = pke_sub(m2, p, m2, pWordLen);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}
	}
	else
	{;}

	//m1 = (m1-m2) mod p
	ret = pke_modsub(p, m1, m2, m1, pWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//m1 = h = u*(m1-m2) mod p
#if 0
	ret = pke_modmul_internal(p, m1, u, m1, pWordLen);
#else
	ret = pke_modmul(p, m1, u, m1, pWordLen);
#endif
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//store the nBitLen step
	pke_set_operand_width(nBitLen);
	tmp_step = pke_get_operand_bytes();

	//A1 = hq
	ret = pke_mul(m1, q, (unsigned int *)(PKE_A(1,tmp_step)), pWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//out = m2+hq
	uint32_copy((unsigned int *)(PKE_B(1,tmp_step)), m2, pWordLen);
	uint32_clear((unsigned int *)(PKE_B(1,tmp_step))+pWordLen, nWordLen-pWordLen);
	return pke_add((unsigned int *)(PKE_A(1,tmp_step)), (unsigned int *)(PKE_B(1,tmp_step)), out, nWordLen);
}

/**
 * @brief       get big odd integer e of eBitLen.
 * @param[in]   e       - unsigned int big odd integer e.
 * @param[in]   eBitLen - bit length of unsigned int big odd integer e.
 * @return   	0(success), 1(error: eBitLen<2).
 * @caution        eBitLen must be big than 1.
 */
unsigned int RSA_Get_E1(unsigned int e[], unsigned int eBitLen)
{
	unsigned int eWordLen = (eBitLen+0x1F)>>5;
	unsigned int ret;

	if(eBitLen<2)
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	ret = rand_get((unsigned char *)e, eWordLen<<2);
	if(TRNG_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	eBitLen &= 31;

	if(eBitLen)
	{
#if 0
		e[eWordLen - 1] <<= (32 - eBitLen);
		e[eWordLen - 1] |= 0x80000000;
		e[eWordLen - 1] >>= (32 - eBitLen);
#else
		e[eWordLen - 1] &= (1<<(eBitLen))-1;
		e[eWordLen - 1] |= 1<<(eBitLen - 1);
#endif
	}
	else
	{
		e[eWordLen - 1] |= 0x80000000;
	}

	e[0] |= 0x01;          //make e odd

	return 0;
}

/**
 * @brief       get big odd integer e of eBitLen, satisfies e < fai_n of bitLen.
 * @param[in]   e      - unsigned int big odd integer e.
 * @param[in]   fai_n  - unsigned int big even integer fai_n.
 * @param[in]   bitLen - bit length of unsigned int big odd integer e and n.
 * @return   	0(success), 1(error: bitLen<66), 2(error, n is 1000000000...000000).
 * @caution     1. eBitLen must be big than 65.
 *              2. n can not be 1000000000...000000.
 */
unsigned int RSA_Get_E2(unsigned int e[], unsigned int fai_n[], unsigned int bitLen)
{
	unsigned int wordLen;
	int i;
	unsigned char j;

	if(bitLen < 66)
		return 1;

	RSA_Get_E1(e, bitLen);
	wordLen = (bitLen+0x1F)>>5;
	j = bitLen&31;                   // namely j = eBitLen%32;
	if(j==0)
	{
		j = 32;
	}                                //at this time, j represents the bit length of the highest word of e or phi_n
	j--;                             //at this time, j indicates that the second highest bit of e or phi_n is in the position
									 //of the highest word. If it is 0, the second highest word will be reached.
	i = wordLen - 1;                 //i represents the highest word position

	if(j==0)                         //j is 0, then the second highest word
	{
		i--;
		j=32;
	}

	while(i>=0)
	{

		e[i] &= (~(1<<(j-1)));
		if(uint32_BigNumCmp(e, i+1, fai_n, i+1) < 0)	   //if e < n
		{
			return 0;
		}

		j--;
		if(0 == j)                     //j is 0, then go to the next word
		{
			i--;
			j=32;
		}
	}

	return 2;		  //fail, because n is 1000000000...000000
}

/**
 * @brief       judge whether big integer a is equal to 0x5a5a5a5a5a...5a or not.
 * @param[in]   a       - unsigned int big integer a.
 * @param[in]   aBitLen - real bit length of a.
 * @return   	0(a==0x5a5a5a5a5a...5a), 1(a!=0x5a5a5a5a5a...5a).
 * @caution     1. aBitLen can not be 0.
 *              2. if aBitLen%32 != 0, then the highest word of a should be 0.
 */
unsigned int CheckValue_0x5a5a5a5a(unsigned int a[], unsigned int aBitLen)
{
	unsigned int i, wordLen = aBitLen>>5;

	if(aBitLen & 0x1F)
	{
		if(a[wordLen] != 0)
		{
			return 1;
		}
		else
		{;}
	}
	else
	{;}

	for(i=0; i<wordLen; i++)
	{
		if(a[i] != 0x5a5a5a5a)
		{
			return 1;
		}
		else
		{;}
	}

	return 0;
}

/**
 * @brief       generate RSA key (e,d,n).
 * @param[in]   eBitLen - real bit length of e.
 * @param[in]   nBitLen - real bit length of n.
 * @param[out]	e       - unsigned int big integer, RSA public key e.
 * @param[out]	d       - unsigned int big integer, RSA private key d.
 * @param[out]	n       - unsigned int big integer, RSA public module n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @caution     1. nBitLen can not be even.
 *              2. eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
unsigned int RSA_GetKey(unsigned int *e, unsigned int *d, unsigned int *n, unsigned int eBitLen, unsigned int nBitLen)
{
	unsigned int buf[MAX_RSA_WORD_LEN];
	unsigned int *p, *q, *in, *out;
	unsigned int pBitLen, pWordLen, eWordLen, nWordLen, tmpLen;
	unsigned int count,flag;

	if(NULL == e || NULL == d || NULL == n)
	{
		return RSA_BUFFER_NULL;
	}
	else if(nBitLen&1 || nBitLen < MIN_RSA_BIT_LEN || nBitLen > MAX_RSA_BIT_LEN)  //nBitLen can not be odd
	{
		return RSA_INPUT_INVALID;
	}
	else if(eBitLen<2 || eBitLen>nBitLen)
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	p = buf;
	q = buf+MAX_RSA_WORD_LEN/2;

	pke_set_operand_width(nBitLen);
	tmpLen = pke_get_operand_bytes();
	in = (unsigned int *)(PKE_B(1,tmpLen));
	out = (unsigned int *)(PKE_A(2,tmpLen));

	eWordLen = GET_WORD_LEN(eBitLen);
	nWordLen = GET_WORD_LEN(nBitLen);
	pBitLen = nBitLen>>1;
	pWordLen = GET_WORD_LEN(pBitLen);

GET_PQ:
	flag = get_prime(p, pBitLen);
	if(flag)
	{
		return flag;
	}
	else
	{;}

	flag = get_prime(q, pBitLen);
	if(flag)
	{
		return flag;
	}
	else
	{;}

	p[0]--;                                            // p=p-1
	q[0]--;                                            // q=q-1
	pke_mul(p, q, n, pWordLen);                        // get fai(n)=(p-1)(q-1)

	count = 0;
GET_E:
	count++;
	if(count == 7)
	{
		goto GET_PQ;
	}
	else
	{;}

	switch(eBitLen)
	{
		case 2 :  {e[0] = 3; break;}
		case 5 :  {e[0] = 17; break;}
		case 17:  {e[0] = 65537; break;}
		default:
		{
			if(eBitLen == nBitLen)
			{
				flag = RSA_Get_E2(e, n, eBitLen);
				if(flag)
				{
					return flag;
				}
				else
				{;}
			}
			else
			{
				flag = RSA_Get_E1(e, eBitLen);
				if(flag)
				{
					return flag;
				}
				else
				{;}
			}
			break;
		}
	}

	//get d = e^(-1) mod n
	flag = pke_modinv(n, e, d, nWordLen, eWordLen);
	if(PKE_NO_MODINV == flag)                          //if d doesn't exist
	{
		if(eBitLen==2 || eBitLen==5 || eBitLen==17)    //if e is prime, and e divide fai(n)
		{
			goto GET_PQ;
		}
		else                                           //1. e is prime, and e divide fai(n) 2.e is not prime, and
		{                                              //e, fai(n) have common divisor.
			goto GET_E;
		}
	}
	else if(PKE_SUCCESS != flag)
	{
		return flag;
	}
	else
	{;}

	//get n = pq
	p[0]++;
	q[0]++;
	flag = pke_mul(p, q, n, pWordLen);
	if(PKE_SUCCESS != flag)
	{
		return flag;
	}
	else
	{;}

	//Encryption test
	if(nBitLen & 0x1F)
	{
		in[nWordLen-1]=0;
	}
	else
	{;}

	tmpLen = nBitLen>>5;
	uint32_set(in, 0x5a5a5a5a, tmpLen);

	flag = pke_pre_calc_mont(n, nBitLen, NULL, NULL);
	if(PKE_SUCCESS != flag)
	{
		return flag;
	}
	else
	{;}

	flag = pke_modexp(n, e, in, out, nWordLen, eWordLen);
	if(PKE_SUCCESS != flag)
	{
		return flag;
	}
	else
	{;}

	flag = pke_modexp(n, d, out, out, nWordLen, nWordLen);
	if(PKE_SUCCESS != flag)
	{
		return flag;
	}
	else
	{;}

	if(CheckValue_0x5a5a5a5a(out, nBitLen))
	{
		goto GET_PQ;
	}
	else
	{
		return RSA_SUCCESS;
	}
}

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
 * @Caution     1. nBitLen can not be even.
 *              2. eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
unsigned int RSA_GetCRTKey(unsigned int *e, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u,
		unsigned int *n, unsigned int eBitLen, unsigned int nBitLen)
{
	unsigned int buf[MAX_RSA_WORD_LEN];
	unsigned int pBitLen, pWordLen, eWordLen, nWordLen, i, wordLen;
	int count;
	unsigned int ret;

	if(NULL == e || NULL == p || NULL == q || NULL == dp || NULL == dq || NULL == u || NULL == n)
	{
		return RSA_BUFFER_NULL;
	}
	else if(nBitLen&1 || nBitLen<MIN_RSA_BIT_LEN || nBitLen > MAX_RSA_BIT_LEN)  //nBitLen can not be odd
	{
		return RSA_INPUT_INVALID;
	}
	else if(eBitLen<2 || eBitLen>nBitLen)
	{
		return RSA_INPUT_INVALID;
	}
	else
	{;}

	eWordLen = GET_WORD_LEN(eBitLen);
	nWordLen = GET_WORD_LEN(nBitLen);
	pBitLen = nBitLen>>1;
	pWordLen = GET_WORD_LEN(pBitLen);

GET_PQ:
	ret = get_prime(p, pBitLen);
	if(ret)
	{
		return ret;
	}
	else
	{;}

	ret = get_prime(q, pBitLen);
	if(ret)
	{
		return ret;
	}
	else
	{;}

	count = uint32_BigNumCmp(p, pWordLen, q, pWordLen);         // make p > q, for get u = q^(-1) mod p convenient
	if(count == -1)
	{
		for(i=0; i<pWordLen; i++)
		{
			wordLen = p[i];
			p[i] = q[i];
			q[i] = wordLen;
		}
	}
	else if(count == 0)
	{
		goto GET_PQ;
	}
	else
	{;}

	p[0]--;                                                // p=p-1
	q[0]--;                                                // q=q-1
	if(eBitLen == nBitLen)
	{
		ret = pke_mul(p, q, n, pWordLen); // get fai(n)=(p-1)(q-1)
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}
	}
	else
	{;}

	count = 0;
GET_E:
	count++;
	if(count == 7)
	{
		goto GET_PQ;
	}
	else
	{;}

	switch(eBitLen)
	{
		case 2 :  {e[0] = 3; break;}
		case 5 :  {e[0] = 17; break;}
		case 17:  {e[0] = 65537; break;}
		default:
		{
			if(eBitLen == nBitLen)
			{
				ret = RSA_Get_E2(e, n, eBitLen);
				if(ret)
				{
					return ret;
				}
				else
				{;}
			}
			else
			{
				ret = RSA_Get_E1(e, eBitLen);
				if(ret)
				{
					return ret;
				}
				else
				{;}
			}
			break;
		}
	}

	// dp = e^(-1) mod (p-1)
	if(uint32_BigNumCmp(e, eWordLen, p, pWordLen) > 0)
	{
		ret = pke_mod(e, eWordLen, p, NULL, NULL, pWordLen, u);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}

		wordLen = pWordLen;
	}
	else
	{
		uint32_copy(u, e, eWordLen);
		wordLen = eWordLen;
	}

	ret = pke_modinv(p, u, dp, pWordLen, wordLen);
	if(PKE_NO_MODINV == ret)
	{
		if(eBitLen==2 || eBitLen==5 || eBitLen==17)    //if e is prime, and e divide fai(n)
		{
			goto GET_PQ;
		}
		else                                           //1. e is prime, and e divide fai(n) 2.e is not prime, and
		{                                              //e, fai(n) have common divisor.
			goto GET_E;
		}
	}
	else if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	// dq = e^(-1) mod (q-1)
	if(uint32_BigNumCmp(e, eWordLen, q, pWordLen) > 0)
	{
		ret = pke_mod(e, eWordLen, q, NULL, NULL, pWordLen, u);
		if(PKE_SUCCESS != ret)
		{
			return ret;
		}
		else
		{;}

		wordLen = pWordLen;
	}
	else
	{
		uint32_copy(u, e, eWordLen);
		wordLen = eWordLen;
	}

	ret = pke_modinv(q, u, dq, pWordLen, wordLen);
	if(PKE_NO_MODINV == ret)
	{
		if(eBitLen==2 || eBitLen==5 || eBitLen==17)    //if e is prime, and e divide fai(n)
		{
			goto GET_PQ;
		}
		else                                           //1. e is prime, and e divide fai(n) 2.e is not prime, and
		{                                              //e, fai(n) have common divisor.
			goto GET_E;
		}
	}
	else if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	p[0]++;
	q[0]++;

	// u = q^(-1) mod p
	ret = pke_modinv(p, q, u, pWordLen, pWordLen);
	if(PKE_NO_MODINV == ret)
	{
		goto GET_PQ;
	}
	else if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	// get n
	ret = pke_mul(p, q, n, pWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	//Encryption test
	if(nBitLen & 0x1F)
	{
		buf[nWordLen-1]=0;
	}
	else
	{;}

	wordLen = nBitLen>>5;
	uint32_set(buf, 0x5a5a5a5a, wordLen);

	ret = pke_pre_calc_mont(n, nBitLen, NULL, NULL);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	ret = pke_modexp(n, e, buf, buf, nWordLen, eWordLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	if(!CheckValue_0x5a5a5a5a(buf, nBitLen))
	{
		goto GET_PQ;
	}
	else
	{;}

	ret = RSA_CRTModExp(buf, p, q, dp, dq, u, buf, nBitLen);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	if(CheckValue_0x5a5a5a5a(buf, nBitLen))
	{
		goto GET_PQ;
	}
	else
	{
		return RSA_SUCCESS;
	}
}


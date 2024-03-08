/********************************************************************************************************
 * @file	pke_utility.c
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
#include "lib/include/pke/pke_utility.h"


/**
 * @brief       set uint32 buffer.
 * @param[out]	a       - output word buffer.
 * @param[in]   value   - input word value.
 * @param[in]   wordLen - word length of buffer a.
 * @return   	none.
 */
void uint32_set(unsigned int *a, unsigned int value, unsigned int wordLen)
{
	while(wordLen)
	{
		a[--wordLen] = value;
	}
}

/**
 * @brief       copy uint32 buffer.
 * @param[out]	dst     - output word buffer.
 * @param[in]   src   	- input word value.
 * @param[in]   wordLen - word length of buffer dst or src.
 * @return   	none.
 */
void uint32_copy(unsigned int *dst, unsigned int *src, unsigned int wordLen)
{
	unsigned int i;

	if(dst != src)
	{
		for(i=0; i<wordLen; i++)
		{
			dst[i] = src[i];
		}
	}
}

/**
 * @brief       	clear uint32 buffer.
 * @param[in&out]   a   	 - word buffer a.
 * @param[in]   	aWordLen - word length of buffer a.
 * @return   		none.
 */
void uint32_clear(unsigned int *a, unsigned int wordLen)
{
	while(wordLen)
	{
		a[--wordLen] = 0;
	}
}

/**
 * @brief       sleep for a while.
 * @param[in]   count - count value.
 * @return   	none.
 */
void uint32_sleep(unsigned int count)
{
	volatile unsigned int i=0;

	while(i<count)
	{
		i++;
	}
}


#ifdef PKE_BIG_ENDIAN
/**
 * @brief       reverse word array.
 * @param[in]	in     	- input buffer.
 * @param[out]  out   	- output buffer.
 * @param[in]   wordLen - word length of in or out.
 * @return   	none.
 * @caution		in and out could point the same buffer.
 */
void reverse_word_array(unsigned char *in, unsigned int *out, unsigned int wordLen)
{
    unsigned int idx, round = wordLen >> 1;
	unsigned int tmp;
	unsigned int *p_in;

	if(((unsigned int)(in))&3)
	{
		memcpy(out, in, wordLen<<2);
		p_in = out;
	}
	else
	{
		p_in = (unsigned int *)in;
	}

    for (idx = 0; idx < round; idx++)
	{
        tmp = p_in[idx];
        out[idx] = p_in[wordLen - 1 - idx];
        out[wordLen - 1 - idx] = tmp;
    }

    if ((wordLen & 0x1) && (p_in != out))
	{
        out[round] = p_in[round];
    }
}
#else

/**
 * @brief       reverse byte array.
 * @param[in]	in     	- input buffer.
 * @param[out]  out   	- output buffer.
 * @param[in]   byteLen - byte length of in or out.
 * @return   	none.
 * @caution		in and out could point the same buffer.
 */
void reverse_byte_array(unsigned char *in, unsigned char *out, unsigned int byteLen)
{
    unsigned int idx, round = byteLen >> 1;
	unsigned char tmp;

    for (idx = 0; idx < round; idx++)
	{
        tmp = in[idx];
        out[idx] = in[byteLen - 1 - idx];
        out[byteLen - 1 - idx] = tmp;
    }

    if ((byteLen & 0x1) && (in != out))
	{
        out[round] = in[round];
    }
}
#endif

/**
 * @brief       reverse byte order in every unsigned int word.
 * @param[in]	in     	- input byte buffer.
 * @param[out]  out   	- output word buffer.
 * @param[in]   byteLen - byte length of buffer in or out.
 * @return   	none.
 * @caution		byteLen must be multiple of 4.

void reverse_word(unsigned char *in, unsigned char *out, unsigned int bytelen)
{
	unsigned int i, len;
	unsigned char tmp;
	unsigned char *p = in;

	if(in == out)
	{
		while(bytelen>0)
		{
			tmp=*p;
			*p=*(p+3);
			*(p+3)=tmp;
			p+=1;
			tmp=*p;
			*p=*(p+1);
			*(p+1)=tmp;
			bytelen-=4;
			p+=3;
		}
	}
	else
	{
	    for (i = 0; i < bytelen; i++)
		{
			len = i >> 2;
			len = len << 3;
			out[i] = p[len + 3 - i];
		}
    }
}*/


/**
 * @brief       reverse word order.
 * @param[in]	in     		 - input word buffer.
 * @param[out]  out   		 - output word buffer.
 * @param[in]   wordLen 	 - word length of buffer in or out.
 * @param[in]   reverse_word - whether to reverse byte order in every word, 0:no, other:yes.
 * @return   	none.
 * @caution		in DAM mode, the memory may be accessed by words, not by bytes, this function is designed for the case

void dma_reverse_word_array(unsigned int *in, unsigned int *out, unsigned int wordLen, unsigned int reverse_word)
{
	unsigned int i, j;
	unsigned int tmp;
	unsigned int *p=out;

	if(in == out)
	{
		for(i=0; i<wordLen; i+=4)
		{
			for (j = 0; j < 2; j++)
			{
				tmp = p[j];
				p[j] = p[4 - 1 - j];
				p[4 - 1 - j] = tmp;
			}
			p+=4;
		}
	}
	else
	{
		for(i=0; i<wordLen; i+=4)
		{
			p[0] = in[3];
			p[1] = in[2];
			p[2] = in[1];
			p[3] = in[0];
			p+=4;
			in+=4;
		}
	}

	if(reverse_word)
	{
		for (i = 0; i < wordLen; i++)
		{
			tmp = *out;
			*out = tmp&0xFF;
			*out <<= 8;
			*out |= (tmp>>8)&0xFF;
			*out <<= 8;
			*out |= (tmp>>16)&0xFF;
			*out <<= 8;
			*out |= (tmp>>24)&0xFF;

			out++;
		}
	}
	else
	{;}
} */


/**
 * @brief       C = A XOR B.
 * @param[in]   A     	- byte buffer a.
 * @param[in]   B     	- byte buffer b.
 * @param[out]  C 		- C = A XOR B.
 * @param[in]   byteLen - byte length of A,B,C.
 * @return   	none.
 */
void uint8_XOR(unsigned char *A, unsigned char *B, unsigned char *C, unsigned int byteLen)
{
	unsigned int i;

	for(i=0; i<byteLen; i++)
	{
		C[i] = A[i] ^ B[i];
	}
}

/**
 * @brief       get real bit length of big number a of wordLen words.
 * @param[in]   a		- word buffer a.
 * @param[in]   wordLen	- word length of buffer a.
 * @return   	none.
 */
unsigned int get_valid_bits(const unsigned int *a, unsigned int wordLen)
{
    unsigned int i = 0;
    unsigned int j = 0;

	if(0 == wordLen)
	{
		return 0;
	}

    for (i = wordLen; i > 0; i--)
    {
        if (a[i - 1])
        {
            break;
        }
    }

	if(0 == i)
	{
		return 0;
	}

    for (j = 32; j > 0; j--)
    {
        if (a[i - 1] & (((unsigned int)0x1) << (j - 1)))
        {
            break;
        }
    }

    return ((i - 1) << 5) + j;
}

/**
 * @brief       get real word length of big number a of max_words words.
 * @param[in]   a         - big integer a.
 * @param[in]   max_words - max word length of a.
 * @return   	real word length of big number a.
 */
unsigned int get_valid_words(unsigned int *a, unsigned int max_words)
{
    unsigned int i;

    for (i = max_words; i > 0; i--)
    {
        if (a[i - 1])
        {
            return i;
        }
    }

    return 0;
}

/**
 * @brief       check whether big number or unsigned char buffer a is all zero or not.
 * @param[in]   a         - byte buffer a.
 * @param[in]   aByteLen  - byte length of a.
 * @return   	0(a is not zero), 1(a is all zero).
 */
unsigned char uint8_BigNum_Check_Zero(unsigned char a[], unsigned int aByteLen)
{
	unsigned int i;

	for(i=0; i<aByteLen; i++)
	{
		if(a[i])
		{
			return 0;
		}
	}

	return 1;
}

/**
 * @brief       check whether big number or uint32_t buffer a is all zero or not.
 * @param[in]   a        - big integer or word buffer a.
 * @param[in]   aWordLen - word length of a.
 * @return   	0(a is not zero), 1(a is all zero).
 */
unsigned char uint32_BigNum_Check_Zero(unsigned int a[], unsigned int aWordLen)
{
	unsigned int i;

	for(i=0; i<aWordLen; i++)
	{
		if(a[i])
		{
			return 0;
		}
	}

	return 1;
}

/**
 * @brief       compare big integer a and b.
 * @param[in]   a 		 - big integer a.
 * @param[in]   aWordLen - word length of a.
 * @param[in]   b 	 	 - big integer b.
 * @param[in]   bWordLen - word length of b.
 * @return   	0:a=b,   1:a>b,   -1: a<b.
 */
int uint32_BigNumCmp(unsigned int *a, unsigned int aWordLen, unsigned int *b, unsigned int bWordLen)
{
	int i;

	aWordLen = get_valid_words(a, aWordLen);
	bWordLen = get_valid_words(b, bWordLen);

	if(aWordLen > bWordLen)
	{
		return 1;
	}

	if(aWordLen < bWordLen)
	{
		return -1;
	}

	for(i=(aWordLen-1);i>=0;i--)
	{
		if(a[i] > b[i])
		{
			return 1;
		}

		if(a[i] < b[i])
		{
			return -1;
		}
	}

	return 0;
}

/**
 * @brief       for a = b*2^t, b is odd, get t.
 * @param[in]   a	- big integer a.
 * @return   	number of multiple by 2, for a.
 * @caution		make sure a != 0.
 */
unsigned int Get_Multiple2_Number(unsigned int a[])
{
	unsigned int t, i=0, j=0;

	while(0 == (a[i]))
	{
		i++;
	}

	t = a[i];
	while(!(t&1))
	{
		j++;
		t>>=1;
	}

	return (i<<5)+j;
}

/**
 * @brief       a = a/(2^n).
 * @param[in]   a 		 - big integer a.
 * @param[in]   aWordLen - word length of a.
 * @param[in]   n 		 - exponent of 2^n.
 * @return   	word length of a = a/(2^n).
 * @caution     1. make sure aWordLen is real word length of a.
 *              2. a may be 0, then aWordLen is 0, to make sure aWordLen-1 is available, so data type of aWordLen is int32_t, not uint32_t.
 */
unsigned int Big_Div2n(unsigned int a[], signed int aWordLen, unsigned int n)
{
	signed int i;
	unsigned int j;

	aWordLen = get_valid_words(a, aWordLen);
	if(0 == n)
	{
		return aWordLen;
	}

	if(!aWordLen)
		return 0;

	if(n<=32)
	{
		for(i=0; i<aWordLen-1; i++)
		{
			a[i] >>= n;
			a[i] |= (a[i+1]<<(32-n));
		}
		a[i] >>= n;

		if(!a[i])
			return i;
		return aWordLen;
	}
	else        //general method
	{
		j=n>>5; //j=n/32;
		n&=31;  //n=n%32;
		for(i=0; i<aWordLen-(int)j-1; i++)
		{
			a[i] = a[i+j]>>n;
			a[i] |= (a[i+j+1]<<(32-n));
		}
		a[i] = a[i+j]>>n;
		uint32_clear(a+aWordLen-j, j);

		if(!a[i])
			return i;
		return aWordLen-j;
	}
}

/**
 * @brief       check whether a is equal to 1 or not.
 * @param[in]   a        - pointer to uint32_t big integer a.
 * @param[in]   aWordLen - word length of big integer a.
 * @return   	1(a is 1), 0(a is not 1).
 */
unsigned char Bigint_Check_1(unsigned int a[], unsigned int aWordLen)
{
	unsigned int i;

	if(!aWordLen)
	{
		return 0;
	}

	if(a[0] != 1)
	{
		return 0;
	}

	for(i=1; i<aWordLen; i++)
	{
		if(a[i])
			return 0;
	}

	return 1;
}

/**
 * @brief       check whether a is equal to p-1 or not.
 * @param[in]   a        - pointer to unsigned int big integer a.
 * @param[in]   p        - pointer to unsigned int big integer p, p must be odd.
 * @param[in]   aWordLen - word length of a and p.
 * @return   	1(a is 1), 0(a is not 1).
 * @caution		make sure p is odd.
 */
unsigned char Bigint_Check_p_1(unsigned int a[], unsigned int p[], unsigned int wordLen)
{
	unsigned int i;

	if(!wordLen)
	{
		return 0;
	}

	if(a[0] != p[0] - 1)
	{
		return 0;
	}

	for(i=1; i<wordLen; i++)
	{
		if(a[i] != p[i])
			return 0;
	}

	return 1;
}


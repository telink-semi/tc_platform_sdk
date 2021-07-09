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
#include"app_config.h"
#include"pke_common/pke_common.h"

#if(PKE_TEST_MODE == PKE_ECDSA)


/**************************** secp256r1 ******************************/
uint32_t const secp256r1_p[8]        = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
uint32_t const secp256r1_p_h[8]      = {0x00000003,0x00000000,0xFFFFFFFF,0xFFFFFFFB,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFD,0x00000004};
uint32_t const secp256r1_p_n1[1]     = {1};
uint32_t const secp256r1_a[8]        = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
uint32_t const secp256r1_b[8]        = {0x27D2604B,0x3BCE3C3E,0xCC53B0F6,0x651D06B0,0x769886BC,0xB3EBBD55,0xAA3A93E7,0x5AC635D8};
uint32_t const secp256r1_Gx[8]       = {0xD898C296,0xF4A13945,0x2DEB33A0,0x77037D81,0x63A440F2,0xF8BCE6E5,0xE12C4247,0x6B17D1F2};
uint32_t const secp256r1_Gy[8]       = {0x37BF51F5,0xCBB64068,0x6B315ECE,0x2BCE3357,0x7C0F9E16,0x8EE7EB4A,0xFE1A7F9B,0x4FE342E2};
uint32_t const secp256r1_n[8]        = {0xFC632551,0xF3B9CAC2,0xA7179E84,0xBCE6FAAD,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF};
uint32_t const secp256r1_n_h[8]      = {0xBE79EEA2,0x83244C95,0x49BD6FA6,0x4699799C,0x2B6BEC59,0x2845B239,0xF3D95620,0x66E12D94};
uint32_t const secp256r1_n_n1[1]     = {0xEE00BC4F};
eccp_curve_t secp256r1=
	{
		256,
		256,
		(uint32_t *)secp256r1_p,
		NULL, //(uint32_t *)secp256r1_p_h,
		NULL, //(uint32_t *)secp256r1_p_n1,
		(uint32_t *)secp256r1_a,
		(uint32_t *)secp256r1_b,
		(uint32_t *)secp256r1_Gx,
		(uint32_t *)secp256r1_Gy,
		(uint32_t *)secp256r1_n,
		NULL, //(uint32_t *)secp256r1_n_h,
		NULL, //(uint32_t *)secp256r1_n_n1,
	};


typedef struct
{
	uint8_t *rand_k;
	uint8_t *priKey;
	uint8_t *pubKey;
	uint8_t *std_signature_r;
	uint8_t *std_E_md5;
	uint8_t *std_signature_s_md5;
	uint8_t *std_E_sha1;
	uint8_t *std_signature_s_sha1;
	uint8_t *std_E_192;
	uint8_t *std_signature_s_192;
	uint8_t *std_E_sha224;
	uint8_t *std_signature_s_sha224;
	uint8_t *std_E_sha256;
	uint8_t *std_signature_s_sha256;
	uint8_t *std_E_sha384;
	uint8_t *std_signature_s_sha384;
	uint8_t *std_E_sha512;
	uint8_t *std_signature_s_sha512;
	uint8_t *std_E_544;
	uint8_t *std_signature_s_544;
	uint8_t *std_signature_s_zero;
	uint8_t *std_signature_s_1;
	uint8_t *std_signature_s_FF;
} ecdsa_test_vector_t;

/***************************** standard data E ********************************/

//message: "abc"
//uint8_t msg[3]={0x61,0x62,0x63};

//E(hash digest from md5("abc")):
//0x900150983CD24FB0D6963F7D28E17F72
uint8_t std_E_md5[]={
	0x90,0x01,0x50,0x98,0x3C,0xD2,0x4F,0xB0,0xD6,0x96,0x3F,0x7D,0x28,0xE1,0x7F,0x72,};

//E(hash digest from sha1("abc")):
//0xA9993E364706816ABA3E25717850C26C9CD0D89D
uint8_t std_E_sha1[]={
	0xA9,0x99,0x3E,0x36,0x47,0x06,0x81,0x6A,0xBA,0x3E,0x25,0x71,0x78,0x50,0xC2,0x6C,
	0x9C,0xD0,0xD8,0x9D,};

//E(random data, 192 bits):
//0xEB3C60084A65169533826A231C5F590D9FDB8EBEF8160FCE
uint8_t std_E_192[]={
	0xEB,0x3C,0x60,0x08,0x4A,0x65,0x16,0x95,0x33,0x82,0x6A,0x23,0x1C,0x5F,0x59,0x0D,
	0x9F,0xDB,0x8E,0xBE,0xF8,0x16,0x0F,0xCE,};

//E(hash digest from sha224("abc")):
//0x23097D223405D8228642A477BDA255B32AADBCE4BDA0B3F7E36C9DA7
uint8_t std_E_sha224[]={
	0x23,0x09,0x7D,0x22,0x34,0x05,0xD8,0x22,0x86,0x42,0xA4,0x77,0xBD,0xA2,0x55,0xB3,
	0x2A,0xAD,0xBC,0xE4,0xBD,0xA0,0xB3,0xF7,0xE3,0x6C,0x9D,0xA7,};

//E(hash digest from sha256("abc")):
//0xBA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD
uint8_t std_E_sha256[]={
	0xBA,0x78,0x16,0xBF,0x8F,0x01,0xCF,0xEA,0x41,0x41,0x40,0xDE,0x5D,0xAE,0x22,0x23,
	0xB0,0x03,0x61,0xA3,0x96,0x17,0x7A,0x9C,0xB4,0x10,0xFF,0x61,0xF2,0x00,0x15,0xAD,};

//E(hash digest from sha384("abc")):
//0xCB00753F45A35E8BB5A03D699AC65007272C32AB0EDED1631A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7
uint8_t std_E_sha384[]={
	0xCB,0x00,0x75,0x3F,0x45,0xA3,0x5E,0x8B,0xB5,0xA0,0x3D,0x69,0x9A,0xC6,0x50,0x07,
	0x27,0x2C,0x32,0xAB,0x0E,0xDE,0xD1,0x63,0x1A,0x8B,0x60,0x5A,0x43,0xFF,0x5B,0xED,
	0x80,0x86,0x07,0x2B,0xA1,0xE7,0xCC,0x23,0x58,0xBA,0xEC,0xA1,0x34,0xC8,0x25,0xA7,};

//E(hash digest from sha512("abc")):
//0xDDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F
uint8_t std_E_sha512[]={
	0xDD,0xAF,0x35,0xA1,0x93,0x61,0x7A,0xBA,0xCC,0x41,0x73,0x49,0xAE,0x20,0x41,0x31,
	0x12,0xE6,0xFA,0x4E,0x89,0xA9,0x7E,0xA2,0x0A,0x9E,0xEE,0xE6,0x4B,0x55,0xD3,0x9A,
	0x21,0x92,0x99,0x2A,0x27,0x4F,0xC1,0xA8,0x36,0xBA,0x3C,0x23,0xA3,0xFE,0xEB,0xBD,
	0x45,0x4D,0x44,0x23,0x64,0x3C,0xE8,0x0E,0x2A,0x9A,0xC9,0x4F,0xA5,0x4C,0xA4,0x9F,};

//E(random data, 544 bits):
//0xD29BFAE2211F962C46AD5515C076A5092EF1B6BE15F2578D02AC001AB841933EA0A28710CCD538EA896E4C19DB29488A5DCEFABA10EC81668808BF39B6BEFD5A7831CD18
uint8_t std_E_544[]={
	0xD2,0x9B,0xFA,0xE2,0x21,0x1F,0x96,0x2C,0x46,0xAD,0x55,0x15,0xC0,0x76,0xA5,0x09,
	0x2E,0xF1,0xB6,0xBE,0x15,0xF2,0x57,0x8D,0x02,0xAC,0x00,0x1A,0xB8,0x41,0x93,0x3E,
	0xA0,0xA2,0x87,0x10,0xCC,0xD5,0x38,0xEA,0x89,0x6E,0x4C,0x19,0xDB,0x29,0x48,0x8A,
	0x5D,0xCE,0xFA,0xBA,0x10,0xEC,0x81,0x66,0x88,0x08,0xBF,0x39,0xB6,0xBE,0xFD,0x5A,
	0x78,0x31,0xCD,0x18,};


uint32_t ecdsa_std_sign_verify_test_internal(eccp_curve_t *curve, char *curve_name, uint8_t *E, uint32_t EByteLen,
		uint8_t *rand_k, uint8_t *prikey, uint8_t *pubkey, uint8_t *signature_r, uint8_t *signature_s,
		uint8_t sign_ret_value, char *info)
{
	uint8_t signature[2*ECCP_MAX_BYTE_LEN];
	uint8_t buf[2*ECCP_MAX_BYTE_LEN]={0};
	uint8_t tmp[2*ECCP_MAX_BYTE_LEN+1]={0};
	uint32_t pByteLen, nByteLen;
	uint32_t pubkeyByteLen, signatureByteLen;
	uint32_t ret;
	char *inf="";

	if(info)
		inf=info;

	pByteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
	nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	pubkeyByteLen = 2*pByteLen;
	signatureByteLen = 2*nByteLen;

	memcpy_(signature, signature_r, nByteLen);
	memcpy_(signature+nByteLen, signature_s, nByteLen);

	ret = ecdsa_sign(curve, E, EByteLen, rand_k, prikey, buf);
    if((sign_ret_value != ret))
    {
    	printf("\r\n %s ECDSA sign %s failure, ret=%d", curve_name, inf, ret);
    	return 1;
    }

    if(ECDSA_SUCCESS == ret)
    {
    	if((NULL != rand_k) && (NULL != signature))
    	{
			if(memcmp_(buf, signature, signatureByteLen))
			{
				printf("\r\n %s ECDSA sign %s error, ret = %02x", curve_name, inf, ret);
//				print_buf_U8(E, EByteLen, "E");
//
//				print_buf_U8(rand_k, nByteLen, "rand_k");
//				print_buf_U8(prikey, pByteLen, "prikey");
//				print_buf_U8(signature, signatureByteLen, "signature");
//				print_buf_U8(buf, signatureByteLen, "buf");
				return 1;
			}
    	}

    	ret = ecdsa_verify(curve, E, EByteLen, pubkey, buf);
    	if(ECDSA_SUCCESS != ret)
    	{
    		printf("\r\n %s ECDSA verify %s error, ret = %02x", curve_name, inf, ret);
    		return 1;
    	}

		//invalid E
		memcpy_(tmp, E, EByteLen);
		tmp[0]+=3;
		ret = ecdsa_verify(curve, tmp, EByteLen, pubkey, buf);
		if(ECDSA_VERIFY_FAILED != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid E), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid pubkey test 1
		memcpy_(tmp, pubkey, pubkeyByteLen);
		tmp[0]+=3;
		ret = ecdsa_verify(curve, E, EByteLen, tmp, buf);
		if(PKE_NOT_ON_CURVE != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid public key 1), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid pubkey test 2
		memcpy_(tmp, pubkey, pubkeyByteLen);
		tmp[1]+=3;
		ret = ecdsa_verify(curve, E, EByteLen, tmp, buf);
		if(PKE_NOT_ON_CURVE != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid public key 2), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid pubkey test 3
		memcpy_(tmp, pubkey, pubkeyByteLen);
		tmp[pubkeyByteLen-1]+=3;
		ret = ecdsa_verify(curve, E, EByteLen, tmp, buf);
		if(PKE_NOT_ON_CURVE != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid public key 3), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 1(corrupted r)
		memcpy_(tmp, buf, signatureByteLen);
		tmp[2]+=3;
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_VERIFY_FAILED != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: corrupted r), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 2(r=0)
		memcpy_(tmp, buf, signatureByteLen);
		memset_(tmp, 0, nByteLen);
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_ZERO_ALL != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: r=0), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 3(r=n-1)
		memcpy_(tmp, buf, signatureByteLen);
		reverse_byte_array((uint8_t *)(curve->eccp_n), tmp, nByteLen);
		tmp[nByteLen-1]-=1;
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_VERIFY_FAILED != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: r=n-1), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 4(r=n)
		memcpy_(tmp, buf, signatureByteLen);
		reverse_byte_array((uint8_t *)(curve->eccp_n), tmp, nByteLen);
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_INTEGER_TOO_BIG != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: r=n), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 5(r=n+1)
		memcpy_(tmp, buf, signatureByteLen);
		reverse_byte_array((uint8_t *)(curve->eccp_n), tmp, nByteLen);
		tmp[nByteLen-1]+=1;
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_INTEGER_TOO_BIG != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: r=n+1), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 6(corrupted s)
		memcpy_(tmp, buf, signatureByteLen);
		tmp[signatureByteLen-1]+=3;
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_VERIFY_FAILED != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: corrupted s), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 7(s=0)
		memcpy_(tmp, buf, signatureByteLen);
		memset_(tmp+nByteLen, 0, nByteLen);
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_ZERO_ALL != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: s=0), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 8(s=n-1)
		memcpy_(tmp, buf, signatureByteLen);
		reverse_byte_array((uint8_t *)(curve->eccp_n), tmp+nByteLen, nByteLen);
		tmp[signatureByteLen-1]-=1;
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_VERIFY_FAILED != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: s=n-1), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 9(s=n)
		memcpy_(tmp, buf, signatureByteLen);
		reverse_byte_array((uint8_t *)(curve->eccp_n), tmp+nByteLen, nByteLen);
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_INTEGER_TOO_BIG != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: s=n), ret=%x", curve_name, inf, ret);
			return 1;
		}

		//invalid signature test 10(s=n+1)
		memcpy_(tmp, buf, signatureByteLen);
		reverse_byte_array((uint8_t *)(curve->eccp_n), tmp+nByteLen, nByteLen);
		tmp[signatureByteLen-1]+=1;
		ret = ecdsa_verify(curve, E, EByteLen, pubkey, tmp);
		if(ECDSA_INTEGER_TOO_BIG != ret)
		{
			printf("\r\n %s ECDSA verify %s failure(invalid signature: r=n+1), ret=%x", curve_name, inf, ret);
			return 1;
		}
    }

    if(NULL != info)
    	printf("\r\n %s ECDSA sign & verify %s success", curve_name, inf);

    return 0;
}

uint32_t ecdsa_std_sign_verify_test(eccp_curve_t *curve, char *curve_name, ecdsa_test_vector_t *vector)
{
	uint8_t E[ECCP_MAX_BYTE_LEN];
	uint32_t i, tmpLen, nByteLen;

	nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);

	printf("\r\n -------------- %s ECDSA sign & verify test -------------- ", curve_name);

	//E(hash digest) 128 bits, actually here E is md5("abc")
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_md5, 16, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r, vector->std_signature_s_md5, ECDSA_SUCCESS,"standard data test(E-128bits)"))
	{
		//debug("app_ecdsa.c", "ecdsa_std_sign_verify_test", "failed");
		return 1;
	}

	//E(hash digest) 160 bits, actually here E is sha1("abc")
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_sha1, 20, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_sha1, ECDSA_SUCCESS,"standard data test(E-160bits)"))
	{
		return 1;
	}
	//E(hash digest) 192 bits, here E is random data
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_192, 24, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_192, ECDSA_SUCCESS,"standard data test(E-192bits)"))
	{
		return 1;
	}

	//E(hash digest) 224 bits, actually here E is sha224("abc")
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_sha224, 28, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_sha224, ECDSA_SUCCESS,"standard data test(E-224bits)"))
	{
		return 1;
	}

	//E(hash digest) 256 bit, actually here E is sha256("abc")
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_sha256, 32, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_sha256, ECDSA_SUCCESS,"standard data test(E-256bits)"))
	{
		return 1;
	}

	//E(hash digest) 384 bit, actually here E is sha384("abc")
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_sha384, 48, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_sha384, ECDSA_SUCCESS,"standard data test(E-384bits)"))
	{
		return 1;
	}

	//E(hash digest) 512 bit, actually here E is sha512("abc")
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_sha512, 64, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_sha512, ECDSA_SUCCESS,"standard data test(E-512bits)"))
	{
		return 1;
	}

	//E(hash digest) 544 bit, here E is random data
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, vector->std_E_544, 68, vector->rand_k,
		vector->priKey, vector->pubKey, vector->std_signature_r,vector->std_signature_s_544, ECDSA_SUCCESS,"standard data test(E-544bits)"))
	{
		return 1;
	}

	/********** e = 0 **********/
	memset_(E, 0, ECCP_MAX_BYTE_LEN);
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, E, nByteLen, vector->rand_k, vector->priKey,
		vector->pubKey, vector->std_signature_r,vector->std_signature_s_zero, ECDSA_SUCCESS,"test(E=0)"))
	{
		return 1;
	}

	/********** e = 1 **********/
	memset_(E, 0, ECCP_MAX_BYTE_LEN);
	E[nByteLen-1]=1;
	if((curve->eccp_n_bitLen)&7)
	{
		E[nByteLen-1] <<= (8-((curve->eccp_n_bitLen)&7));
	}
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, E, nByteLen, vector->rand_k, vector->priKey,
		vector->pubKey, vector->std_signature_r,vector->std_signature_s_1, ECDSA_SUCCESS,"test(E=1)"))
	{
		return 1;
	}

	/********** e = n **********/
	reverse_byte_array((uint8_t *)(curve->eccp_n), E, nByteLen);
	//make sure the MSB nBitLen is n
	tmpLen = (curve->eccp_n_bitLen)&7;
	if(tmpLen)
	{
		for(i=0;i<nByteLen-1;i++)
		{
			E[i] <<= (8-tmpLen);
			E[i] |= E[i+1]>>tmpLen;
		}
		E[i] <<= (8-tmpLen);
	}
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, E, nByteLen, vector->rand_k, vector->priKey,
		vector->pubKey, vector->std_signature_r,vector->std_signature_s_zero, ECDSA_SUCCESS,"test(E=n)"))
	{
		return 1;
	}

	/********** e = 0xFF...FF **********/
	memset_(E, 0xFF, nByteLen);
	if(ecdsa_std_sign_verify_test_internal(curve, curve_name, E, nByteLen, vector->rand_k, vector->priKey,
		vector->pubKey, vector->std_signature_r,vector->std_signature_s_FF, ECDSA_SUCCESS,"test(E=0xFF..FF)"))
	{
		return 1;
	}

	printf("\r\n");
	return 0;
}

ecdsa_test_vector_t vector;
//k:
//0x05B2FAC1D8E526D8B6DBC634EBEEDFF98C8B0F246E7FFFA0D972D8FF8E58F288
uint8_t rand_k[32]={
	0x05,0xB2,0xFA,0xC1,0xD8,0xE5,0x26,0xD8,0xB6,0xDB,0xC6,0x34,0xEB,0xEE,0xDF,0xF9,
	0x8C,0x8B,0x0F,0x24,0x6E,0x7F,0xFF,0xA0,0xD9,0x72,0xD8,0xFF,0x8E,0x58,0xF2,0x88,};

//private key:
//0xF43BAFC3C7B2FB10AFEED65668D3634E3239C108BB6ACF34333149025584B30E
uint8_t priKey[32]={
	0xF4,0x3B,0xAF,0xC3,0xC7,0xB2,0xFB,0x10,0xAF,0xEE,0xD6,0x56,0x68,0xD3,0x63,0x4E,
	0x32,0x39,0xC1,0x08,0xBB,0x6A,0xCF,0x34,0x33,0x31,0x49,0x02,0x55,0x84,0xB3,0x0E,};

//public key:
//x coordinate:
//0xBD2FEF24AC636F55DBF1F5203885097E18629EDFFB36F40389612744A10F02D0
//y coordinate:
//0x6B519E0F82BB90D1633D07E8708F6595CEBFF3866CFE33F3CBE8A3681EB820EA
uint8_t pubKey[64]={
	0xBD,0x2F,0xEF,0x24,0xAC,0x63,0x6F,0x55,0xDB,0xF1,0xF5,0x20,0x38,0x85,0x09,0x7E,
	0x18,0x62,0x9E,0xDF,0xFB,0x36,0xF4,0x03,0x89,0x61,0x27,0x44,0xA1,0x0F,0x02,0xD0,
	0x6B,0x51,0x9E,0x0F,0x82,0xBB,0x90,0xD1,0x63,0x3D,0x07,0xE8,0x70,0x8F,0x65,0x95,
	0xCE,0xBF,0xF3,0x86,0x6C,0xFE,0x33,0xF3,0xCB,0xE8,0xA3,0x68,0x1E,0xB8,0x20,0xEA,};

//standard signature(the default data is r, it is fixed for the curve and rand_k)
//r:0xE53135AC20D40A1F2388A9C53D723C906B4366FCFAD5BC8F4987F7D60EA64B1C
uint8_t std_signature_r[]={
	0xE5,0x31,0x35,0xAC,0x20,0xD4,0x0A,0x1F,0x23,0x88,0xA9,0xC5,0x3D,0x72,0x3C,0x90,
	0x6B,0x43,0x66,0xFC,0xFA,0xD5,0xBC,0x8F,0x49,0x87,0xF7,0xD6,0x0E,0xA6,0x4B,0x1C,};

/********************* standard data test(E=0) **********************/
//signature s:
//0xB76EF4B0E5A683F374BF90D0B90A3A1B15087C1EBAA154FD3EBC8A7B8F02B20D
uint8_t std_signature_s_zero[]={
	0xB7,0x6E,0xF4,0xB0,0xE5,0xA6,0x83,0xF3,0x74,0xBF,0x90,0xD0,0xB9,0x0A,0x3A,0x1B,
	0x15,0x08,0x7C,0x1E,0xBA,0xA1,0x54,0xFD,0x3E,0xBC,0x8A,0x7B,0x8F,0x02,0xB2,0x0D,};

/********************* standard data test(E=1) **********************/
//signature s:
//0xC3C9ADD415CB37268B1FC1CD656508EBE592528FA251AFA9F9E1E0F700242B0A
uint8_t std_signature_s_1[]={
	0xC3,0xC9,0xAD,0xD4,0x15,0xCB,0x37,0x26,0x8B,0x1F,0xC1,0xCD,0x65,0x65,0x08,0xEB,
	0xE5,0x92,0x52,0x8F,0xA2,0x51,0xAF,0xA9,0xF9,0xE1,0xE0,0xF7,0x00,0x24,0x2B,0x0A,};

/********************* standard data test(E=0xFF..FF) **********************/
//signature:
//0x560FEF78F603AEA04BC9DBA467279D6A9CFB406EA21E8A6F8DC643EE56A419DC
uint8_t std_signature_s_FF[]={
	0x56,0x0F,0xEF,0x78,0xF6,0x03,0xAE,0xA0,0x4B,0xC9,0xDB,0xA4,0x67,0x27,0x9D,0x6A,
	0x9C,0xFB,0x40,0x6E,0xA2,0x1E,0x8A,0x6F,0x8D,0xC6,0x43,0xEE,0x56,0xA4,0x19,0xDC,};


/********************* standard data test(E-128bits) **********************/
//signature s:
//0x3A19ECEC89A48943070B12ACF2C6874DFF4685A7E31805047B6300C9AE156160
uint8_t std_signature_s_md5[]={
	0x3A,0x19,0xEC,0xEC,0x89,0xA4,0x89,0x43,0x07,0x0B,0x12,0xAC,0xF2,0xC6,0x87,0x4D,
	0xFF,0x46,0x85,0xA7,0xE3,0x18,0x05,0x04,0x7B,0x63,0x00,0xC9,0xAE,0x15,0x61,0x60,};

/********************* standard data test(E-160bits) **********************/
//signature s:
//0x46D96A779797568B552FE77A721EC4F6D575FA924AADE918D27372A603160B5F
uint8_t std_signature_s_sha1[]={
	0x46,0xD9,0x6A,0x77,0x97,0x97,0x56,0x8B,0x55,0x2F,0xE7,0x7A,0x72,0x1E,0xC4,0xF6,
	0xD5,0x75,0xFA,0x92,0x4A,0xAD,0xE9,0x18,0xD2,0x73,0x72,0xA6,0x03,0x16,0x0B,0x5F,};

/********************* standard data test(E-192bits) **********************/
//signature s:
//0x8F38365A9C7375E6517C786760B1D5CD050481CA8A8AD0D4D4BC1F390E4CE4C3
uint8_t std_signature_s_192[]={
	0x8F,0x38,0x36,0x5A,0x9C,0x73,0x75,0xE6,0x51,0x7C,0x78,0x67,0x60,0xB1,0xD5,0xCD,
	0x05,0x04,0x81,0xCA,0x8A,0x8A,0xD0,0xD4,0xD4,0xBC,0x1F,0x39,0x0E,0x4C,0xE4,0xC3,};

/********************* standard data test(E-224bits) **********************/
//signature s:
//0x79759E92B853474D7CE512F4B24D0408350BBB34A5E1B893F742967D67A266FE
uint8_t std_signature_s_sha224[]={
	0x79,0x75,0x9E,0x92,0xB8,0x53,0x47,0x4D,0x7C,0xE5,0x12,0xF4,0xB2,0x4D,0x04,0x08,
	0x35,0x0B,0xBB,0x34,0xA5,0xE1,0xB8,0x93,0xF7,0x42,0x96,0x7D,0x67,0xA2,0x66,0xFE,};

/********************* standard data test(E-256bits) **********************/
//signature s:
//0xC988BF9FC414AD7B1227BE793B5F27242E1A4CA952275F4CF20DD401FB547CF5
uint8_t std_signature_s_sha256[]={
	0xC9,0x88,0xBF,0x9F,0xC4,0x14,0xAD,0x7B,0x12,0x27,0xBE,0x79,0x3B,0x5F,0x27,0x24,
	0x2E,0x1A,0x4C,0xA9,0x52,0x27,0x5F,0x4C,0xF2,0x0D,0xD4,0x01,0xFB,0x54,0x7C,0xF5,};

/********************* standard data test(E-384bits) **********************/
//signature s:
//0x9846A7A7759E76E92B39105DECF8FAB1BD18FB67A44300583413BDE34E8EB6AB
uint8_t std_signature_s_sha384[]={
	0x98,0x46,0xA7,0xA7,0x75,0x9E,0x76,0xE9,0x2B,0x39,0x10,0x5D,0xEC,0xF8,0xFA,0xB1,
	0xBD,0x18,0xFB,0x67,0xA4,0x43,0x00,0x58,0x34,0x13,0xBD,0xE3,0x4E,0x8E,0xB6,0xAB,};

/********************* standard data test(E-512bits) **********************/
//signature s:
//0xF679EFB89D01F6250ECABA041BF9FEE7284FD955FF3D2A62AF395F76F11DFA77
uint8_t std_signature_s_sha512[]={
	0xF6,0x79,0xEF,0xB8,0x9D,0x01,0xF6,0x25,0x0E,0xCA,0xBA,0x04,0x1B,0xF9,0xFE,0xE7,
	0x28,0x4F,0xD9,0x55,0xFF,0x3D,0x2A,0x62,0xAF,0x39,0x5F,0x76,0xF1,0x1D,0xFA,0x77,};

/********************* standard data test(E-544bits) **********************/
//signature s:
//0xE102ED6183A2A2F3322FE5D5421C226000CFA6B90AF7A1451C5820D6DC446DD7
uint8_t std_signature_s_544[]={
	0xE1,0x02,0xED,0x61,0x83,0xA2,0xA2,0xF3,0x32,0x2F,0xE5,0xD5,0x42,0x1C,0x22,0x60,
	0x00,0xCF,0xA6,0xB9,0x0A,0xF7,0xA1,0x45,0x1C,0x58,0x20,0xD6,0xDC,0x44,0x6D,0xD7,};
uint32_t ecdsa_std_test_secp256r1(void)
{

//	eccp_curve_t * curve = &secp256r1;
	char * curve_name = "secp256r1";

	vector.rand_k = rand_k;
	vector.priKey = priKey;
	vector.pubKey = pubKey;
	vector.std_signature_r = std_signature_r;
	vector.std_E_md5 = std_E_md5;
	vector.std_signature_s_md5 = std_signature_s_md5;
	vector.std_E_sha1 = std_E_sha1;
	vector.std_signature_s_sha1 = std_signature_s_sha1;
	vector.std_E_192 = std_E_192;
	vector.std_signature_s_192 = std_signature_s_192;
	vector.std_E_sha224 = std_E_sha224;
	vector.std_signature_s_sha224 = std_signature_s_sha224;
	vector.std_E_sha256 = std_E_sha256;
	vector.std_signature_s_sha256 = std_signature_s_sha256;
	vector.std_E_sha384 = std_E_sha384;
	vector.std_signature_s_sha384 = std_signature_s_sha384;
	vector.std_E_sha512 = std_E_sha512;
	vector.std_signature_s_sha512 = std_signature_s_sha512;
	vector.std_E_544 = std_E_544;
	vector.std_signature_s_544 = std_signature_s_544;
	vector.std_signature_s_zero = std_signature_s_zero;
	vector.std_signature_s_1 = std_signature_s_1;
	vector.std_signature_s_FF = std_signature_s_FF;
//	debug("app_ecdsa.c", "ecdsa_std_test_secp256r1", "vector->std_signature_s_FF = std_signature_s_FF;");
	return ecdsa_std_sign_verify_test(&secp256r1, curve_name, &vector);
}


uint8_t ECDSA_all_test(void)
{
	printf("\r\n\r\n =================== ECDSA test =================== \r\n");

#if 0
	if(ecdsa_speed_test(brainpoolp512r1))
		return 1;
#endif

#if 1
	/********** standard data test **********/
#if (ECCP_MAX_BIT_LEN >= 256)
	if(ecdsa_std_test_secp256r1())
		return 1;
#endif
#endif

	return 0;
}


void user_init()
{
#if FPGA_TEST
	gpio_set_func(LED1|LED2|LED3|LED4|LED5|LED6, AS_GPIO);
	gpio_set_output_en(LED1|LED2|LED3|LED4|LED5|LED6, 1);
	gpio_set_input_en(LED1|LED2|LED3|LED4|LED5|LED6, 0);
	gpio_write(LED1|LED2|LED3|LED4|LED5|LED6, 1);

#else
//	gpio_set_func(LED1|LED2|LED3|LED4, AS_GPIO);
//	gpio_set_output_en(LED1|LED2|LED3|LED4, 1);
//	gpio_set_input_en(LED1|LED2|LED3|LED4, 0);
//	gpio_write(LED1|LED2|LED3|LED4, 1);
#endif
	ECDSA_all_test();
}

void main_loop()
{
	sleep_ms(500);
#if FPGA_TEST
	gpio_toggle(LED1|LED2|LED3|LED4|LED5|LED6);
	printf("\r\n hello world!");
#else
	gpio_toggle(LED1|LED2|LED3|LED4);
#endif
}

#endif

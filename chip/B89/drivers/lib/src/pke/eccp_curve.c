/********************************************************************************************************
 * @file	eccp_curve.c
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
#include "lib/include/pke/eccp_curve.h"

/**************************** brainpoolp160r1 ******************************/
#ifdef SUPPORT_BRAINPOOLP160R1
unsigned int const brainpoolp160r1_p[5]        = {0x9515620F,0x95B3D813,0x60DFC7AD,0x737059DC,0xE95E4A5F};
#ifdef PKE_HP
unsigned int const brainpoolp160r1_p_h[5]      = {0x532B7BEB,0xC57E4353,0xB4BA7FB8,0x4CC30F3B,0xB3945136};
#else
unsigned int const brainpoolp160r1_p_h[5]      = {0x25BC14FF,0xB333F8D6,0xFED717E0,0xC0CA7EF8,0x6CF12F81};
unsigned int const brainpoolp160r1_p_n1[1]     = {0xADBCB311};
#endif
unsigned int const brainpoolp160r1_a[5]        = {0xE8F7C300,0xDA745D97,0xE2BE61BA,0xA280EB74,0x340E7BE2};
unsigned int const brainpoolp160r1_b[5]        = {0xD8675E58,0xBDEC95C8,0x134FAA2D,0x95423412,0x1E589A85};
unsigned int const brainpoolp160r1_Gx[5]       = {0xBDBCDBC3,0x31EB5AF7,0x62938C46,0xEA3F6A4F,0xBED5AF16};
unsigned int const brainpoolp160r1_Gy[5]       = {0x16DA6321,0x669C9763,0x38F94741,0x7A1A8EC3,0x1667CB47};
unsigned int const brainpoolp160r1_n[5]        = {0x9E60FC09,0xD4502940,0x60DF5991,0x737059DC,0xE95E4A5F};
#ifdef PKE_HP
unsigned int const brainpoolp160r1_n_h[5]      = {0x9ADFB54B,0xE00DFA53,0x0E7C2B8D,0x5C5494B1,0x9B44D4F6};
#else
unsigned int const brainpoolp160r1_n_h[5]      = {0x1FDF90EA,0xFC61D435,0x9E31FE16,0xFC9BE6F6,0x2BC73851};
unsigned int const brainpoolp160r1_n_n1[1]     = {0x5C7AADC7};
#endif

//[2^80]G
#ifdef PKE_HP
unsigned int brainpoolp160r1_2_80_Gx[5]  = {0xB19BD5A1,0xD3B8210C,0xBB518725,0x39FC8E94,0x8E63BD39};
unsigned int brainpoolp160r1_2_80_Gy[5]  = {0x68DE4448,0xAF1015A3,0x8E138836,0x17A68390,0x4D1E977B};
#endif

#ifdef PKE_HP
eccp_curve_t brainpoolp160r1[1]={
	{
		160,
		160,
		(unsigned int *)brainpoolp160r1_p,
		(unsigned int *)brainpoolp160r1_p_h,
		(unsigned int *)brainpoolp160r1_a,
		(unsigned int *)brainpoolp160r1_b,
		(unsigned int *)brainpoolp160r1_Gx,
		(unsigned int *)brainpoolp160r1_Gy,
		(unsigned int *)brainpoolp160r1_n,
		(unsigned int *)brainpoolp160r1_n_h,
		(unsigned int *)brainpoolp160r1_2_80_Gx,
		(unsigned int *)brainpoolp160r1_2_80_Gy,
	},
};
#else
eccp_curve_t brainpoolp160r1[1]={
	{
		160,
		160,
		(unsigned int *)brainpoolp160r1_p,
		(unsigned int *)brainpoolp160r1_p_h,
		(unsigned int *)brainpoolp160r1_p_n1,
		(unsigned int *)brainpoolp160r1_a,
		(unsigned int *)brainpoolp160r1_b,
		(unsigned int *)brainpoolp160r1_Gx,
		(unsigned int *)brainpoolp160r1_Gy,
		(unsigned int *)brainpoolp160r1_n,
		(unsigned int *)brainpoolp160r1_n_h,
		(unsigned int *)brainpoolp160r1_n_n1,
	},
};
#endif
#endif


/**************************** secp192r1 ******************************/
#ifdef SUPPORT_SECP192R1
unsigned int const secp192r1_p[6]        = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#ifdef PKE_HP
unsigned int const secp192r1_p_h[6]      = {0x00000002,0x00000000,0x00000003,0x00000000,0x00000002,0x00000000};
#else
unsigned int const secp192r1_p_h[6]      = {0x00000001,0x00000000,0x00000002,0x00000000,0x00000001,0x00000000};
unsigned int const secp192r1_p_n1[1]     = {1};
#endif
unsigned int const secp192r1_a[6]        = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp192r1_b[6]        = {0xC146B9B1,0xFEB8DEEC,0x72243049,0x0FA7E9AB,0xE59C80E7,0x64210519};
unsigned int const secp192r1_Gx[6]       = {0x82FF1012,0xF4FF0AFD,0x43A18800,0x7CBF20EB,0xB03090F6,0x188DA80E};
unsigned int const secp192r1_Gy[6]       = {0x1E794811,0x73F977A1,0x6B24CDD5,0x631011ED,0xFFC8DA78,0x07192B95};
unsigned int const secp192r1_n[6]        = {0xB4D22831,0x146BC9B1,0x99DEF836,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#ifdef PKE_HP
unsigned int const secp192r1_n_h[6]      = {0x83134C27,0x01D1770A,0xCAAF687F,0xD69C6961,0xCEF5D8C5,0x126792C4};
#else
unsigned int const secp192r1_n_h[6]      = {0xDEB35961,0xCE66BACC,0xBB3A6BEE,0x4696EA5B,0xEA0581A2,0x28BE5677};
unsigned int const secp192r1_n_n1[1]     = {0x0DDBCF2F};
#endif

//[2^96]G
#ifdef PKE_HP
unsigned int secp192r1_2_96_Gx[6]  = {0xC0A1E340,0xB19963D8,0x80D1090B,0x4730D4F4,0x184AC737,0x51A581D9};
unsigned int secp192r1_2_96_Gy[6]  = {0xE69912A5,0xECC56731,0x2F683F16,0x7CDFCEA0,0xE0BB9F6E,0x5BD81EE2};
#endif

#ifdef PKE_HP
eccp_curve_t secp192r1[1]={
	{
		192,
		192,
		(unsigned int *)secp192r1_p,
		(unsigned int *)secp192r1_p_h,
		(unsigned int *)secp192r1_a,
		(unsigned int *)secp192r1_b,
		(unsigned int *)secp192r1_Gx,
		(unsigned int *)secp192r1_Gy,
		(unsigned int *)secp192r1_n,
		(unsigned int *)secp192r1_n_h,
		(unsigned int *)secp192r1_2_96_Gx,
		(unsigned int *)secp192r1_2_96_Gy,
	},
};
#else
eccp_curve_t secp192r1[1]={
	{
		192,
		192,
		(unsigned int *)secp192r1_p,
		(unsigned int *)secp192r1_p_h,
		(unsigned int *)secp192r1_p_n1,
		(unsigned int *)secp192r1_a,
		(unsigned int *)secp192r1_b,
		(unsigned int *)secp192r1_Gx,
		(unsigned int *)secp192r1_Gy,
		(unsigned int *)secp192r1_n,
		(unsigned int *)secp192r1_n_h,
		(unsigned int *)secp192r1_n_n1,
	},
};
#endif
#endif


/**************************** secp224r1 ******************************/
#ifdef SUPPORT_SECP224R1
unsigned int const secp224r1_p[7]        = {0x00000001,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#ifdef PKE_HP
unsigned int const secp224r1_p_h[7]      = {0x00000001,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFE,0xFFFFFFFF};
#else
unsigned int const secp224r1_p_h[7]      = {0x00000001,0x00000000,0x00000000,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0x00000000};
unsigned int const secp224r1_p_n1[1]     = {0xFFFFFFFF};
#endif
unsigned int const secp224r1_a[7]        = {0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp224r1_b[7]        = {0x2355FFB4,0x270B3943,0xD7BFD8BA,0x5044B0B7,0xF5413256,0x0C04B3AB,0xB4050A85};
unsigned int const secp224r1_Gx[7]       = {0x115C1D21,0x343280D6,0x56C21122,0x4A03C1D3,0x321390B9,0x6BB4BF7F,0xB70E0CBD};
unsigned int const secp224r1_Gy[7]       = {0x85007E34,0x44D58199,0x5A074764,0xCD4375A0,0x4C22DFE6,0xB5F723FB,0xBD376388};
unsigned int const secp224r1_n[7]        = {0x5C5C2A3D,0x13DD2945,0xE0B8F03E,0xFFFF16A2,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#ifdef PKE_HP
unsigned int const secp224r1_n_h[7]      = {0x5F517D15,0x29947A69,0x31D63F4B,0xABC8FF59,0xD9714856,0x6AD15F7C,0xB1E97961};
#else
unsigned int const secp224r1_n_h[7]      = {0x3AD01289,0x6BDAAE6C,0x97A54552,0x6AD09D91,0xB1E97961,0x1822BC47,0xD4BAA4CF};
unsigned int const secp224r1_n_n1[1]     = {0x6A1FC2EB};
#endif

//[2^112]G
#ifdef PKE_HP
unsigned int secp224r1_2_112_Gx[7] = {0x6CAB26E3,0xA0064196,0x2991FAB0,0x3A0B91FB,0xEC27A4E1,0x5F8EBEEF,0x0499AA8A};
unsigned int secp224r1_2_112_Gy[7] = {0x7766AF5D,0x50751040,0x29610D54,0xF70684D9,0xD77AAE82,0x338C5B81,0x6916F6D4};
#endif

#ifdef PKE_HP
eccp_curve_t secp224r1[1]={
	{
		224,
		224,
		(unsigned int *)secp224r1_p,
		(unsigned int *)secp224r1_p_h,
		(unsigned int *)secp224r1_a,
		(unsigned int *)secp224r1_b,
		(unsigned int *)secp224r1_Gx,
		(unsigned int *)secp224r1_Gy,
		(unsigned int *)secp224r1_n,
		(unsigned int *)secp224r1_n_h,
		(unsigned int *)secp224r1_2_112_Gx,
		(unsigned int *)secp224r1_2_112_Gy,
	},
};
#else
eccp_curve_t secp224r1[1]={
	{
		224,
		224,
		(unsigned int *)secp224r1_p,
		(unsigned int *)secp224r1_p_h,
		(unsigned int *)secp224r1_p_n1,
		(unsigned int *)secp224r1_a,
		(unsigned int *)secp224r1_b,
		(unsigned int *)secp224r1_Gx,
		(unsigned int *)secp224r1_Gy,
		(unsigned int *)secp224r1_n,
		(unsigned int *)secp224r1_n_h,
		(unsigned int *)secp224r1_n_n1,
	},
};
#endif
#endif


/**************************** secp256r1 ******************************/
#ifdef SUPPORT_SECP256R1
unsigned int const secp256r1_p[8]        = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int const secp256r1_p_h[8]      = {0x00000003,0x00000000,0xFFFFFFFF,0xFFFFFFFB,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFD,0x00000004};
#ifndef PKE_HP
unsigned int const secp256r1_p_n1[1]     = {1};
#endif
unsigned int const secp256r1_a[8]        = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int const secp256r1_b[8]        = {0x27D2604B,0x3BCE3C3E,0xCC53B0F6,0x651D06B0,0x769886BC,0xB3EBBD55,0xAA3A93E7,0x5AC635D8};
unsigned int const secp256r1_Gx[8]       = {0xD898C296,0xF4A13945,0x2DEB33A0,0x77037D81,0x63A440F2,0xF8BCE6E5,0xE12C4247,0x6B17D1F2};
unsigned int const secp256r1_Gy[8]       = {0x37BF51F5,0xCBB64068,0x6B315ECE,0x2BCE3357,0x7C0F9E16,0x8EE7EB4A,0xFE1A7F9B,0x4FE342E2};
unsigned int const secp256r1_n[8]        = {0xFC632551,0xF3B9CAC2,0xA7179E84,0xBCE6FAAD,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF};
unsigned int const secp256r1_n_h[8]      = {0xBE79EEA2,0x83244C95,0x49BD6FA6,0x4699799C,0x2B6BEC59,0x2845B239,0xF3D95620,0x66E12D94};
#ifndef PKE_HP
unsigned int const secp256r1_n_n1[1]     = {0xEE00BC4F};
#endif

//[2^128]G
#ifdef PKE_HP
unsigned int secp256r1_2_128_Gx[8] = {0xD789BD85,0x57C84FC9,0xC297EAC3,0xFC35FF7D,0x88C6766E,0xFB982FD5,0xEEDB5E67,0x447D739B};
unsigned int secp256r1_2_128_Gy[8] = {0x72E25B32,0x0C7E33C9,0xA7FAE500,0x3D349B95,0x3A4AAFF7,0xE12E9D95,0x834131EE,0x2D4825AB};
#endif

#ifdef PKE_HP
eccp_curve_t secp256r1[1]={
	{
		256,
		256,
		(unsigned int *)secp256r1_p,
		NULL, //(unsigned int *)secp256r1_p_h,
		(unsigned int *)secp256r1_a,
		(unsigned int *)secp256r1_b,
		(unsigned int *)secp256r1_Gx,
		(unsigned int *)secp256r1_Gy,
		(unsigned int *)secp256r1_n,
		NULL, //(unsigned int *)secp256r1_n_h,
		(unsigned int *)secp256r1_2_128_Gx,
		(unsigned int *)secp256r1_2_128_Gy,
	},
};
#else
eccp_curve_t secp256r1[1]={
	{
		256,
		256,
		(unsigned int *)secp256r1_p,
		NULL, //(unsigned int *)secp256r1_p_h,
		NULL, //(unsigned int *)secp256r1_p_n1,
		(unsigned int *)secp256r1_a,
		(unsigned int *)secp256r1_b,
		(unsigned int *)secp256r1_Gx,
		(unsigned int *)secp256r1_Gy,
		(unsigned int *)secp256r1_n,
		NULL, //(unsigned int *)secp256r1_n_h,
		NULL, //(unsigned int *)secp256r1_n_n1,
	},
};
#endif
#endif


/**************************** secp384r1 ******************************/
#ifdef SUPPORT_SECP384R1
unsigned int const secp384r1_p[12]       = {0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,
										0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#ifdef PKE_HP
unsigned int const secp384r1_p_h[12]     = {0x00000000,0xFFFFFFFE,0x00000002,0x00000001,0xFFFFFFFD,0xFFFFFFFD,
										0x00000002,0x00000003,0x00000002,0xFFFFFFFE,0x00000000,0x00000002};
#else
unsigned int const secp384r1_p_h[12]     = {0x00000001,0xFFFFFFFE,0x00000000,0x00000002,0x00000000,0xFFFFFFFE,
										0x00000000,0x00000002,0x00000001,0x00000000,0x00000000,0x00000000};
unsigned int const secp384r1_p_n1[1]     = {1};
#endif
unsigned int const secp384r1_a[12]       = {0xFFFFFFFC,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,
										0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp384r1_b[12]       = {0xD3EC2AEF,0x2A85C8ED,0x8A2ED19D,0xC656398D,0x5013875A,0x0314088F,
										0xFE814112,0x181D9C6E,0xE3F82D19,0x988E056B,0xE23EE7E4,0xB3312FA7};
unsigned int const secp384r1_Gx[12]      = {0x72760AB7,0x3A545E38,0xBF55296C,0x5502F25D,0x82542A38,0x59F741E0,
										0x8BA79B98,0x6E1D3B62,0xF320AD74,0x8EB1C71E,0xBE8B0537,0xAA87CA22};
unsigned int const secp384r1_Gy[12]      = {0x90EA0E5F,0x7A431D7C,0x1D7E819D,0x0A60B1CE,0xB5F0B8C0,0xE9DA3113,
										0x289A147C,0xF8F41DBD,0x9292DC29,0x5D9E98BF,0x96262C6F,0x3617DE4A};
unsigned int const secp384r1_n[12]       = {0xCCC52973,0xECEC196A,0x48B0A77A,0x581A0DB2,0xF4372DDF,0xC7634D81,
										0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#ifdef PKE_HP
unsigned int const secp384r1_n_h[12]     = {0xAE87B9E7,0x7FDB954A,0xB6F62810,0x8C23F7F3,0xAAAE6873,0xE99276EA,
										0xB33C33C5,0xD558BFBC,0xEED33213,0x3B277D79,0xF4A0E792,0xCC9601F9};
#else
unsigned int const secp384r1_n_h[12]     = {0x19B409A9,0x2D319B24,0xDF1AA419,0xFF3D81E5,0xFCB82947,0xBC3E483A,
										0x4AAB1CC5,0xD40D4917,0x28266895,0x3FB05B7A,0x2B39BF21,0x0C84EE01};
unsigned int const secp384r1_n_n1[1]     = {0xE88FDC45};
#endif

//[2^192]G
#ifdef PKE_HP
unsigned int secp384r1_2_192_Gx[12] = {0xAA03BD53,0xA628B09A,0xA4F52D78,0xBA065458,0x4D10DDEA,0xDB298789,
								   0x8A3E297D,0xB42A31AF,0x06421279,0x40F7F9E7,0x800119C4,0xC19E0B4C};
unsigned int secp384r1_2_192_Gy[12] = {0xE6C88C41,0x822D0FC5,0xE639D858,0xAF68AA6D,0x35F6EBF2,0xC1C7CAD1,
								   0xE3567AF9,0x577A30EA,0x1F5B77F6,0xE5A0191D,0x0356B301,0x16F3FDBF};
#endif

#ifdef PKE_HP
eccp_curve_t secp384r1[1]={
	{
		384,
		384,
		(unsigned int *)secp384r1_p,
		(unsigned int *)secp384r1_p_h,
		(unsigned int *)secp384r1_a,
		(unsigned int *)secp384r1_b,
		(unsigned int *)secp384r1_Gx,
		(unsigned int *)secp384r1_Gy,
		(unsigned int *)secp384r1_n,
		(unsigned int *)secp384r1_n_h,
		(unsigned int *)secp384r1_2_192_Gx,
		(unsigned int *)secp384r1_2_192_Gy,
	},
};
#else
eccp_curve_t secp384r1[1]={
	{
		384,
		384,
		(unsigned int *)secp384r1_p,
		(unsigned int *)secp384r1_p_h,
		(unsigned int *)secp384r1_p_n1,
		(unsigned int *)secp384r1_a,
		(unsigned int *)secp384r1_b,
		(unsigned int *)secp384r1_Gx,
		(unsigned int *)secp384r1_Gy,
		(unsigned int *)secp384r1_n,
		(unsigned int *)secp384r1_n_h,
		(unsigned int *)secp384r1_n_n1,
	},
};
#endif
#endif


/**************************** brainpoolp512r1 ******************************/
#ifdef SUPPORT_BRAINPOOLP512R1
unsigned int const brainpoolp512r1_p[16]     = {0x583A48F3,0x28AA6056,0x2D82C685,0x2881FF2F,0xE6A380E6,0xAECDA12A,0x9BC66842,0x7D4D9B00,
											0x70330871,0xD6639CCA,0xB3C9D20E,0xCB308DB3,0x33C9FC07,0x3FD4E6AE,0xDBE9C48B,0xAADD9DB8};
unsigned int const brainpoolp512r1_p_h[16]   = {0x6158F205,0x49AD144A,0x27157905,0x793FB130,0x905AFFD3,0x53B7F9BC,0x83514A25,0xE0C19A77,
											0xD5898057,0x19486FD8,0xD42BFF83,0xA16DAA5F,0x2056EECC,0x202E1940,0xA9FF6450,0x3C4C9D05};
#ifndef PKE_HP
unsigned int const brainpoolp512r1_p_n1[1]   = {0x7D89EFC5};
#endif
unsigned int const brainpoolp512r1_a[16]     = {0x77FC94CA,0xE7C1AC4D,0x2BF2C7B9,0x7F1117A7,0x8B9AC8B5,0x0A2EF1C9,0xA8253AA1,0x2DED5D5A,
											0xEA9863BC,0xA83441CA,0x3DF91610,0x94CBDD8D,0xAC234CC5,0xE2327145,0x8B603B89,0x7830A331};
unsigned int const brainpoolp512r1_b[16]     = {0x8016F723,0x2809BD63,0x5EBAE5DD,0x984050B7,0xDC083E67,0x77FC94CA,0xE7C1AC4D,0x2BF2C7B9,
											0x7F1117A7,0x8B9AC8B5,0x0A2EF1C9,0xA8253AA1,0x2DED5D5A,0xEA9863BC,0xA83441CA,0x3DF91610};
unsigned int const brainpoolp512r1_Gx[16]    = {0xBCB9F822,0x8B352209,0x406A5E68,0x7C6D5047,0x93B97D5F,0x50D1687B,0xE2D0D48D,0xFF3B1F78,
											0xF4D0098E,0xB43B62EE,0xB5D916C1,0x85ED9F70,0x9C4C6A93,0x5A21322E,0xD82ED964,0x81AEE4BD};
unsigned int const brainpoolp512r1_Gy[16]    = {0x3AD80892,0x78CD1E0F,0xA8F05406,0xD1CA2B2F,0x8A2763AE,0x5BCA4BD8,0x4A5F485E,0xB2DCDE49,
											0x881F8111,0xA000C55B,0x24A57B1A,0xF209F700,0xCF7822FD,0xC0EABFA9,0x566332EC,0x7DDE385D};
unsigned int const brainpoolp512r1_n[16]     = {0x9CA90069,0xB5879682,0x085DDADD,0x1DB1D381,0x7FAC1047,0x41866119,0x4CA92619,0x553E5C41,
											0x70330870,0xD6639CCA,0xB3C9D20E,0xCB308DB3,0x33C9FC07,0x3FD4E6AE,0xDBE9C48B,0xAADD9DB8};
unsigned int const brainpoolp512r1_n_h[16]   = {0xCDA81671,0xD2A3681E,0x95283DDD,0x0886B758,0x33B7627F,0x3EC64BD0,0x2F0207E8,0xA6F230C7,
											0x3B790DE3,0xD7F9CC26,0x2F16BBDF,0x723C37A2,0x194B2E56,0x95DF1B4C,0x718407B0,0xA794586A};
#ifndef PKE_HP
unsigned int const brainpoolp512r1_n_n1[1]   = {0x0F1B7027};
#endif

//[2^256]G
#ifdef PKE_HP
unsigned int brainpoolp512r1_2_256_Gx[16] = {0xEF66EFB5,0xCA448127,0x08AC7DA5,0x9D189C54,0x730A3721,0x97502F33,0xACA3B94E,0x1BAE0F47,
										 0x945E8460,0x79A83D9B,0x8F284870,0xC056FD65,0xB8F1B2AF,0xD5262CE1,0x66C4ED95,0x7B5913F7};
unsigned int brainpoolp512r1_2_256_Gy[16] = {0xC2F45872,0xD610453C,0xDA0092C9,0x3E9470F8,0x4EB496C3,0x24EC6F84,0xBC578877,0xFB4EDB0D,
										 0x5FC450A8,0xE46656A7,0x61306AF0,0xC21FCA91,0xEA20B708,0x3727EA1E,0x4FFC593A,0x3B4D8E45};
#endif

#ifdef PKE_HP
eccp_curve_t brainpoolp512r1[1]={
	{
		512,
		512,
		(unsigned int *)brainpoolp512r1_p,
		(unsigned int *)brainpoolp512r1_p_h,
		(unsigned int *)brainpoolp512r1_a,
		(unsigned int *)brainpoolp512r1_b,
		(unsigned int *)brainpoolp512r1_Gx,
		(unsigned int *)brainpoolp512r1_Gy,
		(unsigned int *)brainpoolp512r1_n,
		(unsigned int *)brainpoolp512r1_n_h,     //NULL,//
		(unsigned int *)brainpoolp512r1_2_256_Gx,
		(unsigned int *)brainpoolp512r1_2_256_Gy,
	},
};
#else
eccp_curve_t brainpoolp512r1[1]={
	{
		512,
		512,
		(unsigned int *)brainpoolp512r1_p,
		(unsigned int *)brainpoolp512r1_p_h,
		(unsigned int *)brainpoolp512r1_p_n1,
		(unsigned int *)brainpoolp512r1_a,
		(unsigned int *)brainpoolp512r1_b,
		(unsigned int *)brainpoolp512r1_Gx,
		(unsigned int *)brainpoolp512r1_Gy,
		(unsigned int *)brainpoolp512r1_n,
		(unsigned int *)brainpoolp512r1_n_h,     //NULL,//
		(unsigned int *)brainpoolp512r1_n_n1,    //NULL,//
	},
};
#endif
#endif


/**************************** secp521r1 ******************************/
#ifdef SUPPORT_SECP521R1
unsigned int const secp521r1_p[17]        = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
										 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
										 0x000001FF};
#ifdef PKE_HP
unsigned int const secp521r1_p_h[17]      = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
										 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00004000,
										 0x00000000};
#else
unsigned int const secp521r1_p_h[17]      = {0x00000000,0x00004000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
										 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
										 0x00000000};
unsigned int const secp521r1_p_n1[1]      = {1};
#endif
unsigned int const secp521r1_a[17]        = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
										 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
										 0x000001FF};
unsigned int const secp521r1_b[17]        = {0x6B503F00,0xEF451FD4,0x3D2C34F1,0x3573DF88,0x3BB1BF07,0x1652C0BD,0xEC7E937B,0x56193951,
										 0x8EF109E1,0xB8B48991,0x99B315F3,0xA2DA725B,0xB68540EE,0x929A21A0,0x8E1C9A1F,0x953EB961,
										 0x00000051};
unsigned int const secp521r1_Gx[17]       = {0xC2E5BD66,0xF97E7E31,0x856A429B,0x3348B3C1,0xA2FFA8DE,0xFE1DC127,0xEFE75928,0xA14B5E77,
										 0x6B4D3DBA,0xF828AF60,0x053FB521,0x9C648139,0x2395B442,0x9E3ECB66,0x0404E9CD,0x858E06B7,
										 0x000000C6};
unsigned int const secp521r1_Gy[17]       = {0x9FD16650,0x88BE9476,0xA272C240,0x353C7086,0x3FAD0761,0xC550B901,0x5EF42640,0x97EE7299,
										 0x273E662C,0x17AFBD17,0x579B4468,0x98F54449,0x2C7D1BD9,0x5C8A5FB4,0x9A3BC004,0x39296A78,
										 0x00000118};
unsigned int const secp521r1_n[17]        = {0x91386409,0xBB6FB71E,0x899C47AE,0x3BB5C9B8,0xF709A5D0,0x7FCC0148,0xBF2F966B,0x51868783,
										 0xFFFFFFFA,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
										 0x000001FF};
#ifdef PKE_HP
unsigned int const secp521r1_n_h[17]      = {0x60EC0915,0xABEE0E49,0xA4DEB96A,0x39B88F86,0x6440173A,0x36439BEA,0x53EBDD25,0xABED7C82,
										 0x7A1814F8,0xDB25B111,0xB2DD5C97,0xE290362F,0x158143E2,0xA9EA49FF,0x141CEAE0,0x07E35810,
										 0x000001CD};
#else
unsigned int const secp521r1_n_h[17]      = {0x61C64CA7,0x1163115A,0x4374A642,0x18354A56,0x0791D9DC,0x5D4DD6D3,0xD3402705,0x4FB35B72,
										 0xB7756E3A,0xCFF3D142,0xA8E567BC,0x5BCC6D61,0x492D0D45,0x2D8E03D1,0x8C44383D,0x5B5A3AFE,
										 0x0000019A};
unsigned int const secp521r1_n_n1[1]      = {0x79A995C7};
#endif

//[2^260]G
#ifdef PKE_HP
unsigned int const secp521r1_2_260_Gx[17] = {0x9185544D,0x6D9B0C3C,0x8DF2765F,0xAD21890E,0xCBE030A2,0x47836EE3,0xF7651AED,0x606B9133,
										 0x71C00932,0xB1A31586,0xCFE05F47,0x9806A369,0xF57F3700,0xC2EBC613,0xF065F07C,0x1022D6D2,
										 0x00000109};
unsigned int const secp521r1_2_260_Gy[17] = {0x514C45ED,0xB292C583,0x947E68A1,0x89AC5BF2,0xAF507C14,0x633C4300,0x7DA4020A,0x943D7BA5,
										 0xC0ED8274,0xD1E90C7A,0xE59426E6,0x9634868C,0xC26BC9DE,0x24A6FFF2,0x152416CD,0x1A012168,
										 0x0000000C};
#endif

#ifdef PKE_HP
eccp_curve_t secp521r1[1] = {
	{
		521,
		521,
		(unsigned int *)secp521r1_p,
		(unsigned int *)secp521r1_p_h,  //NULL,//
		(unsigned int *)secp521r1_a,
		(unsigned int *)secp521r1_b,
		(unsigned int *)secp521r1_Gx,
		(unsigned int *)secp521r1_Gy,
		(unsigned int *)secp521r1_n,
		(unsigned int *)secp521r1_n_h,
		(unsigned int *)secp521r1_2_260_Gx,
		(unsigned int *)secp521r1_2_260_Gy,
	},
};
#else
eccp_curve_t secp521r1[1] = {
	{
		521,
		521,
		(unsigned int *)secp521r1_p,
		(unsigned int *)secp521r1_p_h,  //NULL,//
		(unsigned int *)secp521r1_p_n1, //NULL,//
		(unsigned int *)secp521r1_a,
		(unsigned int *)secp521r1_b,
		(unsigned int *)secp521r1_Gx,
		(unsigned int *)secp521r1_Gy,
		(unsigned int *)secp521r1_n,
		(unsigned int *)secp521r1_n_h,
		(unsigned int *)secp521r1_n_n1,
	},
};
#endif
#endif



/********************************************************************************************************
 * @file    app_rsa.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
/********* pke version:1.1 *********/
#include "app_config.h"


#if (MCU_CORE_B89)
#if (PKE_TEST_MODE == PKE_RSA)

#if 1

unsigned int rsa_modexp_1024_test(void)
{
	unsigned int n[]=
	{
		0xFEA18675,0x1538B245,0x84E8E165,0x91BEB0D8,0x9D0B6997,0x1D7B833A,0xEFA2C3F6,0x7DF45F14,
		0x55565D78,0x69230B98,0x591F158B,0xE106BDD8,0x26AF59F0,0xE430C55E,0xB9EE5D48,0xC74A11EF,
		0x16D22D06,0xC453225E,0xEF660075,0x7119E5DB,0x57111778,0x66DAC542,0x0E2A4D2E,0xA6A47646,
		0x04D816F2,0xB5704D41,0x282C21EE,0x9D950126,0xA69A2B1C,0xEADB452B,0x0CF439F0,0xBEC7A940,
	};
	unsigned int e[]= {0x00010001};
	unsigned int d[]=
	{
		0x39CD7DE1,0x765A9FBE,0x28A3F778,0x9C0BBB36,0x92F2B6D6,0xA7B0D6E6,0x4AC3AC9D,0x495F0629,
		0x2B15D590,0x1DEA94F0,0x09655DB7,0x4A2A74BE,0xA830AEE4,0xA5C64770,0x36013850,0xF0667BA8,
		0xF32A533E,0x882FD8F7,0x045FBEC6,0x132E91D0,0xC83A5CE5,0xEBCEC702,0xE2698BC8,0xA8CE2AE4,
		0xB91FAE10,0xA06B202B,0xD8013303,0x1E06979A,0xC5FAE3E4,0xDAD7E244,0xD8366EDA,0xBC5D2053,
	};
	unsigned int p[]=
	{
		0x0C3B82FD,0x563A7F00,0x4C43666F,0xBC274F20,0x30247539,0x071A594D,0x0E069034,0x184B665F,
		0x8421D20C,0xBD3B7A16,0x0F45B041,0x6B1BA853,0xEC556813,0x3B21167E,0x2D08AE4A,0xE5324D00,
	};
	unsigned int q[]=
	{
		0x7A70D6D9,0xD91EB87C,0xA610AF00,0x2664B357,0x250F0450,0x963FBF1F,0xC423CD7D,0xE12EA919,
		0x1B0EB235,0x7F77F947,0x4A17C833,0x84715F7A,0xF264953B,0xC0695F17,0x75796707,0xD5173E2C,
	};
	unsigned int dp[]=
	{
		0x8AF1926D,0xE6B3D129,0x2BCEE822,0x8F142AB6,0xA9A1922B,0x36FB2DD9,0x50BA4D2D,0x43D30545,
		0xF4A72E1B,0xFB17B678,0x117F6668,0x3E3DB8E1,0x49818B30,0x26BB5E12,0x949AB54F,0x14B89077,
	};
	unsigned int dq[]=
	{
		0x69E1C5B1,0x12B84DA8,0x823396C5,0xF1EB5D37,0x8E8FF40D,0x9260C390,0x48E1B1D6,0xFCA4FE82,
		0x2F02230F,0x38A57BF4,0x99B73F12,0xCA14A8FF,0xCB3D116D,0xDCAA4637,0x7C7AA349,0x4A1674B5,
	};
	unsigned int u[]=
	{
		0xA2964E5B,0xF37FB26D,0x8B15254C,0xCAFAAD33,0xE3B1270B,0xCD27513C,0x7385C488,0x48899F5B,
		0x2234D1AA,0x893B7E4D,0xB343300B,0xEDA155DE,0x53C472E3,0x1602F8E0,0xBFE44F44,0x3FB0D177,
	};
	unsigned int std_out[]=
	{
		0x6918BEE1,0x5A78A5AE,0x537C93F8,0xBADE3C88,0x906FD649,0x082DA994,0x10BC7D78,0xE02469F7,
		0x20F71F91,0x9D56188D,0xA1933733,0x3A0FE3FD,0xFA551EC2,0x0F7BD1AC,0xDA221E31,0x60DA6F4F,
		0xC424F841,0xDB256AAD,0xB3C3B259,0x5480DDC6,0xBCF51563,0x9A245963,0x3106369F,0xDAD4C504,
		0x9C960A3B,0x357044F9,0x95F5521A,0x9F6D9285,0xC13468B2,0x8CD09D47,0x45A4B220,0x5351A82E,
	};


	unsigned int in[128], out[128], out2[128];
	unsigned int eBitLen = 17;
	unsigned int nBitLen = 1024;
	unsigned int wordLen=(nBitLen+31)/32;
	unsigned int ret;

	//set input
	memset(in, 0x5a, wordLen<<2);
	//sign(non-CRT mode)
printf("\r\n RSA_ModExp start!");
	ret = RSA_ModExp(in, d, n, out, get_valid_bits(d, wordLen), nBitLen);
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA MOD EXP 1024 test failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 1024 test success");
	}
	//sign(CRT mode)
	memset(out, 0, wordLen<<2);
printf("\r\n RSA_CRTModExp start!");
	ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, std_out, wordLen))
	{
		printf("\r\n RSA CRT MOD EXP 1024 test failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA CRT MOD EXP 1024 test success");
	}

	//verify
printf("\r\n RSA_ModExp start!");
	ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
	if(RSA_SUCCESS != ret || uint32_BigNumCmp(out2, wordLen, in, wordLen))
	{
		printf("\r\n RSA MOD EXP 1024 test failure, ret=%d", ret);
		return 1;
	}
	else
	{
		printf("\r\n RSA MOD EXP 1024 test success");
	}

	return 0;
}

unsigned int rsa_test(void)
{
	unsigned int e[128]={0};
	unsigned int d[128];
	unsigned int n[128];
	unsigned int p[128];
	unsigned int q[128];
	unsigned int dp[128];
	unsigned int dq[128];
	unsigned int u[128];
//	unsigned int in[128];
//	unsigned int out[128];
//	unsigned int out2[128];
	unsigned int eBitLen=17;
	unsigned int nBitLen=1024;
//	unsigned int eWordLen=(eBitLen+0x1F)>>5;
//	unsigned int nWordLen=(nBitLen+0x1F)>>5;
//	unsigned int pWordLen=(nBitLen/2+0x1F)>>5;
	unsigned int ret;
	unsigned int i;

#if 1
	//RSA NON-CRT
	eBitLen=17;
	nBitLen=1024;//2048;//4096;//
//	eWordLen=(eBitLen+0x1F)>>5;
//	nWordLen=(nBitLen+0x1F)>>5;
//	pWordLen=(nBitLen/2+0x1F)>>5;
	for(i=0;i<2;i++)
	{
		ret = RSA_GetKey(e, d, n, eBitLen, nBitLen);
		if(ret)
		{
			printf("\r\n RSA_GetKey failure, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_GetKey times = %d", i+1);
	}
#endif

#if 1
	//RSA CRT
	eBitLen=17;
	nBitLen=1024;//2048;//4096;//
//	eWordLen=(eBitLen+0x1F)>>5;
//	nWordLen=(nBitLen+0x1F)>>5;
//	pWordLen=(nBitLen/2+0x1F)>>5;
	for(i=0;i<2;i++)
	{
		ret = RSA_GetCRTKey(e, p, q, dp, dq, u, n, eBitLen, nBitLen);
		if(ret)
		{
			printf("\r\n RSA_GetCRTKey failure, ret = %d", ret);
			return 1;
		}
		printf("\r\n RSA_GetCRTKey times = %d", i+1);
	}

#endif

	return 0;
}


unsigned char RSA_all_test(void)
{
	printf("\r\n\r\n =================== rsa test =================== \r\n");

	if(rsa_modexp_1024_test())
		return 1;
	sleep_ms(5);
	if(rsa_test())
		return 1;

	return 0;
}

#endif


void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);
    gpio_set_func(LED3, AS_GPIO);
    gpio_set_func(LED4, AS_GPIO);

    gpio_set_output_en(LED1, 1);
    gpio_set_output_en(LED2, 1);
    gpio_set_output_en(LED3, 1);
    gpio_set_output_en(LED4, 1);

    gpio_set_input_en(LED1, 0);
    gpio_set_input_en(LED2, 0);
    gpio_set_input_en(LED3, 0);
    gpio_set_input_en(LED4, 0);

    gpio_write(LED1, 1);
    gpio_write(LED2, 1);
    gpio_write(LED3, 1);
    gpio_write(LED4, 1);

	RSA_all_test();
}

void main_loop(void)
{
	sleep_ms(500);
    gpio_toggle(LED1);
    gpio_toggle(LED2);
    gpio_toggle(LED3);
    gpio_toggle(LED4);
}
#endif

#endif

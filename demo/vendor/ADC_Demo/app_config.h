/********************************************************************************************************
 * @file	app_config.h
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
#pragma once
#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif





#if (MCU_CORE_B89)
#define ADC_BASE_MODE			    1	//GPIO voltage
#define ADC_VBAT_CHANNEL_MODE		2	//Vbat channel Battery Voltage
#define ADC_TEMP_MODE_EE			3	//Temp test
#define ADC_RNG_MODE			    4	//Random number Gernerator,to use this Random mode need set TRNG_MODE_ENABLE=0 in random.h file.(By lirui)

#define ADC_MODE				    ADC_BASE_MODE

#define MANNUAL_MODE_GET_ADC_SAMPLE_RESULT		0	//Vbat channel Battery Voltage mannual mode

#elif (MCU_CORE_B87)
#define ADC_BASE_MODE			    1	//GPIO voltage
#define ADC_VBAT_MODE			    2	//Battery Voltage
#define ADC_VBAT_CHANNEL_MODE		3	//Vbat channel Battery Voltage
#define ADC_TEMP_MODE_EE			4	//Temp test
#define ADC_RNG_MODE			    5	//Random number Gernerator,to use this Random mode need set TRNG_MODE_ENABLE=0 in random.h file.(By lirui)

#define ADC_MODE				    ADC_BASE_MODE

#define MANNUAL_MODE_GET_ADC_SAMPLE_RESULT		0	//Vbat channel Battery Voltage mannual mode

#elif (MCU_CORE_B85)
#define ADC_BASE_MODE			    1	//GPIO voltage
#define ADC_VBAT_MODE			    2	//Battery Voltage
#define ADC_RNG_MODE			    3	//Random number Gernerator,to use this Random mode need set TRNG_MODE_ENABLE=0 in random.h file.(By lirui)

#define ADC_MODE				    ADC_VBAT_MODE
#endif












/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	24000000

#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#elif ((CLOCK_SYS_CLOCK_HZ==32000000) && (MCU_CORE_B85 || MCU_CORE_B87))
	#define SYS_CLK  	SYS_CLK_32M_Crystal
#elif ((CLOCK_SYS_CLOCK_HZ==48000000) && (MCU_CORE_B85 || MCU_CORE_B87))
	#define SYS_CLK  	SYS_CLK_48M_Crystal
#endif

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};







/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

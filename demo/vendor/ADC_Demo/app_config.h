/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#define ADC_MODE				    ADC_BASE_MODE
#define MANNUAL_MODE_GET_ADC_SAMPLE_RESULT		0	// mannual mode
#elif (MCU_CORE_B80)
#define ADC_BASE_MODE			    1	//GPIO voltage
#define ADC_VBAT_CHANNEL_MODE		2	//Vbat channel Battery Voltage
#define ADC_TEMP_MODE_EE			3	//Temp test
#define ADC_RNG_MODE			    4	//Random number Gernerator

#define ADC_MODE				    ADC_BASE_MODE

#define ADC_INTER_TEST				0
#define MANNUAL_MODE_GET_ADC_SAMPLE_RESULT		0	//Vbat channel Battery Voltage mannual mode
#endif












/* Define system clock */
#define CLOCK_SYS_CLOCK_HZ  	24000000

#if(MCU_CORE_B89)
#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#endif
#else
#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==32000000)
	#define SYS_CLK  	SYS_CLK_32M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==48000000)
	#define SYS_CLK  	SYS_CLK_48M_Crystal
#endif
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

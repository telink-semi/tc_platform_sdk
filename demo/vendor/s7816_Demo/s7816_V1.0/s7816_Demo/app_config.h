/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for B85m
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
#pragma once
#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#if (MCU_CORE_B80)
#define LED1     		        GPIO_PA4
#define LED2     		        GPIO_PA5
#define LED3     		        GPIO_PA6
#define LED4     		        GPIO_PA7
#define S7816_TRX_PIN			GPIO_PB2
#define S7816_VCC_PIN			GPIO_PB3
#define S7816_RST_PIN			GPIO_PB4
#define S7816_CLK_PIN			GPIO_PB5
#elif (MCU_CORE_B80B)
#define LED1                    GPIO_PA4
#define LED2                    GPIO_PA5
#define LED3                    GPIO_PA6
#define LED4                    GPIO_PA7
#define S7816_TRX_PIN           GPIO_PB2
#define S7816_VCC_PIN           GPIO_PB3
#define S7816_RST_PIN           GPIO_PB4
#define S7816_CLK_PIN           GPIO_PB5
#elif (MCU_CORE_B85)
#define LED1     		        GPIO_PD2
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5
#define S7816_TRX_PIN			S7816_TRX_D0
#define S7816_VCC_PIN			GPIO_PB0
#define S7816_RST_PIN			GPIO_PB1
#define S7816_CLK_PIN			S7817_CLK_PA1 //the clk-source of s7816 is 24M.
#elif (MCU_CORE_B87)
#define LED1     		        GPIO_PD2
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5
#define S7816_TRX_PIN			S7816_TRX_D0
#define S7816_VCC_PIN			GPIO_PB0
#define S7816_RST_PIN			GPIO_PB1
#define S7816_CLK_PIN			S7817_CLK_PA1
#elif (MCU_CORE_B89)
#define LED1     		        GPIO_PD0
#define LED2     		        GPIO_PD1
#define LED3     		        GPIO_PD2
#define LED4     		        GPIO_PD3
#define S7816_TRX_PIN			S7816_TRX_PD3
#define S7816_VCC_PIN			GPIO_PB5
#define S7816_RST_PIN			GPIO_PB6
#define S7816_CLK_PIN			S7817_CLK_PB2
#endif



#define F   372      //clock frequency regulator ,372 is the initialized  value.
#define D    1       //bitrate regulator,1 is the initialized value.


#define S7816_RX_BUFF_LEN   48

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
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

#if (MCU_CORE_B80)
#define LED1     		        GPIO_PA4
#define LED2     		        GPIO_PA5
#define LED3     		        GPIO_PA6
#define LED4     		        GPIO_PA7
#define UART_TX_PIN				GPIO_PD0
#define UART_RX_PIN				GPIO_PD1
#define UART_CTS_PIN			GPIO_PD2
#define UART_RTS_PIN			GPIO_PD3
#define UART_RTX_PIN			GPIO_PC7
#elif (MCU_CORE_B85)
#define LED1     		        GPIO_PD2
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5
#define UART_TX_PIN				UART_TX_PA2
#define UART_RX_PIN				UART_RX_PA0
#define UART_CTS_PIN			UART_CTS_PA3
#define UART_RTS_PIN			UART_RTS_PB3
#define UART_RTX_PIN			UART_RTX_PD3//The RTX can be used as UART-TX,but only some of the UART-TX can be used as RTX.
#elif (MCU_CORE_B87)
#define LED1     		        GPIO_PD2
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5
#define UART_TX_PIN				UART_TX_PA2
#define UART_RX_PIN				UART_RX_PA0
#define UART_CTS_PIN			UART_CTS_PA3
#define UART_RTS_PIN			UART_RTS_PA4
#define UART_RTX_PIN			UART_RTX_PD3//The RTX can be used as UART-TX,but only some of the UART-TX can be used as RTX.
#elif (MCU_CORE_B89)
#define LED1     		        GPIO_PD0
#define LED2     		        GPIO_PD1
#define LED3     		        GPIO_PD2
#define LED4     		        GPIO_PD3
#define UART_TX_PIN				UART_TX_PA1
#define UART_RX_PIN				UART_RX_PA0
#define UART_CTS_PIN			UART_CTS_PA3
#define UART_RTS_PIN			UART_RTS_PA2
#define UART_RTX_PIN			UART_RTX_PD3  //The RTX can be used as UART-RX,but only some of the UART-RX can be used as RTX.
#endif


/******************set mode**********************/
#define UART_DMA  		 1     //uart use dma
#define UART_NDMA  		 2     //uart not use dma
#define UART_SOFTWARE_RX 3     //software simulates the UART receiving function,occupy an GPIO interrupt and a hardware timer
#define UART_MODE	 	UART_DMA


/***********set uart_dma interrupt type*************/
#define UART_RXDMA_IRQ   1
#if(MCU_CORE_B80)
#define UART_RXDONE_IRQ  2
#endif
#define UART_DMA_INT_TYPE UART_RXDMA_IRQ




#define NONE            0
#define USE_CTS    		1
#define USE_RTS    		2

#define FLOW_CTR  		NONE

#define UART_1WIRE_MODE       0  //tx and rx is a same line
#define UART_2WIRE_MODE       1  //tx and rx are two lines
#define UART_WIRE_MODE        UART_2WIRE_MODE
#if( FLOW_CTR==USE_CTS)
	#define STOP_VOLT   	1			//0 :Low level stops TX.  1 :High level stops TX.
#elif (FLOW_CTR==USE_RTS)
	#define RTS_MODE		UART_RTS_MODE_AUTO 		    //It can be UART_RTS_MODE_AUTO/UART_RTS_MODE_MANUAL.
	#define RTS_THRESH		5			//UART_RTS_MODE_AUTO need.It indicates RTS trigger threshold.
	#define RTS_INVERT		1			//UART_RTS_MODE_AUTO need.0 indicates RTS_pin will change from low to hign.
	#define RTS_POLARITY	0			//UART_RTS_MODE_MANUAL need. It indicates RTS_POLARITY .
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

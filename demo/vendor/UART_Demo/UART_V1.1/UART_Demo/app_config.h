/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for B80B
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
#include "common.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Define system clock
 */
#define CLOCK_SYS_CLOCK_HZ 24000000

/**
 * @note The sys_clock.h file depends on the definition of CLOCK_SYS_CLOCK_HZ so CLOCK_SYS_CLOCK_HZ must be placed before #include "sys_clock.h" without reversing the order.
 */
#include "sys_clock.h"

#define UART_TX_PIN				GPIO_PD0
#define UART_RX_PIN				GPIO_PD1
#define UART_CTS_PIN			GPIO_PD2
#define UART_RTS_PIN			GPIO_PD3
#define UART_RTX_PIN			GPIO_PC7

#define UART0_MODULE            0 /* UART0 */
#define UART1_MODULE            1 /* UART1 */

/* uart select */
#define UART_MODULE_SEL         UART1_MODULE

/******************set mode**********************/
#define UART_DMA  		 1     //uart use dma
#define UART_NDMA  		 2     //uart not use dma
#define UART_SOFTWARE_RX 3     //software simulates the UART receiving function,occupy an GPIO interrupt and a hardware timer
#define UART_MODE	 	UART_DMA


/***********set uart_dma interrupt type*************/
#define UART_RXDMA_IRQ   1
#define UART_RXDONE_IRQ  2

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
	#define RTS_INVERT		0			//UART_RTS_MODE_AUTO need.1 indicates RTS_pin will change from low to high.
	#define RTS_POLARITY	0			//UART_RTS_MODE_MANUAL need. It indicates RTS_POLARITY .
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

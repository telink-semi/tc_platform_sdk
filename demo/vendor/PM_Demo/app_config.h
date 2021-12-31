/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for b85m
 *
 * @author	Driver Group
 * @date	2018
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

#if (MCU_CORE_B80)
#define LED1     		        GPIO_PB3
#define LED2     		        GPIO_PB4
#define LED3     		        GPIO_PB5
#define LED4     		        GPIO_PB6
#elif (MCU_CORE_B89)
#define LED1     		        GPIO_PD0
#define LED2     		        GPIO_PD1
#define LED3     		        GPIO_PD2
#define LED4     		        GPIO_PD3
#elif (MCU_CORE_B87)
#define LED1     		        GPIO_PD2
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5

#define SW1      		        GPIO_PD1
#define SW2      		        GPIO_PD2

#elif (MCU_CORE_B85)
#define LED1     		        GPIO_PD0
#define LED2     		        GPIO_PD3
#define LED3     		        GPIO_PD4
#define LED4     		        GPIO_PD5

#define SW1      		        GPIO_PD1
#define SW2      		        GPIO_PD2

#endif


/* IDLE MODE */
#define IDLE_TIMER_WAKEUP				1

/* SUSPEND MODE */
#define SUSPEND_PAD_WAKEUP   			2
#define SUSPEND_32K_RC_WAKEUP   		3
#define SUSPEND_32K_XTAL_WAKEUP			4
#if(MCU_CORE_B87)
#define SUSPEND_MDEC_WAKEUP				5
#define SUSPEND_CORE_WAKEUP				6
#endif

/* DEEPSLEEP MODE */
#define DEEP_PAD_WAKEUP		 			7
#define DEEP_32K_RC_WAKEUP      		8
#define DEEP_32K_XTAL_WAKEUP      		9
#if(MCU_CORE_B89)
#define DEEP_DEBOUNCE_WAKEUP      		10
#elif(MCU_CORE_B87)
#define DEEP_MDEC_WAKEUP      			10
#endif

/* DEEPSLEEP WITH RETENTION MODE */
#define DEEP_RET_PAD_WAKEUP     		11
#define DEEP_RET_32K_RC_WAKEUP     		12
#define DEEP_RET_32K_XTAL_WAKEUP     	13
#if(MCU_CORE_B89)
#define DEEP_RET_DEBOUNCE_WAKEUP      	14
#elif(MCU_CORE_B87)
#define DEEP_RET_MDEC_WAKEUP      		14
#endif

#if(MCU_CORE_B89)
/* SHUTDOWN_MODE */
#define	SHUTDOWN_PAD_WAKEUP				15
#endif

#define PM_MODE			     			SUSPEND_PAD_WAKEUP


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

/* List tick per second/millisecond/microsecond */
enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,				///< system tick per 1 second
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),		///< system tick per 1 millisecond
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),   ///< system tick per 1 microsecond
};












/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

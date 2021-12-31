/********************************************************************************************************
 * @file	bqb.h
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
#ifndef BQB_H_
#define BQB_H_

#include "../app_config.h"

#if(TEST_DEMO==BQB_DEMO)

/*Set to 1, open the code related to BQB tool configuration (uart port, CAP value, TX Power, PA port, calibration position, etc.).*/
#define SUPPORT_CONFIGURATION			0

/* set power of Tx */
#if MCU_CORE_B85
#define BQB_TX_POWER					RF_POWER_P7p02dBm
#elif MCU_CORE_B87
#define BQB_TX_POWER					RF_POWER_P6p91dBm
#elif MCU_CORE_B89
#define BQB_TX_POWER					RF_POWER_P4p98dBm
#elif(MCU_CORE_B80)
#define BQB_TX_POWER					RF_POWER_P6p97dBm
#endif

#define ACCESS_CODE        	0x29417671

/* set configuration flash address */
#if SUPPORT_CONFIGURATION
#define CONFIGURATION_ADDR_UART_TX		0x12
#define	CONFIGURATION_ADDR_UART_RX		0x13
#define CONFIGURATION_ADDR_PA_TX		0x14
#define	CONFIGURATION_ADDR_PA_RX		0x15
#define CONFIGURATION_ADDR_CAP			0x16
#define	CONFIGURATION_ADDR_POWER		0x17
#endif

/* set flash address to set internal cap value and switch internal/external cap */
#define CAP_SET_FLASH_ADDR_2M 			0x1fe000
#define CAP_SET_FLASH_ADDR_1M 			0xfe000
#define CAP_SET_FLASH_ADDR_512K 		0x77000 //B85m:0x77000 B91m:0x7e000
#define CAP_SET_FLASH_ADDR_128K 		0x1e000
#define CAP_SET_FLASH_ADDR_64K			0xe000

#if MCU_CORE_B80
#define CAP_SET_OTP_16K					0x3fc8 //0x3fc8-0x3fcb,32bit
#endif

//#define TP0_CALIBRATION_SRAM		0x04
//#define TP1_CALIBRATION_SRAM		0x05
#define CAP_CALIBRATION_SRAM		0x06

#if !SUPPORT_CONFIGURATION
#define SWITCH_INTERNAL_CAP 			1		// 0: external cap, 1: internal cap
#define CAP_SET_FLASH_ADDR 				CAP_SET_FLASH_ADDR_128K
#endif

/* set uart port */
#if (MCU_CORE_B85 || MCU_CORE_B87)
#define BQB_UART_TX_PORT   				UART_TX_PA2
#define BQB_UART_RX_PORT   				UART_RX_PB0
#elif(MCU_CORE_B89)
#define BQB_UART_TX_PORT   				UART_TX_PB4
#define BQB_UART_RX_PORT   				UART_RX_PB3
#elif(MCU_CORE_B80)
#define BQB_UART_TX_PORT   				GPIO_PA7
#define BQB_UART_RX_PORT   				GPIO_PA6
#endif
#define BQB_UART_BUAD	   	115200

/* set pa port */
#if !SUPPORT_CONFIGURATION
#define BQB_PA_TX_PORT   	GPIO_PA0
#define BQB_PA_RX_PORT   	GPIO_PA0
#endif

/* set the length of preamble */
#define BQB_PREAMBLE_LEN   		6
/* set TX return result */
#define RETURN_TX_LEN_EN    0          			//1: return tx length, 0:not return tx length

/*set calibration position*/
#define SWITCH_CALIBRATION_POSITION		1		//1:FLASH, 2:SRAM

#if SUPPORT_CONFIGURATION
/**
 *  @brief  union structure for configuration
 */
typedef struct
{
	unsigned char uart_tx;
	unsigned char uart_rx;
	unsigned char pa_tx;
	unsigned char pa_rx;
	unsigned char flash:7;
	unsigned char cap:1;
	unsigned char cal_pos:2;
	unsigned char power_mode:2;
	unsigned char power;
}usr_def_t;

/* global value for configuration */
extern usr_def_t usr_config;
#define get_pin(value) (((unsigned short)((value) >> 3) << 8) | BIT((value) & 0x07))
#endif


/**
 *  @brief  command type for BQB Test
 */
#define	CMD_SETUP			0
#define	CMD_RX_TEST			1
#define	CMD_TX_TEST			2
#define	CMD_END				3

/**
 *  @brief  Event type for BQB Test
 */
#define STATUS_EVENT_MASK	(0)
#define REPORT_EVENT_MASK	(BIT(15))

#define DEBUG_FLAG			0
#define LED1     			GPIO_PB2


/**
 *  @brief  command status for BQB Test
 */
typedef enum {
	SETUP_STATE=0x10,
	RX_STATE,
	TX_STATE,
	END_STATE
}Test_Status_t;


/**
 * @brief   This function serves to initialize  BQB
 * @param   none.
 * @return  none.
 */
void  bqbtest_init(void);



/**
 * @brief   This function serves to read the usrt data and execute BQB program
 * @param   none.
 * @return  none.
 */
void bqb_serviceloop (void);


extern void bqb_pa_init();

extern void bqb_pa_set_mode(unsigned char rtx); //0:rx, 1:tx, other:off

#endif

#endif /* BQB_H_ */

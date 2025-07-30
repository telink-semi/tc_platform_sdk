/********************************************************************************************************
 * @file    bqb.h
 *
 * @brief   This is the header file for Telink MCU
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
#ifndef BQB_H_
#define BQB_H_

#include "../app_config.h"
#include "calibration.h"

#if(TEST_DEMO==BQB_DEMO)

/*Set to 1, open the code related to BQB tool configuration (uart port, CAP value, TX Power, PA port, calibration position, etc.).*/
#define SUPPORT_CONFIGURATION			0

/* set power of Tx */
#if MCU_CORE_B85
#define BQB_TX_POWER					RF_POWER_P7p02dBm
#elif MCU_CORE_B87
#define BQB_TX_POWER					RF_POWER_P6p91dBm
#elif MCU_CORE_B89
#define BQB_TX_POWER					RF_POWER_INDEX_P4p98dBm
#elif(MCU_CORE_B80)
#define BQB_TX_POWER					RF_POWER_P6p97dBm
#elif MCU_CORE_B80B
#define BQB_TX_POWER                    RF_POWER_P6p97dBm
#elif MCU_CORE_TC321X
#define BQB_TX_POWER                    RF_POWER_P7p00dBm
#elif MCU_CORE_TC122X
#define BQB_TX_POWER                    RF_POWER_P7p00dBm
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
#define CAP_SET_FLASH_ADDR_512K 		0x77000 //Telink MCU:0x77000 Telink RISC-V:0x7e000
#define CAP_SET_FLASH_ADDR_128K 		0x1e000
#define CAP_SET_FLASH_ADDR_64K			0xe000

#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X)
#define CAP_SET_OTP_16K					0x3fc8 //0x3fc8-0x3fcb,32bit
#endif

//#define TP0_CALIBRATION_SRAM		0x04
//#define TP1_CALIBRATION_SRAM		0x05
#define CAP_CALIBRATION_SRAM		0x06

#if !SUPPORT_CONFIGURATION
#define SWITCH_INTERNAL_CAP 			1		// 0: external cap, 1: internal cap
#define CAP_SET_FLASH_ADDR 				FLASH_CAP_VALUE_ADDR_128K
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
#elif(MCU_CORE_B80B)
#define BQB_UART_TX_PORT   				GPIO_PD0
#define BQB_UART_RX_PORT   				GPIO_PD1
#elif(MCU_CORE_TC321X)
#define BQB_UART_TX_PORT   				GPIO_PC2
#define BQB_UART_RX_PORT   				GPIO_PC3
#elif(MCU_CORE_TC122X)
#define BQB_UART_TX_PORT   				GPIO_PA0
#define BQB_UART_RX_PORT   				GPIO_PA1
#endif
#define BQB_UART_BAUD	   	115200

#if(MCU_CORE_B80B || MCU_CORE_TC321X)
#define UART0_MODULE            0 /* UART0 */
#define UART1_MODULE            1 /* UART1 */
/* uart select */
#define UART_MODULE_SEL         UART0_MODULE

/* set uart config */
#define REG_UART_BUF_CNT           reg_uart_buf_cnt(UART_MODULE_SEL)
#define REG_UART_DATA_BUF(i)       reg_uart_data_buf(UART_MODULE_SEL,i)
#define UART_NDMA_SENT_BYTE(i)     uart_ndma_send_byte(UART_MODULE_SEL,i)
#elif(MCU_CORE_TC122X)
#define UART0_MODULE            0 /* UART0 */
/* uart select */
#define UART_MODULE_SEL         UART0_MODULE

/* set uart config */
#define REG_UART_BUF_CNT           reg_uart_buf_cnt(UART_MODULE_SEL)
#define REG_UART_DATA_BUF(i)       reg_uart_data_buf(i)
#define UART_NDMA_SENT_BYTE(i)     uart_ndma_send_byte(UART_MODULE_SEL,i)
#else
#define REG_UART_BUF_CNT           reg_uart_buf_cnt
#define REG_UART_DATA_BUF(i)       reg_uart_data_buf(i)
#define UART_NDMA_SENT_BYTE(i)     uart_ndma_send_byte(i)
#endif

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
 * @brief   This function serves to read the uart data and execute BQB program
 * @param   none.
 * @return  none.
 */
void bqb_serviceloop (void);


extern void bqb_pa_init();

extern void bqb_pa_set_mode(unsigned char rtx); //0:rx, 1:tx, other:off

#endif

#endif /* BQB_H_ */

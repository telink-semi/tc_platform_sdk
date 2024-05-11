/********************************************************************************************************
 * @file    s7816.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef S7816_H_
#define S7816_H_

#include "gpio.h"
#if (MCU_CORE_B80B)
#include "uart_b80b.h"
#else
#include "uart.h"
#endif
/**
 *  @brief  Define 7816 clock
 */
typedef enum {
	S7816_4MHZ  = 4,
	S7816_6MHZ  = 6,
	S7816_12MHZ = 12,
}S7816_ClkTypeDef;


typedef enum {
	S7816_TX,
	S7816_RX,
}S7816_ModeDef;


/**
 * @brief      	This function is used to set the rst-wait time of the s7816 module.
 * @param[in]  	rst_time_us - set the s7816_rst_time.
 * @return     	none.
 */
void s7816_set_time(int rst_time_us);

/**
 * @brief      	This function is used to set the RST pin of s7816.
 * @param[in]  	pin_7816_rst - the RST pin of s7816.
 * @return     	none.
 */
void s7816_set_rst_pin(GPIO_PinTypeDef pin_7816_rst);

/**
 * @brief      	This function is used to set the VCC pin of s7816.
 * @param[in]   pin_7816_vcc - the VCC pin of s7816.
 * @return     	none.
 */
void s7816_set_vcc_pin(GPIO_PinTypeDef pin_7816_vcc);
#if(MCU_CORE_B80B)

/**
 * @brief      	This function is used to set the s7816 clock.
 * @param[in]   uart_num - UART0 or UART1.
 * @param[in]  	div	- set the divider of clock of 7816 module.
 * @return     	none.
 * @note        the clk-source of s7816 is 24M,the clk of clk-pin can be divided as follow.
 * 				div:        0x06-4Mhz     0x04-6Mhz   0x02-12Mhz
 * 				baudrate:   0x06-10752    0x04-16194  0x02-32388
 */
void s7816_set_clk(uart_num_e uart_num,unsigned char Div);

/**
 * @brief      	This function is used to set the clk pin of s7816.
 * @param[in]   uart_num - UART0 or UART1.
 * @param[in]   pin_7816_clk - the clk pin of s7816.
 * @return     	none.
 */
void s7816_set_clk_pin(uart_num_e uart_num,GPIO_PinTypeDef pin_7816_clk);

/**
 * @brief       This function is used to set the trx pin of s7816.
 * @param[in]   uart_num - UART0 or UART1.
 * @param[in]  	Pin_7816_TRX	-select the RTX pin of 7816 module
 * @return     	none
 */
void s7816_set_trx_pin(uart_num_e uart_num,GPIO_PinTypeDef Pin_7816_TRX);

/**
 * @brief      	This function is used to set all the pin of s7816 module.
 * @param[in]   uart_num    - UART0 or UART1.
 * @param[in]  	rst_pin     - the rst pin of s7816.
 * @param[in]  	vcc_pin     - the vcc pin of s7816.
 * @param[in]  	clk_pin     - the clk pin of s7816.
 * @param[in]  	trx_pin     - the trx pin of s7816.
 * @return     	none.
 */
void s7816_set_pin(uart_num_e uart_num,GPIO_PinTypeDef rst_pin,GPIO_PinTypeDef vcc_pin,GPIO_PinTypeDef clk_pin,GPIO_PinTypeDef trx_pin);

/**
 * @brief      	This function is used to initialize the s7816 module.
 * @param[in]   uart_num     - UART0 or UART1.
 * @param[in]  	clock        - the clock of s7816.
 * @param[in]  	f            - the clock frequency regulator of s7816,372 by default.
 * @param[in]  	d            - the bitrate regulator of s7816,1 by default.
 * @return     	none.
 */
void s7816_init(uart_num_e uart_num,S7816_ClkTypeDef clock,int f,int d);

/**
 * @brief      	This function is used to active the IC card,set the trx pin and coldreset.
 * @param[in]   uart_num - UART0 or UART1.
 * @return     	none.
 * @note        extra time is needed for initial-atr after the function.
 */
void s7816_coldreset(uart_num_e uart_num);

/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]   uart_num - UART0 or UART1.
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
void s7816_mode(uart_num_e uart_num,S7816_ModeDef mode);

/**
 * @brief      	This function is used to release the trigger.
 * @param[in]   uart_num - UART0 or UART1.
 * @return     	none.
 */
void s7816_release_trig(uart_num_e uart_num);

#else

/**
 * @brief      	This function is used to set the s7816 clock.
 * @param[in]  	div	- set the divider of clock of 7816 module.
 * @return     	none.
 * @note        the clk-source of s7816 is 24M,the clk of clk-pin can be divided as follow.
 * 				div:        0x06-4Mhz     0x04-6Mhz   0x02-12Mhz
 * 				baudrate:   0x06-10752    0x04-16194  0x02-32388
 */
void s7816_set_clk(unsigned char Div);

/**
 * @brief      	This function is used to set the clk pin of s7816.
 * @param[in]   pin_7816_clk - the clk pin of s7816.
 * @return     	none.
 */
void s7816_set_clk_pin(GPIO_PinTypeDef pin_7816_clk);

/**
 * @brief       This function is used to set the trx pin of s7816.
 * @param[in]  	Pin_7816_TRX	-select the RTX pin of 7816 module
 * @return     	none
 */
void s7816_set_trx_pin(GPIO_PinTypeDef Pin_7816_TRX);

/**
 * @brief      	This function is used to set all the pin of s7816 module.
 * @param[in]  	rst_pin     - the rst pin of s7816.
 * @param[in]  	vcc_pin     - the vcc pin of s7816.
 * @param[in]  	clk_pin     - the clk pin of s7816.
 * @param[in]  	trx_pin     - the trx pin of s7816.
 * @return     	none.
 */
void s7816_set_pin(GPIO_PinTypeDef rst_pin,GPIO_PinTypeDef vcc_pin,GPIO_PinTypeDef clk_pin,GPIO_PinTypeDef trx_pin);

/**
 * @brief      	This function is used to initialize the s7816 module.
 * @param[in]  	clock        - the clock of s7816.
 * @param[in]  	f            - the clock frequency regulator of s7816,372 by default.
 * @param[in]  	d            - the bitrate regulator of s7816,1 by default.
 * @return     	none.
 */
void s7816_init(S7816_ClkTypeDef clock,int f,int d);

/**
 * @brief      	This function is used to active the IC card,set the trx pin and coldreset.
 * @param[in]  	none.
 * @return     	none.
 * @note        extra time is needed for initial-atr after the function.
 */
void s7816_coldreset(void);

/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
void s7816_mode(S7816_ModeDef mode);

/**
 * @brief      	This function is used to release the trigger.
 * @param[in]  	none.
 * @return     	none.
 */
void s7816_release_trig(void);
#endif



/**
 * @brief      	This function is used to warmreset.
 * @param[in]  	none.
 * @return     	none.
 * @note        the warmreset is required after the IC-CARD active,extra time is needed for initial-atr after the function.
 */
void s7816_warmreset(void);
#endif /* S7816_H_ */

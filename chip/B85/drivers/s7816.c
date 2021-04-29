/********************************************************************************************************
 * @file	s7816.c
 *
 * @brief	This is the source file for b85
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
#include "s7816.h"
#include "uart.h"
#include "irq.h"
#include "dma.h"
#include "timer.h"

volatile unsigned int  s7816_rst_pin;
volatile unsigned int  s7816_vcc_pin;
volatile unsigned int  s7816_trx_pin;
volatile unsigned char s7816_clock;
volatile int s7816_rst_time;//us

/**
 * @brief      	This function is used to set the s7816 clock.
 * @param[in]  	div	- set the divider of clock of 7816 module.
 * @return     	none.
 * @note        the clk-source of s7816 is 24M,the clk of clk-pin can be divided as follow.
 * 				div:        0x60-4Mhz     0x40-6Mhz   0x20-12Mhz
 * 				baudrate:   0x60-10752    0x40-16194  0x20-32388
 */
void s7816_set_clk(unsigned char Div)
{
	//---Set 7816 CLK divider
    //caution:7816 clk module only controlled by the highest bit of 7816 clk mode
	reg_7816_clk_div&=0x0f;
	reg_7816_clk_div|=(unsigned char)Div;
}

/**
 * @brief      	This function is used to set the rst-wait time of the s7816 module.
 * @param[in]  	rst_time_us - set the s7816_rst_time.
 * @return     	none.
 */
void s7816_set_time(int rst_time_us)
{
	s7816_rst_time=rst_time_us;
}

/**
 * @brief      	This function is used to set the RST pin of s7816.
 * @param[in]  	pin_7816_rst - the RST pin of s7816.
 * @return     	none.
 */
void s7816_set_rst_pin(GPIO_PinTypeDef pin_7816_rst)
{
	s7816_rst_pin=pin_7816_rst;
	gpio_set_func(pin_7816_rst,AS_GPIO);
	gpio_set_output_en(pin_7816_rst,1);
	gpio_set_input_en(pin_7816_rst,0);
	gpio_write(pin_7816_rst,0);
}

/**
 * @brief      	This function is used to set the VCC pin of s7816.
 * @param[in]   pin_7816_vcc - the VCC pin of s7816.
 * @return     	none.
 */
void s7816_set_vcc_pin(GPIO_PinTypeDef pin_7816_vcc)
{
	s7816_vcc_pin=pin_7816_vcc;
	gpio_set_func(pin_7816_vcc,AS_GPIO);
	gpio_set_output_en(pin_7816_vcc,1);
	gpio_set_input_en(pin_7816_vcc,0);
	gpio_write(pin_7816_vcc,0);
}

/**
 * @brief      	This function is used to set the clk pin of s7816.
 * @param[in]   pin_7816_vcc - the clk pin of s7816.
 * @return     	none.
 */
void s7816_set_clk_pin(GPIO_PinTypeDef pin_7816_clk)
{
	gpio_set_func(pin_7816_clk,AS_UART);
}

/**
 * @brief       This function is used to set the trx pin of s7816.
 * @param[in]  	Pin_7816_TRX	-select the RTX pin of 7816 module
 * @return     	none
 */
void s7816_set_trx_pin(S7816_TRx_PinDef Pin_7816_TRX)
{
	//---enable UART 7816 TRX(Optional)
	switch(Pin_7816_TRX)
	{
		case S7816_TRX_C2://take care the details of every pin when using this pin
			gpio_setup_up_down_resistor(GPIO_PC2, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PC2,AS_UART);
			gpio_set_input_en(GPIO_PC2, 1);
			break;
		case S7816_TRX_D0:
			gpio_setup_up_down_resistor(GPIO_PD0, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PD0,AS_UART);
			gpio_set_input_en(GPIO_PD0, 1);
			break;
		case S7816_TRX_D3:
			gpio_setup_up_down_resistor(GPIO_PD3, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PD3,AS_UART);
			gpio_set_input_en(GPIO_PD3, 1);
			break;
		case S7816_TRX_D7:
			gpio_setup_up_down_resistor(GPIO_PD7, PM_PIN_PULLUP_10K);
			gpio_set_func(GPIO_PD7,AS_UART);
			gpio_set_input_en(GPIO_PD7, 1);
			break;
		default:
			break;
	}
}

/**
 * @brief      	This function is used to set all the pin of s7816 module.
 * @param[in]  	rst_pin     - the rst pin of s7816.
 * @param[in]  	vcc_pin     - the vcc pin of s7816.
 * @param[in]  	clk_pin     - the clk pin of s7816.
 * @param[in]  	trx_pin     - the trx pin of s7816.
 * @return     	none.
 */
void s7816_set_pin(GPIO_PinTypeDef rst_pin,GPIO_PinTypeDef vcc_pin,S7816_Clk_PinDef clk_pin,S7816_TRx_PinDef trx_pin)
{
	s7816_set_rst_pin(rst_pin);
	s7816_rst_pin=rst_pin;

	s7816_set_vcc_pin(vcc_pin);
	s7816_vcc_pin=vcc_pin;

	s7816_set_clk_pin(clk_pin);

	s7816_trx_pin = trx_pin;
}

/**
 * @brief      	This function is used to initialize the s7816 module.
 * @param[in]  	clock        - the clock of s7816.
 * @param[in]  	f            - the clock frequency regulator of s7816,372 by default.
 * @param[in]  	d            - the bitrate regulator of s7816,1 by default.
 * @return     	none.
 */
void s7816_init(S7816_ClkTpyeDef clock,int f,int d)
{
	s7816_clock=clock;
	s7816_rst_time=40000/clock;//us

	int baud=clock*1000000*d/f;
	if(clock==S7816_4MHZ)
	{
		s7816_set_clk(0x60);
	}
	else if(clock==S7816_6MHZ)
	{
		s7816_set_clk(0x40);
	}
	else if(clock==S7816_12MHZ)
	{
		s7816_set_clk(0x20);
	}
	uart_reset();
	uart_init_baudrate(baud,24000000,PARITY_EVEN, STOP_BIT_ONE);
}

/**
 * @brief      	This function is used to active the IC card,set the trx pin and coldreset.
 * @param[in]  	none.
 * @return     	none.
 * @note        extra time is needed for initial-atr after the function.
 */
void s7816_coldreset()
{
	gpio_write(s7816_vcc_pin,1);
	sleep_us(20);//wait for the vcc  stable.
	reg_7816_clk_div|=BIT(7); //enable the 7816 clk
	sleep_us(s7816_rst_time);
	s7816_set_trx_pin(s7816_trx_pin);// uart tx/rx pin set,if the trx pin set before this place,it may
    gpio_write(s7816_rst_pin,1);//the IC card will return the initial ATR.
}

/**
 * @brief      	This function is used to release the trigger.
 * @param[in]  	none.
 * @return     	none.
 */
void s7816_release_trig()
{
	gpio_write(s7816_rst_pin,0);
	reg_7816_clk_div&=(BIT(7)-1);
	gpio_write(s7816_vcc_pin,0);
}

/**
 * @brief      	This function is used to warmreset.
 * @param[in]  	none.
 * @return     	none.
 * @note        the warmreset is required after the IC-CARD active,extra time is needed for initial-atr after the function.
 */
void s7816_warmreset()
{
    gpio_write(s7816_rst_pin,0);
    sleep_us(s7816_rst_time);
    gpio_write(s7816_rst_pin,1);//The IC card will return the initial ATR.
}

/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
void s7816_mode(S7816_ModeDef mode)
{
	if(mode==S7816_RX){
		reg_uart_rx_timeout1|=FLD_UART_P7816_EN;
	}
	else if(mode==S7816_TX)
	{
		reg_uart_rx_timeout1&=~FLD_UART_P7816_EN;
	}
}


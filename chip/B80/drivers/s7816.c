/********************************************************************************************************
 * @file    s7816.c
 *
 * @brief   This is the source file for B80
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
#include "s7816.h"
#include "irq.h"
#include "dma.h"
#include "timer.h"

volatile unsigned int  s7816_rst_pin;
volatile unsigned int  s7816_vcc_pin;
volatile unsigned int  s7816_trx_pin;
volatile unsigned char s7816_clock;
volatile int s7816_rst_time;//us


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
	gpio_set_output_en(pin_7816_rst,1);
	gpio_set_input_en(pin_7816_rst,0);
	gpio_set_func(pin_7816_rst,AS_GPIO);
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
	gpio_set_output_en(pin_7816_vcc,1);
	gpio_set_input_en(pin_7816_vcc,0);
	gpio_set_func(pin_7816_vcc,AS_GPIO);
	gpio_write(pin_7816_vcc,0);
}
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
void s7816_set_clk(uart_num_e uart_num,unsigned char Div)
{
    //---Set 7816 CLK divider
    //caution:7816 clk module only controlled by the highest bit of 7816 clk mode
    reg_7816_clk_div(uart_num) = ((reg_7816_clk_div(uart_num) & 0x0f)|(Div<<4));

}

/**
 * @brief      	This function is used to set the clk pin of s7816.
 * @param[in]   uart_num     - UART0 or UART1.
 * @param[in]   pin_7816_clk - the clk pin of s7816.
 * @return     	none.
 */
void s7816_set_clk_pin(uart_num_e uart_num,GPIO_PinTypeDef pin_7816_clk)
{
   if(uart_num ==UART0){
	   gpio_set_func(pin_7816_clk,CLK_7816);
   }else if(uart_num == UART1){
	   gpio_set_func(pin_7816_clk,CLK_78161);
   }


}

/**
 * @brief       This function is used to set the trx pin of s7816.
 * @param[in]   uart_num        - UART0 or UART1.
 * @param[in]  	Pin_7816_TRX	-select the RTX pin of 7816 module
 * @return     	none
 */
void s7816_set_trx_pin(uart_num_e uart_num,GPIO_PinTypeDef Pin_7816_TRX)
{
	 //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
	 //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will may misread the short low-level timing.confirmed by minghai.20210709.
	gpio_set_input_en(Pin_7816_TRX, 1);
	gpio_setup_up_down_resistor(Pin_7816_TRX, PM_PIN_PULLUP_10K);  //must  for stability and prevent from current leakage
    if(uart_num == UART0){
    	gpio_set_func(Pin_7816_TRX,UART_RTX); // set rx pin
    }else if(uart_num == UART1){
    	gpio_set_func(Pin_7816_TRX,UART1_RTX); // set rx pin
    }


}

/**
 * @brief      	This function is used to set all the pin of s7816 module.
 * @param[in]   uart_num    - UART0 or UART1.
 * @param[in]  	rst_pin     - the rst pin of s7816.
 * @param[in]  	vcc_pin     - the vcc pin of s7816.
 * @param[in]  	clk_pin     - the clk pin of s7816.
 * @param[in]  	trx_pin     - the trx pin of s7816.
 * @return     	none.
 */
void s7816_set_pin(uart_num_e uart_num,GPIO_PinTypeDef rst_pin,GPIO_PinTypeDef vcc_pin,GPIO_PinTypeDef clk_pin,GPIO_PinTypeDef trx_pin)
{
	s7816_set_rst_pin(rst_pin);
	s7816_rst_pin=rst_pin;

	s7816_set_vcc_pin(vcc_pin);
	s7816_vcc_pin=vcc_pin;

	s7816_set_clk_pin(uart_num,clk_pin);

	s7816_trx_pin = trx_pin;
}

/**
 * @brief      	This function is used to initialize the s7816 module.
 * @param[in]   uart_num     - UART0 or UART1.
 * @param[in]  	clock        - the clock of s7816.
 * @param[in]  	f            - the clock frequency regulator of s7816,372 by default.
 * @param[in]  	d            - the bitrate regulator of s7816,1 by default.
 * @return     	none.
 */
void s7816_init(uart_num_e uart_num,S7816_ClkTypeDef clock,int f,int d)
{
	s7816_clock=clock;
	s7816_rst_time=40000/clock;//us

	int baud=clock*1000000*d/f;
	if(clock==S7816_4MHZ)
	{
		s7816_set_clk(uart_num,0x06);
	}
	else if(clock==S7816_6MHZ)
	{
		s7816_set_clk(uart_num,0x04);
	}
	else if(clock==S7816_12MHZ)
	{
		s7816_set_clk(uart_num,0x02);
	}
	uart_reset(uart_num);
	uart_init_baudrate(uart_num,baud,24000000,PARITY_EVEN, STOP_BIT_ONE);
}

/**
 * @brief      	This function is used to active the IC card,set the trx pin and coldreset.
 * @param[in]   uart_num - UART0 or UART1.
 * @return     	none.
 * @note        extra time is needed for initial-atr after the function.
 */
void s7816_coldreset(uart_num_e uart_num)
{
	gpio_write(s7816_vcc_pin,1);
	sleep_us(20);//wait for the vcc  stable.
	reg_7816_clk_div(uart_num)|=BIT(7); //enable the 7816 clk
	sleep_us(s7816_rst_time);
	s7816_set_trx_pin(uart_num,s7816_trx_pin);// uart tx/rx pin set,if the trx pin set before this place,it may
    gpio_write(s7816_rst_pin,1);//the IC card will return the initial ATR.
}

/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]   uart_num - UART0 or UART1.
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
void s7816_mode(uart_num_e uart_num,S7816_ModeDef mode)
{
	//---------------------------------------------------------
	if(mode==S7816_RX){
		reg_uart_rx_timeout1(uart_num)|=FLD_UART_P7816_EN;
	}
	else if(mode==S7816_TX)
	{
		reg_uart_rx_timeout1(uart_num)&=~FLD_UART_P7816_EN;
	}
}

/**
 * @brief      	This function is used to release the trigger.
 * @param[in]   uart_num - UART0 or UART1.
 * @return     	none.
 */
void s7816_release_trig(uart_num_e uart_num)
{
    gpio_write(s7816_rst_pin,0);
    reg_7816_clk_div(uart_num)&=(BIT(7)-1);
    gpio_write(s7816_vcc_pin,0);
}
#else

/**
 * @brief      	This function is used to set the s7816 clock.
 * @param[in]  	div	- set the divider of clock of 7816 module.
 * @return     	none.
 * @note        the clk-source of s7816 is 24M,the clk of clk-pin can be divided as follow.
 * 				div:        0x06-4Mhz     0x04-6Mhz   0x02-12Mhz
 * 				baudrate:   0x06-10752    0x04-16194  0x02-32388
 */
void s7816_set_clk(unsigned char Div)
{
	//---Set 7816 CLK divider
    //caution:7816 clk module only controlled by the highest bit of 7816 clk mode
	reg_7816_clk_div = ((reg_7816_clk_div & 0x0f)|(Div<<4));

}


/**
 * @brief      	This function is used to set the clk pin of s7816.
 * @param[in]   pin_7816_clk - the clk pin of s7816.
 * @return     	none.
 */
void s7816_set_clk_pin(GPIO_PinTypeDef pin_7816_clk)
{

	    gpio_set_func(pin_7816_clk,CLK_7816);

}

/**
 * @brief       This function is used to set the trx pin of s7816.
 * @param[in]  	Pin_7816_TRX	-select the RTX pin of 7816 module
 * @return     	none
 */
void s7816_set_trx_pin(GPIO_PinTypeDef Pin_7816_TRX)
{
	 //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
	 //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will may misread the short low-level timing.confirmed by minghai.20210709.
	gpio_set_input_en(Pin_7816_TRX, 1);
	gpio_setup_up_down_resistor(Pin_7816_TRX, PM_PIN_PULLUP_10K);  //must  for stability and prevent from current leakage

	gpio_set_func(Pin_7816_TRX,UART_RTX); // set rx pin

}

/**
 * @brief      	This function is used to set all the pin of s7816 module.
 * @param[in]  	rst_pin     - the rst pin of s7816.
 * @param[in]  	vcc_pin     - the vcc pin of s7816.
 * @param[in]  	clk_pin     - the clk pin of s7816.
 * @param[in]  	trx_pin     - the trx pin of s7816.
 * @return     	none.
 */
void s7816_set_pin(GPIO_PinTypeDef rst_pin,GPIO_PinTypeDef vcc_pin,GPIO_PinTypeDef clk_pin,GPIO_PinTypeDef trx_pin)
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
void s7816_init(S7816_ClkTypeDef clock,int f,int d)
{
	s7816_clock=clock;
	s7816_rst_time=40000/clock;//us

	int baud=clock*1000000*d/f;
	if(clock==S7816_4MHZ)
	{
		s7816_set_clk(0x06);
	}
	else if(clock==S7816_6MHZ)
	{
		s7816_set_clk(0x04);
	}
	else if(clock==S7816_12MHZ)
	{
		s7816_set_clk(0x02);
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
void s7816_coldreset(void)
{
	gpio_write(s7816_vcc_pin,1);
	sleep_us(20);//wait for the vcc  stable.
	reg_7816_clk_div|=BIT(7); //enable the 7816 clk
	sleep_us(s7816_rst_time);
	s7816_set_trx_pin(s7816_trx_pin);// uart tx/rx pin set,if the trx pin set before this place,it may
    gpio_write(s7816_rst_pin,1);//the IC card will return the initial ATR.
}

/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
void s7816_mode(S7816_ModeDef mode)
{
	//---------------------------------------------------------
	if(mode==S7816_RX){
		reg_uart_rx_timeout1|=FLD_UART_P7816_EN;
	}
	else if(mode==S7816_TX)
	{
		reg_uart_rx_timeout1&=~FLD_UART_P7816_EN;
	}
}

/**
 * @brief      	This function is used to release the trigger.
 * @param[in]  	none.
 * @return     	none.
 */
void s7816_release_trig(void)
{
	gpio_write(s7816_rst_pin,0);
	reg_7816_clk_div&=(BIT(7)-1);
	gpio_write(s7816_vcc_pin,0);
}
#endif

/**
 * @brief      	This function is used to warmreset.
 * @param[in]  	none.
 * @return     	none.
 * @note        the warmreset is required after the IC-CARD active,extra time is needed for initial-atr after the function.
 */
void s7816_warmreset(void)
{
    gpio_write(s7816_rst_pin,0);
    sleep_us(s7816_rst_time);
    gpio_write(s7816_rst_pin,1);//The IC card will return the initial ATR.
}



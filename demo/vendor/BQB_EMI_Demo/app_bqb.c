/********************************************************************************************************
 * @file	app_bqb.c
 *
 * @brief	This is the source file for b85m
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
#include "app_config.h"
#include "BQB/bqb.h"

#if(TEST_DEMO==BQB_DEMO)

#define read_config(addr) (*(volatile unsigned char*)(0x840000 | (addr)))
#define write_config(addr,v) 	(*(volatile unsigned char*)(0x840000 | (addr)) = (unsigned char)(v))

#if SUPPORT_CONFIGURATION


void rd_usr_definition(unsigned char _s)
{
	if(_s)
	{
		usr_config.uart_tx = read_config(CONFIGURATION_ADDR_UART_TX);
		usr_config.uart_rx = read_config(CONFIGURATION_ADDR_UART_RX);
		usr_config.pa_tx = read_config(CONFIGURATION_ADDR_PA_TX);
		usr_config.pa_rx = read_config(CONFIGURATION_ADDR_PA_RX);
		unsigned char tmp = read_config(CONFIGURATION_ADDR_CAP);
		usr_config.cap = (tmp >> 7) & 0x01;
		usr_config.cal_pos = tmp & 0x03;
		usr_config.power_mode = (tmp >> 5) & 0x03;
		usr_config.power = read_config(CONFIGURATION_ADDR_POWER);
	}
	else
	{
		usr_config.flash = (flash_read_mid() >> 16) & 0x7f;
	}
}
#if (MCU_CORE_B80)
void get_uart_port(GPIO_PinTypeDef* bqb_uart_tx_port, GPIO_PinTypeDef* bqb_uart_rx_port)
#else
void get_uart_port(UART_TxPinDef* bqb_uart_tx_port, UART_RxPinDef* bqb_uart_rx_port)
#endif
{
	if(usr_config.uart_tx == usr_config.uart_rx) return;
	*bqb_uart_tx_port = get_pin(usr_config.uart_tx);
	*bqb_uart_rx_port = get_pin(usr_config.uart_rx);
}
#endif

void read_bqb_calibration()
{
	unsigned char chnidx = 0xff;
#if SUPPORT_CONFIGURATION
	if(usr_config.cap)
	{
		rf_turn_off_internal_cap();
	}
	else
	{
		if(usr_config.cal_pos == 1)//OTP
		{
#if MCU_CORE_B80
			unsigned int temp;
			otp_read(CAP_SET_OTP_16K, 1, &temp);
			chnidx = temp & 0xff;
#endif
		}
		else if(usr_config.cal_pos == 2)//SRAM
		{
			chnidx = read_config(CAP_CALIBRATION_SRAM);
		}
		else//FLASH
		{
			if(usr_config.flash == FLASH_SIZE_2M)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_2M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_1M)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_1M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_512K)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_512K, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_128K || usr_config.flash == FLASH_SIZE_256K)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_128K, 1, &chnidx);
			}
			else
			{
				flash_read_page(CAP_SET_FLASH_ADDR_64K, 1, &chnidx);
			}
		}

		if(chnidx!=0xff)
		{
			rf_update_internal_cap(chnidx);
		}
	}
#else
	#if(SWITCH_INTERNAL_CAP)
		#if SWITCH_CALIBRATION_POSITION
			flash_read_page(CAP_SET_FLASH_ADDR, 1, &chnidx);
		#else
			chnidx = read_config(CAP_CALIBRATION_SRAM);
		#endif
			if(chnidx!=0xff)
			{
				rf_update_internal_cap(chnidx);
			}

	#else
			rf_turn_off_internal_cap();
	#endif
#endif
}


void user_init(void)
{
#if DEBUG_FLAG
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);	 //disable input
	gpio_write(LED1, 0);         //LED On
#endif

#if(MCU_CORE_B80)
	GPIO_PinTypeDef bqb_uart_tx_port = BQB_UART_TX_PORT;
	GPIO_PinTypeDef bqb_uart_rx_port = BQB_UART_RX_PORT;
#else
	UART_TxPinDef bqb_uart_tx_port = BQB_UART_TX_PORT;
	UART_RxPinDef bqb_uart_rx_port = BQB_UART_RX_PORT;
#endif
#if SUPPORT_CONFIGURATION
	if(usr_config.cal_pos == 0)
	{
		rd_usr_definition(0);
	}
	get_uart_port(&bqb_uart_tx_port, &bqb_uart_rx_port);
#endif
	uart_gpio_set(bqb_uart_tx_port, bqb_uart_rx_port);// uart tx/rx pin set
	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
	uart_init_baudrate(BQB_UART_BUAD,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);
	uart_dma_enable(0, 0);
	irq_disable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);
	uart_irq_enable(0,0);   //uart RX irq enable
	uart_ndma_irq_triglevel(0,0);	//set the trig level. 1 indicate one byte will occur interrupt

	/* for SRAM calibration */
	write_config(CAP_CALIBRATION_SRAM, 0xff);
	read_bqb_calibration();

	bqbtest_init();
	//SET PA PORT
	bqb_pa_init();
}

void main_loop(void)
{
	bqb_serviceloop();
}
#endif

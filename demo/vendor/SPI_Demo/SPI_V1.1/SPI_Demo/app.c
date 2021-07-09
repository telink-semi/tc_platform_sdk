/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
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
#if (SPI_MODE == SPI_NDMA_MODE)

//To make sure the communication between master and slave device is normal,the slave device should power on first.
#define SPI_MASTER_DEVICE		1
#define SPI_SLAVE_DEVICE		2
#define SPI_DEVICE				SPI_SLAVE_DEVICE

#define B85_B87_B89_SLAVE_PROTOCOL		   	    1 // B85/B97/B89 as slave
#define B91M_B80_SLAVE_MODE_PROTOCOL		   	2 // B91m/B80 spi slave mode as slave
#define B91M_SLAVE_PROTOCOL		       	        3 // B91m spi slave as slave

#define SPI_PROTOCOL			       B91M_B80_SLAVE_MODE_PROTOCOL


/**********************************************************************************************************************
 *                                         SPI multiple slave                                               	 	  *
 *********************************************************************************************************************/
#define ONE_SLAVE	       						 0
#define MULTI_SLAVE		   						 1

#define SPI_SLAVE_NUM	    				 ONE_SLAVE

/**********************************************************************************************************************
 *                                         3line SPI slave enable                                              	 	  *
 *********************************************************************************************************************/
#define SPI_NORMAL_SLAVE	       					0
#define SPI_3LINE_SLAVE		   						1
#define SPI_SLAVE_MODE	    				   SPI_NORMAL_SLAVE
/**********************************************************************************************************************
 *                                         SPI CLK                                               	 	  *
 *********************************************************************************************************************/
#define SPI_CLK	    					  	   500000


 spi_pin_config_t spi_pin_config = {
		.spi_clk_pin	   = GPIO_PA0,// GPIO_SPI_CK =GPIO_PB1  (default),if you not use default,should disable it.
		.spi_csn_pin 	   = GPIO_PA4,// GPIO_SPI_CN =GPIO_PB0  (default),if you not use default,should disable it.
		.spi_mosi_io0_pin  = GPIO_PB2,// GPIO_SPI_MOSI=GPIO_PF0(default),if you not use default,should disable it.
		.spi_miso_io1_pin  = GPIO_PB4,// GPIO_SPI_MISO=GPIO_PF1(default),if you not use default,should disable it.                                        3line mode set none
		.spi_wp_io2_pin    = GPIO_PB3,//set quad mode otherwise set none, only PB3 support
		.spi_hold_io3_pin  = GPIO_PD4,//set quad mode otherwise set none, only PD4 support
 };

#if (SPI_DEVICE==SPI_MASTER_DEVICE)
#define 	DATA_BYTE_LEN     16
volatile unsigned char spi_rx_buff[DATA_BYTE_LEN] __attribute__((aligned(4))) = {0x00};


#if (SPI_PROTOCOL == B85_B87_B89_SLAVE_PROTOCOL)
#define SPI_B85M_READ_CMD			0x80
#define SPI_B85M_WRITE_CMD			0x00
typedef struct {
	unsigned char address[3];
	unsigned char cmd;
	unsigned char data[DATA_BYTE_LEN];
} __attribute__((aligned(4))) spi_b85_b87_b89_slave_protocol_t;

volatile spi_b85_b87_b89_slave_protocol_t spi_tx_buff = {
	.address = {0x04, 0x10, 0x2c},
	.cmd = SPI_B85M_WRITE_CMD,
	.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
};

#elif (SPI_PROTOCOL == B91M_B80_SLAVE_MODE_PROTOCOL)
spi_config_t spi_b91m_b80_slave_protocol_config = {
		.spi_io_mode 		= SPI_SINGLE_MODE,//if SPI_3LINE_SLAVE=1,set SPI_3LINE_MODE.
		.spi_dummy_cnt   	= 8,
		.spi_cmd_en 		= 1,
		.spi_addr_en 		= 0,//must
		.spi_addr_len 		= 0,//when spi_addr_en = false, invalid set.
		.spi_cmd_fmt_en 	= 0,//when spi_cmd_en = false, invalid set.
		.spi_addr_fmt_en 	= 0,//when spi_addr_en = false, invalid set.
	};
typedef struct
{
	unsigned char address[4];
	unsigned int  data_len;
	unsigned char data[DATA_BYTE_LEN];
}  __attribute__((aligned(4))) spi_b91m_b80_slave_protocol_t;

spi_b91m_b80_slave_protocol_t spi_tx_buff =
{
	.address = {0xc0, 0x20, 0x04, 0x00},
	.data_len = DATA_BYTE_LEN,
	.data = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
};
#elif (SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
#define spi_slave_address	0xc0200400
spi_config_t spi_b91m_spi_slave_protocol_config = {
	.spi_io_mode 		= SPI_SINGLE_MODE,
	.spi_dummy_cnt 		= 8,
	.spi_cmd_en 		= 1,
	.spi_addr_en 		= 1,
	.spi_addr_len 		= 4,//when spi_addr_en = 0,invalid set.
	.spi_cmd_fmt_en 	= 0,//when spi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 0,//when spi_addr_en=0,invalid set.
};
typedef struct
{
	unsigned char data[DATA_BYTE_LEN];
} __attribute__((aligned(4))) spi_b91m_spi_slave_protocol_t;
spi_b91m_spi_slave_protocol_t spi_tx_buff = {
	.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
};
#endif

void user_init()
{
	gpio_set_func(LED1|LED2|LED3|LED4 ,AS_GPIO);
	gpio_set_output_en(LED1|LED2|LED3|LED4, 1); 		//enable output
	gpio_set_input_en(LED1|LED2|LED3|LED4,0);			//disable input
	/*if you not use spi pin default,should disable it.*/
	gpio_set_func(GPIO_SPI_MOSI,AS_GPIO);
	gpio_set_func( GPIO_SPI_MISO,AS_GPIO);
	gpio_set_func( GPIO_SPI_CK,AS_GPIO);
	gpio_set_func( GPIO_SPI_CN,AS_GPIO);
	spi_set_pin(&spi_pin_config);
	spi_master_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*SPI_CLK)-1),SPI_MODE0);

#if (SPI_PROTOCOL == B85_B87_B89_SLAVE_PROTOCOL)
	spi_master_config(SPI_SINGLE);
#elif(SPI_PROTOCOL == B91M_B80_SLAVE_MODE_PROTOCOL)
	spi_master_config_plus(&spi_b91m_b80_slave_protocol_config);
#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
	spi_master_config_plus(&spi_b91m_spi_slave_protocol_config);
#endif
}
unsigned int mian_loop_cnt=1;
void main_loop (void)
{
	sleep_ms(500);
	gpio_toggle(LED1);

#if (SPI_PROTOCOL == B85_B87_B89_SLAVE_PROTOCOL)
	spi_tx_buff.cmd = SPI_B85M_WRITE_CMD;
	spi_master_write((unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff));
	spi_tx_buff.cmd = SPI_B85M_READ_CMD;
	spi_master_write_read((unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff.address)+ sizeof(spi_tx_buff.cmd),(unsigned char*)spi_rx_buff, DATA_BYTE_LEN);
	for (int i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.data[i] != (spi_rx_buff[i]))
		{
			gpio_write(LED2,1);//LED on indicate data error
		}
	}
	spi_tx_buff.data[0]++;
#elif(SPI_PROTOCOL == B91M_B80_SLAVE_MODE_PROTOCOL)

	spi_master_write_plus( SPI_WRITE_DATA_SINGLE_CMD, 0, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff), SPI_MODE_WR_DUMMY_WRITE);
	while (spi_is_busy());
	spi_master_read_plus(SPI_READ_DATA_SINGLE_CMD,0, (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);
	for (int i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.data[i] != (spi_rx_buff[i]))
		{
			gpio_write(LED2,1);//LED on indicate data error
		}
	}
	spi_tx_buff.data[0]++;
#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
	spi_master_write_plus(SPI_SLAVE_WRITE_DATA_CMD, spi_slave_address, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff), SPI_MODE_WR_WRITE_ONLY);
	while (spi_is_busy());
	spi_master_read_plus( SPI_SLAVE_READ_DATA_CMD, spi_slave_address, (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);
	for (int i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.data[i] != (spi_rx_buff[i]))
		{
			gpio_write(LED2,1);//LED on indicate data error
		}
	}
	spi_tx_buff.data[0]++;

#endif

#if (SPI_SLAVE_NUM == MULTI_SLAVE)
	if(mian_loop_cnt&1)
	{
		spi_pin_config.spi_csn_pin=spi_change_csn_pin(spi_pin_config.spi_csn_pin,SLAVE_X_CSN_PIN);
	}
	else
	{
		spi_pin_config.spi_csn_pin=spi_change_csn_pin(spi_pin_config.spi_csn_pin,GPIO_PA4);
	}
	cnt++;
#endif
}
_attribute_ram_code_sec_ void irq_handler(void)
{
}
#elif(SPI_DEVICE== SPI_SLAVE_DEVICE&&(B91M_B80_SLAVE_MODE_PROTOCOL==SPI_PROTOCOL))
/**********************************************************************************************************************
*                                         SPI clock set
* * When cclk = 24MHz, the SPI_CLK range are as follows£¬
*                                                   spi_set_slave_dummy_cnt(16/8);
*                                                      16 dummy cycle    8 dummy cycle
*	                                         single      max:1M             max:800K
* 											 dual   	 max:800K           max:800K
* 											 quad   	 max:500K           max:500K
*********************************************************************************************************************/
void user_init()
{
	gpio_set_func(LED1|LED2|LED3|LED4|LED5|LED6 ,AS_GPIO);
	gpio_set_output_en(LED1|LED2|LED3|LED4|LED5|LED6, 1); 		//enable output
	gpio_set_input_en(LED1|LED2|LED3|LED4|LED5|LED6,0);			//disable input

	/*if you not use spi pin default,should disable it.*/
	gpio_set_func(GPIO_SPI_MOSI,AS_GPIO);
	gpio_set_func(GPIO_SPI_MISO,AS_GPIO);
	gpio_set_func(GPIO_SPI_CK,AS_GPIO);
	gpio_set_func(GPIO_SPI_CN,AS_GPIO);

	spi_set_pin(&spi_pin_config);
	spi_slave_init(SPI_MODE0);
	spi_set_irq_mask( SPI_SLV_CMD_EN |SPI_RXFIFO_INT_EN |SPI_END_INT_EN );//endint_en txfifoint_en rxfifoint_en
	irq_set_mask(FLD_IRQ_SPI_EN);
	irq_enable();
#if(SPI_SLAVE_MODE == SPI_3LINE_SLAVE)
	spi_set_3line_mode();
#endif
}
void main_loop (void)
{
	gpio_toggle(LED1);
	sleep_ms(50);
}
#define SPI_RX_BUFF_LEN     24
#define DATA_BYTE_LEN    	16
#define DATA_BYTE_OFFSET    8 //must be a multiple 4
#define TX_RX_FIFO_THR      4
unsigned char spi_rx_buff[SPI_RX_BUFF_LEN] __attribute__((aligned(4))) = {0x00};
unsigned int spi_rx_buff1[6] __attribute__((aligned(4))) = {0x00};
volatile  unsigned int spi_irq_rx_cnt = 0;
volatile  unsigned char  slave_cmd;
volatile  unsigned int spi_tx_count = 0;
volatile  unsigned int spi_tx_num = 16;

volatile  unsigned char  spi_tx_cmd = 0,spi_rx_cmd=0;
/* handle cmd  interrupt when received read cmd enable txfifo interrupt */
_attribute_ram_code_sec_ void spi_irq_slv_cmd_process(void)
{
	slave_cmd = spi_slave_get_cmd();
	switch (slave_cmd)
	{
		case SPI_READ_DATA_SINGLE_CMD:
		case SPI_READ_DATA_DUAL_CMD:
		case SPI_READ_DATA_QUAD_CMD:
			spi_set_irq_mask(SPI_TXFIFO_INT_EN);//enable txfifo_int
		break;
	}

}

/*   handle txfifo  interrupt to return data that received from data master */
_attribute_ram_code_sec_ void spi_irq_tx_fifo_process(void)
{
	if ((spi_tx_num - spi_tx_count) >= 4)
		{
		    spi_write_word( (unsigned int*)(spi_rx_buff + DATA_BYTE_OFFSET + spi_tx_count));
			spi_clr_irq_status(SPI_TXF_INT);//clr
			spi_tx_count += 4;
		}
		else
		{
			spi_clr_irq_mask(SPI_TXFIFO_INT_EN);//disable txfifo_in
			spi_write( (unsigned char*)spi_rx_buff + DATA_BYTE_OFFSET + spi_tx_count, 16 - spi_tx_count);
			spi_tx_count = 0;//reset
			spi_tx_num = 0;
		}
}


 /*  handle rxfifo  interrupt to  received data from  master */
_attribute_ram_code_sec_ void spi_irq_rx_fifo_process(void)
{
	if(spi_irq_rx_cnt < (SPI_RX_BUFF_LEN-(SPI_RX_BUFF_LEN % 4)))
	{
		spi_read_word( (unsigned int*)(spi_rx_buff + spi_irq_rx_cnt));
		spi_irq_rx_cnt += 4;
	}
	else
	{
		spi_read(spi_rx_buff + spi_irq_rx_cnt, SPI_RX_BUFF_LEN % 4);
		spi_irq_rx_cnt += SPI_RX_BUFF_LEN % 4;
		spi_rx_fifo_clr();
	}

}

 /*  handle end  interrupt to  received remains data  from master */
_attribute_ram_code_sec_ void spi_irq_end_process(void)
{
	unsigned char rx_fifo_num= spi_get_rxfifo_num();

	if((rx_fifo_num != 0) && (spi_irq_rx_cnt < SPI_RX_BUFF_LEN))
	{
		spi_read((unsigned char*)spi_rx_buff + spi_irq_rx_cnt,rx_fifo_num);
		spi_irq_rx_cnt+=rx_fifo_num;

	}
	spi_tx_fifo_clr();
	spi_rx_fifo_clr();
	spi_irq_rx_cnt=0;
}
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_ void irq_handler(void)
{

	 gpio_toggle(GPIO_PB6);
		if (spi_get_irq_status(SPI_SLV_CMD_INT))
		{ gpio_toggle(GPIO_PB0);
			spi_irq_slv_cmd_process();
			spi_clr_irq_status(SPI_SLV_CMD_INT);//clr
		}

		if (spi_get_irq_status(SPI_TXF_INT))
		{
			gpio_toggle(GPIO_PB1);
			spi_irq_tx_fifo_process();
			spi_clr_irq_status(SPI_TXF_INT);//clr
		}
		if (spi_get_irq_status(SPI_RXF_INT))
		{

			 spi_irq_rx_fifo_process();
			spi_clr_irq_status(SPI_RXF_INT);//clr
		}

		if(spi_get_irq_status(SPI_END_INT))
		{

			spi_irq_end_process();
			spi_clr_irq_status(SPI_RXF_INT);
			spi_clr_irq_status(SPI_END_INT);//clr
		}
}
#endif
#endif







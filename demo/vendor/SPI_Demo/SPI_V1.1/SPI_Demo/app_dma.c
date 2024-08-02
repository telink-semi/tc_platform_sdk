/********************************************************************************************************
 * @file    app_dma.c
 *
 * @brief   This is the source file for B85m
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
#include "app_config.h"

#if(SPI_MODE == SPI_DMA_MODE)
/**********************************************************************************************************************
 *                                         SPI device  selection                                                	 *
 *********************************************************************************************************************/
#define SPI_MASTER_DEVICE		       		  1
#define SPI_SLAVE_DEVICE		      		  2

#define SPI_DEVICE				   	         SPI_MASTER_DEVICE

/**********************************************************************************************************************
 *                                         SPI protocol demo selection                                                *
 *********************************************************************************************************************/
#define B85_B87_B89_SLAVE_PROTOCOL		   	    1 // B85/B97/B89 as slave
#define B91M_B80_SLAVE_MODE_PROTOCOL		   	2 // B91m/B80 spi slave mode as slave
#define B91M_SLAVE_PROTOCOL		       	        3 // B91m spi slave as slave

#define SPI_PROTOCOL			       B91M_B80_SLAVE_MODE_PROTOCOL

/**********************************************************************************************************************
 *                                         3line SPI slave enable                                              	 	  *
 *********************************************************************************************************************/
#define SPI_NORMAL_SLAVE	       					0
#define SPI_3LINE_SLAVE		   						1

#define SPI_SLAVE_MODE	    				   SPI_NORMAL_SLAVE
/**********************************************************************************************************************
 *                                         SPI CLK                                               	 	  *
 *********************************************************************************************************************/
#define SPI_CLK	    					  	    100000
/**********************************************************************************************************************
 *                                         SPI pin selection                                               	 	  *
 *********************************************************************************************************************/
 spi_pin_config_t spi_pin_config = {
		.spi_clk_pin	   = GPIO_PA0,// GPIO_SPI_CK  =GPIO_PB1  (default),if you not use default,should disable it.
		.spi_csn_pin 	   = GPIO_PA4,// GPIO_SPI_CN  =GPIO_PB0  (default),if you not use default,should disable it.
		.spi_mosi_io0_pin  = GPIO_PB2,// GPIO_SPI_MOSI=GPIO_PF0(default),if you not use default,should disable it.
		.spi_miso_io1_pin  = GPIO_PB4,// GPIO_SPI_MISO=GPIO_PF1(default),if you not use default,should disable it.                                        3line mode set none
		.spi_wp_io2_pin    = GPIO_PB3,//set quad mode otherwise set none, only PB3 support
		.spi_hold_io3_pin  = GPIO_PD4,//set quad mode otherwise set none, only PD4 support
 };
#if(SPI_DEVICE == SPI_MASTER_DEVICE)
 /**********************************************************************************************************************
  *                                         SPI multiple slave                                               	 	  *
  *********************************************************************************************************************/
 #define ONE_SLAVE	       						 0
 #define MULTI_SLAVE		   					 1

 #define SPI_SLAVE_NUM	    				 ONE_SLAVE

 #if (SPI_SLAVE_NUM == MULTI_SLAVE)
 #define SLAVE_X_CSN_PIN(x)  ((x)==1 ? GPIO_PA5 : GPIO_PA4)
 #endif
#define 	DATA_BYTE_LEN     16

#if(SPI_PROTOCOL == B85_B87_B89_SLAVE_PROTOCOL)
	#define SPI_KITE_VULTURE_READ_CMD			0x80
	#define SPI_KITE_VULTURE_WRITE_CMD			0x00
	typedef struct {
	unsigned int  dma_len ;
	unsigned char address[3];
	unsigned char cmd;
	unsigned char tx_data[DATA_BYTE_LEN];
	} __attribute__((aligned(4))) spi_tx_b85_b87_b89_slave_protocol_t;

	spi_tx_b85_b87_b89_slave_protocol_t spi_tx_buff = {
	.dma_len=DATA_BYTE_LEN+4,//dma send length.
	.address = {0x04, 0x40, 0x00},
	.cmd = SPI_KITE_VULTURE_WRITE_CMD,
	.tx_data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
	};

	typedef struct {
	unsigned int  dma_len ;
	unsigned char address[3];
	unsigned char cmd;
	unsigned char rx_data[DATA_BYTE_LEN+4];
	} __attribute__((aligned(4))) spi_rx_b85_b87_b89_slave_protocol_t;
	spi_rx_b85_b87_b89_slave_protocol_t spi_rx_buff = {
		.dma_len=4,//dma send length.
		.address = {0x04, 0x40, 0x00},
		.cmd = SPI_KITE_VULTURE_READ_CMD,
		.rx_data = {0},
		};
#elif(SPI_PROTOCOL==B91M_B80_SLAVE_MODE_PROTOCOL)


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
		unsigned int  dma_len ;
		unsigned char address[4];
		unsigned int data_len;
		unsigned char data[DATA_BYTE_LEN];
	}  __attribute__((aligned(4))) spi_b91m_b80_slave_protocol_t;

	spi_b91m_b80_slave_protocol_t spi_tx_buff =
	{
		.dma_len=DATA_BYTE_LEN+8,//dma send length.
		.address = {0x00,0x04,0x40,0x00},//distinguish B91m and B80
		.data_len =DATA_BYTE_LEN,
		.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
	};
	unsigned char spi_rx_buff[DATA_BYTE_LEN+4] __attribute__((aligned(4))) = {0x00};
#elif(SPI_PROTOCOL==B91M_SLAVE_PROTOCOL)
    #define spi_slave_address	0xc0000200
	spi_config_t b91m_spi_slave_protocol_config = {
		.spi_io_mode 		= SPI_SINGLE_MODE,
		.spi_dummy_cnt    	= 8,
		.spi_cmd_en 		= 1,
		.spi_addr_en 		= 1,
		.spi_addr_len 		= 4,//when spi_addr_en = false,invalid set.
		.spi_cmd_fmt_en 	= 0,//when spi_cmd_en = false,invalid set.
		.spi_addr_fmt_en 	= 0,//when spi_addr_en=false,invalid set.
	};
	typedef struct
	{
		unsigned int  dma_len ;
		unsigned char data[DATA_BYTE_LEN];
	}  __attribute__((aligned(4))) spi_b91m_slave_protocol_t;

	spi_b91m_slave_protocol_t spi_tx_buff =
	{
		.dma_len=DATA_BYTE_LEN,//dma send length
		.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
	};
	unsigned char spi_rx_buff[DATA_BYTE_LEN+4] __attribute__((aligned(4))) = {0x00};
#endif

void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);

    gpio_set_output_en(LED1, 1); //enable output
    gpio_set_output_en(LED2, 1); //enable output

    gpio_set_input_en(LED1, 0); //disable input
    gpio_set_input_en(LED2, 0); //disable input

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
	spi_master_config_plus(&b91m_spi_slave_protocol_config);
#endif
	spi_set_irq_mask(SPI_END_INT_EN );
	irq_set_mask(FLD_IRQ_MIX_CMD_EN);
	irq_enable();
}
unsigned int mian_loop_cnt=1;
volatile  unsigned char end_irq_flag = 0;
void main_loop (void)
{
	mian_loop_cnt++;
	sleep_ms(100);
	gpio_toggle(LED1);
#if (SPI_PROTOCOL == B85_B87_B89_SLAVE_PROTOCOL)
	spi_master_write_dma( (unsigned char*)&spi_tx_buff, DATA_BYTE_LEN+4);
	while(!end_irq_flag);//Wait for spi transmission end interrupt.
	end_irq_flag = 0;
    while (spi_is_busy());//Spi transmission end interrupt doesn't mean spi is free. Better to check spi busy state again.
	spi_master_write_read_dma((unsigned char*)&spi_rx_buff,4,(unsigned char*)&spi_rx_buff.rx_data, DATA_BYTE_LEN);
    while(!end_irq_flag);//Wait for spi transmission end interrupt.
	end_irq_flag = 0;
	while (spi_is_busy());//Spi transmission end interrupt doesn't mean spi is free. Better to check spi busy state again.
	for (unsigned char i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.tx_data[i] != (spi_rx_buff.rx_data[i]))
		{
			gpio_write(LED2,1);//LED on indicate data error
		}
	}
	spi_tx_buff.tx_data[0]++;

#elif(SPI_PROTOCOL == B91M_B80_SLAVE_MODE_PROTOCOL)
	spi_master_write_dma_plus(SPI_WRITE_DATA_SINGLE_CMD, 0x00,(unsigned char*)&spi_tx_buff,DATA_BYTE_LEN+8,SPI_MODE_WR_DUMMY_WRITE);
	while(!end_irq_flag);//Wait for spi transmission end interrupt.
	end_irq_flag = 0;
	while (spi_is_busy());//Spi transmission end interrupt doesn't mean spi is free. Better to check spi busy state again.
    spi_master_read_dma_plus( SPI_READ_DATA_SINGLE_CMD, 0x00, (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);
	while(!end_irq_flag);//Wait for spi transmission end interrupt.
	end_irq_flag = 0;
	while (spi_is_busy());//Spi transmission end interrupt doesn't mean spi is free. Better to check spi busy state again.
		for (unsigned char i = 0; i < DATA_BYTE_LEN; i++)
		{
			if(spi_tx_buff.data[i] != (spi_rx_buff[i+4]))
			{
				gpio_write(LED2,1);//LED on indicate data error
			}
		}
		spi_tx_buff.data[0]++;
#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
	spi_master_write_dma_plus(SPI_SLAVE_WRITE_DATA_CMD, spi_slave_address, (unsigned char*)&spi_tx_buff, DATA_BYTE_LEN,SPI_MODE_WR_WRITE_ONLY);
	while(!end_irq_flag);//Wait for spi transmission end interrupt.
	end_irq_flag = 0;
	while (spi_is_busy());//Spi transmission end interrupt doesn't mean spi is free. Better to check spi busy state again.
	spi_master_read_dma_plus(SPI_SLAVE_READ_DATA_CMD, spi_slave_address,(unsigned char*)spi_rx_buff, DATA_BYTE_LEN,SPI_MODE_RD_DUMMY_READ);
	while(!end_irq_flag);//Wait for spi transmission end interrupt.
	end_irq_flag = 0;
	while (spi_is_busy());//Spi transmission end
		for (unsigned char i = 0; i < DATA_BYTE_LEN; i++)
		{
			if(spi_tx_buff.data[i] != (spi_rx_buff[i+4]))
			{
				gpio_write(LED2,1);//LED on indicate data error
			}
		}
		spi_tx_buff.data[0]++;

#endif

#if (SPI_SLAVE_NUM == MULTI_SLAVE)
	if(mian_loop_cnt&1)
	{
		spi_change_csn_pin(SLAVE_X_CSN_PIN(0),SLAVE_X_CSN_PIN(1));
	}
	else
	{
		spi_change_csn_pin(SLAVE_X_CSN_PIN(1),SLAVE_X_CSN_PIN(0));
	}
	mian_loop_cnt++;
#endif
}

_attribute_ram_code_sec_ void irq_handler(void)
{
	if(spi_get_irq_status(SPI_END_INT))
	{
		spi_clr_irq_status(SPI_END_INT);//clr
		end_irq_flag = 1;
	}

}
#elif((SPI_DEVICE== SPI_SLAVE_DEVICE)&&(B91M_B80_SLAVE_MODE_PROTOCOL==SPI_PROTOCOL))

/**********************************************************************************************************************
*
* * When cclk = 24MHz, the SPI_CLK range are as follows
*                                                      16 dummy cycle    8 dummy cycle
*	                                         single      max:3M             max:1.5M
* 											 dual   	 max:3M             max:1.5M
* 											 quad   	 max:3M             max:1.5M
*********************************************************************************************************************/
unsigned char spi_tx_buff[20] __attribute__((aligned(4))) = {0x10,0x00,0x00,0x00,0xdc,0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

#define SPI_RX_BUFF_LEN     28
#define DATA_BYTE_LEN    	16
#define DATA_BYTE_OFFSET    8 //must be a multiple 4
unsigned char spi_rx_buff[SPI_RX_BUFF_LEN] __attribute__((aligned(4))) = {0x00};
void user_init(void)
{
	//1.init the LED pin,for indication
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);

    gpio_set_output_en(LED1, 1); //enable output
    gpio_set_output_en(LED2, 1); //enable output

    gpio_set_input_en(LED1,0);	//disable input
    gpio_set_input_en(LED2,0);	//disable input

	gpio_set_func(GPIO_SPI_MOSI,AS_GPIO);
	gpio_set_func( GPIO_SPI_MISO,AS_GPIO);
	gpio_set_func( GPIO_SPI_CK,AS_GPIO);
	gpio_set_func( GPIO_SPI_CN,AS_GPIO);//must disable
	spi_set_pin(&spi_pin_config);
#if(SPI_SLAVE_MODE == SPI_3LINE_SLAVE)
	spi_set_3line_mode();
#endif
	spi_slave_init( SPI_MODE0);
	spi_set_irq_mask( SPI_SLV_CMD_EN );
	irq_set_mask(FLD_IRQ_SPI_EN);
	irq_enable();
	spi_set_rx_dma((unsigned char*)spi_rx_buff);
	spi_set_tx_dma((unsigned char*)(spi_rx_buff+DATA_BYTE_OFFSET));//rx_buff must have length information
}

void main_loop (void)
{
	sleep_ms(500);
	gpio_toggle(LED1);
}

_attribute_ram_code_sec_ void irq_handler(void)
{
	unsigned char slave_cmd ;
	if (spi_get_irq_status(SPI_SLV_CMD_INT))
	{
		slave_cmd = spi_slave_get_cmd();
		switch (slave_cmd)
		{
			case SPI_READ_DATA_SINGLE_CMD:
			case SPI_READ_DATA_DUAL_CMD:
			case SPI_READ_DATA_QUAD_CMD:
				spi_tx_dma_en();
			break;

			case SPI_WRITE_DATA_SINGLE_CMD:
			case SPI_WRITE_DATA_DUAL_CMD:
			case SPI_WRITE_DATA_QUAD_CMD:
				spi_rx_dma_en();
			break;
		}
		spi_clr_irq_status( SPI_SLV_CMD_INT);//clr
	}
}
#endif
#endif


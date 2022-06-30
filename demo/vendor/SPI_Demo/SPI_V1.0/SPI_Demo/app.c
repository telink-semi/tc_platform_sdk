/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

unsigned long firmwareVersion;

//To make sure the communication between master and slave device is normal,the slave device should power on first.
#define SPI_MASTER_DEVICE		1
#define SPI_SLAVE_DEVICE		2
#define SPI_DEVICE				SPI_MASTER_DEVICE

//3line mode
#define SHARE_MODE				0

#if (SPI_DEVICE==SPI_MASTER_DEVICE)

#define BUFF_DATA_LEN    		16
#define SPI_CS_PIN				GPIO_PD6

#define SPI_READ_CMD			0x80
#define SPI_WRITE_CMD			0x00

#define MCU_CORE_B89_SLAVE_ADDR				0x4102c
#define MCU_CORE_B87_SLAVE_ADDR				0x41938
#define MCU_CORE_B85_SLAVE_ADDR				0x41020
#define SLAVE_ADDR   MCU_CORE_B89_SLAVE_ADDR

#define CMD_BUF_LEN				4
unsigned char cmd_buf[4];

volatile unsigned char spi_tx_buff[BUFF_DATA_LEN]={0xAA,0x10,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xBB};
volatile unsigned char spi_rx_buff[BUFF_DATA_LEN]={0x00};

void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	spi_master_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*500000)-1),SPI_MODE0);
#if SHARE_MODE
	spi_set_share_mode_en();   //Connect the MASTER's DI and the slave's DI if you want to use this mode
#endif

#if (MCU_CORE_B89)
	spi_master_gpio_set(SPI_GPIO_SCL_A5,SPI_CS_PIN,SPI_GPIO_SDO_A2,SPI_GPIO_SDI_A0);
#elif (MCU_CORE_B87)
	spi_master_gpio_set(GPIO_PA4,SPI_CS_PIN,GPIO_PA2,GPIO_PA3);
#elif (MCU_CORE_B85)
	spi_master_gpio_set(SPI_GPIO_GROUP_A2A3A4D6);
#endif


}
void main_loop (void)
{
	sleep_ms(500);
	spi_tx_buff[2] ++;
	gpio_toggle(LED1);
	cmd_buf[0]= (SLAVE_ADDR>>16)&0xff;
	cmd_buf[1]= (SLAVE_ADDR>>8)&0xff;
	cmd_buf[2]= SLAVE_ADDR&0xff;
	cmd_buf[3]= SPI_WRITE_CMD;
	spi_write((unsigned char*)cmd_buf, CMD_BUF_LEN,(unsigned char*)spi_tx_buff, BUFF_DATA_LEN,SPI_CS_PIN);
	cmd_buf[3]= SPI_READ_CMD;
	spi_read((unsigned char*)cmd_buf, CMD_BUF_LEN,(unsigned char*)spi_rx_buff, BUFF_DATA_LEN,SPI_CS_PIN);
}

#elif(SPI_DEVICE== SPI_SLAVE_DEVICE)

void user_init()
{
	sleep_ms(2000);

	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2 ,0);			//disable input
	gpio_write(LED2, 0);              	//LED On

	spi_slave_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*500000)-1),SPI_MODE0);
#if SHARE_MODE
	spi_set_share_mode_en();  //Connect the MASTER's DI and the slave's DI if you want to use this mode
#endif

#if (MCU_CORE_B89)
	spi_slave_gpio_set(SPI_GPIO_SCL_A5,SPI_GPIO_CS_A1,SPI_GPIO_SDO_A2,SPI_GPIO_SDI_A0);
#elif (MCU_CORE_B87)
	spi_slave_gpio_set(GPIO_PA4,GPIO_PD6,GPIO_PA2,GPIO_PA3);
#elif (MCU_CORE_B85)
	spi_slave_gpio_set(SPI_GPIO_GROUP_A2A3A4D6);
#endif

	reg_irq_mask |= FLD_IRQ_MIX_CMD_EN;
	irq_enable();
}
void main_loop (void)
{
	gpio_toggle(LED1);
	sleep_ms(50);
}

#endif


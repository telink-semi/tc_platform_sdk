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

#if(I2C_MODE == I2C_DMA_MODE)

#define  	I2C_MASTER_DEVICE        	1   //i2c master demo
#define     I2C_SLAVE_DEVICE			2   //i2c master demo

#define     I2C_DEVICE					1

#define     BUFF_DATA_LEN				16
#define     SLAVE_DEVICE_ADDR			0x48000
#define     SLAVE_DEVICE_ADDR_LEN		3
#define     I2C_CLK_SPEED				200000
volatile unsigned char i2c_tx_buff[BUFF_DATA_LEN] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
volatile unsigned char i2c_rx_buff[BUFF_DATA_LEN] = {0};
volatile int irq_cnt = 0;
volatile int i2c_read_cnt = 0;
volatile int i2c_write_cnt = 0;


void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED Off

	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2 ,0);			//disable input
	gpio_write(LED2, 1);              	//LED On

#if(MCU_CORE_B87||MCU_CORE_B89||MCU_CORE_B80 || MCU_CORE_B80B)
	i2c_gpio_set(I2C_GPIO_SDA,I2C_GPIO_SCL);
#elif(MCU_CORE_B85)
	i2c_gpio_set(I2C_GPIO_SDA_SCL);
#endif
#if(I2C_DEVICE == I2C_MASTER_DEVICE)

	i2c_master_init(0x5C, (unsigned char)(CLOCK_SYS_CLOCK_HZ/(4*I2C_CLK_SPEED)) ); // 200KHz

#elif(I2C_DEVICE == I2C_SLAVE_DEVICE)

	i2c_slave_init(0x5C, I2C_SLAVE_DMA,0);
	irq_set_mask(FLD_IRQ_MIX_CMD_EN);
	irq_enable();
#endif

}

void main_loop (void)
{
#if(I2C_DEVICE == I2C_MASTER_DEVICE)
	sleep_ms(500);
	i2c_tx_buff[0]++;
	i2c_write_series(SLAVE_DEVICE_ADDR, SLAVE_DEVICE_ADDR_LEN, (unsigned char *)i2c_tx_buff, BUFF_DATA_LEN);
	sleep_ms(500);
	i2c_read_series(SLAVE_DEVICE_ADDR,  SLAVE_DEVICE_ADDR_LEN, (unsigned char *)i2c_rx_buff, BUFF_DATA_LEN);
	gpio_toggle(LED1);
#endif

}


/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	irq_cnt ++;

	unsigned char  irq_status = i2c_get_interrupt_status(FLD_HOST_CMD_IRQ|FLD_HOST_READ_IRQ);

	if(irq_status & FLD_HOST_CMD_IRQ)
	{
		i2c_clear_interrupt_status(FLD_HOST_CMD_IRQ|FLD_HOST_READ_IRQ);//clear all irq status

		if(irq_status & FLD_HOST_READ_IRQ)
		{
			i2c_read_cnt ++;
			gpio_toggle(LED1);
		}
		else
		{
			i2c_write_cnt ++;
			gpio_toggle(LED2);
		}
	}
}
#endif

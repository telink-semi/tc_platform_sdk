/********************************************************************************************************
 * @file	app_dma.c
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

   #if(MCU_CORE_B89)
	i2c_gpio_set(I2C_GPIO_SDA_A0,I2C_GPIO_SCL_A1);
   #elif(MCU_CORE_B87)
	i2c_gpio_set(I2C_GPIO_SDA_A3,I2C_GPIO_SCL_A4);
   #elif(MCU_CORE_B85)
	i2c_gpio_set(I2C_GPIO_GROUP_C0C1);
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

/********************************************************************************************************
 * @file    main.c
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
#include "calibration.h"

extern void user_init(void);
extern void main_loop (void);

int slave_rx_length = 0;
int irq_cnt = 0;
int spi_irq_cnt=0;
#define SPI_BUFFER_CNT 32
#define SPI_DATA_HEAD  0xAA
#define SPI_DATA_END   0xBB


/*The 'buff' is defined on the  "spi_slave_buff" section. The address of this section  varies with code size in the boot.link file.
  This address is also the starting address that the master can read and write, that is, "SLAVE_ADDR" in the demo.
  How to determine the specific address for "SLAVE_ADDR" in app.c file?
  step1:Compile the project in spi slave mode.
  step2:Find the ".mycode" section in the generated list file and the address corresponding to "buff" is "SLAVE_ADDR".

  The following is part of the contents of the list file.The SLAVE_ADDR is "0x841024".
  ****************************************
  Disassembly of section .my_code:
  00841024 <buff>:
  ****************************************


 */

volatile unsigned char  buff[SPI_BUFFER_CNT] __attribute__((section(".spi_slave_buff")));
unsigned char slave_buf[SPI_BUFFER_CNT];


/*SPI interrupt can be only used for Slave mode. When SPI Master writes data to Slave or reads data from Slave, SPI interrupt can be generated.
  Note that SPI interrupt flag is unable to distinguish whether this interrupt belongs to TX or RX.
  Master SPI Can send data according to the data transmission protocol below to help Slave SPI to determine data length and integrity.
  Data transmission protocol of Master SPI is as follows:
  DataHead DataLen xx xx xx  DataEnd
  eg. unsigned char TxBuffer[5]={0xAA,0x05,0x01,0x02,0xBB};
  DataHead = 0xAA ; DataLen =0x05; DataEnd= 0xBB;

 *The process of SPI interrupt in Slave Mode is as follows .
 *Step 1: Judge whether SPI interrupt generates.
 *Step 2: Judge DataHead is correct.
 *Step 3: Judge whether DataEnd is correct.
 *if buff data is not equal to slave buff, copy buff data to slave buff.
 *Note that once one write irq is executed successfully, subsequent read irq will also meet Step 1,2 and 3.
 */

void spi_slave_irq_handler(void)
{
	if(buff[0]==SPI_DATA_HEAD){//Judge whether DataHead is correct.
		slave_rx_length=buff[1];
		if( buff[slave_rx_length-1]==SPI_DATA_END ){//Judge whether DataEnd is correct.
			for(int i=0;i<slave_rx_length;i++){
				if(slave_buf[i]!=buff[i]){//Get data that Master SPI writes to Slave.
					slave_buf[i]=buff[i];//If Master data has been updated, copy data to slave buff.
				}
			}
		}
	}
}
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	irq_cnt ++;

	unsigned char  irq_status = reg_spi_slave_irq_status;
	//An interrupt is triggered after each master spi write or read is completed (CS changes from low to high).
	//Note that it is unable to distinguish which condition triggers it.
	if(irq_status & FLD_SLAVE_SPI_IRQ)
	{
		reg_spi_slave_irq_status = irq_status;
		spi_irq_cnt ++;
		spi_slave_irq_handler();
	}
}
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)   //must on ramcode
{
    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}


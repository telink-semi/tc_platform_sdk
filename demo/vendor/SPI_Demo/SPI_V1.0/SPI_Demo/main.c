/********************************************************************************************************
 * @file	main.c
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
#include "calibration.h"

extern void user_init();
extern void main_loop (void);

int spi_read_cnt = 0;
int spi_write_cnt = 0;
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
unsigned char slave_rxbuf[SPI_BUFFER_CNT];


/*SPI interrupt can be only used for Slave mode. When SPI Master writes data to Slave or reads data from Slave, SPI interrupt can be generated.
  Note that SPI interrupt flag is unable to distinguish whether this interrupt belongs to TX or RX.
  Master SPI Can send data according the next data transmission protocol to help Slave SPI judge whether this interrupt belongs to TX or RX.
  Data transmission protocol of Master SPI is as follows:
  DataHead DataLen xx xx xx  DataEnd
  eg. unsigned char TxBuffer[5]={0xAA,0x05,0x01,0x02,0xBB};
  DataHead = 0xAA ; DataLen =0x05; DataEnd= 0xBB;

 *The process of SPI interrupt in Slave Mode is as follows .
 *Step 1: Judge whether SPI interrupt generates.
 *Step 2: Judge DataHead is correct.
 *Step 3: Judge whether DataEnd is correct.
 *When all the above conditions are met,this process is that Master SPI writes data to the Slave.
 *When SPI interrupt generates and Step 2 and Step 3 can not meet the conditions, this process is that Master SPI reads data from Slave.

 */

void spi_slave_irq_handler(void)
{
	if(buff[0]==SPI_DATA_HEAD){//Judge whether DataHead is correct.
		slave_rx_length= buff[1];
		if( buff[slave_rx_length-1]==SPI_DATA_END ){//Judge whether DataEnd is correct.
		   spi_write_cnt++;  //
		   for(int i = 0;i<slave_rx_length;i++){
			   slave_rxbuf[i]=buff[i];//Get data that Master SPI writes to Slave.
		   }
		 }
	}
	else{ spi_read_cnt++;}
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
	//SPI Interrupt means that every WRITE or READ will generate one interrupt(capture CS signal)
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

#if (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B89)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#endif

#if (MCU_CORE_B85) || (MCU_CORE_B87)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif (MCU_CORE_B89)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_otp_value_calib();
#endif

	gpio_init(0);

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}


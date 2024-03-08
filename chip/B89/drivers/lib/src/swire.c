/********************************************************************************************************
 * @file	swire.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "lib/include/swire.h"
#include "timer.h"
#include "gpio.h"
/**
 * @brief     This function is to send a specific timing to reset the status of the slave device.
 * 			  When the master device connects the DP pin of the slave device, this function must be called first,
 * 			  because the DP pin of the slave device is USB function by default. Only after calling this function to send a specific timing,
 * 			  the slave device will be enabled as the swire function.
 * 			  When communication fails, you can also try to call this function to resume communication.
 * @param[in] gpio_swm - the swm pin.
 * @param[in] dp_through_swire_en - If the master is connected to the DP pin of the slave device, this parameter needs to be set to 1.
 * @return	  none.
 */
void swire_sync (swm_pin_e gpio_swm,unsigned char dp_through_swire_en)
{
	swire_reset();
	//gpio output
	gpio_set_input_en(gpio_swm,0);
		gpio_set_output_en(gpio_swm, 1); 		//enable output
		gpio_set_func(gpio_swm ,AS_GPIO);


	if (dp_through_swire_en) {

		gpio_write(gpio_swm,0);
		sleep_ms (40);		//wait 40ms
		gpio_write(gpio_swm,1);
		sleep_ms (80);		//wait 80ms
		gpio_write(gpio_swm,0);
		sleep_ms (40);		//wait 40ms
		gpio_write(gpio_swm,1);
		sleep_ms (20);		//wait 20ms
	}
	gpio_write(gpio_swm,1);
	sleep_us (10);
	int i;
	for (i=0; i<12; i++) {
		gpio_write(gpio_swm,0);
		sleep_us (4);
		gpio_write(gpio_swm,1);
		sleep_us (1);
	}
	swire_set_swm_en(gpio_swm);
}
/**
 * @brief     This function is to set gpio as the swm function.
 * @param[in] gpio_swm - the swm pin needs to enable.
 * @return	  none.
 */
void swire_set_swm_en(swm_pin_e gpio_swm)
{
	if(gpio_swm == SWM_PA5)
		gpio_set_func(gpio_swm,PA5_SWM);
	if(gpio_swm == SWM_PA6)
		gpio_set_func(gpio_swm,PA6_SWM);
	gpio_set_input_en(gpio_swm,1);
}

/**
 * @brief     This function is to set GPIO_PA7 as the sws function.
 * @return	  none.
 */
void swire_set_sws_en(void)
{
    unsigned char	bit = GPIO_SWS & 0xff;
	reg_gpio_func_mux(GPIO_SWS)= reg_gpio_func_mux(GPIO_SWS) & 0x0f;
	BM_CLR(reg_gpio_func(GPIO_SWS), bit);
	gpio_set_input_en(GPIO_SWS,1);
}

/**
 * @brief     This function is to set the clock of the swire module,
 * 			  the clock source of the swire module is sys_clk, the clock of the actual swire module:  swire_clock = sys_clk/div.
 * 			  swire_clock cannot be greater than sys_clk.(div cannot be 0)
 * 			  Whether the chip as a master or slave, clock source are the following principles:
 * 			  the clock for sending data is swire_clock, and the clock for detecting received data is sys_clk.
 * @div_clock - swire_div
 * @return	  none.
 */
void swire_set_clk(unsigned char div_clock)
{
	reg_swire_ctl2 = (div_clock & 0x7f);
}

/**
 * @brief     This function is used to set the ID of the slave.
 *			  When the host accesses multiple slaves, the device can be distinguished by the ID of the slave.
 * @param[in] id	- slave id.The setting range is 0~0x1f.
 * @return	  none.
 */
void swire_set_slave_id(unsigned char id)
{
	BM_CLR(reg_swire_id, FLD_SWIRE_ID_VALID);
	BM_SET(reg_swire_id, id & FLD_SWIRE_ID_VALID);
}
/**
 * @brief     This function is used by the master device to write data to the slave device.
 *			  The swire protocol is to send the address first and then send the data.
 * @param[in] slave_id	- slave id.The setting range is 0~0x1f.
 * 						  [6:5]: this function tells the slave to operate in byte/half-word/word.
 * 						  		 the reason is that some addresses on the slave side can only be operated with 4 bytes.
 * 						  		 10--32bit; 01--16bit ; 00--8bit
 * 						  [4:0]: ID
 * @param[in] addr 		- slave address(little endian format).
 * @param[in] addr_len 	- slave address byte length,It is determined by the slave device, for example: B91s is four bytes, B85s is three bytes.
 * @param[in] data 		- The data that needs to be written to the slave.
 * @param[in] data_len	- data length.
 * @return	  none.
 */

void swire_master_write(unsigned char slave_id,unsigned char *addr, unsigned char addr_len,unsigned char *data,unsigned int data_len)
{
	unsigned int i;
	swire_reset();
	swire_master_write_cmd(0xff);
	swire_master_write_cmd(0x5a);
	for(i=0;i<addr_len;i++){
		swire_master_write_data(addr[addr_len-1-i]);
	}
	swire_master_write_data(slave_id & 0x1f);//bit7:0 for write;bit4~bit0 :slave id
	for(i=0;i<data_len;i++){
		swire_master_write_data(data[i]);//0 for write
	}
	swire_master_write_cmd(0xff);
}
/**
 * @brief      This function is used by the master device to read data to the slave device.
 *			   The swire protocol is to send the address first and then wait to read the data returned by the slave.
 * @param[in] slave_id	 - slave id.The setting range is 0~0x1f.
 * 						 	 [6:5]: this function tells the slave to operate in byte/half-word/word.
 * 						  			the reason is that some addresses on the slave side can only be operated with 4 bytes.
 * 						  			10--32bit; 01--16bit ; 00--8bit
 * 						  	 [4:0]: ID
 * @param[in]  addr 	 - slave address(little endian format).
 * @param[in]  addr_len  - slave address byte length,It is determined by the slave device, for example: B91s is four bytes, B85s is three bytes.
 * @param[out] data 	 - this is the data read from the slave.
 * @param[in]  data_len	 - data length.
 * @return	   0:read timeout  1:read success.
 */

unsigned char  swire_master_read (unsigned char slave_id,unsigned char *addr, unsigned char addr_len,unsigned char *data,unsigned int data_len)
{
	unsigned int i;
	swire_reset();
	swire_master_write_cmd(0xff);
	swire_master_write_cmd(0x5a);
	for(i=0;i<addr_len;i++){
		swire_master_write_data(addr[addr_len-1-i]);
	}
	swire_master_write_data(0x80 | (slave_id & 0x1f));//bit7: 1 for read;bit4~bit0 :slave id
	for(i=0;i<data_len;i++){
		reg_swire_ctl =  FLD_SWIRE_RD ;
		unsigned long t =  clock_time();
		while ( clock_time() - t < 300000){
			if (!(reg_swire_ctl & FLD_SWIRE_RD)) {
				data[i] = reg_swire_data;
				break;
			}
		}
	}
	swire_master_write_cmd(0xff);
	if(i<data_len){
		return 0;
	}else{
		return 1;
	}

}




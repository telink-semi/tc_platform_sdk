/********************************************************************************************************
 * @file    spi.c
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
#include "spi.h"
#include "register.h"
#include "gpio.h"


/**
 * @brief      This function selects  pin for spi master or slave mode.
 * @param[in]  pin  - the selected pin.
 * @return     none
 */
void spi_set_pin_mux(GPIO_PinTypeDef pin,gpio_func_e function)
{
	if (pin != 0)
	{
		/* only GPIO_SPI_IO2=GPIO_PB3, GPIO_SPI_IO3=GPIO_PD4*/
		gpio_set_input_en(pin,1);
		gpio_set_func( pin, function);
	}
}

/**
 * @brief     	This function servers configures spi pin.
 * @param[in] 	config - the pointer of pin config struct.
 * @return 		none
 */
void spi_set_pin(spi_pin_config_t *spi_pin_config)
{
	spi_set_pin_mux(spi_pin_config->spi_clk_pin,(spi_pin_config->spi_clk_pin != GPIO_SPI_CK) ? SPI_CLK:0);
	spi_set_pin_mux(spi_pin_config->spi_csn_pin,(spi_pin_config->spi_csn_pin != GPIO_SPI_CN) ? SPI_CSN:0);

	spi_set_pin_mux(spi_pin_config->spi_mosi_io0_pin,(spi_pin_config->spi_mosi_io0_pin != GPIO_SPI_MOSI) ? SPI_MOSI_IO0:0);
	spi_set_pin_mux(spi_pin_config->spi_miso_io1_pin,(spi_pin_config->spi_miso_io1_pin != GPIO_SPI_MISO) ? SPI_MISO_IO1:0);
	spi_set_pin_mux(spi_pin_config->spi_wp_io2_pin,0);
	spi_set_pin_mux(spi_pin_config->spi_hold_io3_pin,0);
}

/**
 * @brief     	This function disable spi csn pin.
 * @param[in] 	pin - the csn pin.
 * @return 		none
 */
void spi_cs_pin_dis(GPIO_PinTypeDef pin)
{
	gpio_set_output_en(pin,1);
	gpio_set_input_en(pin,0);
	gpio_write(pin,1);
	gpio_set_func( pin, AS_GPIO);
}
/**
 * @brief     	This function change hspi csn pin.
 * @param[in] 	current_csn_pin - the current csn pin.
 * @param[in] 	next_csn_pin    - the next csn pin.
 * @return 		none.
 */
void spi_change_csn_pin(GPIO_PinTypeDef current_csn_pin,GPIO_PinTypeDef next_csn_pin)
{
	spi_cs_pin_dis(current_csn_pin);
	spi_set_pin_mux(next_csn_pin,(next_csn_pin != GPIO_SPI_CN) ? SPI_CSN:0);
}
/**
 * @brief     	This function configures the clock and working mode for SPI master interface.
 * @param[in] 	div_clock 	- the division factor for SPI module.
 *           	spi_clock_out = ahb_clock / ((div_clock+1)*2)
 * @param[in]	 mode 		- the selected working mode of SPI module.
 *           	bit5:CPHA-Clock Polarity ; bit6:CPOL:CPHA-Clock Phase
 *            	MODE0:  CPOL = 0, CPHA =0;
 *            	MODE1:  CPOL = 0, CPHA =1;
 *            	MODE2:  CPOL = 1, CPHA =0;
 *            	MODE3:  CPOL = 1, CPHA =1;
 * @return  	none
 */
void spi_master_init(unsigned char div_clock, spi_mode_type_e mode)
{
	 reg_spi_div = div_clock;
	 reg_spi_mode0	|= FLD_SPI_MASTER_MODE;//master
	 reg_spi_mode0 = ((reg_spi_mode0 & (~FLD_SPI_MODE_WORK_MODE)) | (mode << 5));// select SPI mode, support four modes
}

/**
 * @brief     	This function configures work mode for SPI slave interface.
 * @param[in] 	mode 	- the selected working mode of SPI module.
 *          	bit5:CPHA-Clock Polarity ; bit6:CPOL:CPHA-Clock Phase
 *            	MODE0:  CPOL = 0, CPHA =0;
 *            	MODE1:  CPOL = 0, CPHA =1;
 *            	MODE2:  CPOL = 1, CPHA =0;
 *            	MODE3:  CPOL = 1, CPHA =1;
 * @return  	none
 * @note  		spi_clock_in  (spi_slave_clock frequency)/3
 */
void spi_slave_init(spi_mode_type_e mode)
{
	 reg_spi_mode0	&= (~FLD_SPI_MASTER_MODE);//slave
	 reg_spi_mode0 = ((reg_spi_mode0 & (~FLD_SPI_MODE_WORK_MODE)) | (mode << 5));// select SPI mode, support four modes
}

/**
 * @brief     	This function servers to set normal mode.
 * @return  	none
 */
void spi_set_normal_mode(void)
{
	spi_dual_mode_dis();
	spi_3line_mode_dis();
	spi_quad_mode_dis();
}

/**
 * @brief     	This function servers to set dual mode.
 * @return  	none
 */
void spi_set_dual_mode(void)
{
	spi_dual_mode_en();//quad  precede over dual
	spi_3line_mode_dis();
	spi_quad_mode_dis();
}

/**
 * @brief	This function servers to set quad mode.
 * @return	none
 */
void spi_set_quad_mode(void)
{
	spi_quad_mode_en();
	spi_dual_mode_dis();
	spi_3line_mode_dis();
}

/**
 * @brief    This function servers to set 3line mode.
 * @return  none
 */
void spi_set_3line_mode(void)
{
	/*must disable dual and quad*/
	spi_3line_mode_en();
	spi_dual_mode_dis();
	spi_quad_mode_dis();

}

/**
 * @brief     	This function servers to set spi io  mode.
 * @param[in]	mode 	- single/dual/quad /3line.
 * @return  	none
  */
void spi_set_io_mode( spi_io_mode_e mode)
{
		switch (mode)
		{
			case SPI_SINGLE_MODE:
				spi_set_normal_mode();
				break;
			case SPI_DUAL_MODE:
				spi_set_dual_mode();
				break;
			case SPI_QUAD_MODE:
				spi_set_quad_mode();
				break;
			case SPI_3LINE_MODE:
				spi_set_3line_mode();
				break;
		}
}

/**
 * @brief     	This function servers to set dummy cycle cnt.
 * @param[in] 	dummy_cnt 	- the cnt of dummy clock.
 * @return  	none
 */
void spi_set_dummy_cnt( unsigned char dummy_cnt)
{
	reg_spi_trans0 |= ((reg_spi_trans0 & (~FLD_SPI_DUMMY_CNT)) | ((dummy_cnt - 1) & FLD_SPI_DUMMY_CNT));
}

/**
 * @brief     	This function servers to config normal mode.
 * @param[in] 	mode 	- nomal ,mode 3line.
 * @return  	none
 */
void spi_master_config( spi_nomal_3line_mode_e mode)
{
	spi_cmd_dis();
	spi_addr_dis();
	spi_set_io_mode(mode);
}

/**
 * @brief     	This function servers to config spi special mode.
 * @param[in] 	config 	- the pointer of pin special config struct.
 * @return  	none
 */
void spi_master_config_plus(spi_config_t *config)
{
	spi_set_io_mode( config->spi_io_mode);
	spi_set_addr_len(config->spi_addr_len);
	spi_set_dummy_cnt( config->spi_dummy_cnt);

	if (config->spi_cmd_en == 1)
	{
		spi_cmd_en();
	}
	else if (config->spi_cmd_en == 0)
	{
		spi_cmd_dis();
	}

	if (config->spi_cmd_fmt_en == 1)
	{
		spi_cmd_fmt_en();
	}
	else if (config->spi_cmd_fmt_en == 0)
	{
	   spi_cmd_fmt_dis();
	}

	if (config->spi_addr_en == 1)
	{
		spi_addr_en();
	}
	else if (config->spi_addr_en == 0)
	{
		spi_addr_dis();
	}

	if (config->spi_addr_fmt_en == 1)
	{
		spi_addr_fmt_en();
	}
	else if (config->spi_addr_fmt_en == 0)
	{
		spi_addr_fmt_dis();
	}
}

/**
 * @brief     	This function servers to set spi transfer mode.
 * @param[in] 	mode 	- transfer mode.
 * @return  	none
 */
void spi_set_transmode( spi_tans_mode_e mode)
{
	reg_spi_trans0 = ((reg_spi_trans0 & (~FLD_SPI_TRANSMODE)) | ((mode & 0xf) << 4));
}

/**
 * @brief 		This function servers to get spi slave ready status. When slave is ready, slave ready reply a byte data:0x5a.
 * slave reply a byte data: 0x00.indicating that slave is not ready for data transmission.
 * @param[in] 	mode	 	- the spi master io mode.
 * @return 		1:Indicates that the slave is ready. 0:Indicates that slave is not ready.
 */
_Bool spi_master_get_slave_ready(spi_io_mode_e mode)
{
	unsigned char slave_ready = 0;
	spi_rx_fifo_clr();
	spi_rx_cnt(1);
	spi_set_transmode(SPI_MODE_DUMMY_READ);
	switch (mode)
	{
		case SPI_SINGLE_MODE:
		case SPI_3LINE_MODE:
			spi_master_set_cmd(SPI_READ_STATUS_SINGLE_CMD);
			break;
		case SPI_DUAL_MODE:
			spi_master_set_cmd(SPI_READ_STATUS_DUAL_CMD);
			break;
		case SPI_QUAD_MODE:
			spi_master_set_cmd(SPI_READ_STATUS_QUAD_CMD);
			break;
	}
	spi_read( (unsigned char*)&slave_ready, 1);

	if (slave_ready == 0x5a)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief     	This function servers to write spi fifo.
 * @param[in] 	data 	- the pointer to the data for write.
 * @param[in] 	len 	- write length.
 * @return    	none
 */
void spi_write( unsigned char *data, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
	{
		while (reg_spi_fifo_state & FLD_SPI_TXF_FULL);
		reg_spi_wr_rd_data( i % 4) = data[i];
	}
}

/**
 * @brief     	This function servers to write spi fifo by word.
 * @param[in] 	data 	- the pointer(unsigned int*) to the data for write.
 * @return    	none
 */
void spi_write_word( unsigned int *data)
{
	 reg_spi_wr_rd_data32= *data;
}

/**
 * @brief     	This function servers to read spi fifo.
 * @param[in] 	data 	- the pointer to the data for read.
 * @param[in] 	len 	- write length.
 * @return    	none
 */
void spi_read( unsigned char *data, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
	{
	    while (reg_spi_fifo_state & FLD_SPI_RXF_EMPTY);
	    data[i] = reg_spi_wr_rd_data( i % 4);
	}
}

 /**
  * @brief     	This function servers to read spi fifo.
  * @param[in] 	data 	- the pointer(unsigned int*) to the data for read.
  * @return    	none
  */
void spi_read_word( unsigned int *data)
{
	 *data = reg_spi_wr_rd_data32;
}


/**
 * @brief     	This function serves to normal write data in normal.
 * @param[in] 	data 	- the pointer to the data for write.
 * @param[in] 	len 	- write length.
 * @return  	none
 */
void spi_master_write( unsigned char *data, unsigned int len)
{
	spi_tx_dma_dis();
	spi_tx_fifo_clr();
	spi_tx_cnt(len);
	spi_set_transmode( SPI_MODE_WRITE_ONLY);
	spi_master_set_cmd(0x00);//when  cmd  disable that  will not sent cmd,just trigger spi send .
	spi_write((unsigned char *)data, len);
	while (spi_is_busy());
}


/**
 * @brief     	This function serves to normal write and read data.
 * @param[in] 	wr_data - the pointer to the data for write.
 * @param[in] 	wr_len 	- write length.
 * @param[in] 	rd_data - the pointer to the data for read.
 * @param[in] 	rd_len 	- read length.
 * @return  	none
 */
void spi_master_write_read( unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len)
{
	spi_tx_dma_dis();
	spi_rx_dma_dis();
	spi_tx_fifo_clr();
	spi_rx_fifo_clr();
	spi_tx_cnt(wr_len);
	spi_rx_cnt(rd_len);
	spi_set_transmode(SPI_MODE_WRITE_READ);
	spi_master_set_cmd(0x00);//when  cmd  disable that  will not sent cmd,just trigger spi send .
	spi_write((unsigned char *)wr_data, wr_len);
	spi_read( (unsigned char *)rd_data, rd_len);
	while (spi_is_busy());
}

/**
 * @brief     	This function serves to single/dual/quad write to the SPI slave.
 * @param[in] 	cmd 		- cmd one byte will first write.
 * @param[in] 	addr 		- the address of slave.
 * @param[in] 	data 		-  pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @param[in] 	wr_mode 	- write mode.dummy or not dummy.
 * @return  	none
 */
void spi_master_write_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode)
{
	spi_tx_dma_dis();
	spi_tx_fifo_clr();
	if(addr)
	{
		spi_set_address(addr);
	}
	spi_set_transmode(wr_mode);
	spi_tx_cnt( data_len);
	spi_master_set_cmd(cmd);
	spi_write((unsigned char *)data, data_len);
	while (spi_is_busy());
}




/**
 * @brief     	This function serves to single/dual/quad write (fetch data from flash )to the SPI slave.
 * @param[in] 	addr 		- the address of slave.
 * @param[in] 	cmd 		- cmd one byte will first write.
 * @param[in] 	data 		-  pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @param[in] 	wr_mode 	- write mode.dummy or not dummy.
 * @return  	none
 */
void spi_master_write_plus_fetch_flash( unsigned char cmd,unsigned int addr,unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode)
{
	spi_tx_dma_dis();
	spi_tx_fifo_clr();
	spi_set_fetch_flash_address((unsigned int)data);//set the address for fetching data from flash.
	reg_spi_panel_ctrl|=FLD_SPI_FETCH_FLASH;
	if(addr)
	{
		spi_set_address(addr);
	}
	spi_set_transmode(wr_mode);
	spi_tx_cnt( data_len);
	spi_master_set_cmd(cmd);
	while (spi_is_busy());
}



/**
 * @brief     	This function serves to single/dual/quad  read from the SPI slave.
 * @param[in]  	cmd 		- cmd one byte will first write.
 * @param[in]  	addr 		- the address of slave.
 * @param[in]  	data 		- pointer to the data need to read.
 * @param[in]  	data_len 	- the length of data.
 * @param[in]  	rd_mode 	- read mode.dummy or not dummy.
 * @return   	none
 */
void spi_master_read_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e rd_mode)
{
	spi_rx_dma_dis();
	spi_rx_fifo_clr();
	if(addr)
	{
		spi_set_address( addr);
	}
	spi_set_transmode( rd_mode);
	spi_rx_cnt( data_len);
	spi_master_set_cmd(cmd);
	spi_read((unsigned char *)data, data_len);
	while (spi_is_busy());
}

/**
 * @brief       This function serves to set dma tx burst.
 * @param[in]  	burst_size  0-word ,1- 2 words,2-4words
 * @return   	none
 */
void spi_set_dma_tx_burst( spi_burst_size_e  burst_size )
{
	spi_set_dma_burst_size(burst_size);
	spi_rtx_irq_trig_cnt((burst_size == 0)? 4:(8*burst_size) );
	spi_set_burst_size(burst_size);
}
/**
 * @brief     	This function serves to normal write data by dma.
 * @param[in] 	data 	   - the pointer to the data for write.
 * @param[in] 	data_len   - write length.
 * @return  	none
 */
void spi_master_write_dma( unsigned char *data, unsigned int data_len)
{
	reg_dma9_addrhh = (unsigned char)((unsigned int)data>>16);
	reg_dma9_addrh  = (unsigned char)((unsigned int)data>>8);
    reg_dma9_addrl =  (unsigned char)((unsigned int)data);
    reg_dma9_size = 0xff;

	spi_tx_cnt(data_len);
	spi_tx_dma_en();
	spi_set_transmode(SPI_MODE_WRITE_ONLY);
	spi_master_set_cmd(0x00);
}

/**
 * @brief     	This function serves to normal write cmd and address, then read data by dma.
 * @param[in] 	tx_data     - the pointer to the cmd and address for write.
 * @param[in] 	tx_len 	    - write length.
 * @param[in] 	rx_data 	- the pointer to the data for read.
 * @param[in] 	rx_len 	    - read length.
 * @return  	none
 */
void spi_master_write_read_dma( unsigned char *tx_data, unsigned int tx_len, unsigned char *rx_data, unsigned int rx_len)
{
	spi_rx_fifo_clr();
	spi_tx_fifo_clr();
	/*dma tx*/
	reg_dma9_addrhh = (unsigned char)((unsigned int)tx_data>>16);
	reg_dma9_addrh  = (unsigned char)((unsigned int)tx_data>>8);
	reg_dma9_addrl =  (unsigned char)((unsigned int)tx_data);
	reg_dma9_size = 0xff;

	spi_tx_cnt(tx_len);
	spi_tx_dma_en();

	/*dma   rx*/
	reg_dma8_addrhh = (unsigned char)((unsigned int)rx_data>>16);
	reg_dma8_addrh  = (unsigned char)((unsigned int)rx_data>>8);
	reg_dma8_addrl =  (unsigned char)((unsigned int)rx_data);
	reg_dma8_mode=FLD_DMA_WR_MEM;

	spi_rx_cnt(rx_len);
	spi_rx_dma_en();
	spi_set_transmode(SPI_MODE_WRITE_READ);
	spi_master_set_cmd(0x00);
}

/**
 * @brief     	This function serves to write and read data simultaneously by dma.
 * @param[in] 	tx_data     - the pointer to the data for write.
 * @param[in] 	rx_data 	- the pointer to the data for read.
 * @param[in] 	len 	    - data length.
 * @return  	none
 */
void spi_master_write_read_dma_full_duplex(unsigned char* tx_data,unsigned char* rx_data,unsigned int len)
{
	spi_rx_fifo_clr();
	spi_tx_fifo_clr();
	/*dma tx*/
	reg_dma9_addrhh = (unsigned char)((unsigned int)tx_data>>16);
	reg_dma9_addrh  = (unsigned char)((unsigned int)tx_data>>8);
	reg_dma9_addrl =  (unsigned char)((unsigned int)tx_data);
	reg_dma9_size = 0xff;
	spi_tx_cnt(len);
	spi_tx_dma_en();

	/*dma   rx*/
	reg_dma8_addrhh = (unsigned char)((unsigned int)rx_data>>16);
	reg_dma8_addrh  = (unsigned char)((unsigned int)rx_data>>8);
	reg_dma8_addrl =  (unsigned char)((unsigned int)rx_data);
	reg_dma8_mode=FLD_DMA_WR_MEM;

	spi_rx_cnt(len);
	spi_rx_dma_en();
	spi_set_transmode(SPI_MODE_WRITE_AND_READ);
	spi_master_set_cmd(0x00);
}

/**
 * @brief      	This function serves to single/dual/quad  write to the SPI slave by dma.
 * @param[in]  	cmd 		- cmd one byte will first write.
 * @param[in]  	addr 		- the address of slave.
 * @param[in]  	data 	- pointer to the data need to write.
 * @param[in]  	data_len 	- length in byte of the data need to write.
 * @param[in]  	wr_mode 	- write mode.dummy or not dummy.
 * @return   	none
 */
void spi_master_write_dma_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode)
{
	spi_tx_fifo_clr();
	reg_dma9_addrhh = (unsigned char)((unsigned int)data>>16);
	reg_dma9_addrh  = (unsigned char)((unsigned int)data>>8);
    reg_dma9_addrl =  (unsigned char)((unsigned int)data);

    reg_dma9_size = 0xff;
    if(addr)
  	{
  		spi_set_address(addr);
  	}
    spi_tx_cnt(data_len);
    spi_tx_dma_en();
    spi_set_transmode(wr_mode);
	spi_master_set_cmd(cmd);
}

/**
 * @brief      	This function serves to single/dual/quad  read from the SPI slave by dma.
 * @param[in]  	cmd 		- cmd one byte will first write.
 * @param[in]  	addr 		- the address of slave.
 * @param[in]  	data 	    - pointer to the buffer that will cache the reading out data.
 * @param[in]  	data_len    - length in byte of the data need to read.
 * @param[in]  	rd_mode 	- read mode.dummy or not dummy.
 * @return   	none
 */
void spi_master_read_dma_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e rd_mode)
{
	spi_rx_fifo_clr();
	reg_dma8_addrhh = (unsigned char)((unsigned int)data>>16);
	reg_dma8_addrh  = (unsigned char)((unsigned int)data>>8);
	reg_dma8_addrl  = (unsigned char)((unsigned int)data);
	reg_dma8_mode=FLD_DMA_WR_MEM;
	if(addr)
	{
		spi_set_address(addr);
	}
	spi_rx_cnt(data_len);
	spi_set_transmode(rd_mode);
	spi_rx_dma_en();

	spi_master_set_cmd(cmd);

}

/**
 * @brief   	this  function set spi rx dma channel.
 * @param[in]  	rx_data 	- the address of destination.
 * */
void spi_set_rx_dma( unsigned char* rx_data)
{
	reg_dma8_addrhh = (unsigned char)((unsigned int)rx_data>>16);
	reg_dma8_addrh  = (unsigned char)((unsigned int)rx_data>>8);
	reg_dma8_addrl =  (unsigned char)((unsigned int)rx_data);
	reg_dma8_mode=FLD_DMA_WR_MEM;

}


/**
 * @brief   	this  function set spi tx dma channel.
 * @param[in]  	tx_data 	- the address of source.
 * */
void spi_set_tx_dma(unsigned char* tx_data)
{
	reg_dma9_addrhh = (unsigned char)((unsigned int)tx_data>>16);
	reg_dma9_addrh  = (unsigned char)((unsigned int)tx_data>>8);
	reg_dma9_addrl =  (unsigned char)((unsigned int)tx_data);
    reg_dma9_size = 0xff;
}


/**
 * @brief      	This function serves to write and read data simultaneously.
 * This interface can only be used by the master.
 * When initializing the master, call the spi_master_config() interface to disable the hardware cmd and hardware address,
 * and then start sending and receiving data.
 * The default chunk size sent and received by this interface is 8byte.
 * @param[in]  	write_data  - write data pointer.
 * @param[in]  	read_data 	- read data pointer.
 * @param[in]  	len 	    - write/read length.
 * @return   	none
 */
void spi_master_write_read_full_duplex(unsigned char *write_data, unsigned char *read_data, unsigned int len)
{
	unsigned int spi_wr_rd_size = 8;
	unsigned int chunk_size = spi_wr_rd_size;
	spi_tx_dma_dis();
	spi_rx_dma_dis();
	spi_set_transmode(SPI_MODE_WRITE_AND_READ);
	spi_master_set_cmd(0);
	spi_tx_fifo_clr();
	spi_rx_fifo_clr();
	spi_tx_cnt(len);
	spi_rx_cnt(len);

	for(unsigned int i = 0; i<len; i = i +chunk_size){
		if(chunk_size > (len - i)){
			chunk_size = len - i;
		}
		spi_write( write_data + i, chunk_size);
		if(len < spi_wr_rd_size){
			spi_read( read_data, chunk_size);
		}
		else if(i == 0){
			spi_read(read_data, chunk_size - 1);
		}else if((len - i) > spi_wr_rd_size){
			spi_read(read_data + i -1, chunk_size);
		}else{
			spi_read(read_data + i -1, chunk_size + 1);
		}
		spi_rx_fifo_clr();
		spi_tx_fifo_clr();
	}
	while(spi_is_busy());
}

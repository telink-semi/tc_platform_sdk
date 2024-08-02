/********************************************************************************************************
 * @file    spi.h
 *
 * @brief   This is the header file for B80
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
#pragma once
#ifndef SPI_H
#define SPI_H

#include "bsp.h"
#include "gpio.h"
#include "compiler.h"
typedef struct spi_pin_config_t
{
	GPIO_PinTypeDef      spi_clk_pin;
	GPIO_PinTypeDef      spi_csn_pin;
	GPIO_PinTypeDef      spi_mosi_io0_pin;
	GPIO_PinTypeDef      spi_miso_io1_pin;
	GPIO_PinTypeDef      spi_wp_io2_pin;  //only GPIO_PB4 support
	GPIO_PinTypeDef      spi_hold_io3_pin;//only  GPIO_PB4 support
}spi_pin_config_t;

/**
 *  @brief  Define the mode for SPI interface
 */
typedef enum {
    SPI_MODE0 = 0,
    SPI_MODE2,
    SPI_MODE1,
    SPI_MODE3,
} spi_mode_type_e;
/**
 * @brief  Define the mode for SPI io mode.
 */
typedef enum{
	SPI_SINGLE_MODE	 = 0,
    SPI_DUAL_MODE	 = 1 ,
    SPI_QUAD_MODE 	 = 2,
	SPI_3LINE_MODE	 = 3
} spi_io_mode_e;

typedef enum{
	SPI_SINGLE = 0,
    SPI_3LINE = 3,
} spi_nomal_3line_mode_e;




typedef struct{
	spi_io_mode_e  spi_io_mode;//set spi interface mode
	unsigned char spi_dummy_cnt;//set dummy cnt if tans_mode have dummy .
	unsigned char spi_cmd_en;//enable cmd phase
	unsigned char spi_addr_en;//enable address phase
	unsigned char spi_addr_len;//enable address phase
	unsigned char spi_cmd_fmt_en;//if cmd_en enable cmd fmt will follow the interface (dual/quad)
	unsigned char spi_addr_fmt_en;//if addr_en enable addr fmt will follow the interface (dual/quad)
}spi_config_t;

/**
 * @brief  Define the SPI command & translate mode.
 */
typedef enum{
	SPI_MODE_WRITE_AND_READ = 0,//write and read at the same.
	SPI_MODE_WRITE_ONLY,//write
	SPI_MODE_READ_ONLY,// read must enable CmdEn
	SPI_MODE_WRITE_READ,//write_ read
	SPI_MODE_READ_WRITE,//read_write
	SPI_MODE_WRITE_DUMMY_READ,//write_dummy_read
	SPI_MODE_READ_DUMMY_WRITE,//read_ dummy_write must enable CmdEn
	SPI_MODE_NONE_DATA,//must enable CmdEn
	SPI_MODE_DUMMY_WRITE,//dummy_write
	SPI_MODE_DUMMY_READ,//dummy_read
	SPI_MODE_RESERVED,
}spi_tans_mode_e;

typedef enum{
	SPI_MODE_WR_WRITE_ONLY  = 1,//write
	SPI_MODE_WR_DUMMY_WRITE = 8,//dummy_write
}spi_wr_tans_mode_e;

typedef enum{
	SPI_MODE_RD_READ_ONLY  = 2,//must enable CmdEn
	SPI_MODE_RD_DUMMY_READ = 9,//dummy_read
}spi_rd_tans_mode_e;

typedef enum{
	SPI_MODE_WR_RD 		 = 3,//must enable CmdEn
	SPI_MODE_WR_DUMMY_RD = 5,//write_dummy_read
}spi_wr_rd_tans_mode_e;

/**
 * @brief  Define the SPI command for spi slave.
 */
typedef enum{
	SPI_READ_STATUS_SINGLE_CMD  = 0x05,
	SPI_READ_STATUS_DUAL_CMD 	= 0x15,
	SPI_READ_STATUS_QUAD_CMD  	= 0x25,
	SPI_READ_DATA_SINGLE_CMD    = 0x0B,
	SPI_READ_DATA_DUAL_CMD 		= 0x0C,
	SPI_READ_DATA_QUAD_CMD 		= 0x0E,
	SPI_WRITE_DATA_SINGLE_CMD   = 0x51,
	SPI_WRITE_DATA_DUAL_CMD 	= 0x52,
	SPI_WRITE_DATA_QUAD_CMD 	= 0x54,
}spi_cmd_e;

/**
 * @brief  Define the SPI command for b91m spi slave.
 */
typedef enum{
	SPI_CMD_RESERVED            	=BIT(0),
	SPI_CMD_TRANS_HWORD         	=BIT(1),//1  hword transfer
	SPI_CMD_TRANS_WORD	         	=BIT(2),//1  word transfer
	SPI_CMD_RD_DUMMY_4CYCLE	 		=BIT(3),// 0 8cycle 1 4cycle
	SPI_CMD_ADDR_AUTO_INCREASE  	=BIT(4),// 0 AUTO incease
	SPI_CMD_DATA_DUAL	         	=BIT(5),// 0 Single 1 DuaL
	SPI_CMD_ADDR_DUAL           	=BIT(6),// 0 Single 1 DuaL
	SPI_CMD_RD_EN	             	=BIT(7),// 0  write 1 read
}spi_slave_cmd_e;

typedef enum{
	SPI_SLAVE_WRITE_DATA_CMD = 0x00,
	SPI_SLAVE_WRITE_DATA_DUAL_CMD = FLD_SPI_CMD_DATA_DUAL,
	SPI_SLAVE_WRITE_ADDR_DUAL_CMD = FLD_SPI_CMD_ADDR_DUAL,

	SPI_SLAVE_WRITE_DATA_DUAL_4CYC_CMD = FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
	SPI_SLAVE_WRITE_ADDR_DUAL_4CYC_CMD = FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
	SPI_SLAVE_WRITE_DATA_AND_ADDR_DUL_4CYC_CMD = FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
}spi_slave_write_cmd_e;

typedef enum{
	SPI_SLAVE_READ_DATA_CMD = FLD_SPI_CMD_RD_EN,
	SPI_SLAVE_READ_DATA_DUAL_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_DATA_DUAL,
	SPI_SLAVE_READ_ADDR_DUAL_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_ADDR_DUAL,

	SPI_SLAVE_READ_DATA_DUAL_4CYC_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
	SPI_SLAVE_READ_ADDR_DUAL_4CYC_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,

	SPI_SLAVE_READ_DATA_AND_ADDR_DUL_4CYC_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,

}spi_slave_read_cmd_e;


typedef enum{
	SPI_RXFIFO_OR_INT_EN        =BIT(0),
	SPI_TXFIFO_UR_INT_EN        =BIT(1),
	SPI_RXFIFO_INT_EN 	        =BIT(2),
	SPI_TXFIFO_INT_EN		    =BIT(3),
	SPI_END_INT_EN              =BIT(4),
	SPI_SLV_CMD_EN	            =BIT(5),
}spi_irq_mask;

typedef enum{
	SPI_RXF_OR_INT	    =BIT(2),
	SPI_TXF_UR_INT	    =BIT(3),
	SPI_RXF_INT         =BIT(4),
	SPI_TXF_INT 	    =BIT(5),
	SPI_END_INT         =BIT(6),
    SPI_SLV_CMD_INT	    =BIT(7),
}spi_irq_status_e;

typedef enum{
	SPI_BURST_WORD_SIZE ,
	SPI_BURST_DWORD_SIZE,
	SPI_BURST_QWORD_SIZE,
} spi_burst_size_e;

/**
 * @brief Define panel 2data_lane_mode
 */
typedef enum{
	SPI_2DATA_LANE_CLOSE   = 0x00,
	SPI_2DATA_LANE_RGB565  = 0x01,
	SPI_2DATA_LANE_RGB666  = 0x03,
	SPI_2DATA_LANE_RGB888  = 0x07,
}spi_panel_2data_lane_mode_e;

/**
 * @brief 		This function set spi command content and enable translate process.
 * @param[in] 	cmd 		- command content .
 * @return 		none
 */
static inline void spi_master_set_cmd( unsigned char cmd)
{
	reg_spi_trans1 = cmd;
}

/**
 * @brief 		This function servers to read cmd transferred from master for slave.
 * @return  	cmd transferred from by master.
 */
static inline unsigned char spi_slave_get_cmd(void)
{
	return reg_spi_trans1;
}

/**
 * @brief 		This function servers to enable cmd which will sent a byte cmd.
 * @return 		none
 */
static inline void spi_cmd_en(void)
{
	BM_SET(	reg_spi_mode2, FLD_SPI_CMD_EN);
}

/**
 * @brief 		This function servers to disable cmd which will not sent cmd.
 * @return 		none
 */
static inline void spi_cmd_dis(void)
{
	BM_CLR(reg_spi_mode2, FLD_SPI_CMD_EN);
}

/**
 * @brief 	This function servers enable cmd format,the format of cmd phase is the same as the data phase(Dual/Quad).
 * @return 	none
 */
static inline void spi_cmd_fmt_en(void)
{
	BM_SET(	reg_spi_mode2, FLD_SPI_CMD_FMT);
}

/**
 * @brief 	This function servers disable cmd format,the format of cmd phase is single.
 * @return 	none
 */
static inline void spi_cmd_fmt_dis(void)
{
	BM_CLR(reg_spi_mode2, FLD_SPI_CMD_FMT);
}

/**
 * @brief 	This function enable address phase.
 * @return 	none
 */
static inline void spi_addr_en(void)
{
	BM_SET(reg_spi_addr_ctrl, FLD_SPI_ADDR_EN);
}

/**
 * @brief 	This function disable address phase.
 * @return 	none
 */
static inline void spi_addr_dis(void)
{
	BM_CLR(reg_spi_addr_ctrl, FLD_SPI_ADDR_EN);
}

/**
 * @brief 	This function to enable address format.the format of addr phase is the same as the data phase(Dual/Quad).
 * @return 	none
 */
static inline void spi_addr_fmt_en(void)
{
	BM_SET(reg_spi_addr_ctrl, FLD_SPI_ADDR_FMT);

}

/**
 * @brief 	This function to disable address format.
 * @return 	none
 */
static inline void spi_addr_fmt_dis(void)
{
	BM_CLR(	reg_spi_addr_ctrl, FLD_SPI_ADDR_FMT);

}

/**
 * @brief 		This function servers to set spi address length.
 * @param[in] 	len - byte number of address.
 * @return 		none
 */
static inline void spi_set_addr_len( unsigned char len)
{
	reg_spi_addr_ctrl = ((reg_spi_addr_ctrl & (~FLD_SPI_ADDR_LEN)) | (((len - 1) & 0x3) << 2));
}

/**
 * @brief 	This function servers to enable spi quad mode.
 * @return 	none
 */
static inline void spi_quad_mode_en(void)
{
	BM_SET(reg_spi_mode2, FLD_SPI_QUAD);
}

/**
 * @brief 		This function servers to disable spi quad mode.
 * @return 		none
 */
static inline void spi_quad_mode_dis(void)
{
	BM_CLR(reg_spi_mode2, FLD_SPI_QUAD);
}

/**
 * @brief 		This function servers to enable spi dual mode.
 * @return 		none
 */
static inline void spi_dual_mode_en(void)
{
	BM_SET(reg_spi_mode0, FLD_SPI_DUAL);
}

/**
 * @brief 		This function servers to disable spi dual mode.
 * @return 		none
 */
static inline void spi_dual_mode_dis(void)
{
	BM_CLR(reg_spi_mode0, FLD_SPI_DUAL);
}
/**
 * @brief 		This function servers to enable spi 3line mode.
 * @return 		none
 */
static inline void spi_3line_mode_en(void)
{
	BM_SET(reg_spi_mode0, FLD_SPI_3LINE);
}

/**
 * @brief 		This function servers to disable spi 3line mode..
 * @return 		none
 */
static inline void spi_3line_mode_dis(void)
{
	BM_CLR(reg_spi_mode0, FLD_SPI_3LINE);
}

/**
 * @brief 		This function clear tx_fifo.
 * @return 		none
 */
static inline void spi_tx_fifo_clr(void)
{
	BM_SET(reg_spi_fifo_state, FLD_SPI_TXF_CLR);
}

/**
 * @brief 		This function clear rx_fifo.
 * @return 		none
 */
static inline void spi_rx_fifo_clr(void)
{
	BM_SET(reg_spi_fifo_state, FLD_SPI_RXF_CLR);
}

/**
 * @brief 		This function set SPI rx_cnt.
 * @param[in] 	cnt  - rx amount of data.
 * @return 		none
 */
static inline void spi_rx_cnt(unsigned int cnt)
{
	reg_spi_rx_cnt2 = ((cnt - 1) >> 16) & 0xff;
	reg_spi_rx_cnt1 = ((cnt - 1) >> 8) & 0xff;
	reg_spi_rx_cnt0 = (cnt - 1) & 0xff;
}

/**
 * @brief 		This function set SPI tx_cnt.
 * @param[in] 	cnt  - tx amount of data.
 * @return 		none
 */
static inline void spi_tx_cnt(unsigned int cnt)
{
	reg_spi_tx_cnt2 = ((cnt - 1) >> 16) & 0xff;
	reg_spi_tx_cnt1 = ((cnt - 1) >> 8) & 0xff;
    reg_spi_tx_cnt0 = (cnt - 1) & 0xff;

}
/**
 * @brief 		This function to determine whether the bus is busy.
 * @return   	1:Indicates that the bus is busy. 0:Indicates that the bus is free.
 */
static inline _Bool spi_is_busy(void)
{
	return  reg_spi_status & FLD_SPI_BUSY;

}

/**
 * @brief     	This function servers to set slave address spi only.
 * @param[in] 	addr - address of slave.
 * @return    	none
 */
static inline void spi_set_address(unsigned int addr)
{
	reg_spi_addr_32 = addr;
}


/**
 * @brief     	This function servers to set fetch flash address by spi .
 * @param[in] 	addr - flash address,only 3 bytes
 * @return    	none
 */
static inline void spi_set_fetch_flash_address(unsigned int addr)
{
	reg_spi_fetch_flash_addr0 = addr&0xff;
	reg_spi_fetch_flash_addr1_2 = (addr>>8)&0xffff;
}

/**
 * @brief 		This function servers to set irq mask.
 * @param[in] 	mask 	- the irq mask.
 * @return  	cmd 	- transferred by master.
 */
static inline void spi_set_irq_mask(spi_irq_mask mask)
{
   BM_SET(reg_spi_trans2, mask);
}

/**
 * @brief 		This function servers to clear irq mask.
 * @param[in] 	mask 	- the irq mask.
 * @return  	cmd 	- transferred by master.
 */
static inline void spi_clr_irq_mask( spi_irq_mask mask)
{
	BM_CLR(reg_spi_trans2, mask);
}

/**
 * @brief 		This function servers to get irq status.
 * @param[in] 	status 	- the irq status.
 * @return    - the value of status is be set.
 */
static inline unsigned char spi_get_irq_status(spi_irq_status_e status )
{
	return reg_spi_irq_state&status;
}

/**
 * @brief 		This function servers to clear irq status.
 * @param[in] 	status 	- the irq status.
 * @return  	none.
 */
static inline void spi_clr_irq_status(spi_irq_status_e status)
{
	reg_spi_irq_state = status;
}


/**
 * @brief 		This function get tx_fifo number.
 * @return 		the number that wait to be sent.
 */
static inline  unsigned char spi_get_txfifo_num(void)
{
   return reg_spi_txfifo_num & FLD_SPI_TXF_NUM;
}

/**
 * @brief 		This function get rx_fifo number.
 * @return 		the number that have been received.
 */
static inline unsigned char spi_get_rxfifo_num(void)
{
   return reg_spi_rxfifo_num& FLD_SPI_RXF_NUM;
}

/**
 * @brief 		This function enable tx dma.
 * @return 		none
 */
static inline void spi_tx_dma_en(void)
{
	BM_SET(reg_dma_chn_en_h,FLD_DMA_CHN_SPI_TX);
	reg_dma_rdy0_h |= FLD_DMA_READY_9;
	BM_SET(reg_spi_trans2, FLD_SPI_TX_DMA_EN);
}


/**
 * @brief 		This function disable tx dma.
 * @return 		none
 */
static inline void spi_tx_dma_dis(void)
{
	BM_CLR(reg_dma_chn_en_h,FLD_DMA_CHN_SPI_TX);
	BM_CLR(reg_spi_trans2, FLD_SPI_TX_DMA_EN);
}

/**
 * @brief 		This function enable rx dma.
 * @return 		none
 */
static inline void spi_rx_dma_en(void)
{
	BM_SET(reg_dma_chn_en_h,FLD_DMA_CHN_SPI_RX);
	BM_SET(reg_spi_trans2, FLD_SPI_RX_DMA_EN);
}

/**
 * @brief 		This function enable rx dma.
 * @return 		none
 */
static inline void spi_rx_dma_dis(void)
{
	BM_CLR(reg_dma_chn_en_h,FLD_DMA_CHN_SPI_RX);
	BM_CLR(reg_spi_trans2, FLD_SPI_RX_DMA_EN);
}
/**
 * @brief 		This function servers set dma burst_size by dma.
 * @param[in] 	burst_size 	- 0-word ,1- 2 words,2-4words
 * @return 		none
 */
static inline void spi_set_dma_burst_size(spi_burst_size_e burst_size)
{
	reg_dma9_burst_size=burst_size<<2;
}

/**
 * @brief 		This function servers set dma burst_size by spi.
 * @param[in] 	burst_size 	- 0-word ,1- 2 words,2-4words
 * @return 		none
 */
static inline void spi_set_burst_size(spi_burst_size_e burst_size)
{
	reg_spi_tx_burst=burst_size;
}

/**
 * @brief     	This function servers to set spi slave dummy cycle cnt.
 * @param[in] 	dummy_cnt 	- the cnt of dummy clock.
 * @return  	none
 */
static inline void spi_set_slave_dummy_cnt(unsigned char dummy_cnt)
{
	reg_spi_tx_burst = ((reg_spi_tx_burst & (~FLD_SPI_SLV_DUMMY_NUM)) | ((dummy_cnt - 1)<<2));
}

/**
 * @brief      	The function of this API is to set the number of bytes to triggered the receive and transmit interrupt.
 *            	Its default value is 4. recommend setting 4.
 * @param[in]  	cnt 	- the interrupt trigger level.
 * @return   	none
 */
static inline void spi_rtx_irq_trig_cnt( unsigned char cnt)
{
	reg_spi_status = ((reg_spi_status & (~FLD_SPI_TXFIFO_THRES)) | (cnt&FLD_SPI_TXFIFO_THRES));
}

/**
 * @brief 		This function enable 3line_dcx module which is use for panel(lcd oled..).
 * @return 		none
 */
static inline void spi_3line_dcx_en(void)
{
	BM_SET(reg_spi_panel_ctrl, FLD_SPI_PANEL_3LINE_DCX_EN);
}

/**
 * @brief 		This function set 3line_dcx translate command.
 * @return 		none
 */
static inline void spi_set_3line_dcx_cmd(void)
{
	BM_CLR(reg_spi_panel_ctrl, FLD_SPI_PANEL_3LINE_DCX);
}

/**
 * @brief 		This function set 3line_dcx translate data.
 * @return 		none
 */
static inline void spi_set_3line_dcx_data(void)
{
	BM_SET(reg_spi_panel_ctrl, FLD_SPI_PANEL_3LINE_DCX);
}

/**
 * @brief 		This function set 2data_lane mode for panel.
 * @return 		none
 */
static inline void spi_set_panel_2data_lane_mode(spi_panel_2data_lane_mode_e mode)
{
	reg_spi_panel_ctrl = ((reg_spi_panel_ctrl & (~FLD_SPI_PANEL_2DATA_LANE)) | ((mode & 0xf) << 2));
}

/**
 * @brief 		This function set rgb endian en. for example rgb888: RED 0x0000FF(default)  rgb888: RED 0xFF0000
 * @return 		none
 */
static inline void spi_set_rgb_endian_en(void)
{
	BM_SET(reg_spi_panel_ctrl, FLD_SPI_ENDIAN_MODE);
}

/**
 * @brief 		This function set rgb endian dis . for example rgb888: RED 0x0000FF(default)  rgb888: RED 0xFF0000
 * @return 		none
 */
static inline void spi_set_rgb_endian_dis(void)
{
	BM_CLR(reg_spi_panel_ctrl, FLD_SPI_ENDIAN_MODE);
}

/**
 * @brief This function reset SPI module.
 * @param[in] none
 * @return none
 */
static inline void reset_spi_module(void)
{
	reg_rst0 |= FLD_RST0_SPI;
	reg_rst0 &= (~FLD_RST0_SPI);
}

/**
 * @brief     	This function servers to set spi transfer mode.
 * @param[in] 	mode 	- transfer mode.
 * @return  	none
 */
void spi_set_transmode( spi_tans_mode_e mode);

/**
 * @brief 		This function servers to get spi slave ready status. When slave is ready, slave ready reply a byte data:0x5a.
 * slave reply a byte data: 0x00.indicating that slave is not ready for data transmission.
 * @param[in] 	mode	 	- the spi master io mode.
 * @return 		1:Indicates that the slave is ready. 0:Indicates that slave is not ready.
 */
_Bool spi_master_get_slave_ready(spi_io_mode_e mode);


/**
 * @brief     	This function servers configures spi pin.
 * @param[in] 	config - the pointer of pin config struct.
 * @return 		none
 */
void spi_set_pin(spi_pin_config_t *spi_pin_config);

/**
 * @brief     	This function disable spi csn pin.
 * @param[in] 	pin - the csn pin.
 * @return 		none
 */
void spi_cs_pin_dis(GPIO_PinTypeDef pin);

/**
 * @brief     	This function change hspi csn pin.
 * @param[in] 	current_csn_pin - the current csn pin.
 * @param[in] 	next_csn_pin    - the next csn pin.
 * @return 		none.
 */
void spi_change_csn_pin(GPIO_PinTypeDef current_csn_pin,GPIO_PinTypeDef next_csn_pin);

/**
 * @brief     	This function servers to set normal mode.
 * @return  	none
 */
void spi_set_normal_mode(void);

/**
 * @brief     	This function servers to set dual mode.
 * @return  	none
 */
void spi_set_dual_mode(void);

/**
 * @brief	This function servers to set quad mode.
 * @return	none
 */
void spi_set_quad_mode(void);

/**
 * @brief     	This function servers to set 3line mode.
 * @return  	none
 */
void spi_set_3line_mode(void);

/**
 * @brief     	This function servers to set spi io  mode.
 * @param[in]	mode 	- single/dual/quad /3line.
 * @return  	none
  */
void spi_set_io_mode( spi_io_mode_e mode);

/**
 * @brief     	This function servers to set dummy cycle cnt.
 * @param[in] 	dummy_cnt 	- the cnt of dummy clock.
 * @return  	none
 */
void spi_set_dummy_cnt( unsigned char dummy_cnt);

/**
 * @brief     	This function servers to config normal mode.
 * @param[in] 	mode 	- nomal ,mode 3line.
 * @return  	none
 */
void spi_master_config( spi_nomal_3line_mode_e mode);


/**
 * @brief     	This function servers to config spi special mode.
 * @param[in] 	config 	- the pointer of pin special config struct.
 * @return  	none
 */
void spi_master_config_plus(spi_config_t *config);

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */



extern void spi_masterCSpin_select(GPIO_PinTypeDef CSPin);

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
void spi_master_init(unsigned char div_clock, spi_mode_type_e mode);

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
void spi_slave_init(spi_mode_type_e mode);
/**
 * @brief     	This function servers to write spi fifo.
 * @param[in] 	data 	- the pointer to the data for write.
 * @param[in] 	len 	- write length.
 * @return    	none
 */
_attribute_ram_code_sec_ void spi_write( unsigned char *data, unsigned int len);

/**
 * @brief     	This function servers to write spi fifo by word.
 * @param[in] 	data 	- the pointer(unsigned int*) to the data for write.
 * @return    	none
 */
_attribute_ram_code_sec_ void spi_write_word( unsigned int *data);
/**
 * @brief     	This function servers to read spi fifo.
 * @param[in] 	data 	- the pointer to the data for read.
 * @param[in] 	len 	- write length.
 * @return    	none
 */
_attribute_ram_code_sec_ void spi_read( unsigned char *data, unsigned int len);

/**
 * @brief     	This function servers to read spi fifo.
 * @param[in] 	data 	- the pointer(unsigned int*) to the data for read.
 * @return    	none
 */
_attribute_ram_code_sec_ void spi_read_word( unsigned int *data);

/**
 * @brief     	This function serves to normal write data in normal.
 * @param[in] 	data 	- the pointer to the data for write.
 * @param[in] 	len 	- write length.
 * @return  	none
 */
void spi_master_write( unsigned char *data, unsigned int len);


/**
 * @brief     	This function serves to normal write and read data.
 * @param[in] 	wr_data - the pointer to the data for write.
 * @param[in] 	wr_len 	- write length.
 * @param[in] 	rd_data - the pointer to the data for read.
 * @param[in] 	rd_len 	- read length.
 * @return  	none
 */
void spi_master_write_read( unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len);

/**
 * @brief     	This function serves to single/dual/quad write to the SPI slave.
 * @param[in] 	cmd 		- cmd one byte will first write.
 * @param[in] 	addr 		- the address of slave.
 * @param[in] 	data 		-  pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @param[in] 	wr_mode 	- write mode.dummy or not dummy.
 * @return  	none
 */
void spi_master_write_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode);


/**
 * @brief     	This function serves to single/dual/quad write (fetch data from flash )to the SPI slave.
 * @param[in] 	addr 		- the address of slave.
 * @param[in] 	cmd 		- cmd one byte will first write.
 * @param[in] 	data 		-  pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @param[in] 	wr_mode 	- write mode.dummy or not dummy.
 * @return  	none
 */
void spi_master_write_plus_fetch_flash( unsigned char cmd,unsigned int addr,unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode);


/**
 * @brief     	This function serves to single/dual/quad  read from the SPI slave.
 * @param[in]  	cmd 		- cmd one byte will first write.
 * @param[in]  	addr 		- the address of slave.
 * @param[in]  	data 		- pointer to the data need to read.
 * @param[in]  	data_len 	- the length of data.
 * @param[in]  	rd_mode 	- read mode.dummy or not dummy.
 * @return   	none
 */
void spi_master_read_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e rd_mode);

/**
 * @brief       This function serves to set dma tx burst.
 * @param[in]  	burst_size  0-word ,1- 2 words,2-4words
 * @return   	none
 */
void spi_set_dma_tx_burst( spi_burst_size_e  burst_size );

/**
 * @brief     	This function serves to normal write data by dma.
 * @param[in] 	data 	   - the pointer to the data for write.
 * @param[in] 	data_len   - write length.
 * @return  	none
 */
void spi_master_write_dma( unsigned char *data, unsigned int data_len);

/**
 * @brief     	This function serves to normal write cmd and address, then read data by dma.
 * @param[in] 	tx_data     - the pointer to the cmd and address for write.
 * @param[in] 	tx_len 	    - write length.
 * @param[in] 	rx_data 	- the pointer to the data for read.
 * @param[in] 	rx_len 	    - read length.
 * @return  	none
 */
void spi_master_write_read_dma( unsigned char *tx_data, unsigned int tx_len, unsigned char *rx_data, unsigned int rx_len);

/**
 * @brief     	This function serves to write and read data simultaneously by dma.
 * @param[in] 	tx_data     - the pointer to the data for write.
 * @param[in] 	rx_data 	- the pointer to the data for read.
 * @param[in] 	len 	    - data length.
 * @return  	none
 */
void spi_master_write_read_dma_full_duplex(unsigned char* tx_data,unsigned char* rx_data,unsigned int len);

/**
 * @brief      	This function serves to single/dual/quad  write to the SPI slave by dma.
 * @param[in]  	cmd 		- cmd one byte will first write.
 * @param[in]  	addr 		- the address of slave.
 * @param[in]  	data 	- pointer to the data need to write.
 * @param[in]  	data_len 	- length in byte of the data need to write.
 * @param[in]  	wr_mode 	- write mode.dummy or not dummy.
 * @return   	none
 */
void spi_master_write_dma_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode);

/**
 * @brief      	This function serves to single/dual/quad  read from the SPI slave by dma.
 * @param[in]  	cmd 		- cmd one byte will first write.
 * @param[in]  	addr 		- the address of slave.
 * @param[in]  	data 	    - pointer to the buffer that will cache the reading out data.
 * @param[in]  	data_len    - length in byte of the data need to read.
 * @param[in]  	rd_mode 	- read mode.dummy or not dummy.
 * @return   	none
 */
void spi_master_read_dma_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e rd_mode);

/**
 * @brief   	this  function set spi rx dma channel.
 * @param[in]  	rx_data 	- the address of destination.
 * */
void spi_set_rx_dma( unsigned char* rx_data);

/**
 * @brief   	this  function set spi tx dma channel.
 * @param[in]  	tx_data 	- the address of source.
 * @param[in]  	len 		- the length of data.
 * */
void spi_set_tx_dma(unsigned char* tx_data);


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
void spi_master_write_read_full_duplex(unsigned char *write_data, unsigned char *read_data, unsigned int len);

#endif


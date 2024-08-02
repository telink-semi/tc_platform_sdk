/********************************************************************************************************
 * @file    register.h
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

#include "bsp.h"


/********************************************************************************************
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 *****|								Digital  Register Table  						   |*****
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 ********************************************************************************************/

/*******************************      i2c registers: 0x00      ******************************/
#define reg_i2c_set				REG_ADDR32(0x00)
#define reg_i2c_speed			REG_ADDR8(0x00)
#define reg_i2c_id				REG_ADDR8(0x01)
enum{
	FLD_I2C_WRITE_READ_BIT  =  BIT(0),
	FLD_I2C_ID              =  BIT_RNG(1,7),
};

#define reg_i2c_status			REG_ADDR8(0x02)
enum{
	FLD_I2C_CMD_BUSY		= 	BIT(0),
	FLD_I2C_BUS_BUSY		= 	BIT(1),
	FLD_I2C_NAK				= 	BIT(2),
};

#define reg_i2c_mode			REG_ADDR8(0x03)
enum{
	FLD_I2C_ADDR_AUTO_ADD 		=	BIT(0),
	FLD_I2C_MASTER_EN			= 	BIT(1),      // 1: master, 0: slave
	FLD_I2C_SLAVE_MAPPING 		=	BIT(2),      // write i2c data to predefined memory address which set by other register
	FLD_I2C_HOLD_MASTER     	=   BIT(3),
};

#define reg_i2c_adr_dat			REG_ADDR16(0x04)
#define reg_i2c_dat_ctrl		REG_ADDR32(0x04)
#define reg_i2c_di_ctrl			REG_ADDR16(0x06)
#define reg_i2c_adr				REG_ADDR8(0x04)
#define reg_i2c_do				REG_ADDR8(0x05)
#define reg_i2c_di				REG_ADDR8(0x06)
#define reg_i2c_ctrl			REG_ADDR8(0x07)
enum{
	FLD_I2C_CMD_ID			= 		BIT(0),
	FLD_I2C_CMD_ADDR		= 		BIT(1),
	FLD_I2C_CMD_DO			= 		BIT(2),
	FLD_I2C_CMD_DI			= 		BIT(3),
	FLD_I2C_CMD_START		= 		BIT(4),
	FLD_I2C_CMD_STOP		= 		BIT(5),
	FLD_I2C_CMD_READ_ID		= 		BIT(6),
	FLD_I2C_CMD_ACK			= 		BIT(7),
};
/*******************************  i2c address map registers: 0xe0      ******************************/
#define reg_i2c_map_hadr		 	 REG_ADDR8(0xe0)

#define reg_i2c_slave_map_addrl      REG_ADDR8(0xe1)
#define reg_i2c_slave_map_addrm      REG_ADDR8(0xe2)
#define reg_i2c_slave_map_addrh      REG_ADDR8(0xe3)

#define reg_i2c_slave_irq_status 	 REG_ADDR8(0xe4)
#define reg_spi_slave_irq_status 	 REG_ADDR8(0xe4)
#define reg_i2c_map_host_status      REG_ADDR8(0xe4)
enum{
	FLD_HOST_CMD_IRQ  = 	BIT(0),  FLD_SLAVE_SPI_IRQ = FLD_HOST_CMD_IRQ,  //both host write & read trigger this status
	FLD_HOST_READ_IRQ = 	BIT(1),                                        //only host read trigger this status
};

#define reg_i2c_map_read0		     REG_ADDR8(0xe5)
#define reg_i2c_map_read1		     REG_ADDR8(0xe6)
#define reg_i2c_map_read2		     REG_ADDR8(0xe7)

/*******************************      spi registers: 0x08      ******************************/
#define    SPI_BASE_ADDR			        0x20
/**
 * BIT[0:1] the minimum time between the edge of SPI_CS and  the edges of SPI_CLK.the actual duration is (SPI_CLK_OUT/2)*(cs2sclk+1).master only
 * BIT[2]  set 3line mode ,MOSI is bi-directional signal in regular mode.master only
 * BIT[3]  transfer data with least significant bit first.1: LSB  0: MSB default. master/slave
 * BIT[4]  set dual io mode.master only
 * BIT[5:6] set spi 4 mode. master/slave
 *  *  *         bit5: CPHA-SPI_CLK Phase,bit6: CPOL-SPI_CLK Polarity
 *            MODE0:  CPOL = 0, CPHA =0;
 *            MODE1:  CPOL = 0, CPHA =1;
 *            MODE2:  CPOL = 1, CPHA =0;
 *            MODE3:  CPOL = 1, CPHA =1;
 * BIT[7]  set master/slave mode. 0 slave 1 master default.master/slave
 */

#define reg_spi_mode0			REG_ADDR8(SPI_BASE_ADDR)
enum{
	FLD_SPI_CS2SCLK 	        = BIT_RNG(0,1),
	FLD_SPI_3LINE 	     	    = BIT(2),
	FLD_SPI_LSB 		        = BIT(3),
	FLD_SPI_DUAL			    = BIT(4),
    FLD_SPI_MODE_WORK_MODE      = BIT_RNG(5,6),
	FLD_SPI_MASTER_MODE      	= BIT(7),
};
/*the clock freq ratio between the source_clock and spi_clock.master only
 * spi_clock=source_clock/((spi_clk_div+1)*2)
 * spi_clk_div=reg_hspi_mode1[7:0]. max_value=0xff,spi_clock==source_clock
 */
#define reg_spi_div			REG_ADDR8(SPI_BASE_ADDR+0x01)

/**
 * BIT[0]  set cmd format 0: single mode  1: the format of the cmd phase is the same as the data phase(Dual/Quad).master only
 * BIT[1]  set spi quad I/O mode. master only
 * BIT[2]  set the spi command phase enable.master only
 * BIT[4:7]   the minimum time that SPI CS should stay HIGH.the actual duration is (SPI_CLK period_out / 2)*(csht+1).default=2,master only
 */
#define reg_spi_mode2			REG_ADDR8(SPI_BASE_ADDR+0x02)
enum{
	FLD_SPI_CMD_FMT 	         =BIT(0),
	FLD_SPI_QUAD 	             =BIT(1),
	FLD_SPI_CMD_EN 	       		 =BIT(2),
	FLD_SPI_CSHT                 =BIT_RNG(4,7),
};

/**
 * BIT[0:3]  dummy data cnt, dummy is always single wire mode, dummy number = dummy_cnt + 1.master only
 * BIT[4:7]  the transfer mode.master only
 * the transfer sequence could be:
 * 0x0:write and read at the same time(must enable CmdEn)
 * 0x1:write only
 * 0x2:read only(must enable CmdEn)
 * 0x3:write,read
 * 0x4:read,write
 * 0x5:write,dummy,read
 * 0x6:read,dummy,write(must enable CmdEn)
 * 0x7:None Data(must enable CmdEn)
 * 0x8:Dummy,write
 * 0x9:Dummy,read
 * 0xa~0xf:reserved
 */
#define reg_spi_trans0 		REG_ADDR8(SPI_BASE_ADDR+0x03)

enum{
	FLD_SPI_DUMMY_CNT	       = BIT_RNG(0,3),
	FLD_SPI_TRANSMODE 	        =BIT_RNG(4,7),
};

/**
 * BIT[0:7]  SPI Command.master only
 */
#define reg_spi_trans1		REG_ADDR8(SPI_BASE_ADDR+0x04)
enum{
	FLD_SPI_CMD_RESERVED            =BIT(0),
	FLD_SPI_CMD_TRANS_HWORD         =BIT(1),//1 apb hword transfer
	FLD_SPI_CMD_TRANS_WORD	        =BIT(2),//1 apb word transfer
	FLD_SPI_CMD_RD_DUMMY_4CYCLE	    =BIT(3),// 0 8cycle 1 4cycle
	FLD_SPI_CMD_ADDR_AUTO_INCREASE  =BIT(4),// 0 AUTO incease
	FLD_SPI_CMD_DATA_DUAL	        =BIT(5),// 0 Single 1 DuaL
	FLD_SPI_CMD_ADDR_DUAL           =BIT(6),// 0 Single 1 DuaL
	FLD_SPI_CMD_RD_EN	            =BIT(7),// 0  write 1 read
};
/**
 * BIT[0] enable the SPI Receive FIFO Overrun interrupt . slave only
 * BIT[1] enable the SPI Transmit FIFO Underrun interrupt. slave only
 * BIT[2] enable the SPI Receive FIFO Threshold interrupt.master/slave
 * BIT[3] enable the SPI Transmit FIFO Threshold interrupt.master/slave
 * BIT[4] enable the SPI Transmit end interrupt.master/slave
 * BIT[5] enable  slvCmdint.The slave command interrupt is triggered each byte command received (starting 8 bit) .slave only
 * BIT[6] enable RX DMA
 * BIT[7] enable TX DMA
 */

#define reg_spi_trans2 		REG_ADDR8(SPI_BASE_ADDR+0x05)
enum{
	FLD_SPI_RXFIFO_OR_INT_EN        =BIT(0),
	FLD_SPI_TXFIFO_UR_INT_EN        =BIT(1),
	FLD_SPI_RXFIFO_INT_EN 	        =BIT(2),
	FLD_SPI_TXFIFO_INT_EN		    =BIT(3),
	FLD_SPI_END_INT_EN              =BIT(4),
	FLD_SPI_SLV_CMD_EN	            =BIT(5),
	FLD_SPI_RX_DMA_EN               =BIT(6),
	FLD_SPI_TX_DMA_EN 	            =BIT(7),
};

/**
 * BIT[0:3]  number of valid entries in the rxfifo.
 */
#define reg_spi_rxfifo_num		REG_ADDR8(SPI_BASE_ADDR+0x06)

enum{
	FLD_SPI_RXF_NUM          = BIT_RNG(0,3),
};
/**
 * BIT[4:7]  number of valid entries in the txfifo.
 */
#define reg_spi_txfifo_num		REG_ADDR8(SPI_BASE_ADDR+0x07)

enum{
	FLD_SPI_TXF_NUM          = BIT_RNG(0,5),
};

/**
 * BIT[0:7]   data0[7:0] to transmit or received.
 */
#define reg_spi_wr_rd_data0 		REG_ADDR8(SPI_BASE_ADDR+0x08)
/**
 * BIT[0:7]   data1[7:0] to transmit or received.
 */

#define reg_spi_wr_rd_data1 		REG_ADDR8(SPI_BASE_ADDR+0x09)
/**
 * BIT[0:7]   data2[7:0] to transmit or received.
 */
#define reg_spi_wr_rd_data2		    REG_ADDR8(SPI_BASE_ADDR+0x0a)

/**
 * BIT[0:7]   data3[7:0] to transmit or received.
 */
#define reg_spi_wr_rd_data3		  	REG_ADDR8(SPI_BASE_ADDR+0x0b)

#define reg_spi_wr_rd_data32 		REG_ADDR32(SPI_BASE_ADDR+0x08)

#define reg_spi_wr_rd_data(i)		REG_ADDR8(SPI_BASE_ADDR+0x08+(i))

/**
 * BIT[0]  clr tx fifo for dma .
 * BIT[1]  clr rx fifo for dma .
 * BIT[2]  rxfifo reset,write 1 to reset. Spi clock must turn on.
 * BIT[3]  txfifo reset,write 1 to reset. Spi clock must turn on
 * BIT[4]  rxfifo full flag.
 * BIT[5]  rxfifo empty flag
 * BIT[6]  txfifo full flag.
 * BIT[7]  txfifo empty flag
 */
#define reg_spi_fifo_state			REG_ADDR8(SPI_BASE_ADDR+0x0c)
enum{
	FLD_SPI_TXFIFO_SOF_CLR 		=BIT(0),
	FLD_SPI_RXFIFO_EOF_CLR 		=BIT(1),
	FLD_SPI_RXF_CLR	      		=BIT(2),
	FLD_SPI_TXF_CLR		        =BIT(3),
	FLD_SPI_RXF_FULL            =BIT(4),
	FLD_SPI_RXF_EMPTY	        =BIT(5),
	FLD_SPI_TXF_FULL            =BIT(6),
    FLD_SPI_TXF_EMPTY	        =BIT(7),
};
/**
 * BIT[2]  RX FIFO Overrun interrupt status. slave only
 * BIT[3]  TX FIFO Underrun interrupt status. slave only
 * BIT[4]  RX FIFO Threshold interrupt status.set 1 to clear. master/slave
 * BIT[5]  TX FIFO Threshold interrupt status.set 1 to clear. master/slave
 * BIT[6]  End of SPI Transfer interrupt status.set 1 to clear.master/slave
 * BIT[7]  Slave Command Interrupt status.set 1 to clear.slave only
 */
#define reg_spi_irq_state		REG_ADDR8(SPI_BASE_ADDR + 0x0d)

enum{
	FLD_SPI_STATE_RESERVED  =BIT_RNG(0,1),
	FLD_SPI_RXF_OR_INT	    =BIT(2),
	FLD_SPI_TXF_UR_INT	    =BIT(3),
	FLD_SPI_RXF_INT         =BIT(4),
	FLD_SPI_TXF_INT 	    =BIT(5),
	FLD_SPI_END_INT         =BIT(6),
    FLD_SPI_SLV_CMD_INT	    =BIT(7),
};

/**
 * BIT[0:4] fifo threshold. 0x4 default.
 * BIT[5]  set this bit to indicate that spi as slave is ready for data transaction
 * BIT[6]  spi soft reset.high valid.
 * BIT[7]  SPI transfer status .1 is busy, 0 not busy.
 */
#define reg_spi_status 		REG_ADDR8(SPI_BASE_ADDR + 0x0e )
enum{
	FLD_SPI_TXFIFO_THRES  		  =BIT_RNG(0,4),
	FLD_SPI_SLAVE_READY	      	  =BIT(5),
	FLD_SPI_SOFT_RESET	          =BIT(6),
	FLD_SPI_BUSY                  =BIT(7),
};

/**
 * BIT[0]  1:enable addr phase, master only
 * BIT[1]  0:single mode  1:the format of addr phase is the same as the data phase(Dual/Quad).master only
 * BIT[2:3] address length.2'b00:1bye  2'b01:2bytes  2'b10:3bytes  2'b11:4bytes.master only
 */
#define reg_spi_addr_ctrl		REG_ADDR8(SPI_BASE_ADDR+0x0f)
enum{
	FLD_SPI_ADDR_EN	       	   = BIT(0),
	FLD_SPI_ADDR_FMT	       = BIT(1),
	FLD_SPI_ADDR_LEN           = BIT_RNG(2,3),
};
/**   spi_addr0~ spi_addr3.
 */
#define reg_spi_addr_32		REG_ADDR32(SPI_BASE_ADDR+0x10)

/**
 * BIT[0:7]  transfer count0 for read data, byte0
 */
#define reg_spi_rx_cnt0		  	REG_ADDR8(SPI_BASE_ADDR+0x14)

/**
 * BIT[0:7]  transfer count1 for read data, byte1
 */
#define reg_spi_rx_cnt1 		REG_ADDR8(SPI_BASE_ADDR+0x15)
/**
 * BIT[0:7]  transfer count2 for read data, byte2
 */
#define reg_spi_rx_cnt2			REG_ADDR8(SPI_BASE_ADDR+0x16)

/**
 * BIT[0]    1:enable spi 3line dcx (data/command selection), master only,for spi panel(LCD OLED ...)
 * BIT[1]    spi 3line dcx (data/command selection). 0:command 1:data
 * BIT[2:4]  2data_lane mode.3'b000:2data_lane close  3'b001:RGB565  3'b011:RGB666  3'b011:RGB888.
 * BIT[5]    0:RGB byte is from low address to high address
 * BIT[6]    fetch from flash directly
 */
#define reg_spi_panel_ctrl		REG_ADDR8(SPI_BASE_ADDR+0x17)
enum{
	FLD_SPI_PANEL_3LINE_DCX_EN     = BIT(0),
	FLD_SPI_PANEL_3LINE_DCX	   	   = BIT(1),
	FLD_SPI_PANEL_2DATA_LANE       = BIT_RNG(2,4),
	FLD_SPI_ENDIAN_MODE	   	   	   = BIT(5),
	FLD_SPI_FETCH_FLASH	   	   	   = BIT(6),
};

/**
 * BIT[0:7]   transfer count0 for write data.master only
 */
#define reg_spi_tx_cnt0		      REG_ADDR8(SPI_BASE_ADDR+0x18)

/**
 * BIT[0:7]   transfer count1 for write data.master only
 */
#define reg_spi_tx_cnt1 		  REG_ADDR8(SPI_BASE_ADDR+0x19)
/**
 * BIT[0:7]   transfer count2 for write data.master only
 */
#define reg_spi_tx_cnt2 		  REG_ADDR8(SPI_BASE_ADDR+0x1a)

#define reg_spi_tx_burst 		  REG_ADDR8(SPI_BASE_ADDR+0x1b)
enum{
	FLD_SPI_TX_BURST              = BIT_RNG(0,1),
	FLD_SPI_SLV_DUMMY_NUM       = BIT_RNG(2,6),
};

#define reg_spi_rxfifo_thr 		  REG_ADDR8(SPI_BASE_ADDR+0x1c)
enum{
	FLD_SPI_RXFIFO_THRESHOLD              = BIT_RNG(0,2),
	FLD_SPI_AUTO_CLEAR                    = BIT(3),
};

#define reg_spi_fetch_flash_addr0 		  REG_ADDR8(SPI_BASE_ADDR+0x1d)
#define reg_spi_fetch_flash_addr1 		  REG_ADDR8(SPI_BASE_ADDR+0x1e)
#define reg_spi_fetch_flash_addr2 		  REG_ADDR8(SPI_BASE_ADDR+0x1f)
#define reg_spi_fetch_flash_addr1_2 	  REG_ADDR16(SPI_BASE_ADDR+0x1e)

/*******************************      mspi registers: 0x0c      ******************************/

#define reg_mspi_data		REG_ADDR8(0x0c)
#define reg_mspi_ctrl		REG_ADDR8(0x0d)

enum{
	FLD_MSPI_CS 	= 		BIT(0),
	FLD_MSPI_SDO 	= 		BIT(1),
	FLD_MSPI_CONT 	= 		BIT(2),
	FLD_MSPI_RD 	= 		BIT(3),
	FLD_MSPI_BUSY 	= 		BIT(4),
};

#define reg_mspi_mode      		REG_ADDR8(0x0f)
enum
{
	FLD_MSPI_DUAL_DATA_MODE_EN  = BIT(0),
	FLD_MSPI_DUAL_ADDR_MODE_EN  = BIT(1),
	FLD_MSPI_CLK_DIV     	 	= BIT_RNG(2,7),
};
/*******************************      otp registers: 0x10      ******************************/

#define		OTP_BASE_ADDR		0x10
#define		reg_otp_ctrl0		read_reg8(OTP_BASE_ADDR+0x0)
enum{
#if (MCU_CORE_B80)
	FLD_OTP_PCLK_EN             = BIT(0),//clock input
#endif
	FLD_OTP_PCE		            = BIT(1),//IP enabling
	FLD_OTP_PPROG	            = BIT(2),//program mode enabling
	FLD_OTP_PWE		            = BIT(3),//define program cycle
	FLD_OTP_PAS		            = BIT(4),//program redundancy
	FLD_OTP_PTR		            = BIT(5),//test row enable
#if (MCU_CORE_B80B)
    FLD_OTP_AUTO_PROGRAM        = BIT(6), /**< auto program mode 0:disable 1:enable */
#endif
};
#define		reg_otp_ctrl1		read_reg8(OTP_BASE_ADDR+0x01)
enum{
	FLD_OTP_PTM             	= BIT_RNG(0,3),
	FLD_OTP_PLDO		        = BIT(4),//enable LDO to generate 2.5V power
	FLD_OTP_PDSTD	            = BIT(5),//pin for deep standby mode,low active.
};
#define  reg_otp_ctrl2          read_reg8(OTP_BASE_ADDR+0x02)
enum{
	FLD_OTP_TIMEOUT_CONFIG     = BIT_RNG(0,3),
	FLD_OTP_AUTO_PCE_MODE      = BIT(4),
	FLD_OTP_TCS_CONFIG         = BIT_RNG(5,7),
};

#if (MCU_CORE_B80B)
#define reg_otp_ctrl3           read_reg8(OTP_BASE_ADDR+0x03)
enum
{
    FLD_OTP_SAMPLE_POINT       = BIT_RNG(0, 3),
    FLD_OTP_CLK_DIV            = BIT_RNG(4, 6),
    FLD_OTP_PADR_AUTO          = BIT(7),
};
#endif

#define		reg_otp_pa			read_reg16(OTP_BASE_ADDR+0x04)//PA[11:0],address input

#define		reg_otp_paio		read_reg8(OTP_BASE_ADDR+0x06)//PAIO[5:0]address input programming operation
enum{
	FLD_OTP_PAIO             	= BIT_RNG(0,5),
#if (MCU_CORE_B80)
	FLD_OTP_CLK_DIV             = BIT(6),
	FLD_OTP_PADR_AUTO           = BIT(7),
#endif
};

#define		reg_otp_dat			read_reg8(OTP_BASE_ADDR+0x07)//OTP state
enum{
	FLD_OTP_BUSY             	= BIT(0),
	FLD_OTP_READY		        = BIT(1),
#if (MCU_CORE_B80B)
    FLD_OTP_AUTO_EN             = BIT(2),
#endif
};

#define		reg_otp_wr_dat		read_reg32(OTP_BASE_ADDR+0x08)// data input PDIN[31:0]

#define		reg_otp_rd_dat		read_reg32(OTP_BASE_ADDR+0x0c)// data output[31:0]

#if (MCU_CORE_B80B)
#define     reg_otp_timing_cfg0 read_reg8(0x500 + 0xd8)
enum
{
    FLD_OTP_TIMING_CFG0         = BIT_RNG(0, 6),
};

#define     reg_otp_timing_cfg1 read_reg8(0x500 + 0xd9)
enum
{
    FLD_OTP_TIMING_CFG1         = BIT_RNG(0, 4),
};
#endif

/*******************************      reset registers: 0x60      ******************************/

#define reg_rst0				REG_ADDR8(0x60)
enum{
	FLD_RST0_SPI = 				BIT(0),
	FLD_RST0_I2C = 				BIT(1),
	FLD_RST0_UART = 			BIT(2),
	FLD_RST0_USB = 				BIT(3),
	FLD_RST0_PWM = 				BIT(4),
	FLD_RST0_QDEC =				BIT(5),
	FLD_RST0_SWIRE = 			BIT(7),
};

#define reg_rst1				REG_ADDR8(0x61)
enum{
	FLD_RST1_ZB = 				BIT(0),
	FLD_RST1_SYS_TIMER = 		BIT(1),
	FLD_RST1_DMA =				BIT(2),
	FLD_RST1_ALGM = 			BIT(3),
	FLD_RST1_AES = 				BIT(4),
	FLD_RST1_ADC = 				BIT(5),
	FLD_RST1_ALG =				BIT(6),
};

#define reg_rst2				REG_ADDR8(0x62)
enum{
	FLD_RST2_AIF =				BIT(0),
	FLD_RST2_AUD =				BIT(1),
	FLD_RST2_DFIFO =			BIT(2),
	FLD_RST2_RISC =				BIT(4),
	FLD_RST2_MCIC = 			BIT(5),
	FLD_RST2_RISC1 =			BIT(6),
	FLD_RST2_MCIC1 = 			BIT(7),
};

#if(MCU_CORE_B80B)
#define reg_rst3                REG_ADDR8(0x5e)
enum{
	FLD_RST3_UART1  =   BIT(0),
};
#endif

#define reg_clk_en0				REG_ADDR8(0x63)
enum{
	FLD_CLK0_SPI_EN = 			BIT(0),
	FLD_CLK0_I2C_EN = 			BIT(1),
	FLD_CLK0_UART_EN = 		BIT(2),
	FLD_CLK0_USB_EN = 			BIT(3),
	FLD_CLK0_PWM_EN = 			BIT(4),
	FLD_CLK0_QDEC_EN = 			BIT(5),
	FLD_CLK0_SWIRE_EN = 		BIT(7),
};

#define reg_clk_en1				REG_ADDR8(0x64)
enum{
	FLD_CLK1_ZB_EN = 			BIT(0),
	FLD_CLK1_SYS_TIMER_EN = 	BIT(1),
	FLD_CLK1_DMA_EN = 			BIT(2),
	FLD_CLK1_ALGM_EN = 			BIT(3),
	FLD_CLK1_AES_EN = 			BIT(4),
	FLD_CLK1_KS_32K_EN = 		BIT(5),/**< The keyscan scan clock comes from 32K rc or 32K crystal, which is configured by pm */
	FLD_CLK1_KS_EN = 			BIT(6),/**< keyscan_clk = system_clk */

};


#define reg_clk_en2				REG_ADDR8(0x65)
enum{
	FLD_CLK2_AIF_EN = 			BIT(0),
	FLD_CLK2_AUD_EN = 			BIT(1),
	FLD_CLK2_DFIFO_EN = 		BIT(2),
	FLD_CLK2_MC_EN = 			BIT(4),
	FLD_CLK2_MCIC_EN = 			BIT(5),
};

#if (MCU_CORE_B80B)
#define reg_clk_en3             REG_ADDR8(0x5f)
enum{
	FLD_CLK3_UART1_EN       =  BIT(0),
};
#endif

#define reg_clk_sel				REG_ADDR8(0x66)
enum{
	FLD_SCLK_DIV = 				BIT_RNG(0,4),
	FLD_SCLK_SEL =				BIT_RNG(5,6),
	FLD_SCLK_HS_SEL =			BIT(7),
};

#define reg_i2s_step			REG_ADDR8(0x67)
enum{
	FLD_I2S_STEP = 				BIT_RNG(0,6),
	FLD_I2S_CLK_EN =			BIT(7),
};

#define reg_i2s_mod				REG_ADDR8(0x68)

#define reg_codec_clk_step			REG_ADDR8(0x6c)

#define reg_codec_clk_mod 			REG_ADDR8(0x6d)

#define reg_wakeup_en			REG_ADDR8(0x6e)
enum{
	FLD_WAKEUP_SRC_I2C = 		BIT(0),
	FLD_WAKEUP_SRC_SPI =		BIT(1),
	FLD_WAKEUP_SRC_USB =		BIT(2),
	FLD_WAKEUP_SRC_GPIO =		BIT(3),
	FLD_WAKEUP_SRC_I2C_SYN =	BIT(4),
	FLD_WAKEUP_SRC_GPIO_RM =	BIT(5),
	FLD_WAKEUP_SRC_USB_RESM =	BIT(6),
	FLD_WAKEUP_SRC_RST_SYS =	BIT(7),
};

#define reg_pwdn_ctrl			REG_ADDR8(0x6f)
enum
{
	FLD_PWDN_CTRL_REBOOT = BIT(5),
	FLD_PWDN_CTRL_SLEEP  = BIT(7),
};


#define reg_mcu_wakeup_mask		REG_ADDR32(0x78)

#if(MCU_CORE_B80B)
/*******************************      7816 registers: 0x7b      ******************************/
/*******************************      78161 registers: 0x5d      ******************************/
#define reg_7816_clk_div(i)		REG_ADDR8((i==0) ? (0x7b):(0x5d))


/*******************************      uart0 registers: 0x90      ******************************/
/*******************************      uart1 registers: 0xc0      ******************************/

#define reg_uart_data_buf0(i)		REG_ADDR8(0x90+0x30*(i))
#define reg_uart_data_buf1(i)		REG_ADDR8(0x91+0x30*(i))
#define reg_uart_data_buf2(i)		REG_ADDR8(0x92+0x30*(i))
#define reg_uart_data_buf3(i)		REG_ADDR8(0x93+0x30*(i))

#define reg_uart_data_buf(i,j)    REG_ADDR8(0x90 + 0x30*(i)+j)  //i = 0~3
#define reg_uart_clk_div(i)		REG_ADDR16(0x94 +  0x30*(i))
enum{
	FLD_UART_CLK_DIV = 			BIT_RNG(0,14),
	FLD_UART_CLK_DIV_EN = 		BIT(15)
};

#define reg_uart_ctrl0(i)			REG_ADDR8(0x96 + 0x30*(i))
enum{
	FLD_UART_BPWC = 			BIT_RNG(0,3),
	FLD_UART_RX_DMA_EN = 		BIT(4),
	FLD_UART_TX_DMA_EN =		BIT(5),
	FLD_UART_RX_IRQ_EN = 		BIT(6),
	FLD_UART_TX_IRQ_EN =		BIT(7),
};

#define reg_uart_ctrl1(i)         		REG_ADDR8(0x97 +0x30*(i))
enum {
    FLD_UART_CTRL1_CTS_SELECT	   = BIT(0),
    FLD_UART_CTRL1_CTS_EN 		   = BIT(1),
    FLD_UART_CTRL1_PARITY_EN 	   = BIT(2),
    FLD_UART_CTRL1_PARITY_POLARITY = BIT(3),       //1:odd parity   0:even parity
    FLD_UART_CTRL1_STOP_BIT 	   = BIT_RNG(4,5),
    FLD_UART_CTRL1_TTL 			   = BIT(6),
    FLD_UART_CTRL1_LOOPBACK 	   = BIT(7),
};

#define reg_uart_ctrl2(i)			REG_ADDR8(0x98 + 0x30*(i))
enum {
    FLD_UART_CTRL2_RTS_TRIG_LVL   	 = BIT_RNG(0,3),
    FLD_UART_CTRL2_RTS_PARITY 		 = BIT(4),
    FLD_UART_CTRL2_RTS_MANUAL_VAL 	 = BIT(5),
    FLD_UART_CTRL2_RTS_MANUAL_EN 	 = BIT(6),
    FLD_UART_CTRL2_RTS_EN 			 = BIT(7),
};


#define reg_uart_ctrl3(i)        	REG_ADDR8(0x99 + 0x30*(i))
enum {
	FLD_UART_RX_IRQ_TRIG_LEV = BIT_RNG(0,3),
	FLD_UART_TX_IRQ_TRIG_LEV = BIT_RNG(4,7),
};

#define reg_uart_rx_timeout0(i)	REG_ADDR8(0x9a + 0x30*(i))
enum{
	FLD_UART_TIMEOUT_BW		 = 	BIT_RNG(0,7),
};

#define reg_uart_rx_timeout1(i)    REG_ADDR8(0x9b + 0x30*(i))
enum{
	FLD_UART_TIMEOUT_MUL	      =  BIT_RNG(0,1),
	FLD_URAT_RXDONE_RTS_EN        =  BIT(2),
	FLD_URAT_RXTIMEOUT_RTS_EN     =  BIT(3),
	FLD_UART_MASK_RXDONE_IRQ      =  BIT(4),
	FLD_UART_P7816_EN	 	      =  BIT(5),
	FLD_UART_MASK_TXDONE_IRQ      =  BIT(6),
	FLD_UART_MASK_ERR_IRQ 	      =  BIT(7),
};


#define reg_uart_buf_cnt(i)       REG_ADDR8(0x9c + 0x30*(i))

enum{
	FLD_UART_RX_BUF_CNT		=  BIT_RNG(0,3),
	FLD_UART_TX_BUF_CNT		=  BIT_RNG(4,7),
};

#define reg_uart_status0(i)       REG_ADDR8(0x9d + 0x30*(i))
enum{
	FLD_UART_RBCNT 	     	=  BIT_RNG(0,2),
	FLD_UART_IRQ_FLAG    	=  BIT(3),
	FLD_UART_WBCNT 	     	=  BIT_RNG(4,6),
	FLD_URAT_CLEAR_RXDONE_FLAG = BIT(4),
	FLD_UART_CLEAR_RX_FLAG 	=  BIT(6),
	FLD_UART_RX_ERR_FLAG 	=  BIT(7),
};

#define reg_uart_status1(i)       REG_ADDR8(0x9e + 0x30*(i))
enum{
	FLD_UART_TX_DONE   	  =  BIT(0),
	FLD_UART_TX_BUF_IRQ   =  BIT(1),
	FLD_UART_RX_DONE   	  =  BIT(2),
	FLD_UART_RX_BUF_IRQ   =  BIT(3),
	FLD_UART_RXDONE_IRQ   =  BIT(4),
	FLD_UART_TIMEOUT_EN   =  BIT(5),
	FLD_UART_AUTO_RXCLR_EN = BIT(6),
	FLD_UART_NDMA_RXDONE_EN=BIT(7),
};


#define reg_uart_state(i)       REG_ADDR8(0x9f + 0x30*(i))
enum{
	FLD_UART_TSTATE_I 	     =  BIT_RNG(0,2),
	FLD_UART_RSTATE_I	     =  BIT_RNG(4,7),
};
#else

/*******************************      7816 registers: 0x7b      ******************************/

#define reg_7816_clk_div	REG_ADDR8(0x7b)


/*******************************      uart registers: 0x90      ******************************/

#define reg_uart_data_buf0		REG_ADDR8(0x90)
#define reg_uart_data_buf1		REG_ADDR8(0x91)
#define reg_uart_data_buf2		REG_ADDR8(0x92)
#define reg_uart_data_buf3		REG_ADDR8(0x93)

#define reg_uart_data_buf(i)    REG_ADDR8(0x90 + (i))  //i = 0~3
#define reg_uart_clk_div		REG_ADDR16(0x94)
enum{
	FLD_UART_CLK_DIV = 			BIT_RNG(0,14),
	FLD_UART_CLK_DIV_EN = 		BIT(15)
};

#define reg_uart_ctrl0			REG_ADDR8(0x96)
enum{
	FLD_UART_BPWC = 			BIT_RNG(0,3),
	FLD_UART_RX_DMA_EN = 		BIT(4),
	FLD_UART_TX_DMA_EN =		BIT(5),
	FLD_UART_RX_IRQ_EN = 		BIT(6),
	FLD_UART_TX_IRQ_EN =		BIT(7),
};

#define reg_uart_ctrl1         		REG_ADDR8(0x97)
enum {
    FLD_UART_CTRL1_CTS_SELECT	   = BIT(0),
    FLD_UART_CTRL1_CTS_EN 		   = BIT(1),
    FLD_UART_CTRL1_PARITY_EN 	   = BIT(2),
    FLD_UART_CTRL1_PARITY_POLARITY = BIT(3),       //1:odd parity   0:even parity
    FLD_UART_CTRL1_STOP_BIT 	   = BIT_RNG(4,5),
    FLD_UART_CTRL1_TTL 			   = BIT(6),
    FLD_UART_CTRL1_LOOPBACK 	   = BIT(7),
};

#define reg_uart_ctrl2			REG_ADDR16(0x98)
enum {
    FLD_UART_CTRL2_RTS_TRIG_LVL   	 = BIT_RNG(0,3),
    FLD_UART_CTRL2_RTS_PARITY 		 = BIT(4),
    FLD_UART_CTRL2_RTS_MANUAL_VAL 	 = BIT(5),
    FLD_UART_CTRL2_RTS_MANUAL_EN 	 = BIT(6),
    FLD_UART_CTRL2_RTS_EN 			 = BIT(7),
	FLD_UART_CTRL3_RX_IRQ_TRIG_LEVEL = BIT_RNG(8,11),
	FLD_UART_CTRL3_TX_IRQ_TRIG_LEVEL = BIT_RNG(12,15),
};


#define reg_uart_ctrl3        	REG_ADDR8(0x99)
enum {
	FLD_UART_RX_IRQ_TRIG_LEV = BIT_RNG(0,3),
	FLD_UART_TX_IRQ_TRIG_LEV = BIT_RNG(4,7),
};

#define reg_uart_rx_timeout0	REG_ADDR8(0x9a)
enum{
	FLD_UART_TIMEOUT_BW		 = 	BIT_RNG(0,7),
};

#define reg_uart_rx_timeout1    REG_ADDR8(0x9b)
enum{
	FLD_UART_TIMEOUT_MUL	      =  BIT_RNG(0,1),
	FLD_URAT_RXDONE_RTS_EN        =  BIT(2),
	FLD_URAT_RXTIMEOUT_RTS_EN     =  BIT(3),
	FLD_UART_MASK_RXDONE_IRQ      =  BIT(4),
	FLD_UART_P7816_EN	 	      =  BIT(5),
	FLD_UART_MASK_TXDONE_IRQ      =  BIT(6),
	FLD_UART_MASK_ERR_IRQ 	      =  BIT(7),
};


#define reg_uart_buf_cnt       REG_ADDR8(0x9c)

enum{
	FLD_UART_RX_BUF_CNT		=  BIT_RNG(0,3),
	FLD_UART_TX_BUF_CNT		=  BIT_RNG(4,7),
};

#define reg_uart_status0       REG_ADDR8(0x9d)
enum{
	FLD_UART_RBCNT 	     	=  BIT_RNG(0,2),
	FLD_UART_IRQ_FLAG    	=  BIT(3),
	FLD_UART_WBCNT 	     	=  BIT_RNG(4,6),
	FLD_URAT_CLEAR_RXDONE_FLAG = BIT(4),
	FLD_UART_CLEAR_RX_FLAG 	=  BIT(6),
	FLD_UART_RX_ERR_FLAG 	=  BIT(7),
};

#define reg_uart_status1       REG_ADDR8(0x9e)
enum{
	FLD_UART_TX_DONE   	  =  BIT(0),
	FLD_UART_TX_BUF_IRQ   =  BIT(1),
	FLD_UART_RX_DONE   	  =  BIT(2),
	FLD_UART_RX_BUF_IRQ   =  BIT(3),
	FLD_UART_RXDONE_IRQ   =  BIT(4),
	FLD_UART_AUTO_RXCLR_EN = BIT(6),
	FLD_UART_NDMA_RXDONE_EN=BIT(7),
};


#define reg_uart_state       REG_ADDR8(0x9f)
enum{
	FLD_UART_TSTATE_I 	     =  BIT_RNG(0,2),
	FLD_UART_RSTATE_I	     =  BIT_RNG(4,7),
};
#endif
/*******************************  (il) ir learn registers: 0xa0      ****************************/

#define reg_il_run           	REG_ADDR8(0xa0)

#define reg_il_cfg0          	REG_ADDR8(0xa1)
enum{
	FLD_IL_INPUT_SEL            = BIT_RNG(0,4),
	FLD_IL_MODE			  	    = BIT_RNG(5,6),
};

#define reg_il_cfg1          	REG_ADDR8(0xa2)
enum{
	FLD_IL_SFT                  = BIT_RNG(0,3),
	FLD_IL_TIMEOUT              = BIT_RNG(4,7),
};

#define reg_il_carry_stat    	REG_ADDR8(0xa3)
enum{
	FLD_IL_NOT_CARRY_STAT		= BIT(0),
};

#define reg_il_mask_0        	REG_ADDR8(0xa4)
enum{
	FLD_IL_INT_HIGH_MASK        = BIT(0),
	FLD_IL_INT_CYC_MASK         = BIT(1),
	FLD_IL_INT_TIMEOUT_MASK     = BIT(2),
};

#define reg_il_mask_1        	REG_ADDR8(0xa5)
enum{
	FLD_IL_INT_HIGH       		= BIT(0),
	FLD_IL_INT_CYC       		= BIT(1),
	FLD_IL_INT_TIMEOUT			= BIT(2),
};

#define reg_il_cnt_low			REG_ADDR8(0xa6)
#define reg_il_cnt_high			REG_ADDR8(0xa7)

#define reg_il_high_0			REG_ADDR32(0xa8)
#define reg_il_high_1			REG_ADDR32(0xa9)
#define reg_il_high_2			REG_ADDR32(0xaa)

#define reg_il_cycle_0			REG_ADDR32(0xac)
#define reg_il_cycle_1			REG_ADDR32(0xad)
#define reg_il_cycle_2			REG_ADDR32(0xae)


/*******************************      swire registers: 0xb0      ******************************/

#define reg_swire_data			REG_ADDR8(0xb0)
#define reg_swire_ctl			REG_ADDR8(0xb1)
enum{
	FLD_SWIRE_WR  = 			BIT(0),
	FLD_SWIRE_RD  = 			BIT(1),
	FLD_SWIRE_CMD =				BIT(2),
	FLD_SWIRE_ERR_FLAG			= BIT(3),
	FLD_SWIRE_EOP     			= BIT(4),
	FLD_SWIRE_USB_DET =			BIT(6),
	FLD_SWIRE_USB_EN =			BIT(7),
};

#define reg_swire_ctl2		REG_ADDR8(0xb2)

enum
{
	FLD_SWIRE_CLK_DIV = BIT_RNG(0,6),
};

#define reg_swire_id      		REG_ADDR8(0xb3)

enum
{
	FLD_SWIRE_ID_VALID	      = BIT_RNG(0,6),
	FLD_SWIRE_FIFO_MODE	 = BIT(7),
};

/*******************************      analog control registers: 0xb8      ******************************/

#define reg_ana_ctrl32			REG_ADDR32(0xb8)
#define reg_ana_addr_data		REG_ADDR16(0xb8)
#define reg_ana_addr			REG_ADDR8(0xb8)
#define reg_ana_data			REG_ADDR8(0xb9)
#define reg_ana_ctrl			REG_ADDR8(0xba)

enum{
	FLD_ANA_BUSY  = 			BIT(0),
	FLD_ANA_RSV	=				BIT(4),
	FLD_ANA_RW  = 				BIT(5),
	FLD_ANA_START  = 			BIT(6),
	FLD_ANA_CYC  = 				BIT(7),
};




/*******************************      qdec registers: 0xd0      ******************************/
#define 	REG_QDEC_BASE		    0xd0

#define reg_qdec_count0        REG_ADDR8(REG_QDEC_BASE+0x00)

#define reg_qdec_dbntime       REG_ADDR8(REG_QDEC_BASE+0x01)
enum{
	FLD_QDEC_DBNTIME  = BIT_RNG(0,2),
	FLD_QDEC_POLA     = BIT(4),
	FLS_QDEC_SHUTTLE0 = BIT(5),
};

#define reg_qdec_channel_a     REG_ADDR8(REG_QDEC_BASE+0x02)
enum{
	FLD_QDEC_CHANNEL_A   = BIT_RNG(0,2),
};
#define reg_qdec_channel_b     REG_ADDR8(REG_QDEC_BASE+0x03)
enum{
	FLD_QDEC_CHANNEL_B   = BIT_RNG(0,2),
};

#define reg_qdec_mask     REG_ADDR8(REG_QDEC_BASE+0x04)
enum{
	FLD_QDEC_MASK       = BIT(0),
};

#define reg_qdec_int0     REG_ADDR8(REG_QDEC_BASE+0x05)
enum{
	FLD_QDEC_INT0     = BIT(0),
};


#define reg_qdec_reset         REG_ADDR8(REG_QDEC_BASE+0x06)

#define reg_qdec_mode          REG_ADDR8(REG_QDEC_BASE+0x07)
enum{
	FLD_QDEC_DOUBLE0   = BIT(0),
};

#define reg_qdec_load 	       REG_ADDR8(REG_QDEC_BASE+0x08)
enum{
	FLD_QDEC_COUNT0_RELOAD   = BIT(0),
};

/*******************************      usb registers: 0x100      ******************************/

#define reg_ctrl_ep_ptr			REG_ADDR8(0x100)
#define reg_ctrl_ep_dat			REG_ADDR8(0x101)
#define reg_ctrl_ep_ctrl		REG_ADDR8(0x102)

enum{
	FLD_EP_DAT_ACK  = 			BIT(0),
	FLD_EP_DAT_STALL =			BIT(1),
	FLD_EP_STA_ACK = 			BIT(2),
	FLD_EP_STA_STALL = 			BIT(3),
};

#define reg_ctrl_ep_irq_sta		REG_ADDR8(0x103)
#if (MCU_CORE_B80)
enum{
	FLD_CTRL_EP_IRQ_TRANS  = 	BIT_RNG(0,3),
	FLD_CTRL_EP_IRQ_SETUP  =	BIT(4),
	FLD_CTRL_EP_IRQ_DATA  =		BIT(5),
	FLD_CTRL_EP_IRQ_STA  = 		BIT(6),
	FLD_CTRL_EP_IRQ_INTF  = 	BIT(7),
};
#else
enum
{
    FLD_USB_IRQ_RESET_STATUS    = BIT(0),
    FLD_USB_IRQ_250US_STATUS    = BIT(1),
    FLD_USB_IRQ_SUSPEND_STATUS  = BIT(2),
    FLD_USB_IRQ_SOF_STATUS      = BIT(3),
    FLD_CTRL_EP_IRQ_SETUP       = BIT(4),
    FLD_CTRL_EP_IRQ_DATA        = BIT(5),
    FLD_CTRL_EP_IRQ_STA         = BIT(6),
    FLD_CTRL_EP_IRQ_INTF        = BIT(7),
};
#endif

#define reg_ctrl_ep_irq_mode	REG_ADDR8(0x104)
enum{
	FLD_CTRL_EP_AUTO_ADDR = 	BIT(0),
	FLD_CTRL_EP_AUTO_CFG =		BIT(1),
	FLD_CTRL_EP_AUTO_INTF =		BIT(2),
	FLD_CTRL_EP_AUTO_STA =		BIT(3),
	FLD_CTRL_EP_AUTO_SYN =		BIT(4),
	FLD_CTRL_EP_AUTO_DESC =		BIT(5),
	FLD_CTRL_EP_AUTO_FEAT =		BIT(6),
	FLD_CTRL_EP_AUTO_STD =		BIT(7),
};

#define reg_usb_ctrl			REG_ADDR8(0x105)
enum{
	FLD_USB_CTRL_AUTO_CLK = 	BIT(0),
	FLD_USB_CTRL_LOW_SPD = 		BIT(1),
	FLD_USB_CTRL_LOW_JITT =		BIT(2),
	FLD_USB_CTRL_TST_MODE = 	BIT(3),
};

#define reg_usb_cyc_cali		REG_ADDR16(0x106)
#if (MCU_CORE_B80B)
#define reg_usb_edp0_size       REG_ADDR8(0x109) /* the size of edp0 */
enum
{
    FLD_USB_CTR_EP_SIZE         = BIT_RNG(0, 1), /**< 0-8bytes,1-16bytes,2-32bytes,3-64bytes, */
    FLD_USB_CTR_EP_LENS         = BIT_RNG(2, 3),
    FLD_USB_IRQ_RESET_LVL       = BIT(4),/**< lvl[0]:0-->irq_reset_edge; 1-->usb_reset_i */
    FLD_USB_IRQ_250US_LVL       = BIT(5),/**< lvl[1]:0-->irq_250us_edge; 1-->usb_250us_i */
    FLD_USB_IRQ_SOF_LVL         = BIT(7),/**< lvl[3]:0-->irq_sof_edge; 1-->usb_sof_i */
};
#endif
#define reg_usb_mdev			REG_ADDR8(0x10a)
#define reg_usb_host_conn		REG_ADDR8(0x10b)
enum{
	FLD_USB_MDEV_SELF_PWR = 	BIT(0),
	FLD_USB_MDEV_SUSP_STA = 	BIT(1),
};

#define reg_usb_sups_cyc_cali	REG_ADDR8(0x10c)
#define reg_usb_intf_alt		REG_ADDR8(0x10d)
#define reg_usb_ep_en			REG_ADDR8(0x10e)
typedef enum{
	FLD_USB_EDP8_EN 		= 	BIT(0),	// printer
	FLD_USB_EDP1_EN 		= 	BIT(1),	// keyboard
	FLD_USB_EDP2_EN 		= 	BIT(2),	// mouse
	FLD_USB_EDP3_EN 		= 	BIT(3),
	FLD_USB_EDP4_EN 		= 	BIT(4),
	FLD_USB_EDP5_EN 		= 	BIT(5),	// printer
	FLD_USB_EDP6_EN 		= 	BIT(6),	// audio
	FLD_USB_EDP7_EN 		= 	BIT(7),	// audio
}usb_ep_en_e;
#if (MCU_CORE_B80B)
#define reg_usb_irq_mask        REG_ADDR8(0x10f)
typedef enum 
{
    FLD_USB_IRQ_RESET_MASK      = BIT(0),
    FLD_USB_IRQ_250US_MASK      = BIT(1),
    FLD_USB_IRQ_SUSPEND_MASK    = BIT(2),
    FLD_USB_IRQ_SOF_MASK        = BIT(3),
    FLD_USB_IRQ_SETUP_MASK      = BIT(4),
    FLD_USB_IRQ_DATA_MASK       = BIT(5),
    FLD_USB_IRQ_STATUS_MASK     = BIT(6),
    FLD_USB_IRQ_SETINF_MASK     = BIT(7),
} usb_irq_mask;

#endif
#define reg_usb_ep8123_ptr		REG_ADDR32(0x110)
#define reg_usb_ep8_ptr			REG_ADDR8(0x110)
#define reg_usb_ep1_ptr			REG_ADDR8(0x111)
#define reg_usb_ep2_ptr			REG_ADDR8(0x112)
#define reg_usb_ep3_ptr			REG_ADDR8(0x113)
#define reg_usb_ep4567_ptr		REG_ADDR32(0x114)
#define reg_usb_ep4_ptr			REG_ADDR8(0x114)
#define reg_usb_ep5_ptr			REG_ADDR8(0x115)
#define reg_usb_ep6_ptr			REG_ADDR8(0x116)
#define reg_usb_ep7_ptr			REG_ADDR8(0x117)
#define reg_usb_ep_ptr(i)		REG_ADDR8(0x110+((i) & 0x07))

#define reg_usb_ep8123_dat		REG_ADDR32(0x118)
#define reg_usb_ep8_dat			REG_ADDR8(0x118)
#define reg_usb_ep1_dat			REG_ADDR8(0x119)
#define reg_usb_ep2_dat			REG_ADDR8(0x11a)
#define reg_usb_ep3_dat			REG_ADDR8(0x11b)
#define reg_usb_ep4567_dat		REG_ADDR32(0x11c)
#define reg_usb_ep4_dat			REG_ADDR8(0x11c)
#define reg_usb_ep5_dat			REG_ADDR8(0x11d)
#define reg_usb_ep6_dat			REG_ADDR8(0x11e)
#define reg_usb_ep7_dat			REG_ADDR8(0x11f)
#define reg_usb_ep_dat(i)		REG_ADDR8(0x118+((i) & 0x07))
#define reg_usb_mic_dat0		REG_ADDR16(0x1800)
#define reg_usb_mic_dat1		REG_ADDR16(0x1802)

#define reg_usb_ep8_ctrl		REG_ADDR8(0x120)
#define reg_usb_ep1_ctrl		REG_ADDR8(0x121)
#define reg_usb_ep2_ctrl		REG_ADDR8(0x122)
#define reg_usb_ep3_ctrl		REG_ADDR8(0x123)
#define reg_usb_ep4_ctrl		REG_ADDR8(0x124)
#define reg_usb_ep5_ctrl		REG_ADDR8(0x125)
#define reg_usb_ep6_ctrl		REG_ADDR8(0x126)
#define reg_usb_ep7_ctrl		REG_ADDR8(0x127)
#define reg_usb_ep_ctrl(i)		REG_ADDR8(0x120+((i) & 0x07))

enum{
	FLD_USB_EP_BUSY = 			BIT(0),
	FLD_USB_EP_STALL =			BIT(1),
	FLD_USB_EP_DAT0 =			BIT(2),
	FLD_USB_EP_DAT1 =			BIT(3),
	FLD_USB_EP_MONO =			BIT(6),
	FLD_USB_EP_EOF_ISO =		BIT(7),
};

#define reg_usb_ep8123_buf_addr	REG_ADDR32(0x128)
#define reg_usb_ep8_buf_addr	REG_ADDR8(0x128)
#define reg_usb_ep1_buf_addr	REG_ADDR8(0x129)
#define reg_usb_ep2_buf_addr	REG_ADDR8(0x12a)
#define reg_usb_ep3_buf_addr	REG_ADDR8(0x12b)
#define reg_usb_ep4567_buf_addr	REG_ADDR32(0x12c)
#define reg_usb_ep4_buf_addr	REG_ADDR8(0x12c)
#define reg_usb_ep5_buf_addr	REG_ADDR8(0x12d)
#define reg_usb_ep6_buf_addr	REG_ADDR8(0x12e)
#define reg_usb_ep7_buf_addr	REG_ADDR8(0x12f)
#define reg_usb_ep_buf_addr(i)	REG_ADDR8(0x128+((i) & 0x07))

#define reg_usb_ram_ctrl		REG_ADDR8(0x130)
enum{
	FLD_USB_CEN_PWR_DN =		BIT(0),
	FLD_USB_CLK_PWR_DN =		BIT(1),
	FLD_USB_WEN_PWR_DN =		BIT(3),
	FLD_USB_CEN_FUNC =			BIT(4),
};

#if (MCU_CORE_B80B)
#define reg_usb_map             REG_ADDR8(0x131)
enum
{
    FLD_CTRL_EP8_SEND_THRES_H   = BIT_RNG(0, 2),
    FLD_USB_EDP_MAP_MANUAL_EN   = BIT(3),
    FLD_USB_EDP_MAP_AUTO_EN     = BIT(4),
    FLD_USB_EDPS_SM_MAP_EN      = BIT(5),
    FLD_USB_EDPS_MAP_TGL_EN     = BIT(6),
    FLD_USB_GET_STA_MAP_EN      = BIT(7),
};
#define reg_usb_edps_map_en     REG_ADDR8(0x132)
#define reg_usb_edps_logic_en   REG_ADDR8(0x133)

#define reg_usb_rdps_map(i)     REG_ADDR8(0x134 + (((i)&0x07) >> 1))
#endif

#define reg_usb_iso_mode		REG_ADDR8(0x138)
#define reg_usb_irq				REG_ADDR8(0x139)
#define reg_usb_mask			REG_ADDR8(0x13a)
typedef enum
{
    FLD_USB_EDP8_IRQ        = BIT(0),
    FLD_USB_EDP1_IRQ        = BIT(1),
    FLD_USB_EDP2_IRQ        = BIT(2),
    FLD_USB_EDP3_IRQ        = BIT(3),
    FLD_USB_EDP4_IRQ        = BIT(4),
    FLD_USB_EDP5_IRQ        = BIT(5),
    FLD_USB_EDP6_IRQ        = BIT(6),
    FLD_USB_EDP7_IRQ        = BIT(7),
} usb_eps_irq_e;

#define reg_usb_ep8_send_max	    REG_ADDR8(0x13b)
#define reg_usb_ep8_send_thres		REG_ADDR8(0x13c)
#define reg_usb_ep8_fifo_mode		REG_ADDR8(0x13d)
#define reg_usb_ep_max_size			REG_ADDR8(0x13e)

enum{
	FLD_USB_ENP8_FIFO_MODE =	BIT(0),
	FLD_USB_ENP8_FULL_FLAG =	BIT(1),
};

#if (MCU_CORE_B80B)
#define reg_usb_edps_eptrl          REG_ADDR8(0x142)
#define reg_usb_edps_eptrh          REG_ADDR8(0x143)
#define reg_usb_tstamp0             REG_ADDR8(0x144)
#define reg_usb_tstamp1             REG_ADDR8(0x145)
#define reg_usb_tstamp2             REG_ADDR8(0x146)
#define reg_usb_tstamp3             REG_ADDR8(0x147)
#define reg_usb_sof_frame0          REG_ADDR8(0x148)
#define reg_usb_sof_frame1          REG_ADDR8(0x149)
#define reg_usb_edps_full_thrd      REG_ADDR8(0x14a)/* zero package flag */
#endif

#define reg_rf_acc_len			REG_ADDR8(0x405)
enum{
	FLD_RF_ACC_LEN	    = 		BIT_RNG(0,2),
	FLD_RF_LR_MAN_EN	=       BIT(3),   //long range manual enable
	FLD_RF_LR_TX_SEL	=   	BIT(4),
	FLD_RF_BLE_LR		=   	BIT(5),
	FLD_RF_LR_ACC_TRIG	=   	BIT(7),
};


#define reg_rf_timestamp		REG_ADDR32(0x450)


/*******************************      aes registers: 0x700      ******************************/

#define reg_aes_ctrl            REG_ADDR8(0x700)

enum {
    FLD_AES_CTRL_CODEC_TRIG = BIT(0),
    FLD_AES_CTRL_DATA_FEED = BIT(1),
    FLD_AES_CTRL_CODEC_FINISHED = BIT(2),
};

#define reg_aes_data            REG_ADDR32(0x708)
#define reg_aes_key(v)     		REG_ADDR8(0x710+v)

/*******************************      gpio registers: 0x500      ******************************/
#define GPIO_BASE_ADDR          0x500
#define reg_gpio_pa_in			REG_ADDR8(GPIO_BASE_ADDR)
#define reg_gpio_pa_ie			REG_ADDR8(GPIO_BASE_ADDR+0x01)
#define reg_gpio_pa_oen			REG_ADDR8(GPIO_BASE_ADDR+0x02)
#define reg_gpio_pa_out			REG_ADDR8(GPIO_BASE_ADDR+0x03)
#define reg_gpio_pa_pol			REG_ADDR8(GPIO_BASE_ADDR+0x04)
#define reg_gpio_pa_ds			REG_ADDR8(GPIO_BASE_ADDR+0x05)
#define reg_gpio_pa_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x06)
#define reg_gpio_pa_irq_en		REG_ADDR8(GPIO_BASE_ADDR+0x07)

#define reg_gpio_pb_in			REG_ADDR8(GPIO_BASE_ADDR+0x08)
#define reg_gpio_pb_ie			REG_ADDR8(GPIO_BASE_ADDR+0x09)
#define reg_gpio_pb_oen			REG_ADDR8(GPIO_BASE_ADDR+0x0a)
#define reg_gpio_pb_out			REG_ADDR8(GPIO_BASE_ADDR+0x0b)
#define reg_gpio_pb_pol			REG_ADDR8(GPIO_BASE_ADDR+0x0c)
#define reg_gpio_pb_ds			REG_ADDR8(GPIO_BASE_ADDR+0x0d)
#define reg_gpio_pb_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x0e)
#define reg_gpio_pb_irq_en		REG_ADDR8(GPIO_BASE_ADDR+0x0f)

#define reg_gpio_pc_in			REG_ADDR8(GPIO_BASE_ADDR+0x10)
#define areg_gpio_pc_ie			0xc0
#define reg_gpio_pc_oen			REG_ADDR8(GPIO_BASE_ADDR+0x12)
#define reg_gpio_pc_out			REG_ADDR8(GPIO_BASE_ADDR+0x13)
#define reg_gpio_pc_pol			REG_ADDR8(GPIO_BASE_ADDR+0x14)
#define areg_gpio_pc_pe         0xc1
#define areg_gpio_pc_ds			0xc2
#define reg_gpio_pc_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x16)
#define reg_gpio_pc_irq_en		REG_ADDR8(GPIO_BASE_ADDR+0x17)

#define reg_gpio_pd_in			REG_ADDR8(GPIO_BASE_ADDR+0x18)
#define reg_gpio_pd_ie			REG_ADDR8(GPIO_BASE_ADDR+0x19)
#define reg_gpio_pd_oen			REG_ADDR8(GPIO_BASE_ADDR+0x1a)
#define reg_gpio_pd_out			REG_ADDR8(GPIO_BASE_ADDR+0x1b)
#define reg_gpio_pd_pol			REG_ADDR8(GPIO_BASE_ADDR+0x1c)
#define reg_gpio_pd_ds			REG_ADDR8(GPIO_BASE_ADDR+0x1d)
#define reg_gpio_pd_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x1e)
#define reg_gpio_pd_irq_en		REG_ADDR8(GPIO_BASE_ADDR+0x1f)

#define reg_gpio_pe_in			REG_ADDR8(GPIO_BASE_ADDR+0x20)
#define reg_gpio_pe_ie			REG_ADDR8(GPIO_BASE_ADDR+0x21)
#define reg_gpio_pe_oen			REG_ADDR8(GPIO_BASE_ADDR+0x22)
#define reg_gpio_pe_out			REG_ADDR8(GPIO_BASE_ADDR+0x23)
#define reg_gpio_pe_pol			REG_ADDR8(GPIO_BASE_ADDR+0x24)
#define reg_gpio_pe_ds			REG_ADDR8(GPIO_BASE_ADDR+0x25)
#define reg_gpio_pe_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x26)
#define reg_gpio_pe_irq_en		REG_ADDR8(GPIO_BASE_ADDR+0x27)

#define reg_gpio_pf_in			REG_ADDR8(GPIO_BASE_ADDR+0x28)
#define reg_gpio_pf_ie			REG_ADDR8(GPIO_BASE_ADDR+0x29)
#define reg_gpio_pf_oen			REG_ADDR8(GPIO_BASE_ADDR+0x2a)
#define reg_gpio_pf_out			REG_ADDR8(GPIO_BASE_ADDR+0x2b)
#define reg_gpio_pf_pol			REG_ADDR8(GPIO_BASE_ADDR+0x2c)
#define reg_gpio_pf_ds			REG_ADDR8(GPIO_BASE_ADDR+0x2d)
#define reg_gpio_pf_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x2e)
#define reg_gpio_pf_irq_en		REG_ADDR8(GPIO_BASE_ADDR+0x2f)

#define reg_gpio_pa_setting1	REG_ADDR32(GPIO_BASE_ADDR)
#define reg_gpio_pa_setting2	REG_ADDR32(GPIO_BASE_ADDR+0x04)
#define reg_gpio_pb_setting1	REG_ADDR32(GPIO_BASE_ADDR+0x08)
#define reg_gpio_pb_setting2	REG_ADDR32(GPIO_BASE_ADDR+0x0c)
#define reg_gpio_pc_setting1	REG_ADDR32(GPIO_BASE_ADDR+0x10)
#define reg_gpio_pc_setting2	REG_ADDR32(GPIO_BASE_ADDR+0x14)
#define reg_gpio_pd_setting1	REG_ADDR32(GPIO_BASE_ADDR+0x18)
#define reg_gpio_pd_setting2	REG_ADDR32(GPIO_BASE_ADDR+0x1c)
#define reg_gpio_pf_setting1	REG_ADDR32(GPIO_BASE_ADDR+0x28)
#define reg_gpio_pf_setting2	REG_ADDR32(GPIO_BASE_ADDR+0x2c)

#define reg_gpio_in(i)				REG_ADDR8(GPIO_BASE_ADDR+((i>>8)<<3))
#define reg_gpio_ie(i)				REG_ADDR8(GPIO_BASE_ADDR+0x01+((i>>8)<<3))
#define reg_gpio_oen(i)				REG_ADDR8(GPIO_BASE_ADDR+0x02+((i>>8)<<3))
#define reg_gpio_pol(i)				REG_ADDR8(GPIO_BASE_ADDR+0x04+((i>>8)<<3))
#define reg_gpio_ds(i)				REG_ADDR8(GPIO_BASE_ADDR+0x05+((i>>8)<<3))
#define reg_gpio_func(i)			REG_ADDR8(GPIO_BASE_ADDR+0x06+((i>>8)<<3))
#define reg_gpio_irq_wakeup_en(i)	REG_ADDR8(GPIO_BASE_ADDR+0x07+((i>>8)<<3))  // reg_irq_mask: FLD_IRQ_GPIO_EN
#if(MCU_CORE_B80)
#define reg_gpio_out(i)				REG_ADDR8(GPIO_BASE_ADDR+0x03+((i>>8)<<3))
#elif (MCU_CORE_B80B)
#define reg_gpio_out_set_clear(i)   REG_ADDR16(GPIO_BASE_ADDR+0xc0+(((i)>>8)<<2))
#define reg_gpio_out_set(i)         REG_ADDR8(GPIO_BASE_ADDR+0xc0+(((i)>>8)<<2))
#define reg_gpio_out_clear(i)       REG_ADDR8(GPIO_BASE_ADDR+0xc1+(((i)>>8)<<2))
#define reg_gpio_out_toggle(i)      REG_ADDR8(GPIO_BASE_ADDR+0xc2+(((i)>>8)<<2))
#endif
#define reg_gpio_pe(i)              REG_ADDR8(GPIO_BASE_ADDR+0xc3+(((i)>>8)<<2))


#define reg_gpio_func_mux(i)        REG_ADDR8(GPIO_BASE_ADDR+0x48+(((i&0x500) == 0x500) ? ((((i>>8)-1)<<3)+4) : ((i>>8)<<3))+((i&0x80) ? 7 : 0)+((i&0x40) ? 6 : 0)+((i&0x20) ? 5 : 0)+((i&0x10) ? 4 : 0) +((i&0x8) ? 3 : 0)+((i&0x4) ? 2 : 0)+((i&0x2) ? 1 : 0))

#define reg_gpio_irq_risc0_en(i)  	REG_ADDR8(GPIO_BASE_ADDR+0x30 + (i >> 8))	  // reg_irq_mask: FLD_IRQ_GPIO_RISC0_EN
#define reg_gpio_irq_risc1_en(i)  	REG_ADDR8(GPIO_BASE_ADDR+0x38 + (i >> 8))	  // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
#define reg_gpio_irq_risc2_en(i)  	REG_ADDR8(GPIO_BASE_ADDR+0x40 + (i >> 8))

#if(MCU_CORE_B80B)
#define reg_gpio_irq_risc3_risc7_en(i,j)    REG_ADDR8(0x598 + ((i)>>8)+((j)<<3))
#define reg_gpio_irq_risc3_en(i)            REG_ADDR8(0x598 + (i >> 8))
#define reg_gpio_irq_risc4_en(i)            REG_ADDR8(0x5a0 + (i >> 8))
#define reg_gpio_irq_risc5_en(i)            REG_ADDR8(0x5a8 + (i >> 8))
#define reg_gpio_irq_risc6_en(i)            REG_ADDR8(0x5b0 + (i >> 8))
#define reg_gpio_irq_risc7_en(i)            REG_ADDR8(0x5b8 + (i >> 8))
#endif
#define reg_comb_irq				REG_ADDR8(0x56e)
enum{
	FLD_IRQ_STMER_LEV					= BIT(0),
	FLD_IRQ_SPI							= BIT(1),
	FLD_IRQ_KS							= BIT(2),//When used to query the interrupt status of the keyscan, the interrupt mask bit (FLD_KS_IE) of the keyscan must be set.
	FLD_IRQ_IRLERAN						= BIT(3),
	FLD_IRQ_SOFTWARE					= BIT(4),
	FLD_IRQ_QDEC						= BIT(5),
	FLD_IRQ_HOST_CMD					= BIT(6),
};

#define reg_gpio_wakeup_irq  		REG_ADDR8(0x574)
enum{
    FLD_GPIO_CORE_WAKEUP_EN  	= BIT(1),
    FLD_GPIO_CORE_INTERRUPT_EN 	= BIT(2),
};
#if(MCU_CORE_B80)
#define reg_gpio_irq_sel          REG_ADDR8(0x575)
#elif (MCU_CORE_B80B)
#define reg_gpio_irq_ctrl_mux     REG_ADDR8(0x575)
enum{
	FLD_IRQ_MUX_EN0                      = BIT(0),
	FLD_IRQ_MUX_EN1                      = BIT(1),
	FLD_IRQ_MUX_EN2                      = BIT(2),
};
#endif
#define reg_gpio_irq_from_pad     REG_ADDR8(0x56f)
#define reg_gpio_irq_pad_mask     REG_ADDR8(0x576)
#define reg_gpio_irq_lvl          REG_ADDR8(0x577)








/*******************************      timer registers: 0x620      ******************************/

#define reg_tmr_ctrl			REG_ADDR32(0x620)
#define reg_tmr_ctrl16			REG_ADDR16(0x620)
#define reg_tmr_ctrl8			REG_ADDR8(0x620)
enum{
	FLD_TMR0_EN =				BIT(0),
	FLD_TMR0_MODE =				BIT_RNG(1,2),
	FLD_TMR1_EN = 				BIT(3),
	FLD_TMR1_MODE =				BIT_RNG(4,5),
	FLD_TMR2_EN =				BIT(6),
	FLD_TMR2_MODE = 			BIT_RNG(7,8),
	FLD_TMR_WD_CAPT = 			BIT_RNG(9,22),
	FLD_TMR_WD_EN =				BIT(23),
	FLD_TMR0_STA =				BIT(24),
	FLD_TMR1_STA =				BIT(25),
	FLD_TMR2_STA =				BIT(26),
	FLD_CLR_WD =				BIT(27),
};

#define reg_wd_ctrl1            REG_ADDR8(0x622)
enum{
	FLD_WD_EN     = BIT(7),
};
#define reg_tmr_sta				REG_ADDR8(0x623)
enum{
	FLD_TMR_STA_TMR0 =			BIT(0),
	FLD_TMR_STA_TMR1 =			BIT(1),
	FLD_TMR_STA_TMR2 =			BIT(2),
	FLD_TMR_STA_WD =			BIT(3),
};

#define reg_tmr0_capt			REG_ADDR32(0x624)
#define reg_tmr1_capt			REG_ADDR32(0x628)
#define reg_tmr2_capt			REG_ADDR32(0x62c)
#define reg_tmr_capt(i)			REG_ADDR32(0x624 + ((i) << 2))
#define reg_tmr0_tick			REG_ADDR32(0x630)
#define reg_tmr1_tick			REG_ADDR32(0x634)
#define reg_tmr2_tick			REG_ADDR32(0x638)
#define reg_tmr_tick(i)			REG_ADDR32(0x630 + ((i) << 2))

/*******************************      irq registers: 0x640      ******************************/

#define reg_irq_mask			REG_ADDR32(0x640)
#define reg_irq_pri				REG_ADDR32(0x644)
#define reg_irq_src				REG_ADDR32(0x648)
#define reg_irq_src3			REG_ADDR8(0x64a)
typedef enum{
	FLD_IRQ_TMR0_EN =			BIT(0),
	FLD_IRQ_TMR1_EN =			BIT(1),
	FLD_IRQ_TMR2_EN =			BIT(2),
	FLD_IRQ_USB_PWDN_EN =		BIT(3),
	FLD_IRQ_DMA_EN =			BIT(4),
	FLD_IRQ_DAM_FIFO_EN =		BIT(5),
	FLD_IRQ_UART_EN 	=		BIT(6),
	FLD_IRQ_MIX_CMD_EN =		BIT(7), FLD_IRQ_HOST_CMD_EN  =	BIT(7),FLD_IRQ_SPI_EN =BIT(7),FLD_IRQ_IR_LEARN_EN = BIT(7),FLD_IRQ_KS_CMD_EN = BIT(7),//< MIX = I2C/QDEC/SPI/KEYSCAN

	FLD_IRQ_EP0_SETUP_EN =		BIT(8),
	FLD_IRQ_EP0_DAT_EN =		BIT(9),
	FLD_IRQ_EP0_STA_EN =		BIT(10),
#if(MCU_CORE_B80B)
	FLD_IRQ_SET_INTF_EN =		BIT(11),  FLD_IRQ_UART1_EN = BIT(11),
#else
	FLD_IRQ_SET_INTF_EN =		BIT(11),
#endif
	FLD_IRQ_EP_DATA_EN =		BIT(12),  FLD_IRQ_IRQ4_EN = BIT(12),
	FLD_IRQ_ZB_RT_EN =			BIT(13),
	FLD_IRQ_SW_PWM_EN =			BIT(14),  //irq_software | irq_pwm
	FLD_IRQ_GPIO_NEW_EN =		BIT(15),//	RSVD 		=			BIT(15),

#if (MCU_CORE_B80)
	FLD_IRQ_USB_250US_EN =		BIT(16),
#else
	FLD_IRQ_USB_250US_OR_SOF_EN = BIT(16),
#endif
	FLD_IRQ_USB_RST_EN =		BIT(17),
	FLD_IRQ_GPIO_EN =			BIT(18),
	FLD_IRQ_PM_EN =				BIT(19),
	FLD_IRQ_SYSTEM_TIMER =		BIT(20),
	FLD_IRQ_GPIO_RISC0_EN =		BIT(21),
	FLD_IRQ_GPIO_RISC1_EN =		BIT(22),
    FLD_IRQ_GPIO_RISC2_EN =     BIT(23),

	FLD_IRQ_EN =				BIT_RNG(24,31),
	FLD_IRQ_ALL           =     0XFFFFFFFF,
}irq_list_e;
#define reg_irq_en				REG_ADDR8(0x643)




/*******************************      system timer registers: 0x740      ******************************/

#define reg_system_tick				REG_ADDR32(0x740)
#define reg_system_tick_irq_level	REG_ADDR32(0x744)
#define reg_system_irq_mask			REG_ADDR8(0x748)
#define reg_system_cal_irq			REG_ADDR8(0x749)
#define reg_system_ctrl				REG_ADDR8(0x74a)
enum{
	FLD_SYSTEM_32K_CAL_MODE 	= 	BIT_RNG(4,7),
	FLD_SYSTEM_32K_CAL_EN 		= 	BIT(3),
	FLD_SYSTEM_TIMER_AUTO_EN 	= 	BIT(2),
	FLD_SYSTEM_TIMER_MANUAL_EN 	= 	BIT(1),
	FLD_SYSTEM_32K_WR_EN 		= 	BIT(0),
};

#define reg_system_status			REG_ADDR8(0x74b)
enum{
	FLD_SYSTEM_32K_TIMER_BUSY 			=   BIT(6),
	FLD_SYSTEM_32K_TIMER_UPDATE_RD 		=   BIT(5),			FLD_SYSTEM_32K_TIMER_CLEAR_RD		=   BIT(5),
	FLD_SYSTEM_STATE_MACHINE_STATUS		=   BIT_RNG(3,4),  	FLD_SYSTEM_32K_TIMER_SYNC_TRIG		=   BIT(3),
	FLD_SYSTEM_CMD_SET_TRIG 			=   BIT(2),
	FLD_SYSTEM_CMD_SYNC_TRIG 			=   BIT(1),
	FLD_SYSTEM_TIMER_STATUS 			=   BIT(0),
};

#define reg_system_32k_tick_wt  	REG_ADDR32(0x74c)
#define reg_system_32k_tick_rd  	REG_ADDR32(0x750)
#define reg_system_32k_tick_cal  	REG_ADDR32(0x754)

/*******************************      pwm registers: 0x780      ******************************/

#define reg_pwm_m_enable		REG_ADDR16(0x780)
#define reg_pwm_enable			REG_ADDR8(0x780)
#define reg_pwm0_enable			REG_ADDR8(0x781)
typedef enum{
	FLD_PWM1_EN = BIT(1),
	FLD_PWM2_EN = BIT(2),
	FLD_PWM3_EN = BIT(3),
	FLD_PWM4_EN = BIT(4),
	FLD_PWM5_EN = BIT(5),
	FLD_PWM0_EN = BIT(8),
}pwm_en_e;

#define reg_pwm_clk				REG_ADDR8(0x782)

#define reg_pwm0_mode			REG_ADDR8(0x783)


#define reg_pwm_invert			REG_ADDR8(0x784)
#define reg_pwm_n_invert		REG_ADDR8(0x785)
#define reg_pwm_pol				REG_ADDR8(0x786)

#define reg_pwm_cycle(i)		REG_ADDR32(0x794 + (i << 2))   //<15:0>: TCMP 0~5  <31:16>: TMAX 0~5
#define reg_pwm_cmp(i)			REG_ADDR16(0x794 + (i << 2))   //TCMP 0~5
#define reg_pwm_max(i)			REG_ADDR16(0x796 + (i << 2))   //TMAX 0~5
enum{
	FLD_PWM_CMP  = 				BIT_RNG(0,15),
	FLD_PWM_MAX  = 				BIT_RNG(16,31),
};

#define reg_pwm0_pulse_num		REG_ADDR16(0x7ac)

#define reg_pwm_irq_mask		REG_ADDR8(0x7b0)
#define reg_pwm_irq_sta			REG_ADDR8(0x7b1)
enum{
	FLD_IRQ_PWM0_PNUM =					BIT(0),
	FLD_IRQ_PWM0_IR_DMA_FIFO_DONE  =	BIT(1),
	FLD_IRQ_PWM0_FRAME =				BIT(2),
	FLD_IRQ_PWM1_FRAME =				BIT(3),
	FLD_IRQ_PWM2_FRAME =				BIT(4),
	FLD_IRQ_PWM3_FRAME =				BIT(5),
	FLD_IRQ_PWM4_FRAME 	=				BIT(6),
	FLD_IRQ_PWM5_FRAME =				BIT(7),
};



#define reg_pwm0_fifo_mode_irq_mask		REG_ADDR8(0x7b2)

enum{
	FLD_PWM0_IRQ_IR_FIFO_EN  	 = BIT(0),
};

#define reg_pwm0_fifo_mode_irq_sta		REG_ADDR8(0x7b3)

enum{
	FLD_PWM0_IRQ_IR_FIFO_CNT 	 = BIT(0),
};



#define reg_pwm_tcmp0_shadow		REG_ADDR16(0x7c4)   //<15:0>: TCMP 0~5  <31:16>: TMAX 0~5
#define reg_pwm_tmax0_shadow		REG_ADDR16(0x7c6)   //TCMP 0~5

#define reg_pwm_ir_fifo_dat(i)			REG_ADDR16(0x7c8+i*2)
#define reg_pwm_ir_fifo_irq_trig_level	REG_ADDR8(0x7cc)

#define reg_pwm_ir_fifo_data_status		REG_ADDR8(0x7cd)
enum{
	FLD_PWM0_IR_FIFO_DATA_NUM 	=		BIT_RNG(0,3),
	FLD_PWM0_IR_FIFO_EMPTY 		=		BIT(4),
	FLD_PWM0_IR_FIFO_FULL 		=		BIT(5),
};

#define reg_pwm_ir_clr_fifo_data		REG_ADDR8(0x7ce)

enum{
	FLD_PWM0_IR_FIFO_CLR_DATA 	=		BIT(0),
};

/*******************************      keyscan registers: 0x800      ******************************/
#define KS_BASE						0x0800
#define reg_ks_col_mask0			REG_ADDR32(KS_BASE)
#define reg_ks_row_sel0				REG_ADDR32(KS_BASE+0x04)
#define reg_ks_row_sel4				REG_ADDR8(KS_BASE+0x08)
#define reg_ks_end_flag				REG_ADDR8(KS_BASE+0x09)
#define reg_ks_en					REG_ADDR8(KS_BASE+0x0a)
enum{
	FLD_KS_EN 				= BIT(0),
	FLD_KS_CLK_EN 			= BIT(1),
	FLD_KS_IE 				= BIT(2),
	FLD_KS_IN_INV 			= BIT(3),
	FLD_KS_OUT_INV			= BIT(4),
	FLD_KS_MOD				= BIT(5),
	FLD_KS_RESET			= BIT(6),
	FLD_KS_TRI_CHK_DIS		= BIT(7),
};
#define reg_ks_frm_num				REG_ADDR8(KS_BASE+0x0b)
enum{
	FLD_KS_FRM_NUM 				= BIT_RNG(0,4),
	FLD_KS_DEBOUNCE_PERIOD		= BIT_RNG(5,7),
};
#define reg_ks_irq					REG_ADDR8(KS_BASE+0x0c)
enum{
	FLD_KS_RPTR 				= BIT_RNG(0,4),
	FLD_KS_FRM_END				= BIT(7),
};
#define reg_ks_rptr					REG_ADDR8(KS_BASE+0x0d)
enum{
	FLD_KS_R_WPTR 				= BIT_RNG(0,4),
	FLD_KS_KEY_DET_CAP			= BIT(6),
	FLD_KS_STATE				= BIT(7),
};
#define reg_ks_wptr					REG_ADDR8(KS_BASE+0x0e)
enum{
	FLD_KS_WPTR 				= BIT_RNG(0,4),
	FLD_KS_NO_KEY				= BIT(5),
	FLD_KS_KEY_DET				= BIT(6),
	FLD_KS_CNT_EN				= BIT(7),
};
#define reg_ks_gated				REG_ADDR8(KS_BASE+0x0f)
enum{
	FLD_KS_CNT_H 				= BIT_RNG(0,2),
	FLD_KS_GATED_CLR				= BIT(4),
	FLD_KS_GATED				= BIT(5),
	FLD_KS_CNT16_EN				= BIT(6),
	FLD_KS_WAKE_UP				= BIT(7),
};
#define reg_ks_key					REG_ADDR8(KS_BASE+0x10)
#define reg_ks_lptr					REG_ADDR8(KS_BASE+0x11)
enum{
	FLD_KS_LPTR 				= BIT_RNG(0,4),
};
#define reg_ks_cnt128					REG_ADDR8(KS_BASE+0x12)
enum{
	FLD_KS_CNT 					= BIT_RNG(0,6),
};
#define reg_ks_cnt16					REG_ADDR8(KS_BASE+0x13)
enum{
	FLD_KS_CNT_16 					= BIT_RNG(0,3),
	FLD_KS_ROW_NUM 					= BIT_RNG(4,6),
};

/*******************************      dfifo registers: 0xb00      ******************************/

#define reg_dfifo0_addr			REG_ADDR16(0xb00)
#define reg_dfifo0_size			REG_ADDR8(0xb02)
#define reg_dfifo0_addHi		REG_ADDR8(0xb03)  //default 0x04, no need set

#define reg_dfifo1_addr			REG_ADDR16(0xb04)
#define reg_dfifo1_size			REG_ADDR8(0xb06)
#define reg_dfifo1_addHi		REG_ADDR8(0xb07)  //default 0x04, no need set

//misc channel only use dfifo2
#define reg_dfifo2_addr			REG_ADDR16(0xb08)
#define reg_dfifo2_size			REG_ADDR8(0xb0a)
#define reg_dfifo2_addHi		REG_ADDR8(0xb0b)  //default 0x04, no need set

#define reg_dfifo_audio_addr		reg_dfifo0_addr
#define reg_dfifo_audio_size		reg_dfifo0_size

#define reg_dfifo_misc_chn_addr		reg_dfifo2_addr
#define reg_dfifo_misc_chn_size		reg_dfifo2_size


#define reg_dfifo0_l_level		REG_ADDR8(0xb0c)  //dfifo0  low int threshold(wptr - rptr)
#define reg_dfifo0_h_level		REG_ADDR8(0xb0d)  //dfifo0 high int threshold(wptr - rptr)
#define reg_dfifo1_h_level		REG_ADDR8(0xb0e)  //dfifo1 high int threshold(wptr - rptr)
#define reg_dfifo2_h_level		REG_ADDR8(0xb0f)  //dfifo2 high int threshold(wptr - rptr)


#define	reg_dfifo_mode	REG_ADDR8(0xb10)
enum{
	FLD_AUD_DFIFO0_IN 		= BIT(0),
	FLD_AUD_DFIFO1_IN 		= BIT(1),
	FLD_AUD_DFIFO2_IN 		= BIT(2),
	FLD_AUD_DFIFO0_OUT 		= BIT(3),
	FLD_AUD_DFIFO0_L_INT	= BIT(4),
	FLD_AUD_DFIFO0_H_INT	= BIT(5),
	FLD_AUD_DFIFO1_H_INT	= BIT(6),
	FLD_AUD_DFIFO2_H_INT	= BIT(7),
};

#define	reg_dfifo_ain			REG_ADDR8(0xb11)
enum{
	FLD_AUD_SAMPLE_TIME_CONFIG        = BIT_RNG(0,1),
	FLD_AUD_FIFO0_INPUT_SELECT     	  = BIT_RNG(2,3),
	FLD_AUD_FIFO1_INPUT_SELECT     	  = BIT_RNG(4,5),
	FLD_AUD_MIC_LEFT_CHN_SELECT       = BIT(6),
	FLD_AUD_MIC_RIGHT_CHN_SELECT      = BIT(7),
};

enum{  //core_b11<3:2>  audio input select
	AUDIO_FIFO0_INPUT_SELECT_USB 	        = 0x00,
	AUDIO_FIFO0_INPUT_SELECT_I2S 	        = 0x01,
	AUDIO_FIFO0_INPUT_SELECT_16BIT 	        = 0x02,
	AUDIO_FIFO0_INPUT_SELECT_20BIT 	        = 0x03,
};

enum{  //core_b11<5:4>  audio input select
	AUDIO_FIFO1_INPUT_SELECT_USB 	        = 0x00,
	AUDIO_FIFO1_INPUT_SELECT_I2S 	        = 0x01,
	AUDIO_FIFO1_INPUT_SELECT_16BIT 	        = 0x02,
	AUDIO_FIFO1_INPUT_SELECT_20BIT 	        = 0x03,
};


#define	reg_mic_ctrl			REG_ADDR8(0xb12)
enum{
	FLD_AUD_MIC_VOL_CONTROL           = BIT_RNG(0,5),
	FLD_AUD_MIC_MONO_EN               = BIT(6),
	FLD_AUD_AMIC_DMIC_SELECT          = BIT(7),
};

enum{
	MIC_VOL_CONTROL_m48DB = 		0x00,
	MIC_VOL_CONTROL_m42DB = 		0x04,
	MIC_VOL_CONTROL_m36DB = 		0x08,
	MIC_VOL_CONTROL_m30DB = 		0x0c,
	MIC_VOL_CONTROL_m24DB = 		0x10,
	MIC_VOL_CONTROL_m18DB = 		0x14,
	MIC_VOL_CONTROL_m16DB = 		0x15,
	MIC_VOL_CONTROL_m12DB = 		0x18,
	MIC_VOL_CONTROL_m6DB  = 	   	0x1c,
	MIC_VOL_CONTROL_0DB   = 	    0x20,
	MIC_VOL_CONTROL_6DB   =  		0x24,
	MIC_VOL_CONTROL_12DB  = 		0x28,
	MIC_VOL_CONTROL_18DB  = 		0x2c,
	MIC_VOL_CONTROL_24DB  = 		0x30,
	MIC_VOL_CONTROL_30DB  = 		0x34,
	MIC_VOL_CONTROL_36DB  = 		0x38,
	MIC_VOL_CONTROL_42DB  =     	0x3c,
};


#define reg_set_filter_para     REG_ADDR8(0xb80)

#define reg_dfifo_dec_ratio	    REG_ADDR8(0xb8a)

#define reg_codec_dec_en	    REG_ADDR8(0xb8b)
enum{
	FLD_AUD_CODEC_DEC_EN	=   BIT(0),
};

#define reg_dfifo_irq_status	REG_ADDR8(0xb13)
enum{
	FLD_AUD_DFIFO0_L_IRQ	= BIT(4),
	FLD_AUD_DFIFO0_H_IRQ	= BIT(5),
	FLD_AUD_DFIFO1_L_IRQ	= BIT(6),
	FLD_AUD_DFIFO1_H_IRQ	= BIT(7),
};
#define reg_dfifo0_rptr			REG_ADDR16(0xb14)
#define reg_dfifo0_wptr			REG_ADDR16(0xb16)

#define reg_dfifo1_rptr			REG_ADDR16(0xb18)
#define reg_dfifo1_wptr			REG_ADDR16(0xb1a)

#define reg_dfifo2_rptr			REG_ADDR16(0xb1c)
#define reg_dfifo2_wptr			REG_ADDR16(0xb1e)


#define reg_audio_wptr			reg_dfifo0_wptr

#define reg_dfifo0_num			REG_ADDR16(0xb20)
#define reg_dfifo1_num			REG_ADDR16(0xb24)
#define reg_dfifo2_num			REG_ADDR16(0xb28)

#define reg_dfifo_manual_mode		REG_ADDR8(0xb2c)
enum{
	FLD_DFIFO_MANUAL_MODE_EN	= BIT(0),
};

#define reg_dfifo0_man_dat		REG_ADDR32(0xb30)


/*******************************      dma registers: 0xc00      ******************************/

//uart rx
#define reg_dma0_addr			REG_ADDR16(0xc00)
#define reg_dma0_size			REG_ADDR8(0xc02)
#define reg_dma0_mode			REG_ADDR8(0xc03)
enum{
	FLD_DMA_WR_MEM =			BIT(0),
	FLD_DMA_PINGPONG_EN =		BIT(1),
	FLD_DMA_FIFO_EN =			BIT(2),
	FLD_DMA_AUTO_MODE =			BIT(3),
	FLD_DMA_READ_MODE =			BIT(4),
	FLD_DMA_BYTE_MODE =			BIT(5)
};


//uart tx
#define reg_dma1_addr			REG_ADDR16(0xc04)
#define reg_dma1_size			REG_ADDR8(0xc06)
#define reg_dma1_mode			REG_ADDR8(0xc07)

//rf rx dma
#define reg_dma2_addr			REG_ADDR16(0xc08)
#define reg_dma2_size			REG_ADDR8(0xc0a)
#define reg_dma2_mode			REG_ADDR8(0xc0b)

//rf tx dma
#define reg_dma3_addr			REG_ADDR16(0xc0c)
#define reg_dma3_size			REG_ADDR8(0xc0e)
#define reg_dma3_mode			REG_ADDR8(0xc0f)

#define reg_dma4_addr			REG_ADDR16(0xc10)
#define reg_dma4_size			REG_ADDR8(0xc12)
#define reg_dma4_mode			REG_ADDR8(0xc13)

#define reg_dma5_addr			REG_ADDR16(0xc14)
#define reg_dma5_size			REG_ADDR8(0xc16)
#define reg_dma5_mode			REG_ADDR8(0xc17)

//pwm tx dma
#define reg_dma7_addr			REG_ADDR16(0xc18)
#define reg_dma7_size			REG_ADDR8(0xc1a)
#define reg_dma7_mode			REG_ADDR8(0xc1b)


#define reg_dma_t_addr			REG_ADDR16(0xc1c)
#define reg_dma_t_size			REG_ADDR8(0xc1e)

#define reg_dma_size(v)			REG_ADDR8(0xc02+4*v)

//  The default channel assignment
#define reg_dma_uart_rx_addr	reg_dma0_addr
#define reg_dma_uart_rx_size	reg_dma0_size
#define reg_dma_uart_rx_mode	reg_dma0_mode

#define reg_dma_uart_tx_addr	reg_dma1_addr
#define reg_dma_uart_tx_size	reg_dma1_size
#define reg_dma_uart_tx_mode	reg_dma1_mode

#define reg_dma_rf_rx_addr		reg_dma2_addr
#define reg_dma_rf_rx_size		reg_dma2_size
#define reg_dma_rf_rx_mode		reg_dma2_mode

#define reg_dma_rf_tx_addr		reg_dma3_addr
#define reg_dma_rf_tx_size		reg_dma3_size
#define reg_dma_rf_tx_mode		reg_dma3_mode

#define reg_dma_pwm_addr		reg_dma7_addr
#define reg_dma_pwm_size		reg_dma7_size
#define reg_dma_pwm_mode		reg_dma7_mode



#define reg_dma_chn_en			REG_ADDR8(0xc20)
#define reg_dma_chn_irq_msk		REG_ADDR8(0xc21)
#define reg_dma_tx_rdy0			REG_ADDR8(0xc24)
#define reg_dma_tx_rdy1			REG_ADDR8(0xc25)
#define reg_dma_rx_rdy0			REG_ADDR8(0xc26)
#define reg_dma_rx_rdy1			REG_ADDR8(0xc27)

#define reg_dma_irq_status		reg_dma_rx_rdy0

enum{
	FLD_DMA_CHN0 =	BIT(0),		FLD_DMA_CHN_UART_RX =	BIT(0),
	FLD_DMA_CHN1 =	BIT(1),		FLD_DMA_CHN_UART_TX =	BIT(1),
	FLD_DMA_CHN2 =	BIT(2),		FLD_DMA_CHN_RF_RX =		BIT(2),
	FLD_DMA_CHN3 =	BIT(3),		FLD_DMA_CHN_RF_TX =		BIT(3),
	FLD_DMA_CHN4 =	BIT(4),		FLD_DMA_CHN_AES_OUT =  BIT(4),
	FLD_DMA_CHN5 =	BIT(5),     FLD_DMA_CHN_AES_IN =  BIT(5),
	FLD_DMA_CHN7 =	BIT(7),		FLD_DMA_CHN_PWM  	 =	BIT(7),
	FLD_DMA_CHN8 =  BIT(8)|BIT(0),FLD_DMA_CHN_SPI_RX = BIT(8)|BIT(0),
	FLD_DMA_CHN9 =  BIT(8)|BIT(1),FLD_DMA_CHN_SPI_TX = BIT(8)|BIT(1),
#if(MCU_CORE_B80B)
	FLD_DMA_CHN10 = BIT(8)|BIT(2), FLD_DMA_CHN_UART1_RX = BIT(8)|BIT(2),
	FLD_DMA_CHN11 = BIT(8)|BIT(3), FLD_DMA_CHN_UART1_TX = BIT(8)|BIT(3),
#endif
};

typedef enum {
    FLD_DMA_IRQ_UART_RX  = BIT(0),
    FLD_DMA_IRQ_UART_TX  = BIT(1),
    FLD_DMA_IRQ_RF_RX    = BIT(2),
    FLD_DMA_IRQ_RF_TX    = BIT(3),
    FLD_DMA_IRQ_AES_DECO = BIT(4),
    FLD_DMA_IRQ_AES_CODE = BIT(5),
    FLD_DMA_IRQ_PWM		 = BIT(7),
    FLD_DMA_IRQ_ALL      = 0xff,
} IRQ_DMAIrqTypeDef;


#define reg_dma_rx_rptr			REG_ADDR8(0xc28)
#define reg_dma_rx_wptr			REG_ADDR8(0xc29)

#define reg_dma_tx_rptr			REG_ADDR8(0xc2a)
#define reg_dma_tx_wptr			REG_ADDR8(0xc2b)
#define reg_dma_tx_fifo			REG_ADDR16(0xc2c)
enum{
	FLD_DMA_RPTR_CLR =			BIT(4),
	FLD_DMA_RPTR_NEXT =			BIT(5),
	FLD_DMA_RPTR_SET =			BIT(6),
};

#if(MCU_CORE_B80B)
#define reg_rf_multi_dma_tx_wptr(i)			REG_ADDR8(0xc30+i)
#define reg_rf_multi_dma_tx_rptr(i)			REG_ADDR8(0xc38+i)//This definition is used for the multiceiver function of RF.
															  //The "i" is represent different pipe id.
#endif

#define reg_dma0_addrHi			REG_ADDR8(0xc40)
#define reg_dma1_addrHi			REG_ADDR8(0xc41)
#define reg_dma2_addrHi			REG_ADDR8(0xc42)
#define reg_dma3_addrHi			REG_ADDR8(0xc43)
#define reg_dma4_addrHi			REG_ADDR8(0xc44)
#define reg_dma5_addrHi			REG_ADDR8(0xc45)
#define reg_dma_ta_addrHi		REG_ADDR8(0xc46)
#define reg_dma_a3_addrHi		REG_ADDR8(0xc47)
#define reg_dma7_addrHi			REG_ADDR8(0xc48)

#define reg_dma8_addrhh			REG_ADDR8(0xc49)
#define reg_dma8_addrh			REG_ADDR8(0xc4d)
#define reg_dma8_addrl			REG_ADDR8(0xc4c)
#define reg_dma8_mode			REG_ADDR8(0xc4f)


#define reg_dma9_addrhh			REG_ADDR8(0xc4a)
#define reg_dma9_addrh			REG_ADDR8(0xc51)
#define reg_dma9_addrl			REG_ADDR8(0xc50)
#define reg_dma9_mode			REG_ADDR8(0xc53)
#define reg_dma9_size			REG_ADDR8(0xc66)
#if(MCU_CORE_B80B)
#define reg_dma10_addrhh		REG_ADDR8(0xc68)
#define reg_dma10_addrh			REG_ADDR8(0xc6d)
#define reg_dma10_addrl			REG_ADDR8(0xc6c)
#define reg_dma10_mode			REG_ADDR8(0xc6f)
#define reg_dma10_size			REG_ADDR8(0xc6e)

#define reg_dma11_addrhh		REG_ADDR8(0xc69)
#define reg_dma11_addrh			REG_ADDR8(0xc71)
#define reg_dma11_addrl			REG_ADDR8(0xc70)
#define reg_dma11_mode			REG_ADDR8(0xc73)
#define reg_dma11_size			REG_ADDR8(0xc72)

#endif
#define reg_dma9_burst_size		REG_ADDR8(0xc5c)
enum{
	FLD_DMA_SRB_BURST_SIZE_CH7 =	BIT_RNG(2,3)
};

#define reg_dma_chn_en_h			REG_ADDR8(0xc54)
#define reg_dma_chn_irq_msk_h		REG_ADDR8(0xc55)

#define reg_dma_rdy0_h		  REG_ADDR8(0xc56)
#define reg_dma_rdy1_h		  REG_ADDR8(0xc57)
#define reg_dma_rx_rdy0_h	  REG_ADDR8(0xc58)
#define reg_dma_rx_rdy1_h	  REG_ADDR8(0xc59)

#define reg_dma_irq_status_h		reg_dma_rx_rdy0_h
enum{
	FLD_DMA_READY_9 		=   BIT(1),
};
/*******************************      linklayer registers: 0xf00      ******************************/

#define reg_rf_ll_ctrl_0		REG_ADDR8(0xf02)

#define reg_rf_ll_ctrl_1		REG_ADDR8(0xf03)
enum{
	FLD_RF_FSM_TIMEOUT_EN 		=   BIT(0),
	FLD_RF_RX_FIRST_TIMEOUT_EN	=   BIT(1),
	FLD_RF_RX_TIMEOUT_EN		=   BIT(2),
	FLD_RF_CRC_2_EN 			=   BIT(3),

	//BLE mode
	FLD_RF_BRX_SN_INIT	 		=   BIT(4),
	FLD_RF_BRX_NESN_INIT	 	=   BIT(5),
	FLD_RF_BTX_SN_INIT	 		=   BIT(6),
	FLD_RF_BTX_NESN_INIT	 	=   BIT(7),
};

#define  	FSM_TIMEOUT_ENABLE 	 			( reg_rf_ll_ctrl_1 |= FLD_RF_FSM_TIMEOUT_EN )
#define  	FSM_TIMEOUT_DISABLE				( reg_rf_ll_ctrl_1 &= (~FLD_RF_FSM_TIMEOUT_EN) )
#define		FSM_RX_FIRST_TIMEOUT_ENABLE		( reg_rf_ll_ctrl_1 |= FLD_RF_RX_FIRST_TIMEOUT_EN )
#define 	FSM_RX_FIRST_TIMEOUT_DISABLE	( reg_rf_ll_ctrl_1 &= (~FLD_RF_RX_FIRST_TIMEOUT_EN) )

#define reg_rf_rx_timeout		REG_ADDR16(0xf0a)

#define reg_rf_ll_ctrl_2		REG_ADDR8(0xf15)
enum{
	FLD_RF_R_TXCHN_MAN            =	BIT_RNG(0,2),
	FLD_RF_R_NOACK_RECENT_EN      =	BIT(3),
	FLD_RF_R_TXCHN_MAN_EN         =	BIT(4),
	FLD_RF_R_NOACK_REV_EN         =	BIT(5),
	FLD_RF_R_RXIRQ_REPORT_ALL     =	BIT(6),
	FLD_RF_R_REP_SN_PID_EN        =	BIT(7),
};

#define reg_rf_ll_ctrl_3		REG_ADDR8(0xf16)
enum{
	FLD_RF_TX_EN_DLY_EN = 		BIT(0),
	FLD_RF_PLL_RST_EN	=       BIT(1),
	FLD_RF_CMD_SCHEDULE_EN	=   BIT(2),
	FLD_RF_R_TX_EN_DLY =		BIT_RNG(4,7),
};


#define reg_rf_irq_mask			REG_ADDR16(0xf1c)
#define reg_rf_irq_status		REG_ADDR16(0xf20)
#define reg_rf_fsm_timeout		REG_ADDR32(0xf2c)

#define		CLEAR_ALL_RFIRQ_STATUS   ( reg_rf_irq_status = 0xffff )

typedef enum{
	FLD_RF_IRQ_RX = 			BIT(0),
	FLD_RF_IRQ_TX =				BIT(1),
	FLD_RF_IRQ_RX_TIMEOUT =		BIT(2),
	FLD_RF_IRQ_RX_FIFO_FULL =	BIT(3),
	FLD_RF_IRQ_RX_CRC_2 =		BIT(4),
	FLD_RF_IRQ_CMD_DONE  =		BIT(5),
	FLD_RF_IRQ_FSM_TIMEOUT  =	BIT(6),
	FLD_RF_IRQ_TX_RETRYCNT  =	BIT(7),
	FLD_RF_IRQ_TX_DS =          BIT(8),
    FLD_RF_IRQ_RX_DR =          BIT(9),
	FLD_RF_IRQ_FIRST_TIMEOUT =	BIT(10),
	FLD_RF_IRQ_INVALID_PID =    BIT(11),
	FLD_RF_IRQ_STX_TIMEOUT =    BIT(12),
	FLD_RF_IRQ_WIFI_DENY   =    BIT(13),
	FLD_RF_IRQ_ALL =            0X3FFF,
}rf_irq_e;
#define reg_rf_ll_wifi_coex		REG_ADDR8(0xf30)

enum{
	FLD_RF_COEX_EN			=	BIT(0),
	FLD_RF_COEX_WF_DN_POL	=	BIT(1),
	FLD_RF_COEX_STATUS		=	BIT(2),
	FLD_RF_COEX_TRX_POL		=	BIT(3),
	FLD_RF_TRX_PRIO			=	BIT(4),
	FLD_RF_TX_PRIO			=	BIT(5),
	FLD_RF_RX_PRIO			=	BIT(6),
};
/*******************************      aura registers: 0x1200      ******************************/
#define REG_AURA_BASE            0x1200
#define reg_rf_tx_flex_mi_0      REG_ADDR8(REG_AURA_BASE+0x1c)
enum
{
	FLD_RF_TX_MI_FRACTION     = BIT_RNG(0,5),
	FLD_RF_TX_MI_INTEGER_1    = BIT_RNG(6,7),
};
#define reg_rf_tx_flex_mi_1     REG_ADDR8(REG_AURA_BASE+0x1d)
enum
{
	FLD_RF_TX_MI_INTEGER_2   = BIT(0),
};

#define reg_rf_rx_flex_mi_0      REG_ADDR8(REG_AURA_BASE+0x1e)
enum
{
	FLD_RF_RX_MI_FRACTION     = BIT_RNG(0,5),
	FLD_RF_RX_MI_INTEGER_1    = BIT_RNG(6,7),
};
#define reg_rf_rx_flex_mi_1     REG_ADDR8(REG_AURA_BASE+0x1f)
enum
{
	FLD_RF_RX_MI_INTEGER_2   = BIT(0),
};

#define		reg_rf_mode_cfg_tx1_0		REG_ADDR8(REG_AURA_BASE+0x22)
enum
{
	FLD_RF_BLE_MODE_TX			      =	BIT(0),
	FLD_RF_VCO_TRIM_KVM			      = BIT_RNG(1,3),
	FLD_RF_HPMC_EXP_DIFF_COUNT_L	  = BIT_RNG(4,7),
};

/********************************************************************************************
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 *****|								Aanlog  Register Table  						   |*****
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 ********************************************************************************************/

/*******************************      analog registers(3v3): 0x00      ***************************/

#define areg_06_pll_bg			        0x06
enum{
	FLD_PLL_BG_POWER_DOWN = BIT(4),      //    	1: Power down;     0: Power up
};

#define areg_0e_pa0_pa3_pull			0x0E
#define areg_0f_pa4_pa7_pull			0x0F
#define areg_10_pb0_pb3_pull			0x10
#define areg_11_pb4_pb7_pull			0x11
#define areg_12_pc0_pc3_pull			0x12
#define areg_13_pc4_pc7_pull			0x13
#define areg_14_pd0_pd3_pull			0x14
#define areg_15_pd4_pd7_pull			0x15
#define areg_16_pf0_pf1_pull			0x16

/*******************************      analog registers(1v8): 0x80      ***************************/
#define areg_auto_pd_ctrl_0				0x2B
enum{
	FLD_AUTO_PD_PL_ALL			  = BIT(0),
	FLD_AUTO_PD_PL_DCDC 		  = BIT(1),

};

#define areg_auto_pd_ctrl_1				0x2C
enum{
	FLD_AUTO_PD_RC_32K			  = BIT(0),
	FLD_AUTO_PD_XTAL_32K 		  = BIT(1),
	FLD_AUTO_PD_XTAL_24M		  = BIT(2),
	FLD_AUTO_PD_LC_COMP		  	  = BIT(3),
	FLD_AUTO_PD_MISC			  = BIT(4),
	FLD_AUTO_PD_OTP			      = BIT(5),
    // <7> rsvd
};



#define areg_clk_setting				0x82
enum{
	FLD_DCCC_DOUBLER_POWER_DOWN	  = BIT(3),
	FLD_CLK_48M_TO_RX_EN 		  = BIT(4),
	FLD_CLK_48M_TO_DIG_EN 		  = BIT(5),
	FLD_CLK_24M_TO_SAR_EN 		  = BIT(6),
	FLD_CLK_48M_TO_CAL_DIG_MAN_EN = BIT(7),
};

#define areg_xo_setting				0x8a
enum{
	FLD_XO_CDAC_ANA	  	= BIT_RNG(0,5),
	FLD_XO_MODE_ANA 	= BIT(6),
	FLD_XO_CAP_OFF_ANA  = BIT(7),
};

/*******************************        analog registers(PGA)        ***************************/
#define codec_ana_cfg2				   0xe6
#define codec_ana_cfg3                 0xe7
#define codec_ana_cfg4                 0xe8

#define pga_audio_enable               0x34


#if defined(__cplusplus)
}
#endif





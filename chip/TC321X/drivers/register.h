/********************************************************************************************************
 * @file    register.h
 *
 * @brief   This is the header file for TC321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/*******************************      spi registers: 0x20      ******************************/

#define reg_spi_data            REG_ADDR8(0x20)
#define reg_spi_ctrl            REG_ADDR8(0x21)
enum{
    FLD_SPI_CS                  = BIT(0),
    FLD_SPI_MASTER_MODE_EN      = BIT(1),
    FLD_SPI_DATA_OUT_DIS        = BIT(2),
    FLD_SPI_RD                  = BIT(3),
    FLD_SPI_ADDR_AUTO_ADD       = BIT(4),
    FLD_SPI_SHARE_MODE          = BIT(5),
    FLD_SPI_BUSY                = BIT(6),
};

#define reg_spi_sp              REG_ADDR8(0x22)
enum{
    FLD_MSPI_CLK =              BIT_RNG(0,6),
    FLD_SPI_ENABLE =            BIT(7),
};

#define reg_spi_inv_clk         REG_ADDR8(0x23) //spi supports four modes
enum {
    FLD_SPI_MODE_WORK_MODE =    BIT_RNG(0,1),
};

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

/*******************************    sc-registers: 0x40     ******************************/

#define reg_rst3                REG_ADDR8(0x5e)
enum{
	FLD_RST3_UART1  =   BIT(0),
};

#define reg_rst			    	REG_ADDR32(0x60)
#define reg_rst0				REG_ADDR8(0x60)
enum{
	FLD_RST0_SPI = 				BIT(0),
	FLD_RST0_I2C = 				BIT(1),
	FLD_RST0_UART0 = 			BIT(2),
	FLD_RST0_PWM = 				BIT(4),
	FLD_RST0_QDEC =				BIT(5),
	FLD_RST0_IR_LEARN =		    BIT(6),
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
	FLD_RST2_UART2 =			BIT(6),

};


#define reg_clk_sel				REG_ADDR8(0x66)
enum{
	FLD_SCLK_DIV = 				BIT_RNG(0,4),
	FLD_SCLK_SEL =				BIT_RNG(5,6),
	FLD_SCLK_HS_SEL =			BIT(7),
};

#define reg_i2s_step            REG_ADDR8(0x67)
enum{
    FLD_I2S_STEP                = BIT_RNG(0,6),
    FLD_I2S_CLK_EN              = BIT(7),
};

#define reg_i2s_mod             REG_ADDR8(0x68)

#define reg_codec_clk_step      REG_ADDR8(0x6c)
enum{
    FLD_DMIC_STEP               = BIT_RNG(0,6),
    FLD_DMIC_SEL                = BIT(7),
};

#define reg_codec_clk_mod       REG_ADDR8(0x6d)

#define reg_wakeup_en			REG_ADDR8(0x6e)
enum{
	FLD_WAKEUP_SRC_QDEC    = BIT(0),
	FLD_WAKEUP_SRC_KS      = BIT(1), /**< keyscan. */
	FLD_WAKEUP_SRC_RST_SYS = BIT(7),
};

#define reg_pwdn_en			    REG_ADDR8(0x6f)
enum
{
	FLD_PWDN_CTRL_REBOOT = BIT(5),
	FLD_PWDN_CTRL_SLEEP  = BIT(7),
};

#define reg_trng_data			REG_ADDR8(0x76)

#define reg_probe_clk_sel		REG_ADDR8(0x75)

#define reg_mcu_wakeup_mask		REG_ADDR32(0x78)


/*******************************      7816 registers: 0x7b      ******************************/
/*******************************      78161 registers: 0x5d      ******************************/
#define reg_7816_clk_div(i)		REG_ADDR8((i==0) ? (0x7b):(0x5d))


/*******************************      uart0 registers: 0x90      ******************************/
/*******************************      uart1 registers: 0xc0      ******************************/


#define reg_uart_data_buf0(i) REG_ADDR8(0x90 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

#define reg_uart_data_buf1(i) REG_ADDR8(0x91 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

#define reg_uart_data_buf2(i) REG_ADDR8(0x92 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

#define reg_uart_data_buf3(i) REG_ADDR8(0x93 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

#define reg_uart_data_buf(i, j) REG_ADDR8(0x90 + ((i) == 2 ? 0x3C8 : 0x30) * (i) + j) // i = 0~3

#define reg_uart_clk_div(i) REG_ADDR16(0x94 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_CLK_DIV = BIT_RNG(0, 14),
	FLD_UART_CLK_DIV_EN = BIT(15)
};

#define reg_uart_ctrl0(i) REG_ADDR8(0x96 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_BPWC = BIT_RNG(0, 3),
	FLD_UART_RX_DMA_EN = BIT(4),
	FLD_UART_TX_DMA_EN = BIT(5),
	FLD_UART_RX_IRQ_EN = BIT(6),
	FLD_UART_TX_IRQ_EN = BIT(7),
};

#define reg_uart_ctrl1(i) REG_ADDR8(0x97 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_CTRL1_CTS_SELECT = BIT(0),
	FLD_UART_CTRL1_CTS_EN = BIT(1),
	FLD_UART_CTRL1_PARITY_EN = BIT(2),
	FLD_UART_CTRL1_PARITY_POLARITY = BIT(3), // 1:odd parity   0:even parity
	FLD_UART_CTRL1_STOP_BIT = BIT_RNG(4, 5),
	FLD_UART_CTRL1_TTL = BIT(6),
	FLD_UART_CTRL1_LOOPBACK = BIT(7),
};

#define reg_uart_ctrl2(i) REG_ADDR8(0x98 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_CTRL2_RTS_TRIG_LVL = BIT_RNG(0, 3),
	FLD_UART_CTRL2_RTS_PARITY = BIT(4),
	FLD_UART_CTRL2_RTS_MANUAL_VAL = BIT(5),
	FLD_UART_CTRL2_RTS_MANUAL_EN = BIT(6),
	FLD_UART_CTRL2_RTS_EN = BIT(7),
};

#define reg_uart_ctrl3(i) REG_ADDR8(0x99 + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_RX_IRQ_TRIG_LEV = BIT_RNG(0, 3),
	FLD_UART_TX_IRQ_TRIG_LEV = BIT_RNG(4, 7),
};

#define reg_uart_rx_timeout0(i) REG_ADDR8(0x9a + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_TIMEOUT_BW = BIT_RNG(0, 7),
};

#define reg_uart_rx_timeout1(i) REG_ADDR8(0x9b + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_TIMEOUT_MUL = BIT_RNG(0, 1),
	FLD_URAT_RXDONE_RTS_EN = BIT(2),
	FLD_URAT_RXTIMEOUT_RTS_EN = BIT(3),
	FLD_UART_MASK_RXDONE_IRQ = BIT(4),
	FLD_UART_P7816_EN = BIT(5),
	FLD_UART_MASK_TXDONE_IRQ = BIT(6),
	FLD_UART_MASK_ERR_IRQ = BIT(7),
};

#define reg_uart_buf_cnt(i) REG_ADDR8(0x9c + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_RX_BUF_CNT = BIT_RNG(0, 3),
	FLD_UART_TX_BUF_CNT = BIT_RNG(4, 7),
};

#define reg_uart_status0(i) REG_ADDR8(0x9d + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_RBCNT = BIT_RNG(0, 2),
	FLD_UART_IRQ_FLAG = BIT(3),
	FLD_UART_WBCNT = BIT_RNG(4, 6),
	FLD_URAT_CLEAR_RXDONE_FLAG = BIT(4),
	FLD_UART_CLEAR_RX_FLAG = BIT(6),
	FLD_UART_RX_ERR_FLAG = BIT(7),
};

#define reg_uart_status1(i) REG_ADDR8(0x9e + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum
{
	FLD_UART_TX_DONE = BIT(0),
	FLD_UART_TX_BUF_IRQ = BIT(1),
	FLD_UART_RX_DONE = BIT(2),
	FLD_UART_RX_BUF_IRQ = BIT(3),
	FLD_UART_RXDONE_IRQ = BIT(4),
	FLD_UART_TIMEOUT_EN = BIT(5),
	FLD_UART_AUTO_RXCLR_EN = BIT(6),
	FLD_UART_NDMA_RXDONE_EN = BIT(7),
};

#define reg_uart_state(i) REG_ADDR8(0x9f + ((i) == 2 ? 0x3C8 : 0x30) * (i))

enum{
	FLD_UART_TSTATE_I 	     =  BIT_RNG(0,2),
	FLD_UART_RSTATE_I	     =  BIT_RNG(4,7),
};

/*******************************  (il) ir learn registers: 0xa0      ****************************/

#define reg_il_run           	REG_ADDR8(0xa0)

#define reg_il_cfg0          	REG_ADDR8(0xa1)
enum{
	FLD_IL_INPUT_SEL            = BIT(0),
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

#define reg_il_cycle_0			REG_ADDR32(0xac)


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
#define reg_gpio_pa_oe			REG_ADDR8(GPIO_BASE_ADDR+0x02)
#define reg_gpio_pa_sr			REG_ADDR8(GPIO_BASE_ADDR+0x03)
#define reg_gpio_pa_ds0			REG_ADDR8(GPIO_BASE_ADDR+0x04)
#define reg_gpio_pa_ds1			REG_ADDR8(GPIO_BASE_ADDR+0x05)
#define reg_gpio_pa_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x06)
#define reg_gpio_pa_out_set  	REG_ADDR8(GPIO_BASE_ADDR+0xC0)
#define reg_gpio_pa_out_clear	REG_ADDR8(GPIO_BASE_ADDR+0xC1)

#define reg_gpio_pb_in			REG_ADDR8(GPIO_BASE_ADDR+0x08)
#define reg_gpio_pb_ie			REG_ADDR8(GPIO_BASE_ADDR+0x09)  //B0~B3
#define areg_gpio_pb_ie			0xa0						    //B4~B7
#define reg_gpio_pb_oe			REG_ADDR8(GPIO_BASE_ADDR+0x0a)
#define areg_gpio_pb_sr			0xa1						    //B4~B7
#define areg_gpio_pb_ds0		0xa2						    //B4~B7
#define areg_gpio_pb_ds1		0xa3						    //B4~B7
#define reg_gpio_pb_sr			REG_ADDR8(GPIO_BASE_ADDR+0x0b)
#define reg_gpio_pb_ds0			REG_ADDR8(GPIO_BASE_ADDR+0x0c)
#define reg_gpio_pb_ds1			REG_ADDR8(GPIO_BASE_ADDR+0x0d)
#define reg_gpio_pb_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x0e)
#define reg_gpio_pb_out_set  	REG_ADDR8(GPIO_BASE_ADDR+0xC4)
#define reg_gpio_pb_out_clear	REG_ADDR8(GPIO_BASE_ADDR+0xC5)

#define reg_gpio_pc_in			REG_ADDR8(GPIO_BASE_ADDR+0x10)
#define areg_gpio_pc_ie			0xa4
#define reg_gpio_pc_oe			REG_ADDR8(GPIO_BASE_ADDR+0x12)
#define areg_gpio_pc_sr			0xa5
#define areg_gpio_pc_ds0		0xa6
#define areg_gpio_pc_ds1		0xa7
#define reg_gpio_pc_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x16)
#define reg_gpio_pc_out_set  	REG_ADDR8(GPIO_BASE_ADDR+0xC8)
#define reg_gpio_pc_out_clear	REG_ADDR8(GPIO_BASE_ADDR+0xC9)

#define reg_gpio_pd_in			REG_ADDR8(GPIO_BASE_ADDR+0x18)
#define reg_gpio_pd_ie			REG_ADDR8(GPIO_BASE_ADDR+0x19)
#define reg_gpio_pd_oe			REG_ADDR8(GPIO_BASE_ADDR+0x1a)
#define reg_gpio_pd_sr			REG_ADDR8(GPIO_BASE_ADDR+0x1b)
#define reg_gpio_pd_ds0			REG_ADDR8(GPIO_BASE_ADDR+0x1c)
#define reg_gpio_pd_ds1			REG_ADDR8(GPIO_BASE_ADDR+0x1d)
#define reg_gpio_pd_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x1e)
#define reg_gpio_pd_out_set  	REG_ADDR8(GPIO_BASE_ADDR+0xCC)
#define reg_gpio_pd_out_clear	REG_ADDR8(GPIO_BASE_ADDR+0xCD)

#define reg_gpio_pe_in			REG_ADDR8(GPIO_BASE_ADDR+0x20)
#define reg_gpio_pe_ie			REG_ADDR8(GPIO_BASE_ADDR+0x21)
#define reg_gpio_pe_oe			REG_ADDR8(GPIO_BASE_ADDR+0x22)
#define reg_gpio_pe_sr			REG_ADDR8(GPIO_BASE_ADDR+0x23)
#define reg_gpio_pe_ds0			REG_ADDR8(GPIO_BASE_ADDR+0x24)
#define reg_gpio_pe_ds1			REG_ADDR8(GPIO_BASE_ADDR+0x25)
#define reg_gpio_pe_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x26)
#define reg_gpio_pe_out_set  	REG_ADDR8(GPIO_BASE_ADDR+0xD0)
#define reg_gpio_pe_out_clear	REG_ADDR8(GPIO_BASE_ADDR+0xD1)

#define reg_gpio_pf_in			REG_ADDR8(GPIO_BASE_ADDR+0x28)
#define reg_gpio_pf_ie			REG_ADDR8(GPIO_BASE_ADDR+0x29)
#define reg_gpio_pf_oe			REG_ADDR8(GPIO_BASE_ADDR+0x2a)
#define reg_gpio_pf_sr			REG_ADDR8(GPIO_BASE_ADDR+0x2b)
#define reg_gpio_pf_ds0			REG_ADDR8(GPIO_BASE_ADDR+0x2c)
#define reg_gpio_pf_ds1			REG_ADDR8(GPIO_BASE_ADDR+0x2d)
#define reg_gpio_pf_gpio		REG_ADDR8(GPIO_BASE_ADDR+0x2e)
#define reg_gpio_pf_out_set  	REG_ADDR8(GPIO_BASE_ADDR+0xD4)
#define reg_gpio_pf_out_clear	REG_ADDR8(GPIO_BASE_ADDR+0xD5)

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

#define areg_gpio_ds0(i)            0xa2+(((i)>>9)<<2)                             //B4~B7 C0~C7
#define areg_gpio_ds1(i)            0xa3+(((i)>>9)<<2)                             //B4~B7 C0~C7

#define reg_gpio_in(i)				REG_ADDR8(GPIO_BASE_ADDR+((i>>8)<<3))
#define reg_gpio_ie(i)				REG_ADDR8(GPIO_BASE_ADDR+0x01+((i>>8)<<3))
#define reg_gpio_oe(i)				REG_ADDR8(GPIO_BASE_ADDR+0x02+((i>>8)<<3))
#define reg_gpio_sr(i)				REG_ADDR8(GPIO_BASE_ADDR+0x03+((i>>8)<<3))
#define reg_gpio_ds0(i)				REG_ADDR8(GPIO_BASE_ADDR+0x04+((i>>8)<<3))
#define reg_gpio_ds1(i)				REG_ADDR8(GPIO_BASE_ADDR+0x05+((i>>8)<<3))
#define reg_gpio_func(i)			REG_ADDR8(GPIO_BASE_ADDR+0x06+((i>>8)<<3))
#define reg_gpio_pol(i)	            REG_ADDR8(GPIO_BASE_ADDR+0x07+((i>>8)<<3)) 

#define reg_gpio_out_set_clear(i)   REG_ADDR16(GPIO_BASE_ADDR+0xc0+(((i)>>8)<<2))
#define reg_gpio_out_set(i)         REG_ADDR8(GPIO_BASE_ADDR+0xc0+(((i)>>8)<<2))
#define reg_gpio_out_clear(i)       REG_ADDR8(GPIO_BASE_ADDR+0xc1+(((i)>>8)<<2))
#define reg_gpio_out_toggle(i)      REG_ADDR8(GPIO_BASE_ADDR+0xc2+(((i)>>8)<<2))

#define reg_gpio_func_mux(i)        REG_ADDR8(GPIO_BASE_ADDR+0x48+(((i&0x500) == 0x500) ? ((((i>>8)-1)<<3)+4) : ((i>>8)<<3))+((i&0x80) ? 7 : 0)+((i&0x40) ? 6 : 0)+((i&0x20) ? 5 : 0)+((i&0x10) ? 4 : 0) +((i&0x8) ? 3 : 0)+((i&0x4) ? 2 : 0)+((i&0x2) ? 1 : 0))

#define reg_gpio_irq_risc3_risc7_en(i,j)    REG_ADDR8(0x530 + ((i)>>8)+((j)<<3))
#define reg_gpio_irq_risc0_en(i)            REG_ADDR8(0x530 + (i >> 8))
#define reg_gpio_irq_risc1_en(i)            REG_ADDR8(0x538 + (i >> 8))
#define reg_gpio_irq_risc2_en(i)            REG_ADDR8(0x540 + (i >> 8))
#define reg_gpio_irq_risc3_en(i)            REG_ADDR8(0x598 + (i >> 8))

#define reg_comb_irq				REG_ADDR8(0x5da)
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

enum{
	FLD_IRQ_MUX_EN0                      = BIT(0),
	FLD_IRQ_MUX_EN1                      = BIT(1),
	FLD_IRQ_MUX_EN2                      = BIT(2),
};
#define reg_gpio_irq_from_pad     REG_ADDR8(0x56f)
#define reg_gpio_irq_pad_mask     REG_ADDR8(0x576)
#define reg_gpio_irq_lvl          REG_ADDR8(0x577)
#define reg_gpio_irq_status       REG_ADDR8(0x5db)
enum{
  FLD_IRQ_RISC0     = BIT(0),
  FLD_IRQ_RISC1     = BIT(1),
  FLD_IRQ_RISC2     = BIT(2),
  FLD_IRQ_RISC3     = BIT(3),
};


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
#define reg_irq_mask            REG_ADDR32(0x640)
#define reg_irq_pri             REG_ADDR32(0x644)
#define reg_irq_src             REG_ADDR32(0x648)
typedef enum{
    /*irq0~irq15 LEVEL*/
    FLD_IRQ_TMR0_EN =           BIT(0),
    FLD_IRQ_TMR1_EN =           BIT(1),
    FLD_IRQ_TMR2_EN =           BIT(2),

    FLD_IRQ_DMA_EN =            BIT(4),
    FLD_IRQ_DAM_FIFO_EN =       BIT(5),//FLD_IRQ_DAM_FIFO_EN is the master interrupt enable bit for dc fifo interrupts (FLD_R_MASK) and audio fifo0/1 interrupts (FLD_AUD_DFIFO0_L_INT/FLD_AUD_DFIFO0_H_INT/FLD_AUD_DFIFO1_H_INT).
    FLD_IRQ_UART_EN     =       BIT(6),
    FLD_IRQ_MIX_CMD_EN =        BIT(7), FLD_IRQ_HOST_CMD_EN  = BIT(7),FLD_IRQ_SPI_EN =BIT(7),FLD_IRQ_SW_EN = BIT(7),FLD_IRQ_IR_LEARN_EN = BIT(7),FLD_IRQ_KS_CMD_EN = BIT(7),FLD_IRQ_SYSTEM_TIMER_OR=BIT(7),//< MIX = I2C/QDEC/SW/IRLEARN/KEYSCAN/SPI/STIMER

    FLD_IRQ_UART1_EN =  BIT(11),
    FLD_IRQ_UART2_EN =  BIT(12),

    FLD_IRQ_ZB_RT_EN =          BIT(13),
    FLD_IRQ_SW_PWM_EN =         BIT(14), FLD_IRQ_PWM_EN = BIT(14),//only for pwm now
    FLD_IRQ_GPIO3_LEVEL_EN =    BIT(15),

    /*irq16~irq23 EDGE*/
    FLD_IRQ_GPIO_RISC3_EN =     BIT(18),
    FLD_IRQ_PM_EN =             BIT(19),
    FLD_IRQ_SYSTEM_TIMER =      BIT(20),
    FLD_IRQ_GPIO_RISC0_EN =     BIT(21),
    FLD_IRQ_GPIO_RISC1_EN =     BIT(22),
    FLD_IRQ_GPIO_RISC2_EN =     BIT(23),

    FLD_IRQ_EN =                BIT_RNG(24,31),
    FLD_IRQ_ALL           =     0XFFFFFFFF,
}irq_list_e;
#define reg_irq_en              REG_ADDR8(0x643)

/*******************************      system timer registers: 0x740      ******************************/

#define reg_system_tick			REG_ADDR32(0x740)
#define reg_system_irq_level	REG_ADDR32(0x744)
#define reg_system_irq_mask		REG_ADDR8(0x748)
typedef enum
{
	FLD_SYSTEM_IRQ_32K_CAL_MASK    = BIT(0), /**<
                                            When the current tick value of the system timer reaches the preset tick value,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                          */
    FLD_SYSTEM_READ_32K_DONE_MASK  = BIT(1), /**<
                                            When the system timer completes the calibration of 32k timer,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                          */
    FLD_SYSTEM_IRQ_MASK            = BIT(2),
} stimer_irq_mask_e;

#define reg_system_cal_irq			REG_ADDR8(0x749)
typedef enum
{
	FLD_SYSTEM_32K_CAL_IRQ          = BIT(0), /**<
	                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
	                                            The interrupt flag bit needs to be manually cleared.
	                                          */
	FLD_SYSTEM_READ_32K_DONE_IRQ    = BIT(1), /**<
	                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
	                                            The interrupt flag bit needs to be manually cleared.
	                                         <p>
	                                            In Non-dma mode,the received data is read by stimer_get_tracking_32k_value().
	                                          */
    FLD_SYSTEM_IRQ                  = BIT(2),
} stimer_irq_status_e;

#define reg_system_ctrl				REG_ADDR8(0x74a)
enum{
	FLD_SYSTEM_32K_WR_EN 		= 	BIT(0),
	FLD_SYSTEM_TIMER_MANUAL_EN 	= 	BIT(1),
	FLD_SYSTEM_TIMER_AUTO_EN 	= 	BIT(2),
	FLD_SYSTEM_32K_CAL_EN 		= 	BIT(3),
	FLD_SYSTEM_32K_CAL_MODE 	= 	BIT_RNG(4,7),
};

#define reg_system_status			REG_ADDR8(0x74b)
enum{
	FLD_SYSTEM_TIMER_STATUS 			=   BIT(0),
	FLD_SYSTEM_CMD_SYNC_TRIG 			=   BIT(1),
	FLD_SYSTEM_CMD_SET_TRIG 			=   BIT(2),
	FLD_SYSTEM_CMD_SYNC		            =   BIT(3),
	FLD_SYSTEM_RD_BUSY 			        =   BIT(6),

	FLD_SYSTEM_32K_TIMER_SYNC_TRIG		=   BIT(3),
	FLD_SYSTEM_STATE_MACHINE_STATUS		=   BIT_RNG(3,4),
	FLD_SYSTEM_32K_TIMER_UPDATE_RD 		=   BIT(5),
	FLD_SYSTEM_32K_TIMER_CLEAR_RD		=   BIT(5),
	FLD_SYSTEM_32K_TIMER_BUSY 			=   BIT(6),
};

#define reg_system_32k_tick_wt  	REG_ADDR32(0x74c)
#define reg_system_32k_tick_rd  	REG_ADDR32(0x750)
#define reg_system_32k_tick_cal  	REG_ADDR32(0x754)

/*******************************      pwm registers: 0x780      ******************************/

#define reg_pwm_enable			REG_ADDR8(0x780)
#define reg_pwm0_enable			REG_ADDR8(0x781)
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

/*******************************      audio registers: 0xa00      ******************************/
#define AUDIO_BASE                  (0xa00)

#define reg_audio_ctrl              REG_ADDR8(AUDIO_BASE)
enum{
    AUDIO_OUTPUT_OFF                = 0,
    FLD_AUDIO_MONO_MODE             = BIT(0),//1-mono mode audio output,0--stereo mode audio output  for SDM
    FLD_AUDIO_I2S_PLAYER_EN         = BIT(1),
    FLD_AUDIO_SDM_PLAYER_EN         = BIT(2),//1-enable SDM player, 0--disable SDM player
    FLD_AUDIO_ISO_PLAYER_EN         = BIT(3),
    FLD_AUDIO_I2S_RECORDER_EN       = BIT(4),
    FLD_AUDIO_I2S_INTERFACE_EN      = BIT(5),
    FLD_AUDIO_GRP_EN                = BIT(6),//1-enable GRP, 0--disable GRP
    FLD_AUDIO_HPF_EN                = BIT(7),
};

/**
 * BIT[0:6] audio sound volume.[0]:add a quarter;[1]add a half;[6:2]:shift left.
 * BIT[7] mute enable.
 */
#define reg_audio_vol               REG_ADDR8(AUDIO_BASE + 0x02)
enum{
    FLD_AUDIO_VOL                   = BIT_RNG(0,6),
    FLD_AUDIO_VOL_MUTE              = BIT(7),//1--mute, 0--normal
};

#define reg_pwm_ctrl                REG_ADDR8(AUDIO_BASE + 0x03)
enum{
    FLD_PWM_MULTIPLY2               = BIT(0),//1--not multiply 2 when PWM, 0--multiply2 when PWM
    FLD_PWM_ENABLE                  = BIT(1),//1--PWM, 0--not PWM
    FLD_LINER_INTERPOLATE_EN        = BIT(2),//1-linear interpolate,  0-delay interpolate
    FLD_LEFT_SHAPING_EN             = BIT(5),//1-left Shapping used,   0-left Shapping not used
    FLD_RIGHT_SHAPING_EN            = BIT(6),//1-right Shapping used,  0-right Shapping not used
};

#define reg_ascl_tune               REG_ADDR32(AUDIO_BASE + 0x04)
enum{
    TUNE_STEP                       = BIT_RNG(12,31),//block step_i[31:12]
};
#define reg_ascl_tune0              REG_ADDR8(AUDIO_BASE + 0x04)//[7:0] tune step_i for rate matching block

#define reg_ascl_tune1              REG_ADDR8(AUDIO_BASE + 0x05)
enum{
    FACTOR_TO_GN_I2S_CLK            = BIT_RNG(0,3),//factor to generate I2S clock
    STEPI_LOW_BITS                  = BIT_RNG(4,7),//low 4 bits of rate matching block step_i[3:0]
};
#define reg_ascl_tune2              REG_ADDR8(AUDIO_BASE + 0x06)
enum{
    STEPI_MID_BITS                  = BIT_RNG(0,7),// middle byte of rate matching block step_i[11:4]
};
#define reg_ascl_tune3              REG_ADDR8(AUDIO_BASE + 0x07)
enum{
    STEPI_HIGH_BITS                 = BIT_RNG(0,7),//high byte of rate matching block step_i[19:12]
};
#define reg_pn1_left                REG_ADDR8(AUDIO_BASE + 0x08)
enum{
    PN1_LEFT_CHN_BITS               = BIT_RNG(0,4),//bits used in pn1 of left channel, range from 0 to 16
    PN2_LEFT_CHN_EN                 = BIT(5),//1-pn2 of left enable, 0-pn2 of left disable
    PN1_LEFT_CHN_EN                 = BIT(6),//1-pn1 of left enable, 0-pn1 of left disable
};

#define reg_pn2_left                REG_ADDR8(AUDIO_BASE + 0x09)
enum{
    PN2_LEFT_CHN_BITS               = BIT_RNG(0,4),//bits used in pn2 of left channel, range from 0 to 16
    PN2_RIGHT_CHN_EN                = BIT(5),//1-pn2 of right enable,  0-pn2 of right disable
    PN1_RIGHT_CHN_EN                = BIT(6),//1-pn1 of right enable,  0-pn1 of right disable
};

#define reg_pn1_right               REG_ADDR8(AUDIO_BASE + 0x0a)
enum{
    PN1_RIGHT_CHN_BITS              = BIT_RNG(0,4),//bits used in pn1 of right channel, range from 0 to 16
    CLK2A_AUDIO_CLK_EN              = BIT(5),
    EXCHANGE_SDM_DATA_EN            = BIT(6),
};

#define reg_pn2_right               REG_ADDR8(AUDIO_BASE + 0x0b)
enum{
    PN2_RIGHT_CHN_BITS              = BIT_RNG(0,4),//bits used in pn2 of right channel, range from 0 to 16
    SDM_LEFT_CHN_CONST_EN           = BIT(5),//1-left channel use const value,0-left channel use pn
    SDM_RIGHT_CHN_CONST_EN          = BIT(6),//1-right channel use const value,0-right channel use pn
};

/*******************************      dfifo registers: 0xb00      ******************************/
#define DFIFO_AUDIO_BASE            (0xb00)
#define reg_dfifo0_addr             REG_ADDR16(DFIFO_AUDIO_BASE)
#define reg_dfifo0_size             REG_ADDR8(DFIFO_AUDIO_BASE + 0x02)
#define reg_dfifo0_addHi            REG_ADDR8(DFIFO_AUDIO_BASE + 0x03)  //default 0x04, no need set

#define reg_dfifo1_addr             REG_ADDR16(DFIFO_AUDIO_BASE + 0x04)
#define reg_dfifo1_size             REG_ADDR8(DFIFO_AUDIO_BASE + 0x06)
#define reg_dfifo1_addHi            REG_ADDR8(DFIFO_AUDIO_BASE + 0x07)  //default 0x04, no need set

#define reg_usb_mic_dat0            REG_ADDR16(0x1800)
#define reg_usb_mic_dat1            REG_ADDR16(0x1802)

#define reg_dfifo0_l_level          REG_ADDR8(DFIFO_AUDIO_BASE + 0x0c)  //dfifo0  low int threshold(wptr - rptr)
#define reg_dfifo0_h_level          REG_ADDR8(DFIFO_AUDIO_BASE + 0x0d)  //dfifo0 high int threshold(wptr - rptr)
#define reg_dfifo1_h_level          REG_ADDR8(DFIFO_AUDIO_BASE + 0x0e)  //dfifo1 high int threshold(wptr - rptr)

#define reg_dfifo_mode              REG_ADDR8(DFIFO_AUDIO_BASE + 0x10)
enum{
    FLD_AUD_DFIFO0_IN               = BIT(0),
    FLD_AUD_DFIFO1_IN               = BIT(1),
    FLD_AUD_DFIFO0_OUT              = BIT(3),
    FLD_AUD_DFIFO0_L_INT            = BIT(4),
    FLD_AUD_DFIFO0_H_INT            = BIT(5),
    FLD_AUD_DFIFO1_H_INT            = BIT(6),
};

#define reg_dfifo_ain               REG_ADDR8(DFIFO_AUDIO_BASE + 0x11)
enum{
    FLD_AUD_SAMPLE_TIME_CONFIG      = BIT_RNG(0,1),//change sampling point, set 2'b01
    FLD_AUD_FIFO0_INPUT_SELECT      = BIT_RNG(2,3),// 0:USB; 1:I2S; 2:16bit adc; 3:20bit adc
    FLD_AUD_FIFO1_INPUT_SELECT      = BIT_RNG(4,5),// 0:USB; 1:I2S; 2:16bit adc; 3:20bit adc
    FLD_AUD_MIC_LEFT_CHN_SELECT     = BIT(6),
    FLD_AUD_MIC_RIGHT_CHN_SELECT    = BIT(7),
};

enum{  //core_b11<3:2> <5:4> audio input select
    AUDIO_FIFO_INPUT_SELECT_USB     = 0x00,
    AUDIO_FIFO_INPUT_SELECT_I2S     = 0x01,
    AUDIO_FIFO_INPUT_SELECT_16BIT   = 0x02,
    AUDIO_FIFO_INPUT_SELECT_20BIT   = 0x03,
};

#define reg_mic_ctrl                REG_ADDR8(DFIFO_AUDIO_BASE + 0x12)
enum{
    FLD_AUD_MIC_VOL_CONTROL         = BIT_RNG(0,5),
    FLD_AUD_MIC_MONO_EN             = BIT(6),
    FLD_AUD_AMIC_DMIC_SELECT        = BIT(7),
};

#define reg_dfifo_irq_status        REG_ADDR8(DFIFO_AUDIO_BASE + 0x13)
enum{
    FLD_AUD_DFIFO0_IRQ0             = BIT(0), // w1c
    FLD_AUD_DFIFO0_IRQ1             = BIT(1), // w1c
    FLD_AUD_DFIFO1_IRQ2             = BIT(2), // w1c

    FLD_AUD_DFIFO0_L_IRQ            = BIT(4),
    FLD_AUD_DFIFO0_H_IRQ            = BIT(5),
    FLD_AUD_DFIFO1_M_IRQ            = BIT(6),
};
#define reg_dfifo0_rptr             REG_ADDR16(DFIFO_AUDIO_BASE + 0x14)
#define reg_dfifo0_wptr             REG_ADDR16(DFIFO_AUDIO_BASE + 0x16)

#define reg_dfifo1_rptr             REG_ADDR16(DFIFO_AUDIO_BASE + 0x18)
#define reg_dfifo1_wptr             REG_ADDR16(DFIFO_AUDIO_BASE + 0x1a)


#define reg_audio_wptr              reg_dfifo0_wptr

#define reg_dfifo0_num              REG_ADDR16(DFIFO_AUDIO_BASE + 0x20)
#define reg_dfifo1_num              REG_ADDR16(DFIFO_AUDIO_BASE + 0x24)

#define reg_dfifo_manual_mode       REG_ADDR8(DFIFO_AUDIO_BASE + 0x2c)
enum{
    FLD_DFIFO_MANUAL_MODE_EN        = BIT(0),
    FLD_AMIC_SAMP_CLK_NEG           = BIT(1),
};

#define reg_dfifo0_man_dat          REG_ADDR32(DFIFO_AUDIO_BASE + 0x30)

#define AUDIO_CIP_BASE              (0xb80)
#define reg_codec_config            REG_ADDR8(AUDIO_CIP_BASE)
enum{
    FLD_HPF_EN                      = BIT(0),
    FLD_CK_SEL                      = BIT(1),
    FLD_SFT_ZC                      = BIT(2),
    FLD_R_SEL_2M                    = BIT(3),
    FLD_DATI_SOTE_MUTE              = BIT(4),
    FLD_DATO_SOTE_MUTE              = BIT(5),
};

#define reg_dfifo_dec_ratio         REG_ADDR8(AUDIO_CIP_BASE + 0x0a)
enum{
    FLD_CLK_USB                     = BIT(0),
    FLD_CLK_SR                      = BIT_RNG(1,5),
    FLD_CLK_DIV2                    = BIT(6),
    FLD_CLK_EN                      = BIT(7),
};

#define reg_codec_dec_en            REG_ADDR8(AUDIO_CIP_BASE + 0x0b)
enum{
    FLD_AUD_CODEC_DEC_EN            = BIT(0),
};

/*******************************      dfifo dc registers: 0xb40      ******************************/
#define reg_dfifo_ba2            REG_ADDR16(0xb48)
#define reg_dfifo_ba2l            REG_ADDR8(0xb48)
#define reg_dfifo_ba2h            REG_ADDR8(0xb49)
#define reg_dfifo_depth           REG_ADDR8(0xb4a)
#define reg_dfifo_aidx2            REG_ADDR8(0xb4b)
enum{
	FLD_R_AIDX2	= BIT_RNG(0,2),
	FLD_R_DEC	= BIT_RNG(4,6),
};

#define reg_dfifo_dc_mode            REG_ADDR8(0xb50)
enum{
	FLD_R_EN_AIN2	= BIT(2),
	FLD_R_MASK	= BIT(7),
};

#define reg_dfifo_dc_clk_div        REG_ADDR8(0xb4c)
enum{
	FLD_DFIFO_DC_CLK_DIV	= BIT_RNG(0,6),
};

#define reg_op_mode                 REG_ADDR8(0xb4d)
enum{
	FLD_OP_MODE	= BIT_RNG(0,2),
};

#define reg_dfifon2                 REG_ADDR8(0xb4f)

#define reg_irq_fifo            REG_ADDR8(0xb53)
enum{
	FLD_R_IRQ	= BIT(3),
	FLD_FIFO2_MID	= BIT(7),
};

#define reg_dfifo_wptr2                 REG_ADDR16(0xb5e)
enum{
	FLD_DFIFO_WPTR2	= BIT_RNG(2,12),
};

/*******************************      dma registers: 0xc00      ******************************/
//uart rx
#define reg_dma0_addr           REG_ADDR16(0xc00)
#define reg_dma0_size           REG_ADDR8(0xc02)
#define reg_dma0_mode           REG_ADDR8(0xc03)
enum{
    FLD_DMA_WR_MEM =            BIT(0),
    FLD_DMA_PINGPONG_EN =       BIT(1),
    FLD_DMA_FIFO_EN =           BIT(2),
    FLD_DMA_AUTO_MODE =         BIT(3),
    FLD_DMA_READ_MODE =         BIT(4),
    FLD_DMA_BYTE_MODE =         BIT(5)
};

//uart tx
#define reg_dma1_addr           REG_ADDR16(0xc04)
#define reg_dma1_size           REG_ADDR8(0xc06)
#define reg_dma1_mode           REG_ADDR8(0xc07)

//rf rx dma
#define reg_dma2_addr           REG_ADDR16(0xc08)
#define reg_dma2_size           REG_ADDR8(0xc0a)
#define reg_dma2_mode           REG_ADDR8(0xc0b)

//rf tx dma
#define reg_dma3_addr           REG_ADDR16(0xc0c)
#define reg_dma3_size           REG_ADDR8(0xc0e)
#define reg_dma3_mode           REG_ADDR8(0xc0f)

#define reg_dma4_addr           REG_ADDR16(0xc10)
#define reg_dma4_size           REG_ADDR8(0xc12)
#define reg_dma4_mode           REG_ADDR8(0xc13)

#define reg_dma5_addr           REG_ADDR16(0xc14)
#define reg_dma5_size           REG_ADDR8(0xc16)
#define reg_dma5_mode           REG_ADDR8(0xc17)

//pwm tx dma
#define reg_dma7_addr           REG_ADDR16(0xc18)
#define reg_dma7_size           REG_ADDR8(0xc1a)
#define reg_dma7_mode           REG_ADDR8(0xc1b)

#define reg_dma_t_addr          REG_ADDR16(0xc1c)
#define reg_dma_t_size          REG_ADDR8(0xc1e)

#define reg_dma_size(v)         REG_ADDR8(0xc02+4*v)

//  The default channel assignment
#define reg_dma_uart_rx_addr    reg_dma0_addr
#define reg_dma_uart_rx_size    reg_dma0_size
#define reg_dma_uart_rx_mode    reg_dma0_mode

#define reg_dma_uart_tx_addr    reg_dma1_addr
#define reg_dma_uart_tx_size    reg_dma1_size
#define reg_dma_uart_tx_mode    reg_dma1_mode

#define reg_dma_rf_rx_addr      reg_dma2_addr
#define reg_dma_rf_rx_size      reg_dma2_size
#define reg_dma_rf_rx_mode      reg_dma2_mode

#define reg_dma_rf_tx_addr      reg_dma3_addr
#define reg_dma_rf_tx_size      reg_dma3_size
#define reg_dma_rf_tx_mode      reg_dma3_mode

#define reg_dma_pwm_addr        reg_dma7_addr
#define reg_dma_pwm_size        reg_dma7_size
#define reg_dma_pwm_mode        reg_dma7_mode

#define reg_dma_chn_en          REG_ADDR8(0xc20)
#define reg_dma_chn_irq_msk     REG_ADDR8(0xc21)
#define reg_dma_tx_en           REG_ADDR8(0xc22)
enum{
    FLD_DMA_TX_EN_COMMIT =      BIT(0),
    FLD_DMA_TX_EN_MULTI =       BIT(1),
    FLD_DMA_TX_EN_BUFF =        BIT(2),
    FLD_DMA_RX_EN_COMMIT =      BIT(4),
};

#define reg_dma_tx_rdy0         REG_ADDR8(0xc24)
#define reg_dma_tx_rdy1         REG_ADDR8(0xc25)
#define reg_dma_rx_rdy0         REG_ADDR8(0xc26)
#define reg_dma_rx_rdy1         REG_ADDR8(0xc27)

#define reg_dma_irq_status      reg_dma_rx_rdy0
enum{
    FLD_DMA_CHN0 =  BIT(0),     FLD_DMA_CHN_UART_RX =   BIT(0),
    FLD_DMA_CHN1 =  BIT(1),     FLD_DMA_CHN_UART_TX =   BIT(1),
    FLD_DMA_CHN2 =  BIT(2),     FLD_DMA_CHN_RF_RX =     BIT(2),
    FLD_DMA_CHN3 =  BIT(3),     FLD_DMA_CHN_RF_TX =     BIT(3),
    FLD_DMA_CHN4 =  BIT(4),     FLD_DMA_CHN_AES_OUT =   BIT(4),
    FLD_DMA_CHN5 =  BIT(5),     FLD_DMA_CHN_AES_IN =    BIT(5),
    FLD_DMA_CHN7 =  BIT(7),     FLD_DMA_CHN_PWM =       BIT(7),
};

typedef enum {
    FLD_DMA_IRQ_UART_RX =   BIT(0),
    FLD_DMA_IRQ_UART_TX =   BIT(1),
    FLD_DMA_IRQ_RF_RX =     BIT(2),
    FLD_DMA_IRQ_RF_TX =     BIT(3),
    FLD_DMA_IRQ_AES_DECO =  BIT(4),
    FLD_DMA_IRQ_AES_CODE =  BIT(5),
    FLD_DMA_IRQ_PWM =       BIT(7),
    FLD_DMA_IRQ_ALL =       0xff,
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

#define reg_rf_multi_dma_tx_wptr(i)			REG_ADDR8(0xc30+i)
#define reg_rf_multi_dma_tx_rptr(i)			REG_ADDR8(0xc38+i)//This definition is used for the multiceiver function of RF.
															  //The "i" is represent different pipe id.
#define reg_dma0_addrHi			REG_ADDR8(0xc40)
#define reg_dma1_addrHi			REG_ADDR8(0xc41)
#define reg_dma2_addrHi			REG_ADDR8(0xc42)
#define reg_dma3_addrHi			REG_ADDR8(0xc43)
#define reg_dma4_addrHi			REG_ADDR8(0xc44)
#define reg_dma5_addrHi			REG_ADDR8(0xc45)
#define reg_dma_ta_addrHi		REG_ADDR8(0xc46)
#define reg_dma_a3_addrHi		REG_ADDR8(0xc47)
#define reg_dma7_addrHi			REG_ADDR8(0xc48)

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
	FLD_RF_IRQ_HIT_SYNC   =     BIT(14),
	FLD_RF_IRQ_HEADER_DONE   =  BIT(15),
	FLD_RF_IRQ_ALL =            0XFFFF,
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

//******************************      RF_Register_list      ******************************/
#define reg_rf_mode_cfg_rx1_0 REG_ADDR8(REG_AURA_BASE + 0x20)

enum
{
    FLD_RF_BW_CODE = BIT_RNG(1, 3),
    FLD_RF_SC_CODE = BIT(4),
};

#define reg_rf_mode_cfg_rx1_1 REG_ADDR8(REG_AURA_BASE + 0x21)

enum
{
    FLD_RF_MODE_VANT_RX = BIT(1),
    FLD_RF_FE_RTRIM_RX  = BIT_RNG(2, 4),
    FLD_RF_IF_FREQ      = BIT_RNG(5, 6),

};

#define reg_rf_mode_cfg_tx1_1 REG_ADDR8(REG_AURA_BASE + 0x23)

enum
{
    FLD_RF_HPMC_EXP_DIFF_COUNT_H = BIT_RNG(0, 4),
    FLD_RF_DAC_TRIM_CFBK         = BIT_RNG(5, 6),

};

#define reg_rf_mode_cfg_txrx_0 REG_ADDR8(REG_AURA_BASE + 0x26)

enum
{
    FLD_RF_DIS_CLK_DIG_O    = BIT(0),
    FLD_RF_VANT_PULLDN      = BIT(1),
    FLD_RF_GF_BT            = BIT(2),
    FLD_RF_LDO_ANT_TRIM     = BIT_RNG(3, 5),
    FLD_RF_CBPF_TYPE        = BIT(6),
    FLD_RF_TX_PA_PWR_L      = BIT(7),
};

#define reg_rf_mode_cfg_txrx_1 REG_ADDR8(REG_AURA_BASE + 0x27)

enum
{
    FLD_RF_TX_PA_PWR_H = BIT_RNG(0, 4),
};

#define reg_rf_burst_cfg_txrx_0 REG_ADDR8(REG_AURA_BASE + 0x28)

enum
{
    FLD_RF_CHNL_NUM = BIT_RNG(0, 5),
    FLD_RF_TX_EN    = BIT(6),
    FLD_RF_RX_EN    = BIT(7),
};

#define reg_rf_burst_cfg_txrx_1 REG_ADDR8(REG_AURA_BASE + 0x29)

enum
{
    FLD_RF_RX_TIM_SRQ_SEL_TESQ = BIT(0),
    FLD_RF_TX_TIM_SRQ_SEL_TESQ = BIT(1),
    FLD_RF_FE_CTRIM            = BIT_RNG(2, 5),
};

#define reg_rf_txrx_dbg3_0 REG_ADDR8(REG_AURA_BASE + 0x44)

enum
{
    FLD_RF_CHNL_FREQ_DIRECT = BIT(0),
    FLD_RF_CHNL_FREQ_L      = BIT_RNG(1, 7),
};

#define reg_rf_txrx_dbg3_1 REG_ADDR8(REG_AURA_BASE + 0x45)

enum
{
    FLD_RF_CHNL_FREQ_H        = BIT_RNG(0, 5),
    FLD_RF_DSN_DITHER_DISABLE = BIT(6),
    FLD_RF_DSM_INT_MODE       = BIT(7),
};

#define reg_rf_cal_ow_ctrl_0 REG_ADDR8(REG_AURA_BASE + 0x80)

enum
{
    FLD_RF_DSM_RUN_OW          = BIT(0),
    FLD_RF_LDOT_TX_RUN_OW      = BIT(1),
    FLD_RF_RCCAL_RUN_OW        = BIT(2),
    FLD_RF_FCAL_TX_RUN_OW      = BIT(3),
    FLD_RF_RXDCOC_RUN_OW       = BIT(4),
    FLD_RF_HPMC_RUN_OW         = BIT(5),
    FLD_RF_LOCD_RUN_OW         = BIT(6),
    FLD_RF_DCOC_GAIN_CFG_OW    = BIT(6),
};

#define reg_rf_txrx_cb_cal_ctrl REG_ADDR8(REG_AURA_BASE + 0x84)

enum
{
    FLD_RF_LDOT_TX_RUN_CB         = BIT(0),
    FLD_RF_FCAL_TX_RUN_CB         = BIT(1),
    FLD_RF_HPMC_RUN_CB            = BIT(2),
    FLD_RF_LDOT_RX_RUN_CB         = BIT(3),
    FLD_RF_FCAL_RX_RUN_CB         = BIT(4),
    FLD_RF_RCCAL_RUN_CB           = BIT(5),
    FLD_RF_RXDCOC_RUN_CB          = BIT(6),
    FLD_RF_EN_RXDCOC_ADC_DATA_CB  = BIT(6),
};

#define reg_rf_idle_txfsk_ss1_ss2_strt_cb_0 REG_ADDR8(REG_AURA_BASE + 0xa4)

#define reg_rf_idle_txfsk_ss1_ss2_strt_cb_0_1 REG_ADDR8(REG_AURA_BASE + 0xa5)

#define reg_rf_idle_txfsk_ss1_ss2_strt_cb_1 REG_ADDR8(REG_AURA_BASE + 0xa6)

#define reg_rf_idle_txfsk_ss1_ss2_strt_cb_1_1 REG_ADDR8(REG_AURA_BASE + 0xa7)

#define reg_rf_idle_txfsk_ss3_ss4_strt_cb0  REG_ADDR8(REG_AURA_BASE + 0xa8)

#define reg_rf_idle_txfsk_ss3_ss4_strt_cb0_1  REG_ADDR8(REG_AURA_BASE + 0xa9)

#define reg_rf_idle_txfsk_ss3_ss4_strt_cb1  REG_ADDR8(REG_AURA_BASE + 0xaa)

#define reg_rf_idle_txfsk_ss3_ss4_strt_cb1_1  REG_ADDR8(REG_AURA_BASE + 0xab)

#define reg_rf_idle_txfsk_ss5_ss6_strt_cb_0 REG_ADDR8(REG_AURA_BASE + 0xac)

#define reg_rf_idle_txfsk_ss5_ss6_strt_cb_0_1 REG_ADDR8(REG_AURA_BASE + 0xad)

#define reg_rf_idle_txfsk_ss5_ss6_strt_cb_1 REG_ADDR8(REG_AURA_BASE + 0xae)

#define reg_rf_idle_txfsk_ss5_ss6_strt_cb_1_1 REG_ADDR8(REG_AURA_BASE + 0xaf)

#define reg_rf_idle_txfsk_ss7_strt_cb_0 REG_ADDR8(REG_AURA_BASE + 0xbc)

#define reg_rf_idle_txfsk_ss7_strt_cb_0_1 REG_ADDR8(REG_AURA_BASE + 0xbd)

#define reg_rf_idle_rx_ss1_ss2_strt_cb_0    REG_ADDR8(REG_AURA_BASE + 0xb0)

#define reg_rf_idle_rx_ss1_ss2_strt_cb_0_1    REG_ADDR8(REG_AURA_BASE + 0xb1)

#define reg_rf_idle_rx_ss1_ss2_strt_cb_1    REG_ADDR8(REG_AURA_BASE + 0xb2)

#define reg_rf_idle_rx_ss1_ss2_strt_cb_1_1    REG_ADDR8(REG_AURA_BASE + 0xb3)

#define reg_rf_idle_rx_ss3_ss4_strt_cb_0    REG_ADDR8(REG_AURA_BASE + 0xb4)

#define reg_rf_idle_rx_ss3_ss4_strt_cb_0_1    REG_ADDR8(REG_AURA_BASE + 0xb5)

#define reg_rf_idle_rx_ss3_ss4_strt_cb_1    REG_ADDR8(REG_AURA_BASE + 0xb6)

#define reg_rf_idle_rx_ss3_ss4_strt_cb_1_1    REG_ADDR8(REG_AURA_BASE + 0xb7)

#define reg_rf_idle_rx_ss5_ss6_strt_cb_0    REG_ADDR8(REG_AURA_BASE + 0xb8)

#define reg_rf_idle_rx_ss5_ss6_strt_cb_0_1    REG_ADDR8(REG_AURA_BASE + 0xb9)

#define reg_rf_idle_rx_ss5_ss6_strt_cb_1    REG_ADDR8(REG_AURA_BASE + 0xba)

#define reg_rf_idle_rx_ss5_ss6_strt_cb_1_1    REG_ADDR8(REG_AURA_BASE + 0xbb)

#define reg_rf_rccal_dbg1_0 REG_ADDR8(REG_AURA_BASE + 0xc6)

enum
{
    FLD_RF_BYPASS_RCCAL           = BIT(0),
    FLD_RF_RCCAL_OVERWRITE        = BIT_RNG(1, 5),
    FLD_RF_CBPF_CCODE_BYPASS      = BIT(6),
    FLD_RF_CBPF_CCODE_OVERWRITE_L = BIT(7),
};

#define reg_rf_rccal_dbg1_1 REG_ADDR8(REG_AURA_BASE + 0xc7)

enum
{
    FLD_RF_CBPF_CCODE_OVERWRITE_H = BIT_RNG(0, 5),
    FLD_RF_COMP_POL               = BIT(6),
};

#define reg_rf_rccal_dbg2 REG_ADDR8(REG_AURA_BASE + 0xc8)

enum
{
    FLD_RF_RCCAL_CBPF_OFFSET = BIT_RNG(0, 2),
};

#define reg_rf_rccal_rdbk_0 REG_ADDR8(REG_AURA_BASE + 0xca)

enum
{
    FLD_RF_RCCAL_CODE   = BIT_RNG(0, 4),
    FLD_RF_CBPF_CCODE_L = BIT_RNG(5, 7),
};

#define reg_rf_rccal_rdbk_1 REG_ADDR8(REG_AURA_BASE + 0xcb)

enum
{
    FLD_RF_CBPF_CCODE_H = BIT_RNG(0, 3),
};

#define reg_rf_dcoc_bypass_adc_0 REG_ADDR8(REG_AURA_BASE + 0xce)

enum
{
    FLD_RF_DCOC_BYPASS_ADC            = BIT(0),
    FLD_RF_DCOC_IADC_OFFSET_OVERWRITE = BIT_RNG(1, 7),
};

#define reg_rf_dcoc_bypass_adc_1 REG_ADDR8(REG_AURA_BASE + 0xcf)

enum
{
    FLD_RF_DCOC_QADC_OFFSET_OVERWRITE = BIT_RNG(0, 6),
};

#define reg_rf_dcoc_bypass_dac_0 REG_ADDR8(REG_AURA_BASE + 0xd0)

enum
{
    FLD_RF_DCOC_BYPASS_DAC          = BIT(0),
    FLD_RF_DCOC_IDAC_CODE_OVERWRITE = BIT_RNG(1, 7),
};

#define reg_rf_dcoc_bypass_dac_1 REG_ADDR8(REG_AURA_BASE + 0xd1)

enum
{
    FLD_RF_DCOC_QDAC_CODE_OVERWRITE = BIT_RNG(0, 6),
};

#define reg_rf_dcoc_dbg0 REG_ADDR8(REG_AURA_BASE + 0xd2)

enum
{
    FLD_RF_DCOC_SFIQ         = BIT(0),
    FLD_RF_DCOC_SFQI         = BIT(1),
    FLD_RF_DCOC_IDAC_SRC_POL = BIT(2),
    FLD_RF_DCOC_QDAC_SRC_POL = BIT(3),
    FLD_RF_DCOC_SFIIP        = BIT(4),
    FLD_RF_DCOC_SFQQP        = BIT(5),
    FLD_RF_DCOC_SFII_L       = BIT_RNG(6, 7),
};

#define reg_rf_dcoc_dbg1 REG_ADDR8(REG_AURA_BASE + 0xd3)

enum
{
    FLD_RF_DCOC_SFII_H    = BIT_RNG(0, 1),
    FLD_RF_DCOC_SFQQ      = BIT_RNG(2, 5),
    FLD_RF_DCOC_DAC_ORDER = BIT(6),
};

#define reg_rf_dcoc_rdbk1_0 REG_ADDR8(REG_AURA_BASE + 0xd8)

enum
{
    FLD_RF_DCOC_IDAC_CODE = BIT_RNG(0, 6),
    FLD_RF_SPARE_3        = BIT(7),
};

#define reg_rf_dcoc_rdbk1_1 REG_ADDR8(REG_AURA_BASE + 0xd9)

enum
{
    FLD_RF_SPARE_4 = BIT_RNG(0, 3),
};

#define reg_rf_dcoc_rdbk2 REG_ADDR8(REG_AURA_BASE + 0xda)

enum
{
    FLD_RF_DCOC_QDAC_CODE = BIT_RNG(0, 6),
};

#define reg_rf_dcoc_rdbk3_0 REG_ADDR8(REG_AURA_BASE + 0xdc)

enum
{
    FLD_RF_DCOC_IADC_OFFSET   = BIT_RNG(0, 6),
    FLD_RF_DCOC_QADC_OFFSET_L = BIT(7),
};

#define reg_rf_dcoc_rdbk3_1 REG_ADDR8(REG_AURA_BASE + 0xdd)

enum
{
    FLD_RF_DCOC_QADC_OFFSET_H = BIT_RNG(0, 5),
};

#define reg_rf_ldot_dbg1 REG_ADDR8(REG_AURA_BASE + 0xe2)

enum
{
    FLD_RF_LDOT_LDO_CAL_BYPASS         = BIT(0),
    FLD_RF_LDOT_LDO_CAL_TRIM_OVERWRITE = BIT_RNG(1, 6),
};

#define reg_rf_ldot_dbg2_0 REG_ADDR8(REG_AURA_BASE + 0xe4)

enum
{
    FLD_RF_LDOT_LDO_RXTXHF_BYPASS         = BIT(0),
    FLD_RF_LDOT_LDO_RXTXLF_BYPASS         = BIT(1),
    FLD_RF_LDOT_LDO_RXTXHF_TRIM_OVERWRITE = BIT_RNG(2, 7),
};

#define reg_rf_ldot_dbg2_1 REG_ADDR8(REG_AURA_BASE + 0xe5)

enum
{
    FLD_RF_LDOT_LDO_RXTXLF_TRIM_OVERWRITE = BIT_RNG(0, 5),
};

#define reg_rf_ldot_dbg3_0 REG_ADDR8(REG_AURA_BASE + 0xe6)

enum
{
    FLD_RF_LDOT_LDO_PLL_BYPASS         = BIT(0),
    FLD_RF_LDOT_LDO_VCO_BYPASS         = BIT(1),
    FLD_RF_LDOT_LDO_PLL_TRIM_OVERWRITE = BIT_RNG(2, 7),
};

#define reg_rf_ldot_dbg3_1 REG_ADDR8(REG_AURA_BASE + 0xe7)

enum
{
    FLD_RF_LDOT_LDO_VCO_TRIM_OVERWRITE = BIT_RNG(0, 5),
};

#define reg_rf_ldot_dbg4_0 REG_ADDR8(REG_AURA_BASE + 0xe8)

enum
{
    FLD_RF_LDOT_INCCAL_DIS        = BIT(0),
    FLD_RF_LDOT_LDO_RXTXHF_OFFSET = BIT_RNG(1, 2),
    FLD_RF_LDOT_LDO_RXTXLF_OFFSET = BIT_RNG(3, 4),
    FLD_RF_LDOT_LDO_PLL_OFFSET    = BIT_RNG(5, 6),
    FLD_RF_LDOT_LDO_VCO_OFFSET_L  = BIT(7),
};

#define reg_rf_ldot_dbg4_1 REG_ADDR8(REG_AURA_BASE + 0xe9)

enum
{
    FLD_RF_LDOT_LDO_VCO_OFFSET_H = BIT(0),
};

#define reg_rf_ldot_rdbk1 REG_ADDR8(REG_AURA_BASE + 0xea)

enum
{
    FLD_RF_LDOT_LDO_CAL_TRIM = BIT_RNG(0, 5),
};

#define reg_rf_ldot_rdbk2_0 REG_ADDR8(REG_AURA_BASE + 0xec)

enum
{
    FLD_RF_LDOT_LDO_RXTXHF_TRIM   = BIT_RNG(0, 5),
    FLD_RF_LDOT_LDO_RXTXLF_TRIM_L = BIT_RNG(6, 7),
};

#define reg_rf_ldot_rdbk2_1 REG_ADDR8(REG_AURA_BASE + 0xed)

enum
{
    FLD_RF_LDOT_LDO_RXTXLF_TRIM_H = BIT_RNG(0, 3),
};

#define reg_rf_ldot_rdbk3_0 REG_ADDR8(REG_AURA_BASE + 0xee)

enum
{
    FLD_RF_LDOT_LDO_PLL_TRIM   = BIT_RNG(0, 5),
    FLD_RF_LDOT_LDO_VCO_TRIM_L = BIT_RNG(6, 7),
};

#define reg_rf_ldot_rdbk3_1 REG_ADDR8(REG_AURA_BASE + 0xef)

enum
{
    FLD_RF_LDOT_LDO_VCO_TRIM_H = BIT_RNG(0, 3),
};

#define reg_rf_hpmc_cfg REG_ADDR8(REG_AURA_BASE + 0xf4)

enum
{
    FLD_RF_HPMC_DAC_SWING = BIT(0),
    FLD_RF_HPMC_SLOW_MODE = BIT(1),
    FLD_RF_HPMC_COUNT_POL = BIT(2),
};

#define reg_rf_hpmc_debug_0 REG_ADDR8(REG_AURA_BASE + 0xf6)

enum
{
    FLD_RF_HPMC_BYPASS           = BIT(0),
    FLD_RF_HPMC_GAIN_OVERWRITE_L = BIT_RNG(1, 7),
};

#define reg_rf_hpmc_debug_1 REG_ADDR8(REG_AURA_BASE + 0xf7)

enum
{
    FLD_RF_HPMC_GAIN_OVERWRITE_H = BIT_RNG(0, 3),
};

#define reg_rf_hpmc_rdbk1_0 REG_ADDR8(REG_AURA_BASE + 0xf8)


#define reg_rf_hpmc_rdbk1_1 REG_ADDR8(REG_AURA_BASE + 0xf9)


#define reg_rf_hpmc_rdbk2_0 REG_ADDR8(REG_AURA_BASE + 0xfa)


#define reg_rf_hpmc_rdbk2_1 REG_ADDR8(REG_AURA_BASE + 0xfb)


#define reg_rf_hpmc_rdbk3_0 REG_ADDR8(REG_AURA_BASE + 0xfc)

enum
{
    FLD_RF_HPMC_COUNT1_LSW        = BIT_RNG(0, 2),
    FLD_RF_HPMC_COUNT2_LSW        = BIT_RNG(3, 5),
    FLD_RF_HPMC_MEAS_DIFF_COUNT_L = BIT_RNG(6, 7),
};

#define reg_rf_hpmc_rdbk3_1 REG_ADDR8(REG_AURA_BASE + 0xfd)

enum
{
    FLD_RF_HPMC_MEAS_DIFF_COUNT_H = BIT_RNG(0, 6),
    FLD_RF_HPMC_PASS              = BIT(7),
};

#define reg_rf_hpmc_rdbk4_0 REG_ADDR8(REG_AURA_BASE + 0xfe)


#define reg_rf_hpmc_rdbk4_1 REG_ADDR8(REG_AURA_BASE + 0xff)

enum
{
    FLD_RF_HPMC_GAIN_H = BIT_RNG(0, 2),
};

#define reg_rf_fcal_rdbk_0 REG_ADDR8(REG_AURA_BASE + 0xc0)

enum
{
    FLD_RF_FCAL_DCAP_COARSE_L = BIT(7),
    FLD_RF_FCAL_DCAP_FINE   = BIT_RNG(5, 6),
};

#define reg_rf_fcal_rdbk_1 REG_ADDR8(REG_AURA_BASE + 0xc1)

enum
{
    FLD_RF_FCAL_DCAP_COARSE_H = BIT_RNG(0, 4),
};

#define reg_rf_tx_frac_ctr2_0 REG_ADDR8(REG_AURA_BASE + 0xc2)

enum
{
    FLD_RF_FCAL_STL_DCAP_EN      = BIT(0),
    FLD_RF_DCAP_COARSE_OVERWRITE = BIT_RNG(1, 6),
    FLD_RF_DCAP_FINE_OVERWRITE_L = BIT(7),
};
#define reg_rf_tx_frac_ctr2_1 REG_ADDR8(REG_AURA_BASE + 0xc3)

enum
{
    FLD_RF_DCAP_FINE_OVERWRITE_H = BIT(0),
    FLD_RF_FCAL_DEBUG_CNT_SEL = BIT_RNG(2, 5),
};

/********************************************************************************************
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 *****|								Aanlog  Register Table  						   |*****
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 ********************************************************************************************/

/*******************************      analog registers(3v3): 0x00      ***************************/

#define areg_aon_0x02                   0x02
enum{
	FLD_EN_BYPASS_LDO_1P8V            = BIT(0),
};

#define areg_aon_0x05			        0x05
enum{
	FLD_32K_RC_PD						= BIT(0),
	//rsvd
	FLD_24M_RC_PD						= BIT(2),
	FLD_24M_XTL_PD						= BIT(3),
	FLD_PD_PL_ALL_3V					= BIT(4),
	FLD_PD_PL_DCDC_LDO_3V				= BIT(5),
	//rsvd
	FLD_ANA_LDO_PD_3V					= BIT(7),
};
#define areg_aon_0x06			        0x06
enum{
	//rsvd  = BIT(0),
	//rsvd  = BIT(1),
	FLD_PD_TEMP_SENSOR_3V     = BIT(2),//1
	//rsvd  = BIT(3),
	FLD_PM_PD_UVLO_IB         = BIT(4),//0
	FLD_PD_VDD_DCORE          = BIT(5),//0
	FLD_PD_SPD_LDO            = BIT(6),//1
	FLD_PD_DIG_RET_LDO        = BIT(7),//1
};

#define areg_aon_0x08         0x08
enum{
    FLD_SD_ADC_REF_VOL_SEL    = BIT(3),//0:select VMID as ADC reference voltage; 1:select VBG as ADC reference voltage
};

#define areg_aon_0x0b			        0x0b
enum{

	FLD_PD_NTV_1P25           = BIT(0),//1:power down   0: active
	FLD_PD_NTV_1P8            = BIT(1),//1:power down   0: active
	FLD_MSCN_1M_PULLUP           = BIT(6),//0: enable      1: disable
	CS_PULLUP_AND_NATIVE_LDO_EN  = 0x0,
	CS_PULLUP_AND_NATIVE_LDO_DIS = 0x3,
};


#define areg_aon_0x0e        0x0e
#define areg_aon_0x0f        0x0f

#define areg_aon_0x11        0x11
enum{
    FLD_SD_ADC_PATH_SEL      = BIT(5),//0:DC; 1:audio
};

#define areg_0e_pa0_pa3_pull			0x17
#define areg_0f_pa4_pa7_pull			0x18
#define areg_10_pb0_pb3_pull			0x19
#define areg_11_pb4_pb7_pull			0x1a
#define areg_12_pc0_pc3_pull			0x1b
#define areg_13_pc4_pc7_pull			0x1c
#define areg_14_pd0_pd3_pull			0x1d
#define areg_15_pd4_pd7_pull			0x1e
#define areg_16_pe0_pe1_pull			0x1f

#define areg_auto_pd_ctrl_0				0x2B
enum{
	FLD_AUTO_PD_PL_ALL			        = BIT(0),
	FLD_AUTO_PD_PL_DCDC 		        = BIT(1),

};

#define areg_auto_pd_ctrl_1				0x2C
enum{
	FLD_AUTO_PD_RC_32K			        = BIT(0),
	FLD_AUTO_PD_XTAL_32K 		        = BIT(1),
	FLD_AUTO_PD_XTAL_24M		        = BIT(2),
	FLD_AUTO_PD_LC_COMP		  	        = BIT(3),
	FLD_AUTO_PD_MISC			        = BIT(4),
    // <7> rsvd
};

/**
 * Customers cannot use analog register 0x35 because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot or power on has occurred. If this bit is 0, it means that sleep has occurred.
 * [Bit1~7]: These bits are used by the driver and cannot be used by the customer.
 */
#define PM_ANA_REG_WD_CLR_BUF0          0x35 // initial value 0xff.
enum{
    POWERON_FLAG                 = BIT(0),
};

/**
 * Customers cannot use analog register 0x3a because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot has occurred.
 * REBOOT_MANUAL, it means that manually invoke the reboot interface.
 * REBOOT_XTAL_UNSTABLE, it means that the software calls the function sys_reboot() when the crystal oscillator does not start up normally.
 * REBOOT_PM_CLR_PLIC_REQUEST_FAIL, it means that the pm_sleep_wakeup function failed to clear the PM wake flag bit when using the deep wake source, and the software called sys_reboot().
 * Bit0 has been used and can no longer be used, any combination of other bits can be used.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF0    0x3a // initial value 0x00.
typedef enum{
    REBOOT_FLAG                   = BIT(0),
    SW_REBOOT_REASON              = BIT_RNG(1,7),
}pm_poweron_clr_buf0_e;
//Since there are only 7 bits, this enumeration ranges from 0 to 127.
typedef enum{
    APPLIC_INTERFACE              = 0x00,
    XTAL_UNSTABLE                 = 0x01,
    PM_CLR_PLIC_REQUEST_FAIL      = 0x02,
    WAIT_TIMEOUT                  = 0x03,
}pm_sw_reboot_reason_e;


#define areg_aon_0x4c          0x4c
enum{
    FLD_PD_32K_RC               = BIT(0),
	//rsvd             = BIT(1),
    FLD_PD_24M_RC            = BIT(2),
    FLD_PD_24M_XTAL             = BIT(3),
    FLD_PD_LOGIC                = BIT(4),
    FLD_PD_DCDC                 = BIT(5),
	//rsvd             = BIT(6),
    FLD_PD_ANA_BBPLL_TEMP_LDO   = BIT(7),

};
#define areg_aon_0x4d          0x4d
enum{

	//rsvd                 = BIT(0),
//    rsvd            = BIT(1),
    FLD_PD_UVLO_IB              = BIT(2),////Cannot be turned off during sleep.
	//rsvd              = BIT(3),
//    rsvd           = BIT(4),
//    rsvd           = BIT(5),
    FLD_PD_SEQUENCE_EN          = BIT(6),
    FLD_PD_ISOLATION            = BIT(7),
};
#define areg_aon_0x4f                   0x4f
enum{
    FLD_RC_32K_CAP_SEL                  = BIT(6),//0:sel cap from pm_top,1: sel cap from calibration module
    FLD_RC_24M_CAP_SEL                  = BIT(7),//0:sel cap from pm_top,1: sel cap from calibration module
};

#define areg_aon_0x64           0x64
typedef enum {
    FLD_WAKEUP_STATUS_PAD           = BIT(0),
   // FLD_WAKEUP_STATUS_CORE          = BIT(1),
    FLD_WAKEUP_STATUS_TIMER         = BIT(2),
    //FLD_WAKEUP_STATUS_COMPARATOR    = BIT(3),
	FLD_32K_WD_OVERFLOW_STATUS      = BIT(7),
    //To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
    //instead of FLD_WAKEUP_STATUS_INUSE_ALL.
    FLD_WAKEUP_STATUS_ALL           = 0x7f,

    //After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
    //whether the wake source in use has been cleared, because some of the wake sources
    //that are not in use may have been set up.
    FLD_WAKEUP_STATUS_INUSE_ALL     = 0x05,
}pm_wakeup_status_e;
#define areg_aon_rc32k_cap_bits_0_to_2  0x4f
#define areg_aon_rc32k_res_bits_0_to_5  0x50
#define areg_aon_rc32k_res_bits_6_to_13 0x51

#define areg_rc_24m_cap                 0x52

/*******************************      analog registers(1v): 0x80      ***************************/
#define areg_clk_setting				0x82
enum{
	FLD_DCCC_DOUBLER_POWER_DOWN	        = BIT(3),
	FLD_CLK_48M_TO_RX_EN 		        = BIT(4),
	FLD_CLK_48M_TO_DIG_EN 		        = BIT(5),
	FLD_CLK_24M_TO_SAR_EN 		        = BIT(6),
	FLD_CLK_48M_TO_CAL_DIG_MAN_EN       = BIT(7),
};

#define areg_xo_setting				    0x8a
enum{
	FLD_XO_CDAC_ANA	  	                = BIT_RNG(0,5),
	FLD_XO_MODE_ANA 	                = BIT(6),
	FLD_XO_CAP_OFF_ANA                  = BIT(7),
};

#define areg_0xc6                       0xc6
enum{
	FLD_CAL_32K_RC_DISABLE              = 0xf6,
    FLD_CAL_32K_RC_ENABLE               = 0xf7,
};

#define areg_0xc7                       0xc7
enum{
	FLD_CAL_24M_RC_DISABLE             = 0x0e,
    FLD_CAL_24M_RC_ENABLE              = 0x0f,
};

#define areg_cal_24m_wait_len           0xc8//wait 24m rc stable cycles
#define areg_0xc9                       0xc9
#define areg_0xca                       0xca
#define areg_rc_24m_cal_cap             0xcb
#define areg_0xcf                       0xcf
enum{
    FLD_CAL_32K_DONE                  = BIT(6),
    FLD_CAL_24M_DONE                  = BIT(7),
};
/*******************************        analog registers(PGA)        ***************************/
#define codec_ana_cfg2                  0xe6
#define codec_ana_cfg3                  0xe7
#define codec_ana_cfg4                  0xe8
/**
 * BIT[0] 1:power down baseband.
 * BIT[2] 1:power down audio.
 * BIT[7] 1:enable change power sequence clk.
 */
#define areg_aon_0x7d                0x7d //analog always on, even in deep retention
enum
{
    FLD_PG_ZB_EN                        = BIT(0),
    FLD_PG_AUDIO_EN                     = BIT(2),
    FLD_PG_CLK_EN                       = BIT(7),
};

/**
 * @brief   pm power weather to power down definition
 */
typedef enum {
     PM_POWER_BASEBAND      = BIT(0),   //weather to power on the BASEBAND before suspend.
     PM_POWER_AUDIO         = BIT(2),   //weather to power on the AUDIO before suspend.
}pm_pd_module_e;

/**
 * BIT[0] 1:Force start for XO LDO PTAT loop
 * BIT[1] xtal_24M clock to analog   0:turn off  1:turn on   default :0
 * BIT[2] 0: DEM function of sigma-delta ADC is disabled;1: DEM function of sigma-delta ADC is enabled
 * BIT[3:4] PD signal for two adc buffers
 * BIT[5] buffer
 * BIT[6] Selecting positive signal sent to adc_buffer between ai_p(aio)_div and vbat_div. 0 for ai_p(aio)_div;  1 for vbat_div.
 * BIT[7] Adjust output current in CODEC_BIAS
 */
#define areg_0x8c                       0x8c
enum
{
    FLD_LDO_FORCE_ANA                   = BIT(0),
    FLD_EN_CLK_ANA                      = BIT(1),
    FLD_L_DEM_EN                        = BIT(2),
    FLD_L_PD_BUFFER                     = BIT_RNG(3,4),
    FLD_BUFFER                          = BIT(5),
    FLD_L_SEL_DIVOUT_P                  = BIT(6),
    FLD_L_LV_EN                         = BIT(7),
};

/**
 * BIT[0:1] Selecting dividing ratio of vbatt.
 *          00:1/8; 01:1/4; 10: 1/2; 11:3/4
 * BIT[2] 0: Unmute PGA 1: Mute PGA.
 * BIT[3] 0: Enable PGA input 1: Disable PGA input.
 * BIT[4:7] 1111 for min PGA gain; 0000 for max PGA gain.
 */
#define areg_0x8d                       0x8d
enum
{
    FLD_SEL_VBAT_DIV                    = BIT_RNG(0,1),
    FLD_AUDIO_MUTE_PGA                  = BIT(2),
    FLD_AUDIO_PGA_DIS                   = BIT(3),
    FLD_AUDIO_PGAVOL_IN                 = BIT_RNG(4,7),
};

/**
 * BIT[0:1] Control the filtering cap value in PGA.
 * BIT[2:3] Control the driving ability of VMID.
 * BIT[4] PD signal for PGA.
 * BIT[5] PD signal for CODEC Bias
 * BIT[6] PD signal for ADC
 * BIT[7] PD signal for VMID
 */
#define areg_0x8e                       0x8e
enum
{
    FLD_AUDIO_PD_FILTCAP_CTL            = BIT_RNG(0,1),
    FLD_AUDIO_PD_VMIDSEL                = BIT_RNG(2,3),
    FLD_AUDIO_PD_PGA                    = BIT(4),
    FLD_AUDIO_PD_BIAS                   = BIT(5),
    FLD_AUDIO_PD_ADC                    = BIT(6),
    FLD_AUDIO_PD_VMID                   = BIT(7),
};

/**
 * BIT[3] Low-power mode enable signal. <3>for PGA.
          0: default mode 1: low-power mode
 * BIT[4:6] Control the selection of test signal from ADC.
 * BIT[7]   vbat detector enable signal 0:disable; 1:enable; default:0
 */
#define areg_0x8f                       0x8f
enum{
    FLD_PGA_LOWPOWER                    = BIT(3),
    FLD_ADC_TEST                        = BIT_RNG(4,6),
    FLD_EN_VBAT                         = BIT(7),
};

#define areg_sel_ana_input     0x90
enum{
    FLD_SEL_ANA_INPUT_N                   =  BIT_RNG(0,3),
    FLD_SEL_ANA_INPUT_P                   =  BIT_RNG(4,7),
};

#define areg_sel_ana_input_div     0x91
enum{
    FLD_SEL_ANA_INPUT_N_DIV                   =  BIT_RNG(0,1),
    FLD_SEL_ANA_INPUT_P_DIV                   =  BIT_RNG(2,3),
};
#if defined(__cplusplus)
}
#endif





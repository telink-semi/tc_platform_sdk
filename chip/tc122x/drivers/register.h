/********************************************************************************************************
 * @file    register.h
 *
 * @brief   This is the header file for TC122X
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#define reg_rst0				REG_ADDR8(0x60)
enum{
	FLD_RST0_QDEC    = 			BIT(0),
	FLD_RST0_SWIRE  = 			BIT(1),
	FLD_RST0_ZB     = 			BIT(2),
	FLD_RST0_STIMER = 			BIT(3),
	FLD_RST0_DMA    =			BIT(4),
	FLD_RST0_ALGM   =		    BIT(5),
	FLD_RST0_MC     = 			BIT(6),
	FLD_RST0_MCIC   = 			BIT(7),
};

#define reg_rst1				REG_ADDR8(0x61)
enum{
    FLD_RST1_UART   = 			BIT(0),
	FLD_RST1_PWM    = 			BIT(1),
    FLD_RST1_SAR    = 			BIT(2),
};

#define reg_clk_sel				REG_ADDR8(0x64)
enum{
	FLD_SCLK_DIV = 				BIT_RNG(0,3),
	FLD_SCLK_SEL =				BIT_RNG(4,5),
};

#define reg_wakeup_en			REG_ADDR8(0x6e)
enum{
	FLD_WAKEUP_SRC_GPIO            = BIT(0),
	FLD_WAKEUP_SRC_USB             = BIT(1),
	FLD_WAKEUP_SRC_GPIO_RESUME     = BIT(5),
	FLD_WAKEUP_SRC_USB_RESUME      = BIT(6),
	FLD_WAKEUP_SRC_POWER_RESUME    = BIT(7),
};

#define reg_pwdn_en			    REG_ADDR8(0x6f)
enum
{
	FLD_PWDN_CTRL_REBOOT = BIT(5),
	FLD_PWDN_CTRL_SLEEP  = BIT(7),
};

#define reg_trng_data			REG_ADDR8(0x76)

#define reg_mcu_wakeup_mask		REG_ADDR32(0x78)

/*******************************      uart0 registers: 0x90      ******************************/
#define reg_uart_data_buf0(i) REG_ADDR8(0x90)
#define reg_uart_data_buf1(i) REG_ADDR8(0x91)
#define reg_uart_data_buf2(i) REG_ADDR8(0x92)
#define reg_uart_data_buf3(i) REG_ADDR8(0x93)
#define reg_uart_data_buf(i)  REG_ADDR8(0x90 + i) // i = 0~3
#define reg_uart_clk_div(i)   REG_ADDR16(0x94)
enum
{
    FLD_UART_CLK_DIV    = BIT_RNG(0, 14),
    FLD_UART_CLK_DIV_EN = BIT(15)
};

#define reg_uart_ctrl0(i) REG_ADDR8(0x96)
enum
{
	FLD_UART_BPWC = BIT_RNG(0, 3),
	FLD_UART_RX_DMA_EN = BIT(4),
	FLD_UART_TX_DMA_EN = BIT(5),
	FLD_UART_RX_IRQ_EN = BIT(6),
	FLD_UART_TX_IRQ_EN = BIT(7),
};

#define reg_uart_ctrl1(i) REG_ADDR8(0x97)
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

#define reg_uart_ctrl2(i) REG_ADDR8(0x98)
enum
{
	FLD_UART_CTRL2_RTS_TRIG_LVL = BIT_RNG(0, 3),
	FLD_UART_CTRL2_RTS_PARITY = BIT(4),
	FLD_UART_CTRL2_RTS_MANUAL_VAL = BIT(5),
	FLD_UART_CTRL2_RTS_MANUAL_EN = BIT(6),
	FLD_UART_CTRL2_RTS_EN = BIT(7),
};

#define reg_uart_ctrl3(i) REG_ADDR8(0x99 )
enum
{
	FLD_UART_RX_IRQ_TRIG_LEV = BIT_RNG(0, 3),
	FLD_UART_TX_IRQ_TRIG_LEV = BIT_RNG(4, 7),
};

#define reg_uart_rx_timeout0(i) REG_ADDR8(0x9a)
enum
{
	FLD_UART_TIMEOUT_BW = BIT_RNG(0, 7),
};

#define reg_uart_rx_timeout1(i) REG_ADDR8(0x9b)
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

#define reg_uart_buf_cnt(i) REG_ADDR8(0x9c)
enum
{
	FLD_UART_RX_BUF_CNT = BIT_RNG(0, 3),
	FLD_UART_TX_BUF_CNT = BIT_RNG(4, 7),
};

#define reg_uart_status0(i) REG_ADDR8(0x9d)
enum
{
	FLD_UART_RBCNT = BIT_RNG(0, 2),
	FLD_UART_IRQ_FLAG = BIT(3),
	FLD_UART_WBCNT = BIT_RNG(4, 6),
	FLD_URAT_CLEAR_RXDONE_FLAG = BIT(4),
	FLD_UART_CLEAR_RX_FLAG = BIT(6),
	FLD_UART_RX_ERR_FLAG = BIT(7),
};

#define reg_uart_status1(i) REG_ADDR8(0x9e)
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

#define reg_uart_state(i) REG_ADDR8(0x9f)
enum{
	FLD_UART_TSTATE_I 	     =  BIT_RNG(0,2),
	FLD_UART_RSTATE_I	     =  BIT_RNG(4,7),
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

#define reg_qdec_count0           REG_ADDR8(REG_QDEC_BASE+0x00)

#define reg_qdec_dbntime          REG_ADDR8(REG_QDEC_BASE+0x01)
enum{
	FLD_QDEC_DBNTIME  = BIT_RNG(0,2),
	FLD_QDEC_POLA     = BIT(4),
	FLS_QDEC_SHUTTLE0 = BIT(5),
};

#define reg_qdec_channel_a        REG_ADDR8(REG_QDEC_BASE+0x02)
enum{
	FLD_QDEC_CHANNEL_A   = BIT_RNG(0,2),
};

#define reg_qdec_channel_b        REG_ADDR8(REG_QDEC_BASE+0x03)
enum{
	FLD_QDEC_CHANNEL_B   = BIT_RNG(0,2),
};

#define reg_qdec_mask             REG_ADDR8(REG_QDEC_BASE+0x04)
enum{
	FLD_QDEC_MASK       = BIT(0),
};

#define reg_qdec_int0             REG_ADDR8(REG_QDEC_BASE+0x05)
enum{
	FLD_QDEC_INT0     = BIT(0),
};

#define reg_qdec_read             REG_ADDR8(REG_QDEC_BASE+0x06)

#define reg_qdec_mode          REG_ADDR8(REG_QDEC_BASE+0x07)
enum{
	FLD_QDEC_DOUBLE0   = BIT(0),
};

#define reg_qdec_load    REG_ADDR8(REG_QDEC_BASE+0x08)
enum{
	FLD_QDEC_COUNT0_RELOAD   = BIT(0),
};

/*******************************      usb registers: 0x100      ******************************/
#define reg_ctrl_ep_ptr  REG_ADDR8(0x100)
#define reg_ctrl_ep_dat  REG_ADDR8(0x101)
#define reg_ctrl_ep_ctrl REG_ADDR8(0x102)
enum
{
    FLD_EP_DAT_ACK     = BIT(0),
    FLD_EP_DAT_STALL   = BIT(1),
    FLD_EP_STA_ACK     = BIT(2),
    FLD_EP_STA_STALL   = BIT(3),
    FLD_EP_IRQ_SETADDR = BIT(6),
};

#define reg_ctrl_ep_irq_sta REG_ADDR8(0x103)
enum
{
    FLD_USB_IRQ_RESET_STATUS   = BIT(0),
    FLD_USB_IRQ_250US_STATUS   = BIT(1),
    FLD_USB_IRQ_SUSPEND_STATUS = BIT(2),
    FLD_USB_IRQ_SOF_STATUS     = BIT(3),
    FLD_CTRL_EP_IRQ_SETUP      = BIT(4),
    FLD_CTRL_EP_IRQ_DATA       = BIT(5),
    FLD_CTRL_EP_IRQ_STA        = BIT(6),
    FLD_CTRL_EP_IRQ_INTF       = BIT(7),
};

#define reg_ctrl_ep_irq_mode REG_ADDR8(0x104)
enum
{
    FLD_CTRL_EP_AUTO_ADDR = BIT(0),
    FLD_CTRL_EP_AUTO_CFG  = BIT(1),
    FLD_CTRL_EP_AUTO_INTF = BIT(2),
    FLD_CTRL_EP_AUTO_STA  = BIT(3),
    FLD_CTRL_EP_AUTO_SYN  = BIT(4),
    FLD_CTRL_EP_AUTO_DESC = BIT(5),
    FLD_CTRL_EP_AUTO_FEAT = BIT(6),
    FLD_CTRL_EP_AUTO_STD  = BIT(7),
};

#define reg_usb_ctrl REG_ADDR8(0x105)
typedef enum
{
    FLD_USB_AUTO_HALT_CLR    = BIT(0), /* enable auto clear halt feature(STALL) after either a SetConfiguration() or SetInterface() */
    FLD_USB_AUTO_HALT_STALL  = BIT(1), /* enable auto decoding the halt feature(STALL) by hardware */
    FLD_USB_AUTO_HALT_TOGGLE = BIT(2), /* enable auto decoding the data toggle */
    FLD_USB_AUTO_WKUP_FEA    = BIT(3), /* enable auto decoding the wakeup feature command */
} usb_hw_feature_e;

#define reg_usb_pid_l     REG_ADDR8(0x106)
#define reg_usb_pid_h     REG_ADDR8(0x107)
#define reg_usb_edp0_udc  REG_ADDR8(0x108)
#define reg_usb_edp0_size REG_ADDR8(0x109) /* the size of edp0 */
enum
{
    FLD_USB_CTR_EP_SIZE      = BIT_RNG(0, 1), /**< 0-8bytes,1-16bytes,2-32bytes,3-64bytes, */
    FLD_USB_CTR_EP_LENS      = BIT_RNG(2, 3),
    FLD_USB_IRQ_RESET_LVL    = BIT(4), /**< lvl[0]:0-->irq_reset_edge; 1-->usb_reset_i */
    FLD_USB_IRQ_250US_LVL    = BIT(5), /**< lvl[1]:0-->irq_250us_edge; 1-->usb_250us_i */
    FLD_USB_IRQ_SETADDR_MASK = BIT(6),
    FLD_USB_IRQ_SOF_LVL      = BIT(7), /**< lvl[3]:0-->irq_sof_edge; 1-->usb_sof_i */
};

#define reg_usb_mdev REG_ADDR8(0x10a)
enum
{
    FLD_USB_MDEV               = BIT(0),
    FLD_USB_SET_WAKEUP_FEATURE = BIT(1),
    FLD_USB_WAKEUP_FEATURE     = BIT(2),
    FLD_USB_VENDOR             = BIT(3),
    FLD_USB_VENDOR_DISABLE     = BIT(4),
    FLD_USB_CONFIG             = BIT(7),
};

#define reg_usb_sups_cyc_cali REG_ADDR8(0x10c)
enum
{
    FLD_USB_SCNT       = BIT_RNG(0, 4),
    FLD_USB_EDP0_STALL = BIT(5),
    FLD_USB_CLR_UDC_EN = BIT(6),
};

#define reg_usb_intf_alt REG_ADDR8(0x10d)
#define reg_usb_ep_en    REG_ADDR8(0x10e)
typedef enum
{
    FLD_USB_EDP8_EN = BIT(0),
    FLD_USB_EDP1_EN = BIT(1),
    FLD_USB_EDP2_EN = BIT(2),
    FLD_USB_EDP3_EN = BIT(3),
    FLD_USB_EDP4_EN = BIT(4),
    FLD_USB_EDP5_EN = BIT(5),
    FLD_USB_EDP6_EN = BIT(6),
    FLD_USB_EDP7_EN = BIT(7),
} usb_ep_en_e;

#define reg_usb_irq_mask REG_ADDR8(0x10f)
typedef enum
{
    FLD_USB_IRQ_RESET_MASK   = BIT(0),
    FLD_USB_IRQ_250US_MASK   = BIT(1),
    FLD_USB_IRQ_SUSPEND_MASK = BIT(2),
    FLD_USB_IRQ_SOF_MASK     = BIT(3),
    FLD_USB_IRQ_SETUP_MASK   = BIT(4),
    FLD_USB_IRQ_DATA_MASK    = BIT(5),
    FLD_USB_IRQ_STATUS_MASK  = BIT(6),
    FLD_USB_IRQ_SETINF_MASK  = BIT(7),
} usb_irq_mask_e;

#define reg_usb_ep8123_ptr REG_ADDR32(0x110)
#define reg_usb_ep8_ptr    REG_ADDR8(0x110)
#define reg_usb_ep1_ptr    REG_ADDR8(0x111)
#define reg_usb_ep2_ptr    REG_ADDR8(0x112)
#define reg_usb_ep3_ptr    REG_ADDR8(0x113)
#define reg_usb_ep4567_ptr REG_ADDR32(0x114)
#define reg_usb_ep4_ptr    REG_ADDR8(0x114)
#define reg_usb_ep5_ptr    REG_ADDR8(0x115)
#define reg_usb_ep6_ptr    REG_ADDR8(0x116)
#define reg_usb_ep7_ptr    REG_ADDR8(0x117)
#define reg_usb_ep_ptr(i)  REG_ADDR8(0x110 + ((i) & 0x07))

#define reg_usb_ep8123_dat REG_ADDR32(0x118)
#define reg_usb_ep8_dat    REG_ADDR8(0x118)
#define reg_usb_ep1_dat    REG_ADDR8(0x119)
#define reg_usb_ep2_dat    REG_ADDR8(0x11a)
#define reg_usb_ep3_dat    REG_ADDR8(0x11b)
#define reg_usb_ep4567_dat REG_ADDR32(0x11c)
#define reg_usb_ep4_dat    REG_ADDR8(0x11c)
#define reg_usb_ep5_dat    REG_ADDR8(0x11d)
#define reg_usb_ep6_dat    REG_ADDR8(0x11e)
#define reg_usb_ep7_dat    REG_ADDR8(0x11f)
#define reg_usb_ep_dat(i)  REG_ADDR8(0x118 + ((i) & 0x07))
#define reg_usb_mic_dat0   REG_ADDR16(0x1800)
#define reg_usb_mic_dat1   REG_ADDR16(0x1802)

#define reg_usb_ep8_ctrl   REG_ADDR8(0x120)
#define reg_usb_ep1_ctrl   REG_ADDR8(0x121)
#define reg_usb_ep2_ctrl   REG_ADDR8(0x122)
#define reg_usb_ep3_ctrl   REG_ADDR8(0x123)
#define reg_usb_ep4_ctrl   REG_ADDR8(0x124)
#define reg_usb_ep5_ctrl   REG_ADDR8(0x125)
#define reg_usb_ep6_ctrl   REG_ADDR8(0x126)
#define reg_usb_ep7_ctrl   REG_ADDR8(0x127)
#define reg_usb_ep_ctrl(i) REG_ADDR8(0x120 + ((i) & 0x07))
enum
{
    FLD_USB_EP_BUSY    = BIT(0),
    FLD_USB_EP_STALL   = BIT(1),
    FLD_USB_EP_DAT0    = BIT(2),
    FLD_USB_EP_DAT1    = BIT(3),
    FLD_USB_EP_MONO    = BIT(6),
    FLD_USB_EP_EOF_ISO = BIT(7),
};

#define reg_usb_ep8123_buf_addr REG_ADDR32(0x128)
#define reg_usb_ep8_buf_addr    REG_ADDR8(0x128) /* reset value: 0x80 */
#define reg_usb_ep1_buf_addr    REG_ADDR8(0x129) /* reset value: 0x00 */
#define reg_usb_ep2_buf_addr    REG_ADDR8(0x12a) /* reset value: 0x08 */
#define reg_usb_ep3_buf_addr    REG_ADDR8(0x12b) /* reset value: 0x10 */
#define reg_usb_ep4567_buf_addr REG_ADDR32(0x12c)
#define reg_usb_ep4_buf_addr    REG_ADDR8(0x12c) /* reset value: 0x40 */
#define reg_usb_ep5_buf_addr    REG_ADDR8(0x12d) /* reset value: 0xc0 */
#define reg_usb_ep6_buf_addr    REG_ADDR8(0x12e) /* reset value: 0x20 */
#define reg_usb_ep7_buf_addr    REG_ADDR8(0x12f) /* reset value: 0x30 */
#define reg_usb_ep_buf_addr(i)  REG_ADDR8(0x128 + ((i) & 0x07))

#define reg_usb_ram_ctrl REG_ADDR8(0x130)
enum
{
    FLD_USB_CEN_PWR_DN      = BIT(0),
    FLD_USB_CLK_PWR_DN      = BIT(1),
    FLD_USB_WEN_PWR_DN      = BIT(3),
    FLD_USB_CEN_FUNC        = BIT(4),
    FLD_USB_TIMEOUT_NAK_EN  = BIT(6),
    FLD_USB_RX_DAT_ASYNC_EN = BIT(7),
};

#define reg_usb_map REG_ADDR8(0x131)
enum
{
    FLD_USB_EDP_MAP_MANUAL_EN = BIT(3),
    FLD_USB_EDP_MAP_AUTO_EN   = BIT(4),
    FLD_USB_EDPS_SM_MAP_EN    = BIT(5),
    FLD_USB_EDPS_MAP_TGL_EN   = BIT(6),
    FLD_USB_GET_STA_MAP_EN    = BIT(7),
};
#define reg_usb_edps_map_en   REG_ADDR8(0x132)
#define reg_usb_edps_logic_en REG_ADDR8(0x133)

#define reg_usb_edps_map(i) REG_ADDR8(0x134 + (((i) & 0x07) >> 1))

#define reg_usb_iso_mode REG_ADDR8(0x138)
#define reg_usb_irq      REG_ADDR8(0x139)
#define reg_usb_mask     REG_ADDR8(0x13a)
typedef enum
{
    FLD_USB_EDP8_IRQ = BIT(0),
    FLD_USB_EDP1_IRQ = BIT(1),
    FLD_USB_EDP2_IRQ = BIT(2),
    FLD_USB_EDP3_IRQ = BIT(3),
    FLD_USB_EDP4_IRQ = BIT(4),
    FLD_USB_EDP5_IRQ = BIT(5),
    FLD_USB_EDP6_IRQ = BIT(6),
    FLD_USB_EDP7_IRQ = BIT(7),
} usb_eps_irq_e;

#define reg_usb_ep8_send_max   REG_ADDR8(0x13b)
#define reg_usb_ep8_send_thres REG_ADDR8(0x13c)
#define reg_usb_ep8_fifo_mode  REG_ADDR8(0x13d)
enum
{
    FLD_USB_ENP8_FIFO_MODE = BIT(0),
    FLD_USB_ENP8_FULL_FLAG = BIT(1),
};

#define reg_usb_ep_max_size REG_ADDR8(0x13e)
#define reg_usb_usbtick     REG_ADDR8(0x13f)

#define reg_usb_edps_eptrl     REG_ADDR8(0x142)
#define reg_usb_edps_eptrh     REG_ADDR8(0x143)
#define reg_usb_tstamp0        REG_ADDR8(0x144)
#define reg_usb_tstamp1        REG_ADDR8(0x145)
#define reg_usb_tstamp2        REG_ADDR8(0x146)
#define reg_usb_tstamp3        REG_ADDR8(0x147)
#define reg_usb_sof_frame0     REG_ADDR8(0x148)
#define reg_usb_sof_frame1     REG_ADDR8(0x149)
#define reg_usb_edps_full_thrd REG_ADDR8(0x14a) /* zero package flag */
#define reg_usb_edps_fulls     REG_ADDR8(0x14b)
#define reg_usb_edps_udc_ptrl  REG_ADDR8(0x14c)
#define reg_usb_edps_udc_ptrh  REG_ADDR8(0x14d)
#define reg_usb_edps_s_ptr     REG_ADDR8(0x14e)
#define reg_usb_edps_edp_r     REG_ADDR8(0x14f)

#define reg_usb_cyc_cali REG_ADDR16(0x150)
#define reg_usb_calcyc_l REG_ADDR8(0x150)
#define reg_usb_calcyc_h REG_ADDR8(0x151)
#define reg_usb_usbct    REG_ADDR8(0x152)

#define reg_rf_acc_len			REG_ADDR8(0x405)
enum{
	FLD_RF_ACC_LEN	    = 		BIT_RNG(0,2),
	FLD_RF_LR_MAN_EN	=       BIT(3),   //long range manual enable
	FLD_RF_LR_TX_SEL	=   	BIT(4),
	FLD_RF_BLE_LR		=   	BIT(5),
	FLD_RF_LR_ACC_TRIG	=   	BIT(7),
};


#define reg_rf_timestamp		REG_ADDR32(0x450)

/*******************************      gpio registers: 0x500      ******************************/
#define GPIO_BASE_ADDR          0x500

#define reg_gpio_pa_setting1	  REG_ADDR32(GPIO_BASE_ADDR)
#define reg_gpio_pa_in            REG_ADDR8(GPIO_BASE_ADDR)
#define reg_gpio_pa_ie            REG_ADDR8(GPIO_BASE_ADDR+0x01)
#define reg_gpio_pa_oe            REG_ADDR8(GPIO_BASE_ADDR+0x02)
#define reg_gpio_pa_pe            REG_ADDR8(GPIO_BASE_ADDR+0x03)
#define reg_gpio_pa_ds            REG_ADDR8(GPIO_BASE_ADDR+0x04)

#define reg_gpio_pa_gpio          REG_ADDR8(GPIO_BASE_ADDR+0x06)//PA0-7
#define reg_gpio_pa_pol           REG_ADDR8(GPIO_BASE_ADDR+0x07)

#define reg_gpio_pa_out_set_clear REG_ADDR16(GPIO_BASE_ADDR+0x20)
#define reg_gpio_pa_out_set       REG_ADDR8(GPIO_BASE_ADDR+0x20)
#define reg_gpio_pa_out_clear     REG_ADDR8(GPIO_BASE_ADDR+0x21)
#define reg_gpio_pa_out_toggle    REG_ADDR8(GPIO_BASE_ADDR+0x22)

#define reg_gpio_pb_setting1	  REG_ADDR32(GPIO_BASE_ADDR+0x08)
#define reg_gpio_pb_in			  REG_ADDR8(GPIO_BASE_ADDR+0x08)//PB0-6
#define reg_gpio_pb_ie            REG_ADDR8(GPIO_BASE_ADDR+0x09)
#define reg_gpio_pb_oe            REG_ADDR8(GPIO_BASE_ADDR+0x0a)
#define reg_gpio_pb_pe            REG_ADDR8(GPIO_BASE_ADDR+0x0b)
#define reg_gpio_pb_ds            REG_ADDR8(GPIO_BASE_ADDR+0x0c)

#define reg_gpio_pb_gpio          REG_ADDR8(GPIO_BASE_ADDR+0x0e)
#define reg_gpio_pb_pol           REG_ADDR8(GPIO_BASE_ADDR+0x0f)

#define reg_gpio_pb_out_set_clear REG_ADDR16(GPIO_BASE_ADDR+0x24)
#define reg_gpio_pb_out_set       REG_ADDR8(GPIO_BASE_ADDR+0x24)
#define reg_gpio_pb_out_clear     REG_ADDR8(GPIO_BASE_ADDR+0x25)
#define reg_gpio_pb_out_toggle    REG_ADDR8(GPIO_BASE_ADDR+0x26)

#define areg_gpio_pa6to7_pb0to2_ie    0xe4
#define areg_gpio_pa6to7_pb0to2_pd    0xe5
#define areg_gpio_pa6to7_pb0to2_ds    0xe6

#define reg_gpio_in(i)              REG_ADDR8(GPIO_BASE_ADDR+((i>>8)<<3))
#define reg_gpio_ie(i)              REG_ADDR8(GPIO_BASE_ADDR+0x01+((i>>8)<<3))
#define reg_gpio_oe(i)              REG_ADDR8(GPIO_BASE_ADDR+0x02+((i>>8)<<3))
#define reg_gpio_pe(i)              REG_ADDR8(GPIO_BASE_ADDR+0x03+((i>>8)<<3))

#define reg_gpio_ds(i)              REG_ADDR8(GPIO_BASE_ADDR+0x04+((i>>8)<<3))
#define reg_gpio_func(i)            REG_ADDR8(GPIO_BASE_ADDR+0x06+((i>>8)<<3))
#define reg_gpio_pol(i)             REG_ADDR8(GPIO_BASE_ADDR+0x07+((i>>8)<<3))

#define reg_gpio_out_set_clear(i)   REG_ADDR16(GPIO_BASE_ADDR+0x20+(((i)>>8)<<2))
#define reg_gpio_out_set(i)         REG_ADDR8(GPIO_BASE_ADDR+0x20+(((i)>>8)<<2))
#define reg_gpio_out_clear(i)       REG_ADDR8(GPIO_BASE_ADDR+0x21+(((i)>>8)<<2))
#define reg_gpio_out_toggle(i)      REG_ADDR8(GPIO_BASE_ADDR+0x22+(((i)>>8)<<2))


#define reg_gpio_irq0_en(i)  	    REG_ADDR8(GPIO_BASE_ADDR+0x10 + (i >> 8))
#define reg_gpio_irq1_en(i)  	    REG_ADDR8(GPIO_BASE_ADDR+0x12 + (i >> 8))
#define reg_gpio_irq2_en(i)  	    REG_ADDR8(GPIO_BASE_ADDR+0x14 + (i >> 8))

#define reg_gpio_irq_pad_mask     REG_ADDR8(GPIO_BASE_ADDR+0x16)
#define reg_gpio_irq_lvl          REG_ADDR8(GPIO_BASE_ADDR+0x17)
#define reg_gpio_irq_status       REG_ADDR8(GPIO_BASE_ADDR+0x18)
enum{
  FLD_IRQ_RISC0     = BIT(0),
  FLD_IRQ_RISC1     = BIT(1),
  FLD_IRQ_RISC2     = BIT(2),
};

#define reg_gpio_risc_en       REG_ADDR8(GPIO_BASE_ADDR+0x19)
enum{
  FLD_RISC0_EN     = BIT(0),
  FLD_RISC1_EN     = BIT(1),
  FLD_RISC2_EN     = BIT(2),
};

#define reg_gpio_fs(i)          REG_ADDR8(GPIO_BASE_ADDR + 0x30 + ((i >> 8) << 1) + ((i & 0xf0) ? 1 : 0))

/*******************************      timer registers: 0x620      ******************************/
#define reg_tmr0_ctrl8			REG_ADDR8(0x620)
enum{
	FLD_TMR0_EN =				BIT(0),
	FLD_TMR0_MODE =				BIT_RNG(1,2),
	FLD_TMR0_NO_WRAP =          BIT(3),
};

#define reg_tmr0_capt0			REG_ADDR8(0x621)
#define reg_tmr0_capt1			REG_ADDR8(0x622)
#define reg_tmr0_capt2			REG_ADDR8(0x623)

#define reg_tmr1_ctrl8			REG_ADDR8(0x624)
enum{
	FLD_TMR1_EN =				BIT(0),
	FLD_TMR1_MODE =				BIT_RNG(1,2),
	FLD_TMR1_NO_WRAP =          BIT(3),
};

#define reg_tmr1_capt0			REG_ADDR8(0x625)
#define reg_tmr1_capt1			REG_ADDR8(0x626)
#define reg_tmr1_capt2			REG_ADDR8(0x627)

#define reg_tmr0_tick0			REG_ADDR8(0x630)
#define reg_tmr0_tick1			REG_ADDR8(0x631)
#define reg_tmr0_tick2			REG_ADDR8(0x632)

#define reg_tmr0_sta				REG_ADDR8(0x633)
enum{
	FLD_TMR_STA_TMR0 =			BIT(0),
};

#define reg_tmr1_tick0			REG_ADDR8(0x634)
#define reg_tmr1_tick1			REG_ADDR8(0x635)
#define reg_tmr1_tick2			REG_ADDR8(0x636)

#define reg_tmr1_sta				REG_ADDR8(0x637)
enum{
	FLD_TMR_STA_TMR1 =			BIT(0),
};
/*******************************      irq registers: 0x640      ******************************/

#define reg_irq_mask            REG_ADDR32(0x640)
#define reg_irq_pri             REG_ADDR32(0x644)
#define reg_irq_src             REG_ADDR32(0x648)
typedef enum
{
    FLD_IRQ_TMR0_EN              = BIT(0),
    FLD_IRQ_TMR1_EN              = BIT(1),
 
    FLD_IRQ_DMA_EN               = BIT(4),
    FLD_IRQ_UART_EN              = BIT(5),
    FLD_IRQ_SYSTEM_TIMER_LVL_EN  = BIT(6),
    FLD_IRQ_QDEC_EN              = BIT(7),
    FLD_IRQ_EP0_SETUP_EN         = BIT(8),
    FLD_IRQ_EP0_DAT_EN           = BIT(9),
    FLD_IRQ_EP0_STA_EN           = BIT(10),
    FLD_IRQ_SET_INTF_EN          = BIT(11),
    FLD_IRQ_EP_DATA_EN           = BIT(12), FLD_IRQ_SW_PWM_EN = BIT(12),
    FLD_IRQ_ZB_RT_EN             = BIT(13),
    FLD_IRQ_GPIO_RISC0_LVL_EN    = BIT(14),
    FLD_IRQ_GPIO_RISC1_LVL_EN    = BIT(15),
    FLD_IRQ_GPIO_OR0_EDGE_EN     = BIT(16),
    FLD_IRQ_GPIO_OR1_EDGE_EN     = BIT(17),
    FLD_IRQ_GPIO_OR2_EDGE_EN     = BIT(18),
    FLD_IRQ_PM_EN                = BIT(19),
    FLD_IRQ_SYSTEM_TIMER_EDG_EN  = BIT(20),
    FLD_IRQ_GPIO_RISC0_EDGE_EN   = BIT(21),
    FLD_IRQ_GPIO_RISC1_EDGE_EN   = BIT(22),
    FLD_IRQ_GPIO_RISC2_EDGE_EN   = BIT(23),
    FLD_IRQ_ALL                  = 0XFFFFFFFF,
} irq_list_e;
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
	FLD_SYSTEM_CMD_RUN   			=   BIT(0),
	FLD_SYSTEM_CMD_SYNC_TRIG 		=   BIT(1),
	FLD_SYSTEM_CMD_SET_TRIG 		=   BIT(2),
	FLD_SYSTEM_CMD_SYNC		        =   BIT(3),
	FLD_SYSTEM_MACHINE_STATUS		=   BIT(4),
	FLD_SYSTEM_RD_UPDATE			=   BIT(5),
	FLD_SYSTEM_32K_TIMER_BUSY       =   BIT(6),
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

/*******************************      dma registers: 0xc00      ******************************/
//uart rx
#define reg_dma0_addr           REG_ADDR16(0xc00)
#define reg_dma0_size           REG_ADDR8(0xc02)
#define reg_dma0_mode           REG_ADDR8(0xc03)

//uart tx
#define reg_dma1_addr           REG_ADDR16(0xc04)
#define reg_dma1_size           REG_ADDR8(0xc06)
#define reg_dma1_mode           REG_ADDR8(0xc07)

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
#define reg_dma2_addr           REG_ADDR16(0xc08)
#define reg_dma2_size           REG_ADDR8(0xc0a)
#define reg_dma2_mode           REG_ADDR8(0xc0b)
enum{
    FLD_DMA_RX_FIFO_MODE =      BIT_RNG(6,7)
};

//rf tx dma
#define reg_dma3_addr           REG_ADDR16(0xc0c)
#define reg_dma3_size           REG_ADDR8(0xc0e)
#define reg_dma3_mode           REG_ADDR8(0xc0f)
enum{
    FLD_DMA_AD_ACK =            BIT_RNG(6,7)
};
//not for adc
#define reg_dma4_addr			REG_ADDR16(0xc10)
#define reg_dma4_size			REG_ADDR8(0xc12)
#define reg_dma4_mode			REG_ADDR8(0xc13)

//adc rx dma
#define reg_dma5_addr			REG_ADDR16(0xc14)
#define reg_dma5_size			REG_ADDR8(0xc16)
#define reg_dma5_mode			REG_ADDR8(0xc17)

//pwm tx dma
#define reg_dma7_addr			REG_ADDR16(0xc18)
#define reg_dma7_size			REG_ADDR8(0xc1a)
#define reg_dma7_mode			REG_ADDR8(0xc1b)

#define reg_dma_t_addr          REG_ADDR16(0xc1c)
#define reg_dma_t_size          REG_ADDR8(0xc1e)

#define reg_dma_size(v)         REG_ADDR8(0xc02+4*v)
//  The default channel assignment
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
    FLD_DMA_TX_EN_BUFF =        BIT_RNG(2,3),
    FLD_DMA_RX_EN_COMMIT =      BIT(4),
};

#define reg_dma_tx_rdy0         REG_ADDR8(0xc24)
#define reg_dma_tx_rdy1         REG_ADDR8(0xc25)
#define reg_dma_rx_rdy0         REG_ADDR8(0xc26)
#define reg_dma_rx_rdy1         REG_ADDR8(0xc27)

#define reg_dma_irq_status      reg_dma_rx_rdy0
enum{
	FLD_DMA_CHN0 =	BIT(0),		FLD_DMA_CHN_UART_RX =	BIT(0),
	FLD_DMA_CHN1 =	BIT(1),		FLD_DMA_CHN_UART_TX =	BIT(1),
    FLD_DMA_CHN2 =  BIT(2),     FLD_DMA_CHN_RF_RX =     BIT(2),
    FLD_DMA_CHN3 =  BIT(3),     FLD_DMA_CHN_RF_TX =     BIT(3),
	FLD_DMA_CHN5 =	BIT(5),		FLD_DMA_CHN_SAR_ADC =   BIT(5),
	FLD_DMA_CHN7 =	BIT(7),		FLD_DMA_CHN_PWM  	 =	BIT(7),
};

typedef enum {
    FLD_DMA_IRQ_UART_RX  =  BIT(0),
    FLD_DMA_IRQ_UART_TX  =  BIT(1),
    FLD_DMA_IRQ_RF_RX =     BIT(2),
    FLD_DMA_IRQ_RF_TX =     BIT(3),
	FLD_DMA_IRQ_SAR_ADC =   BIT(4),
	FLD_DMA_IRQ_PWM		 =  BIT(7),
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

/********************************************************************************************
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 *****|								Aanlog  Register Table  						   |*****
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 ********************************************************************************************/

/*******************************      analog registers(3v3): 0x00      ***************************/

#define areg_0x00                   0x00

#define areg_0x01                   0x01

#define areg_0x03                   0x03
enum{
	//RSVD
    FLD_48M_DOUBLER_EN              = BIT(3),
};

#define areg_0x05                   0x05
enum{
	FLD_32K_RC_PD                   = BIT(0),
    FLD_24M_RC_PD                   = BIT(1),
	FLD_24M_XTL_PD                  = BIT(2),
	//RSVD
	FLD_PD_SPD_LDO                  = BIT(5),
	FLD_PD_RET_LDO                  = BIT(6),
};

#define areg_0x06                   0x06

#define areg_0x0a                   0x0a

#define areg_0x0d                   0x0d

#define areg_0x0e                   0x0e
enum{
    FLD_CAL_24M_RC_ENABLE           = BIT(0),
};

#define areg_0x16                   0x16
enum{
	//RSVD
    FLD_CAL_32K_DONE                = BIT(6),
    FLD_CAL_24M_DONE                = BIT(7),
};

#define areg_0x25                   0x25
enum{
    FLD_WAKEUP_PAD_EN               = BIT(0),
    FLD_WAKEUP_TIMER_EN             = BIT(1),
    FLD_WAKEUP_CORE_EN              = BIT(2),
    FLD_WAKEUP_SHUTDOWN_EN          = BIT(3),
};

#define areg_0x26                   0x26
enum{
    FLD_PD_RC32K_AUTO               = BIT(0),
    FLD_PD_XTAL24M_AUTO             = BIT(1),
	FLD_PD_PL_ALL_AUTO              = BIT(2),
	//RSVD
    FLD_PWDN_EN                     = BIT(6),
	FLD_ISO_EN                      = BIT(7),
};

#define areg_0x2c                   0x2c
typedef enum {
    FLD_WAKEUP_STATUS_PAD           = BIT(0),
    FLD_WAKEUP_STATUS_TIMER         = BIT(1),
    FLD_WAKEUP_STATUS_CORE          = BIT(2),

    //To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
    //instead of FLD_WAKEUP_STATUS_INUSE_ALL.
    FLD_WAKEUP_STATUS_ALL           = 0x07,

    //After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
    //whether the wake source in use has been cleared, because some of the wake sources
    //that are not in use may have been set up.
    FLD_WAKEUP_STATUS_INUSE_ALL     = 0x07,
}pm_wakeup_status_e;
enum {
	//RSVD
    FLD_RESET_32K_TIMER_CNT         = BIT(5),//W 1
    FLD_RESET_XTAL_QICK_START       = BIT(6),//W 1
	FLD_32K_WD_OVERFLOW_STATUS      = BIT(7),
};

#define areg_0x31                   0x31
enum{
	//RSVD
    FLD_32K_WATCHDOG_EN             = BIT(7),
};

/**
 * Customers cannot use analog register 0x35 because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot or power on has occurred. If this bit is 0, it means that sleep has occurred.
 * [Bit1~7]: These bits are used by the driver and cannot be used by the customer.
 */
#define PM_ANA_REG_WD_CLR_BUF0      0x35 // initial value 0xff.
enum{
	POWERON_REBOOT_OR_DEEP_FLAG     = BIT(0),
	FLASH_BIN                     	= BIT(1),
};

/**
 * Customers cannot use analog register 0x3a because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot has occurred.
 * REBOOT_MANUAL, it means that manually invoke the reboot interface.
 * REBOOT_XTAL_UNSTABLE, it means that the software calls the function sys_reboot() when the crystal oscillator does not start up normally.
 * REBOOT_PM_CLR_PLIC_REQUEST_FAIL, it means that the pm_sleep_wakeup function failed to clear the PM wake flag bit when using the deep wake source, and the software called sys_reboot().
 * Bit0 has been used and can no longer be used, any combination of other bits can be used.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF0 0x3a // initial value 0x00.
typedef enum{
    REBOOT_OR_POWERON_FLAG          = BIT(0),
    SW_REBOOT_REASON              	= BIT_RNG(1,7),
}pm_poweron_clr_buf0_e;
//Since there are only 7 bits, this enumeration ranges from 0 to 127.
typedef enum{
    APPLIC_INTERFACE              	= 0x00,
    XTAL_UNSTABLE                 	= 0x01,
    PM_CLR_PLIC_REQUEST_FAIL      	= 0x02,
    WAIT_TIMEOUT                  	= 0x03,
}pm_sw_reboot_reason_e;

#define areg_0x3d                   0x3d



/*******************************      adc digital registers: 0x00e0      ******************************/

#define ADC_BASE_ADDR    0x00e0

#define reg_adc_r_mux(i) REG_ADDR8(ADC_BASE_ADDR + (i))

enum
{
    FLD_P_INPUT_PIN = BIT_RNG(0, 2),
    FLD_N_INPUT_PIN = BIT_RNG(4, 6),
};

#define reg_adc_tsamp REG_ADDR8(ADC_BASE_ADDR + 0x03)

enum
{
    FLD_M_TSAMP = BIT_RNG(0, 3),
    FLD_L_TSAMP = BIT_RNG(4, 7),
};

#define reg_adc_r_tsamp REG_ADDR8(ADC_BASE_ADDR + 0x04)

enum
{
    FLD_R_TSAMP = BIT_RNG(0, 3),
    FLD_RX_FIFO_CLR = BIT(4),
    FLD_DMA_CNT_CLR = BIT(5),
};

#define reg_adc_vbat_div REG_ADDR8(ADC_BASE_ADDR + 0x05)

enum
{
    FLD_M_VBAT_DIV = BIT_RNG(0, 1),
    FLD_L_VBAT_DIV = BIT_RNG(2, 3),
    FLD_R_VBAT_DIV = BIT_RNG(4, 5),
};

#define reg_adc_channel_set_state(i) REG_ADDR8(ADC_BASE_ADDR + 0x06 + (i))
enum
{
    FLD_R_MAX_S      = BIT_RNG(0, 3),
    FLD_SEL_AI_SCALE = BIT_RNG(4, 5),
    FLD_SEL_VREF     = BIT_RNG(6, 7),
};

#define reg_rxfifo_wr_through   REG_ADDR8(ADC_BASE_ADDR + 0x09)
enum
{
    FLD_RXFIFO_WR_THROUGH = BIT(0),
};

#define reg_adc_capture_state(i) REG_ADDR16(ADC_BASE_ADDR + 0x0a + (i) * 0x02)

enum
{
    FLD_R_MAX_C_L = BIT_RNG(0, 7),
    FLD_R_MAX_C_H = BIT_RNG(8, 9),
};

#define reg_adc_config0           REG_ADDR8(ADC_BASE_ADDR + 0x10)

enum
{
    FLD_SCANT_MAX = BIT_RNG(4, 6),
    FLD_DMA_TRANS_EN =  BIT(7),
};

#define reg_adc_config1 REG_ADDR8(ADC_BASE_ADDR + 0x11)

enum
{
    FLD_SAR_ADC_CLK_DIV = BIT_RNG(0, 3),
    FLD_ADC_MODE        = BIT(5),
};

#define reg_adc_config2 REG_ADDR8(ADC_BASE_ADDR + 0x12)

enum
{
    FLD_M_CHANNEL_EN            = BIT(0),
    FLD_L_CHANNEL_EN            = BIT(1),
    FLD_R_CHANNEL_EN            = BIT(2),
    FLD_RX_IRQ_ENABLE           = BIT(4),
    FLD_CLK_EN                  = BIT(5),
	FLD_SAR_IRQ_RX_STATUS       = BIT(6),
    FLD_RX_FIFO_OVERRUN            = BIT(7),
};

#define reg_adc_rxfifo_trig_num REG_ADDR8(ADC_BASE_ADDR + 0x13)

enum
{
	FLD_RX_FIFO_NUM = BIT_RNG(0, 3),
    FLD_RXFIFO_TRIG_NUM = BIT_RNG(4, 6),
    FLD_SOFT_START = BIT(7),
};

#define reg_adc_rxfifo_dat  REG_ADDR32(ADC_BASE_ADDR + 0x14)

#define SAR_ADC_FIFO          (ADC_BASE_ADDR + 0x14)

#define reg_dma_length  REG_ADDR16(ADC_BASE_ADDR + 0x18)

/*******************************      adc analog registers: 0xec      ******************************/

#define areg_adc_res_m 0xec

enum
{
	FLD_ADC_RES_M         = BIT(0),
    FLD_ADC_EN_DIFF_CHN_M = BIT(6),
};

#define areg_adc_data_transfer_control  0xf2
enum
{
    FLD_AUTO_NOT_EN = BIT(3),
};

#define areg_adc_data_sample_control 0xf3

enum
{
    FLD_NOT_SAMPLE_ADC_DATA = BIT(0), //hold mode
    FLD_DWA_EN_O            = BIT(1),
    FLD_ANA_RD_EN           = BIT(2),
};

#define areg_adc_sample_clk_div 0xf4

enum
{
    FLD_ADC_SAMPLE_CLK_DIV = BIT_RNG(0, 3),
};

#define areg_m_chn_data_valid_status 0xf6

enum
{
    FLD_M_CHN_DATA_VALID_STATUS = BIT(0),
};

/** The areg_adc_misc register stores only the data for the M channel
 *  and will not store the data for the L and R channels.
 */
#define areg_adc_misc_l 0xf7
#define areg_adc_misc_h 0xf8

#define areg_adc_sel_atb_o 0xf9 //????????????????

#define areg_ain_scale  0xfa

enum
{
    FLD_ADC_ITRIM_PREAMP  = BIT_RNG(0, 1),
    FLD_ADC_ITRIM_VREFBUF = BIT_RNG(2, 3),
    FLD_ADC_ITRIM_VCMBUF  = BIT_RNG(4, 5),
};

#define areg_adc_pga_ctrl 0xfc

enum
{
    FLD_SAR_ADC_POWER_DOWN = BIT(5),
};



#if defined(__cplusplus)
}
#endif





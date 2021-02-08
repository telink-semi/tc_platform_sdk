/********************************************************************************************************
 * @file	bqb.c
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
#include "bqb.h"

#if(TEST_DEMO==BQB_DEMO)


static unsigned short pkt_cnt =0,cmd_pkt,l, h;
static unsigned char chn, pkt_type,freq,uart_tx_index,uart_rx_index,para, ctrl;
static unsigned char bb_mode;
static unsigned int pkt_interval;
static unsigned int tick_rx = 0;
volatile unsigned int t0,tick_tx;
Test_Status_t test_state;


unsigned char	bqbtest_buffer[256] __attribute__ ((aligned (4)));
unsigned char bqbtest_pkt [64] = {
	39, 0, 0, 0,
	0, 37,
	0, 1, 2, 3, 4, 5, 6, 7
};

static union pkt_length_u
{
	unsigned char len;
	struct len_t
	{
		unsigned char low:6;
		unsigned char upper:2;
	}l;
}pkt_length;

/**
 * @brief   This function serves to get ble channel index
 * @param   chn: input channel
 * @return  ble channel index
 */
unsigned char bqbtest_channel (unsigned char chn)
{
	if (chn == 0)
	{
		return 37;
	}
	else if (chn < 12)
	{
		return chn - 1;
	}
	else if (chn == 12)
	{
		return 38;
	}
	else if (chn < 39)
	{
		return chn - 2;
	}
	else
	{
		return 39;
	}
}

#if SUPPORT_CONFIGURATION
void rd_usr_definition()
{
	flash_read_page(ADDR_USR_DEFINITION, 1, &(usr_def_byte.usr_def));
}

void get_uart_port(UART_TxPinDef* bqb_uart_tx_port, UART_RxPinDef* bqb_uart_rx_port)
{
	switch(usr_def_byte.usr_def_t.uart)
	{
//	UART0
//	UART0_TX_PA3
#if (MCU_CORE_B87 || MCU_CORE_B85)
	case 0:
		*bqb_uart_tx_port = UART_TX_PA2;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 1:
		*bqb_uart_tx_port = UART_TX_PA2;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 2:
		*bqb_uart_tx_port = UART_TX_PA2;
		*bqb_uart_rx_port = UART_RX_PB7;
		break;
//	UART0_TX_PB2
	case 3:
		*bqb_uart_tx_port = UART_TX_PA2;
		*bqb_uart_rx_port = UART_RX_PC3;
		break;
	case 4:
		*bqb_uart_tx_port = UART_TX_PA2;
		*bqb_uart_rx_port = UART_RX_PC5;
		break;
	case 5:
		*bqb_uart_tx_port = UART_TX_PA2;
		*bqb_uart_rx_port = UART_RX_PD6;
		break;
//	UART0_TX_PD2
	case 6:
		*bqb_uart_tx_port = UART_TX_PB1;
		*bqb_uart_rx_port = UART_RX_PA0;;
		break;
	case 7:
		*bqb_uart_tx_port = UART_TX_PB1;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 8:
		*bqb_uart_tx_port = UART_TX_PB1;
		*bqb_uart_rx_port = UART_RX_PB7;
		break;
//	UART1
//	UART1_TX_PC6
	case 9:
		*bqb_uart_tx_port = UART_TX_PB1;
		*bqb_uart_rx_port = UART_RX_PC3;
		break;
	case 10:
		*bqb_uart_tx_port = UART_TX_PB1;
		*bqb_uart_rx_port = UART_RX_PC5;
		break;
	case 11:
		*bqb_uart_tx_port = UART_TX_PB1;
		*bqb_uart_rx_port = UART_RX_PD6;
		break;
//	UART1_TX_PD6
	case 12:
		*bqb_uart_tx_port = UART_TX_PC2;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 13:
		*bqb_uart_tx_port = UART_TX_PC2;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 14:
		*bqb_uart_tx_port = UART_TX_PC2;
		*bqb_uart_rx_port = UART_RX_PB7;
		break;
//	UART1_TX_PE0
	case 15:
		*bqb_uart_tx_port = UART_TX_PC2;
		*bqb_uart_rx_port = UART_RX_PC3;
		break;
	case 16:
		*bqb_uart_tx_port = UART_TX_PC2;
		*bqb_uart_rx_port = UART_RX_PC5;
		break;
	case 17:
		*bqb_uart_tx_port = UART_TX_PC2;
		*bqb_uart_rx_port = UART_RX_PD6;
		break;
	case 18:
		*bqb_uart_tx_port = UART_TX_PD0;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 19:
		*bqb_uart_tx_port = UART_TX_PD0;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 20:
		*bqb_uart_tx_port = UART_TX_PD0;
		*bqb_uart_rx_port = UART_RX_PB7;
		break;
//	UART1_TX_PE0
	case 21:
		*bqb_uart_tx_port = UART_TX_PD0;
		*bqb_uart_rx_port = UART_RX_PC3;
		break;
	case 22:
		*bqb_uart_tx_port = UART_TX_PD0;
		*bqb_uart_rx_port = UART_RX_PC5;
		break;
	case 23:
		*bqb_uart_tx_port = UART_TX_PD0;
		*bqb_uart_rx_port = UART_RX_PD6;
		break;
	case 24:
		*bqb_uart_tx_port = UART_TX_PD3;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 25:
		*bqb_uart_tx_port = UART_TX_PD3;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 26:
		*bqb_uart_tx_port = UART_TX_PD3;
		*bqb_uart_rx_port = UART_RX_PB7;
		break;
//	UART1_TX_PE0
	case 27:
		*bqb_uart_tx_port = UART_TX_PD3;
		*bqb_uart_rx_port = UART_RX_PC3;
		break;
	case 28:
		*bqb_uart_tx_port = UART_TX_PD3;
		*bqb_uart_rx_port = UART_RX_PC5;
		break;
	case 29:
		*bqb_uart_tx_port = UART_TX_PD3;
		*bqb_uart_rx_port = UART_RX_PD6;
		break;
	case 30:
		*bqb_uart_tx_port = UART_TX_PD7;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 31:
		*bqb_uart_tx_port = UART_TX_PD7;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 32:
		*bqb_uart_tx_port = UART_TX_PD7;
		*bqb_uart_rx_port = UART_RX_PB7;
		break;
//	UART1_TX_PE0
	case 33:
		*bqb_uart_tx_port = UART_TX_PD7;
		*bqb_uart_rx_port = UART_RX_PC3;
		break;
	case 34:
		*bqb_uart_tx_port = UART_TX_PD7;
		*bqb_uart_rx_port = UART_RX_PC5;
		break;
	case 35:
		*bqb_uart_tx_port = UART_TX_PD7;
		*bqb_uart_rx_port = UART_RX_PD6;
		break;
#elif (MCU_CORE_B89)
	case 0:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 1:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
	case 2:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
//	UART0_TX_PB2
	case 3:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
	case 4:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PB3;
		break;
	case 5:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PB4;
		break;
//	UART0_TX_PD2
	case 6:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PC0;;
		break;
	case 7:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 8:
		*bqb_uart_tx_port = UART_TX_PA1;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
//	UART1
//	UART1_TX_PC6
	case 9:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 10:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
	case 11:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
//	UART1_TX_PD6
	case 12:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PB3;
		break;
	case 13:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PB4;
		break;
	case 14:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PC0;
		break;
//	UART1_TX_PE0
	case 15:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 16:
		*bqb_uart_tx_port = UART_TX_PB0;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
	case 17:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 18:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
	case 19:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 20:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
//	UART1_TX_PE0
	case 21:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PB4;
		break;
	case 22:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PC0;
		break;
	case 23:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 24:
		*bqb_uart_tx_port = UART_TX_PB3;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
	case 25:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 26:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
//	UART1_TX_PE0
	case 27:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 28:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
	case 29:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PB3;
		break;
	case 30:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PC0;
		break;
	case 31:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 32:
		*bqb_uart_tx_port = UART_TX_PB4;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
//	UART1_TX_PE0
	case 33:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
	case 34:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
	case 35:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 36:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
	case 37:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PB3;
		break;
	case 38:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PB4;
		break;
//	UART0_TX_PB2
	case 39:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PC0;
		break;
	case 40:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 41:
		*bqb_uart_tx_port = UART_TX_PB6;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
//	UART0_TX_PD2
	case 42:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PA0;;
		break;
	case 43:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
	case 44:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
//	UART1
//	UART1_TX_PC6
	case 45:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
	case 46:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PB3;
		break;
	case 47:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PB4;
		break;
//	UART1_TX_PD6
	case 48:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 49:
		*bqb_uart_tx_port = UART_TX_PC0;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
	case 50:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PA0;
		break;
//	UART1_TX_PE0
	case 51:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PA5;
		break;
	case 52:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PB0;
		break;
	case 53:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PB1;
		break;
	case 54:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PB3;
		break;
	case 55:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PB4;
		break;
	case 56:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PC0;
		break;
//	UART1_TX_PE0
	case 57:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PC2;
		break;
	case 58:
		*bqb_uart_tx_port = UART_TX_PD4;
		*bqb_uart_rx_port = UART_RX_PD3;
		break;
#endif
	default:
		break;
	}
}
#endif

/**
 * @brief   This function serves to obtain the pkt interval in different payload lengths and different modes
 * @param   payload_len: payload length
 * @param   mode: tx mode
 * 				1:BLE1M
 * 				2:BLE2M
 * 				3:BLE125K
 * 				4:BLE500K
 * @return  the pkt interval
 */
unsigned int get_pkt_interval(unsigned char payload_len, unsigned char mode)
{
	unsigned int total_len,byte_time=8;
	unsigned char preamble_len;

	preamble_len = read_reg8(0x402) & 0x1f ;
	total_len = preamble_len + 4 + 2 + payload_len +3; // preamble + access_code + header + payload + crc

	if(mode==1)//1m
	{
		byte_time = 8;
	}
	else if(mode==2)//2m
	{
		byte_time = 4;
	}
	else if(mode==3)//s=8
	{
		byte_time = 64;
	}
	else if(mode==4) // s=2
	{
		byte_time = 16;
	}
	return (((byte_time * total_len + 249  + 624)/625)*625);

}


/**
 * @brief   This function serves to read the usrt data and execute BQB program
 * @param   Pointer to uart data
 * @return  1:  2 bytes data is received.
 * 			0:  no data is received.
 */
unsigned short uart_bqbtest_get(unsigned short* cmd)
{
	if (!tick_rx && ((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT) == 1))
	{
		tick_rx = reg_system_tick;
	}
	else if ((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT) == 2)
	{
		h = reg_uart_data_buf(uart_rx_index)&0xff;
		uart_rx_index++;
		uart_rx_index &= 0x03;
		l = reg_uart_data_buf(uart_rx_index)&0xff;
		uart_rx_index++;
		uart_rx_index &= 0x03;

		*cmd = (l | (h<<8));

		tick_rx = 0;
		return 1;
	}
	else if (tick_rx && clock_time_exceed(tick_rx, 5000))
	{
		tick_rx = 0;
		reg_uart_data_buf(uart_rx_index);
		uart_rx_index++;
		uart_rx_index &= 0x03;
	}
	else if((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT)>2)
	{
		unsigned char i;

		unsigned char l = REG_ADDR8(0x9c)&0x0f;
		for(i=0; i<l; i++)
		{
			reg_uart_data_buf(uart_rx_index);
			uart_rx_index++;
			uart_rx_index &= 0x03;
		}
	}
	return 0;
}

/**
 * @brief   This function serves to generate random packets that need to be sent in burst mode
 * @param   *p - the address of random packets.
 * @param   n - the number of random packets.
 * @return  none.
 */
static void rf_phy_test_prbs9 (unsigned char *p, int n)
{
	//PRBS9: (x >> 1) | (((x<<4) ^ (x<<8)) & 0x100)
	unsigned short x = 0x1ff;
	int i;
	int j;
	for ( i=0; i<n; i++)
	{
		unsigned char d = 0;
		for (j=0; j<8; j++)
		{
			if (x & 1)
			{
				d |= BIT(j);
			}
			x = (x >> 1) | (((x<<4) ^ (x<<8)) & 0x100);
		}
		*p++ = d;
	}
}


/**
 * @brief   This function serves to read the usrt data and execute BQB program
 * @param   none.
 * @return  none.
 */
void bqb_serviceloop (void)
{
	if (uart_bqbtest_get (&cmd_pkt))
	{
		unsigned short rsp=0;
		unsigned char cmd = cmd_pkt >> 14;
		unsigned char k;
		gpio_write(LED1, !gpio_read(LED1));
		tick_tx =  reg_system_tick;
		switch(cmd)
		{
			case CMD_SETUP:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 0);
				gpio_write(BQB_PA_TX_PORT, 0);
#endif
				ctrl = (cmd_pkt >> 8)&0x3f;
				para = (cmd_pkt >> 2)&0x3f;
				if(ctrl==0)
				{
					if(para==0)
					{
						pkt_length.l.upper =0;
						rsp = 0;
					}
					else
					{
						rsp = BIT(0);//Error
					}
#if (MCU_CORE_B85 || MCU_CORE_B87)
					rf_drv_init(RF_MODE_BLE_1M_NO_PN);
#elif(MCU_CORE_B89)
					rf_set_ble_1M_NO_PN_mode();
#endif
					rf_set_preamble_len(BQB_PREAMBLE_LEN+0x40); //add by junwei
				}
				else if(ctrl== 1)
				{
					if((para>=0) && (para<=3))
					{
						pkt_length.l.upper = para &0x03;
					}
					else
					{
						rsp = BIT(0); //Error
					}
				}
				else if(ctrl==2)
				{
					if(para==1)//BLE 1M
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_BLE_1M_NO_PN);
#elif(MCU_CORE_B89)
						rf_set_ble_1M_NO_PN_mode();
#endif
						rsp = 0;
					}
					else if(para==2)//BLE 2M
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_BLE_2M);
#elif(MCU_CORE_B89)
						rf_set_ble_2M_mode();
#endif

						rsp = 0;
					}
					else if(para==3)//s=8
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_LR_S8_125K);
#elif(MCU_CORE_B89)
						rf_set_ble_125K_mode();
#endif

						rsp = 0;
					}
					else if(para==4)//s=2
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_LR_S2_500K);
#elif(MCU_CORE_B89)
						rf_set_ble_500K_mode();
#endif

						rsp = 0;
					}
					else
					{
						rsp = BIT(0);
					}
					rf_pn_disable();
					bb_mode = para;
				}
				else if(ctrl==3)
				{
					rsp = 0;
				}
				else if(ctrl==4)
				{
					rsp |= BIT(1) | BIT(2);
					rsp = (rsp <<1);
				}
				else if(ctrl==5)
				{
					if(para==0)
					{
						rsp = (251<<1)&0x7ffe;
					}
					else if(para==1)
					{
						rsp = (17040 << 1)&0x7ffe;
					}
					else if(para==2)
					{
						rsp = (251<<1)&0x7ffe;
					}
					else if(para==3)
					{
						rsp = (17040 << 1)&0x7ffe;
					}
					else
					{
						rsp = BIT(0);//status EVENT Error
					}
				}
				pkt_cnt = 0;
				test_state = SETUP_STATE;
				bqb_uart_ndma_send_byte((rsp>>8)&0x7f);
				bqb_uart_ndma_send_byte(rsp&0xff);
				break;
			}
			case CMD_RX_TEST:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 1);
				gpio_write(BQB_PA_TX_PORT, 0);
#endif
				chn = (cmd_pkt >> 8) & 0x3f;	//frequency
				pkt_length.l.low  = (cmd_pkt >> 2) & 0x3f;
				pkt_type = cmd_pkt & 0x03;
				freq = bqbtest_channel(chn);//set channel

				rf_set_ble_channel(freq);
				rf_start_srx(reg_system_tick);
				bqb_uart_ndma_send_byte((rsp>>8)&0xff);
				bqb_uart_ndma_send_byte(rsp&0xff);
				pkt_cnt = 0;
				test_state = RX_STATE;
				break;
			}
			case CMD_TX_TEST:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 0);
				gpio_write(BQB_PA_TX_PORT, 1);
#endif
				chn = (cmd_pkt >> 8) & 0x3f;	//frequency
				pkt_length.l.low  = (cmd_pkt >> 2) & 0x3f;
				pkt_type = cmd_pkt & 0x03;
				if(pkt_length.len<=253)
				{
				bqbtest_pkt[0] = pkt_length.len + 2;
				bqbtest_pkt[1] = 0;
				}
				else
				{
					bqbtest_pkt[0] = pkt_length.len + 2-256;
					bqbtest_pkt[1] = 1;
				}

				bqbtest_pkt[5] = pkt_length.len ;
				if(pkt_type==0)
				{

					rf_phy_test_prbs9 (bqbtest_pkt + 6, pkt_length.len);
				}
				else if(pkt_type==1)
				{

					for(k=0;k<(pkt_length.len);k++)
					{
						bqbtest_pkt[k+6] = 0x0f;
					}

				}
				else if(pkt_type==2)
				{

					for(k=0;k<(pkt_length.len);k++)
					{
						bqbtest_pkt[k+6] = 0x55;
					}

				}
				else if(pkt_type==3)
				{
					pkt_type = 4;

					//todo if not LE Coded
					for(k=0;k<(pkt_length.len);k++)
					{
						bqbtest_pkt[k+6] = 0xff;
					}
				}
				else
				{
					rsp = BIT(0);
				}
				pkt_interval = get_pkt_interval(pkt_length.len, bb_mode);//TODO

				bqbtest_pkt[4] = pkt_type;
				freq = bqbtest_channel(chn);//set channel
				rf_set_tx_rx_off_auto_mode();
				rf_set_ble_channel(freq);
				rf_set_power_level_index(BQB_TX_POWER);
				test_state = TX_STATE;

				bqb_uart_ndma_send_byte((rsp>>8)&0xff);
				bqb_uart_ndma_send_byte(rsp&0xff);

				pkt_cnt = 0;
				break;
			}
			case CMD_END:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 0);
				gpio_write(BQB_PA_TX_PORT, 0);
#endif
				ctrl = (cmd_pkt >> 8)&0x3f;
				para = (cmd_pkt >> 2)&0x3f;

#if (!RETURN_TX_LEN_EN)
				if(test_state==TX_STATE)//The tx quantity does not need to be reported
					pkt_cnt = 0;
#endif
				test_state = END_STATE;
				if((ctrl==0) && (para==0))
				{
					pkt_length.len =0;
					rf_set_tx_rx_off_auto_mode();

					bqb_uart_ndma_send_byte((BIT(7))|((pkt_cnt>>8)&0x7f));
					bqb_uart_ndma_send_byte(pkt_cnt&0xff);

				}

				break;
			}
			default:
				break;
		}
	}
	if(test_state==RX_STATE)
	{
		if (rf_is_rx_finish())
		{
			rf_rx_finish_clear_flag();
			if(rf_is_rx_right())
			{
					pkt_cnt++;
			}
			rf_start_srx(reg_system_tick);
		}
	}
	else if(test_state==TX_STATE)
	{
		if (clock_time_exceed(tick_tx,pkt_interval) || (pkt_cnt == 0))//pkt_interval
		{
			tick_tx =  reg_system_tick;
			rf_set_tx_rx_off_auto_mode();
			rf_start_stx(bqbtest_pkt,reg_system_tick);
			pkt_cnt++;
		}
	}


}


/**
 * @brief   This function serves to initialize  BQB
 * @param   none.
 * @return  none.
 */
void  bqbtest_init(void)
{
	unsigned char chnidx=0;
	t0 = reg_system_tick;
	rf_phy_test_prbs9 (bqbtest_pkt + 6, 37);
	rf_rx_buffer_set(bqbtest_buffer,256,0);
	FSM_RX_FIRST_TIMEOUT_DISABLE;
#if (MCU_CORE_B85 || MCU_CORE_B87)
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);
#elif(MCU_CORE_B89)
	rf_mode_init();
	rf_set_ble_1M_NO_PN_mode();
#endif
	rf_set_preamble_len(BQB_PREAMBLE_LEN);
	rf_access_code_comm(0x29417671);
	uart_tx_index=0;
	uart_rx_index=0;
	flash_read_page(0x77000,1,&chnidx);
	if(chnidx!=0xff)
	{
		rf_update_internal_cap(chnidx);
	}
}

/**
 * @brief     uart send data function with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uartData - the data to be send.
 * @return    none
 */
void bqb_uart_ndma_send_byte(unsigned char uartData)
{
	int t;
//	static unsigned char uart_TxIndex = 0;

	t = 0;
	while( uart_tx_is_busy() && (t<0xfffff))
	{
		t++;
	}
	if(t >= 0xfffff)
		return;

	reg_uart_data_buf(uart_tx_index) = uartData;

	uart_tx_index++;
	uart_tx_index &= 0x03;// cycle the four register 0x90 0x91 0x92 0x93.
}



#endif

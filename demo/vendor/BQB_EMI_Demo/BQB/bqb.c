/********************************************************************************************************
 * @file    bqb.c
 *
 * @brief   This is the source file for Telink MCU
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
#include "bqb.h"

#if(TEST_DEMO==BQB_DEMO)

#if SUPPORT_CONFIGURATION
usr_def_t usr_config;
#endif


static unsigned char rxpara_flag  = 1;
static unsigned short pkt_cnt =0,cmd_pkt,l, h;
static unsigned char chn, pkt_type,freq,uart_tx_index,uart_rx_index,para, ctrl;
static unsigned char bb_mode;
static unsigned int pkt_interval;
static unsigned int tick_rx = 0;
volatile unsigned int t0,tick_tx;
Test_Status_t test_state;


unsigned char bqbtest_buffer[256] __attribute__ ((aligned (4)));
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
 * @brief   This function serves to read the uart data and execute BQB program
 * @param   Pointer to uart data
 * @return  1:  2 bytes data is received.
 * 			0:  no data is received.
 */
unsigned short uart_bqbtest_get(unsigned short* cmd)
{
	if (!tick_rx && ((REG_UART_BUF_CNT&FLD_UART_RX_BUF_CNT) == 1))
	{
		tick_rx = reg_system_tick;
	}
	else if ((REG_UART_BUF_CNT&FLD_UART_RX_BUF_CNT) == 2)
	{
		h = REG_UART_DATA_BUF(uart_rx_index)&0xff;
		uart_rx_index++;
		uart_rx_index &= 0x03;
		l = REG_UART_DATA_BUF(uart_rx_index)&0xff;
		uart_rx_index++;
		uart_rx_index &= 0x03;

		*cmd = (l | (h<<8));

		tick_rx = 0;
		return 1;
	}
	else if (tick_rx && clock_time_exceed(tick_rx, 5000))
	{
		tick_rx = 0;
		REG_UART_DATA_BUF(uart_rx_index);
		uart_rx_index++;
		uart_rx_index &= 0x03;
	}
	else if((REG_UART_BUF_CNT&FLD_UART_RX_BUF_CNT)>2)
	{
		unsigned char i;

		unsigned char l = REG_ADDR8(0x9c)&0x0f;
		for(i=0; i<l; i++)
		{
			REG_UART_DATA_BUF(uart_rx_index);
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

extern void read_bqb_calibration();
/**
 * @brief   This function serves to read the uartdata and execute BQB program
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

		tick_tx =  reg_system_tick;
		switch(cmd)
		{
			case CMD_SETUP:
			{
				bqb_pa_set_mode(3);
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
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X)
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
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X)
						rf_set_ble_1M_NO_PN_mode();
#endif
						rsp = 0;
					}
					else if(para==2)//BLE 2M
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_BLE_2M);
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X)
						rf_set_ble_2M_mode();
#endif

						rsp = 0;
					}
					else if(para==3)//s=8
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_LR_S8_125K);
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B)
						rf_set_ble_125K_mode();
#endif

						rsp = 0;
					}
					else if(para==4)//s=2
					{
#if (MCU_CORE_B85 || MCU_CORE_B87)
						rf_drv_init(RF_MODE_LR_S2_500K);
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B)
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
				UART_NDMA_SENT_BYTE((rsp>>8)&0x7f);
				UART_NDMA_SENT_BYTE(rsp&0xff);
				break;
			}
			case CMD_RX_TEST:
			{
				bqb_pa_set_mode(0);
				read_bqb_calibration();

				chn = (cmd_pkt >> 8) & 0x3f;	//frequency
				pkt_length.l.low  = (cmd_pkt >> 2) & 0x3f;
				pkt_type = cmd_pkt & 0x03;
				freq = bqbtest_channel(chn);//set channel

				rf_set_ble_channel(freq);
				rf_start_srx(reg_system_tick);

				sleep_us(30);
				if(rxpara_flag == 1)
				{
					rf_set_rxpara();
					rxpara_flag = 0;
				}

				if(freq == 10 || freq == 21 || freq == 33)
				{
					rf_ldot_ldo_rxtxlf_bypass_en();
				}
				else
				{
					rf_ldot_ldo_rxtxlf_bypass_dis();
				}

				UART_NDMA_SENT_BYTE((rsp>>8)&0xff);
				UART_NDMA_SENT_BYTE(rsp&0xff);
				pkt_cnt = 0;
				test_state = RX_STATE;
				break;
			}
			case CMD_TX_TEST:
			{
				bqb_pa_set_mode(1);
				read_bqb_calibration();

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
#if SUPPORT_CONFIGURATION
#if(MCU_CORE_B85||MCU_CORE_B87||MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X)
				rf_set_power_level_index((usr_config.power == 0)?BQB_TX_POWER:rf_power_Level_list[usr_config.power-1]);
#elif(MCU_CORE_B89)
				rf_set_power_level_index((usr_config.power == 0)?BQB_TX_POWER:(usr_config.power-1));
#endif
#else
				rf_set_power_level_index(BQB_TX_POWER);
#endif
				test_state = TX_STATE;

				UART_NDMA_SENT_BYTE((rsp>>8)&0xff);
				UART_NDMA_SENT_BYTE(rsp&0xff);

				pkt_cnt = 0;
				break;
			}
			case CMD_END:
			{
				bqb_pa_set_mode(3);
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

					/*Wait for the end of the state machine to prevent the state machine from being forced to stop during the process of sending and receiving packets
					 * to prevent abnormal interruptions from causing abnormal packet reception in S2/S8 mode.Modified by Pengcheng,confirmed by Qiangkai, 2022/07/19*/
					while(is_rf_receiving_pkt());

					rf_set_tx_rx_off_auto_mode();
					UART_NDMA_SENT_BYTE((BIT(7))|((pkt_cnt>>8)&0x7f));
					UART_NDMA_SENT_BYTE(pkt_cnt&0xff);

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
	t0 = reg_system_tick;
	rf_phy_test_prbs9 (bqbtest_pkt + 6, 37);
	rf_rx_buffer_set(bqbtest_buffer,256,0);
	FSM_RX_FIRST_TIMEOUT_DISABLE;
#if (MCU_CORE_B85 || MCU_CORE_B87)
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X)
	rf_mode_init();
	rf_set_ble_1M_NO_PN_mode();
#endif
	rf_set_preamble_len(BQB_PREAMBLE_LEN);
	rf_access_code_comm(ACCESS_CODE);
	uart_tx_index=0;
	uart_rx_index=0;
}


#if (MCU_CORE_B85 || MCU_CORE_B87 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X)
#define gpio_function_en(pin)			gpio_set_func((pin), AS_GPIO)
#define gpio_output_en(pin)				gpio_set_output_en((pin), 1)
#define gpio_output_dis(pin)			gpio_set_output_en((pin), 0)
#define gpio_input_en(pin)				gpio_set_input_en((pin), 1)
#define gpio_input_dis(pin)				gpio_set_input_en((pin), 0)
#define gpio_set_low_level(pin)			gpio_write((pin), 0)
#define gpio_set_high_level(pin)		gpio_write((pin), 1)
#endif

void bqb_pa_init()
{
	unsigned short tx_pin, rx_pin;
#if SUPPORT_CONFIGURATION
	tx_pin = get_pin(usr_config.pa_tx);
	rx_pin = get_pin(usr_config.pa_rx);
#else
	tx_pin = BQB_PA_TX_PORT;
	rx_pin = BQB_PA_RX_PORT;
#endif
	if(tx_pin == rx_pin) return;
	gpio_function_en(rx_pin);
	gpio_output_en(rx_pin);
	gpio_set_low_level(rx_pin);
	gpio_input_dis(rx_pin);
	gpio_function_en(tx_pin);
	gpio_output_en(tx_pin);
	gpio_input_dis(tx_pin);
	gpio_set_low_level(tx_pin);
}

void bqb_pa_set_mode(unsigned char rtx) //0:rx, 1:tx, other:off
{
	unsigned short tx_pin, rx_pin;
#if SUPPORT_CONFIGURATION
	tx_pin = get_pin(usr_config.pa_tx);
	rx_pin = get_pin(usr_config.pa_rx);
#else
	tx_pin = BQB_PA_TX_PORT;
	rx_pin = BQB_PA_RX_PORT;
#endif
	if(tx_pin == rx_pin) return;
	if(rtx == 0)
	{
		gpio_set_high_level(rx_pin);
		gpio_set_low_level(tx_pin);
	}
	else if(rtx == 1)
	{
		gpio_set_low_level(rx_pin);
		gpio_set_high_level(tx_pin);
	}
	else
	{
		gpio_set_low_level(rx_pin);
		gpio_set_low_level(tx_pin);
	}
}

#endif

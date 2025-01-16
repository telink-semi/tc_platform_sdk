/********************************************************************************************************
 * @file    app.c
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
#include "app_config.h"

// The demo uses 8bit and 20bit data as examples.
#if(MCU_CORE_B85)//Choose matching pins according to hardware design.
/**
 * @brief	Define and initialize the structure variable of the IO port related to Antsel.
 */
rf_ant_pin_sel_t ant_config =
{
	.antsel0_pin = RF_ANT_SEL0_PC5,
	.antsel1_pin = RF_ANT_SEL1_PC6,
	.antsel2_pin = RF_ANT_SEL2_PC7,
};
#else
/**
 * @brief	Define and initialize the structure variable of the IO port related to Antsel.
 */
rf_ant_pin_sel_t ant_config =
{
	.antsel0_pin = RF_ANT_SEL0_PD6,
	.antsel1_pin = RF_ANT_SEL1_PB0,
	.antsel2_pin = RF_ANT_SEL2_PB1,
};
#endif

#define BY_TSCRIPT			1
#define	BY_UART				2
#define USER_MODE			BY_TSCRIPT

#define TX     	1
#define RX		2
#define RF_TXRX_MODE 			RX


#define RF_FREQ					(RF_REAL_FREQ - 2400)
#define TX_PKT_PAYLOAD			0x12

#if (MCU_CORE_B85)
#define RF_POWER			RF_POWER_P10p46dBm
#elif (MCU_CORE_B87)
#define RF_POWER			RF_POWER_P11p26dBm
#define IQ_DATA_8_BIT_EN	0 /** 8bit, 16bit, 16bit(low),16bit(high),20bit IQ data.kite only supports 8bit iq data.*/
#endif
#define ACCESS_CODE				0Xfcaab2c1

#define RF_AOD_EN			1
#define	calibration_no_Z 	0

#define TX_INTERVAL_MS    100

unsigned char para_buff[commandBuffSize] = {0};
unsigned char temp_para[commandBuffSize] = {0};

volatile unsigned int rx_cnt=0;
volatile unsigned char tx_cnt=0;

unsigned int tick_now;
unsigned int angle_value;
unsigned char theta_angle;

unsigned char trans_buff[136] = {0x00};
unsigned char raw_data[90];
unsigned char trans_table[10];

unsigned char  rx_packet[280]  __attribute__ ((aligned (4))) = {0x00};
unsigned char  data_has_amplitude[256]  __attribute__ ((aligned (4))) = {0};
unsigned char  data_no_amplitude[256]  __attribute__ ((aligned (4))) = {0};


unsigned char  ble_tx_packet_AOD[48] __attribute__ ((aligned (4))) = {((TX_PKT_PAYLOAD+3)&0xff),(((TX_PKT_PAYLOAD+3)>>8)&0xff),0x00,0x00,0xed,(TX_PKT_PAYLOAD&0xff),0x94,0x30,
										0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41};

unsigned char  ble_tx_packet_AOA[48] __attribute__ ((aligned (4))) = {((TX_PKT_PAYLOAD+3)&0xff),(((TX_PKT_PAYLOAD+3)>>8)&0xff),0x00,0x00,0xed,(TX_PKT_PAYLOAD&0xff),0x14,0x30,
										0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41};

unsigned int rx_tick=0;
unsigned char tx_mode = 0;

int phase_rx_angle_cali_angle[8] = {0,-41,82,163,41,-122,122,122};
/**
 * Here the antenna serial number starts from 0, and the hardware design starts from the antenna serial number 1,
 * so the data in the table corresponds to adding 1 to the antenna number marked on the hardware.
 */
unsigned char antenna_switch_seq[8] = {0,1,2,3,4,5,6,7};
void user_init(void)
{
	gpio_init(1);

#if (MCU_CORE_B87 || MCU_CORE_B85)
	usb_set_pin(1);
#endif
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);

#if(MCU_CORE_B85)

	rf_aoa_aod_ant_init(8,&ant_config,SWITCH_SEQ_MODE0);
	rf_aoa_aod_sample_init(IQ_8_BIT_MODE,SAMPLE_AOA_4US_AOD_CTEINFO_INTERVAL,-4);
#else
	rf_aoa_aod_ant_init(8,&ant_config,SWITCH_SEQ_MODE0,antenna_switch_seq);
#if(IQ_DATA_8_BIT_EN)
	rf_aoa_aod_sample_init(IQ_8_BIT_MODE,SAMPLE_AOA_4US_AOD_CTEINFO_INTERVAL,-4);
#else
	rf_aoa_aod_sample_init(IQ_20_BIT_MODE,SAMPLE_AOA_4US_AOD_CTEINFO_INTERVAL,-4);
#endif
#endif

#if calibration_no_Z
	init_lookup_table_algorithm_no_Z();
#else
	init_lookup_table_algorithm_with_Z();
#endif


#if(RF_TXRX_MODE==TX)
	rf_aoa_aod_set_tx_mode(RF_TX_ACL_AOA_AOD_EN);
	rf_set_power_level_index (RF_POWER);
#elif(RF_TXRX_MODE==RX)
	rf_aoa_aod_set_rx_mode(RF_RX_ACL_AOA_AOD_EN);
#endif

	rf_set_tx_rx_off();
	rf_set_tx_rx_off_auto_mode();
	rf_set_channel(RF_FREQ,0);
	rf_access_code_comm(ACCESS_CODE);

#if(USER_MODE == BY_TSCRIPT)
	//2.initiate parameter buffer to receive CMD and parameter
	ParaBuf_Init((unsigned int)commandBuff,commandBuffSize,commandBuffCnt);
	//3.initiate result buffer to send the result
	ResuBuf_Init((unsigned int)resultBuff,resultBuffSize,resultBuffCnt);
#endif
}


void main_loop (void)
{
#if(RF_TXRX_MODE==TX)
	rf_set_txmode();
	while(1)
	{
		while((unsigned int)(clock_time()-tick_now) < 400000);
		tick_now = clock_time();
		tx_cnt++;
#if RF_AOD_EN
		ble_tx_packet_AOD[7]  =tx_cnt;
		rf_tx_pkt (ble_tx_packet_AOD);
#else
		ble_tx_packet_AOA[7]  =tx_cnt;
		rf_tx_pkt (ble_tx_packet_AOA);
#endif
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
	}



#elif(RF_TXRX_MODE==RX)
		rf_rx_buffer_set((unsigned char*)rx_packet,296, 0);
		rf_set_rxmode ();
		sleep_us(85);
		rx_tick = clock_time();
	while(1)
	{
#if(USER_MODE == BY_TSCRIPT)
		if(is_ParaBuf_HaveData() != 0)
		{

			ParaBuf_Read(para_buff,commandBuffSize);

			if(para_buff[0] == 0x33)
			{
				trans_table[1] = RF_FREQ;
				trans_table[2] = ACCESS_CODE&0xff;
				trans_table[3] = (ACCESS_CODE>>8)&0xff;
				trans_table[4] = (ACCESS_CODE>>16)&0xff;
				trans_table[5] = (ACCESS_CODE>>24)&0xff;
				trans_table[6] = 2;
#if(MCU_CORE_B85)
				trans_table[7] = 1;
#else
				trans_table[7] = IQ_DATA_8_BIT_EN;
#endif
				trans_table[8] = (TX_PKT_PAYLOAD&0xff);
				ResuBuf_Write(trans_table, 32);
				break;
			}

		 if(para_buff[0] == 0x55)
#endif
			{
				while(1)
				{
#if(USER_MODE == BY_TSCRIPT)
					if(is_ParaBuf_HaveData() != 0)
					{
						ParaBuf_Read(para_buff,commandBuffSize);
						if(para_buff[0] == 0x11)
						{
							while(is_rf_receiving_pkt());
							rf_set_tx_rx_off();
							Result_Buff_Clear();
							break;
						}

					}
#endif
					if(rf_is_rx_finish())
					{

					  if(rf_is_rx_right()&&rf_aoa_aod_is_rx_pkt_len_ok(rx_packet))
						{
							rx_cnt++;
							rx_tick = clock_time();
							for(unsigned char i=0;i<rf_aoa_aod_iq_data_offset(rx_packet);i++)
							{
								data_has_amplitude[i] = rx_packet[i];
								data_no_amplitude[i] = rx_packet[i];
							}

#if(IQ_DATA_8_BIT_EN||MCU_CORE_B85)

#else
							frond_end(&rx_packet[rf_aoa_aod_iq_data_offset(rx_packet)], &data_has_amplitude[rf_aoa_aod_iq_data_offset(rx_packet)], &data_no_amplitude[rf_aoa_aod_iq_data_offset(rx_packet)]);
							for(unsigned int i=0;i<9;i++)
							{
								data_has_amplitude[rf_aoa_aod_iq_group_number(rx_packet)*2+rx_packet[5]+6+i] = rx_packet[rf_aoa_aod_hdinfo_offset(rx_packet)+i];
								data_no_amplitude[rf_aoa_aod_iq_group_number(rx_packet)*2+rx_packet[5]+6+i] = rx_packet[rf_aoa_aod_hdinfo_offset(rx_packet)+i];
							}
#endif


#if calibration_no_Z
							angle_value = raw_data_to_angle_no_z(&rx_packet[0]);
#else
#if(IQ_DATA_8_BIT_EN||MCU_CORE_B85)
					angle_value = raw_data_to_angle_with_z(&rx_packet[0],(unsigned char *)&theta_angle);
#else
					angle_value = raw_data_to_angle_with_z(&data_no_amplitude[0],(unsigned char *)&theta_angle);
#endif
#if(USER_MODE == BY_UART)
						printf("theta = %d\n",temp_theta);
#endif
#endif
#if(USER_MODE == BY_UART)
					get_raw_data(&raw_data[0],&data_has_amplitude[0],90);
					printf("angle = %d\n",angle_value);
					sleep_us(150);
#else
							for(unsigned int i=0;i<TX_PKT_PAYLOAD+110;i++)
							{
#if(IQ_DATA_8_BIT_EN||MCU_CORE_B85)
								trans_buff[i] = rx_packet[i];
#else
								trans_buff[i] = data_has_amplitude[i];
#endif
							}

							trans_buff[TX_PKT_PAYLOAD+109] = angle_value/256;
							trans_buff[TX_PKT_PAYLOAD+110] = angle_value%256;

							trans_buff[TX_PKT_PAYLOAD+117] =  theta_angle;
							ResuBuf_Write(trans_buff, TX_PKT_PAYLOAD+118);
#endif


							rf_rx_finish_clear_flag();

						}
						else
						{
							rf_set_tx_rx_off();
							rf_set_rxmode ();
							sleep_ms(100);
						}

					}

				}
			}
#if(USER_MODE == BY_TSCRIPT)
			else
			{
				sleep_us(10);
			}
		}
#endif
	}

#endif
}





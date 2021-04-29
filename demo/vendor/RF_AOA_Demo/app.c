/********************************************************************************************************
 * @file	app.c
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
#include "app_config.h"

#define BY_TSCRIPT			1
#define	BY_UART				2
#define USER_MODE			BY_TSCRIPT

#define TX     	1
#define RX		2
#define RF_TXRX_MODE 			RX


#define LED1     				GPIO_PB1
#define LED2     				GPIO_PA3
#define LED3     				GPIO_PA4
#define LED4     				GPIO_PB0
#define SW2     				GPIO_PC1
#define SW3     				GPIO_PC0


#define RF_FREQ					(RF_REAL_FREQ - 2400)
#define TEST_CHN   				1		// 37/38/39 adv channel
#if (MCU_CORE_B85)
#define RF_POWER			RF_POWER_P10p46dBm
#elif (MCU_CORE_B87)
#define RF_POWER			RF_POWER_P11p26dBm
#endif
#define ACCESS_CODE				0Xfcaab2c1

#define RF_AOD_EN			1
#define RF_AOA_1US_EN  	 	0
#define	calibration_no_Z 	0
#define IQ_DATA_8_BIT_EN	1

volatile unsigned int rx_cnt=0;
volatile unsigned char tx_cnt=0;
unsigned int full_count = 0;
unsigned int ttest1,ttest2;

unsigned char full_flag = 0;
unsigned int addr;
unsigned int tick_now;
char phase_value;
unsigned int angle_value;
unsigned char sym;
extern unsigned int rsquare;
//extern int minval;
extern int temp_theta;
unsigned int rsquare_value;

unsigned char trans_buff[136] = {0x00};
unsigned char raw_data[90];
unsigned char trans_table[10];
#if(RF_AOA_1US_EN)
unsigned char  rx_packet[256]  __attribute__ ((aligned (4)));
#else
unsigned char  rx_packet[280]  __attribute__ ((aligned (4))) = {0x00};
unsigned char  temp_packet[280]  __attribute__ ((aligned (4))) = {0};
unsigned char  data_has_amplitude[256]  __attribute__ ((aligned (4))) = {0};
unsigned char  data_no_amplitude[256]  __attribute__ ((aligned (4))) = {0};
#endif

unsigned char  ble_tx_packet_AOD[48] __attribute__ ((aligned (4))) = {0x15,0x00,0x00,0x00,0xed,0x12,0x94,0x30,
										0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41};

unsigned char  ble_tx_packet_AOA[48] __attribute__ ((aligned (4))) = {0x15,0x00,0x00,0x00,0xed,0x12,0x14,0x30,
										0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41};

#define TX_INTERVAL_MS    100

unsigned int rx_tick=0;

int phase_diff[4]={0, 918, 462, 207};
int phase_rx_angle_cali_angle[8] = {0,-41,82,163,41,-122,122,122};
unsigned char antenna_switch_seq[16] = {1,2,1,3,1,3,1,2,1,2,2,1,2,2,1,3};

void init_test_gpio(void)
{
#if (MCU_CORE_B87 || MCU_CORE_B85)
	gpio_set_func(GPIO_PA5, AS_USB);
	gpio_set_func(GPIO_PA6, AS_USB);
	usb_dp_pullup_en (1);
#endif

	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); //enable output
	gpio_set_input_en(LED2 ,0);//disable input
	gpio_write(LED2,1);              //LED On

}


void user_init()
{
	gpio_init(0);
	init_test_gpio();
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);

#if (MCU_CORE_B87)
	supp_ant_lut(antenna_switch_seq,SWITCH_SEQ_MODE0);
	rx_dly_dis();
#endif

	WRITE_REG8(0x43b,0x29);// forward 0.5us.  default:0x2d , add 1 means sample point delay 0.125us.

#if (MCU_CORE_B87)
	triangle_all_open();
	set_antenna_num(7);
#else
	ant_all_open();
#endif
#if calibration_no_Z
	init_lookup_table_algorithm_no_Z();
#else
//	init_lookup_table();
	init_lookup_table_algorithm_with_Z();
#endif


#if(RF_AOA_1US_EN)
	WRITE_REG8 (0x43c, ( READ_REG8(0x43c)|0x30 ));
#endif

#if(IQ_DATA_8_BIT_EN)
#else
	write_reg8(0x43d, (read_reg8(0x43d) & 0x8f) | 0x40);   //20bit
//	write_reg8(0x43d, (read_reg8(0x43d) & 0x8f) );   //20bit
#endif

#if(RF_TXRX_MODE==TX)
	sub_wr(0x438, 1, 2, 2); //supp_tx_dat_en
	rf_set_power_level_index (RF_POWER);
#elif(RF_TXRX_MODE==RX)
	sub_wr(0x438, 1, 0, 0); //supp_rx_dat_en
#endif
	rf_set_tx_rx_off();
	WRITE_REG8(0xf00, 0x80);
	rf_set_channel(RF_FREQ,0);
	rf_access_code_comm(ACCESS_CODE);
#if(USER_MODE == BY_UART)
	WRITE_REG8(0x43b,0x29);					// adjust sample point location .
#else
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
		//gpio_toggle(LED2);
#else
		ble_tx_packet_AOA[7]  =tx_cnt;
		rf_tx_pkt (ble_tx_packet_AOA);
#endif
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
	}



#elif(RF_TXRX_MODE==RX)

#if(RF_AOA_1US_EN)
		rf_rx_buffer_set((unsigned char*)rx_packet,256, 0);
#else
		rf_rx_buffer_set((unsigned char*)rx_packet,296, 0);
#endif
#if(USER_MODE == BY_TSCRIPT)
	trans_table[1] = RF_FREQ;
	trans_table[2] = READ_REG8(0x408);
	trans_table[3] = READ_REG8(0x409);
	trans_table[4] = READ_REG8(0x40a);
	trans_table[5] = READ_REG8(0x40b);
	trans_table[6] = 2;

	memcpy((char *)(0x84c350), &trans_table[0], 7);

	write_reg8(0x40005, 0x00);
	while(read_reg8(0x40005) == 0x00);
#endif
	rf_set_rxmode ();
	rx_tick = clock_time();

	while(1)
	{
		if(rf_is_rx_finish())
		{
#if(RF_AOA_1US_EN)
			if((rx_packet[0] == 0xC4) && ((REG_ADDR8(0x44f)&0x0f)==0)&& (rx_packet[4]==0xed))
#else
#if(IQ_DATA_8_BIT_EN)
					if((rx_packet[0] == 0x7a) && ((REG_ADDR8(0x44f)&0x0f)==0)&& (rx_packet[4]==0xed))
#else

					if((rx_packet[0] == 0x01) && ((REG_ADDR8(0x44f)&0x0f)==0)&& (rx_packet[4]==0xed) && (rx_packet[260]==0x10))
#endif
#endif
			{
#if(USER_MODE == BY_TSCRIPT)
				ttest1++;
				rf_set_tx_rx_off();
				rx_cnt++;
				rx_tick = clock_time();
				addr = Get_Next_Packet();
				if(addr != 0)
				{
					full_flag = 0;
#endif
#if(RF_AOA_1US_EN)
					memcpy((char *)(0x840000+addr), rx_packet, 256);
#else

					for(unsigned char i=0;i<28;i++)
					{
//						temp_packet[i] = rx_packet[i];
						data_has_amplitude[i] = rx_packet[i];
						data_no_amplitude[i] = rx_packet[i];
					}
#if(IQ_DATA_8_BIT_EN)
#else
					frond_end(&rx_packet[28], &data_has_amplitude[28], &data_no_amplitude[28]);
#endif
					for(unsigned int i=0;i<9;i++)
					{
//						temp_packet[i] = rx_packet[253+i];
						data_has_amplitude[118+i] = rx_packet[253+i];
						data_no_amplitude[118+i] = rx_packet[253+i];
					}
#if calibration_no_Z
					angle_value = raw_data_to_angle_no_z(&data_no_amplitude[0]);
#else
#if(IQ_DATA_8_BIT_EN)
					angle_value = raw_data_to_angle_with_z(&rx_packet[0]);
//					angle_value = raw_data_to_angle_with_z(&temp_packet[0]);
#else
					angle_value = raw_data_to_angle_with_z(&data_no_amplitude[0]);
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


					for(unsigned int i=0;i<128;i++)
					{
#if(IQ_DATA_8_BIT_EN)
						trans_buff[i] = rx_packet[i];
#else
						trans_buff[i] = data_has_amplitude[i];
#endif
					}

					trans_buff[127] = angle_value/256;
					trans_buff[128] = angle_value%256;

#endif


#if(USER_MODE == BY_TSCRIPT)
					trans_buff[135] = temp_theta;

					memcpy((char *)(0x840000+addr), &trans_buff[0], 136);
#endif
#endif
#if(USER_MODE == BY_TSCRIPT)
					write_reg8(0x40004, 0x11);
					sleep_ms(1);
					while(read_reg8(0x40004) != 0x55 && read_reg8(0x40005) != 0x33);

					rf_rx_finish_clear_flag();
					rf_set_rxmode ();
				}

				else
				{
					rf_set_rxmode ();
					full_count++;
					sleep_ms(100);
					full_flag = 1;
				}
			}
		}
		else
		{
			sleep_us(10);
		}
#else
		rx_cnt++;
				}
				rf_rx_finish_clear_flag();
				rf_set_tx_rx_off_auto_mode();
		}

#endif
	}

#endif
}






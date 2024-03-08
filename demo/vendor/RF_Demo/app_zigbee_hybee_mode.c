/********************************************************************************************************
 * @file	app_zigbee_hybee_mode.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
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
#if(RF_MODE==RF_ZIGBEE_250K || RF_MODE==RF_HYBEE_1M || RF_MODE==RF_HYBEE_2M || RF_MODE==RF_HYBEE_500K)

#define TX					1
#define RX					2
#define RF_TX_RX_MODE			RX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		AUTO

#define RF_FREQ				71

#define ACCESS_CODE			0x29417671
volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;
unsigned char  rx_packet[64]  __attribute__ ((aligned (4)));
unsigned char  Zigbee_tx_packet[48] __attribute__ ((aligned (4))) = {0x12,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

#if(RF_AUTO_MODE == AUTO)

#define TX_INTERVAL_MS    1

void user_init()
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);	 //disable input
	gpio_write(LED1, 0);         //LED On

	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off_auto_mode();
	rf_set_channel(RF_FREQ,0);

}

void main_loop (void)
{
#if(RF_TX_RX_MODE==TX)
	while(1)
	{
		sleep_ms(1);
		rf_start_stx (Zigbee_tx_packet, clock_time() + 16*1000*TX_INTERVAL_MS);
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
		tx_cnt++;
	}


#elif(RF_TX_RX_MODE==RX && !MCU_CORE_B89 && !MCU_CORE_B80)
	rf_rx_buffer_set(rx_packet,64, 0);
	rf_start_srx(clock_time() + 100*16);

	while(1)
	{
		if(rf_is_rx_finish())
		{
			if(RF_ZIGBEE_PACKET_CRC_OK(rx_packet)&&RF_ZIGBEE_PACKET_LENGTH_OK(rx_packet))
			{
				gpio_toggle(LED1);
				rx_cnt++;
			}
			rf_rx_finish_clear_flag();
			rf_set_tx_rx_off_auto_mode();
			rf_start_srx(clock_time() + 100*16);
		}
	}
#endif
}


#elif(RF_AUTO_MODE == MANUAL)
void user_init()
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);//disable input
	gpio_write(LED1, 0);              //LED On

	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off();
	rf_set_channel(RF_FREQ,0);

}

void main_loop (void)
{
#if(RF_TX_RX_MODE==TX)
	rf_set_txmode();
	while(1)
	{
		sleep_ms(1);
		rf_tx_pkt (Zigbee_tx_packet);
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
		gpio_toggle(LED1);
		tx_cnt++;
	}


#elif(RF_TX_RX_MODE==RX && !MCU_CORE_B89 && !MCU_CORE_B80)
	rf_rx_buffer_set(rx_packet,64, 0);
	rf_set_rxmode ();

	while(1)
	{
		if(rf_is_rx_finish())
		{
			if(RF_ZIGBEE_PACKET_CRC_OK(rx_packet)&&RF_ZIGBEE_PACKET_LENGTH_OK(rx_packet))
			{
				gpio_toggle(LED1);
				rx_cnt++;
			}
			rf_rx_finish_clear_flag();
			rf_set_tx_rx_off_auto_mode();
		}
	}
#endif
}
#endif
#endif

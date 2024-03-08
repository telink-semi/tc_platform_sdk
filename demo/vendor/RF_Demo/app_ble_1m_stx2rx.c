/********************************************************************************************************
 * @file	app_ble_1m_stx2rx.c
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

#if(RF_MODE==RF_BLE_1M_STX2RX)
#define TX_FIRST				1
#define RX_FIRST				2
#define RF_STRX_MODE			TX_FIRST

#define STX_WAITTIME_US         4000
#define SRX_WAITTIME_US         10000
#define TX_DELAY_US             10

#define RF_FREQ					35

#define ACCESS_CODE				0x29417671

#define PTA_NONE				0
#define PTA_2WIRE				1
#define PTA_3WIRE				2
#define PTA_MODE				PTA_NONE
#define WLAN_ACTIVE_PIN			GPIO_PB5

#if(MCU_CORE_B87)
#define BLE_PRIORITY_PIN		PTA_BLE_PRIORITY_PB3
#define BLE_ACTIVE_PIN			PTA_BLE_ACTIVE_PB3
#define BLE_STATUS_PIN			PTA_BLE_STATUS_PB4
#define WLAN_DENY_PIN			PTA_WLAN_DENY_PB5
#elif(MCU_CORE_B89)
#define BLE_PRIORITY_PIN		PTA_BLE_PRIORITY_PA3
#define BLE_ACTIVE_PIN			PTA_BLE_ACTIVE_PA3
#define BLE_STATUS_PIN			PTA_BLE_STATUS_PA4
#define WLAN_DENY_PIN			PTA_WLAN_DENY_PA6
#elif(MCU_CORE_B80 || MCU_CORE_B80B)
#define	BLE_PRIORITY_PIN		GPIO_PA3
#define BLE_ACTIVE_PIN			GPIO_PA3
#define BLE_STATUS_PIN			GPIO_PA4
#define WLAN_DENY_PIN			GPIO_PA6
#endif
volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;
volatile unsigned int timeout_cnt=0;
volatile unsigned int tx_state=0;
volatile unsigned int rx_state=0;
volatile unsigned int timeout_state=0;
unsigned char  rx_packet[64]  __attribute__ ((aligned (4)));
unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) = {0x23,0x00,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};



void user_init()
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1|LED2|LED3,AS_GPIO);
	gpio_set_output_en(LED1|LED2|LED3, 1); //enable output
	gpio_set_input_en(LED1|LED2|LED3 ,0);	 //disable input
	gpio_write(LED1|LED2|LED3, 0);         //LED Off

	rf_set_power_level_index (RF_POWER);
	rf_trx_state_set(RF_MODE_AUTO,RF_FREQ);
	rf_rx_buffer_set(rx_packet,64, 0);

#if(!MCU_CORE_B85)
#if (PTA_MODE == PTA_2WIRE)
	rf_2wire_pta_init(BLE_PRIORITY_PIN,WLAN_ACTIVE_PIN,PTA_BLE_PRIORITY_TRX);
#elif(PTA_MODE == PTA_3WIRE)
	rf_3wire_pta_init(BLE_ACTIVE_PIN,BLE_STATUS_PIN,WLAN_DENY_PIN,PTA_BLE_STATUS_RX);
#endif
#endif

#if(RF_STRX_MODE==TX_FIRST)
    irq_disable();
    irq_clr_src();
    irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
    rf_irq_disable(FLD_RF_IRQ_ALL);
    rf_irq_enable(FLD_RF_IRQ_TX|FLD_RF_IRQ_RX|FLD_RF_IRQ_RX_TIMEOUT);//stx2rx irq interrupt
    irq_enable(); //enable general irq
#elif(RF_STRX_MODE==RX_FIRST)
    irq_disable();
    irq_clr_src();
    irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
    rf_irq_disable(FLD_RF_IRQ_ALL);
    rf_irq_enable(FLD_RF_IRQ_TX|FLD_RF_IRQ_RX|FLD_RF_IRQ_FIRST_TIMEOUT);//srx2tx irq interrupt
    irq_enable(); //enable general irq
#endif

    rf_access_code_comm(ACCESS_CODE);
}

void main_loop (void)
{
#if(RF_STRX_MODE==TX_FIRST)
	rf_set_rx_timeout(SRX_WAITTIME_US);
	while(1)
	{
#if ((PTA_MODE == PTA_2WIRE)&&(!MCU_CORE_B85))
		while(gpio_read(WLAN_ACTIVE_PIN));
#endif
		tx_state=0;
		rx_state=0;
		timeout_state=0;
		sleep_ms(100);
		rf_start_stx2rx (ble_tx_packet, clock_time()+16*TX_DELAY_US);
		while(1)
		{
			if(tx_state==1)//tx
			{
				tx_state = 0;
				tx_cnt++;
			}
			else if(rx_state==1)//rx
			{
				if(RF_BLE_PACKET_CRC_OK(rx_packet)&&RF_BLE_PACKET_LENGTH_OK(rx_packet))
				rx_cnt++;
				break;

			}
			else if(timeout_state==1)//time out
			{
				timeout_cnt++;
				break;
			}
#if ((PTA_MODE == PTA_3WIRE)&&(!MCU_CORE_B85))
			else if(rf_irq_src_get() & FLD_RF_IRQ_WIFI_DENY)
			{
				rf_irq_clr_src(FLD_RF_IRQ_WIFI_DENY);
				break;
			}
#endif
		}
	}


#elif(RF_STRX_MODE==RX_FIRST)

	while(1)
	{
#if ((PTA_MODE == PTA_2WIRE)&&(!MCU_CORE_B85))
		while(gpio_read(WLAN_ACTIVE_PIN));
#endif
		tx_state=0;
		rx_state=0;
		timeout_state=0;
		rf_start_srx2tx (ble_tx_packet, clock_time()+16*TX_DELAY_US );
		while(1)
		{
			if(tx_state==1)//tx
			{
				tx_state = 0;
				tx_cnt++;
				break;
			}
			else if(rx_state==1)//rx (check crc & length)
			{
				rx_state=0;
				if(RF_BLE_PACKET_CRC_OK(rx_packet)&&RF_BLE_PACKET_LENGTH_OK(rx_packet))
				rx_cnt++;
			}
			else if(timeout_state==1)//time out
			{
				timeout_state = 0;
				timeout_cnt++;
				break;
			}
#if ((PTA_MODE == PTA_3WIRE)&&(!MCU_CORE_B85))
			else if(rf_irq_src_get() & FLD_RF_IRQ_WIFI_DENY)
			{
				rf_irq_clr_src(FLD_RF_IRQ_WIFI_DENY);
				break;
			}
#endif
		}
	}
#endif
}

#endif



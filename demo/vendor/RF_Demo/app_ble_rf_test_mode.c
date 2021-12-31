/********************************************************************************************************
 * @file	app_ble_rf_test_mode.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2018
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


#if (RF_MODE == RF_BLE_SDK_TEST)
#define TEST_CHN   				1		// 37/38/39 adv channel

#define BLE_ACCESS_CODE			0x9A3CC36A//0xA5CC336A//0xd6be898e//


#define TEST_MAC				0xEE

#define TX_INTERVAL_TICK		(10*CLOCK_16M_SYS_TIMER_CLK_1MS)   //10ms
#define TX_INTERVAL_US			(TX_INTERVAL_TICK/16)


#define TEST_MODE_MANUAL_TX		1
#define TEST_MODE_STX			2
#define TEST_MODE_BTX_TX		3  //test TX only, do not care RX of BTX
#define TEST_MODE_BTX			4

#define TEST_MODE_MANUAL_RX	    10
#define TEST_MODE_SRX	    	11
#define TEST_MODE_BRX_RX	    12	//test RX only, do not care TX of BRX
#define TEST_MODE_BRX	    	13

#define TEST_MODE_SELECT		TEST_MODE_BRX_RX







#define DEBUG_GPIO_ENABLE							1

#if(DEBUG_GPIO_ENABLE)

	#define GPIO_CHN0							GPIO_PB0
	#define GPIO_CHN1							GPIO_PB1
	#define GPIO_CHN2							GPIO_PB6
	#define GPIO_CHN3							GPIO_PB7

	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)

#else

	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE

#endif






void ble_manual_tx_test(void);
void ble_stx_test(void);
void ble_btx_tx_test(void);

void ble_manual_rx_test(void);
void ble_brx_rx_test(void);

typedef enum advChannelPDUType_e {
	LL_TYPE_ADV_IND = 0x00,
	LL_TYPE_ADV_DIRECT_IND = 0x01,
	LL_TYPE_ADV_NONCONN_IND = 0x02,
	LL_TYPE_SCAN_REQ = 0x03,
	LL_TYPE_SCAN_RSP = 0x04,
	LL_TYPE_CONNNECT_REQ = 0x05,
	LL_TYPE_ADV_SCAN_IND = 0x06,
} advChannelPDUType_t;


typedef struct {
	unsigned char type   :4;
	unsigned char rfu1   :2;
	unsigned char txAddr :1;
	unsigned char rxAddr :1;
}rf_adv_head_t;

typedef struct{
	unsigned long dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	rf_adv_head_t  header;	//RA(1)_TA(1)_RFU(2)_TYPE(4)
	unsigned char  rf_len;				//LEN(6)_RFU(2)

	unsigned char	advA[6];			//address
	unsigned char	data[31];			//0-31 byte
}rf_packet_adv_t;

/*
 * app_rf_test.c
 *
 *  Created on: 2018-1-11
 *      Author: Administrator
 */


rf_packet_adv_t	debug_pkt_adv = {
		sizeof (rf_packet_adv_t) - 4,		// dma_len
		{LL_TYPE_ADV_NONCONN_IND, 0, 0, 0},					// type
		sizeof (rf_packet_adv_t) - 6,		// rf_len
		{TEST_MAC, TEST_MAC, TEST_MAC, TEST_MAC, TEST_MAC, TEST_MAC},	// advA
		// data
};

unsigned char debug_pkt_adv_2[30] = { 10,0,0,0,  0, 8, TEST_MAC,TEST_MAC,TEST_MAC,TEST_MAC,TEST_MAC, TEST_MAC,0x01};






void user_init(void)
{
#if(DEBUG_GPIO_ENABLE)
	gpio_set_func(GPIO_CHN0, AS_GPIO);
	gpio_set_func(GPIO_CHN1, AS_GPIO);
	gpio_set_func(GPIO_CHN2, AS_GPIO);
	gpio_set_func(GPIO_CHN3, AS_GPIO);

	gpio_set_output_en(GPIO_CHN0, 1);
	gpio_set_output_en(GPIO_CHN1, 1);
	gpio_set_output_en(GPIO_CHN2, 1);
	gpio_set_output_en(GPIO_CHN3, 1);
#endif
}

void main_loop (void)
{

#if( TEST_MODE_SELECT == TEST_MODE_STX)
	ble_stx_test();
#elif ( TEST_MODE_SELECT == TEST_MODE_BTX_TX)
	ble_btx_tx_test();
#elif ( TEST_MODE_SELECT == TEST_MODE_MANUAL_TX)
	ble_manual_tx_test();
#elif (TEST_MODE_SELECT == TEST_MODE_MANUAL_RX)
	ble_manual_rx_test();
#elif (TEST_MODE_SELECT == TEST_MODE_BRX_RX)
	ble_brx_rx_test();
#endif
}




void ble_stx_test(void)
{

	rf_set_power_level_index (RF_POWER);

	rf_set_ble_crc_adv ();

	rf_set_ble_access_code_value(BLE_ACCESS_CODE);


	unsigned long tx_begin_tick;

	while(1){

		tx_begin_tick = clock_time();

		rf_set_tx_rx_off_auto_mode();
		rf_set_ble_channel (TEST_CHN);  //2402

		debug_pkt_adv.data[0] ++;


		rf_start_stx ((void *)&debug_pkt_adv, clock_time() + 100);

		sleep_us(2000);  //2mS is enough for packet sending
		if(rf_irq_src_get() & FLD_RF_IRQ_TX){
			rf_irq_clr_src(FLD_RF_IRQ_TX);
			DBG_CHN0_TOGGLE;
		}

		while(!clock_time_exceed(tx_begin_tick, TX_INTERVAL_US) );   //10mS
	}

}

void ble_manual_tx_test(){

	rf_set_power_level_index (RF_POWER);

	rf_set_ble_crc_adv ();

	rf_set_ble_access_code_value(BLE_ACCESS_CODE);

	rf_set_ble_channel (TEST_CHN);  //2402

	rf_set_txmode();

	rf_irq_clr_src(FLD_RF_IRQ_ALL);

	unsigned long tx_begin_tick;

	while(1){

		tx_begin_tick = clock_time();

		debug_pkt_adv.data[0] ++;


		rf_tx_pkt ((void *)&debug_pkt_adv);

		sleep_us(2000);  //2mS is enough for packet sending

		if(rf_irq_src_get() & FLD_RF_IRQ_TX){
			rf_irq_clr_src(FLD_RF_IRQ_TX);
			DBG_CHN0_TOGGLE;
		}

		while(!clock_time_exceed(tx_begin_tick, TX_INTERVAL_US) );   //10mS
	}

}

void ble_btx_tx_test(){
	rf_set_power_level_index (RF_POWER);

	rf_set_ble_crc_adv ();

	rf_set_ble_access_code_value(BLE_ACCESS_CODE);


	unsigned long tx_begin_tick;

	while(1){

		tx_begin_tick = clock_time();

		rf_set_tx_rx_off_auto_mode(); // stop SM
		rf_set_ble_channel (TEST_CHN);  //2402

		debug_pkt_adv.data[0] ++;


		rf_start_btx ((void *)&debug_pkt_adv, clock_time() + 100);

		sleep_us(2000);  //2mS is enough for packet sending
		rf_set_tx_rx_off_auto_mode(); // stop SM

		if(rf_irq_src_get() & FLD_RF_IRQ_TX){
			rf_irq_clr_src(FLD_RF_IRQ_TX);
			DBG_CHN0_TOGGLE;
		}

		while(!clock_time_exceed(tx_begin_tick, TX_INTERVAL_US) );   //10mS
	}
}



#define				BLE_LL_BUFF_SIZE		80
unsigned char  		blt_rxbuffer[BLE_LL_BUFF_SIZE*4] __attribute__((aligned(4)));
int					blt_rx_wptr = 0;



int AA_rx_irq_cnt = 0;
int AA_rx_crc_ok_cnt = 0;
int AA_rx_match_cnt = 0;

int AA_rx_match_flag;
int AA_rx_miss_cnt;
int AA_rx_miss_rate;
int AA_rx_set_number = 0;





volatile int brx_mode_flag = 0;

unsigned long bltc_tick_1st_rx = 0;
unsigned long bltc_connExpectTime = 0;
int bltParam_conn_rx_num = 0;

volatile 	int   RX_rcvd_tick;
volatile	int	  RX_rcvd_flag = 0;

unsigned long blt_timeStamp;
unsigned long blt_tick_now;


unsigned char	blt_tx_empty_packet[6] = {2, 0, 0, 0, 1, 0};




_attribute_ram_code_sec_noinline_ void irq_rf_handler(void)
{

	if(rf_irq_src_get() & FLD_RF_IRQ_RX)
	 {

		blt_tick_now = clock_time();

		DBG_CHN0_TOGGLE;

		unsigned char * raw_pkt = (unsigned char *) (blt_rxbuffer + blt_rx_wptr * BLE_LL_BUFF_SIZE);
		blt_rx_wptr = (blt_rx_wptr + 1) & 3;
		unsigned short next_pkt_addr;
		next_pkt_addr = (unsigned short)(unsigned long) (blt_rxbuffer + blt_rx_wptr * BLE_LL_BUFF_SIZE); //set next buffer
		rf_rx_buffer_reconfig(next_pkt_addr);
		rf_irq_clr_src(FLD_RF_IRQ_RX);

		AA_rx_irq_cnt ++;

		if	(RF_BLE_PACKET_LENGTH_OK(blt_rxbuffer) && RF_BLE_PACKET_CRC_OK(blt_rxbuffer) )  //CRC OK
		{
			DBG_CHN1_TOGGLE;
			AA_rx_crc_ok_cnt ++;

			#if (TEST_MODE_SELECT == TEST_MODE_BRX_RX)
				blt_timeStamp = reg_rf_timestamp;

				if(!brx_mode_flag){
					bltParam_conn_rx_num = 0;
				}

				if(!bltc_tick_1st_rx && !bltParam_conn_rx_num && (unsigned long)(blt_tick_now - blt_timeStamp) < CLOCK_16M_SYS_TIMER_CLK_1MS){
					bltc_tick_1st_rx = blt_timeStamp;
					if(!brx_mode_flag){
						brx_mode_flag = 1;
						bltc_connExpectTime = blt_timeStamp + TX_INTERVAL_TICK;
					}
				}
			#endif

		}


		raw_pkt[0] = 1;  //must
	 }




	bltParam_conn_rx_num ++;



	if(rf_irq_src_get() & FLD_RF_IRQ_TX)
	{
		rf_irq_clr_src(FLD_RF_IRQ_TX);
	}

}






_attribute_ram_code_sec_noinline_ void ble_manual_rx_test(void)
{

	rf_rx_buffer_set(blt_rxbuffer,BLE_LL_BUFF_SIZE,0);

	rf_set_power_level_index (RF_POWER);
	rf_set_ble_crc_adv ();


	rf_set_ble_access_code_value(BLE_ACCESS_CODE);


	rf_set_ble_channel (TEST_CHN);  //37:2402;   8:2420

	rf_irq_clr_src(FLD_RF_IRQ_RX);

	rf_set_rxmode ();

	irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
	rf_irq_disable(FLD_RF_IRQ_ALL);
	rf_irq_enable(FLD_RF_IRQ_RX);
	irq_enable();


	while(1)
	{

	}


}





void ble_brx_rx_test(void)
{

	reg_dma_rf_rx_addr = (unsigned int)(unsigned long) (blt_rxbuffer);
	reg_dma_rf_rx_mode = FLD_DMA_WR_MEM;   // rf rx buffer enable & size
	reg_dma_rf_rx_size = (BLE_LL_BUFF_SIZE>>4);


	rf_set_power_level_index (RF_POWER);
	rf_set_ble_crc_adv ();


	rf_set_ble_access_code_value(BLE_ACCESS_CODE);

	rf_irq_clr_src(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);
	irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
	rf_irq_disable(FLD_RF_IRQ_ALL);
	rf_irq_enable(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);
	brx_mode_flag = 0;

	irq_enable();

//enter manual RX first, get peer TX device timeStamp
	rf_set_ble_channel(TEST_CHN);
	rf_set_rxmode ();
	while(!brx_mode_flag)
	{

	}
	rf_set_tx_rx_off();  //stop manual RX
//manual RX exit, timeStamp capture OK




	AA_rx_match_cnt = 0;
	AA_rx_set_number = 0;

	while(1)
	{


		// 2mS before peer devoce TX
		while( (unsigned long)(clock_time() - (bltc_connExpectTime - 2 * CLOCK_16M_SYS_TIMER_CLK_1MS)) > BIT(30) );


		bltc_tick_1st_rx = 0;
		bltParam_conn_rx_num = 0;
		AA_rx_match_flag = 0;


		rf_set_ble_channel(TEST_CHN);

		rf_start_brx ((void *)blt_tx_empty_packet, clock_time () + 100);   //BRX


		DBG_CHN2_HIGH;

		sleep_us(4 * 1000);  //4mS RX duration

		DBG_CHN2_LOW;

		AA_rx_set_number ++;
		AA_rx_miss_rate = AA_rx_miss_cnt*1000/AA_rx_set_number;


		//stop CRX
		irq_disable();
		rf_set_tx_rx_off_auto_mode();   //stop FSM
		sleep_us(100);
		rf_irq_clr_src(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);
		irq_enable();


		//timing control
		if(bltc_tick_1st_rx){
			bltc_connExpectTime = bltc_tick_1st_rx + TX_INTERVAL_TICK;
		}
		else{
			bltc_connExpectTime += TX_INTERVAL_TICK;
		}

	}



}

#endif


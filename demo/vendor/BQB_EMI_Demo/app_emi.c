/********************************************************************************************************
 * @file	app_emi.c
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

#if(TEST_DEMO==EMI_DEMO)
//#define ATE_SW_TEST		//for ate test when it use single wire to send cmd;then close this define for normal EMI

/*
 * @brief 	This macro definition is mainly used to fix the problem of RX_LEAKAGE exceeding the standard during the
 * 			certification test.Since this setting will cause the package not to be received in the rx state, it is
 * 			only used for authentication.This setting is suitable for rx_leakage certification test for B85 series
 * 			chips.This problem does not exist in the certification of the own development board, so it can be given
 * 			to customers as a special version.Confirmed by wenfeng,modified by zhiwei.20210615.
 * */
#define FIX_RX_LEAKAGE			0

/*
 * @brief 	this macro definition serve to open the setting to deal with problem of zigbee mode 2480Mhz
 * 			band edge can't pass the spec.only use it at the time of certification.
 * */
#define FIX_ZIGBEE_BANDEAGE_EN	0



/*
 * @brief 	This macro definition is used to open the CE test code.
 * After opening the macro definition, prbs9,0x55,0x0f will stop sending
 * packages when the noise of the surrounding environment is greater than -70dbm.
 * */
#define CE_ANTI_NOISE_TEST						1

#if CHN_DEFUALT_VALUE_SET_FLASH
#define CHN_SET_FLASH_ADDR				0x770e0
#endif

#if CE_ANTI_NOISE_TEST
#define DEBUG				1
#define MAX_NOISE_VALUE		-70
#define READ_RSSI_TIMES		100
#define COPENSATION_VALUE   0

#if DEBUG
typedef struct{
	signed char rssi;
	unsigned long tick;
	unsigned short temp1;
	unsigned char temp2;
}Debug_t;
Debug_t debug[100];
unsigned char debug_index = 0;
#endif
static signed char rssi_noise = -110;
static unsigned short  rssi_cnt = 0;
//static signed char last_rssi = -110;
signed char get_noise_value()
{
	signed char rtn;
	unsigned char ss = (read_reg8(0x448) & 0x30) >> 4;
	if(((read_reg8(0xf20)&BIT(0)) == 0) && ((ss == 0) || (ss == 1)))
	{
		if(rssi_cnt >= READ_RSSI_TIMES)
		{
//			last_rssi = rssi_noise;
			rssi_noise = -110;
			rssi_cnt = 0;
		}
		signed char rssi_temp = rf_rssi_get_154();
		if (rssi_temp > rssi_noise)
			rssi_noise = rssi_temp;
		rssi_cnt++;
#if DEBUG
		if(debug_index >= 100)
			debug_index = 0;
		debug[debug_index].rssi = rssi_noise + COPENSATION_VALUE;
		debug[debug_index].tick = clock_time();
		debug_index++;
#endif
	}
	rtn = rssi_noise;// changed by Pengcheng 20201222, for calculating the energy value of white noise

	return rtn + COPENSATION_VALUE;
}

#endif

#define FLASH_128K         0

#if FLASH_128K
#define EMI_TEST_TX_MODE  			     0x1c000
#define EMI_TEST_CMD  				     0x1c001
#define EMI_TEST_POWER_LEVEL  		     0x1c002
#define EMI_TEST_CHANNEL  			     0x1c003
#define EMI_TEST_MODE  				     0x1c004
#define EMI_TEST_CD_MODE_HOPPING_CHN     0x1c005
#define CAP_CLOSE_EN                     0x1c006
#define CAP_VALUE					     0x1e000
#else   // 512K

#define EMI_TEST_TX_MODE  			     0x7c000
#define EMI_TEST_CMD  				     0x7c001
#define EMI_TEST_POWER_LEVEL  		     0x7c002
#define EMI_TEST_CHANNEL  			     0x7c003
#define EMI_TEST_MODE  				     0x7c004
#define EMI_TEST_CD_MODE_HOPPING_CHN     0x7c005
#define CAP_CLOSE_EN                     0x7c006
#define CAP_VALUE					     0x77000
#endif

#define TX_PACKET_MODE_ADDR 		     0x40005
#define RUN_STATUE_ADDR 			     0x40006
#define TEST_COMMAND_ADDR			     0x40007
#define POWER_ADDR 					     0x40008
#define CHANNEL_ADDR				     0x40009
#define RF_MODE_ADDR				     0x4000a
#define CD_MODE_HOPPING_CHN			     0x4000b

#define   GPIO_SYS 0xffffffff
const GPIO_PinTypeDef gpio_map[48] = {
	GPIO_PD7,//0
	GPIO_PA0,//1
	GPIO_PA1,//2
	GPIO_PA2,//3
	GPIO_PA3,//4
	GPIO_PA4,//5
	GPIO_PA5,//6
	GPIO_PA6,//7
	GPIO_PA7,//8
	GPIO_PB0,//9
	GPIO_PB1,//10
//	GPIO_SYS,//11
//	GPIO_SYS,//12
//	GPIO_SYS,//13
//	GPIO_SYS,//14
//	GPIO_SYS,//15
//	GPIO_SYS,//16
//	GPIO_SYS,//17
//	GPIO_SYS,//18
	GPIO_PB2,//19
	GPIO_PB3,//20
	GPIO_PB4,//21
	GPIO_PB5,//22
	GPIO_PB6,//23
	GPIO_PB7,//24
//	GPIO_SYS,//25
//	GPIO_SYS,//26
	GPIO_PC0,//27
	GPIO_PC1,//28
	GPIO_PC2,//29
	GPIO_PC3,//30
	GPIO_PC4,//31
	GPIO_PC5,//32
	GPIO_PC6,//33
	GPIO_PC7,//34
//	GPIO_SYS,//35
//	GPIO_SYS,//36
//	GPIO_SYS,//37
//	GPIO_SYS,//38
//	GPIO_SYS,//39
//	GPIO_SYS,//40
	GPIO_PD0,//41
	GPIO_PD1,//42
	GPIO_PD2,//43
	GPIO_PD3,//44
	GPIO_PD4,//45
	GPIO_PD5,//46
	GPIO_PD6//47
};
#ifdef ATE_SW_TEST
const unsigned char rf_power_Level_list_ate[63]={
 /*VBAT*/
 /*RF_POWER_P10p46dBm*/63,
 /*RF_POWER_P10p29dBm*/62,
 /*RF_POWER_P10p01dBm*/61,
 /*RF_POWER_P9p81dBm*/60,
 /*RF_POWER_P9p48dBm*/59,
 /*RF_POWER_P9p24dBm*/58,
 /*RF_POWER_P8p97dBm*/57,
 /*RF_POWER_P8p73dBm*/56,
 /*RF_POWER_P8p44dBm*/55,
 /*RF_POWER_P8p13dBm*/54,
 /*RF_POWER_P7p79dBm*/53,
 /*RF_POWER_P7p41dBm*/52,
 /*RF_POWER_P7p02dBm*/51,
 /*RF_POWER_P6p60dBm*/50,
 /*RF_POWER_P6p14dBm*/49,
 /*RF_POWER_P5p65dBm*/48,
 /*RF_POWER_P5p13dBm*/47,
 /*RF_POWER_P4p57dBm*/46,
 /*RF_POWER_P3p94dBm*/45,
 /*RF_POWER_P3p23dBm=20*/44,

 /*RF_POWER_P3p01dBm*/43,
 /*RF_POWER_P2p81dBm*/42,
 /*RF_POWER_P2p61dBm*/41,
 /*RF_POWER_P2p39dBm*/40,
 /*RF_POWER_P1p99dBm*/39,
 /*RF_POWER_P1p73dBm*/38,
 /*RF_POWER_P1p45dBm*/37,
 /*RF_POWER_P1p17dBm*/36,
 /*RF_POWER_P0p90dBm*/35,
 /*RF_POWER_P0p58dBm*/34,
 /*RF_POWER_P0p04dBm*/33,
 /*RF_POWER_N0p14dBm*/32,
 /*RF_POWER_N0p97dBm*/31,
 /*RF_POWER_N1p42dBm*/30,
 /*RF_POWER_N1p89dBm*/29,
 /*RF_POWER_N2p48dBm*/28,
 /*RF_POWER_N3p03dBm*/27,
 /*RF_POWER_N3p61dBm*/26,
 /*RF_POWER_N4p26dBm*/25,
 /*RF_POWER_N5p03dBm*/24,
 /*RF_POWER_N5p81dBm*/23,
 /*RF_POWER_N6p67dBm*/22,
 /*RF_POWER_N7p65dBm*/21,
 /*RF_POWER_N8p65dBm*/20,
 /*RF_POWER_N9p89dBm*/19,
 /*RF_POWER_N11p4dBm*/18,
 /*RF_POWER_N13p29dBm*/17,
 /*RF_POWER_N15p88dBm*/16,
 /*RF_POWER_N19p27dBm*/15,
 /*RF_POWER_N25p18dBm*/14,
 13,
 12,
 11,
 10,
 9,
 8,
 7,
 6,
 5,
 4,
 3,
 2,
 1


};
#endif

#define MAX_RF_CHANNEL  40
const unsigned char rf_chn_hipping[MAX_RF_CHANNEL] = {
	48,  4, 66, 62, 44,
	12, 38, 16 ,26, 20,
	22, 68, 18, 28,	42,
	32, 34, 36, 14,	40,
	30, 54, 46, 2,	50,
	52, 80, 56, 78,	74,
	8,  64, 6,  24,	70,
	72, 60, 76, 58, 10,
};



unsigned char  mode=1;//1
unsigned char  power_level = 0;
unsigned char  chn = 2;//2
unsigned char  cmd_now=1;//1
unsigned char  run=1;
unsigned char  tx_cnt=0;
unsigned char  hop=0;






struct  test_list_s {
	unsigned char  cmd_id;
	void	 (*func)(RF_ModeTypeDef, unsigned char , signed char);
};

void emicarrieronly(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_prbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emirx(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emitxprbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emitx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emitx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_tx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_tx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);

void emi_deepio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deepio_ren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deeptimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_deeptimer_ren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_suspendio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_suspendtimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);


struct  test_list_s  ate_list[] = {
		{0x01,emicarrieronly},
		{0x02,emi_con_prbs9},
		{0x03,emirx},
		{0x04,emitxprbs9},
		{0x05,emitx55},
		{0x06,emitx0f},
#ifdef ATE_SW_TEST
		{0x07,emi_con_tx55},
		{0x08,emi_con_tx0f},
#else
		{0x07,emi_deepio_noren},//deep with io wakeup without retension
		{0x08,emi_deeptimer_noren},//deep with timer without retension
#endif
		{0x09,emi_suspendio_noren},//suspend with io wakeup without retension
		{0x0a,emi_suspendtimer_noren},//suspend with timer without retension
		{0x0b,emi_deepio_ren},//deep with io wakeup with retension
		{0x0c,emi_deeptimer_ren},//deep with timer with retension
};



void emi_init(void)
{
	rf_access_code_comm(0x29417671 );//access code  0xf8118ac9
	if(read_reg8(0x840004)&0x01)
	{
		rf_turn_off_internal_cap();
	}
	write_reg8(0x40005,tx_cnt);//tx_cnt
	write_reg8(0x40006,run);//run
	write_reg8(0x40007,cmd_now);//cmd
	write_reg8(0x40008,power_level);//power
	write_reg8(0x40009,chn);//chn
	write_reg8(0x4000a,mode);//mode
	write_reg8(0x4000b,hop);//hop
	write_reg8(0x40004,0);
	write_reg32(0x4000c,0);
	gpio_shutdown(GPIO_ALL);//for pm
	analog_write(0x33,0xff);
}



void emi_serviceloop(void)
{
	unsigned char i=0;
	while(1)
	{
	   run = read_reg8(0x40006);  // get the run state!
	   if(run!=0)
	   {
		   power_level = read_reg8(0x40008);
		   chn = read_reg8(0x40009);
		   mode=read_reg8(0x4000a);
		   cmd_now = read_reg8(0x40007);  // get the command!
		   tx_cnt = read_reg8(0x40005);
			for (i=0; i<sizeof (ate_list)/sizeof (struct test_list_s); i++)
			{
				if(cmd_now == ate_list[i].cmd_id)
				{
					if(mode==0)//ble 2M mode
					{
						ate_list[i].func(RF_MODE_BLE_2M,power_level,chn);
					}
					else if(mode==1)//ble 1M mode
					{
						ate_list[i].func(RF_MODE_BLE_1M_NO_PN,power_level,chn);
					}
					else if(mode==2)//zigbee mode
					{
						ate_list[i].func(RF_MODE_ZIGBEE_250K,power_level,chn);
					}
					else if(mode==3)//BLE125K mode
					{
						ate_list[i].func(RF_MODE_LR_S8_125K,power_level,chn);
					}
					else if(mode==4)//BLE500K mode
					{
						ate_list[i].func(RF_MODE_LR_S2_500K,power_level,chn);
					}
					break;
				}
			}
			run = 0;
			write_reg8(0x40006, run);
	   }
	}

}



void emicarrieronly(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{

#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif
	rf_emi_single_tone(power,rf_chn);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ));
	rf_emi_stop();
}

void emi_con_prbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int t0 = reg_system_tick,chnidx=1;
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif
	hop = read_reg8(0x4000b);

	rf_set_tx_rx_off_auto_mode();
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,0);


	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	{

#if FIX_ZIGBEE_BANDEAGE_EN
		if(rf_mode == RF_MODE_ZIGBEE_250K )
		{
			if(rf_chn == 80)
			{
				write_reg8(0x1223,read_reg8(0x1223)&0x7f);
			}
			else
			{
				write_reg8(0x1223,0x86);
			}
		}
#endif
		rf_continue_mode_run();

		if(hop)
		{
			while(!clock_time_exceed(t0,100000))//run 1ms
			rf_continue_mode_run();

			while(!clock_time_exceed(t0,200000));//stop 10ms

			t0 = reg_system_tick;
			rf_emi_tx_continue_setup(rf_mode,power,rf_chn_hipping[chnidx-1],0);
			(chnidx>=MAX_RF_CHANNEL)?(chnidx=1):(chnidx++);
		}

	}
	rf_emi_stop();
	dma_set_buff_size(DMA3_RF_TX,0x60);
	dma_reset();
	reset_baseband();

}

void emirx(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	rf_emi_rx(rf_mode,rf_chn);
	write_reg8(0x40004,0);
	write_reg32(0x4000c,0);
//Solve the problem that the customer's development board cannot pass rx_leakage authentication.Confirmed by wenfeng,modified by zhiwei.20210615
#if(FIX_RX_LEAKAGE)
	write_reg8(0x1360,(read_reg8(0x1360)|BIT(4)));//LDO_VCO_PUP auto to manual
	write_reg8(0x1362,(read_reg8(0x1362)&0xef));//LDO_VCO_PUP close.
#endif

	cmd_now = 0;//Those two sentences for dealing with the problem that click RxTest again the value of emi_rx_cnt not be cleared in emi rx test
	write_reg8(0x40007, cmd_now); //add by zhiwei,confirmed by kaixin
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	{
		rf_emi_rx_loop();
		if(rf_emi_get_rxpkt_cnt()!=read_reg32(0x4000c))
		{
		write_reg8(0x40004,rf_emi_get_rssi_avg());
		write_reg32(0x4000c,rf_emi_get_rxpkt_cnt());
		}
	}
	rf_emi_stop();
}

void emitxprbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif

#if CE_ANTI_NOISE_TEST
	rf_emi_rx(rf_mode,rf_chn);
#else
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,0);
#endif
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode  && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
#if CE_ANTI_NOISE_TEST
		if(get_noise_value() < MAX_NOISE_VALUE)
		{
			rf_emi_stop();
			rf_emi_tx_burst_setup(rf_mode,power,rf_chn,0);
			rf_emi_tx_burst_loop(rf_mode,0);
			rf_emi_stop();
			rf_emi_rx(rf_mode,rf_chn);
		}
#else
		rf_emi_tx_burst_loop(rf_mode,0);
#endif
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}


void emitx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif

#if CE_ANTI_NOISE_TEST
	rf_emi_rx(rf_mode,rf_chn);
#else
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,2);
#endif
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
#if CE_ANTI_NOISE_TEST
		if(get_noise_value() < MAX_NOISE_VALUE)
		{
			rf_emi_stop();
			rf_emi_tx_burst_setup(rf_mode,power,rf_chn,2);
			rf_emi_tx_burst_loop(rf_mode,2);
			rf_emi_stop();
			rf_emi_rx(rf_mode,rf_chn);
		}
#else
		rf_emi_tx_burst_loop(rf_mode,2);
#endif
//		rf_emi_tx_burst_loop_ramp(rf_mode,2);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}

void emitx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif

#if CE_ANTI_NOISE_TEST
	rf_emi_rx(rf_mode,rf_chn);
#else
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
#endif
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
#if CE_ANTI_NOISE_TEST
		if(get_noise_value() < MAX_NOISE_VALUE)
		{
			rf_emi_stop();
			rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
			rf_emi_tx_burst_loop(rf_mode,1);
			rf_emi_stop();
			rf_emi_rx(rf_mode,rf_chn);
		}
#else
		rf_emi_tx_burst_loop(rf_mode,1);
#endif
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}

void emi_con_tx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,2);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	rf_continue_mode_run();
	rf_emi_stop();
}



void emi_con_tx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#endif
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,1);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	rf_continue_mode_run();
	rf_emi_stop();
}



void emi_deepio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	if(gpio_map[pin]==GPIO_SYS) return;
	cpu_set_gpio_wakeup(gpio_map[pin], Level_High, 1);  //gpio pad wakeup
	gpio_setup_up_down_resistor(gpio_map[pin], PM_PIN_PULLDOWN_100K);
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_PAD,0);
}

void emi_deeptimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
}

void emi_deeptimer_ren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
#if(MCU_CORE_B87 || MCU_CORE_B89)
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
#elif(MCU_CORE_B85)
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
#endif
}


void emi_deepio_ren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	if(gpio_map[pin]==GPIO_SYS) return;
	cpu_set_gpio_wakeup(gpio_map[pin], Level_High, 1);  //gpio pad wakeup
	gpio_setup_up_down_resistor(gpio_map[pin], PM_PIN_PULLDOWN_100K);
#if(MCU_CORE_B87 || MCU_CORE_B89)
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_PAD,0);
#elif(MCU_CORE_B85)
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_PAD,0);
#endif
}


void emi_suspendio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	if(gpio_map[pin]==GPIO_SYS) return;
	cpu_set_gpio_wakeup(gpio_map[pin], Level_High, 1);  //gpio pad wakeup
	gpio_setup_up_down_resistor(gpio_map[pin], PM_PIN_PULLDOWN_100K);
	cpu_sleep_wakeup(SUSPEND_MODE , PM_WAKEUP_PAD,0);
}

void emi_suspendtimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	cpu_sleep_wakeup(SUSPEND_MODE , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
}

/**
 * @brief		This function serves to read flash EMI Parameter
 * @param[in] 	none
 * @return 		none
 */

void read_flash_para(void)
{
   unsigned char cap = 0;
   unsigned char cap_close_en = 0;
   unsigned char temp = 0;

   flash_read_page(EMI_TEST_POWER_LEVEL,1,&temp);
   if( temp != 0xff )
   {
	   power_level = temp;
	   write_reg8(POWER_ADDR,power_level);
   }
   flash_read_page(EMI_TEST_CHANNEL,1,&temp);
   if( temp != 0xff )
   {
	   chn = temp;
	   write_reg8(CHANNEL_ADDR,chn);
   }
   flash_read_page(EMI_TEST_MODE,1,&temp);
   if( temp != 0xff )
   {
	   mode = temp;
	   write_reg8(RF_MODE_ADDR,mode);
   }
   flash_read_page(EMI_TEST_CMD,1,&temp);
   if( temp != 0xff )
   {
	   cmd_now = temp;
	   write_reg8(TEST_COMMAND_ADDR,temp);
   }
   flash_read_page(EMI_TEST_TX_MODE,1,&temp);
   if( temp != 0xff )
   {
	   tx_cnt = temp;
	   write_reg8(TX_PACKET_MODE_ADDR,temp);
   }
   flash_read_page(CAP_VALUE,1,&cap);
   if(cap!=0xff)
   {
	   rf_update_internal_cap(cap);
   }
   flash_read_page(CAP_CLOSE_EN,1,&cap_close_en);
   if(cap_close_en != 0xff)
   {
	   rf_turn_off_internal_cap();
   }

   flash_read_page(EMI_TEST_CD_MODE_HOPPING_CHN,1,&temp);
   if( temp!= 0xff )
   {
	   hop = temp;
	   write_reg8(CD_MODE_HOPPING_CHN,hop);
   }
}

void user_init(void)
{
	emi_init();
    read_flash_para();   //  Power on read flash EMI parameter


}

void main_loop(void)
{
	emi_serviceloop();
}

#endif



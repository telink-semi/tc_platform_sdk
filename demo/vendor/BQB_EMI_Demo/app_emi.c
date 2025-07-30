/********************************************************************************************************
 * @file    app_emi.c
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

#if(TEST_DEMO==EMI_DEMO)
#include "calibration.h"

/*
 * @brief 	When you want to perform all pin corner wake-up tests, you need to define the macro ALL_PIN_WAKEUP = 1.
 * */
#define ALL_PIN_WAKEUP			0

//#define ATE_SW_TEST		//for ate test when it use single wire to send cmd;then close this define for normal EMI

#define EMI_IMPROVE_RX_SPURIOUS               0

/*
 * @brief 	This macro definition is used to open the CE test code.
 * After opening the macro definition, prbs9,0x55,0x0f will stop sending
 * packages when the noise of the surrounding environment is greater than -70dbm.
 * */
#define CE_ANTI_NOISE_TEST						0

/*
 * @brief 	This macro definition is used to select whether to shutdown internal cap.
 * */
#define CLOSE_INTERNAL_CAP		0

#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89 || MCU_CORE_TC321X || MCU_CORE_TC1211||MCU_CORE_TC122X)
/*
 * @brief 	This macro definition is used to select whether to read offset calibration from OTP.
 * */
#define READ_OFFSET_CLIBRATION_OTP		0
#endif

extern void rf_emi_rx_spurious(void);

#if CHN_DEFAULT_VALUE_SET_FLASH
#define CHN_SET_FLASH_ADDR				0x770e0
#endif

#if CE_ANTI_NOISE_TEST
#define DEBUG				0
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

		signed char rssi_temp;
		for(int i = 0; i < 50; i++)
		{
			sleep_us(5);
			rssi_temp = rf_rssi_get_154();
			if (rssi_temp > rssi_noise)
				rssi_noise = rssi_temp;
		}
		rssi_cnt++;
#if DEBUG
		if(debug_index >= 100)
			debug_index = 0;
		debug[debug_index].rssi = rssi_noise + COPENSATION_VALUE;
		debug[debug_index].tick = clock_time();
		debug_index++;
#endif
	}
	else if(read_reg8(0xf20)&BIT(0))
	{
		rf_rx_finish_clear_flag();
	}

	rtn = rssi_noise;// changed by Pengcheng 20201222, for calculating the energy value of white noise
	return rtn + COPENSATION_VALUE;
}

#endif
#define EMI_TEST_FLASH_64K_BASE		 	 0xc000
#define EMI_TEST_FLASH_128K_BASE		 0x1c000
#define EMI_TEST_FLASH_512K_BASE		 0x7c000
#define EMI_TEST_FLASH_1M_BASE		 	 0xfc000
#define EMI_TEST_FLASH_2M_BASE		 	 0x1fc000

#define EMI_TEST_TX_MODE  			     0x00
#define EMI_TEST_CMD  				     0x01
#define EMI_TEST_POWER_LEVEL  		     0x02
#define EMI_TEST_CHANNEL  			     0x03
#define EMI_TEST_MODE  				     0x04
#define EMI_TEST_CD_MODE_HOPPING_CHN     0x05
#define CAP_CLOSE_EN                     0x06
#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X)
#define CAP_VALUE_FLASH				 	 0x07
#define CAP_VALUE_OTP					 0x3fc8 //0x3fc8-0x3fcb,32bit
#endif

#define RSSI_ADDR                        0x40004
#define TX_PACKET_MODE_ADDR 		     0x40005
#define RUN_STATUE_ADDR 			     0x40006
#define TEST_COMMAND_ADDR			     0x40007
#define POWER_ADDR 					     0x40008
#define CHANNEL_ADDR				     0x40009
#define RF_MODE_ADDR				     0x4000a
#define CD_MODE_HOPPING_CHN			     0x4000b
#define RX_PACKET_NUM_ADDR               0x4000c
#define PA_TX_RX_SETTING				 0x40014 //2bytes

/*PA setting*/
#define get_pin(value) (((unsigned short)((value) >> 3) << 8) | BIT((value) & 0x07))
#define gpio_function_en(pin)			gpio_set_func((pin), AS_GPIO)
#define gpio_output_en(pin)				gpio_set_output_en((pin), 1)
#define gpio_output_dis(pin)			gpio_set_output_en((pin), 0)
#define gpio_input_en(pin)				gpio_set_input_en((pin), 1)
#define gpio_input_dis(pin)				gpio_set_input_en((pin), 0)
#define gpio_set_low_level(pin)			gpio_write((pin), 0)
#define gpio_set_high_level(pin)		gpio_write((pin), 1)

unsigned char pa_hw_flag = 0;
void pa_init(unsigned short v)
{
	unsigned short tx_pin = get_pin(v&0xff);
	unsigned short rx_pin = get_pin((v&0xff00) >> 8);
	if(tx_pin == rx_pin)
	{
		pa_hw_flag = 2;
		return;
	}

	pa_hw_flag = 0;
	gpio_function_en(tx_pin);
	gpio_input_dis(tx_pin);
	gpio_output_en(tx_pin);
	gpio_set_low_level(tx_pin);
	gpio_function_en(rx_pin);
	gpio_input_dis(rx_pin);
	gpio_output_en(rx_pin);
	gpio_set_low_level(rx_pin);
}

void pa_operation(unsigned short v, unsigned char s)
{
	if(pa_hw_flag == 0)
	{
		unsigned short tx_pin = get_pin(v&0xff);
		unsigned short rx_pin = get_pin((v&0xff00) >> 8);
		if(s == 0) //close
		{
			gpio_set_low_level(tx_pin);
			gpio_set_low_level(rx_pin);
		}
		else if(s == 1) //tx
		{
			gpio_set_high_level(tx_pin);
			gpio_set_low_level(rx_pin);
		}
		else //rx
		{
			gpio_set_low_level(tx_pin);
			gpio_set_high_level(rx_pin);
		}
	}
}
/*PA setting*/


#define   GPIO_SYS 0xffffffff
const GPIO_PinTypeDef gpio_map[48] = {
#if	!(MCU_CORE_TC1211||MCU_CORE_TC122X)
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
	GPIO_PD6,//47
#if	(ALL_PIN_WAKEUP && (MCU_CORE_B80 || MCU_CORE_B80B))
	GPIO_PF0,
	GPIO_PF1
#endif
#endif
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
unsigned short pa_setting=0;






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

#if	!(MCU_CORE_TC1211||MCU_CORE_TC122X)
void emi_deepio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deepio_ren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deeptimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_deeptimer_ren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_suspendio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_suspendtimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
#endif

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
#if	!(MCU_CORE_TC1211||MCU_CORE_TC122X)
		{0x07,emi_deepio_noren},//deep with io wakeup without retension
		{0x08,emi_deeptimer_noren},//deep with timer without retension
#endif
#endif
#if	!(MCU_CORE_TC1211||MCU_CORE_TC122X)
		{0x09,emi_suspendio_noren},//suspend with io wakeup without retension
		{0x0a,emi_suspendtimer_noren},//suspend with timer without retension
		{0x0b,emi_deepio_ren},//deep with io wakeup with retension
		{0x0c,emi_deeptimer_ren},//deep with timer with retension
#endif
};



void emi_init(void)
{
	rf_access_code_comm(0x29417671 );//access code  0xf8118ac9
	write_reg8(TX_PACKET_MODE_ADDR,tx_cnt);//tx_cnt
	write_reg8(RUN_STATUE_ADDR,run);//run
	write_reg8(TEST_COMMAND_ADDR,cmd_now);//cmd
	write_reg8(POWER_ADDR,power_level);//power
	write_reg8(CHANNEL_ADDR,chn);//chn
	write_reg8(RF_MODE_ADDR,mode);//mode
	write_reg8(CD_MODE_HOPPING_CHN,hop);//hop
	write_reg8(RSSI_ADDR,0);
	write_reg16(PA_TX_RX_SETTING, pa_setting);
	write_reg32(RX_PACKET_NUM_ADDR, 0);
	gpio_shutdown(GPIO_ALL);//for pm
#if(!MCU_CORE_B89)&&(!MCU_CORE_TC321X)&&(!MCU_CORE_TC122X)
	usb_set_pin(1); //add for chips only support swire function of through-usb
#endif
#if	ALL_PIN_WAKEUP
	usb_dp_pullup_en(0);
#endif
	analog_write(0x33,0xff);
    /*DP through sws*/
#if(MCU_CORE_B85||MCU_CORE_B87)
	usb_set_pin(1);
	gpio_setup_up_down_resistor(GPIO_PA5, PM_PIN_PULLUP_10K);
#endif

}



void emi_serviceloop(void)
{
	unsigned char i=0;
	while(1)
	{
	   run = read_reg8(RUN_STATUE_ADDR);  // get the run state!
	   if(run!=0)
	   {
			power_level = read_reg8(POWER_ADDR);
			chn = read_reg8(CHANNEL_ADDR);
			mode=read_reg8(RF_MODE_ADDR);
			cmd_now = read_reg8(TEST_COMMAND_ADDR);  // get the command!
			tx_cnt = read_reg8(TX_PACKET_MODE_ADDR);
			pa_setting = read_reg16(PA_TX_RX_SETTING);
			pa_init(pa_setting);
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
#if(!(MCU_CORE_TC321X||MCU_CORE_TC1211||MCU_CORE_TC122X))
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
#endif
					else if(mode==5)
					{
						ate_list[i].func(RF_MODE_PRIVATE_2M,power_level,chn);
					}
					else if(mode==6)
					{
						ate_list[i].func(RF_MODE_PRIVATE_1M,power_level,chn);
					}
					break;
				}
			}
			pa_operation(pa_setting, 0);
			run = 0;
			write_reg8(RUN_STATUE_ADDR, run);
	   }
	}

}



void emicarrieronly(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif
	rf_emi_single_tone(power,rf_chn);
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode ) && ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ));
	rf_emi_stop();
}

void emi_con_prbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int t0 = reg_system_tick,chnidx=1;
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif
	hop = read_reg8(CD_MODE_HOPPING_CHN);

	rf_set_tx_rx_off_auto_mode();
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,0);


	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode ) && ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	{

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
	pa_operation(pa_setting, 2);
	rf_emi_rx(rf_mode,rf_chn);
	write_reg8(RSSI_ADDR,0);
	write_reg32(RX_PACKET_NUM_ADDR,0);
#if(EMI_IMPROVE_RX_SPURIOUS)
	rf_emi_rx_spurious();
#endif

	cmd_now = 0;//Those two sentences for dealing with the problem that click RxTest again the value of emi_rx_cnt not be cleared in emi rx test
	write_reg8(TEST_COMMAND_ADDR, cmd_now); //add by zhiwei,confirmed by kaixin
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode ) && ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	{
		rf_emi_rx_loop();
		if(rf_emi_get_rxpkt_cnt()!=read_reg32(RX_PACKET_NUM_ADDR))
		{
		write_reg8(RSSI_ADDR,rf_emi_get_rssi_avg());
		write_reg32(RX_PACKET_NUM_ADDR,rf_emi_get_rxpkt_cnt());
		}
	}
	rf_emi_stop();
}

void emitxprbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif

#if CE_ANTI_NOISE_TEST
	pa_operation(pa_setting, 2);
	rf_emi_rx(rf_mode,rf_chn);
#else
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,0);
#endif
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode)  && ((read_reg8(TX_PACKET_MODE_ADDR)) == tx_cnt )\
			&& ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	{
#if CE_ANTI_NOISE_TEST
		if(get_noise_value() < MAX_NOISE_VALUE)
		{
			rf_emi_stop();
			pa_operation(pa_setting, 1);
			rf_emi_tx_burst_setup(rf_mode,power,rf_chn,0);
			rf_emi_tx_burst_loop(rf_mode,0);
			rf_emi_stop();
			sleep_ms(6);
			pa_operation(pa_setting, 2);
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
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif

#if CE_ANTI_NOISE_TEST
	pa_operation(pa_setting, 2);
	rf_emi_rx(rf_mode,rf_chn);
#else
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,2);
#endif
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode) && ((read_reg8(TX_PACKET_MODE_ADDR)) == tx_cnt )\
			&& ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	{
#if CE_ANTI_NOISE_TEST
		if(get_noise_value() < MAX_NOISE_VALUE)
		{
			rf_emi_stop();
			pa_operation(pa_setting, 1);
			rf_emi_tx_burst_setup(rf_mode,power,rf_chn,2);
			rf_emi_tx_burst_loop(rf_mode,2);
			rf_emi_stop();
			sleep_ms(6);
			pa_operation(pa_setting, 2);
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
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif

#if CE_ANTI_NOISE_TEST
	pa_operation(pa_setting, 2);
	rf_emi_rx(rf_mode,rf_chn);
#else
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
#endif
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode) && ((read_reg8(TX_PACKET_MODE_ADDR)) == tx_cnt )\
			&& ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	{
#if CE_ANTI_NOISE_TEST
		if(get_noise_value() < MAX_NOISE_VALUE)
		{
			rf_emi_stop();
			pa_operation(pa_setting, 1);
			rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
			rf_emi_tx_burst_loop(rf_mode,1);
			rf_emi_stop();
			sleep_ms(6);
			pa_operation(pa_setting, 2);
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
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,2);
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode ) && ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	rf_continue_mode_run();
	rf_emi_stop();
}



void emi_con_tx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(pa_setting, 1);
#ifdef ATE_SW_TEST
	RF_PowerTypeDef power = rf_power_Level_list_ate[pwr];
#else
#if(!MCU_CORE_B89)
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
#else
	RF_PowerTypeDef power = pwr;
#endif
#endif
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,1);
	while( ((read_reg8(RUN_STATUE_ADDR)) == run ) &&  ((read_reg8(TEST_COMMAND_ADDR)) == cmd_now )\
			&& ((read_reg8(POWER_ADDR)) == power_level ) &&  ((read_reg8(CHANNEL_ADDR)) == chn )\
			&& ((read_reg8(RF_MODE_ADDR)) == mode ) && ((read_reg16(PA_TX_RX_SETTING)) == pa_setting ))
	rf_continue_mode_run();
	rf_emi_stop();
}


#if	!(MCU_CORE_TC1211||MCU_CORE_TC122X)

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
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_SYS_TIMER_CLK_1S));
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
#elif(ALL_PIN_WAKEUP && (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X))
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
	cpu_sleep_wakeup(SUSPEND_MODE , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_SYS_TIMER_CLK_1S));
}

/**
 * @brief		This function serves to read flash EMI Parameter
 * @param[in] 	none
 * @return 		none
 */

void read_flash_para(void)
{
	unsigned char cap_close_en = 0;
	unsigned char temp = 0;
	unsigned long calib_flash_base_addr = EMI_TEST_FLASH_128K_BASE;
	unsigned char flash_size = (flash_read_mid() >> 16) & 0xff;

	switch (flash_size)
	{
		case FLASH_SIZE_64K:
			calib_flash_base_addr = EMI_TEST_FLASH_64K_BASE;
			break;
		case FLASH_SIZE_128K:
			calib_flash_base_addr = EMI_TEST_FLASH_128K_BASE;
			break;
		case FLASH_SIZE_512K:
			calib_flash_base_addr = EMI_TEST_FLASH_512K_BASE;
			break;
		case FLASH_SIZE_1M:
			calib_flash_base_addr = EMI_TEST_FLASH_1M_BASE;
			break;
		case FLASH_SIZE_2M:
			calib_flash_base_addr = EMI_TEST_FLASH_2M_BASE;
			break;
		default:
			break;
	}
	flash_read_page(calib_flash_base_addr+EMI_TEST_POWER_LEVEL,1,&temp);
	if( temp != 0xff )
	{
	   power_level = temp;
	   write_reg8(POWER_ADDR,power_level);
	}
	flash_read_page(calib_flash_base_addr+EMI_TEST_CHANNEL,1,&temp);
	if( temp != 0xff )
	{
	   chn = temp;
	   write_reg8(CHANNEL_ADDR,chn);
	}
	flash_read_page(calib_flash_base_addr+EMI_TEST_MODE,1,&temp);
	if( temp != 0xff )
	{
	   mode = temp;
	   write_reg8(RF_MODE_ADDR,mode);
	}
	flash_read_page(calib_flash_base_addr+EMI_TEST_CMD,1,&temp);
	if( temp != 0xff )
	{
	   cmd_now = temp;
	   write_reg8(TEST_COMMAND_ADDR,temp);
	}
	flash_read_page(calib_flash_base_addr+EMI_TEST_TX_MODE,1,&temp);
	if( temp != 0xff )
	{
	   tx_cnt = temp;
	   write_reg8(TX_PACKET_MODE_ADDR,temp);
	}
	flash_read_page(calib_flash_base_addr+CAP_CLOSE_EN,1,&cap_close_en);
	if(cap_close_en != 0xff)
	{
	   rf_turn_off_internal_cap();
	}

	flash_read_page(calib_flash_base_addr+EMI_TEST_CD_MODE_HOPPING_CHN,1,&temp);
	if( temp!= 0xff )
	{
	   hop = temp;
	   write_reg8(CD_MODE_HOPPING_CHN,hop);
	}
#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X)
	flash_read_page(calib_flash_base_addr+CAP_VALUE_FLASH,1,&temp);
	if( temp!= 0xff )
	{
		rf_update_internal_cap(temp);
	}
#endif
}

#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89)
extern unsigned char otp_program_flag;
/**
 * @brief		This function serves to read calibration from otp
 * @param[in] 	none
 * @return 		none
 */
void read_calibration_otp(void)
{
	unsigned int temp;

	if(otp_program_flag != 1)
	{
		otp_set_active_mode();
	}
	otp_read(OTP_CAP_VALUE_ADDR, 1, &temp);
	if((temp&0xff) != 0xff)
	{
		rf_update_internal_cap(temp&0xff);
	}
	if(otp_program_flag != 1)
	{
		otp_set_deep_standby_mode();
	}
}
#endif

/**
 * @brief		This function serves to read calibration from flash
 * @param[in] 	none
 * @return 		none
 */
void update_calibration_flash(unsigned int addr)
{
	unsigned char temp;
	flash_read_page(addr, 1, &temp);
	if(temp != 0xff)
	{
		rf_update_internal_cap(temp&0xff);
	}
}

/**
 * @brief		This function serves to read calibration from flash
 * @param[in] 	none
 * @return 		none
 */
void read_calibration_flash(void)
{
#if !(MCU_CORE_TC1211||MCU_CORE_TC122X)
	unsigned char flash_size = (flash_read_mid() >> 16) & 0xff;
	switch (flash_size)
	{
		case FLASH_SIZE_64K:
			update_calibration_flash(FLASH_CAP_VALUE_ADDR_64K);
			break;
		case FLASH_SIZE_128K:
		case FLASH_SIZE_256K:
			update_calibration_flash(FLASH_CAP_VALUE_ADDR_128K);
			break;
		case FLASH_SIZE_512K:
			update_calibration_flash(FLASH_CAP_VALUE_ADDR_512K);
			break;
		case FLASH_SIZE_1M:
			update_calibration_flash(FLASH_CAP_VALUE_ADDR_1M);
			break;
		case FLASH_SIZE_2M:
			update_calibration_flash(FLASH_CAP_VALUE_ADDR_2M);
			break;
		default:
			break;
	}
#endif
}

#endif /*!(MCU_CORE_TC1211||MCU_CORE_TC122X)*/

void user_init(void)
{
	emi_init();

#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89 || MCU_CORE_TC321X)
#if READ_OFFSET_CLIBRATION_OTP
	read_calibration_otp();
#else
	read_flash_para();   //  Power on read flash EMI parameter
	read_calibration_flash();
#endif
#else
#if !(MCU_CORE_TC1211||MCU_CORE_TC122X)
    read_flash_para();   //  Power on read flash EMI parameter
	read_calibration_flash();
#endif
#endif

#if CLOSE_INTERNAL_CAP
	rf_turn_off_internal_cap();
#endif
}

void main_loop(void)
{
	emi_serviceloop();
}

#endif



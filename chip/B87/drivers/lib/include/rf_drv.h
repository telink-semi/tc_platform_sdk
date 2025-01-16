/********************************************************************************************************
 * @file    rf_drv.h
 *
 * @brief   This is the header file for B87
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef _RF_DRV_H_
#define _RF_DRV_H_

#include "bsp.h"
#include "compiler.h"
#include "gpio.h"
#define RF_CHN_TABLE 		0x8000


/**
 *  Some notice for timing sequence.
 *  1:The timing sequence can be configured once during initialization.
 *  2:The timing sequence of tx and rx can be configured separately.
 *  3:Call the enable function rf_tx_fast_settle_en or rf_rx_fast_settle_en when using the configured timing sequence.
 *    To close it, call rf_tx_fast_settle_dis or rf_rx_fast_settle_dis.
 *  4:According to the different parameters, a normal calibration should be done regularly, such as parameter notes.
 */
typedef enum{
RX_SETTLE_TIME_77US		 = 0, /**<  disable rx_ldo_trim calibration,reduce 8.5us of rx settle time. Do a normal calibration at the beginning.*/
RX_SETTLE_TIME_44US		 = 1, /**<  disable rx_ldo_trim and rx_dcoc calibration,reduce 48.5us of rx settle time.Receive for a period of time and then do a normal calibration. */
}rf_rx_fast_seetle_time_e;

typedef enum{
TX_SETTLE_TIME_101US	 	= 0, /**<  disable tx_ldo_trim function,reduce 8.5us of tx settle time. Do a normal calibration at the beginning.*/
TX_SETTLE_TIME_53US		= 1, /**<  disable tx_ldo_trim function and tx_hpmc,reduce 61.5us of tx settle time.After frequency hopping, a normal calibration must be done.*/
}rf_tx_fast_seetle_time_e;


/**
 * @brief	Define function to set tx channel or rx channel.
 */
typedef enum
{
	TX_CHANNEL		= 0,
	RX_CHANNEL		= 1,
}rf_trx_chn_e;

/**
 * @brief	Take 4 antennas as an example to illustrate the antenna switching sequence.
 * 			SWITCH_SEQ_MODE0	- antenna index switch sequence 01230123
 * 			SWITCH_SEQ_MODE1	- antenna index switch sequence 0123210
 * 			SWITCH_SEQ_MODE2	- antenna index switch sequence 001000200030
 */
typedef enum{
	SWITCH_SEQ_MODE0		 = 0,
	SWITCH_SEQ_MODE1		 = BIT(6),
	SWITCH_SEQ_MODE2		 = BIT(7)
}rf_ant_pattern_e;

/**
 * @brief	It can be defined as atsel0 pin.
 * @note	B87 found in AOA/AOD verification that PC5, 6, 7 will cause interference to the RF module
 * 			when used as ATSEL, so this group of pins should be commented out in the driver.
 */
typedef enum
{
//	RF_ANT_SEL0_PC5 = GPIO_PC5,
	RF_ANT_SEL0_PD6 = GPIO_PD6,
	RF_ANT_SEL0_NONE = 0,
}rf_antsel0_pin_e;

/**
 * @brief	It can be defined as atsel1 pin.
 * @note	B87 found in AOA/AOD verification that PC5, 6, 7 will cause interference to the RF module
 * 			when used as ATSEL, so this group of pins should be commented out in the driver.
 */
typedef enum
{
//	RF_ANT_SEL1_PC6 = GPIO_PC6,
	RF_ANT_SEL1_PB0 = GPIO_PB0,
	RF_ANT_SEL1_NONE = 0,
}rf_antsel1_pin_e;

/**
 * @brief	It can be defined as atsel2 pin.
 * @note	B87 found in AOA/AOD verification that PC5, 6, 7 will cause interference to the RF module
 * 			when used as ATSEL, so this group of pins should be commented out in the driver.
 */
typedef enum
{
//	RF_ANT_SEL2_PC7 = GPIO_PC7,
	RF_ANT_SEL2_PB1 = GPIO_PB1,
	RF_ANT_SEL2_NONE = 0,
}rf_antsel2_pin_e;

/**
 * @brief	Initialize the structure used to control the antenna IO.
 */
typedef struct{
	rf_antsel0_pin_e	antsel0_pin;
	rf_antsel1_pin_e	antsel1_pin;
	rf_antsel2_pin_e 	antsel2_pin;
}rf_ant_pin_sel_t;

/*
 * @brief  Data length type of AOA/AOD sampling.
 * |                 |                         |
 * | :-------------- | :---------------------- |
 * |   	 <15:8>      |          <7:0>          |
 * |   IQ byte len   |   iq data bit num mode  |
 */
typedef enum{
	IQ_8_BIT_MODE   		= 0x0200,
	IQ_16_BIT_MODE  		= 0x0401,
	IQ_16_BIT_LOW_MODE		= 0x0402,
	IQ_16_BIT_HIGH_MODE		= 0x0403,
	IQ_20_BIT_MODE			= 0x0504/**< Due to hardware reasons,this mode cannot be used with the SAMPLE_0P25US_INTERVAL
										 interval mode,which will cause the sampling data to overflow.*/
}rf_aoa_aod_iq_data_mode_e;


/*
 * @brief   The chip can receive AOA/AOD packets in ADV or ACL format.
 * @note	Only one of the modes can be enabled.In the AOA/AOD mode, the packet format is different from the normal mode,
 * 			so use rf_is_rx_right and rf_aoa_aod_is_rx_pkt_len_ok for crc and length verification.
 */
typedef enum{
	RF_RX_ACL_AOA_AOD_EN  = BIT(0),
	RF_RX_ADV_AOA_AOD_EN  = BIT(1),
	RF_AOA_AOD_RX_OFF     = 0
}rf_aoa_aod_rx_mode_e;

/*
 * @brief   The chip can send AOA/AOD packets in ADV or ACL format.
 * @note	Only one of the modes can be enabled.
 */
typedef enum{
	RF_TX_ACL_AOA_AOD_EN  = BIT(2),
	RF_TX_ADV_AOA_AOD_EN  = BIT(3),
	RF_AOA_AOD_TX_OFF     = 0
}rf_aoa_aod_tx_mode_e;


/*
 * @brief  AOA/AOD sample interval time type enumeration.bit<15:8>:4 times the interval value (to facilitate
 * subsequent data processing),bit<7:0>:Set the register value of the corresponding interval mode.
 * |                 |                         |
 * | :-------------- | :---------------------- |
 * |   	 <15:8>      |          <7:0>          |
 * |(interval time)*4|   sample interval mode  |
 */
typedef enum{
	SAMPLE_AOA_4US_AOD_CTEINFO_INTERVAL  = 0x1000,/**< In this case sample interval of aoa is 4us, and aod will
														judge sample interval is 4us or 2us according to CTE info.*/
	SAMPLE_2US_INTERVAL   		= 0x0803,
	SAMPLE_1US_INTERVAL  		= 0x0404,
	SAMPLE_0P5US_INTERVAL 		= 0x0205,
	SAMPLE_0P25US_INTERVAL 		= 0x0106         /**< Due to hardware reasons, this mode cannot be used together with
													  IQ_20_BIT_MODE, otherwise the sampled data will overflow.*/
}rf_aoa_aod_sample_interval_time_e;


/**
 *  @brief  Define RF mode
 */
typedef enum {
	RF_MODE_BLE_2M =    		BIT(0),
	RF_MODE_BLE_1M = 			BIT(1),
    RF_MODE_BLE_1M_NO_PN   =    BIT(2),
	RF_MODE_ZIGBEE_250K    =    BIT(3),
    RF_MODE_LR_S2_500K     =    BIT(4),
    RF_MODE_LR_S8_125K     =    BIT(5),
    RF_MODE_PRIVATE_250K   =    BIT(6),
    RF_MODE_PRIVATE_500K   =    BIT(7),
    RF_MODE_PRIVATE_1M     =    BIT(8),
    RF_MODE_PRIVATE_2M     =    BIT(9),
    RF_MODE_ANT     	   =    BIT(10),
    RF_MODE_BLE_2M_NO_PN   =    BIT(11),
    RF_MODE_HYBEE_1M   	   =    BIT(12),
    RF_MODE_HYBEE_2M   	   =    BIT(13),
    RF_MODE_HYBEE_500K     =    BIT(14),
} RF_ModeTypeDef;


/**
 *  @brief  Define RF Tx/Rx/Auto mode
 */

typedef enum {
    RF_MODE_TX = 0,
    RF_MODE_RX = 1,
    RF_MODE_AUTO=2
} RF_StatusTypeDef;




/**
 *  @brief  Define RX pin of RF for RFFE
 */
typedef enum {
	RFFE_RX_PB2 = GPIO_PB2,
    RFFE_RX_PC6 = GPIO_PC6,
    RFFE_RX_PD0 = GPIO_PD0
} RF_LNARxPinDef;


/**
 *  @brief  Define TX pin of RF for RFFE
 */
typedef enum {
	RFFE_TX_PB3 = GPIO_PB3,
    RFFE_TX_PC7 = GPIO_PC7,
    RFFE_TX_PD1 = GPIO_PD1
} RF_PATxPinDef;

/**
 *  @brief  Define the pin of PTA-BLE_Priority .
 */
typedef enum {
	PTA_BLE_PRIORITY_PB3 = GPIO_PB3
} PTA_BlePriorityPinDef;

/**
 *  @brief  Define the pin of PTA-BLE_Active .
 */
typedef enum {
	PTA_BLE_ACTIVE_PB3 = GPIO_PB3
} PTA_BleActivePinDef;

/**
 *  @brief  Define the pin of PTA-BLE_Status .
 */
typedef enum {
	PTA_BLE_STATUS_PB4 = GPIO_PB4
} PTA_BleStatusPinDef;

/**
 *  @brief  Define the pin of PTA-WIFI_Deny .
 */
typedef enum {
	PTA_WLAN_DENY_PB5 = GPIO_PB5
} PTA_WlanDenyPinDef;

/**
 *  @brief  Define the mode of 3-wire PTA.
 */
typedef enum {
    PTA_BLE_STATUS_TX = 0,
	PTA_BLE_STATUS_RX = 1,
} PTA_3WireModeDef;

/**
 *  @brief  Define the mode of 2-wire PTA.
 */
typedef enum {
    PTA_BLE_PRIORITY_TX = 0,
	PTA_BLE_PRIORITY_RX = 1,
	PTA_BLE_PRIORITY_TRX = 2,
} PTA_2WireModeDef;

/**
 *  @brief  Define power list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_P11p26dBm = 63, //  11.3 dbm
	 RF_POWER_P11p09dBm = 61, //  11.1 dbm
	 RF_POWER_P10p83dBm = 58, //  10.8 dbm
	 RF_POWER_P10p62dBm  = 56, //  10.6 dbm
	 RF_POWER_P10p30dBm  = 53, //  10.3 dbm
	 RF_POWER_P10p05dBm  = 51, // 10.1 dbm
	 RF_POWER_P9p79dBm  = 49, //   9.8 dbm
	 RF_POWER_P9p54dBm  = 47, //   9.5 dbm
	 RF_POWER_P9p23dBm  = 45, //   9.2 dbm
	 RF_POWER_P8p92dBm  = 43, //   8.9 dbm
	 RF_POWER_P8p57dBm  = 41, //   8.6 dbm
	 RF_POWER_P8p20dBm  = 39, //   8.2 dbm
	 RF_POWER_P7p80dBm  = 37, //   7.8 dbm
	 RF_POWER_P7p37dBm  = 35, //   7.4 dbm
	 RF_POWER_P6p91dBm  = 33, //   6.9 dbm
	 RF_POWER_P6p45dBm  = 31, //   6.5 dbm
	 RF_POWER_P5p92dBm  = 29, //   5.9 dbm
	 RF_POWER_P5p33dBm  = 27, //   5.3 dbm
	 RF_POWER_P4p69dBm  = 25, //   4.7 dbm
	 RF_POWER_P3p99dBm  = 23, //   4.0 dbm
	 /*VANT*/
	 RF_POWER_P3p50dBm  = BIT(7) | 63,   //   3.5 dbm
	 RF_POWER_P3p33dBm  = BIT(7) | 61,   //   3.3 dbm
	 RF_POWER_P3p13dBm  = BIT(7) | 59,   //   3.1 dbm
	 RF_POWER_P2p93dBm  = BIT(7) | 57,   //   2.9 dbm
	 RF_POWER_P2p60dBm  = BIT(7) | 54,   //   2.6 dbm
	 RF_POWER_P2p36dBm  = BIT(7) | 52,   //   2.4 dbm
	 RF_POWER_P2p10dBm  = BIT(7) | 50,   //   2.1 dbm
	 RF_POWER_P1p83dBm  = BIT(7) | 48,   //   1.8 dbm
	 RF_POWER_P1p56dBm  = BIT(7) | 46,   //   1.6 dbm
	 RF_POWER_P1p25dBm  = BIT(7) | 44,   //   1.3 dbm
	 RF_POWER_P0p71dBm  = BIT(7) | 41,   //   0.7 dbm
	 RF_POWER_P0p52dBm  = BIT(7) | 40,   //   0.5 dbm
	 RF_POWER_N0p28dBm  = BIT(7) | 36,   //  -0.3 dbm
	 RF_POWER_N0p51dBm  = BIT(7) | 35,   //  -0.5 dbm
	 RF_POWER_N0p74dBm  = BIT(7) | 34,   //  -0.7 dbm
	 RF_POWER_N1p21dBm  = BIT(7) | 32,   //  -1.2 dbm
	 RF_POWER_N1p69dBm  = BIT(7) | 30,   //  -1.7 dbm
	 RF_POWER_N2p23dBm  = BIT(7) | 28,   //  -2.2 dbm
	 RF_POWER_N2p84dBm  = BIT(7) | 26,   //  -2.8 dbm
	 RF_POWER_N3p48dBm  = BIT(7) | 24,   //  -3.5 dbm
	 RF_POWER_N4p18dBm  = BIT(7) | 22,   //  -4.3 dbm
	 RF_POWER_N4p97dBm  = BIT(7) | 20,   //  -5.0 dbm
	 RF_POWER_N5p85dBm  = BIT(7) | 18,   //  -5.9 dbm
	 RF_POWER_N6p83dBm  = BIT(7) | 16,   //  -6.8 dbm
	 RF_POWER_N7p88dBm  = BIT(7) | 14,   //  -7.9 dbm
	 RF_POWER_N9p14dBm  = BIT(7) | 12,   //  -9.1 dbm
	 RF_POWER_N10p70dBm  = BIT(7) | 10,   //-10.7 dbm
	 RF_POWER_N12p57dBm = BIT(7) | 8,   //  -12.6 dbm
	 RF_POWER_N15p01dBm = BIT(7) | 6,   //  -15.0 dbm
	 RF_POWER_N18p40dBm = BIT(7) | 4,   //  -18.4 dbm
	 RF_POWER_N24p28dBm = BIT(7) | 2,   //  -24.3 dbm

	 RF_POWER_N30dBm    = 0xff,         //  -30 dbm
	 RF_POWER_N50dBm    = BIT(7) | 0,   //  -50 dbm

 /*-----------------------------For Internal Test only-----------------------------*/
   /*
	* Customer attention:
	*
	* 	The following settings are for internal testing only, and customers
	* 	are prohibited from using those settings.
	*
	* 	The following energy values are measured under 3.3V power supply
	* 	voltage.The energy will decrease as the power supply voltage drops.
	* 	Customers are prohibited from using the following energy settings
	* 	in product development.
	*/
	  RF_VBAT_POWER_P2p45dBm  = 0x18, 		//   2.45 dbm
	  RF_VBAT_POWER_P0p95dBm  = 0x14, 		//   0.95 dbm
	  RF_VBAT_POWER_P0p0dBm   = 0x12, 		//   0.00 dbm
	  RF_VBAT_POWER_N1p35dBm  = 0x0f, 		//   -1.35 dbm
	  RF_VBAT_POWER_N2p75dBm  = 0x0d, 		//   -2.75 dbm
	  RF_VBAT_POWER_P4p75dBm  = 0x0a, 		//   -4.75 dbm

} RF_PowerTypeDef;

extern signed char txPower_level;

extern const RF_PowerTypeDef rf_power_Level_list[60];
/**
 *  @brief  Define power index list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_INDEX_P11p26dBm,
	 RF_POWER_INDEX_P11p09dBm,
	 RF_POWER_INDEX_P10p83dBm,
	 RF_POWER_INDEX_P10p62dBm,
	 RF_POWER_INDEX_P10p30dBm,
	 RF_POWER_INDEX_P10p05dBm,
	 RF_POWER_INDEX_P9p79dBm,
	 RF_POWER_INDEX_P9p54dBm,
	 RF_POWER_INDEX_P9p23dBm,
	 RF_POWER_INDEX_P8p92dBm,
	 RF_POWER_INDEX_P8p57dBm,
	 RF_POWER_INDEX_P8p20dBm,
	 RF_POWER_INDEX_P7p80dBm,
	 RF_POWER_INDEX_P7p37dBm,
	 RF_POWER_INDEX_P6p91dBm,
	 RF_POWER_INDEX_P6p45dBm,
	 RF_POWER_INDEX_P5p92dBm,
	 RF_POWER_INDEX_P5p33dBm,
	 RF_POWER_INDEX_P4p69dBm,
	 RF_POWER_INDEX_P3p99dBm,
	 /*VANT*/
	 RF_POWER_INDEX_P3p50dBm,
	 RF_POWER_INDEX_P3p33dBm,
	 RF_POWER_INDEX_P3p13dBm,
	 RF_POWER_INDEX_P2p93dBm,
	 RF_POWER_INDEX_P2p60dBm,
	 RF_POWER_INDEX_P2p36dBm,
	 RF_POWER_INDEX_P2p10dBm,
	 RF_POWER_INDEX_P1p83dBm,
	 RF_POWER_INDEX_P1p56dBm,
	 RF_POWER_INDEX_P1p25dBm,
	 RF_POWER_INDEX_P0p71dBm,
	 RF_POWER_INDEX_P0p52dBm,
	 RF_POWER_INDEX_N0p28dBm,
	 RF_POWER_INDEX_N0p51dBm,
	 RF_POWER_INDEX_N0p74dBm,
	 RF_POWER_INDEX_N1p21dBm,
	 RF_POWER_INDEX_N1p69dBm,
	 RF_POWER_INDEX_N2p23dBm,
	 RF_POWER_INDEX_N2p84dBm,
	 RF_POWER_INDEX_N3p48dBm,
	 RF_POWER_INDEX_N4p18dBm,
	 RF_POWER_INDEX_N4p97dBm,
	 RF_POWER_INDEX_N5p85dBm,
	 RF_POWER_INDEX_N6p83dBm,
	 RF_POWER_INDEX_N7p88dBm,
	 RF_POWER_INDEX_N9p14dBm,
	 RF_POWER_INDEX_N10p70dBm,
	 RF_POWER_INDEX_N12p57dBm,
	 RF_POWER_INDEX_N15p01dBm,
	 RF_POWER_INDEX_N18p40dBm,
	 RF_POWER_INDEX_N24p28dBm,

 /*-----------------------------For Internal Test only-----------------------------*/
  /*
   * Customer attention:
   *
   * 	The following settings are for internal testing only, and customers
   * 	are prohibited from using those settings.
   *
   * 	The following energy values are measured under 3.3V power supply
   * 	voltage.The energy will decrease as the power supply voltage drops.
   * 	Customers are prohibited from using the following energy settings
   * 	in product development.
   */
	  RF_VBAT_POWER_INDEX_P3p23dBm,
	  RF_VBAT_POWER_INDEX_P2p0dBm,
	  RF_VBAT_POWER_INDEX_P1p0dBm,
	  RF_VBAT_POWER_INDEX_P0p2dBm,
	  RF_VBAT_POWER_INDEX_N2p75dBm,
	  RF_VBAT_POWER_INDEX_N4p75dBm,
} RF_PowerIndexTypeDef;

typedef enum {
	RF_MI_P0p00 = 0,            /**< MI = 0 */
	RF_MI_P0p076= 76,			/**< MI = 0.076 This gear is only available in private mode*/
	RF_MI_P0p32 = 320,		 	/**< MI = 0.32 */
	RF_MI_P0p50 = 500,		  	/**< MI = 0.5 */
	RF_MI_P0p60 = 600,		  	/**< MI = 0.6 */
	RF_MI_P0p70 = 700,		  	/**< MI = 0.7 */
	RF_MI_P0p80 = 800,		  	/**< MI = 0.8 */
	RF_MI_P0p90 = 900,		  	/**< MI = 0.9 */
	RF_MI_P1p20 = 1200,		    /**< MI = 1.2 */
	RF_MI_P1p30 = 1300,		    /**< MI = 1.3 */
	RF_MI_P1p40 = 1400,		    /**< MI = 1.4 */
}RF_MIVauleTypeDef;

#ifdef		RF_MODE_250K
#define		RF_FAST_MODE_2M		0
#define		RF_FAST_MODE_1M		0
#endif

#ifndef		RF_FAST_MODE_1M
#define		RF_FAST_MODE_1M		1
#endif

#ifndef		RF_FAST_MODE_2M
#define		RF_FAST_MODE_2M		0
#endif

#ifndef		RF_LONG_PACKET_EN
#define		RF_LONG_PACKET_EN	0
#endif

#if			RF_FAST_MODE_2M
	#if			RF_LONG_PACKET_EN
		#define		RF_PACKET_LENGTH_OK(p)		(p[0] == p[12]+13)
		#define		RF_PACKET_CRC_OK(p)			((p[p[0]+3] & 0x51) == 0x40)
	#else
		#define		RF_PACKET_LENGTH_OK(p)		(p[0] == (p[12]&0x3f)+15)
		#define		RF_PACKET_CRC_OK(p)			((p[p[0]+3] & 0x51) == 0x40)
	#endif
#elif		RF_FAST_MODE_1M
/*According to the BLE packet structure, the maximum payload length is 255Bytes. 
  Combined with the DMA len calculation formula, two bytes can cover all situations. 
  changed by kunhe, confirmed by zhiwei; 20230904.*/
#define RF_BLE_PACKET_LENGTH_OK(p) (((p[1]<<8)|p[0]) == p[5]+13)  
#define RF_BLE_PACKET_CRC_OK(p) ((p[p[5]+13 + 3] & 0x01) == 0x0) /*Change the DMA length index to payload length index.
																   changed by kunhe, confirmed by zhiwei; 20230904.*/

	#if (1) // support RF RX/TX MAX data Length: 251byte
		#define		RF_PACKET_LENGTH_OK(p)				(MAKE_U16(p[1], p[0]) == p[5]+13)
		#define		RF_PACKET_CRC_OK(p)					((p[MAKE_U16(p[1], p[0])+3] & 0x01) == 0x0)
	#else
		#define		RF_PACKET_LENGTH_OK(p)				(p[0] == p[5]+13)
		#define		RF_PACKET_CRC_OK(p)					((p[p[0]+3] & 0x01) == 0x0)
	#endif
#else
#define		RF_PACKET_LENGTH_OK(p)		(p[0] == p[12]+13)
#define		RF_PACKET_CRC_OK(p)			((p[p[0]+3] & 0x51) == 0x10)
#endif

#define		RF_PACKET_1M_LENGTH_OK(p)		(p[0] == p[5]+13)
#define		RF_PACKET_2M_LENGTH_OK(p)		(p[0] == (p[12]&0x3f)+15)


#if (RF_FAST_MODE_2M)
	#define			RF_FAST_MODE			1
	#define			RF_TRX_MODE				0x80
	#define			RF_TRX_OFF				0x44		//f02
#elif (RF_FAST_MODE_1M)
	#define			RF_FAST_MODE			1
	#define			RF_TRX_MODE				0x80
	#define			RF_TRX_OFF				0x45		//f02
#else
	#define			RF_FAST_MODE			0
	#define			RF_TRX_MODE				0xe0
	#define			RF_TRX_OFF				0x45		//f02
#endif



#define    RF_ZIGBEE_PACKET_LENGTH_OK(p)    			(p[0]  == p[4]+9)
#define    RF_ZIGBEE_PACKET_CRC_OK(p)       			((p[p[0]+3] & 0x51) == 0x10)
#define    RF_ZIGBEE_PACKET_RSSI_GET(p)     			(p[p[0]+2])
#define    RF_ZIGBEE_PACKET_TIMESTAMP_GET(p)           	(p[p[0]-4] | (p[p[0]-3]<<8) | (p[p[0]-2]<<16) | (p[p[0]-1]<<24))
#define    RF_ZIGBEE_PACKET_PAYLOAD_LENGTH_GET(p)      	(p[4])
#define    RF_TPLL_PACKET_LENGTH_OK(p)              	(p[0] == (p[4] & 0x3f) + 11)
#define    RF_TPLL_PACKET_CRC_OK(p)                 	((p[p[0]+3] & 0x01) == 0x00)
#define    RF_TPLL_PACKET_RSSI_GET(p)               	(p[p[0]+2])
#define    RF_SB_PACKET_PAYLOAD_LENGTH_GET(p)      	(p[0] - 10)
#define    RF_SB_PACKET_CRC_OK(p)                  	((p[p[0]+3] & 0x01) == 0x00)
#define    RF_SB_PACKET_CRC_GET(p)                 	((p[p[0]-8]<<8) + p[p[0]-7]) //Note: here assume that the MSByte of CRC is received first
#define    RF_SB_PACKET_RSSI_GET(p)                	(p[p[0]+2])
#define    RF_TPLL_PACKET_TIMESTAMP_GET(p)          (p[p[0]-4] | (p[p[0]-3]<<8) | (p[p[0]-2]<<16) | (p[p[0]-1]<<24))
#define    RF_SB_PACKET_TIMESTAMP_GET(p)           (p[p[0]-4] | (p[p[0]-3]<<8) | (p[p[0]-2]<<16) | (p[p[0]-1]<<24))

#define    rf_aoa_aod_iq_data_offset(p)					(p[5]+10)


/*******************************            General APIs            **************************/

/**
*	@brief     This function serves to reset RF BaseBand
*	@param[in] none.
*	@return	   none.
*/
static inline void reset_baseband(void)
{
	REG_ADDR8(0x61) |= BIT(0);		//reset_baseband
	REG_ADDR8(0x61) &= ~BIT(0);		//release reset signal
}


/**
 * @brief      This function serves to optimize RF performance
 * 			   This function must be called every time rx is turned on,
 * 			   and is called by an internal function.
 * 			   If there are other requirements that need to be called,
 * 			   turn off rx first, then call it again to make sure the Settings take effect
 * @param[in]  none
 * @return     none
 * @note	   1. All this function after turning on rx 30us, and the calibration value set by the function
 * 			      will take effect after calling rf_ldot_ldo_rxtxlf_bypass_en;if automatic calibration is
 * 			      required, you can use rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function; how to
 * 			      use it can refer to bqb.c file or rf_emi_rx in emi.c
 *			   2. After using rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function and enter tx/rx
 *			      automatic calibration, to use this function again, you need to call the rf_set_rxpara function
 *			      again after entering rx 30us.
 *
 */

static inline void rf_set_rxpara(void)
{
	unsigned char reg_calibration=0;
	reg_calibration = ((read_reg8(0x12ed)&0xf)<<2)|((read_reg8(0x12ec)&0xc0)>>6);
	if(reg_calibration>7)	reg_calibration -= 7;
	write_reg8(0x12e5,(read_reg8(0x12e5)&0xc0)|reg_calibration);
}


/**
*	@brief     This function serves to initiate the mode of RF
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
*/

extern void rf_drv_init (RF_ModeTypeDef rf_mode);

/**
 * @brief   	This function serves to set RF power level index.
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */
extern void rf_set_power_level_index (RF_PowerTypeDef level);

/**
 * @brief   	This function serves to set RF power level index of zigbee.
 * @param[in]   RF_PowerIndexTypeDef - the RF power level index types.
 * @return  	none.
 */
extern void rf_set_power_level_index_zgb (RF_PowerIndexTypeDef level_idx);

/**
 * @brief   This function serves to get RF TX power level .
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  none.
 */
extern signed char rf_get_tx_power_level(void);

/**
*	@brief	  	this function is to Set byte length for access_code.
*	@param[in]	len  	Optional range: 2~5 , The effect for 3-byte access_code is not good.
*	@return	 	none
*/
extern void rf_acc_len_set(unsigned char len);
/**
*	@brief	  	this function performs to get access_code length.
*
*	@param[in]	len  	Optional range: 3~5
*										Note: The effect for 3-byte access_code is not good.
*
*	@return	 	access_byte_num[2:0]
*/
static inline unsigned char rf_acc_len_get(unsigned char len)
{
    return (read_reg8(0x405) & 0x07);
}
/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/
extern void rf_acc_code_set(unsigned char pipe_id, const unsigned char *addr);
/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe_id  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/
extern void rf_acc_code_get(unsigned char pipe_id, unsigned char *addr);
/**
 * @brief   	This function serves to set RF's channel.
 * @param[in]   chn - RF channel.
 * @param[in]   set - the value to set.
 * @return  	none.
 */
extern void rf_set_channel (signed char chn, unsigned short set);//general
/**
 * @brief   This function serves to set RF access command.
 * @param[in]  acc - the command.
 * @return  none.
 */
static inline void rf_access_code_comm (unsigned int acc)
{
	write_reg32 (0x800408, acc);
	//notice: This state will be reset after reset baseband
	write_reg8 (0x800405, read_reg8(0x405)|0x80);//setting access code needs to writ 0x405 access code trigger bit 1 to enable in long range mode,,and the mode of  BLE1M and BLE2M need not.
}

/**
 * @brief   This function serves to set RF access command.
 * @param[in]   acc - the command.
 * @return  none.
 */
static inline void rf_longrange_access_code_comm (unsigned int acc)
{
	write_reg32 (0x800408, acc);
	write_reg8 (0x800405, read_reg8(0x405)|0x80);//setting access code needs to writ 0x405 access code trigger bit to enable in long range mode,,and the mode of  BLE1M and BLE2M need not.
}


/**
*	@brief		this function is to enable/disable each access_code channel for
*				RF Rx terminal.
*	@param[in]	pipe  	Bit0~bit5 correspond to channel 0~5, respectively.
*						0Disable 1Enable
*						If enable is set as 0x3f (i.e. 00111111),
*						all access_code channels (0~5) are enabled.
*	@return	 	none
*/
static inline void rf_rx_acc_code_enable(unsigned char pipe)
{
    write_reg8(0x407, (read_reg8(0x407)&0xc0) | pipe); //rx_access_code_chn_en
}

/**
*	@brief		this function is to select access_code channel for RF Rx terminal.
*	@param[in]	pipe  	Bit0~bit5 correspond to channel 0~5, respectively.
*						0Disable 1Enable
*						If enable is set as 0x3f (i.e. 00111111),
*						all access_code channels (0~5) are enabled.
*	@return	 	none
*/
static inline void rf_tx_acc_code_select(unsigned char pipe)
{
    write_reg8(0xf15, (read_reg8(0xf15)&0xf8) | pipe); //Tx_Channel_man[2:0]
}


/**
 * @brief   This function serves to reset RF Tx/Rx mode.
 * @param   none.
 * @return  none.
 */

void rf_set_tx_rx_off(void);

/**
 * @brief   This function serves to turn off RF auto mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_rx_off_auto_mode(void)
{
	write_reg8 (0xf00, 0x80);
}
/**
*	@brief	  	This function serves to judge RF Tx/Rx state.
*	@param[in]	rf_status - Tx/Rx status.
*	@param[in]	rf_channel - RF channel.
*	@return	 	failed -1,else success.
*/

extern int rf_trx_state_set(RF_StatusTypeDef rf_status, signed char rf_channel);
/**
*	@brief	  	This function serves to get RF status.
*	@param[in]	none.
*	@return	 	RF Rx/Tx status.
*/
extern RF_StatusTypeDef rf_trx_state_get(void);

/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_txmode (void)
{
	write_reg8 (0x800f02, RF_TRX_OFF | BIT(4));	//TX enable
}

/**
 * @brief   This function serves to set RF tx settle time.
 * @tx_stl_us  tx settle time,the unit is us.The max value of this param is 0xfff;The default settling time value is 150us.
 * 			   The typical value is 113us (tx_settle time should not be less than this value).
 * @return  none.
 * @note	   Attention:It is not necessary to call this function to adjust the settling time in the normal sending state.
 */
static inline void 	tx_settle_adjust(unsigned short txstl_us)
{
	REG_ADDR16(0xf04) = (txstl_us - 1);
}

/**
 * @brief   This function serves to set pipe for RF Tx.
 * @param   pipe - RF Optional range .
 * @return  none
 */
static inline void rf_set_tx_pipe (unsigned char pipe)
{
	write_reg8 (0x800f15, 0xf0 | pipe);
}
/**
*	@brief	  	This function serves to set RF Tx packet.
*	@param[in]	rf_txaddr - the address RF to send packet.
*	@return	 	none.
*/
extern void rf_tx_pkt(unsigned char * addr);
/**
 * @brief   This function serves to send packet for RF.
 * @param   addr - the address need to be send.
 * @return  none.
 */
extern void	rf_tx_pkt_auto (void* addr);

/**
*	@brief	  	This function serves to determine whether sending a packet of data is finished
*	@param[in]	none.
*	@return	 	Yes: 1, NO: 0.
*/
static inline unsigned char rf_tx_finish(void)
{
    return ((read_reg8(0xf20) & BIT(1))==0x02);
}

/**
*	@brief	  	This function serves to clear the Tx finish flag bit.
*				After all packet data are sent, corresponding Tx finish flag bit
*				will be set as 1.By reading this flag bit, it can check whether
*				packet transmission is finished. After the check, its needed to
*				manually clear this flag bit so as to avoid misjudgment.
*   @param      none
*	@return	 	none
*/
static inline void rf_tx_finish_clear_flag(void)
{
    write_reg8(0xf20, 0x02);
}



/**
*	@brief	  	This function serves to determine whether sending a packet of rf_tx_acc_code_select is finished
*	@param[in]	none.
*	@return	 	Yes: 1, NO: 0.
*/

static inline unsigned char rf_is_rx_finish(void)
{
    return (unsigned char)((read_reg8(0xf20) & BIT(0))==0x01);
}

/**
*	@brief	  	This function is to set rx buffer
*
*	@param[out]	RF_RxAddr  	Pointer for Rx buffer in RAM(Generally its starting
*							address of an array.Should be 4-byte aligned)
*	@param[in]	size   		Rx buffer size (Its an integral multiple of 16)
*	@param[in]	PingpongEn 	Enable/Disable Ping-Pong buffer 1Enable 0Disable
*							Note:
*							When PingpongEn is set as 0, received RF data will
*							be stored in RAM pointed by  RF_RxAddr.
*							When PingpongEn is set as 1, received RF data will
*							be stored in buffer0 and buffer1 successively.
*							The RAM size reserved for received RF data should be
*							double of Size.
*
*	@return	 	none
*/

extern void  rf_rx_buffer_set(unsigned char *  RF_RxAddr, int size, unsigned char  PingpongEn);

/**
*	@brief	  	This function serves to  reconfigure the buffer
*	@param[out]	RF_RxAddr  	Pointer for Rx buffer in RAM(Generally its starting
*							address of an array.Should be 4-byte aligned)
*	@return	 	Yes: 1, NO: 0.
*/

static inline void rf_rx_buffer_reconfig(unsigned short RF_RxAddr)
{
	reg_dma2_addr = (unsigned short)(RF_RxAddr);
}

/**
*	@brief	  	This function serves to determine whether a packet of data received is right
*	@param[in]	none.
*	@return	 	Yes: 1, NO: 0.
*/
static inline unsigned char rf_is_rx_right(void)
{
    return ((REG_ADDR8(0x44f)&0x0f)==0);
}

/**
*	@brief	  	This function serves to clear the Rx finish flag bit.
*				After all packet data are resive, corresponding Rx finish flag bit
*				will be set as 1.By reading this flag bit, it can check whether
*				packet transmission is finished. After the check, its needed to
*				manually clear this flag bit so as to avoid misjudgment.
*   @param      none
*	@return	 	none
*/
static inline void rf_rx_finish_clear_flag(void)
{
    write_reg8(0xf20, 0x01);
}

/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_rxmode (void)
{
	write_reg8 (0x800f02, RF_TRX_OFF);
    write_reg8 (0x800428, RF_TRX_MODE | BIT(0));	//rx enable
    write_reg8 (0x800f02, RF_TRX_OFF | BIT(5));	// RX enable
}

/**
*	@brief	  	This function serves to start Tx of ble_mode.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer. It determines when to
*						  	  start ble mode and send packet.
*	@return	 	none
*/
extern void rf_start_btx (void* addr, unsigned int tick);

/**
*	@brief	  	This function serves to start Rx of auto mode. In this mode,
*				RF module stays in Rx status until a packet is received or it fails to receive packet when timeout expires.
*				Timeout duration is set by the parameter "tick".
*				The address to store received data is set by the function addr.
*	@param[in]	addr - The address to store received data.
*	@param[in]	tick - Unit is us. It indicates timeout duration in Rx status.Max value: 0xffffff (16777215)
*	@return	 	none
*/
extern void rf_start_brx  (void* addr, unsigned int tick);

/**
*	@brief	  	This function serves to start Tx.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/

extern void rf_start_stx  (void* addr, unsigned int tick);

/**
*	@brief	  	This function serves to start Rx.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/
extern void rf_start_srx  (unsigned int tick);


/**
*	@brief	  	This function serves to start stx2rx mode of auto_mode.
*				In this mode, a packet is sent first,RF module waits for 10us,
*				stays in Rx status until data is received or timeout expires,
*				then exits this mode.Timeout duration is set by the parameter
*				timeout_us.The address to store received data is set by the
*				function RF_RxBufferSet.
*
*	@param[in]	addr  Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick   	Tick value of system timer. It determines when
*								to start StxToRx mode and send packet.
*	@param[in]	timeout_us  Unit is us. It indicates timeout duration in
*							 	Rx status.Max value: 0xfff (4095)
*
*	@return	 	none
*/


extern void rf_start_stx2rx  (void* addr, unsigned int tick);



/**
*	@brief	  	This function serves to start srx2tx mode of auto_mode.
*				In this mode,RF module stays in Rx status until a packet is
*				received or it fails to receive packetwhen timeout expires.
*				If a packet is received within the timeout duration, RF module
*				will wait for 10us,send a packet, and then exit this mode.
*				If it fails to receive packet when timeout expires, RF module
*				will directly exit this mode.Timeout duration is set by the
*				parameter "timeout_us".	The address to store received data is set
*				by the function RF_RxBufferSet.
*
*	@param[in]	addr 	Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick   Tick value of system timer. It determines when to
*								start SrxToTx mode.
*	@param[in]	timeout_us  Unit is us. It indicates timeout duration in Rx status.
*								Max value: 0xffffff (16777215)
*
*	@return	 	none
*/

extern void rf_start_srx2tx  (void* addr, unsigned int tick);


/*******************************            Private APIs            **************************/

/**
 * @brief   This function serves to set the ble channel.
 * @param   chn_num - channel numbers.
 * @return  none.
 */
void rf_set_ble_channel (signed char chn_num);

/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on ()
{
	write_reg8  (0x800f02, 0x45 | BIT(4));	// TX enable
}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done ()
{
	write_reg8  (0x800f02, 0x45);
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 */
static inline void reset_sn_nesn(void)
{
	REG_ADDR8(0xf01) =  0x01;
}

/**
 * @brief   This function serves to reset the RF sn.
 * @param   none.
 * @return  none.
 */
static inline void rf_reset_sn (void)
{
	write_reg8  (0x800f01, 0x3f);
	write_reg8  (0x800f01, 0x00);
}

/**
 * @brief   This function serves to set pipe for RF Tx.
 * @param   p - RF Optional range .
 * @return  none
 */
static inline void rf_set_ble_crc (unsigned char *p)
{
	write_reg32 (0x800424, p[0] | (p[1]<<8) | (p[2]<<16));
}

/**
 * @brief   This function serves to set CRC value for RF.
 * @param[in]  crc - CRC value.
 * @return  none.
 */
static inline void rf_set_ble_crc_value (unsigned int crc)
{
	write_reg32 (0x800424, crc);
}

/**
 * @brief   This function serves to set CRC advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_crc_adv ()
{
	write_reg32 (0x800424, 0x555555);
}

/**
 * @brief   This function serves to set RF access code.
 * @param[in]   p - the address to access.
 * @return  none
 */
static inline void rf_set_ble_access_code (unsigned char *p)
{
	write_reg32 (0x800408, p[3] | (p[2]<<8) | (p[1]<<16) | (p[0]<<24));
}

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
static inline void rf_set_ble_access_code_value (unsigned int ac)
{
	write_reg32 (0x800408, ac);
}

/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_access_code_adv (void)
{
	write_reg32 (0x800408, 0xd6be898e);
}

/**
 * @brief   This function serves to set RF access code 6bit to 32bit.
 * @param[in]  code - the access code.
 * @return  the value of the access code.
 */
static inline unsigned int rf_access_code_16to32 (unsigned short code)
{
	unsigned int r = 0;
	for (int i=0; i<16; i++) {
		r = r << 2;
		r |= code & BIT(i) ? 1 : 2;
	}
	return r;
}

/**
 * @brief   This function serves to set RF access code 6bit to 32bit.
 * @param[in]   code - the access code.
 * @return  the value of access code.
 */
static inline unsigned short rf_access_code_32to16 (unsigned int code)
{
	unsigned short r = 0;
	for (int i=0; i<16; i++) {
		r = r << 1;

		r |= (code & BIT(i*2)) ? 1 : 0;

	}
	return r;
}


/**
 * @brief   This function serves to judge the statue of  RF receive.
 * @param   none.
 * @return  none.
 */
static inline unsigned char is_rf_receiving_pkt(void)
{
	//if the value of [5:4] of the reg_0x448 is 0b10 or 0b11, it means that the RF is in the receiving packet phase.(confirmed by junwen)
	return (((read_reg8(0x448)>>5)& 1) == 1);
}

/**
 * @brief   This function serves to set The channel .
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  none.
 */
extern void rf_set_channel_500k(signed short chn, unsigned short set);
/**
*	@brief		this function is to set Private mode payload len  for RF.
*	@param[in]	len - length of payload.
*	@return	 	none.
*/
static inline void rf_fix_payload_len_set(int len)
{
    write_reg8(0x404, read_reg8(0x404)|0x03); //select private header mode
    write_reg8(0x406, len);
}


/**
 * @brief      This function
 * @param[in]  size - the expected size of dma2
 * @param[in]  PingpongEn - the flag of g_RFRxPingpongEn
 * @return     none
 */
extern void  rf_rx_cfg(int size, unsigned char  PingpongEn);

/**
*	@brief	  	This function serves to get CRC
*   @param[in]  *input  		- the content of payload
*   @param[in]	len 			- the length of payload
*   @param[in]  init_val 		- the initial value of CRC
*	@return	 	init_val 		- CRC
*/
extern unsigned short crc16_ccitt_cal(unsigned char *input, unsigned int len, unsigned short init_val);

/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *preamble  		- the content of preamble
*   @param[in]	preamble_len 	- the length of preamble
*   @param[in]  *acc_code 		- the content of access code
*   @param[in]  acc_len			- the length of access code
*   @param[in]  *payload		- the content of payload
*   @param[in]	pld_len			- the length of payload
*   @param[in]	*tx_buf			- the data need to be sent
*   @param[in]	crc_init		- the initial value of CRC
*	@return	 	none
*/

extern void rf_tx_500k_simulate_100k(unsigned char *preamble, unsigned char preamble_len,
                              unsigned char *acc_code, unsigned char acc_len,
                              unsigned char *payload, unsigned char pld_len,
                              unsigned char *tx_buf, unsigned short crc_init);



/**
*	@brief	  	This function serves to get rssi
*   @param[in]      none
*	@return	 	none
*/

static inline signed char rf_rssi_get_154(void)
{
	return (((signed char)(read_reg8(0X441))) - 110);
}


/**
*	@brief	  	This function is to start energy detect of the current channel for zigbee mode
*				Before using it.
*   @param[in]  none
*	@return	 	none
*/
extern void rf_ed_detecct_154(void);
/**
*	@brief	  	This function is to stop energy detect and get energy detect value of
*				the current channel for zigbee mode.
*   @param      none
*	@return	 	rf_ed:0x00~0xff
*
*/

extern unsigned char rf_stop_ed_154(void);

/**
*	@brief	  	This function serves to set pin for RFFE of RF
*   @param[in]     tx_pin - select pin to send
*   @param[in]      rx_pin - select pin to receive
*	@return	 	none
*
*/
extern void rf_rffe_set_pin(RF_PATxPinDef tx_pin, RF_LNARxPinDef rx_pin);

/**
 * @brief      This function process the received packet in 1mbps private mode only for hanshow for the
 *             compatibility with third-party chips. The process includes data-whitening
               transformation and crc check.
 * @param[in]  rx_buf - the rf rx buffer containing the received packet(dma length+payload+3 byte crc)
 * @param[in]  len - the expected rx length of private mode, containing payload and 3byte crc
 * @return     the status of the processing procedure. 1: the received packet is correct, 0: the received packet is incorrect
 */

unsigned char rx_packet_process_1mbps(unsigned char *rx_buf, unsigned int len);
/**
 * @brief      This function process the tx packet in 1mbps private mode only for hanshow for the
 *             compatibility with third-party chips. The process includes data-whitening
               transformation and crc padding.
 * @param[in]  tx_buf - the rf tx buffer containing the tx packet(dma length+payload+3 byte crc)
 * @param[in]  len - the expected tx length of private mode, containing payload and 3byte crc
 * @return     none
 */
void tx_packet_process_1mbps(unsigned char *tx_buf, unsigned char *payload, unsigned int len);

/**
 * @brief      This function serves to init the 2-wire-PTA.
 * @param[in]  ble_priority_pin - the pin of ble_priority.
 * @param[in]  wlan_active_pin  - the pin of wlan_active.
 * @param[in]  ble_priority_mode - the mode of ble_priority pin.
 *             when the mode is PTA_BLE_PRIORITY_TX,the pin of ble_priority will be high if tx.
 *             when the mode is PTA_BLE_PRIORITY_RX,the pin of ble_priority will be high if rx.
 *             when the mode is PTA_BLE_PRIORITY_TRX,the pin of ble_priority will be high if tx and rx.
 * @return     none
 */
extern void rf_2wire_pta_init(PTA_BlePriorityPinDef ble_active_pin,GPIO_PinTypeDef wlan_active_pin,PTA_2WireModeDef ble_priority_mode);
/**
 * @brief      This function serves to init the 3-wire-PTA.
 * @param[in]  ble_active_pin - the pin of ble_active.
 * @param[in]  ble_status_pin - the pin of ble_status.
 * @param[in]  wlan_deny_pin  - the pin of wlan_deny.
 * @param[in]  ble_status_mode  - the mode of ble_statu.
               when the mode is PTA_BLE_STATUS_TX,the ble_status pin will be high if stx.
			   when the mode is PTA_BLE_STATUS_RX,the ble_status pin will be high if srx.
 * @return     none
 * @note	   Attention:In the three-wire PTA mode, there will be a period of time t1 to
 * 			   detect wlan_active and a time t2 to switch the ble_status state before the
 * 			   action is triggered.The actual start time of the corresponding RF action will
 * 			   shift backward by the larger of the two.These two periods of time can be set
 * 			   by function rf_set_pta_t1_time and function rf_set_pta_t2_time respectively.
 */
extern void rf_3wire_pta_init(PTA_BleActivePinDef ble_active_pin,PTA_BleStatusPinDef ble_status_pin,PTA_WlanDenyPinDef wlan_deny_pin,PTA_3WireModeDef ble_status_mode);

/**
 * @brief      This function is to disable PN
 * @return     none
 */
void  rf_pn_disable();

/**
 * @brief	  	This function serves to close internal cap;
 * @return	 	none.
 */
void rf_turn_off_internal_cap(void);

/**
 * @brief	  	This function serves to update the value of internal cap.
 * @param[in]  	value   - The value of internal cap which you want to set.the value range from 0 to 0x3f.
 * @return	 	none.
 */
void rf_update_internal_cap(unsigned char value);

/**
 * @brief	This function serve to change the length of preamble.
 * @param[in]	len		-The value of preamble length.Set the register bit<0>~bit<4>.
 * @return		none
 */
void rf_set_preamble_len(unsigned char len);

/**
 * @brief   This function serves to set RF rx timeout.
 * @param[in]	timeout_us	-	rx_timeout after timeout_us us,The maximum of this param is 0xfff.
 * @return  none.
 */
void rf_set_rx_timeout(unsigned short timeout_us);

/**
 * @brief	  	This function is used to  set the modulation index of the receiver.
 *              This function is common to all modes,the order of use requirement:configure mode first,then set the the modulation index,default is 0.5 in drive,
 *              both sides need to be consistent otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100.
 * @return	 	none.
 */
void rf_set_rx_modulation_index(RF_MIVauleTypeDef mi_value);

/**
 * @brief	  	This function is used to  set the modulation index of the sender.
 *              This function is common to all modes,the order of use requirement:configure mode first,then set the the modulation index,default is 0.5 in drive,
 *              both sides need to be consistent otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100.
 * @return	 	none.
 */
void rf_set_tx_modulation_index(RF_MIVauleTypeDef mi_value);

/**
 * @brief   	This function serves to set RF's channel.The step of this function is in KHz.
 *				The frequency set by this function is (chn+2400) MHz+chn_k KHz.
 * @param[in]   chn_m - RF channel. The unit of this parameter is MHz, and its set frequency
 * 					 	point is (2400+chn)MHz.
 * @param[in]   chn_k - The unit of this parameter is KHz, which means to shift chn_k KHz to
 * 						the right on the basis of chn.Its value ranges from 0 to 999.
 * @param[in]	trx_mode - Defines the frequency point setting of tx mode or rx mode.
 * @return  	none.
 */
void rf_set_channel_k_step(signed char chn_m,unsigned int chn_k,rf_trx_chn_e trx_mode);//general

/**
 *	@brief	  	this function serve to enable the tx timing sequence adjusted.
 *	@param[in]	none
 *	@return	 	none
*/
static inline void rf_tx_fast_settle_en(void)
{
	write_reg8(0x1229,read_reg8(0x1229)|0x02);
}

/**
 *	@brief	  	this function serve to disable the tx timing sequence adjusted.
 *	@param[in]	none
 *	@return	 	none
*/
static inline void rf_tx_fast_settle_dis(void)
{
	write_reg8(0x1229,read_reg8(0x1229)&0xfd);
}

/**
 *	@brief	  	this function serve to enable the rx timing sequence adjusted.
 *	@param[in]	none
 *	@return	 	none
*/
static inline void rf_rx_fast_settle_en(void)
{
	write_reg8(0x1229,read_reg8(0x1229)|0x01);
}

/**
 *	@brief	  	this function serve to disable the rx timing sequence adjusted.
 *	@param[in]	none
 *	@return	 	none
*/
static inline void rf_rx_fast_settle_dis(void)
{
	write_reg8(0x1229,read_reg8(0x1229)&0xfe);
}

/**
 *	@brief	  	this function serve to adjust rx settle timing sequence.
 *	@param[in]	rx_settle_us  	After adjusting the timing sequence, the time required for rx to settle.
 *	@return	 	none
 *	@note		RX_SETTLE_TIME_77US :Do a normal calibration at the beginning.
 *				RX_SETTLE_TIME_44US :Receive for a period of time and then do a normal calibration.
*/
void rf_rx_fast_settle_init(rf_rx_fast_seetle_time_e rx_settle_us);

/**
 *	@brief	  	this function serve to adjust tx settle timing sequence.
 *	@param[in]	tx_settle_us  	After adjusting the timing sequence, the time required for tx to settle.
 *	@return	 	none
 *	@note		TX_SETTLE_TIME_101US : After frequency hopping, a normal calibration must be done.
 *				TX_SETTLE_TIME_53US  : Do a normal calibration at the beginning.
*/
void rf_tx_fast_settle_init(rf_tx_fast_seetle_time_e tx_settle_us);

/**
 * @brief   This function serves to set RF tx settle time and rx settle time.
 * @rx_stl_us  rx settle time,the unit is us.The max value of this param is 0xfff;The default settling time value is 150us.
 * 			   The typical value is 85us (rx_settle time should not be less than this value).
 * @return  none.
 * @note	   Attention:It is not necessary to call this function to adjust the settling time in the normal packet receiving state.
 */
static inline void rf_set_rx_settle_time( unsigned short rx_stl_us )
{
	 rx_stl_us &= 0x0fff;
	 write_reg16(0xf0c,(rx_stl_us - 1));
}

/**
 * @brief	    Specifies the time after assertion of BLE_ACTIVITY signal at which the WLAN_DENY should be stable and is sampled by BLE device to determine whether to launch transaction.
 * @param[in]	time_us - Set the time value of the conversion,the unit is "us".The default value is 0x31, and the maximum value is 0xff. No special requirements do not need to call
 * 				this function to make changes.
 * @return	 	none.
 */
static inline void rf_set_pta_t1_time(unsigned char time_us)
{
	write_reg8(0xf12,(time_us - 1));
}

/**
 * @brief	    The state switch of ble_status from the last trigger event to the next trigger event should be completed within this period of time.
 * @param[in]	time_us - Set the time value of the conversion,the unit is "us".The default value is 0x13, and the maximum value is 0xff. No special requirements do not need to call
 * 				this function to make changes.
 * @return	 	none.
 */
static inline void rf_set_pta_t2_time(unsigned char time_us)
{
	write_reg8(0xf13,(time_us - 1));
}

/**
 * @brief	    This function is used to enable the ldo rxtxlf bypass function, and the calibration value
 * 				written by the software will take effect after enabling.
 * @param[in]	none.
 * @return	 	none.
 */
static inline void rf_ldot_ldo_rxtxlf_bypass_en(void)
{
	write_reg8(0x12e4,read_reg8(0x12e4)|BIT(1));
}

/**
 * @brief	    This function is used to close the ldo rxtxlf bypass function, and the hardware will
 * 				automatically perform the calibration function after closing.
 * @param[in]	none.
 * @return	 	none.
 */
static inline void rf_ldot_ldo_rxtxlf_bypass_dis(void)
{
	write_reg8(0x12e4,read_reg8(0x12e4)&(~BIT(1)));
}

/**
 * @brief		This function is mainly used to set the antenna switching mode. Vulture support three different
 * 				table lookup sequences.The setting here is just the order of the table lookup, and the content
 * 				in the table is the number of the antenna to be switched to.The switching sequence of the antenna
 * 				needs to be determined by the combination of the table look-up sequence and the antenna number in
 * 				the table,so this function is usually used together with the rf_aoa_aod_ant_lut function.
 * @param[in]	pattern 	- Enumeration of several different look-up table order modes.Refer to the corresponding
 * 							  enumeration annotation for the meaning of the mode.
 * @return		none.
 */
void rf_aoa_aod_ant_pattern(rf_ant_pattern_e pattern);

/**
 * @brief		This function is mainly used to set the number of antennas enabled by the multi-antenna board in the
 * 				AOA/AOD function;the vulture series chips currently support up to 8 antennas for switching.By default,
 * 				it is set to 8 antennas. After configuring the RF-related settings, you can set the number of enabled
 * 				antennas, and this setting needs to be completed before sending and receiving packets.
 * @param[in]	ant_num 	- The number of antennas, the value ranges from 1 to 8.
 * @return		none.
 */
void rf_aoa_aod_set_ant_num(unsigned char ant_num);


/**
 * @brief		This function is used to set the antenna switching sequence table. The content in the table is the
 * 				antenna sequence number that needs to be switched to when the position is found by the look-up table.
 * 				Since determining the antenna switching sequence needs to determine the order of the table lookup and
 * 				the setting of the table content, this function is usually used in conjunction with the function
 * 				rf_aoa_aod_ant_pattern.
 * @param[in]	dat      - Antenna serial number written into the antenna switching sequence table.The value in the table
 * 						 corresponds to the antenna number that needs to be switched to when it is found in the table.The
 * 						 value range is 0 to 7.
 * @return	  	none.
 */
void rf_aoa_aod_ant_lut(unsigned char *dat);


/**
 * @brief		This function is mainly used to set the type of AOA/AOD iq data. The default data type is 8bit.This
 * 				configuration can be done before starting to receive the package.
 * @param[in]	mode	- The length of each I or Q data.
 * @return		none.
 * @note		Attention :When the iq data is 20bit, it cannot be used with the 0.25us mode, which will cause the
 * 						   sampling data to overflow.
 */
void rf_aoa_aod_iq_data_mode(rf_aoa_aod_iq_data_mode_e mode);

/**
 * @brief		This function enables the receiving functions of AOA/AOD in ordinary format packets or ADV format
 * 				packets.After configuring the RF function, if you want to receive a packet with AOA/AOD information,
 * 				you can call this function to make the chip enter the corresponding mode to receive the packet. The
 * 				default state is a normal package without AOA/AOD information.
 * @param[in]	mode - AOA/AOD broadcast package or normal package rx mode.When the parameter is RF_AOA_AOD_RX_OFF,
 * 				the normal packet without AOA/D is received.
 * @return		none.
 */
void rf_aoa_aod_set_rx_mode(rf_aoa_aod_rx_mode_e mode);

/**
 * @brief		This function enables the sending functions of AOA/AOD in ordinary format packets or ADV format
 * 				packets.After configuring the RF function, if you want to send  a packet with AOA/AOD information,
 * 				you can call this function to make the chip enter the corresponding mode to send the packet. The
 * 				default state is a normal package without AOA/AOD information.
 * @param[in]	mode - AOA/AOD broadcast package or normal package tx mode.When the parameter is RF_AOA_AOD_RX_OFF,
 * 				the normal packet without AOA/D is sending.
 * @return		none.
 */
static inline void rf_aoa_aod_set_tx_mode(rf_aoa_aod_tx_mode_e mode)
{
	write_reg8(0x438,((read_reg8(0x438) & 0xf3) | mode));
}

/**
 * @brief		This function is used to set the position of the first sample after the reference.The default is in
 * 				the middle of the first sample_slot; The starting position of sampling can be fine-tuned through
 * 				parameter setting to select the best sampling point.
 * @param[in]	sample_point_offset - sample_point_offset:The parameter range is -45 to 210.If the parameter is negative,
 * 				the position of the sampling point moves forward. The absolute value of the parameter is multiplied by
 * 				0.125us.If the parameter is positive, the position of the sampling point moves backward. The parameter
 * 				is multiplied by 0.125us.
 * @return		none.
 */
static inline void rf_aoa_aod_sample_point_adjust(char sample_point_offset)
{
	write_reg8(0x43b,0x2d + sample_point_offset);
}

/**
 * @brief		This function is mainly used to set the sampling interval time in the AOA/AOD function.After
 * 				configuring RF, you can call this function to configure sample interval time.
 * @param[in]	sample_time		- AOA or AOD sampling interval time.
 * @return		none.
 * @note	 	When the time is 0.25us, it cannot be used with the 20bit iq data type, which will cause the sampling
 * 				data to overflow.
 */
void rf_aoa_aod_sample_interval_time(rf_aoa_aod_sample_interval_time_e sample_time);

/**
 * @brief		This function is mainly used to initialize the parameters related to AOA/AOD antennas, including the
 * 				number of antennas, the pins for controlling the antennas,the look-up mode of antenna switching, and
 * 				the content of the antenna switching sequence table.
 * @param[in]	ant_num			- The number of antennas, the value ranges from 1 to 8.
 * @param[in]	ant_pin_config:	- Control antenna pin selection and configuration.The parameter setting needs to be
 * 								  set according to the number and position of the control antenna.For example,if you
 * 								  need to control four antennas, it is best to use Antsel0 and Antsel2.
 * @param[in]	pattern			- Enumeration of several different look-up table order modes.
 * @param[in]	dat 			- The antenna value written into the antenna switching sequence table ranges from 0 to 7.
 * @return		none.
 */
void rf_aoa_aod_ant_init(unsigned char num,rf_ant_pin_sel_t * ant_pin_config,rf_ant_pattern_e pattern,unsigned char *dat);

/**
 * @brief		This function is mainly used to set the parameters related to AOA/AOD sampling, including the length
 * 				of IQ data, sampling interval, and sampling offset.
 * @param[in]	iq_data			    - The length of each I or Q data.
 * @param[in]	sample_interval	    - AOA or AOD sampling interval time.
 * @param[in]	sample_point_offset	- The parameter range is -45 to 210.If the parameter is negative,the position of
 * 									  the sampling point moves forward.The absolute value of the parameter is multiplied
 * 									  by 0.125us.If the parameter is positive, the position of the sampling point moves
 * 									  backward. The parameter is multiplied by 0.125us.
 * @return		none.
 */
void rf_aoa_aod_sample_init(rf_aoa_aod_iq_data_mode_e iq_data,rf_aoa_aod_sample_interval_time_e sample_interval,char sample_point_offset);

/**
 * @brief		This function is used to calculate the number of IQ groups in the received AOA/AOD packet.
 * @param[in]	p				- Received packet address pointer.
 * @return		Returns the number of groups of iq in the package.
 */
unsigned int rf_aoa_aod_iq_group_number(unsigned char *p);

/**
 * @brief		This function is mainly used to obtain the offset of header information in the packet data received
 * 				in AOA/AOD mode.
 * @param[in]	p				- Received packet address pointer.
 * @return		The return value is the offset of header information in the packet.
 */
unsigned int rf_aoa_aod_hdinfo_offset(unsigned char *p);

/**
 * @brief		This function is mainly used to detect whether the DMA length of the received packet is correct in
 * 				the AOA/AOD mode.
 * @param[in]	p				- Received packet address pointer.
 * @return		Return length to judge whether it is correct, 1: ok, 0: false
 */
unsigned char rf_aoa_aod_is_rx_pkt_len_ok(unsigned char *p);

/**
 * @brief		This function is mainly used to obtain the CRC value in the AOA/AOD packet.
 * @param[in]	p				- Received packet address pointer.
 * @return		The return value is the rssi value in headerinformation.
 */
signed char rf_aoa_aod_get_pkt_rssi(unsigned char *p);

/**
 * @brief		This function is used to obtain the timestamp information in the AOA/AOD package.
 * @param[in]	p				- Received packet address pointer.
 * @return		Returns the timestamp value in the packet.
 */
unsigned int rf_aoa_aod_get_pkt_timestamp(unsigned char *p);

/**
 * @brief          This function serves to set the which irq enable.
 * @param[in]      mask     - Options that need to be enabled.
 * @return         Yes: 1, NO: 0.
 */
static inline void rf_set_irq_mask(rf_irq_e mask)
{
    BM_SET(reg_rf_irq_mask,mask);
}

/**
 * @brief          This function is used to clear the irq mask that needs to be cleared.
 * @param[in]      mask     - Options that need irq value.
 * @return         none.
 */
static inline void rf_clr_irq_mask(rf_irq_e mask)
{
    BM_CLR(reg_rf_irq_mask ,mask);
}

/**
 * @brief          This function serves to judge whether it is in a certain state.
 * @param[in]      mask     - Options that need irq status.
 * @return         Yes: 1, NO: 0.
 */
static inline unsigned short rf_get_irq_status(rf_irq_e status)
{
    return ((unsigned short )BM_IS_SET(reg_rf_irq_status,status));
}

/**
 * @brief        This function serves to clear the irq finish flag bit that needs to be cleared.
 *              When the status flag bit is 1, this flag bit needs to be cleared manually to avoid the next misjudgment.
 * @param[in]    status     - Options that need irq status.
 * @return       none.
 */
static inline void rf_clr_irq_status(rf_irq_e status)
{
     reg_rf_irq_status = status;
}

#endif

/********************************************************************************************************
 * @file    rf_drv.h
 *
 * @brief   This is the header file for B85
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
#ifndef _RF_DRV_H_
#define _RF_DRV_H_

#include "bsp.h"
#include "compiler.h"
#include "gpio.h"
#define RF_CHN_TABLE 		0x8000

/**
 * @brief	Take 4 antennas as an example to illustrate the antenna switching sequence
 * 			SWITCH_SEQ_MODE0	- antenna index switch sequence 01230123
 * 			SWITCH_SEQ_MODE1	- antenna index switch sequence 0123210
 */
typedef enum{
	SWITCH_SEQ_MODE0		 = 0,
	SWITCH_SEQ_MODE1		 = BIT(6),
}rf_ant_pattern_e;

/**
 * @brief	It can be defined as atsel0 pin.
 */
typedef enum
{
	RF_ANT_SEL0_PC5 = GPIO_PC5,
	RF_ANT_SEL0_PD6 = GPIO_PD6,
	RF_ANT_SEL0_NONE = 0,
}rf_antsel0_pin_e;

/**
 * @brief	It can be defined as atsel1 pin.
 */
typedef enum
{
	RF_ANT_SEL1_PC6 = GPIO_PC6,
	RF_ANT_SEL1_PB0 = GPIO_PB0,
	RF_ANT_SEL1_NONE = 0,
}rf_antsel1_pin_e;

/**
 * @brief	It can be defined as atsel2 pin.
 */
typedef enum
{
	RF_ANT_SEL2_PC7 = GPIO_PC7,
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
 * @note   Attention:Kite only support 8 bit iq data.
 * |                 |                         |
 * | :-------------- | :---------------------- |
 * |   	 <15:8>      |          <7:0>          |
 * |   IQ byte len   |   iq data bit num mode  |
 */
typedef enum{
	IQ_8_BIT_MODE   		= 0x0200,
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
 * @brief  The chip can be enabled to send AOA/AOD packets in ADV or ACL format.
 */
typedef enum{
	RF_TX_ACL_AOA_AOD_EN  = BIT(2),
	RF_TX_ADV_AOA_AOD_EN  = BIT(3),
	RF_AOA_AOD_TX_OFF     = 0
}rf_aoa_aod_tx_mode_e;

/*
 * @brief  AOA/AOD sample interval time type enumeration.bit<15:8>:4 times the interval value (to facilitate subsequent
 *         data processing),bit<7:0>:Set the register value of the corresponding interval mode.
 * |                 |                         |
 * | :-------------- | :---------------------- |
 * |   	  15:8>      |          <7:0>          |
 * |(interval time)*4|   sample interval mode  |
 */
typedef enum{
	SAMPLE_AOA_4US_AOD_CTEINFO_INTERVAL   	= 0x1000,/**< In this case sample interval of aoa is 4us, and aod will judge
														  sample interval is 4us or 2us according to CTE info.*/
	SAMPLE_2US_INTERVAL   		= 0x0803,
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
 *  @brief  Define power list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_P10p46dBm = 63, //  10.46 dbm
	 RF_POWER_P10p29dBm = 61, //  10.29 dbm
	 RF_POWER_P10p01dBm = 58, //  10.01 dbm
	 RF_POWER_P9p81dBm  = 56, //   9.81 dbm
	 RF_POWER_P9p48dBm  = 53, //   9.48 dbm
	 RF_POWER_P9p24dBm  = 51, //   9.24 dbm
	 RF_POWER_P8p97dBm  = 49, //   8.97 dbm
	 RF_POWER_P8p73dBm  = 47, //   8.73 dbm
	 RF_POWER_P8p44dBm  = 45, //   8.44 dbm
	 RF_POWER_P8p13dBm  = 43, //   8.13 dbm
	 RF_POWER_P7p79dBm  = 41, //   7.79 dbm
	 RF_POWER_P7p41dBm  = 39, //   7.41 dbm
	 RF_POWER_P7p02dBm  = 37, //   7.02 dbm
	 RF_POWER_P6p60dBm  = 35, //   6.60 dbm
	 RF_POWER_P6p14dBm  = 33, //   6.14 dbm
	 RF_POWER_P5p65dBm  = 31, //   5.65 dbm
	 RF_POWER_P5p13dBm  = 29, //   5.13 dbm
	 RF_POWER_P4p57dBm  = 27, //   4.57 dbm
	 RF_POWER_P3p94dBm  = 25, //   3.94 dbm
	 RF_POWER_P3p23dBm  = 23, //   3.23 dbm
	 /*VANT*/
	 RF_POWER_P3p01dBm  = BIT(7) | 63,   //   3.01 dbm
	 RF_POWER_P2p81dBm  = BIT(7) | 61,   //   2.81 dbm
	 RF_POWER_P2p61dBm  = BIT(7) | 59,   //   2.61 dbm
	 RF_POWER_P2p39dBm  = BIT(7) | 57,   //   2.39 dbm
	 RF_POWER_P1p99dBm  = BIT(7) | 54,   //   1.99 dbm
	 RF_POWER_P1p73dBm  = BIT(7) | 52,   //   1.73 dbm
	 RF_POWER_P1p45dBm  = BIT(7) | 50,   //   1.45 dbm
	 RF_POWER_P1p17dBm  = BIT(7) | 48,   //   1.17 dbm
	 RF_POWER_P0p90dBm  = BIT(7) | 46,   //   0.90 dbm
	 RF_POWER_P0p58dBm  = BIT(7) | 44,   //   0.58 dbm
	 RF_POWER_P0p04dBm  = BIT(7) | 41,   //   0.04 dbm
	 RF_POWER_N0p14dBm  = BIT(7) | 40,   //  -0.14 dbm
	 RF_POWER_N0p97dBm  = BIT(7) | 36,   //  -0.97 dbm
	 RF_POWER_N1p42dBm  = BIT(7) | 34,   //  -1.42 dbm
	 RF_POWER_N1p89dBm  = BIT(7) | 32,   //  -1.89 dbm
	 RF_POWER_N2p48dBm  = BIT(7) | 30,   //  -2.48 dbm
	 RF_POWER_N3p03dBm  = BIT(7) | 28,   //  -3.03 dbm
	 RF_POWER_N3p61dBm  = BIT(7) | 26,   //  -3.61 dbm
	 RF_POWER_N4p26dBm  = BIT(7) | 24,   //  -4.26 dbm
	 RF_POWER_N5p03dBm  = BIT(7) | 22,   //  -5.03 dbm
	 RF_POWER_N5p81dBm  = BIT(7) | 20,   //  -5.81 dbm
	 RF_POWER_N6p67dBm  = BIT(7) | 18,   //  -6.67 dbm
	 RF_POWER_N7p65dBm  = BIT(7) | 16,   //  -7.65 dbm
	 RF_POWER_N8p65dBm  = BIT(7) | 14,   //  -8.65 dbm
	 RF_POWER_N9p89dBm  = BIT(7) | 12,   //  -9.89 dbm
	 RF_POWER_N11p4dBm  = BIT(7) | 10,   //  -11.4 dbm
	 RF_POWER_N13p29dBm = BIT(7) | 8,   //  -13.29 dbm
	 RF_POWER_N15p88dBm = BIT(7) | 6,   //  -15.88 dbm
	 RF_POWER_N19p27dBm = BIT(7) | 4,   //  -19.27 dbm
	 RF_POWER_N25p18dBm = BIT(7) | 2,   //  -25.18 dbm

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
	 RF_VBAT_POWER_P2p45dBm  = 0x13, 		//   2.45 dbm
	 RF_VBAT_POWER_P0p95dBm  = 0x10, 		//   0.95 dbm
	 RF_VBAT_POWER_P0p0dBm   = 0x0e, 		//   0.00 dbm
	 RF_VBAT_POWER_N1p35dBm  = 0x0c, 		//   -1.35 dbm
	 RF_VBAT_POWER_N2p75dBm  = 0x0a, 		//   -2.75 dbm
	 RF_VBAT_POWER_P4p75dBm  = 0x08, 		//   -4.75 dbm
} RF_PowerTypeDef;

extern const RF_PowerTypeDef rf_power_Level_list[60];
/**
 *  @brief  Define power index list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_INDEX_P10p46dBm,
	 RF_POWER_INDEX_P10p29dBm,
	 RF_POWER_INDEX_P10p01dBm,
	 RF_POWER_INDEX_P9p81dBm,
	 RF_POWER_INDEX_P9p48dBm,
	 RF_POWER_INDEX_P9p24dBm,
	 RF_POWER_INDEX_P8p97dBm,
	 RF_POWER_INDEX_P8p73dBm,
	 RF_POWER_INDEX_P8p44dBm,
	 RF_POWER_INDEX_P8p13dBm,
	 RF_POWER_INDEX_P7p79dBm,
	 RF_POWER_INDEX_P7p41dBm,
	 RF_POWER_INDEX_P7p02dBm,
	 RF_POWER_INDEX_P6p60dBm,
	 RF_POWER_INDEX_P6p14dBm,
	 RF_POWER_INDEX_P5p65dBm,
	 RF_POWER_INDEX_P5p13dBm,
	 RF_POWER_INDEX_P4p57dBm,
	 RF_POWER_INDEX_P3p94dBm,
	 RF_POWER_INDEX_P3p23dBm,
	 /*VANT*/
	 RF_POWER_INDEX_P3p01dBm,
	 RF_POWER_INDEX_P2p81dBm,
	 RF_POWER_INDEX_P2p61dBm,
	 RF_POWER_INDEX_P2p39dBm,
	 RF_POWER_INDEX_P1p99dBm,
	 RF_POWER_INDEX_P1p73dBm,
	 RF_POWER_INDEX_P1p45dBm,
	 RF_POWER_INDEX_P1p17dBm,
	 RF_POWER_INDEX_P0p90dBm,
	 RF_POWER_INDEX_P0p58dBm,
	 RF_POWER_INDEX_P0p04dBm,
	 RF_POWER_INDEX_N0p14dBm,
	 RF_POWER_INDEX_N0p97dBm,
	 RF_POWER_INDEX_N1p42dBm,
	 RF_POWER_INDEX_N1p89dBm,
	 RF_POWER_INDEX_N2p48dBm,
	 RF_POWER_INDEX_N3p03dBm,
	 RF_POWER_INDEX_N3p61dBm,
	 RF_POWER_INDEX_N4p26dBm,
	 RF_POWER_INDEX_N5p03dBm,
	 RF_POWER_INDEX_N5p81dBm,
	 RF_POWER_INDEX_N6p67dBm,
	 RF_POWER_INDEX_N7p65dBm,
	 RF_POWER_INDEX_N8p65dBm,
	 RF_POWER_INDEX_N9p89dBm,
	 RF_POWER_INDEX_N11p4dBm,
	 RF_POWER_INDEX_N13p29dBm,
	 RF_POWER_INDEX_N15p88dBm,
	 RF_POWER_INDEX_N19p27dBm,
	 RF_POWER_INDEX_N25p18dBm,
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

/**
 *  @brief  set the modulation index.
 */
typedef enum {
	RF_MI_P0p32 = 32,		 	/**< MI = 0.32 */
	RF_MI_P0p50 = 50,		  	/**< MI = 0.5 */
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
 * @note	   1.Call this function after turning on rx 30us, and the calibration value set by the function
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
*	@brief     This function serves to RF mode initialization
*			   SDK  may remove some features of the rf_drv_init,and thus not supporting multiple modes
*			   rf_multi_mode_drv_init function does not allow modification and is used to support multiple modes
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
*/
extern void rf_multi_mode_drv_init(RF_ModeTypeDef rf_mode);
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
*	@param[in]	len - Optional range: 3~5
*				Note: The effect for 3-byte access_code is not good.
*	@return	 	access_byte_num[2:0]
*/
static inline unsigned char rf_acc_len_get(unsigned char len)
{
    return (read_reg8(0x405) & 0x07);
}
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
extern void rf_set_channel (signed char chn, unsigned short set);


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

static inline void rf_set_tx_rx_off(void)
{
	write_reg8 (0x800f16, 0x29);
	write_reg8 (0x800428, RF_TRX_MODE);	// rx disable
	write_reg8 (0x800f02, RF_TRX_OFF);	// reset tx/rx state machine
}
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
*	@brief		this function is to set Private mode payload len for RF.
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
 *              This function is common to all modes,both sides need to be consistent otherwise performance will suffer,
 *              default is 0.5 in drive,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100,there are only two values:0.32 and 0.50.
 * @return	 	none.
 */
void rf_set_rx_modulation_index(RF_MIVauleTypeDef mi_value);

/**
 * @brief	  	This function is used to  set the modulation index of the sender.
 *              This function is common to all modes,both sides need to be consistent otherwise performance will suffer,
 *              default is 0.5 in drive,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100,there are only two values:0.32 and 0.50.
 * @return	 	none.
 */
void rf_set_tx_modulation_index(RF_MIVauleTypeDef mi_value);

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
 * @brief		This function is mainly used to set the antenna switching mode. According to the protocol requirements,
 * 				kite support two different table lookup sequences.The setting here is just the order of the table lookup,
 * 				and the content in the table is the number of the antenna to be switched to.The switching sequence of the
 * 				antenna needs to be determined by the combination of the table look-up sequence and the antenna number in
 * 				the table.But the table content of B85 is not allowed to be modified, so in B85, only this function can be
 * 				used to determine the antenna switching order.
 * @param[in]	pattern - Enumeration of several different look-up table order modes
 * @return		none.
 */
void rf_aoa_aod_ant_pattern(rf_ant_pattern_e pattern);

/**
 * @brief		This function is mainly used to set the number of antennas enabled by the multi-antenna board in the AOA/AOD
 * 				function; the kite series chips currently support up to 8 antennas for switching. By default, it is set to 8
 * 				antennas.After configuring the RF-related settings, you can set the number of enabled antennas,and this setting
 * 				needs to be completed before sending and receiving packets.
 * @param[in]	ant_num 	- the number of antenna,the value range is 1 to 8.
 * @return		none.
 */
void rf_aoa_aod_set_ant_num(unsigned char ant_num);

/**
 * @brief		This function is mainly used to set the type of AOA/AODiq data. Kite only support 8 bit length iq data.
 * 				This configuration can be done before starting to receive
 * 				the package.
 * @param[in]	mode	- The length of each I or Q data.
 * @return		none.
 */
void rf_aoa_aod_iq_data_mode(rf_aoa_aod_iq_data_mode_e mode);


/**
 * @brief		This function enables the receiving functions of AOA/AOD in ordinary format packets or ADV format packets.
 * 				After configuring the RF function, if you want to receive a packet with AOA/AOD information, you can call
 * 				this function to make the chip enter the corresponding mode to receive the packet. The default state is a
 * 				normal package without AOA/AOD information.
 * @param[in]	mode - AOA/AOD broadcast package or normal package rx mode.
 * @return		none.
 */
static inline void rf_aoa_aod_set_rx_mode(rf_aoa_aod_rx_mode_e mode)
{
	write_reg8(0x438,((read_reg8(0x438) & 0xfc) | mode));
}

/**
 * @brief		This function enables the sending functions of AOA/AOD in ordinary format packets or ADV format packets.
 * 				After configuring the RF function, if you want to send  a packet with AOA/AOD information,you can call
 * 				this function to make the chip enter the corresponding mode to send the packet. The default state is a
 * 				normal package without AOA/AOD
 * 				information.
 * @param[in]	mode - AOA/AOD broadcast package or normal package tx mode.
 * @return		none.
 */
static inline void rf_aoa_aod_set_tx_mode(rf_aoa_aod_tx_mode_e mode)
{
	write_reg8(0x438,((read_reg8(0x438) & 0xf3) | mode));
}

/**
 * @brief		This function is used to set the position of the first sample after the reference.The default is in the
 * 				middle of the first sample_slot; The starting position of sampling can be fine-tuned through parameter
 * 				setting to select the best sampling point.
 * @param[in]	sample_point_offset - sample_point_offset:The parameter range is -45 to 210.If the parameter is negative,
 * 				the position of the sampling point moves forward. The absolute value of the parameter is multiplied by
 * 				0.125us.If the parameter is positive, the position of the sampling point moves backward. The parameter is
 * 			    multiplied by 0.125us.
 * @return		none.
 */
static inline void rf_aoa_aod_sample_point_adjust(char sample_point_offset)
{
	write_reg8(0x43b,0x2d + sample_point_offset);
}

/**
 * @brief		This function is mainly used to set the sampling interval time in the AOA/AOD function.
 * 				After configuring RF, you can call this function to configure sample interval time.
 * @param[in]	sample_time	- AOA or AOD sampling interval time.
 * @return		none.
 */
void rf_aoa_aod_sample_interval_time(rf_aoa_aod_sample_interval_time_e sample_time);

/**
 * @brief		This function is mainly used to initialize the parameters related to AOA/AOD antennas, including the
 * 				number of antennas, the pins for controlling the antennas,the look-up mode of antenna switching, and
 * 				the content of the antenna switching sequence table.
 * @param[in]	ant_num			- The number of antennas, the value ranges from 1 to 8.
 * @param[in]	ant_pin_config:	- Control antenna pin selection and configuration.The parameter setting needs to be
 * 								  set according to the number and position of the control antenna.
 * 								  For example, if you need to control four antennas(ant1,2,3,4), it is best to use
 * 								  Antsel0 and Antsel2.
 * @param[in]	pattern			- Enumeration of several different look-up table order modes.
 * @return		none.
 */
void rf_aoa_aod_ant_init(unsigned char ant_num,rf_ant_pin_sel_t * ant_pin_config,rf_ant_pattern_e pattern);

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

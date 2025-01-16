/********************************************************************************************************
 * @file    rf_drv.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/*
 * @brief	Due to the limited otp space, this macro definition is used to process the registers
 * 			of adjacent addresses with write_reg32 and write_reg16 to reduce the space occupation.
 */
#define		OTP_SPACE_OPTIMIZATION			1

#if(0)
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
}rf_rx_fast_settle_time_e;

typedef enum{
TX_SETTLE_TIME_101US	 	= 0, /**<  disable tx_ldo_trim function,reduce 8.5us of tx settle time. Do a normal calibration at the beginning.*/
TX_SETTLE_TIME_53US		= 1, /**<  disable tx_ldo_trim function and tx_hpmc,reduce 61.5us of tx settle time.After frequency hopping, a normal calibration must be done.*/
}rf_tx_fast_settle_time_e;
#endif

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
	 RF_POWER_P11p46dBm = 63,  /**< 11.5 dbm */
	 RF_POWER_P11p28dBm = 61,  /**< 11.3 dbm */
	 RF_POWER_P11p00dBm = 58,  /**< 11.0 dbm */
	 RF_POWER_P10p78dBm = 56,  /**< 10.8 dbm */
	 RF_POWER_P10p44dBm = 53,  /**< 10.4 dbm */
	 RF_POWER_P10p19dBm = 51,  /**< 10.2 dbm */
	 RF_POWER_P9p92dBm  = 49,  /**<  9.9 dbm */
	 RF_POWER_P9p60dBm  = 47,  /**<  9.6 dbm */
	 RF_POWER_P9p31dBm  = 45,  /**<  9.3 dbm */
	 RF_POWER_P8p99dBm  = 43,  /**<  9.0 dbm */
	 RF_POWER_P8p63dBm  = 41,  /**<  8.6 dbm */
	 RF_POWER_P8p28dBm  = 39,  /**<  8.3 dbm */
	 RF_POWER_P7p87dBm  = 37,  /**<  7.8 dbm */
	 RF_POWER_P7p43dBm  = 35,  /**<  7.4 dbm */
	 RF_POWER_P6p97dBm  = 33,  /**<  7.0 dbm */
	 RF_POWER_P6p45dBm  = 31,  /**<  6.4 dbm */
	 RF_POWER_P5p92dBm  = 29,  /**<  5.9 dbm */
	 RF_POWER_P5p36dBm  = 27,  /**<  5.5 dbm */
	 RF_POWER_P4p73dBm  = 25,  /**<  4.7 dbm */
	 RF_POWER_P4p08dBm  = 23,  /**<  4.1 dbm */
	 /*VANT*/
	 RF_POWER_P3p95dBm  = BIT(7) | 63,    /**<  4.0 dbm */
	 RF_POWER_P3p72dBm  = BIT(7) | 61,    /**<  3.7 dbm */
	 RF_POWER_P3p48dBm  = BIT(7) | 59,    /**<  3.5 dbm */
	 RF_POWER_P3p24dBm  = BIT(7) | 57,    /**<  3.2 dbm */
	 RF_POWER_P2p87dBm  = BIT(7) | 54,    /**<  2.9 dbm */
	 RF_POWER_P2p60dBm  = BIT(7) | 52,    /**<  2.6 dbm */
	 RF_POWER_P2p30dBm  = BIT(7) | 50,    /**<  2.3 dbm */
	 RF_POWER_P1p99dBm  = BIT(7) | 48,    /**<  2.0 dbm */
	 RF_POWER_P1p67dBm  = BIT(7) | 46,    /**<  1.7 dbm */
	 RF_POWER_P1p33dBm  = BIT(7) | 44,    /**<  1.3 dbm */
	 RF_POWER_P0p78dBm  = BIT(7) | 41,    /**<  0.8 dbm */
	 RF_POWER_P0p59dBm  = BIT(7) | 40,    /**<  0.6 dbm */
	 RF_POWER_N0p22dBm  = BIT(7) | 36,    /**< -0.2 dbm */
	 RF_POWER_N0p44dBm  = BIT(7) | 35,    /**< -0.4 dbm */
	 RF_POWER_N0p67dBm  = BIT(7) | 34,    /**< -0.7 dbm */
	 RF_POWER_N1p15dBm  = BIT(7) | 32,    /**< -1.2 dbm */
	 RF_POWER_N1p71dBm  = BIT(7) | 30,    /**< -1.7 dbm */
	 RF_POWER_N2p26dBm  = BIT(7) | 28,    /**< -2.3 dbm */
	 RF_POWER_N2p84dBm  = BIT(7) | 26,    /**< -2.8 dbm */
	 RF_POWER_N3p51dBm  = BIT(7) | 24,    /**< -3.5 dbm */
	 RF_POWER_N4p18dBm  = BIT(7) | 22,    /**< -4.2 dbm */
	 RF_POWER_N4p97dBm  = BIT(7) | 20,    /**< -5.0 dbm */
	 RF_POWER_N5p85dBm  = BIT(7) | 18,    /**< -5.9 dbm */
	 RF_POWER_N6p83dBm  = BIT(7) | 16,    /**< -6.8 dbm */
	 RF_POWER_N7p97dBm  = BIT(7) | 14,    /**< -8.0 dbm */
	 RF_POWER_N9p27dBm  = BIT(7) | 12,    /**< -9.3 dbm */
	 RF_POWER_N10p84dBm = BIT(7) | 10,    /**<-10.8 dbm */
	 RF_POWER_N12p76dBm = BIT(7) | 8,     /**<-12.8 dbm */
	 RF_POWER_N15p01dBm = BIT(7) | 6,     /**<-15.0 dbm */
	 RF_POWER_N18p40dBm = BIT(7) | 4,     /**<-18.4 dbm */
	 RF_POWER_N24p28dBm = BIT(7) | 2,     /**<-24.3 dbm */

	 RF_POWER_N30dBm    = 0xff,           /**<-30.0 dbm */
	 RF_POWER_N50dBm    = BIT(7) | 0,     /**<-50.0 dbm */

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
	 RF_VBAT_POWER_P2p45dBm  = 0x15, 		//   2.45 dbm
	 RF_VBAT_POWER_P0p95dBm  = 0x11, 		//   0.95 dbm
	 RF_VBAT_POWER_P0p0dBm   = 0x10, 		//   0.00 dbm
	 RF_VBAT_POWER_N1p35dBm  = 0x0d, 		//   -1.35 dbm
	 RF_VBAT_POWER_N2p75dBm  = 0x0b, 		//   -2.75 dbm
	 RF_VBAT_POWER_P4p75dBm  = 0x09, 		//   -4.75 dbm
} RF_PowerTypeDef;


extern const RF_PowerTypeDef rf_power_Level_list[60];

typedef enum {
	RF_MI_P0p00 = 0,        /**< MI = 0 */
	RF_MI_P0p076 = 76,	    /**< MI = 0.076 This gear is only available in private mode*/
	RF_MI_P0p32 = 320,		/**< MI = 0.32 */
	RF_MI_P0p50 = 500,		/**< MI = 0.5 */
	RF_MI_P0p60 = 600,		/**< MI = 0.6 */
	RF_MI_P0p70 = 700,		/**< MI = 0.7 */
	RF_MI_P0p80 = 800,		/**< MI = 0.8 */
	RF_MI_P0p90 = 900,		/**< MI = 0.9 */
	RF_MI_P1p20 = 1200,		/**< MI = 1.2 */
	RF_MI_P1p30 = 1300,		/**< MI = 1.3 */
	RF_MI_P1p40 = 1400,		/**< MI = 1.4 */
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
	#define		RF_BLE_PACKET_LENGTH_OK(p)				(((p[1]<<8)|p[0]) == p[5]+13)
	#define		RF_BLE_PACKET_CRC_OK(p)					((p[p[5]+13 + 3] & 0x01) == 0x0)  /*Change the DMA length index to payload length index.
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
 * @brief      This function serves to initiate information of RF.
 * @return	   none.
 */
void rf_mode_init(void);


/**
 * @brief     This function serves to set ble_1M  mode of RF.
 * @return	  none.
 */
void rf_set_ble_1M_mode(void);


/**
 * @brief     This function serves to  set ble_1M_NO_PN  mode of RF.
 * @return	  none.
 */
void rf_set_ble_1M_NO_PN_mode(void);


/**
 * @brief     This function serves to set ble_2M  mode of RF.
 * @return	  none.
 */
void rf_set_ble_2M_mode(void);


/**
 * @brief     This function serves to set ble_2M_NO_PN  mode of RF.
 * @return	  none.
 */
void rf_set_ble_2M_NO_PN_mode(void);


/**
 * @brief     This function serves to set ble_500K  mode of RF.
 * @return	  none.
 */
void rf_set_ble_500K_mode(void);


/**
 * @brief     This function serves to  set zigbee_125K  mode of RF.
 * @return	  none.
 */
void rf_set_ble_125K_mode(void);


/**
 * @brief     This function serves to set zigbee_250K  mode of RF.
 * @return	  none.
 * @note	  The zigbee of the B80 chip only supports tx mode.
 */
void rf_set_zigbee_250K_mode(void);


/**
 * @brief     This function serves to set pri_250K  mode of RF.
 * @return	  none.
 */
void rf_set_pri_250K_mode(void);


/**
 * @brief     This function serves to  set pri_500K  mode of RF.
 * @return	  none.
 */
void rf_set_pri_500K_mode(void);


/**
 * @brief     This function serves to set pri_1M  mode of RF.
 * @return	  none.
 */
void rf_set_pri_1M_mode(void);


/**
 * @brief     This function serves to set pri_2M  mode of RF.
 * @return	  none.
 */
void rf_set_pri_2M_mode(void);


/**
 * @brief     This function serves to set ant  mode of RF.
 * @return	   none.
 */
void rf_set_ant_mode(void);


/**
 * @brief   	This function serves to set RF power level index.
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */
extern void rf_set_power_level_index (RF_PowerTypeDef level);


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

#if(MCU_CORE_B80B)
/**
 * @brief		This function is to enable/disable each access_code channel for
 *				RF Rx terminal.
 * @param[in]	pipe  	- Bit0~bit5 correspond to channel 0~5, respectively.
 *					  	- #0:Disable.
 *					  	- #1:Enable.
 *						  If "enable" is set as 0x3f (i.e. 00111111),
 *						  all access_code channels (0~5) are enabled.
 * @return	 	none
 */
static inline void rf_rx_acc_code_enable(unsigned char pipe)
{
    write_reg8(0x407, (read_reg8(0x407)&0xc0) | pipe); //Rx_Channel_man[5:0]
}

/**
 * @brief		This function is to select access_code channel for RF tx terminal.
 * @param[in]	pipe  	- tx access code channel,The pipe range is from 0 to 5.
 *						  And only 1 channel can be selected every time.
 *						  If "pipe" is set as 0x05,the access_code channel (5) is enabled.
 * @return	 	none
 */
static inline void rf_tx_acc_code_select(unsigned char pipe)
{
    write_reg8(0xf15, ((read_reg8(0xf15)&0xf8) | pipe)|BIT(4)); //Tx_Channel_man[2:0]

}
#endif

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
 * @note    Before calling this function, you need to call is_rf_receiving_pkt
 * 			to judge whether the state machine is in the receiving state; otherwise,
 * 			it will cause abnormal sending and receiving of packets.
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
static inline void 	rf_set_tx_settle_time(unsigned short tx_stl_us)
{
	 tx_stl_us &= 0x0fff;
	 write_reg16(0xf04,(tx_stl_us - 1));
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
*				packet transmission is finished. After the check, it's needed to
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
*	@param[out]	RF_RxAddr  	Pointer for Rx buffer in RAM(Generally it's starting
*							address of an array.Should be 4-byte aligned)
*	@param[in]	size   		Rx buffer size (It's an integral multiple of 16)
*	@param[in]	PingpongEn 	Enable/Disable Ping-Pong buffer 1:Enable 0:Disable
*							Note:
*							When "PingpongEn" is set as 0, received RF data will
*							be stored in RAM pointed by " RF_RxAddr".
*							When "PingpongEn" is set as 1, received RF data will
*							be stored in buffer0 and buffer1 successively.
*							The RAM size reserved for received RF data should be
*							double of "Size".
*
*	@return	 	none
*/

extern void  rf_rx_buffer_set(unsigned char *  RF_RxAddr, int size, unsigned char  PingpongEn);

/**
*	@brief	  	This function serves to  reconfigure the buffer
*	@param[out]	RF_RxAddr  	Pointer for Rx buffer in RAM(Generally it's starting
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
*				packet transmission is finished. After the check, it's needed to
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
*				The address to store received data is set by the function "addr".
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
*				"timeout_us".The address to store received data is set by the
*				function "RF_RxBufferSet".
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
*				by the function "RF_RxBufferSet".
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
 * @note	Before calling the rf_set_tx_rx_off_auto_mode function to stop the state machine,
 * 			you need to call this function to judge whether the state machine is in the state
 * 			of receiving packets; if it is in the process of receiving packets, it will cause
 * 			the abnormality of receiving packets.
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
extern void rf_rffe_set_pin(GPIO_PinTypeDef tx_pin, GPIO_PinTypeDef rx_pin);

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
 * @brief      This function serves to init the 2-wire-PTA.
 * @param[in]  ble_priority_pin - the pin of ble_priority.
 * @param[in]  wlan_active_pin  - the pin of wlan_active.
 * @param[in]  ble_priority_mode- the mode of ble_priority pin.
 *             when the mode is PTA_BLE_PRIORITY_TX,the pin of ble_priority will be high if tx.
 *             when the mode is PTA_BLE_PRIORITY_RX,the pin of ble_priority will be high if rx.
 *             when the mode is PTA_BLE_PRIORITY_TRX,the pin of ble_priority will be high if tx and rx.
 * @return     none
 */
extern void  rf_2wire_pta_init(GPIO_PinTypeDef ble_priority_pin,GPIO_PinTypeDef wlan_active_pin,PTA_2WireModeDef ble_priority_mode);

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
extern void  rf_3wire_pta_init(GPIO_PinTypeDef ble_active_pin,GPIO_PinTypeDef ble_status_pin,GPIO_PinTypeDef wlan_deny_pin,PTA_3WireModeDef ble_status_mode);

void rx_dly_dis(void);



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
  * @brief	  	 This function is used to set the modulation index of the receiver.
  *              This function is common to all modes,the order of use requirement:configure mode first,then set the the modulation index,default is 0.5 in drive,
  *              both sides need to be consistent otherwise performance will suffer,if don't specifically request,don't need to call this function.
  * @param[in]	 mi_value- the value of modulation_index*100.
  * @return	 	 none.
  */
 void rf_set_rx_modulation_index(RF_MIVauleTypeDef mi_value);

 /**
  * @brief	  	 This function is used to set the modulation index of the sender.
  *              This function is common to all modes,the order of use requirement:configure mode first,then set the the modulation index,default is 0.5 in drive,
  *              both sides need to be consistent otherwise performance will suffer,if don't specifically request,don't need to call this function.
  * @param[in]	 mi_value- the value of modulation_index*100.
  * @return	 	 none.
  */
 void rf_set_tx_modulation_index(RF_MIVauleTypeDef mi_value);

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

 /**
  * @brief	This function to set rf rx sync threshold
  * @param[in] 	threshold	- rx sync threshold,This unit is in bits.
  * 						  The bit length of the access code matched during rx sync;
  * @return		none.
  */
 void rf_set_rx_sync_threshold(unsigned char threshold);

 /**
  * @brief   This function serves to set RF wait time between RX to TX switching.
  * @param[in] tx_wait_us  Wait time between RX to TX switching,the unit is us.
  * 			  The max value of this param is 0xfff;
  * @return  none.
  * @note	Attention:It is not necessary to call this function to adjust the waiting time in the normal sending state.
  */
 static inline void rf_set_tx_wait_time(unsigned short tx_wait_us)
 {
 	tx_wait_us &= 0x0fff;
 	write_reg16(0xf0e, (read_reg8(0xf0e) & 0xf000) | tx_wait_us);
 }

 /**
  * @brief   This function serves to set RF wait time between RX to TX switching.
  * @param[in]  rx_wait_us  Wait time between TX to RX switching,the unit is us.
  * 			   The max value of this param is 0xfff;
  * @return  none.
  * @note	Attention:It is not necessary to call this function to adjust the waiting time in the normal packet receiving state.
  */
 static inline void rf_set_rx_wait_time( unsigned short rx_wait_us)
 {
 	 rx_wait_us &= 0x0fff;
 	 write_reg16(0xf06, (read_reg8(0xf06)& 0xf000) | rx_wait_us);
 }

#if(0)
 /**
   *	@brief	  	this function serve to adjust rx settle timing sequence.
   *	@param[in]	rx_settle_us  	After adjusting the timing sequence, the time required for rx to settle.
   *	@return	 	none
   *	@note		RX_SETTLE_TIME_77US :Do a normal calibration at the beginning.
   *				RX_SETTLE_TIME_44US :Receive for a period of time and then do a normal calibration.
   *
  */
#if(MCU_CORE_B80B)
 /**
   *				Attention:
   *				The fast settle initialization configuration of the B80B chip has been set to RX_SETTLE_TIME_44US mode by default,
   *				When using fast settle and the rx settle time is set to RX_SETTLE_TIME_44US, there is no need to call this interface.
  */
#endif
  void rf_rx_fast_settle_init(rf_rx_fast_settle_time_e rx_settle_us);

  /**
   *	@brief	  	this function serve to adjust tx settle timing sequence.
   *	@param[in]	tx_settle_us  	After adjusting the timing sequence, the time required for tx to settle.
   *	@return	 	none
   *	@note		TX_SETTLE_TIME_101US : After frequency hopping, a normal calibration must be done.
   *				TX_SETTLE_TIME_53US  : Do a normal calibration at the beginning.
  */
#if(MCU_CORE_B80B)
  /**
   *				Attention:
   *				The fast settle initialization configuration of the B80B chip has been set to TX_SETTLE_TIME_53US mode by default,
   *				When using fast settle and the tx settle time is set to TX_SETTLE_TIME_53US, there is no need to call this interface.
  */
#endif
  void rf_tx_fast_settle_init(rf_tx_fast_settle_time_e tx_settle_us);

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
#endif


 /**
  * @brief	This function serve to set the ptx pid init value.
  * @param[in]	tx_pid -Initial value of pid for PTX. (PID range:0~3)
  * @return	none
  */
 static inline void rf_set_ptx_init_pid(unsigned char tx_pid)
 {
 	reg_rf_ll_ctrl_1 |= (tx_pid << 6);
 }

 /**
  * @brief	This function serve to set the prx pid init value.
  * @param[in] rx_pid -Initial value of pid for PRX. (PID range:0~3)
  * @return	none
  */
 static inline void rf_set_prx_init_pid(unsigned char rx_pid)
 {
 	reg_rf_ll_ctrl_1 |= (rx_pid << 4);
 }

 /**
 * @brief	This function serve to enable the ack in ptx and prx modes.
 * @param[in]	none
 * @return		none
 */
 void rf_set_ptx_prx_ack_en(void);

 /**
 * @brief	This function serve to disable the ack in ptx and prx modes.
 * @param[in]	none
 * @return		none
 */
 void rf_set_ptx_prx_ack_dis(void);

 /**
 * @brief	This function serve to initial the ptx/prx setting.
 * @param   none.
 * @return	none.
 */
 void rf_ptx_prx_config(void);

 /**
 * @brief   This function serves to set RF ptx trigger.
 * @param[in]	addr	-	The address of tx_packet.
 * @param[in]	tick	-	Trigger ptx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 */
 void rf_start_ptx  (unsigned char* addr ,unsigned int tick);

 /**
 * @brief   This function serves to set RF prx trigger.
 * @param[in]	tick	-	Trigger prx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 */
 void rf_start_prx(unsigned char* addr ,unsigned int tick);

 /**
 * @brief	This function serves to set retransmit and retransmit delay.
 * @param[in] 	retry_count	- Number of retransmit, 0: retransmit OFF
 * @param[in] 	retry_delay	- Retransmit delay time.
 * @return		none.
 */
 void rf_set_ptx_retry(unsigned char retry_count, unsigned short retry_delay);



#if(MCU_CORE_B80B)
/**
* @brief	This function serves to update the wptr of tx terminal.
* @param[in]	pipe_id	-	The number of pipe which need to update wptr.
* @param[in]	wptr	-	The pointer of write in tx terminal.
* @return		none
*/
static inline void rf_set_tx_wptr(unsigned char pipe_id,unsigned char wptr)
{
  reg_rf_multi_dma_tx_wptr(pipe_id) = wptr;
}

/**
* @brief	This function serves to get the write pointer of tx terminal.
* @param[in]	pipe_id	-	The number of tx pipe.0<= pipe_id <=5.
* @return		The write pointer of the tx.
*/
static inline unsigned char rf_get_tx_wptr(unsigned char pipe_id)
{
 return reg_rf_multi_dma_tx_wptr(pipe_id);
}

/**
* @brief	This function serves to get the read pointer of tx terminal.
* @param[in]	pipe_id	-	The number of tx pipe.0<= pipe_id <=5.
* @return	none.
*/
static inline unsigned char rf_get_tx_rptr(unsigned char pipe_id)
{
 return reg_rf_multi_dma_tx_rptr(pipe_id);
}

/**
  * @brief	This function serves to clear the writer pointer of tx terminal.
  * @param[in]	pipe_id	-	The number of tx pipe.0<= pipe_id <=5.
  * @return	none.
  * @note   Pipe0 corresponds to write pointer dma address 0x30, pipe1 corresponds to 0x31, and so on.
  */
 static inline void rf_clr_tx_wptr(unsigned char pipe_id)
 {
	 reg_rf_multi_dma_tx_wptr(pipe_id) = 0;
 }

 /**
  * @brief	This function serves to clear read pointer of tx terminal.
  * @param[in]	pipe_id	-	The number of tx pipe.0<= pipe_id <=5.
  * @return	none.
  * @note   Pipe0 corresponds to read pointer dma address 0x38, pipe1 corresponds to 0x39, and so on.
  */
 static inline void rf_clr_tx_rptr(unsigned char pipe_id)
 {
	 reg_rf_multi_dma_tx_rptr(pipe_id) = 0x40;
 }

/**
* @brief	This function serves to get the pid of different pipes.
* @param[in] 	pipe_id	- The number of pipe.0<= pipe_id <=5.
* @return		The pid of different pipe.
*/
static inline unsigned char rf_get_pipe_pid(unsigned char pipe_id)
{
	return((unsigned char)(read_reg16(0xf22)>>(pipe_id*2))&0x03);
}


#endif

#endif

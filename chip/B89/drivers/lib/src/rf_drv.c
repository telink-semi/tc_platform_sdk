/********************************************************************************************************
 * @file	rf_drv.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "register.h"
#include "analog.h"
#include "clock.h"
#include "lib/include/pm.h"
#include "lib/include/rf_drv.h"
#include "bsp.h"
#include "compiler.h"
#define		RF_MANUAL_AGC_MAX_GAIN			1
#define     REG_RF_ACCESS_CODE_GTH          REG_ADDR8(0x420)
#define 	ACCESS_CODE_BASE_PIPE0    		(0x800408)
#define 	ACCESS_CODE_BASE_PIPE2    		(0x800418)

extern _attribute_data_retention_  unsigned char 		pm_1p2_power_sel_vbat;
static RF_StatusTypeDef RF_TRxState = RF_MODE_TX;


_attribute_data_retention_ RF_ModeTypeDef   g_RFMode;

unsigned char g_RFRxPingpongEn;

#define FRE_OFFSET		0
#define MAX_RF_CHANNEL  16

/*
 * @brief	Energy array under 1.5V voltage in Vbat power supply mode.
 * @note	Attention:If the voltage is less than 1.5V, the chip emission energy will follow,
 * 			and the energy value in the table will no longer be accurate.
 */
const rf_power_level_e rf_power_Level_list[60] =
{
	 //VBAT power supply
	 RF_POWER_VBAT_P4p98dBm ,
	 RF_POWER_VBAT_P4p03dBm ,
	 RF_POWER_VBAT_P3p08dBm ,
	 RF_POWER_VBAT_P2p05dBm ,
	 RF_POWER_VBAT_P1p12dBm ,
	 RF_POWER_VBAT_P0p65dBm ,
	 RF_POWER_VBAT_N0p12dBm ,
	 RF_POWER_VBAT_N0p72dBm ,
	 RF_POWER_VBAT_N1p75dBm ,
	 RF_POWER_VBAT_N2p51dBm ,
	 RF_POWER_VBAT_N3p39dBm ,
	 RF_POWER_VBAT_N4p33dBm ,
	 RF_POWER_VBAT_N5p46dBm ,
	 RF_POWER_VBAT_N6p78dBm ,
	 RF_POWER_VBAT_N9p35dBm ,
	 RF_POWER_VBAT_N11p77dBm  ,
	 RF_POWER_VBAT_N15p16dBm  ,
	 RF_POWER_VBAT_N20p71dBm  ,
	 RF_POWER_VBAT_N26p45dBm  ,
	 //DCDC power supply
	 RF_POWER_DCDC_P4p98dBm ,
	 RF_POWER_DCDC_P4p03dBm ,
	 RF_POWER_DCDC_P3p08dBm ,
	 RF_POWER_DCDC_P2p05dBm ,
	 RF_POWER_DCDC_P1p12dBm ,
	 RF_POWER_DCDC_P0p65dBm ,
	 RF_POWER_DCDC_N0p12dBm ,
	 RF_POWER_DCDC_N0p72dBm ,
	 RF_POWER_DCDC_N1p75dBm ,
	 RF_POWER_DCDC_N2p51dBm ,
	 RF_POWER_DCDC_N3p39dBm ,
	 RF_POWER_DCDC_N4p33dBm ,
	 RF_POWER_DCDC_N5p46dBm ,
	 RF_POWER_DCDC_N6p78dBm ,
	 RF_POWER_DCDC_N9p35dBm ,
	 RF_POWER_DCDC_N11p77dBm  ,
	 RF_POWER_DCDC_N15p16dBm  ,
	 RF_POWER_DCDC_N20p71dBm  ,
	 RF_POWER_DCDC_N26p45dBm
};

const unsigned char rf_chn_table[MAX_RF_CHANNEL] = {
	FRE_OFFSET+ 5, FRE_OFFSET+ 9, FRE_OFFSET+13, FRE_OFFSET+17,
	FRE_OFFSET+22, FRE_OFFSET+26, FRE_OFFSET+30, FRE_OFFSET+35,
	FRE_OFFSET+40, FRE_OFFSET+45, FRE_OFFSET+50, FRE_OFFSET+55,
	FRE_OFFSET+60, FRE_OFFSET+65, FRE_OFFSET+70, FRE_OFFSET+76,
};

/**
 * @brief      This function serves to initiate information of RF.
 * @return	   none.
 */
void rf_mode_init(void)
{
	write_reg8(0x12d2,0x9b);//DCOC_DBG0
	write_reg8(0x12d3,0x19);//DCOC_DBG1
	write_reg8(0x127b,0x0e);//fm2pm
	write_reg8(0x1279,0x38);//GEN DEC//set 1279 bit<3>:1 to deal with the ble_floor unstable (per not around 0 when power big enough sometime)
							//changed by zhiwei,confirmed by junwen 2019/10/30
	//To set AGC thresholds
	write_reg8(0x124a,0x0e);//POW_001_010_L
	write_reg8(0x124b,0x09);//POW_001_010_H
	write_reg8(0x124e,0x09);//POW_101_100_L
	write_reg8(0x124f,0x0f);//POW_101_100_H



	write_reg8(0x1254, 0x0e); //POW_001_010_L
	write_reg8(0x1255, 0x09); //POW_001_010_H

	write_reg8(0x1256, 0x0c); //POW_011_100_L
	write_reg8(0x1257, 0x08); //POW_011_100_H

	write_reg8(0x1258, 0x09); //POW_101_100_L
	write_reg8(0x1259, 0x0f); //POW_101_100_H

	write_reg8(0x1276, 0x50); //FREQ_CORR_CFG2_0
	write_reg8(0x1277, 0x73); //FREQ_CORR_CFG2_1

	//For optimum Rx chain
	write_reg8(0x134e, 0x45); //CBPF_TRIM_I && CBPF_TRIM_Q
	write_reg8(0x134c, 0x02); //LNA_ITRIM=0x01(default)(change to 0x02[TBD])
	write_reg8(0x430,0x3e);	  /*Set bit<1> to 1 to disable the debug information in hd_info in all modes, and enable the timestamp
									information in hd_info.Modified by zhiwei,confirmed by qiangkai.20211117*/
	reg_dma_chn_en |= FLD_DMA_CHN_RF_RX  | FLD_DMA_CHN_RF_TX;
}

/**
 * @brief     This function serves to set zigbee_250K  mode of RF.
 * @return	  none.
 */
void rf_set_zigbee_250K_mode(void)
{
	write_reg8(0x1220, 0x04);
	write_reg8(0x1221, 0x2b);
	write_reg8(0x1222, 0x43);
	write_reg8(0x1223, 0x86);
	write_reg8(0x122a, 0x90);
	write_reg8(0x122c, 0x39);
	write_reg8(0x123d, 0x01);

	write_reg8(0x1254, 0x0e); //AGC_THRSHLD1_2M_0
	write_reg8(0x1255, 0x09); //AGC_THRSHLD1_2M_1
	write_reg8(0x1256, 0x0c); //AGC_THRSHLD2_2M_0
	write_reg8(0x1257, 0x08); //AGC_THRSHLD2_2M_1
	write_reg8(0x1258, 0x09); //AGC_THRSHLD3_2M_0
	write_reg8(0x1259, 0x0f); //AGC_THRSHLD3_2M_1

	write_reg8(0x400, 0x13);//{0x400, 0x0a,	TCMD_UNDER_BOTH | TCMD_WRITE},
	write_reg8(0x420, 0x18);
	write_reg8(0x402, 0x46); //pre-amble
	write_reg8(0x404, 0xc0);
	write_reg8(0x405, 0x04);
	write_reg8(0x421, 0x23);
	write_reg8(0x422, 0x04);
	write_reg8(0x408, 0xa7);
	write_reg8(0x409, 0x00);
	write_reg8(0x40a, 0x00);
	write_reg8(0x40b, 0x00);
	write_reg8(0x407, 0x0f);
	write_reg8(0x403, 0x44);

	//AGC table 2M
	write_reg8(0x460, 0x36);
	write_reg8(0x461, 0x46);
	write_reg8(0x462, 0x51);
	write_reg8(0x463, 0x61);
	write_reg8(0x464, 0x6d);
	write_reg8(0x465, 0x78);

	g_RFMode = RF_MODE_ZIGBEE_250K;
}

/**
 * @brief     This function serves to set ble_1M  mode of RF.
 * @return	  none.
 */
void rf_set_ble_1M_mode(void)
{
	//these setting below is for AURA RF module BLE 1M mode
	write_reg8(0x1220, 0x16);
	write_reg8(0x1221, 0x0a);
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);

		//8278 script set 0x1f, but 8255 ble  SDK set 0x0f
		//BIT<4>:tx output data mode, 0: xor 32'hcccccccc; 1: xor 32'h66666666
	write_reg8(0x400, 0x1f); //poweron_dft: 0x1f    	//<4>default,8255 use 0 with error

		//core_402, [7:5] trailer_len,  [4:0] new 2m mode preamble length
		//8255 is 0x26, 8278 script is 0x43
	write_reg8(0x402, 0x46);//<4:0>preamble length   	//<7:4>default
	#if 1
	write_reg8(0x401, 0x08);	//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	write_reg8(0x404, 0xf5);	//BLE header and 1Mbps  BIT<5>: ble_wt(PN) enable
	#else
	write_reg8(0x401, 0x00);	//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	write_reg8(0x404, 0xd5);	//BLE header and 1Mbps  BIT<5>: ble_wt(PN) enable
	#endif
	write_reg8(0x405, 0x04);
	write_reg8(0x420, 0x1e); //sync threshold bit match
	write_reg8(0x421, 0xa1); //len_0_en

	    //AGC table 1M
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_BLE_1M;
}

/**
 * @brief     This function serves to set ble_2M_NO_PN  mode of RF.
 * @return	  none.
 */
void rf_set_ble_2M_NO_PN_mode(void)
{
	write_reg8(0x1220, 0x04);
	write_reg8(0x1221, 0x2a);
	write_reg8(0x1222, 0x43);
	write_reg8(0x1223, 0x06);
	write_reg8(0x1254, 0x0e); //AGC_THRSHLD1_2M_0
	write_reg8(0x1255, 0x09); //AGC_THRSHLD1_2M_1
	write_reg8(0x1256, 0x0c); //AGC_THRSHLD2_2M_0
	write_reg8(0x1257, 0x08); //AGC_THRSHLD2_2M_1
	write_reg8(0x1258, 0x09); //AGC_THRSHLD3_2M_0
	write_reg8(0x1259, 0x0f); //AGC_THRSHLD3_2M_1

	write_reg8(0x400, 0x1f);
	write_reg8(0x401, 0x00);	// PN disable
	write_reg8(0x402, 0x46);//preamble length<4:0>
	write_reg8(0x404, 0xc5);//BLE header and 2Mbps  BIT<5>: ble_wt(PN) disable
	write_reg8(0x405, 0x04);
	write_reg8(0x420, 0x1e);//access code match threshold
	write_reg8(0x421, 0xa1);
	//AGC table
	write_reg8(0x460, 0x36);//grx_0
	write_reg8(0x461, 0x46);//grx_1
	write_reg8(0x462, 0x51);//grx_2
	write_reg8(0x463, 0x61);//grx_3
	write_reg8(0x464, 0x6d);//grx_4
	write_reg8(0x465, 0x78);//grx_5


}


/**
 * @brief     This function serves to  set ble_1M_NO_PN  mode of RF.
 * @return	  none.
 */
void rf_set_ble_1M_NO_PN_mode(void)
{
	//these setting below is for AURA RF module BLE 1M mode
	write_reg8(0x1220, 0x16);
	write_reg8(0x1221, 0x0a);
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);

		//8278 script set 0x1f, but 8255 ble  SDK set 0x0f
		//BIT<4>:tx output data mode, 0: xor 32'hcccccccc; 1: xor 32'h66666666
	write_reg8(0x400, 0x1f); //poweron_dft: 0x1f    	//<4>default,8255 use 0 with error

		//core_402, [7:5] trailer_len,  [4:0] new 2m mode preamble length
		//8255 is 0x26, 8278 script is 0x43
	write_reg8(0x402, 0x46);//<4:0>preamble length   	//<7:4>default

	write_reg8(0x401, 0x00);	// PN enable
	write_reg8(0x404, 0xd5);	//BLE header and 1Mbps  BIT<5>: ble_wt(PN) enable

	write_reg8(0x405, 0x04);

	write_reg8(0x420, 0x1e); //sync threshold bit match
	write_reg8(0x421, 0xa1); //len_0_en

	    //AGC table
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_BLE_1M_NO_PN;
}

/**
 * @brief     This function serves to set ble_2M  mode of RF.
 * @return	  none.
 */
void rf_set_ble_2M_mode(void)
{
	write_reg8(0x1220, 0x04);
	write_reg8(0x1221, 0x2a);
	write_reg8(0x1222, 0x43);
	write_reg8(0x1223, 0x06);
	write_reg8(0x1254, 0x0e); //AGC_THRSHLD1_2M_0
	write_reg8(0x1255, 0x09); //AGC_THRSHLD1_2M_1
	write_reg8(0x1256, 0x0c); //AGC_THRSHLD2_2M_0
	write_reg8(0x1257, 0x08); //AGC_THRSHLD2_2M_1
	write_reg8(0x1258, 0x09); //AGC_THRSHLD3_2M_0
	write_reg8(0x1259, 0x0f); //AGC_THRSHLD3_2M_1

	write_reg8(0x400, 0x1f);
	write_reg8(0x402, 0x46); //preamble length<4:0>
	#if 1
	write_reg8(0x404, 0xe5);//BLE header and 2Mbps  BIT<5>: ble_wt(PN) disable
	write_reg8(0x401, 0x08);//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	#else
	write_reg8(0x404, 0xc5);//BLE header and 2Mbps  BIT<5>: ble_wt(PN) disable
	write_reg8(0x401, 0x00);//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	#endif
	write_reg8(0x405, 0x04);
	write_reg8(0x420, 0x1e);//access code match threshold
	write_reg8(0x421, 0xa1);


	    //AGC table
	write_reg8(0x460, 0x36);//grx_0
	write_reg8(0x461, 0x46);//grx_1
	write_reg8(0x462, 0x51);//grx_2
	write_reg8(0x463, 0x61);//grx_3
	write_reg8(0x464, 0x6d);//grx_4
	write_reg8(0x465, 0x78);//grx_5

	g_RFMode = RF_MODE_BLE_2M;
}

/**
 * @brief     This function serves to set ble_500K  mode of RF.
 * @return	  none.
 */
void rf_set_ble_500K_mode(void)
{
	write_reg8(0x1220, 0x17);
	write_reg8(0x1221, 0x0a);
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);

	write_reg8(0x1273, 0x21);
	write_reg8(0x1237, 0x8c);
	write_reg8(0x1236, 0xee);
	write_reg8(0x1238, 0xc8);
	write_reg8(0x1239, 0x7d);//  poweron_dft: 0x71  // //add to resolve frequency offset(add by zhaowei 2019-7-25)

	write_reg8(0x400, 0x1f);

	write_reg8(0x402, 0x4a);//preamble length<4:0>
#if 1
	write_reg8(0x401, 0x08);	//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	write_reg8(0x404, 0xf5);    //BLE header and 1Mbps  BIT<5>: ble_wt(PN) disable
#else
	write_reg8(0x401, 0x00);	//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	write_reg8(0x404, 0xd5);	//BLE header and 1Mbps  BIT<5>: ble_wt(PN) disable
#endif
	write_reg8(0x420, 0xf0);//access code match threshold
	write_reg8(0x421, 0xa1);
	write_reg8(0x405, 0x24);  /*bit<5> 1:ble lr 0:other mode.bit<4> 0:S2 1:S8.bit<7>:1 S2 S8 mode access code trigger
							    The trigger operation of the default access code (usually not used) is removed here,
							    because the access code will be reset and the trigger will be performed later.If the
							    time interval between two triggers is less than 5us, an error will occur. Modify by
							    zhiwei,confirmed by xuqiang 20211117*/

	//AGC table 1M
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76); //grx_5

	g_RFMode = RF_MODE_LR_S2_500K;
}

/**
 * @brief     This function serves to  set zigbee_125K  mode of RF.
 * @return	  none.
 */
void rf_set_ble_125K_mode(void)
{
	write_reg8(0x1220, 0x17);
	write_reg8(0x1221, 0x0a);
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);

	write_reg8(0x1273, 0x21);
	write_reg8(0x1237, 0x8c);
	write_reg8(0x1236, 0xf6);
	write_reg8(0x1238, 0xc8);

	write_reg8(0x1239, 0x7d); //added by junwen kangpingpian


	write_reg8(0x400, 0x1f);
	write_reg8(0x402, 0x4a);//preamble length<4:0>

	#if 1
	write_reg8(0x401, 0x08);	//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	write_reg8(0x404, 0xf5);	//BLE header and 1Mbps  BIT<5>: ble_wt(PN) disable
	#else
	write_reg8(0x401, 0x00);	//<3:3> 1:PN enable 0:Disable changed by zhiwei 2019/10/15 confirmed by junwen
	write_reg8(0x404, 0xd5);//BLE header and 1Mbps  BIT<5>: ble_wt(PN) disable
	#endif


	write_reg8(0x420, 0xf0);//access code match threshold
	write_reg8(0x421, 0xa1);
	write_reg8(0x405, 0x34); /*bit<5> 1:ble lr 0:other mode.bit<4> 0:S2 1:S8.bit<7>:1 S2 S8 mode access code trigger
							   The trigger operation of the default access code (usually not used) is removed here,
							   because the access code will be reset and the trigger will be performed later.If the
							   time interval between two triggers is less than 5us, an error will occur. Modify by
							   zhiwei,confirmed by xuqiang 20211117*/

	//AGC table 1M
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_LR_S8_125K;
}

/**
 * @brief     This function serves to set pri_250K  mode of RF.
 * @return	  none.
 */
void rf_set_pri_250K_mode(void)
{
	write_reg8(0x1220, 0x16); //BW_CODE & SC_CODE
	write_reg8(0x1221, 0x0a); //IF_FREQ
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);
	write_reg8(0x123f, 0x02); //MODE_CFG_RX2_1
	write_reg8(0x122b, 0x0a); //DEBUG_TXRX1_1


	write_reg8(0x403, 0x45); //BBDBG  0x45
	write_reg8(0x400, 0x1f); //TXMODE1
	write_reg8(0x402, 0x44); //preamble and trailer length
	write_reg8(0x404, 0xda); //FORMAT
	write_reg8(0x420, 0x1e); ////sync threshold
	write_reg8(0x421, 0xa1); //RXGTH1
	write_reg8(0x405, 0x04); //access code length

	//AGC table 1M
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_PRIVATE_250K;
}

/**
 * @brief     This function serves to  set pri_500K  mode of RF.
 * @return	  none.
 */
void rf_set_pri_500K_mode(void)
{
	write_reg8(0x1220, 0x16);
	write_reg8(0x1221, 0x0a);
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);
	write_reg8(0x123f, 0x06);
	write_reg8(0x122b, 0x06);


	write_reg8(0x403, 0x47);
	write_reg8(0x400, 0x1f);
	write_reg8(0x402, 0x44);
	write_reg8(0x404, 0xda);
	write_reg8(0x420, 0x1e); ////sync threshold
	write_reg8(0x421, 0xa1);
	write_reg8(0x405, 0x04); //access code length

	//AGC table 1M
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_PRIVATE_500K;
}

/**
 * @brief     This function serves to set pri_1M  mode of RF.
 * @return	  none.
 */
void rf_set_pri_1M_mode(void)
{
	write_reg8(0x1220, 0x16);//SC_CODE
	write_reg8(0x1221, 0x0a);//IF_FREQ	s
	write_reg8(0x1222, 0x20);//HPMC_EXP_DIFF_COUNT_L
	write_reg8(0x1223, 0x23);//HPMC_EXP_DIFF_COUNT_H

	write_reg8(0x400, 0x1f);//tx mode
	write_reg8(0x402, 0x44);//preamble length
	write_reg8(0x404, 0xfa);//pri head
	write_reg8(0x421, 0xa1);//len_0_en
	write_reg8(0x405, 0x04);//access code length

	//AGC table
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_PRIVATE_1M;
}

/**
 * @brief     This function serves to set pri_2M  mode of RF.
 * @return	  none.
 */
void rf_set_pri_2M_mode(void)
{
	write_reg8(0x1220, 0x04);//SC_CODE
	write_reg8(0x1221, 0x2a);//IF_FREQ
	write_reg8(0x1222, 0x43);//HPMC_EXP_DIFF_COUNT_L
	write_reg8(0x1223, 0x06);//HPMC_EXP_DIFF_COUNT_H
	write_reg8(0x1254, 0x0e); //AGC_THRSHLD1_2M_0
	write_reg8(0x1255, 0x09); //AGC_THRSHLD1_2M_1
	write_reg8(0x1256, 0x0c); //AGC_THRSHLD2_2M_0
	write_reg8(0x1257, 0x08); //AGC_THRSHLD2_2M_1
	write_reg8(0x1258, 0x09); //AGC_THRSHLD3_2M_0
	write_reg8(0x1259, 0x0f); //AGC_THRSHLD3_2M_1

	write_reg8(0x400, 0x1f);//tx mode
	write_reg8(0x402, 0x44);//preamble length
	write_reg8(0x404, 0xea);//pri head
	write_reg8(0x421, 0xa1);//len_0_en
	write_reg8(0x405, 0x04);//access code length

	//AGC table
	write_reg8(0x460, 0x36);//grx_0
	write_reg8(0x461, 0x46);//grx_1
	write_reg8(0x462, 0x51);//grx_2
	write_reg8(0x463, 0x61);//grx_3
	write_reg8(0x464, 0x6d);//grx_4
	write_reg8(0x465, 0x78);//grx_5

   //Add the following settings compared with tbl_rf_zigbee_250k and these settings are default values.
	write_reg8(0x122a, 0x90);
	write_reg8(0x122c, 0x38);
	write_reg8(0x123d, 0x00);
	write_reg8(0x420,  0x1e);
	write_reg8(0x405,  0x04);
	write_reg8(0x422,  0x04);
	write_reg8(0x407,  0x01);
	write_reg8(0x403,  0x44);

	g_RFMode = RF_MODE_PRIVATE_2M;
}

/**
 * @brief     This function serves to set ant  mode of RF.
 * @return	   none.
 */
void rf_set_ant_mode(void)
{
	write_reg8(0x1220, 0x16);
	write_reg8(0x1221, 0x0a);
	write_reg8(0x1222, 0x20);
	write_reg8(0x1223, 0x23);



	write_reg8(0x400, 0x1f);
	write_reg8(0x402, 0x43);//<4:0>preamble length
	write_reg8(0x404, 0xdb);//BLE header and 1Mbps
	write_reg8(0x405, 0x04);//access length
	write_reg8(0x420, 0x20);//sync threshold
	write_reg8(0x421, 0xa1);//len_0_en

	//AGC table 1M
	write_reg8(0x460, 0x34);//grx_0
	write_reg8(0x461, 0x44);//grx_1
	write_reg8(0x462, 0x4f);//grx_2
	write_reg8(0x463, 0x5f);//grx_3
	write_reg8(0x464, 0x6b);//grx_4
	write_reg8(0x465, 0x76);//grx_5

	g_RFMode = RF_MODE_ANT;
}



/**
 * @brief   	This function serves to set RF's channel.
 * @param[in]   chn - RF channel.
 * @param[in]   set - the value to set.
 * @return  	none.
 */

void rf_set_channel (signed char chn, unsigned short set)//general
{
	unsigned short rf_chn =0;
	unsigned char ctrim;
	unsigned short chnl_freq;

	if (set & RF_CHN_TABLE) {
		chn = rf_chn_table[chn];
	}

	rf_chn = chn+2400;

	if (rf_chn >= 2550)
	    ctrim = 0;
	else if (rf_chn >= 2520)
	    ctrim = 1;
	else if (rf_chn >= 2495)
	    ctrim = 2;
	else if (rf_chn >= 2465)
	    ctrim = 3;
	else if (rf_chn >= 2435)
		ctrim = 4;
	else if (rf_chn >= 2405)
	    ctrim = 5;
	else if (rf_chn >= 2380)
	    ctrim = 6;
	else
	    ctrim = 7;

	chnl_freq = rf_chn * 2;

	write_reg8(0x1244, ((chnl_freq & 0x7f)<<1) | 1  );   //CHNL_FREQ_DIRECT   CHNL_FREQ_L
	write_reg8(0x1245,  ((read_reg8(0x1245) & 0xc0)) | ((chnl_freq>>7)&0x3f) );  //CHNL_FREQ_H
	write_reg8(0x1229,  (read_reg8(0x1229) & 0xC3) | (ctrim<<2) );  //FE_CTRIM

}

/**
 * @brief   	This function serves to set The channel .
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return  	none.
 */
void rf_set_channel_500k(signed short chn, unsigned short set)//general
{
    unsigned short rf_chn =0;
    unsigned char ctrim;
    unsigned short chnl_freq;

    if (set & RF_CHN_TABLE) {
        chn = rf_chn_table[chn];
    }

    rf_chn = chn/2+2400;

    if (rf_chn >= 2550)
        ctrim = 0;
    else if (rf_chn >= 2520)
        ctrim = 1;
    else if (rf_chn >= 2495)
        ctrim = 2;
    else if (rf_chn >= 2465)
        ctrim = 3;
    else if (rf_chn >= 2435)
        ctrim = 4;
    else if (rf_chn >= 2405)
        ctrim = 5;
    else if (rf_chn >= 2380)
        ctrim = 6;
    else
        ctrim = 7;

    chnl_freq = rf_chn * 2 + (chn % 2);

    write_reg8(0x1244, ((chnl_freq & 0x7f)<<1) | 1  );   //CHNL_FREQ_DIRECT   CHNL_FREQ_L
    write_reg8(0x1245, ((read_reg8(0x1245) & 0xc0)) | ((chnl_freq>>7)&0x3f) );  //CHNL_FREQ_H
    write_reg8(0x1229, (read_reg8(0x1229) & 0xC3) | (ctrim<<2) );  //FE_CTRIM
}

/**
 * @brief   	This function serves to set RF power level index. The power setting is related to the power supply
 * 				mode(DCDC or Vbat).When switching between the two power supply modes through pm_set_1p2_power_sel,
 * 				this function needs to be called again to set the energy. The energy in Vbat power supply mode is
 * 				based on 1.5V voltage asthe reference standard.
 * @param[in]  	level - the RF power level.
 * @return  	none.
 * @note		Attention:When the Vbat mode is used for power supply, the energy will decrease as the voltage drops.
 * 		 		When the pm_set_1p2_power_sel select VBAT_SEL supply voltage is reduced from 1.5V to 1.2V, the energy
 * 		 		value of the same energy gear will drop by about 3 to 4 dbm.If DCDC_OUTPUT_SEL is used for power supply,
 * 		 		although the energy will not change with the drop in voltage, its current will be much larger than in
 * 		 		Vbat power supply mode.
 */
void rf_set_power_level_index (RF_PowerTypeDef level)
{
	unsigned char value;
	unsigned char level_idx = 0;
	if(level < ((sizeof(rf_power_Level_list)/sizeof(unsigned char))/2)){
		if(1 == pm_1p2_power_sel_vbat)
			level_idx = rf_power_Level_list[level];
		else
			level_idx = rf_power_Level_list[level+19];
		}
	REG_ADDR8(0x1225) &= ~BIT(6);
    value = (unsigned char)(level_idx & 0x3F);

	write_reg8(0x1226, (read_reg8(0x1226) & 0x7f) | ((value&0x01)<<7) );
	write_reg8(0x1227, (read_reg8(0x1227) & 0xe0) | (value>>1) );
}


/**
*	@brief	  	this function is to Set byte length for access_code.
*	@param[in]	len  	Optional range: 2~5 , The effect for 3-byte access_code is not good.
*	@return	 	none
*/
void rf_acc_len_set(unsigned char len)
{
	write_reg8(0x405, (read_reg8(0x405)&0xf8) | len); //access_byte_num[2:0]

	REG_RF_ACCESS_CODE_GTH = (len*8); //full bit match
}


/**
*	@brief	  	This function serves to swap bit .
*	@param[in]	original  	The objective needs be swapped.
*	@return	 	The result of swapping
*/
static unsigned char bit_swap8(unsigned char original)
{
    unsigned char ret = 0;
    int i = 0;
    for (i = 0; i < 8; i++) {
        if (original & 0x01) {
            ret |= 0x01;
        }
        else {
            ret &= 0xfe;
        }
        if (i == 7) {
            break;
        }
        ret <<= 1;
        original >>= 1;
    }
    return ret;
}

/**
*	@brief	  	This function servers to set multi byte write.
*   @param[in]      reg_start - register start.
*   @param[in]      buf - store bytes, which need to be write.
*   @param[in]      len - the length of byte.
*	@return	 	rf_ed:0x00~0xff
*/
static void multi_byte_reg_write(unsigned int reg_start, unsigned char *buf, int len)
{
    int i = 0;
    for (i = 0; i < len; i++,reg_start++) {
        write_reg8(reg_start, buf[i]);
    }
}
/**
*	@brief	  	This function servers to set multi byte read.
*   @param      reg_start - register start.
*   @param      buf - store bytes, which need to be read.
*   @param      len - the length of byte.
*	@return	 	rf_ed:0x00~0xff
*/
static void multi_byte_reg_read(unsigned int reg_start, unsigned char *buf, int len)
{
    int i = 0;
    for (i = 0; i < len; i++,reg_start++) {
        buf[i] = read_reg8(reg_start);
    }
}
/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/

void rf_acc_code_set(unsigned char pipe_id, const unsigned char *addr)
{
    unsigned char temp[5];
    unsigned char acc_len = read_reg8(0x405) & 0x07;
    unsigned char i = 0;
    for (i = 0; i < acc_len; i++) {
        temp[i] = bit_swap8(addr[i]);
    }
    switch (pipe_id) {
        case 0:
        case 1:
            multi_byte_reg_write(ACCESS_CODE_BASE_PIPE0 + (pipe_id*8), temp, acc_len);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            write_reg8((ACCESS_CODE_BASE_PIPE2 + (pipe_id-2)), temp[0]);
            break;
        default:
            break;
    }
}


/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe_id  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/

void rf_acc_code_get(unsigned char pipe_id, unsigned char *addr)
{
    unsigned char acc_len = read_reg8(0x405) & 0x07;
    unsigned char i = 0;
    switch (pipe_id) {
        case 0:
        case 1:
            multi_byte_reg_read(ACCESS_CODE_BASE_PIPE0 + (pipe_id*8), addr, acc_len);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            *addr = read_reg8((ACCESS_CODE_BASE_PIPE2 + (pipe_id-2)));
            break;
        default:
            break;
    }
    for (i = 0; i < acc_len; i++) {
        addr[i] = bit_swap8(addr[i]);
    }
}

/**
*	@brief	  	This function serves to judge RF Tx/Rx state.
*	@param[in]	rf_status - Tx/Rx status.
*	@param[in]	rf_channel - RF channel.
*	@return	 	failed -1,else success.
*/

int rf_trx_state_set(RF_StatusTypeDef rf_status, signed char rf_channel)
{
    int err = 0;

    write_reg8(0xf02, 0x45);  //close tx and rx before set channel

    rf_set_channel(rf_channel, 0);

    if (rf_status == RF_MODE_TX) {

        write_reg8(0xf02, 0x55);  //tx_en
        write_reg8(0x428, read_reg8(0x428) & 0xfe);  //rx disable
        RF_TRxState = RF_MODE_TX;
    }
    else if (rf_status == RF_MODE_RX) {
    	write_reg8(0xf02, 0x45);
        write_reg8(0xf02, 0x65);
        write_reg8(0x428, read_reg8(0x428) | 0x01);  //rx disable
        RF_TRxState = RF_MODE_RX;
    }
    else if (rf_status == RF_MODE_AUTO) {
        write_reg8(0xf00,0x80); //stop cmd
        write_reg8(0xf16, 0x29); // reg0xf16 pll_en_man and tx_en_dly_en  enable

        write_reg8(0x428, read_reg8(0x428) & 0xfe);  //rx disable
        write_reg8(0xf02, read_reg8(0xf02) & 0xce);  //reg0xf02 disable rx_en_man and tx_en_man
        RF_TRxState = RF_MODE_AUTO;
    }
    else {
        err = -1;
    }
    return  err;
}

/**
*	@brief	  	This function serves to get RF status.
*	@param[in]	none.
*	@return	 	RF Rx/Tx status.
*/
RF_StatusTypeDef rf_trx_state_get(void)
{
    return RF_TRxState;
}



/**
*	@brief	  	This function serves to set RF Tx packet.
*	@param[in]	addr - the address RF to send packet.
*	@return	 	none.
*/

void rf_tx_pkt(unsigned char *rf_txaddr)
{
    reg_dma3_addrHi = 0x04;
    reg_dma_rf_tx_addr = (unsigned short)(unsigned int)(rf_txaddr);
    reg_dma_tx_rdy0 |= FLD_DMA_CHN_RF_TX;
}

/**
 * @brief   This function serves to send packet for RF.
 * @param[in]  addr - the address need to be send.
 * @return  none.
 */
void rf_tx_pkt_auto (void* addr)
{
    write_reg8(0x800f16, read_reg8(0x800f16) & (~BIT(2)) );	// disable cmd_schedule mode
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8 (0x800f00, 0x85);						// single TX

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

void  rf_rx_buffer_set(unsigned char *  RF_RxAddr, int size, unsigned char  PingpongEn)
{
    unsigned char mode;

	mode = PingpongEn ? 0x03 : 0x01;
	reg_dma2_addr = (unsigned short)((unsigned int)RF_RxAddr);
	write_reg8 (0x800c42, (unsigned char)(((unsigned int)RF_RxAddr)>>16));
	reg_dma2_size = size>>4;
	reg_dma2_mode = mode;
	g_RFRxPingpongEn = PingpongEn;
}
/**
*	@brief	  	This function serves to start Tx of ble_mode.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer. It determines when to
*						  	  start ble mode and send packet.
*	@return	 	none
*/
void rf_start_btx (void* addr, unsigned int tick)
{
//	write_reg32 (0x800f28, 0x0fffffff); 	//first rx timeout is not valid in BTX mode (confirmed by sihui & junwen 20190805)
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x81);						// ble tx

	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8 (0x800c43, (unsigned char)(((unsigned int)addr)>>16));

}

/**
*	@brief	  	This function serves to start Tx of ble_mode.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer. It determines when to
*						  	  start ble mode and send packet.
*	@return	 	none
*/
void rf_start_brx  (void* addr, unsigned int tick)
{
	write_reg32 (0x800f28, 0x0fffffff);
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x82);						// ble rx
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8 (0x800c43, (unsigned char)(((unsigned int)addr)>>16));
}

/**
*	@brief	  	This function serves to start Tx.
*	@param[in]	addr   Tx packet address in RAM. Should be 4-byte aligned.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/

void rf_start_stx  (void* addr, unsigned int tick)
{
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x85);						// single TX
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8 (0x800c43, (unsigned char)(((unsigned int)addr)>>16));
}

/**
*	@brief	  	This function serves to start Rx.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/

void rf_start_srx  (unsigned int tick)
{
    write_reg8 (0x800f00, 0x80);						// disable srx
	write_reg32 (0x800f28, 0x0fffffff);					// first timeout
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg16 (0x800f00, 0x3f86);						// srx
}

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
*
*	@return	 	none
*/

void rf_start_stx2rx  (void* addr, unsigned int tick)
{
    write_reg8 (0x800f00, 0x80);						// disable srx
	write_reg32(0x800f18, tick);
	write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8  (0x800f00, 0x87);	// single tx2rx
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
}

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
*	@return	 	none
*/

void rf_start_srx2tx  (void* addr, unsigned int tick)
{
    write_reg8 (0x800f00, 0x80);						// disable srx
	write_reg32 (0x800f28, 0x0fffffff);					// first timeout
	write_reg32(0x800f18, tick);
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
	write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg16 (0x800f00, 0x3f88);								// single rx2tx
}

/*******************************            Private APIs            **************************/

/**
 * @brief   This function serves to reset RF Tx/Rx mode.
 * @param   none.
 * @return  none.
 */

_attribute_ram_code_sec_noinline_ void rf_set_tx_rx_off(void)  //this function is used for too many times in RamCode
{
	write_reg8 (0x800f16, 0x29);
	write_reg8 (0x800428, RF_TRX_MODE);	// rx disable
	write_reg8 (0x800f02, RF_TRX_OFF);	// reset tx/rx state machine
}

/**
 * @brief   	This function serves to set the ble channel.
 * @param[in]   chn_num - channel numbers.
 * @return  	none.
 */
void rf_set_ble_channel (signed char chn_num)
{
	int freq;
	unsigned short chnl_freq;
    unsigned char ctrim;

	write_reg8 (0x80040d, chn_num);
	if (chn_num < 11)
		chn_num += 2;
    else if (chn_num < 37)
    	chn_num += 3;
    else if (chn_num == 37)
    	chn_num = 1;
    else if (chn_num == 38)
    	chn_num = 13;
    else
    	chn_num = 40;

	chn_num = chn_num << 1;

    freq = 2400 + chn_num;
    chnl_freq = freq<<1;


    //calculate FE_CTRIM
    if(freq >= 2550){
        ctrim = 0;
    }
    else if (freq >= 2520){
        ctrim = 1;
    }
    else if (freq >= 2495){
        ctrim = 2;
    }
    else if (freq >= 2465){
        ctrim = 3;
    }
    else if (freq >= 2435){
        ctrim = 4;
    }
    else if (freq >= 2405){
        ctrim = 5;
    }
    else if (freq >= 2380){
        ctrim = 6;
    }
    else{
        ctrim = 7;
    }

    write_reg8(0x1228,  (read_reg8(0x1228) & 0xC0) | ((chn_num - 2)>>1) );
    write_reg8(0x1229,  (read_reg8(0x1229) & 0xC3) | (ctrim<<2) );  //FE_CTRIM
}
/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *input  		- the content of payload
*   @param[in]	len 			- the length of payload
*   @param[in]  init_val 		- the initial value of CRC
*	@return	 	init_val 		- CRC
*/

unsigned short crc16_ccitt_cal(unsigned char *input, unsigned int len, unsigned short init_val)
{
    unsigned short poly = 0x1021;
    unsigned char tmp = 0;
    int i = 0;
    while (len--) {
        tmp = *(input++);
        init_val ^= (tmp << 8);
        for (i = 0;i < 8;i++) {
            if(init_val & 0x8000)
                init_val = (init_val << 1) ^ poly;
            else
                init_val = init_val << 1;
        }
    } 
    return init_val;
}

/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *input  		- the content of payload
*   @param[in]	len 			- the length of payload
*   @param[in]  init_val 		- the initial value of CRC
*	@return	 	init_val 		- CRC
*/

static void rf_byte_extend(unsigned char value, unsigned char *buf)
{
    unsigned long long extended_val = 0x0000000000000000;
    unsigned int temp = 0x00000000;
    unsigned char bit_1 = 0x1f;
    unsigned char bit_offset = 0;
    unsigned char i = 0;
    for (i = 4; i < 8; i++, bit_offset += 5) {
        if (value & (0x01<<i)) {
            temp |= bit_1<<bit_offset;
        }
        else {
            temp &= (~(bit_1<<bit_offset));
        }
    }
    extended_val = temp;
    extended_val <<= 20;
    bit_offset = 0;
    temp = 0;
    for (i = 0; i < 4; i++, bit_offset += 5) {
        if (value & (0x01<<i)) {
            temp |= bit_1<<bit_offset;
        }
        else {
            temp &= (~(bit_1<<bit_offset));
        }
    }
    extended_val |= temp;
    buf[4] = extended_val;
    buf[3] = extended_val >> 8;
    buf[2] = extended_val >> 16;
    buf[1] = extended_val >> 24;
    buf[0] = extended_val >> 32;
}

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

void rf_tx_500k_simulate_100k(unsigned char *preamble, unsigned char preamble_len,
                              unsigned char *acc_code, unsigned char acc_len,
                              unsigned char *payload, unsigned char pld_len,
                              unsigned char *tx_buf, unsigned short crc_init)
{
    unsigned char tx_len = (preamble_len + acc_len + pld_len + 2) * 5;
    tx_buf[0] = tx_len;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x00;

    rf_fix_payload_len_set(tx_len);

    unsigned char *p = &tx_buf[4];
    unsigned char i = 0;
    for (i = 0; i < preamble_len; i++, p += 5) {
        rf_byte_extend(preamble[i], p);
    }
    for (i = 0; i < acc_len; i++, p += 5) {
        rf_byte_extend(acc_code[acc_len-1-i], p);
    }
    for (i = 0; i < pld_len; i++, p += 5) {
        rf_byte_extend(payload[i], p);
    }

    unsigned short crc = crc16_ccitt_cal(payload, pld_len, crc_init);

    rf_byte_extend(crc >> 8, p);
    p += 5;
    rf_byte_extend(crc & 0xff, p);
} 

signed char rssi_max = -110;
/**
*	@brief	  	This function is to start energy detect of the current channel for zigbee mode
*				Before using it.
*   @param[in]  none
*	@return	 	none
*/
void rf_ed_detecct_154(void)
{
    signed char rssi;
    rssi = rf_rssi_get_154();

    if (rssi > rssi_max) {
        rssi_max = rssi;
    }
}

/**
*	@brief	  	This function is to stop energy detect and get energy detect value of
*				the current channel for zigbee mode.
*   @param[in]  none
*	@return	 	rf_ed:0x00~0xff
*
*/


unsigned char rf_stop_ed_154(void)
{
    unsigned char rf_ed;
    unsigned int Temp;
    if (rssi_max <= -72) {
        rf_ed = 0;
    }
    else if (rssi_max >= -35) {
        rf_ed = 0xff;
    }
    else{
        Temp = (rssi_max + 76) * 255;
        rf_ed = Temp/40;
        if(rssi_max == -50)
        {
            rf_ed = rf_ed+2;
        }
    }
    rssi_max = -110;
    return rf_ed;
}

/**
*	@brief	  	This function serves to set pin for RFFE of RF
*				In single tone mode, it does not take effect
*   @param[in]  tx_pin - select pin to send
*   @param[in]  rx_pin - select pin to receive
*	@return	 	none
*
*/
void rf_rffe_set_pin(RF_PATxPinDef tx_pin, RF_LNARxPinDef rx_pin)
{
	    unsigned char val_tx = 0;
		unsigned char val_rx = 0;
		if(tx_pin==RFFE_TX_PB3)
		{
			val_tx= PB3_TX_CYC2PA;
		}
		else if(tx_pin==RFFE_TX_PD1)
		{
			val_tx= PD1_TX_CYC2PA;
		}
	    gpio_set_func(tx_pin,val_tx);

		if((rx_pin == RFFE_RX_PB2)||(rx_pin == RFFE_RX_PD0))
		{
			val_rx = (BIT(0)|BIT(1));
		}
        gpio_set_func(rx_pin,val_rx);
}


#define    CRC_INIT_VAL                   0x1D0F
#define    CRC_LEN_1MBPS                  3
static unsigned char pn_sequence_1mbps_private[32] = {0xb1, 0x4b, 0xea, 0x85, 0xbc, 0xe5, 0x66, 0x0d,
                                                         0xae, 0x8c, 0x88, 0x12, 0x69, 0xee, 0x1f, 0xc7,
                                                         0x62, 0x97, 0xd5, 0x0b, 0x79, 0xca, 0xcc, 0x1b,
                                                         0x5d, 0x19, 0x10, 0x24, 0xd3, 0xdc, 0x3f, 0x8e};
/**
 * @brief      This function process the received packet in 1mbps private mode only for hanshow for the
 *             compatibility with third-party chips. The process includes data-whitening
               transformation and crc check.
 * @param[in]  rx_buf - the rf rx buffer containing the received packet(dma length+payload+3 byte crc)
 * @param[in]  len - the expected rx length of private mode, containing payload and 3byte crc
 *
 * @return     the status of the processing procedure. 1: the received packet is correct, 0: the received packet is incorrect
 */
unsigned char rx_packet_process_1mbps(unsigned char *rx_buf, unsigned int len)
{
    unsigned char i = 0;
    unsigned char ret = 0;
    unsigned int crc_in_packet = 0;
    unsigned int crc_tmp = 0;

    if ((0 == rx_buf) || (len > 32)) {
        return ret;
    }

    //do data-whitening transformation for both payload and crc fields
    for (i = 0; i < len; i++) {
        rx_buf[4+i] ^= pn_sequence_1mbps_private[i];
    }
    //parse the 3byte crc from the received packet
    crc_in_packet = (rx_buf[4+len-3]<<16) + (rx_buf[4+len-2]<<8) + rx_buf[4+len-1];
    //calculate the crc value of the received payload field
    crc_tmp = crc16_ccitt_cal(&rx_buf[4], len-CRC_LEN_1MBPS, CRC_INIT_VAL);
    crc_tmp <<= 8; //L-shift 8 bit to became a 3byte crc for the compatibility with 2640's 1mbps private mode

    if (crc_tmp != crc_in_packet) {
        return ret;
    }

    //reverse the bit order of the payload field
    for (i = 0; i < (len-CRC_LEN_1MBPS); i++) {
        rx_buf[4+i] = bit_swap8(rx_buf[4+i]);
    }

    ret = 1;
    return ret;
}

/**
 * @brief      This function process the tx packet in 1mbps private mode only for hanshow for the
 *             compatibility with third-party chips. The process includes data-whitening
               transformation and crc padding.
 * @param[in]  tx_buf - the rf tx buffer containing the tx packet(dma length+payload+3 byte crc)
 * @param[in]  len - the expected tx length of private mode, containing payload and 3byte crc
 *
 * @return     none
 */
void tx_packet_process_1mbps(unsigned char *tx_buf, unsigned char *payload, unsigned int len)
{
    unsigned char i = 0;
    unsigned short crc = 0;

    if ((0 == tx_buf) || (0 == payload) || (len > 32)) {
        return;
    }

    //reverse bit order first
    for (i = 0; i < (len-CRC_LEN_1MBPS); i++) {
        payload[i] = bit_swap8(payload[i]);
    }

    //fill Tx Buffer
    tx_buf[0] = len;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x00;
    //reverse bit order first
    for (i = 0; i < (len-CRC_LEN_1MBPS); i++) {
        tx_buf[4+i] = payload[i];
    }

    //fill the 3byte crc, note that crc is trasmitted MSByte first
    crc = crc16_ccitt_cal(payload, len-CRC_LEN_1MBPS, CRC_INIT_VAL);
    tx_buf[4+len-CRC_LEN_1MBPS] = (unsigned char)(crc >> 8);
    tx_buf[4+len-CRC_LEN_1MBPS+1] = (unsigned char)(crc & 0x00ff);
    tx_buf[4+len-CRC_LEN_1MBPS+2] = 0;

    //do data-whitening transformation both for the payload and crc fields
    for (i = 0; i < len; i++) {
        tx_buf[4+i] ^= pn_sequence_1mbps_private[i];
    }

    return;
}



/**
 * @brief      This function
 * @param[in]  size - the expected size of dma2
 * @param[in]  PingpongEn - the flag of g_RFRxPingpongEn
 * @return     none
 */
void  rf_rx_cfg(int size, unsigned char  PingpongEn)
{
    unsigned char mode;
	mode = PingpongEn ? 0x03 : 0x01;
	reg_dma2_size = size>>4;
	reg_dma2_mode = mode;
	g_RFRxPingpongEn = PingpongEn;
}

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
void  rf_2wire_pta_init(PTA_BlePriorityPinDef ble_priority_pin,GPIO_PinTypeDef wlan_active_pin,PTA_2WireModeDef ble_priority_mode)
{

	reg_gpio_wakeup_irq|=BIT(0);  //wifi_2wire_sel
	//ble_activity

    if(ble_priority_mode==PTA_BLE_PRIORITY_TX)
    {
		reg_rf_ll_wifi_coex &= (~FLD_RF_TRX_PRIO);
		reg_rf_ll_wifi_coex &= (~FLD_RF_RX_PRIO);
		reg_rf_ll_wifi_coex |= (FLD_RF_TX_PRIO);
    }
    else if(ble_priority_mode==PTA_BLE_PRIORITY_RX)
    {
		reg_rf_ll_wifi_coex &= (~FLD_RF_TRX_PRIO);
		reg_rf_ll_wifi_coex &= (~FLD_RF_TX_PRIO);
		reg_rf_ll_wifi_coex |= (FLD_RF_RX_PRIO);
    }
    else if(ble_priority_mode==PTA_BLE_PRIORITY_TRX)
    {
		reg_rf_ll_wifi_coex &= (~FLD_RF_RX_PRIO);
		reg_rf_ll_wifi_coex &= (~FLD_RF_TX_PRIO);
		reg_rf_ll_wifi_coex |= (FLD_RF_TRX_PRIO);
    }

    gpio_set_output_en(wlan_active_pin,0);
    gpio_set_input_en(wlan_active_pin,1);

    if(ble_priority_pin==PTA_BLE_PRIORITY_PA3)
	{
		gpio_set_func(GPIO_PA3,PA3_BLE_PRIORITY);
	}
	else if(ble_priority_pin==PTA_BLE_PRIORITY_PB0)
	{
		gpio_set_func(GPIO_PB0,PB0_BLE_PRIORITY);
	}
	//wifi_active
	gpio_set_func(wlan_active_pin,AS_GPIO);


}

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
void  rf_3wire_pta_init(PTA_BleActivePinDef ble_active_pin,PTA_BleStatusPinDef ble_status_pin,PTA_WlanDenyPinDef wlan_deny_pin,PTA_3WireModeDef ble_status_mode)
{

	reg_gpio_wakeup_irq&=(~BIT(0));  //wifi_2wire_sel
	reg_rf_ll_wifi_coex |= FLD_RF_COEX_EN;//r_coex_en
	reg_rf_ll_wifi_coex |= FLD_RF_COEX_STATUS;//r_coex_status

	(ble_status_mode==0)?(reg_rf_ll_wifi_coex&=(~BIT(3))):(reg_rf_ll_wifi_coex|=(BIT(3)));//PTA 3-wire mode choose
	//ble_active
	if(ble_active_pin==PTA_BLE_ACTIVE_PA3)
	{
		gpio_set_func(GPIO_PA3,PA3_BLE_ACTIVE);
	}
	else if(ble_active_pin==PTA_BLE_ACTIVE_PB0)
	{
		gpio_set_func(GPIO_PB0,PB0_BLE_ACTIVE);
	}

	//ble_status
	if(ble_status_pin==PTA_BLE_STATUS_PA4)
	{
	    gpio_set_func(GPIO_PA4,PA4_BLE_STATUS);
	}
	else if(ble_status_pin==PTA_BLE_STATUS_PB3)
	{
		gpio_set_func(GPIO_PB3,PB3_BLE_STATUS);
	}

    //wifi_deny
	if(wlan_deny_pin==PTA_WLAN_DENY_PA6)
    {
        gpio_set_func(GPIO_PA6,PA6_WLAN_DENY);
	}
    else if(wlan_deny_pin==PTA_WLAN_DENY_PB4)
	{
		gpio_set_func(GPIO_PB4,PB4_WLAN_DENY);
	}

}

/**
 * @brief      This function is to disable PN
 * @return     none
 */
void  rf_pn_disable()
{
	write_reg8 (0x401, 0);//// PN disable
	write_reg8 (0x404, read_reg8(0x404)&0xdf);//BIT<5>: ble_wt(PN) disable
}

void rx_dly_dis()
{
	write_reg8(0x127b, 0xfe);//,  TCMD_UNDER_BOTH | TCMD_WRITE}, //fm2pm
	write_reg8(0x1279, 0x08);
}



/**
 * @brief	  	This function serves to close internal cap;
 * @return	 	none.
 */
void rf_turn_off_internal_cap(void)
{
	analog_write(0x8a,analog_read(0x8a)|0x80);//close internal cap
}

/**
 * @brief	  	This function serves to update the value of internal cap.
 * @param[in]  	value   - The value of internal cap which you want to set.the value range from 0 to 0x3f.
 * @return	 	none.
 */
void rf_update_internal_cap(unsigned char value)
{
	analog_write(0x8a,(analog_read(0x8a)&0x40)|(value&0x3f));
}

/**
 * @brief	This function serve to change the length of preamble.
 * @param[in]	len		-The value of preamble length.Set the register bit<0>~bit<4>.
 * @return		none
 */
void rf_set_preamble_len(unsigned char len)
{
	len = len&0x1f;
	write_reg8(0x800402,(read_reg8(0x800402)&0xe0)|len);
}

/**
 * @brief   This function serves to set RF rx timeout.
 * @param[in]	timeout_us	-	rx_timeout after timeout_us us,The maximum of this param is 0xfff.
 * @return  none.
 */
void rf_set_rx_timeout(unsigned short timeout_us)
{
	unsigned short tmp;
	timeout_us &= 0x0fff;
	tmp = reg_rf_rx_timeout & 0xf000;
	reg_rf_rx_timeout = ((tmp | timeout_us) - 1);
}
/**
 * @brief	  	This function is used to set the modulation index of the receiver.
 *              This function is common to all modes,the order of use requirement:configure mode first,then set the the modulation index,default is 0.5 in drive,
 *              both sides need to be consistent otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100.
 * @return	 	none.
 */
void rf_set_rx_modulation_index(RF_MIVauleTypeDef mi_value)
{
	unsigned char modulation_index_high;
	unsigned char modulation_index_low;
	unsigned char kvm_trim;
	unsigned char mi_int = mi_value*128/1000;

	modulation_index_low = mi_int%256;

	modulation_index_high = (mi_int%512)>>8;
	reg_rf_rx_flex_mi_0 = modulation_index_low;
	reg_rf_rx_flex_mi_1 |= modulation_index_high;

	if(reg_rf_mode_cfg_tx1_0 & 0x01)//2MBPS mode
	{
		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 3;
		else if (mi_value > 1000)
			kvm_trim = 7;
		else
			kvm_trim = 1;
	}
	else
	{

		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 1;
		else if ((mi_value > 1000)&&(mi_value <= 1500))
			kvm_trim = 3;
		else if (mi_value > 1500)
			kvm_trim = 7;
		else
			kvm_trim = 0;
	}
	reg_rf_mode_cfg_tx1_0=(reg_rf_mode_cfg_tx1_0&~(FLD_RF_VCO_TRIM_KVM))|(kvm_trim<<1);
}



/**
 * @brief	  	This function is used to  set the modulation index of the sender.
 *              This function is common to all modes,the order of use requirement:configure mode first,then set the the modulation index,default is 0.5 in drive,
 *              both sides need to be consistent otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100.
 * @return	 	none.
 */
void rf_set_tx_modulation_index(RF_MIVauleTypeDef mi_value)
{

	unsigned char modulation_index_high;
	unsigned char modulation_index_low;
	unsigned char kvm_trim;
	unsigned char mi_int = mi_value*128/1000;
	modulation_index_low = mi_int%256;

	modulation_index_high = (mi_int%512)>>8;
	reg_rf_tx_flex_mi_0 = modulation_index_low;
	reg_rf_tx_flex_mi_1 |= modulation_index_high;

	if(reg_rf_mode_cfg_tx1_0 & 0x01)//2MBPS mode
	{
		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 3;
		else if (mi_value > 1000)
			kvm_trim = 7;
		else
			kvm_trim = 1;
	}
	else
	{

		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 1;
		else if ((mi_value > 1000)&&(mi_value <= 1500))
			kvm_trim = 3;
		else if (mi_value > 1500)
			kvm_trim = 7;
		else
			kvm_trim = 0;
	}

	reg_rf_mode_cfg_tx1_0=(reg_rf_mode_cfg_tx1_0&~(FLD_RF_VCO_TRIM_KVM))|(kvm_trim<<1);

}



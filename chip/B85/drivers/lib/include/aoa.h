/********************************************************************************************************
 * @file	aoa.h
 *
 * @brief	This is the header file for b85
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
#pragma once

#include "bsp.h"
#include "analog.h"
#include "register.h"
#include "gpio.h"
#include "printf.h"
#include "string.h"
#include "rf_drv.h"

#define RF_REAL_FREQ			2460
#define IQ_NUM                  45
#define SLOT_TIME				2
#define Lamida                  3*10000*1000/(RF_REAL_FREQ*100 + 25)
#define sign_mark				-1
#define Radius					75
#define XY_TABLE_NUMBER			45
#define Z_TABLE_NUMBER			1
#define AMP_SIZE				32


#define SRAM_BOTTOM			    (int *)sram_bottom_aoa	//0x840000
#define SRAM_TOP				(int *)sram_top_aoa   //0x84FFFF


#define sram_bottom_aoa         0x840000
#define sram_top_aoa            0x84FFFF

typedef struct IQ_VALUE
{
	int real_part;
	int imag_part;
}IQ_Value;

typedef struct IQ_CB
{
	IQ_Value IQ[100];
	int number;
}IQ_cb;

typedef enum{
	BYTE_NUM_2P5	 = 0,
	BYTE_NUM_4		 = 1,
}TypeDefByteNum;

typedef struct FUNC_CB
{
	int* 	int_ptr1;
	int* 	int_ptr2;
	char*   char_ptr1;
	char*	char_ptr2;
	int		int1;
	int		int2;
	char	char1;
	char	char2;
	char	char3;
	char	char4;
}Func_CB;



/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
_attribute_ram_code_sec_noinline_ unsigned int raw_data_to_angle_no_z(unsigned char *ptr_packet);

/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @param[out]  temp_theta  -  Pointer variable of height angle information is stored.
 * @return     The angle.
 */
_attribute_ram_code_sec_noinline_ unsigned int raw_data_to_angle_with_z(unsigned char *ptr_packet,unsigned char * temp_theta);

/**
 * @brief      This function servers to store data to a specific place .
 * @param[in]  data_table  -  Pointer variable to store data.
 * @param[in]  ptr_packet  -  Pointer variable to support data.
 * @param[in]  number      -  The data numbers.
 * @return     none.
 */
void get_raw_data(unsigned char *data_table,unsigned char *ptr_packet,unsigned int number);

/**
 * @brief      This function servers initialization a look up table.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_no_Z(void);

/**
 * @brief      This function servers initialization a look up table.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_with_Z(void);

/**
 * @brief		This function is used to convert 20bit to 8bit data
 * @param[in]  	*data_src 				- the ddr of data.
 * @param[in] 	*data_has_amplitude  	- the data with amplitude.
 * @param[in]	*data_no_amplitude		- the data without amplitude.
 * @return     	none.
 */
void frond_end(unsigned char *data_src, unsigned char *data_has_amplitude, unsigned char *data_no_amplitude);




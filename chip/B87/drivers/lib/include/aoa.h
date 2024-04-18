/********************************************************************************************************
 * @file    aoa.h
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
#pragma once

#include "bsp.h"
#include "analog.h"
#include "register.h"
#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include "rf_drv.h"

#define RF_REAL_FREQ			2440
#define IQ_NUM                  45
#define SLOT_TIME				2
#define Lamida                  3*10000*1000/(RF_REAL_FREQ*100 + 25)
//#define sign_mark				1		//for rectangle
#define sign_mark				-1
#define Radius					75//129
#define XY_TABLE_NUMBER			45
#define Z_TABLE_NUMBER			31//31
#define AMP_SIZE				32

#define SRAM_BOTTOM				0x840000
#define SRAM_TOP				0x84FFFF


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

typedef enum{
	BYTE_NUM_2P5	 = 0,
	BYTE_NUM_4		 = 1,
}TypeDefByteNum;


/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
_attribute_ram_code_sec_noinline_ unsigned int raw_data_to_angle_no_z(unsigned char *ptr_packet);

/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @param[out]  temp_theta  -  Pointer variable of the height angle information is stored.
 * @return     The angle.
 */
_attribute_ram_code_sec_noinline_ unsigned int raw_data_to_angle_with_z(unsigned char *ptr_packet , unsigned char* temp_theta);

/**
 * @brief      This function servers to store data to a specific place .
 * @param[in]  data_table  -  Pointer variable to store data.
 * @param[in]  ptr_packet  -  Pointer variable to support data.
 * @param[in]  number      -  The data numbers.
 * @return     none.
 */
void get_raw_data(unsigned char *data_table,unsigned char *ptr_packet,unsigned int number);

/**
 * @brief      This function servers to initialize the look-up table algorithm without Z-axis information.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_no_Z(void);

/**
 * @brief      This function servers to initialize the look-up table algorithm with Z-axis information.
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




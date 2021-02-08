/********************************************************************************************************
 * @file	aoa.h
 *
 * @brief	This is the header file for b87
 *
 * @author	Driver Group
 * @date	2019
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
#include "timer.h"

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
 * @brief	Take 4 antennas as an example to illustrate the antenna switching sequence
 * 			SWITCH_SEQ_MODE0	: antenna index switch sequence 01230123
 * 			SWITCH_SEQ_MODE1	: antenna index switch sequence 0123210
 * 			SWITCH_SEQ_MODE2	: antenna index switch sequence 001000200030
 */
typedef enum{
	SWITCH_SEQ_MODE0		 = 0,//
	SWITCH_SEQ_MODE1		 = 1,
	SWITCH_SEQ_MODE2		 = 2,
}TypeDefAntennaSeq;

/**
 * @brief      This function servers to get the tan_angle.
 * @param[in]  tan  -  The angel which need to be assignment
 * @return     The tan_angle.
 */
char tan_look_table(unsigned int tan);

/**
 * @brief      This function servers to get the sin_angle.
 * @param[in]  sin  -  The angel which need to be assignment
 * @return     The sin_angle.
 */
char sin_look_table(unsigned int sin);

/**
 * @brief      This function servers to get the cos_angle.
 * @param[in]  cos_val  -  The angel which need to be assignment
 * @return     The cos_angle.
 */
int cos_calculate(int cos_val);

/**
 * @brief      This function servers to demodulation the input angle.
 * @param[in]  real_ptr  -  Pointer variable of real part.
 * @param[in]  imag_ptr  -  Pointer variable of imag part.
 * @param[in]  num       -  The value of the total parts.
 * @return     none.
 */
void demodulation(int* real_ptr,int* imag_ptr,unsigned char num);

//void conj(IQ_cb iq,IQ_cb conj_iq,unsigned char number);

/**
 * @brief      This function servers to calculate the phase.
 * @param[in]  real  -  Stand for the real parts.
 * @param[in]  imag  -  Stand for the image parts.
 * @return     The phase value.
 */
int  calc_phase(int real, int imag);

/**
 * @brief      This function servers to calculate real and imag parts.
 * @param[in]  Table1  -  A struct to store value or stand for numbers.
 * @param[in]  Table2  -  A struct to store value or stand for numbers..
 * @param[in]  fst_num -  The value stand for a start number.
 * @param[in]  scd_num -  The value stand for the next number.
 * @return     none.
 */
void dot_product(IQ_cb Table1,IQ_cb Table2,unsigned char fst_num,unsigned char scd_num);	//,IQ_Value rst_tb

/**
 * @brief      This function servers to get phase.
 * @param[in]  x_ptr  -  Pointer variable of the true phase value.
 * @param[in]  osr    -  The value use for calculation.
 * @return     The phase.
 */
//void dot_product(unsigned char first_num,unsigned char second_num);
int  phase_combine(int* x_ptr,int osr);

/**
 * @brief      This function servers to calculate the angel.
 * @param[in]  x -  The value need to be assignment.
 * @return     The angel.
 */
int  angle_trans(int x);	//input is 1024 times

/**
 * @brief      This function servers sure the angel value is accurate.
 * @param[in]  ptr1 -  Pointer variable of the angel.
 * @param[in]  ptr2 -  Pointer variable of the angel.
 * @return     0.
 */
char unwrap(int* ptr1,int* ptr2);

/**
 * @brief      This function servers to get the angel which already minus the average value.
 * @param[in]  ptr1 -  Pointer variable of the angel.
 * @return     none.
 */
void delate_average(int* ptr);


/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
_attribute_ram_code_ unsigned int raw_data_to_angle_no_z(unsigned char *ptr_packet);

/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
_attribute_ram_code_ unsigned int raw_data_to_angle_with_z(unsigned char *ptr_packet);

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


unsigned char wrap(Func_CB func);
/**
 * @brief      This function servers to shift the circuit.
 * @param[in]  func -  Structure stand for the parameter.
 * @return     Flag. 1 - shift failure 0 - shift success.
 */
unsigned char circshift(Func_CB func);


unsigned char trans_uchar_to_int(unsigned char* src_tb,int* rst_tb,unsigned int number);

/**
 * @brief		This function is used to convert 20bit to 8bit data
 * @param[in]  	*data_src 				- the ddr of data.
 * @param[in] 	*data_has_amplitude  	- the data with amplitude.
 * @param[in]	*data_no_amplitude		- the data without amplitude.
 * @return     	none.
 */
void frond_end(unsigned char *data_src, unsigned char *data_has_amplitude, unsigned char *data_no_amplitude);

/**
 * @brief		This function servers to find the sum of a set of numbers
 * @param[in]  	*data - the ddr of data.
 * @param[in] 	len   - the number of data.
 * @return     	sum.
 */
unsigned int sum(int *data, unsigned char len);

/**
 * @brief		This function servers to find the average of a set of numbers
 * @param[in]  	*data - the ddr of data.
 * @param[in] 	len   - the number of data.
 * @return     	average.
 */
unsigned int mean(int *data, unsigned char len);

/**
 * @brief		This function servers to find the average of a set of numbers
 * @param[in]  	data - the  data.
 * @param[in] 	bytenum   - the type of data.
 * @return     	abs.
 */
unsigned int abs_value(int data, TypeDefByteNum bytenum);


/**
 * @brief		This function serve to find the minimum value of a set of values and return its index value.
 * @param[in]	*data - Address requiring minimum number.
 * @param[in]	len - Number of digits.
 * @return		Minimum.
 */
unsigned int min(int *data, unsigned char len, unsigned char* index);

/**
 * @brief		This function serve to set the antenna switch sequence.
 * @param[in]	TypeDefAntennaSeq - the switch sequence mode num.
 * @return		none.
 */
void set_antenna_switch_sequence(TypeDefAntennaSeq seq_mode);

/**
 * @brief		This function serve to set the antenna switch sequence.
 * @param[in]	ant_num - the number of antenna.
 * @return		none.
 */
void set_antenna_num(unsigned char ant_num);

/**
 * @brief		this function to set the table of antenna switch and select the switch mode.
 * @param[in]	addr - the addr of switch tale.
 * @param[in] 	seq_mode - switch mode.
 * @return	  	none.
 */
void supp_ant_lut(unsigned char * addr,TypeDefAntennaSeq seq_mode);


void triangle_all_open(void);


/********************************************************************************************************
 * @file    keyscan.h
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
/**	@page KEYSCAN
 *
 *	Introduction
 *	===============
 *	1. Keyscan can only save 9 keys at the same time in the same row, and can save 30 keys at the same time in different rows.
 *	2. In each debounce cycle, the hardware will finally store an end flag in the fifo.
 *	3. The duration of a single key must be more than 16ms.
 *
 *	API Reference
 *	===============
 *	Header File: keyscan.h
 */
#ifndef KEYSCAN_H_
#define KEYSCAN_H_
#include "bsp.h"
#include "analog.h"
#include "register.h"
#include "gpio.h"

/**
 * @brief		Set the end flag of the hardware upload key value after each keyscan interrupt is generated.
 * 				Since the configured end_flag is an 8bit value and cannot conflict with the keyboard key value,
 * 				and only number "27" of the column number "0-31" can not be configured as a keyscan column,
 * 				so select "0xfb" to configure the end flag of the hardware upload key value .
 */
#define KESYCAN_END_FLAG	0xfb

/**
 *  @brief  	KEYSCAN pin value
 */
typedef enum{
	KS_PD0 = 0	, KS_PD1, KS_PD2, KS_PD3, KS_PD4, KS_PD5, KS_PD6, KS_PD7,
	KS_PC0		, KS_PC1, KS_PC2, KS_PC3, KS_PC4, KS_PC5, KS_PC6, KS_PC7,
	KS_PB0		, KS_PB1, KS_PB2, KS_PB3, KS_PB4, KS_PB5, KS_PB6, KS_PB7,
	KS_PA0		,
	KS_PF0 = 25, KS_PF1 = 26,
	KS_PA4 = 28, KS_PA5 = 29, KS_PA6 = 30, KS_PA7 = 31,
}ks_value_e;


/**
 *  @brief  	KEYSCAN scan time
 *  			If the debounce period is 8ms,the triple scan times will check the key values at the three time points of 0/4/8,and the double scan times will check the key values at the two time points of 0/8.
 *  			Write the same key value to fifo.
 */
typedef enum{
	DOUBLE_SCAN_TIMES  = 0,	/**< Scan double times in one debounce cycle */
	TRIPLE_SCAN_TIMES = 1,	/**< Scan triple times in one debounce cycle */
}ks_scan_times_e;

/**
 *  @brief  	KEYSCAN Debounce Period
 *  			The debounce period is an integer multiple of a scan period, and the scan period is given by hardware and is fixed at 4ms.
 */
typedef enum{
	DEBOUNCE_PERIOD_8MS   = 0x2,
	DEBOUNCE_PERIOD_12MS  = 0x3,
	DEBOUNCE_PERIOD_16MS  = 0x4,
	DEBOUNCE_PERIOD_20MS  = 0x5,
	DEBOUNCE_PERIOD_24MS  = 0x6,
	DEBOUNCE_PERIOD_28MS  = 0x7,
}ks_debounce_period_e;
/**
 *  @brief  	Define pull up or down types
 *          	The column GPIO must remain unfloating.
 */
typedef enum {
	KS_EXT_PIN_PULLUP     	= (0x1<<4)|PM_PIN_UP_DOWN_FLOAT,/**< External pull-up.When the column of the keyscan is configured as pull-up, when the column is detected as low level, it means that the button is pressed */
	KS_EXT_PIN_PULLDOWN  	= (0x2<<4)|PM_PIN_UP_DOWN_FLOAT,/**< External pull-down.When the column of the keyscan is configured as pull-down, when the column is detected as high level, it means that the button is pressed */
	KS_INT_PIN_PULLUP     	= (0x1<<4)|PM_PIN_PULLUP_10K,	/**< Internal pull-up.When the column of the keyscan is configured as pull-up, when the column is detected as low level, it means that the button is pressed */
	KS_INT_PIN_PULLDOWN  	= (0x2<<4)|PM_PIN_PULLDOWN_100K,/**< Internal pull-down.When the column of the keyscan is configured as pull-down, when the column is detected as high level, it means that the button is pressed */
}ks_col_pull_type_e;

/**
 * @brief 		This function servers to clear irq status.
 * 				After the hardware writes the key value into the fifo in each debounce period, an interrupt is triggered until it enters the idle state.
 * @return      none.
 */
static inline void keyscan_clr_irq_status(void)
{
	reg_ks_irq |= FLD_KS_FRM_END;
}
/**
 * @brief 		This function servers to get read pointer.
 * @return      the value of the read pointer.
 *              Every time the software reads a key value from fifo, the read pointer increases by one.
 */
static inline unsigned char keyscan_get_rptr(void)
{
	return reg_ks_irq & FLD_KS_RPTR;
}
/**
 * @brief 		This function servers to get write pointer.
 * @return      the value of the write pointer.
 *              Every time the hardware writes a key value into fifo, the write pointer increases by one.
 */
static inline unsigned char keyscan_get_wptr(void)
{
	return reg_ks_rptr & FLD_KS_R_WPTR;
}
/**
 * @brief 		This function serves to enable keyscan module.
 * @return 		none
 */
static inline void keyscan_enable(void)
{
	BM_SET(reg_ks_en,FLD_KS_EN);
}
/**
 * @brief 		This function serves to disable keyscan module.
 * @return 		none
 */
static inline void keyscan_disable(void)
{
	BM_CLR(reg_ks_en,FLD_KS_EN);
}
/**
 * @brief 		This function serves to set the rows and columns of the keyscan .
 * @param[in]   ks_row 		- The array element must be a member of the enumeration type ks_value_e.
 * @param[in]   row_cnt 	- Count of rows. Range is 1-8.
 * @param[in]   ks_col 		- The array element must be a member of the enumeration type ks_value_e.
 * @param[in]   col_cnt 	- Count of columns. Range is 1-31.
 * @param[in]   ks_col_pull - Configure the type of pull-up/pull-down resistors for column GPIO.
 * @return 		none
 */
void keyscan_set_martix(unsigned char* ks_row,unsigned char row_cnt,unsigned char* ks_col,unsigned char col_cnt, ks_col_pull_type_e ks_col_pull);
/**
 * @brief 		This function serves to get the key value.
 *				The key value "0xfb"(KESYCAN_END_FLAG) is the end sign, which indicates the end of a debounce period. It is written into fifo by the hardware after uploading the last valid key value.
 *				Each time the software reads the key value, the read pointer will increase by 1.
 *				Each time the hardware writes the key value, the write pointer will increase by 1.
 * @return 		key value - bit[7:5] represents the row number, the range is from 0 to 7.
 * 							bit[4:0] represents the column number, the range is from 0 to 30.
 */
unsigned char keyscan_get_ks_value(void);
/**
 * @brief 		This function serves to set keyscan configuration.
 * @param[in]   debounce_period 		- Set the time of a debounce cycle.
 * @param[in]   enter_idle_period_num 	- Set how many debounce cycles to enter the idle state without detecting a button.
 * 										  The value setting range is 1-32.
 * 										  The idle state will stop scanning and reduce power consumption.
 * 										  Press again to exit the idle state.
 * @param[in] 	scan_times 				- 1 is to set triple scan times,0 is to set double scan times.
 * @return 		none
 */
void keyscan_init(ks_debounce_period_e debounce_period,unsigned char enter_idle_period_num, ks_scan_times_e scan_times);
#endif /* KEYSCAN_H_ */

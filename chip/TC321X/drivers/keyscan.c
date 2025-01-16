/********************************************************************************************************
 * @file    keyscan.c
 *
 * @brief   This is the source file for TC321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "keyscan.h"

//Calculate the corresponding GPIO serial number according to the formula and the pin serial number of the given keyscan.
#define KEYSCAN_PIN(pin) ((3 - pin / 8) << 8) | BIT(pin % 8)

volatile unsigned char g_keyscan_col_cnt = 0;
volatile unsigned char g_keyscan_col[31] = {0};


/**
 * @brief 		This function serves to set the debounce period and how long to enter the idle state.
 * @param[in]   debounce_period 		- Set the time of a debounce cycle.
 * @param[in]   enter_idle_period_num 	- Set how many debounce cycles to enter the idle state without detecting a button.
 * 										  The value setting range is 1-32.
 * 										  The idle state will stop scanning and reduce power consumption.
 * 										  Press again to exit the idle state.
 * @return 		none
 */
void keyscan_set_time(ks_debounce_period_e debounce_period, unsigned char enter_idle_period_num)
{
	reg_ks_frm_num = ((debounce_period<<5) & FLD_KS_DEBOUNCE_PERIOD) | ((enter_idle_period_num - 1) & FLD_KS_FRM_NUM);
}

/**
 * @brief 		This function serves to set keyscan rows.
 * @param[in]   ks_row 	- The array element must be a member of the enumeration type ks_value_e.
 * @param[in]   row_cnt - Count of rows. Range is 1-8.
 * @return 		none
 */
void keyscan_set_row(unsigned char* ks_row, unsigned char row_cnt)
{
	unsigned char i;
	unsigned int martix_row_sel = 0;
	unsigned char martix_row_7_sel = 0;

	for (i = 0; i < row_cnt; i++) {
		gpio_set_func(KEYSCAN_PIN(ks_row[i]), PA_KS0_IO);

		if (i < 6) {
			martix_row_sel |= (ks_row[i] << (5 * i));
		}
		if (i == 6) {
			martix_row_sel |= (ks_row[i] & 0x3) << 30;
			martix_row_7_sel |= ks_row[i] >> 2;
		} 
		if (i == 7) {
			martix_row_7_sel |= ks_row[i] << 3;
		}
	}
	/* Initialize the value of the row register and set each row to 0x1b.
	 * Because the default value of the register is 0, setting PD0 as a column will conflict with the default value of the row register.
	 * And only number "27" of the column number "0-31" can not be configured as a keyscan column*/
	for(i = row_cnt; i < 8; i++) {
		if(i < 6) {
			martix_row_sel |= (0x1b << (5 * i));
		}
		if(i == 6) {
			martix_row_sel |= (0x1b & 0x3) << 30;
			martix_row_7_sel |= 0x1b >> 2;
		}
		if(i == 7) {
			martix_row_7_sel |= 0x1b << 3;
		}
	}
	reg_ks_row_sel0 = martix_row_sel;
	reg_ks_row_sel4 = martix_row_7_sel;
}
/**
 * @brief 		This function serves to set keyscan columns.
 * @param[in]   ks_col 		- The array element must be a member of the enumeration type ks_value_e.
 * @param[in]   col_cnt 	- Count of columns. Range is 1-31.
 * @param[in]   ks_col_pull - Configure the type of pull-up/pull-down resistors for column GPIO.
 * @return 		none
 */
void keyscan_set_col(unsigned char* ks_col, unsigned char col_cnt, ks_col_pull_type_e ks_col_pull)
{
	unsigned char i = 0;
	unsigned int martix_col_sel = 0;

	if((KS_EXT_PIN_PULLUP == ks_col_pull) || (KS_INT_PIN_PULLUP == ks_col_pull)){
		BM_SET(reg_ks_en,FLD_KS_IN_INV | FLD_KS_OUT_INV);
	}else if((KS_EXT_PIN_PULLDOWN == ks_col_pull) || (KS_INT_PIN_PULLDOWN == ks_col_pull)){
		BM_CLR(reg_ks_en,FLD_KS_IN_INV | FLD_KS_OUT_INV);
	}

	ks_col_pull = ks_col_pull & 0xf;

	for(i = 0; i < col_cnt; i++){
		gpio_set_input_en(KEYSCAN_PIN(ks_col[i]), 1);
		gpio_set_output_en(KEYSCAN_PIN(ks_col[i]), 0);
		gpio_setup_up_down_resistor(KEYSCAN_PIN(ks_col[i]), ks_col_pull);
		gpio_set_func(KEYSCAN_PIN(ks_col[i]), PA_KS0_IO);

		martix_col_sel |= (unsigned int)(1 << ks_col[i]);
	}
	reg_ks_col_mask0 = martix_col_sel;
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
void keyscan_set_martix(unsigned char* ks_row, unsigned char row_cnt, unsigned char* ks_col, unsigned char col_cnt, ks_col_pull_type_e ks_col_pull)
{
	unsigned char i;
	/* g_keyscan_col_cnt saves the total number of columns during user configuration,
	 * and g_keyscan_col saves the number of columns during user configuration.*/
	g_keyscan_col_cnt = col_cnt;
	for(i = 0; i < col_cnt; i++) {
		g_keyscan_col[i] = ks_col[i];
	}

	keyscan_set_row((unsigned char* )ks_row, (unsigned char)row_cnt);
	keyscan_set_col((unsigned char* )ks_col, (unsigned char)col_cnt, ks_col_pull);
}


/**
 * @brief 		This function serves to get the key value.
 *				The key value "0xfb"(KESYCAN_END_FLAG) is the end sign, which indicates the end of a debounce period. It is written into fifo by the hardware after uploading the last valid key value.
 *				Each time the software reads the key value, the read pointer will increase by 1.
 *				Each time the hardware writes the key value, the write pointer will increase by 1.
 * @return 		key value - bit[7:5] represents the row number, the range is from 0 to 7.
 * 							bit[4:0] represents the column number, the range is from 0 to 30.
 */
unsigned char keyscan_get_ks_value(void)
{
	unsigned char ks_key;
	ks_key = reg_ks_key;
	unsigned char pos_row = ks_key&0xe0;
	unsigned char pos_col = ks_key&0x1f;
	/* After the software reads the key value in the FIFO,
	 * it converts the column number of the key value to the column number when configured by the user according to g_keyscan_col.*/
	for(unsigned char i = 0; i < g_keyscan_col_cnt; i++){
		if(pos_col == g_keyscan_col[i]){
			pos_col = i;
			break;
		}
	}
	return (pos_row|pos_col);
}

/**
 * @brief 		This function serves to set keyscan scan times.
 * @param[in] 	scan_times - 1 is to set triple scan times,0 is to set double scan times.
 * @return 		none
 */
void keyscan_set_scan_times(ks_scan_times_e scan_times)
{
	if(scan_times){
		BM_CLR(reg_ks_en, FLD_KS_TRI_CHK_DIS);
	}else{
		BM_SET(reg_ks_en, FLD_KS_TRI_CHK_DIS);
	}
}
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
void keyscan_init(ks_debounce_period_e debounce_period, unsigned char enter_idle_period_num, ks_scan_times_e scan_times)
{
	/* There are two 32K clocks working in Keyscan, and the FLD_CLK1_KS_32K_EN bit can be turned on at the same time.
	 * One is the scan clock, the other is the clear interrupt clock, and their clock sources are the same.
	 * When Keyscan enters the idle state, the scan clock will be gated, reducing power consumption.*/
	//BM_SET(reg_clk_en1,FLD_CLK1_KS_32K_EN|FLD_CLK1_KS_EN);


	reg_ks_end_flag = KESYCAN_END_FLAG;

	keyscan_set_time(debounce_period, enter_idle_period_num);
	keyscan_set_scan_times(scan_times);

	//enable keyscan clk and set irq mask.If the mask bit is not enabled, the FLD_IRQ_KS status bit will not be set.
	BM_SET(reg_ks_en, FLD_KS_CLK_EN|FLD_KS_IE);
}

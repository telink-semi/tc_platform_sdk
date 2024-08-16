/********************************************************************************************************
 * @file    app.c
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

#define ROW_CNT		2
#define COL_CNT		2
unsigned char g_ks_row[ROW_CNT] = {	KS_PB1, KS_PB0};
unsigned char g_ks_col[COL_CNT] = { KS_PC1,	KS_PC0};
volatile unsigned char g_key_value[256] = {0};
volatile unsigned char g_key_value_rptr = 0;
volatile unsigned char g_key_value_wptr = 0;
volatile unsigned char g_keyscan_error_flag = 0;//1 indicates that the data stored in the interrupt is abnormal.
void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);

    gpio_set_output_en(LED1, 1); //enable output
    gpio_set_output_en(LED2, 1); //enable output

    gpio_set_input_en(LED1, 0); //disable input
    gpio_set_input_en(LED2, 0); //disable input

	//set gpio as to keyscan.
	keyscan_set_martix((unsigned char*)g_ks_row, ROW_CNT, (unsigned char*)g_ks_col, COL_CNT, KS_INT_PIN_PULLDOWN);

	keyscan_init(DEBOUNCE_PERIOD_8MS, 1, TRIPLE_SCAN_TIMES);

	keyscan_enable();

	irq_enable_type(FLD_IRQ_KS_CMD_EN);
	irq_enable();
	printf("\n Keyscan Test Start! Please press button. \n");
}


void main_loop (void)
{
	sleep_ms(500);
	while(1){
		if(g_key_value_rptr == g_key_value_wptr){
			break;
		}
		if(g_key_value[g_key_value_rptr] != KESYCAN_END_FLAG){
			printf("%d row %d column\n",g_key_value[g_key_value_rptr]>>5, g_key_value[g_key_value_rptr]&0x1f);
		}
		g_key_value_rptr = (g_key_value_rptr + 1)&0xff;
	}
	gpio_toggle(LED1);
	gpio_toggle(LED2);
	sleep_ms(200);
#if(KEYSCAN_TEST_SUSPEND)
	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_CORE_KEY_SCAN, 0);
#endif

}





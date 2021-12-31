/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
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
#include "app_config.h"

#define ROW_CNT		2
#define COL_CNT		2
unsigned char g_ks_row[ROW_CNT] = {	KS_PB1, KS_PB0};
unsigned char g_ks_col[COL_CNT] = { KS_PC1,	KS_PC0};
volatile unsigned char g_key_value[256] = {0};
volatile unsigned char g_key_value_rptr = 0;
volatile unsigned char g_key_value_wptr = 0;
volatile unsigned char g_keyscan_error_flag = 0;//1 indicates that the data stored in the interrupt is abnormal.
void user_init()
{
	gpio_set_func(LED1|LED2 ,AS_GPIO);
	gpio_set_output_en(LED1|LED2 , 1); 		//enable output
	gpio_set_input_en(LED1|LED2 ,0);			//disable input


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
	gpio_toggle(LED1|LED2);
	sleep_ms(200);
#if(KEYSCAN_TEST_SUSPEND)
	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_CORE_KEY_SCAN, 0);
#endif

}





/********************************************************************************************************
 * @file    main.c
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

extern unsigned char g_key_value[256];
extern unsigned char g_key_value_wptr;
extern unsigned char g_keyscan_error_flag;
extern void user_init(void);
extern void main_loop (void);

/**
 * @brief		This function serves to handle the interrupt of MCU.
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	if(reg_comb_irq & FLD_IRQ_KS){
		unsigned char rptr = 0;
		unsigned char wptr = 0;
		unsigned char key_val = 0;

		keyscan_clr_irq_status();

		while(1){
			rptr = keyscan_get_rptr();
			wptr = keyscan_get_wptr();
			key_val = keyscan_get_ks_value();
			g_key_value[g_key_value_wptr] = key_val;
			g_key_value_wptr = (g_key_value_wptr + 1)&0xff;
			if(key_val == KESYCAN_END_FLAG){
				break;
			}else if(rptr == wptr){
				g_keyscan_error_flag = 1;
				break;
			}
		}
	}
}

/**
 * @brief		This is main function
 * @return      none
 */
int main (void)   //must on ramcode
{
#if (MCU_CORE_B80 || MCU_CORE_B80B ||MCU_CORE_B89)
#if(KEYSCAN_32K_SOURCE==SOURCE_32K_XTAL)
	blc_pm_select_external_32k_crystal();
#else
	blc_pm_select_internal_32k_crystal();
#endif
#endif

    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}

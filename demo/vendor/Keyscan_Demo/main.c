/********************************************************************************************************
 * @file	main.c
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

extern unsigned char g_key_value[256];
extern unsigned char g_key_value_wptr;
extern unsigned char g_keyscan_error_flag;
extern void user_init();
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
#if (MCU_CORE_B80||MCU_CORE_B89)
#if(KEYSCAN_32K_SOURCE==SOURCE_32K_XTAL)
	blc_pm_select_external_32k_crystal();
#else
	blc_pm_select_internal_32k_crystal();
#endif
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B85)
	cpu_wakeup_init();
#endif


	gpio_init(1);

	clock_init(SYS_CLK);
	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}

/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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


#ifndef DRIVER_BIN_ENABLE


#include "app_config.h"
extern void user_init(void);
extern void main_loop (void);
extern struct app_spi_s * user_api_ptr;
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{

}
/*
 * 1. Background: Only dp/dm encapsulation is available, requiring conversion to USB-to-SWS functionality for access. USB-to-SWS necessitates switching time, which is difficult to operate on the ATE side. Therefore, a test mode must be added to the ROM SDK.
 * 2. Test Module Implementation:
 * Before power-on to software DP configuration pull-up, check if DP/DM are simultaneously high three times. If true, enter test mode（To prevent accidental entry during normal operation, change the while loop to a 3-second timeout.）.
 * 3. Implementation Details:
 * - Power is only supplied when the dongle is inserted, with DM initially low. DM level transition time on Lark is approximately 300+ ms.
 * - In bare die state, DM/DP are floating, hence the triple check.
 * - On Lark, RAM data persists except during power loss (32k watchdog/software reboot).
 * - The reboot_data segment is defined for two reasons:
 *    - To determine if the system is returning from a state other than power-on via the Telink flag written to an SRAM variable. The S-file initializes the data segment with 0xff, which would overwrite this flag.
 *    - To check for a high state immediately after power-on, requiring a variable that cannot reside in the data segment or BSS segment.
 *  The ROM SDK only supports dongle-based planning.
 */
volatile unsigned int __attribute__((section(".reboot_data"))) g_dp_dm_is_high=0;
volatile unsigned int __attribute__((section(".reboot_data"))) g_is_reboot =0;
void dp_dm_clear_flag(void){
	g_dp_dm_is_high=0;
}
void dp_dm_get_input_is_high(void){
	if((gpio_read(GPIO_PA0))&&(gpio_read(GPIO_PA1))){
		g_dp_dm_is_high++;
	}
}
void sys_check_is_enter_test_mode(void){
	if(!(g_is_reboot == 0x544c4e4b)){
		if(g_dp_dm_is_high == 0x03){
		    for(volatile unsigned int i=0; i<0x500000; i++){

            }
		}
	}
	g_is_reboot = 0x544c4e4b;
}
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
	 user_api_ptr = (struct app_spi_s *) load_user_bin(DRIVER_ROM_START_ADDR);
	 dp_dm_get_input_is_high();
	 sys_check_is_enter_test_mode();

	 user_api_ptr->cpu_wakeup_init(INTERNAL_CAP_XTAL24M);
	 user_api_ptr->wd_32k_stop();
     user_api_ptr->clock_init(SYS_CLK);

     user_init();

     while(1){
   	     main_loop();
     }

}
#endif

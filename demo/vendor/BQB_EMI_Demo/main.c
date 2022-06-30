/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "calibration.h"
#if TEST_DEMO==BQB_DEMO
#include "BQB/bqb.h"
#endif

extern void user_init();
extern void main_loop (void);
#if TEST_DEMO==BQB_DEMO && SUPPORT_CONFIGURATION
extern void rd_usr_definition(unsigned char _s);
extern usr_def_t usr_config;
#endif

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ __attribute__((optimize("-Os"))) void irq_handler(void)
{

}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {

	blc_pm_select_internal_32k_crystal();
#if (MCU_CORE_B85) || (MCU_CORE_B87)
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp
#endif

#if TEST_DEMO==BQB_DEMO && SUPPORT_CONFIGURATION

	rd_usr_definition(1);
#if (MCU_CORE_B89 || MCU_CORE_B80)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B87)
	if(usr_config.power_mode == 0)
	{
		cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
	}
	else
	{
		cpu_wakeup_init(DCDC_MODE, EXTERNAL_XTAL_24M);
	}
#elif (MCU_CORE_B85)
	cpu_wakeup_init();
#endif

#else

#if (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B89 || MCU_CORE_B80)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#endif

#endif


#if (MCU_CORE_B85) || (MCU_CORE_B87)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif (MCU_CORE_B89)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_otp_value_calib();
#endif

#if (MCU_CORE_B85 || MCU_CORE_B87)
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);

	gpio_init(!deepRetWakeUp);
#elif(MCU_CORE_B89 || MCU_CORE_B80)
	rf_mode_init();
	rf_set_ble_1M_NO_PN_mode();
#endif
	clock_init(SYS_CLK);

	user_init();

	while (1)
	{
		main_loop ();
	}
	return 0;
}




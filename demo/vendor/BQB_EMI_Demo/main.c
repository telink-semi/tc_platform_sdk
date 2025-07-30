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
#include "calibration.h"
#if TEST_DEMO==BQB_DEMO
#include "BQB/bqb.h"
#endif

extern void user_init(void);
extern void main_loop (void);
#if TEST_DEMO==BQB_DEMO && SUPPORT_CONFIGURATION
extern void rd_usr_definition(unsigned char _s);
extern usr_def_t usr_config;
#endif

#if(MCU_CORE_B87||MCU_CORE_TC321X)
#define POWER_MODE_LDO			0
#define POWER_MODE_DCDC_LDO 	1
#if(MCU_CORE_B87)
#define POWER_MODE_DCDC			2
#endif
/**
 * @brief		This is macro used to set the initialize power mode.
 */
#define POWER_MODE_SELECT		POWER_MODE_LDO
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
#if (MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC1211||MCU_CORE_TC122X)
	cpu_wakeup_init(INTERNAL_CAP_XTAL24M);

#elif(MCU_CORE_TC321X)
	if(usr_config.power_mode == 0)
	{
		cpu_wakeup_init(LDO_MODE, INTERNAL_CAP_XTAL24M);
	}
	else
	{
		cpu_wakeup_init(DCDC_LDO_MODE, INTERNAL_CAP_XTAL24M);
	}
#elif (MCU_CORE_B87)
	if(usr_config.power_mode == 0)
	{
		cpu_wakeup_init(LDO_MODE, INTERNAL_CAP_XTAL24M);
	}
	else
	{
		cpu_wakeup_init(DCDC_MODE, INTERNAL_CAP_XTAL24M);
	}
#elif (MCU_CORE_B85)
	cpu_wakeup_init();
#endif

#else

#if (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87||MCU_CORE_TC321X)
#if POWER_MODE_SELECT == POWER_MODE_LDO
	cpu_wakeup_init(LDO_MODE, INTERNAL_CAP_XTAL24M);
#elif POWER_MODE_SELECT == POWER_MODE_DCDC
	cpu_wakeup_init(DCDC_MODE, INTERNAL_CAP_XTAL24M);
#elif POWER_MODE_SELECT == POWER_MODE_DCDC_LDO
	cpu_wakeup_init(DCDC_LDO_MODE, INTERNAL_CAP_XTAL24M);
#endif
#elif (MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC1211||MCU_CORE_TC122X)
	cpu_wakeup_init(INTERNAL_CAP_XTAL24M);
#endif

#endif

#if(MCU_CORE_B80 || MCU_CORE_B80B ||MCU_CORE_B89 || MCU_CORE_TC321X || MCU_CORE_TC1211||MCU_CORE_TC122X)
	wd_32k_stop();
#endif
#if (MCU_CORE_B85) || (MCU_CORE_B87)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif (MCU_CORE_B89)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_otp_value_calib();
#elif (MCU_CORE_TC1211||MCU_CORE_TC122X)
	user_read_efuse_value_calib();
#endif

#if (MCU_CORE_B85 || MCU_CORE_B87)
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);

	gpio_init(!deepRetWakeUp);
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC1211||MCU_CORE_TC122X)
	rf_mode_init();
	rf_set_ble_1M_NO_PN_mode();
#endif
/**
	===============================================================================
						##### driver sdk firmware protection #####
	===============================================================================
	Flash write protection must be added, the size of the protected area is decided according to the application bin file,
	the principle is as follows:
	1.The program area is protected (to prevent the program area from being erased by mistake);
	2.the program will modify the data area is not protected (if write-protected, each time before erasing the need to unprotect,
	  so that there is a risk that the status register of the flash has been mistakenly rewritten);

	@note if flash protection fails, LED1 lights up long, and keeps while.
	===============================================================================
*/
#if !(MCU_CORE_TC1211||MCU_CORE_TC122X)
	flash_init(0);
#endif

    CLOCK_INIT;

	user_init();

	while (1)
	{
		main_loop ();
	}
	return 0;
}




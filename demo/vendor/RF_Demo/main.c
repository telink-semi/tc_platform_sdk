/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for B85m
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

extern void user_init();
extern void main_loop (void);
extern _attribute_ram_code_sec_noinline_ void irq_rf_handler(void);
extern unsigned int tx_state;
extern unsigned int rx_state;
extern unsigned int timeout_state;


/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */

_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if(RF_MODE==RF_BLE_1M_STX2RX)//1
	unsigned short rf_irq_src = rf_irq_src_get();
	if (rf_irq_src) {
		if (rf_irq_src & FLD_RF_IRQ_TX) {
			tx_state = 1;
			gpio_toggle(LED1);
			rf_irq_clr_src(FLD_RF_IRQ_TX);
		}

		if (rf_irq_src & FLD_RF_IRQ_RX) {
			rx_state = 1;
			gpio_toggle(LED2);
			rf_irq_clr_src(FLD_RF_IRQ_RX);
		}

		if (rf_irq_src & FLD_RF_IRQ_RX_TIMEOUT)
		{
			timeout_state = 1;
			gpio_toggle(LED3);
			rf_irq_clr_src(FLD_RF_IRQ_RX_TIMEOUT);
		}

		if (rf_irq_src & FLD_RF_IRQ_FIRST_TIMEOUT)
		{
			timeout_state = 1;
			gpio_toggle(LED3);
			rf_irq_clr_src(FLD_RF_IRQ_FIRST_TIMEOUT);
		}
	}
#endif
#if (MCU_CORE_B87)
#if(RF_MODE==RF_BLE_SDK_TEST)
	irq_rf_handler();
#endif
#endif
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {

#if (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#endif
#if(MCU_CORE_B80 || MCU_CORE_B80B ||MCU_CORE_B89)
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
#elif (MCU_CORE_B80 || MCU_CORE_B80B)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
#if(PACKAGE_TYPE == OTP_PACKAGE)
	user_read_otp_value_calib();
#elif(PACKAGE_TYPE == FLASH_PACKAGE)
	user_read_flash_value_calib();
#endif
#endif

#if (MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B)
	rf_mode_init();
#endif

#if (MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B)
#if(RF_MODE==RF_BLE_1M)//1
	 rf_set_ble_1M_mode();
#elif(RF_MODE==RF_BLE_1M_NO_PN)//2
	rf_set_ble_1M_NO_PN_mode();
#elif(RF_MODE==RF_BLE_2M)//3
	rf_set_ble_2M_mode();
#elif(RF_MODE==RF_BLE_2M_NO_PN)//4
	rf_set_ble_2M_NO_PN_mode();
#elif(RF_MODE==RF_LR_S2_500K)//5
	rf_set_ble_500K_mode();
#elif(RF_MODE==RF_LR_S8_125K)//6
	rf_set_ble_125K_mode();
#elif((RF_MODE==RF_ZIGBEE_250K))//7
	rf_set_zigbee_250K_mode();
#elif(RF_MODE==RF_PRIVATE_250K)//8
	rf_set_pri_250K_mode();
#elif(RF_MODE==RF_PRIVATE_500K)//9
	rf_set_pri_500K_mode();
#elif(RF_MODE==RF_PRIVATE_1M)
	 rf_set_pri_1M_mode();
#elif(RF_MODE==RF_PRIVATE_2M)
	rf_set_pri_2M_mode();
#elif(RF_MODE==RF_ANT)
	rf_set_ant_mode();
#elif(RF_MODE==RF_BLE_1M_STX2RX)//1
	 rf_set_ble_1M_mode();
#endif
#else
#if(RF_MODE==RF_BLE_2M)
	rf_drv_init(RF_MODE_BLE_2M);
#elif(RF_MODE==RF_BLE_1M)
	rf_drv_init(RF_MODE_BLE_1M);
#elif(RF_MODE==RF_BLE_1M_NO_PN)
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);
#elif(RF_MODE==RF_ZIGBEE_250K)
	rf_drv_init(RF_MODE_ZIGBEE_250K);
#elif(RF_MODE==RF_LR_S2_500K)
	rf_drv_init(RF_MODE_LR_S2_500K);
#elif(RF_MODE==RF_LR_S8_125K)
	rf_drv_init(RF_MODE_LR_S8_125K);
#elif(RF_MODE==RF_PRIVATE_250K)
	rf_drv_init(RF_MODE_PRIVATE_250K);
#elif(RF_MODE==RF_PRIVATE_500K)
	rf_drv_init(RF_MODE_PRIVATE_500K);
#elif(RF_MODE==RF_PRIVATE_1M)
	rf_drv_init(RF_MODE_PRIVATE_1M);
#elif(RF_MODE==RF_PRIVATE_2M)
	rf_drv_init(RF_MODE_PRIVATE_2M);
#elif(RF_MODE==RF_ANT)
	rf_drv_init(RF_MODE_ANT);
#elif(RF_MODE==RF_BLE_1M_STX2RX)//1
	rf_drv_init(RF_MODE_BLE_1M);

#endif
#if (MCU_CORE_B87)
#if(RF_MODE==RF_HYBEE_1M)
	rf_drv_init(RF_MODE_HYBEE_1M);
#elif(RF_MODE==RF_HYBEE_2M)
	rf_drv_init(RF_MODE_HYBEE_2M);
#elif(RF_MODE==RF_HYBEE_500K)
	rf_drv_init(RF_MODE_HYBEE_500K);
#elif(RF_MODE==RF_BLE_SDK_TEST)//1
	rf_drv_init(RF_MODE_BLE_1M);
#endif
#endif

	gpio_init(0);
#endif

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}

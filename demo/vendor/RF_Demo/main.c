/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
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

extern void user_init();
extern void main_loop (void);
extern unsigned int tx_state;
extern unsigned int rx_state;
extern unsigned int timeout_state;


/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */

_attribute_ram_code_ void irq_handler(void)
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

#if (MCU_CORE_B89)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
	rf_mode_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B85)
	cpu_wakeup_init();
#endif


#if (MCU_CORE_B89)
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
#elif(RF_MODE==RF_ZIGBEE_250K)//7
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

	gpio_init(1);
#endif

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}

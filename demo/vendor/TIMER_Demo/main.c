/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
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

int timer0_irq_cnt = 0;
int timer1_irq_cnt = 0;
int timer2_irq_cnt = 0;
int stimer_irq_cnt = 0;
unsigned int gpio_width =0;

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if(TIMER_MODE == TIMER_SYS_CLOCK_MODE)
	if(timer_get_interrupt_status(FLD_TMR_STA_TMR0))
	{
		timer_clear_interrupt_status(FLD_TMR_STA_TMR0); //clear irq status
		timer0_irq_cnt ++;
		gpio_toggle(LED2);
	}
#elif(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	if(timer_get_interrupt_status(FLD_TMR_STA_TMR0))
	{
		timer_clear_interrupt_status(FLD_TMR_STA_TMR0); //clear irq status
		timer0_irq_cnt ++;
		gpio_toggle(LED2);
	}

#elif(TIMER_MODE == TIMER_GPIO_WIDTH_MODE)
	
	if(timer_get_interrupt_status(FLD_TMR_STA_TMR0))
	{
		timer_clear_interrupt_status(FLD_TMR_STA_TMR0); //clear irq status
		gpio_width = reg_tmr0_tick;
		reg_tmr0_tick = 0;
		gpio_toggle(LED2);
	}
#elif(TIMER_MODE == STIMER_MODE)
	if(stimer_get_irq_status())
	{
		stimer_clr_irq_status();            			//clear irq status
		stimer_set_capture_tick(clock_time() + CLOCK_16M_SYS_TIMER_CLK_1S);
		stimer_irq_cnt++;
		gpio_toggle(LED2);
	}
	
#endif
	
}
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{

#if (MCU_CORE_B85)
	cpu_wakeup_init();
#elif (MCU_CORE_B87)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89)
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

	gpio_init(0);

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop();
	}
	return 0;
}


/********************************************************************************************************
 * @file    app_ir.c
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

#if(PWM_MODE==PWM_IR)
/*********************************************************************************
    B85_B87:
    PWM0   :  PA2.  PC1.  PC2.	PD5
    PWM0_N :  PA0.  PB3.  PC4	PD5
    B89_B80:
    reference gpio.h
 *********************************************************************************/

#if (MCU_CORE_B89)
#define PWM_PIN		GPIO_PC2
#define AS_PWMx         PC2_PWM0 
#elif (MCU_CORE_B87||MCU_CORE_B85)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx			AS_PWM0	
#elif (MCU_CORE_B80 || MCU_CORE_B80B)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx         PWM0
#endif
#define PWM_ID					PWM0_ID
#define PWM_PULSE_NUM			4

#define TX_GROUP_NUM			6
volatile unsigned char cnt=0;

_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	if(pwm_get_interrupt_status(PWM_IRQ_PWM0_PNUM)){
		pwm_clear_interrupt_status(PWM_IRQ_PWM0_PNUM);
		cnt++;
        if(cnt==(TX_GROUP_NUM-1))
		{
        	pwm_set_mode(PWM_ID, PWM_COUNT_MODE);
		}
	}

}

void user_init()
{
	sleep_ms(2000);
	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN, AS_PWMx);
	pwm_set_mode(PWM_ID, PWM_IR_MODE);
	pwm_set_pulse_num(PWM_ID,PWM_PULSE_NUM);
	pwm_set_cycle_and_duty(PWM_ID, 1000 * CLOCK_SYS_CLOCK_1US, 500 * CLOCK_SYS_CLOCK_1US);

	pwm_set_interrupt_enable(PWM_IRQ_PWM0_PNUM);
	irq_set_mask(FLD_IRQ_SW_PWM_EN);
	irq_enable();

	pwm_start(PWM_ID);
#if (TX_GROUP_NUM == 1)
	pwm_set_mode(PWM_ID, PWM_COUNT_MODE);
#endif
}

volatile unsigned char n;

void main_loop (void)
{
   sleep_ms(50);
}

#endif



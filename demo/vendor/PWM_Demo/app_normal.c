/********************************************************************************************************
 * @file    app_normal.c
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


#if(PWM_MODE==PWM_NORMAL)

/*********************************************************************************
    B85_B87:
    PWM0   :  PA2.  PC1.  PC2.	PD5
    PWM1   :  PA3.  PC3.
    PWM2   :  PA4.  PC4.
    PWM3   :  PB0.  PD2.
    PWM4   :  PB1.  PB4.
    PWM5   :  PB2.  PB5.
    PWM0_N :  PA0.  PB3.  PC4	PD5
    PWM1_N :  PC1.  PD3.
    PWM2_N :  PD4.
    PWM3_N :  PC5.
    PWM4_N :  PC0.  PC6.
    PWM5_N :  PC7.

    B89_B80_TC321X:
    reference gpio.h

    TC122X:
    PWM0   :  PA0.  PA4.
    PWM1   :  PA1.  PA5.
    PWM2   :  PA2.  PA6.
    PWM3   :  PB0.  PA7.
    PWM4   :  PB1.
    PWM5   :  PB2.
    PWM0_N :  PA1.
    PWM1_N :  PA0.
    PWM2_N :  PB0.
    PWM3_N :  PA2.
    PWM4_N :  PB2.

    TC123X:
    PWM0 ~ PWM5    :  PA0.  PA2.  PA5.  PA7.  PB1.  PB3.  PB5.  PB7.  PC1.
    PWM0_N ~ PWM5_N:  PA1.  PA4.  PA6.  PB0.  PB2.  PB4.  PB6.  PC0.
 *********************************************************************************/

#if (MCU_CORE_B89)
#define PWM_PIN		GPIO_PC2
#define AS_PWMx         PC2_PWM0 
#elif (MCU_CORE_B87||MCU_CORE_B85)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx			AS_PWM0	
#elif (MCU_CORE_B80 || MCU_CORE_B80B|| MCU_CORE_TC321X)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx         PWM0
#elif (MCU_CORE_TC122X)
#define PWM_PIN		GPIO_PA0
#define AS_PWMx         PWM0
#elif (MCU_CORE_TC123X)
#define PWM_PIN1		GPIO_PA0
#define PWM_PIN2		GPIO_PA2
#define AS_PWM0         PWM0
#define AS_PWM1         PWM1
#define PWM_DEAD_ZONE_MODE     1
#endif
#define PWM_ID		PWM0_ID
volatile unsigned char cnt=0;
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	if(pwm_get_interrupt_status(PWM_IRQ_PWM0_FRAME)){
		pwm_clear_interrupt_status(PWM_IRQ_PWM0_FRAME);
		cnt++;
	}
#if PWM_DEAD_ZONE_MODE
	if(pwm_get_interrupt_status(PWM_IRQ_PWM1_FRAME)){
		pwm_clear_interrupt_status(PWM_IRQ_PWM1_FRAME);
		cnt++;
	}
#endif
}

void user_init(void)
{
	sleep_ms(2000);

	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN1, AS_PWM0);
	pwm_set_mode(PWM0_ID, PWM_NORMAL_MODE);
	pwm_set_cycle_and_duty(PWM0_ID, 1000 * CLOCK_SYS_CLOCK_1US, 400 * CLOCK_SYS_CLOCK_1US);

#if PWM_DEAD_ZONE_MODE
	gpio_set_func(PWM_PIN2, AS_PWM1);
	pwm_set_mode(PWM1_ID, PWM_NORMAL_MODE);
	pwm_set_cycle_and_duty(PWM1_ID, 1000 * CLOCK_SYS_CLOCK_1US, 400 * CLOCK_SYS_CLOCK_1US);
#endif

	pwm_set_interrupt_enable(PWM_IRQ_PWM0_FRAME);
#if PWM_DEAD_ZONE_MODE
	pwm_set_interrupt_enable(PWM_IRQ_PWM1_FRAME);
#endif
	irq_set_mask(FLD_IRQ_SW_PWM_EN);
	irq_enable();

#if PWM_DEAD_ZONE_MODE
	pwm_set_phase(PWM0_ID, 300 * CLOCK_SYS_CLOCK_1US);
	pwm_set_phase(PWM1_ID, 800 * CLOCK_SYS_CLOCK_1US);
	pwm_multi_start(FLD_PWM0_EN | FLD_PWM1_EN);
#endif
}

void main_loop (void)
{
	sleep_ms(50);
}

#endif


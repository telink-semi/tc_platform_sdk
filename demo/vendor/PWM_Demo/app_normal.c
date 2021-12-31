/********************************************************************************************************
 * @file	app_normal.c
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
    B89_B80:
    reference gpio.h
 *********************************************************************************/

#if (MCU_CORE_B89)
#define PWM_PIN		GPIO_PC2
#define AS_PWMx         PC2_PWM0 
#elif (MCU_CORE_B87||MCU_CORE_B85)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx			AS_PWM0	
#elif (MCU_CORE_B80)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx         PWM0
#endif
#define PWM_ID		PWM0_ID
volatile unsigned char cnt=0;
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	if(pwm_get_interrupt_status(PWM_IRQ_PWM0_FRAME)){
		pwm_clear_interrupt_status(PWM_IRQ_PWM0_FRAME);
		cnt++;
	}

}

void user_init()
{
	sleep_ms(2000);

	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN, AS_PWMx);
	pwm_set_mode(PWM_ID, PWM_NORMAL_MODE);
	pwm_set_cycle_and_duty(PWM_ID, 1000 * CLOCK_SYS_CLOCK_1US, 500 * CLOCK_SYS_CLOCK_1US);
	pwm_set_interrupt_enable(PWM_IRQ_PWM0_FRAME);
	irq_set_mask(FLD_IRQ_SW_PWM_EN);
	irq_enable();
	pwm_start(PWM_ID);
}

void main_loop (void)
{
	sleep_ms(50);
}

#endif


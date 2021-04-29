/********************************************************************************************************
 * @file	app_ir_dma_fifo.c
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

#if(PWM_MODE==PWM_IR_DMA_FIFO)
/*********************************************************************************
    PWM0   :  PA2.  PC1.  PC2.	PD5
    PWM0_N :  PA0.  PB3.  PC4	PD5
 *********************************************************************************/

#define PWM_PIN					GPIO_PC2
#if (MCU_CORE_B89)
#define AS_PWMx         PC2_PWM0 
#elif (MCU_CORE_B87||MCU_CORE_B85)
#define AS_PWMx			AS_PWM0	
#endif
#define PWM_ID					PWM0_ID

#define IR_DMA_CARRIER_FREQ				38000
#define IR_DMA_MAX_TICK					(CLOCK_SYS_CLOCK_HZ/IR_DMA_CARRIER_FREQ)
#define IR_DMA_CMP_TICK					(IR_DMA_MAX_TICK/2)

#define IR_DMA_SHADOW_CARRIER_FREQ		56000
#define IR_DMA_SHADOW_MAX_TICK			(CLOCK_SYS_CLOCK_HZ/IR_DMA_SHADOW_CARRIER_FREQ)
#define IR_DMA_SHADOW_CMP_TICK			(IR_DMA_SHADOW_MAX_TICK/2)

unsigned short IR_DMA_Buff[64]={0};
unsigned short IRQ_IR_DMA_Buff[64]={0};
volatile unsigned char irq_index=2;
volatile unsigned char cnt=0;
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
	if(pwm_get_interrupt_status(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE)){
		pwm_clear_interrupt_status(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);
		cnt++;
		IRQ_IR_DMA_Buff[irq_index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
		IRQ_IR_DMA_Buff[irq_index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 1690 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
		unsigned int irq_length = irq_index*2 - 4;//The first four bytes are data length bytes, not included in the actual length to be sent
		unsigned char* irq_buff = &IRQ_IR_DMA_Buff[0];
		//The maximum length that the PWM can send is 511bytes
		irq_buff[0]= irq_length&0xff;
		irq_buff[1]= (irq_length>>8)&0xff;
		irq_buff[2]= (irq_length>>16)&0xff;
		irq_buff[3]= (irq_length>>24)&0xff;
		pwm_set_dma_address(&IRQ_IR_DMA_Buff);
		pwm_start_dma_ir_sending();
		irq_index=2;
    }

}

void user_init()
{
	sleep_ms(2000);

	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN, AS_PWMx);
	pwm_set_mode(PWM_ID, PWM_IR_DMA_FIFO_MODE);
	pwm_set_cycle_and_duty(PWM_ID, IR_DMA_MAX_TICK, IR_DMA_CMP_TICK);
	pwm_set_pwm0_shadow_cycle_and_duty(IR_DMA_SHADOW_MAX_TICK,IR_DMA_SHADOW_CMP_TICK);
	unsigned char index=2;
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 9000 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 4500 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 1690 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);

	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 9000 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 4500 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 1690 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);

	unsigned int length = index*2 - 4;//The first four bytes are data length bytes, not included in the actual length to be sent
	unsigned char* buff = &IR_DMA_Buff[0];
	buff[0]= length&0xff;
	buff[1]= (length>>8)&0xff;
	buff[2]= (length>>16)&0xff;
	buff[3]= (length>>24)&0xff;
	pwm_set_dma_address(&IR_DMA_Buff);

	pwm_set_interrupt_enable(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);
	irq_set_mask(FLD_IRQ_SW_PWM_EN);
	irq_enable();

	pwm_start_dma_ir_sending();
}

void main_loop (void)
{
	sleep_ms(50);
}

#endif

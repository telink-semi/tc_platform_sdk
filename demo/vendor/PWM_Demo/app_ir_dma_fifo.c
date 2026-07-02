/********************************************************************************************************
 * @file    app_ir_dma_fifo.c
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
#include "app_config.h"

#if(PWM_MODE==PWM_IR_DMA_FIFO)
/*********************************************************************************
    B85_B87:
    PWM0   :  PA2.  PC1.  PC2.	PD5
    PWM0_N :  PA0.  PB3.  PC4	PD5
    B89_B80_TC321X:
    reference gpio.h    TC122X:
    PWM0   :  PA0.  PA4.
    PWM0_N :  PA1.
 *********************************************************************************/

#if (MCU_CORE_B89)
#define PWM_PIN		GPIO_PC2
#define AS_PWMx         PC2_PWM0 
#elif (MCU_CORE_B87||MCU_CORE_B85)
#define PWM_PIN		GPIO_PC1
#define AS_PWMx			AS_PWM0	

#elif (MCU_CORE_B80 || MCU_CORE_B80B|| MCU_CORE_TC321X)
#define PWM_PIN		GPIO_PA2
#define AS_PWMx         PWM0

#if (MCU_CORE_TC321X)
#define DMA_NORMAL_MODE     1
#define DMA_PINGPONG_MODE   2
#define DMA_MODE            DMA_NORMAL_MODE
#endif

#elif (MCU_CORE_TC122X)||(MCU_CORE_TC123X)
#define PWM_PIN		GPIO_PA0
#define AS_PWMx         PWM0
#endif
#define PWM_ID					PWM0_ID

#define IR_DMA_CARRIER_FREQ				38000//38K Hz ~=26.32us
#define IR_DMA_MAX_TICK					(CLOCK_SYS_CLOCK_HZ/IR_DMA_CARRIER_FREQ)
#define IR_DMA_CMP_TICK					(IR_DMA_MAX_TICK/2)

#define IR_DMA_SHADOW_CARRIER_FREQ		56000
#define IR_DMA_SHADOW_MAX_TICK			(CLOCK_SYS_CLOCK_HZ/IR_DMA_SHADOW_CARRIER_FREQ)
#define IR_DMA_SHADOW_CMP_TICK			(IR_DMA_SHADOW_MAX_TICK/2)

#if(DMA_MODE == DMA_NORMAL_MODE)
unsigned short IR_DMA_Buff[64]={0};
unsigned short IRQ_IR_DMA_Buff[64]={0};
volatile unsigned char irq_index=2;
volatile unsigned char cnt=0;

#elif(DMA_MODE == DMA_PINGPONG_MODE)
unsigned short IR_DMA_Buff[528]={0};//The array IR_DMA_Buff must be configured with 528 or more in DMA_PINGPONG_MODE, otherwise it may cause an overflow.
volatile unsigned char pingpong0_done_flag = 0;
volatile unsigned char pingpong1_done_flag = 0;
volatile unsigned char irq_cnt=0;

typedef struct {
    unsigned short *buf;     //buffer starting address
    unsigned int size;       // Data length (bytes)
    unsigned short cfg_num;  //Number of fifo_waveform configuration
} dma_chain_t;

/**
 * @brief IR_DMA_Buff Memory layout diagram
 * |                  |                  |                         |                  |                  |
 * | :--------------- | :--------------- | :---------------------- | :--------------- | :--------------- |
 * | byte <1056:532>  | byte <531:528>   |     byte <527:512>      |   byte <511:4>   |    byte <4:0>    |
 * | chain 1 cfg data | chain 1 length   | reserve(can not be use) | chain 0 cfg data |  chain 0 length  |
 */
static dma_chain_t dma_chain[2] = {
    {&IR_DMA_Buff[2], 0, 0},  //Chain 0: Data starts at index 2, length 0-1
    {&IR_DMA_Buff[266], 0, 0} //Chain 1: Data starts at index 266, length 264-265
};

#endif
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if(DMA_MODE == DMA_NORMAL_MODE)
	if(pwm_get_interrupt_status(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE)){
		pwm_clear_interrupt_status(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);
		cnt++;
		IRQ_IR_DMA_Buff[irq_index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 560 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);
		IRQ_IR_DMA_Buff[irq_index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 1690 * CLOCK_SYS_CLOCK_1US/IR_DMA_SHADOW_MAX_TICK);
		unsigned int irq_length = irq_index*2 - 4;//The first four bytes are data length bytes, not included in the actual length to be sent
		unsigned char* irq_buff = (unsigned char*)&IRQ_IR_DMA_Buff[0];
		//The maximum length that the PWM can send is 511bytes
		irq_buff[0]= irq_length&0xff;
		irq_buff[1]= (irq_length>>8)&0xff;
		irq_buff[2]= (irq_length>>16)&0xff;
		irq_buff[3]= (irq_length>>24)&0xff;
		pwm_set_dma_address(&IRQ_IR_DMA_Buff);
		pwm_start_dma_ir_sending();
		irq_index=2;
    }
#elif(DMA_MODE == DMA_PINGPONG_MODE)
    if(pwm_get_interrupt_status(PWM_IRQ_PWM0_PNUM))
	    {
	        pwm_clear_interrupt_status(PWM_IRQ_PWM0_PNUM);
	        gpio_toggle(LED1);
	        const unsigned char curr_chain = pingpong0_done_flag ? 1 : 0;//the current activity chain
	        dma_chain_t *curr = &dma_chain[curr_chain];
	        irq_cnt++;

	        //Start the next chain at the second-to-last interrupt
	        if(irq_cnt == curr->cfg_num - 1)
	        {
	            const unsigned char next_chain = 1 - curr_chain;
	            //Initiate DMA transfer for the next chain
	            if(next_chain == 0) {
	            	pwm_dma_pingpong_chain_start(0);
	            } else {
	            	pwm_dma_pingpong_chain_start(1);
	            }
	            gpio_toggle(LED2);//Indicates the next chain has been configured.
	        }
	        else if(irq_cnt == curr->cfg_num)//the current chain is complete.
	        {
	            pingpong0_done_flag = (curr_chain == 0) ? 1 : 0;
	            pingpong1_done_flag = (curr_chain == 1) ? 1 : 0;
	            irq_cnt = 0;
	            gpio_toggle(LED3);//Indicates the next chain has been started.
	        }
	    }
#endif
}

void user_init(void)
{
	sleep_ms(2000);

    gpio_set_func(LED1,AS_GPIO);
    gpio_set_output_en(LED1, 1);        //enable output
    gpio_set_input_en(LED1,0);          //disable input
    gpio_write(LED1, 0);

    gpio_set_func(LED2,AS_GPIO);
    gpio_set_output_en(LED2, 1);        //enable output
    gpio_set_input_en(LED2,0);          //disable input
    gpio_write(LED2, 0);

    gpio_set_func(LED3,AS_GPIO);
    gpio_set_output_en(LED3, 1);        //enable output
    gpio_set_input_en(LED3,0);          //disable input
    gpio_write(LED3, 0);

	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN, AS_PWMx);
	pwm_set_mode(PWM_ID, PWM_IR_DMA_FIFO_MODE);
	pwm_set_cycle_and_duty(PWM_ID, IR_DMA_MAX_TICK, IR_DMA_CMP_TICK);//26.26us 13.13us
	pwm_set_pwm0_shadow_cycle_and_duty(IR_DMA_SHADOW_MAX_TICK,IR_DMA_SHADOW_CMP_TICK);


#if(DMA_MODE == DMA_NORMAL_MODE)
	unsigned char index=2;
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 9000 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//342 pulse num carrier,342*26.26us ~=9ms
	IR_DMA_Buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 4500 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//171 pulse num not carrier,171*26.26us ~=4.5ms
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
	unsigned char* buff = (unsigned char*)&IR_DMA_Buff[0];
	buff[0]= length&0xff;
	buff[1]= (length>>8)&0xff;
	buff[2]= (length>>16)&0xff;
	buff[3]= (length>>24)&0xff;
	pwm_set_dma_address(&IR_DMA_Buff);

	pwm_set_interrupt_enable(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);

#elif(DMA_MODE == DMA_PINGPONG_MODE)

	 //Initialize chain 0 waveform data
	 unsigned short *ptr0 = dma_chain[0].buf;
	 unsigned short *start_ptr0 = ptr0;
	 *ptr0++= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 2632 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//100 pulse num carrier,38K Hz ~=26.32us,100*26.32us=2632us
	 *ptr0++= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 4500 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//171 pulse num not carrier,38K Hz ~=26.32us,171*26.32us ~=4500us
	 *ptr0++= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 5264 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//200 pulse num carrier,38K Hz ~=26.32us,200*26.32us=5264us
	 //you can continue to add fifo_waveform, dma_chain[0] can be configured with a maximum of 255 groups, i.e., 510byte

	 dma_chain[0].cfg_num = ptr0 - start_ptr0;
	 dma_chain[0].size = dma_chain[0].cfg_num * sizeof(short);
	 //Set the Chain 0 DMA Length(bytes)
	 IR_DMA_Buff[0] = dma_chain[0].size & 0xFFFF;
	 IR_DMA_Buff[1] = (dma_chain[0].size >> 16) & 0xFFFF;

	 //Initialize chain 1 waveform data
	 unsigned short *ptr1 = dma_chain[1].buf;
	 unsigned short *start_ptr1 = ptr1;
	 *ptr1++= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 7896 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//300 pulse num carrier,38K Hz ~=26.32us,300*26.32us=7896us
	 *ptr1++= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 9000 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//342 pulse num not carrier,38K Hz ~=26.32us,342*26.32us ~=9000us
	 *ptr1++= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 10528 * CLOCK_SYS_CLOCK_1US/IR_DMA_MAX_TICK);//400 pulse num carrier,38K Hz ~=26.32us,400*26.32us=10528us
	 //you can continue to add fifo_waveform, dma_chain[1] can be configured with a maximum of 255 groups, i.e., 510byte

	 dma_chain[1].cfg_num = ptr1 - start_ptr1;
	 dma_chain[1].size = dma_chain[1].cfg_num * sizeof(short);
	 //Set the Chain 1 DMA Length(bytes)
	 IR_DMA_Buff[264] = dma_chain[1].size & 0xFFFF;
	 IR_DMA_Buff[265] = (dma_chain[1].size >> 16) & 0xFFFF;

	 pwm_set_dma_address_pingpong(&IR_DMA_Buff);
	 pwm_set_interrupt_enable(PWM_IRQ_PWM0_PNUM);

#endif
	irq_set_mask(FLD_IRQ_SW_PWM_EN);
	irq_enable();
	pwm_start_dma_ir_sending();

}

void main_loop (void)
{
	sleep_ms(50);
}

#endif

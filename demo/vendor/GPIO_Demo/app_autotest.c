/********************************************************************************************************
 * @file	app_autotest.c
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

#if ((GPIO_MODE == AUTO_TEST_MODE) && (MCU_CORE_B80 || MCU_CORE_B80B))

#include "../common/auto_test/pc_interface.h"
#include "../common/auto_test/dut_cmd.h"

#define    COMMAND_BUFF_SIZE    32   // 1Byte
#define    COMMAND_BUFF_CNT     4	 // 1Byte
#define    RESULT_BUFF_SIZE     64	 // 1Byte
#define    RESULT_BUFF_CNT      4	 // 1Byte

unsigned char command_buff[COMMAND_BUFF_CNT][COMMAND_BUFF_SIZE]; // receive CMD and parameter
unsigned char result_buff[RESULT_BUFF_CNT][RESULT_BUFF_SIZE];    // send the result
unsigned char para_buff[COMMAND_BUFF_SIZE] = {0};

void gpio_input_test(void);
void gpio_output_low_test(void);
void gpio_output_high_test(void);
void gpio_irq_test(void);
void gpio_irq_risc0_test(void);
void gpio_irq_risc1_test(void);
void gpio_irq_risc2_test(void);
void gpio_irq_new_risc_test(void);
void gpio_irq_report_test(void);

volatile unsigned int gpio_irq_cnt = 0;
volatile unsigned int gpio_irq_risc0_cnt = 0;
volatile unsigned int gpio_irq_risc1_cnt = 0;
volatile unsigned int gpio_irq_risc2_cnt = 0;
volatile unsigned int gpio_irq_new_risc3_cnt = 0;
volatile unsigned int gpio_irq_new_risc4_cnt = 0;
volatile unsigned int gpio_irq_new_risc5_cnt = 0;
volatile unsigned int gpio_irq_new_risc6_cnt = 0;
volatile unsigned int gpio_irq_new_risc7_cnt = 0;
volatile unsigned int gpio_irq_new_risc_cnt = 0;

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
    if((reg_irq_src & FLD_IRQ_GPIO_EN) == FLD_IRQ_GPIO_EN)
    {
        reg_irq_src = FLD_IRQ_GPIO_EN; // clear the relevant irq
        gpio_irq_cnt++;
    }

    else if((reg_irq_src & FLD_IRQ_GPIO_RISC0_EN) == FLD_IRQ_GPIO_RISC0_EN)
    {
        reg_irq_src = FLD_IRQ_GPIO_RISC0_EN; // clear the relevant irq
        gpio_irq_risc0_cnt++;
    }

    else if((reg_irq_src & FLD_IRQ_GPIO_RISC1_EN) == FLD_IRQ_GPIO_RISC1_EN)
    {
        reg_irq_src = FLD_IRQ_GPIO_RISC1_EN; // clear the relevant irq
        gpio_irq_risc1_cnt++;
    }

    else if((reg_irq_src & FLD_IRQ_GPIO_RISC2_EN) == FLD_IRQ_GPIO_RISC2_EN)
    {
        reg_irq_src = FLD_IRQ_GPIO_RISC2_EN; // clear the relevant irq
        gpio_irq_risc2_cnt++;
    }

    else if((reg_irq_src & FLD_IRQ_GPIO_NEW_EN) == FLD_IRQ_GPIO_NEW_EN)
    {
    	unsigned char gpio_irqsrc = reg_gpio_irq_from_pad;
		if(gpio_irqsrc)
		{
			if (gpio_irqsrc & GPIO_NEW_RISC3_IRQ)
			{
				reg_gpio_irq_from_pad = GPIO_NEW_RISC3_IRQ;
				gpio_irq_new_risc3_cnt++;
			}
			 if (gpio_irqsrc & GPIO_NEW_RISC4_IRQ)
			{
				reg_gpio_irq_from_pad = GPIO_NEW_RISC4_IRQ;
				gpio_irq_new_risc4_cnt++;
			}
			 if (gpio_irqsrc & GPIO_NEW_RISC5_IRQ)
			{
				reg_gpio_irq_from_pad = GPIO_NEW_RISC5_IRQ;
				gpio_irq_new_risc5_cnt++;
			}
			 if (gpio_irqsrc & GPIO_NEW_RISC6_IRQ)
			{
				reg_gpio_irq_from_pad = GPIO_NEW_RISC6_IRQ;
				gpio_irq_new_risc6_cnt++;
			}
			 if (gpio_irqsrc & GPIO_NEW_RISC7_IRQ)
			{
				reg_gpio_irq_from_pad = GPIO_NEW_RISC7_IRQ;
				gpio_irq_new_risc7_cnt++;
			}
		}
    }
}

void gpio_input_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,0);
    gpio_set_input_en(pin,1);
	unsigned char pin_level = gpio_read(pin);
	unsigned char send_data[4];
	send_data[0] = (DUTCMD_GPIO_INPUT>>8)&0xff;
	send_data[1] = DUTCMD_GPIO_INPUT&0xff;
	send_data[2] = 4;
	send_data[3] = pin_level;
	respond_data(send_data,send_data[2]);
}

void gpio_output_low_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,1);
    gpio_set_input_en(pin,0);
	gpio_write(pin,0);
	respond_name(DUTCMD_GPIO_OUTPUT_LOW);
}

void gpio_output_high_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,1);
    gpio_set_input_en(pin,0);
	gpio_write(pin,1);
	respond_name(DUTCMD_GPIO_OUTPUT_HIGH);
}

void gpio_irq_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    unsigned char value = para_buff[5];
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,0);
    gpio_set_input_en(pin,1);
    if(0 == value)
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
        gpio_set_interrupt(pin, POL_RISING);	//When switching PA7, the rising edge triggers an interrupt.
    }
    else
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
        gpio_set_interrupt(pin, POL_FALLING);	//When switching PA7, the rising edge triggers an interrupt.
    }

    irq_enable();
    respond_name(DUTCMD_GPIO_IRQ);
    gpio_irq_cnt = 0;
}

void gpio_irq_risc0_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    unsigned char value = para_buff[5];
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,0);
    gpio_set_input_en(pin,1);
    if(0 == value)
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
        gpio_set_interrupt_risc0(pin, POL_RISING);
    }
    else
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
        gpio_set_interrupt_risc0(pin, POL_FALLING);
    }
    irq_enable();
    respond_name(DUTCMD_GPIO_IRQ_RISC0);
    gpio_irq_risc0_cnt = 0;
}

void gpio_irq_risc1_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    unsigned char value = para_buff[5];
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,0);
    gpio_set_input_en(pin,1);
    if(0 == value)
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
        gpio_set_interrupt_risc1(pin, POL_RISING);
    }
    else
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
        gpio_set_interrupt_risc1(pin, POL_FALLING);
    }
    irq_enable();
    respond_name(DUTCMD_GPIO_IRQ_RISC1);
    gpio_irq_risc1_cnt = 0;
}

void gpio_irq_risc2_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    unsigned char value = para_buff[5];
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,0);
    gpio_set_input_en(pin,1);
    if(0 == value)
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
        gpio_set_interrupt_risc2(pin, POL_RISING);
    }
    else
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
        gpio_set_interrupt_risc2(pin, POL_FALLING);
    }
    irq_enable();
    respond_name(DUTCMD_GPIO_IRQ_RISC2);
    gpio_irq_risc2_cnt = 0;
}

void gpio_irq_new_risc_test(void)
{
	GPIO_PinTypeDef pin = (para_buff[3]<< 8 | (para_buff[4])) & 0xfff;
    unsigned char value = para_buff[5];
    gpio_set_func(pin,AS_GPIO);
    gpio_set_output_en(pin,0);
    gpio_set_input_en(pin,1);
    if(0 == value)
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_RISING_EDGE,RISC3);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_RISING_EDGE,RISC4);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_RISING_EDGE,RISC5);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_RISING_EDGE,RISC6);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_RISING_EDGE,RISC7);

    }
    else
    {
    	gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_FALLING_EDGE,RISC3);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_FALLING_EDGE,RISC4);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_FALLING_EDGE,RISC5);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_FALLING_EDGE,RISC6);
        gpio_set_interrupt_new_risc(pin, SRC_IRQ_FALLING_EDGE,RISC7);
    }
    irq_enable();
    respond_name(DUTCMD_GPIO_IRQ_NEW_RISC);
    gpio_irq_new_risc3_cnt = 0;
	gpio_irq_new_risc4_cnt = 0;
	gpio_irq_new_risc5_cnt = 0;
	gpio_irq_new_risc6_cnt = 0;
	gpio_irq_new_risc7_cnt = 0;

}

void gpio_irq_report_test(void)
{
	unsigned char send_data[12];
	send_data[0] =  (DUTCMD_GPIO_IRQ_REPORT>>8)&0xff;
	send_data[1] =  DUTCMD_GPIO_IRQ_REPORT&0xff;
	send_data[2] =  12;
	send_data[3] =  gpio_irq_cnt;
	send_data[4] =  gpio_irq_risc0_cnt;
	send_data[5] =  gpio_irq_risc1_cnt;
	send_data[6] =  gpio_irq_risc2_cnt;
	send_data[7] =  gpio_irq_new_risc3_cnt;
	send_data[8] =  gpio_irq_new_risc4_cnt;
	send_data[9] =  gpio_irq_new_risc5_cnt;
	send_data[10] =  gpio_irq_new_risc6_cnt;
	send_data[11] =  gpio_irq_new_risc7_cnt;
	respond_data(send_data,send_data[2]);
    gpio_irq_new_risc3_cnt = 0;
	gpio_irq_new_risc4_cnt = 0;
	gpio_irq_new_risc5_cnt = 0;
	gpio_irq_new_risc6_cnt = 0;
	gpio_irq_new_risc7_cnt = 0;
}

struct PC_CMD_FUNC{
    unsigned short cmd_name;
    void (*func)(void);
};

// para_buff[1:0]: cmd_name  para_buff[2]: data_length(not include CRC)
struct PC_CMD_FUNC pc_cmd_func[] = {
	{DUTCMD_GPIO_INPUT, gpio_input_test},
	{DUTCMD_GPIO_OUTPUT_LOW, gpio_output_low_test},
	{DUTCMD_GPIO_OUTPUT_HIGH, gpio_output_high_test},
    {DUTCMD_GPIO_IRQ, gpio_irq_test},
    {DUTCMD_GPIO_IRQ_RISC0, gpio_irq_risc0_test},
    {DUTCMD_GPIO_IRQ_RISC1, gpio_irq_risc1_test},
    {DUTCMD_GPIO_IRQ_RISC2, gpio_irq_risc2_test},
    {DUTCMD_GPIO_IRQ_NEW_RISC, gpio_irq_new_risc_test},
    {DUTCMD_GPIO_IRQ_REPORT, gpio_irq_report_test},
};

void user_init(void)
{
	// initiate parameter buffer to receive CMD and parameter
	para_buff_init((unsigned int )command_buff,COMMAND_BUFF_SIZE,COMMAND_BUFF_CNT);
	// initiate result buffer to send the result
	result_buff_init((unsigned int )result_buff,RESULT_BUFF_SIZE,RESULT_BUFF_CNT);
}

volatile unsigned int correct_cmd_count = 0;
volatile unsigned int cmd_count = 0;

void pc_data_handle(void)
{
	unsigned short cmd = 0;
	unsigned char  para_len;
	unsigned short crc;
	unsigned char  i;
	if(para_buff_have_data() != 0){
		cmd_count++;
		// para_buff[1:0]: cmd_name  para_buff[2]: data_length(not include CRC)
		para_buff_read(para_buff,COMMAND_BUFF_SIZE);
		cmd = para_buff[1] | (para_buff[0] << 8);
		para_len = para_buff[2];
		if((para_len+2) <= COMMAND_BUFF_SIZE){
			crc = crc_16(para_buff,para_len);
			if((para_buff[para_len] == ((crc>>8)&0xff)) && (para_buff[para_len+1] == (crc&0xff))) {
				for(i = 0; i < sizeof(pc_cmd_func)/sizeof(struct PC_CMD_FUNC); i++) {
					if(pc_cmd_func[i].cmd_name == cmd) {
						correct_cmd_count++;
						pc_cmd_func[i].func();
						break;
					}
				}
			}
		}
	}
}

void main_loop(void)
{
    while(1){
        pc_data_handle();
    }
}
#endif

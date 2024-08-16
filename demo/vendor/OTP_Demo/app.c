/********************************************************************************************************
 * @file    app.c
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
extern void otp_test_mode_en(void);
extern void otp_test_mode_dis(void);
#include "app_config.h"
unsigned char write_buff[16]  = {0x11,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
unsigned char read_buff[16] = {0};
unsigned char read_init_margin_buff[16] ={0};
unsigned char read_pgm_margin_buff[16]={0};
unsigned char loop=0;
extern unsigned char	otp_program_flag;
volatile unsigned int flag = 0;

#define OTP_TEST        1 //using the otp test area,only for internal testing.
#define OTP_COMMON      2
#define OTP_TEST_MODE   OTP_COMMON

void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);
    gpio_set_func(LED3, AS_GPIO);
    gpio_set_func(LED4, AS_GPIO);

    gpio_set_output_en(LED1, 1); //enable output
    gpio_set_output_en(LED2, 1); //enable output
    gpio_set_output_en(LED3, 1); //enable output
    gpio_set_output_en(LED4, 1); //enable output

    gpio_set_input_en(LED1, 0); //disable input
    gpio_set_input_en(LED2, 0); //disable input
    gpio_set_input_en(LED3, 0); //disable input
    gpio_set_input_en(LED4, 0); //disable input

    gpio_write(LED1, 0); //LED OFF
    gpio_write(LED2, 0); //LED OFF
    gpio_write(LED3, 0); //LED OFF
    gpio_write(LED4, 0); //LED OFF

if(otp_program_flag==0){
	otp_set_active_mode();
}

#if(OTP_TEST_MODE == OTP_TEST )
	otp_test_mode_en();
    otp_write(0x00,4,(unsigned int *)write_buff);
	otp_read(0x00, 4, (unsigned int *)read_buff);
#if(MCU_CORE_B89)
	pm_set_dcdc_output(DCDC_3P0V);
#endif
	otp_initial_margin_read(0x00, 4, (unsigned int *)read_init_margin_buff);
	otp_pgm_margin_read(0x00, 4, (unsigned int *)read_pgm_margin_buff);
#if(MCU_CORE_B89)
	pm_set_dcdc_output(DCDC_2P5V);
#endif
	otp_test_mode_dis();
#elif(OTP_TEST_MODE == OTP_COMMON )
	otp_write(0x3e00,4,(unsigned int *)write_buff);
	otp_read(0x3e00, 4, (unsigned int *)read_buff);
#if(MCU_CORE_B89)
	pm_set_dcdc_output(DCDC_3P0V);
#endif
	otp_initial_margin_read(0x3e00, 4, (unsigned int *)read_init_margin_buff);
	otp_pgm_margin_read(0x3e00, 4, (unsigned int *)read_pgm_margin_buff);
#if(MCU_CORE_B89)
	pm_set_dcdc_output(DCDC_2P5V);
#endif
#endif

	unsigned char flag_debug = 0;

	for(unsigned int i = 0; i < 16; i++)
	{
		if(write_buff[i] != read_buff[i])
		{
			gpio_toggle(LED2);
			flag_debug = 1;
			break;
		}
	}



	for(unsigned int i = 0; i < 16; i++)
	{
		if(write_buff[i] != read_init_margin_buff[i])
		{
			gpio_toggle(LED3);
			flag_debug = 1;
			break;
		}
	}
	for(unsigned int i = 0; i < 16; i++)
	{
		if(write_buff[i] != read_pgm_margin_buff[i])
		{
			gpio_toggle(LED4);
			flag_debug = 1;
			break;
		}
	}
	if(0 == flag_debug)
		gpio_toggle(LED1);

}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	loop++;
}




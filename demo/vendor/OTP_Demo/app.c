/********************************************************************************************************
 * @file	app.c
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
extern void otp_test_mode_en(void);
extern void otp_test_mode_dis(void);
#include "app_config.h"
unsigned char write_buff[16]  = {0x11,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
unsigned char read_buff[16] = {0};
unsigned char read_init_margain_buff[16] ={0};
unsigned char read_pgm_margain_buff[16]={0};
unsigned char loop=0;
extern unsigned char	otp_program_flag;
volatile unsigned int flag = 0;

#define OTP_TEST        1 //using the otp test area,only for internal testing.
#define OTP_COMMON      2
#define OTP_TEST_MODE   OTP_COMMON

void user_init()
{
	gpio_set_func(LED1 | LED2 | LED3 | LED4 ,AS_GPIO);
	gpio_set_output_en(LED1 | LED2 | LED3 | LED4, 1); 		//enable output
	gpio_set_input_en(LED1 | LED2 | LED3 | LED4 ,0);		//disable input
	gpio_write(LED1 | LED2 | LED3 | LED4, 0);              	//LED On
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
	otp_initial_margin_read(0x00, 4, (unsigned int *)read_init_margain_buff);
	otp_pgm_margin_read(0x00, 4, (unsigned int *)read_pgm_margain_buff);
#if(MCU_CORE_B89)
	pm_set_dcdc_output(DCDC_2P5V);
#endif
	otp_test_mode_dis();
#elif(OTP_TEST_MODE == OTP_COMMON )
	otp_write(0x6000,4,(unsigned int *)write_buff);
	otp_read(0x6000, 4, (unsigned int *)read_buff);
#if(MCU_CORE_B89)
	pm_set_dcdc_output(DCDC_3P0V);
#endif
	otp_initial_margin_read(0x6000, 4, (unsigned int *)read_init_margain_buff);
	otp_pgm_margin_read(0x6000, 4, (unsigned int *)read_pgm_margain_buff);
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
		if(write_buff[i] != read_init_margain_buff[i])
		{
			gpio_toggle(LED3);
			flag_debug = 1;
			break;
		}
	}
	for(unsigned int i = 0; i < 16; i++)
	{
		if(write_buff[i] != read_pgm_margain_buff[i])
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




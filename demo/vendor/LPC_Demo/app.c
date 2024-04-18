/********************************************************************************************************
 * @file    app.c
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


#define OUTPUT_PIN   GPIO_PC0

unsigned char result=0;


void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on
	gpio_set_func(OUTPUT_PIN,AS_GPIO);
	gpio_set_input_en(OUTPUT_PIN,0);
	gpio_set_output_en(OUTPUT_PIN,1);
	gpio_write(OUTPUT_PIN,1);

	lpc_power_on(); //power on
	lpc_set_input_chn(LPC_INPUT_PB2); //select input channel PB2
	lpc_set_input_ref(LPC_LOWPOWER,LPC_LOWPOWER_REF_810MV);//select mode and reference
	lpc_set_scaling_coeff(LPC_SCALING_PER50);//select scalign coefficient
}

void main_loop (void)
{
	result =  lpc_get_result();

	gpio_write(OUTPUT_PIN,result);
}


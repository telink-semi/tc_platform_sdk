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

unsigned char dat[5]={0};

void user_init(void)
{

	sleep_ms(2000);

	//For indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On
	mdec_init(GPIO_PB7_MDEC);
}

void main_loop (void)
{
	if(1 == mdec_read_dat(dat))
	{
		gpio_toggle(LED1);
		sleep_ms(2000);
	}
}




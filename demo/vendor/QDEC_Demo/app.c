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

#define QDEC_CHA 	GPIO_PB6
#define QDEC_CHB 	GPIO_PB7

volatile signed int total_count;
volatile signed char qdec_count = 0;
volatile unsigned int pol = 0x100;

void user_init()
{
	gpio_set_func(QDEC_CHA, AS_GPIO);
	gpio_set_output_en(QDEC_CHA,0);
	gpio_set_input_en(QDEC_CHA,1);

	gpio_set_func(QDEC_CHB, AS_GPIO);
	gpio_set_output_en(QDEC_CHB,0);
	gpio_set_input_en(QDEC_CHB,1);

	qdec_clk_en ();
	qdec_set_mode(DOUBLE_ACCURACY_MODE);
	qdec_set_pin(PB6A,PB7A);

	qdec_set_debouncing(1);   //set debouncing
}

void main_loop (void)
{
	qdec_count = qdec_get_count_value();
	if((qdec_count >> 7) == 0x01)
	   total_count -= (pol-qdec_count);
	else
	   total_count += qdec_count;
	printf(" total_count: %d \t", total_count);
	printf(" qdec_count: %d \r\n", qdec_count);
	sleep_ms(1000);
}




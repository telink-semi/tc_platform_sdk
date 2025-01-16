/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "common.h"

extern int main_coremark (void);

extern int coremark_result;
unsigned char cpu_mhz;
/* Note:  This gets the current main frequency of the running cores,
if you find that the score data is not correct you can check this parameter.*/

void user_init(void)
{
    gpio_set_func(LED2,AS_GPIO);
    gpio_set_output_en(LED2,1);
    gpio_set_input_en(LED2,0);

	printf ("\r\n Core Mark Starts(wait about 10s~20s...) ...\r\n");
	sleep_ms (100);
	main_coremark ();
	printf("coremark result = %d \r\n",coremark_result);
	printf("coremark result/clk(Mhz) = %d \r\n",(coremark_result*1000/(CLOCK_SYS_CLOCK_HZ/1000000)));
    /*
     * The Raptor does not support floating-point calculations.
     * To ensure the accuracy of the calculation results, the final value of coremark_result is scaled by a factor of 1000.
     */

}

void main_loop(void)
{
    sleep_ms(200);
    gpio_toggle(LED2);
}


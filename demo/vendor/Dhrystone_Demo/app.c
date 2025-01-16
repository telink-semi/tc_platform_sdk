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

void user_init(void)
{
	CLOCK_INIT;
#if CURRENT_PER_MHZ_TEST
    gpio_shutdown(GPIO_ALL);
    dhry_main();
#else
    printf ("\r\n\r\n Drystone Benchmark %d Starts ...", 1);
    dhry_main();
#endif

#if !CURRENT_PER_MHZ_TEST
    /*
     * The Raptor does not support floating-point calculations.
     * To ensure the accuracy of the calculation results, the final value of Dhrystone_DMIPS_Per_MHz is scaled by a factor of 1000.
     */
    printf("\r\n[dhrystone] : %d\r\n",Dhrystone_DMIPS_Per_MHz);
#endif

    gpio_set_func(LED4, AS_GPIO);
    gpio_set_output_en(LED4, 1);
    gpio_set_input_en(LED4, 0);
}


void main_loop(void)
{
	sleep_ms(200);
    gpio_toggle(LED4);
}


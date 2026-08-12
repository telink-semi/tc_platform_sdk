/********************************************************************************************************
 * @file    driver_app.c
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
/*
 * driver_app.c
 *
 *  Created on: Sep 5, 2024
 *      Author: Admin
 */
#ifdef DRIVER_BIN_ENABLE
#include "driver_app.h"
volatile const  __attribute__((section(".rodata"))) struct app_spi_s  g_driver_func_list_addr ={
//init
	 .cpu_wakeup_init=cpu_wakeup_init,
	 .wd_32k_stop=wd_32k_stop,
	 .clock_init=clock_init,
//gpio
	 .gpio_set_output_en=gpio_set_output_en,
	 .gpio_set_input_en=gpio_set_input_en,
	 .gpio_toggle=gpio_toggle,
	 .gpio_set_func=gpio_set_func,
	 .gpio_write=gpio_write,
 //stimer
	 .sleep_us=sleep_us,

};



/**
 * @brief       This is main function
 * @param[in]   none
 * @return      none
 */

int main (void)   //must on ramcode
{

    while (1)
    {

    }
    return 0;

}
#endif

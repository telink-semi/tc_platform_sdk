/********************************************************************************************************
 * @file    app.c
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
 * app.c
 *
 *  Created on: 2024年9月5日
 *      Author: Admin
 */
#ifndef DRIVER_BIN_ENABLE
#include "app_config.h"

volatile unsigned long retation_start_lma;
volatile  unsigned long retation_start_vma;
volatile unsigned long retation_end_vma;

volatile  unsigned long data_start_lma;
volatile unsigned long data_start_vma;
volatile  unsigned long data_end_vma;

volatile   unsigned long bss_start_vma;
volatile  unsigned long bss_end_vma ;
volatile  unsigned long api_ptr ;


volatile unsigned long load_user_bin (unsigned long  addr){

	retation_start_lma = *(volatile unsigned long*)(addr+DRIVER_RETENTION_DATA_LMA_START);
	retation_start_vma =*(volatile unsigned long*)(addr+DRIVER_RETENTION_DATA_VMA_START);
	retation_end_vma = *(volatile unsigned long*)(addr+DRIVER_RETENTION_DATA_VMA_END);

	data_start_lma = *(volatile unsigned long*)(addr+DRIVER_DATA_LMA_START_ADDR_OFFSET);
    data_start_vma =*(volatile unsigned long*)(addr+DRIVER_DATA_VMA_START_ADDR_OFFSET);
    data_end_vma = *(volatile unsigned long*)(addr+DRIVER_DATA_VMA_END_ADDR_OFFSET);

    bss_start_vma = *(volatile unsigned long*)(addr+DRIVER_BSS_VMA_START_ADDR_OFFSET);
    bss_end_vma = *(volatile unsigned long*)(addr+DRIVER_BSS_VMA_END_ADDR_OFFSET);

    api_ptr = *(volatile unsigned long*)(addr+DRIVER_FUNC_LIST_ADDR_OFFSET);


    for(unsigned long i = retation_start_vma; i < retation_end_vma; i = i+4){
		*(volatile unsigned long*)((i)) = *(volatile unsigned long*)(retation_start_lma + (i - retation_start_vma));
	}
    for(unsigned long i = data_start_vma; i < data_end_vma; i = i+4){
        *(volatile unsigned long*)((i)) = *(volatile unsigned long*)(data_start_lma + (i - data_start_vma));
    }
    for(unsigned long i = bss_start_vma; i < bss_end_vma; i = i+4){
        *(volatile unsigned long*)((i)) = 0;
    }
    return api_ptr;
}
struct app_spi_s * user_api_ptr;
volatile unsigned char stimer_irq_cnt=0;
void user_init(void){
	user_api_ptr->gpio_set_func(LED1,AS_GPIO);
	user_api_ptr->gpio_set_output_en(LED1, 1); 		//enable output
    user_api_ptr->gpio_set_input_en(LED1,0);	   //disable input
}

void main_loop (void){
	user_api_ptr->gpio_toggle(LED1);
	user_api_ptr->sleep_us(500000);
}
#endif

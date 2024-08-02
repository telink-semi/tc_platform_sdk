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

void user_init(void)
{
	sleep_ms(1000);
	//1.init the LED pin,for indication
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

	//Only B80 B85 and B87 has support USB, B89 has not USB module, not support USB printf.
#if ((DEBUG_BUS == DEBUG_USB) && (MCU_CORE_B80 || MCU_CORE_B80B ||MCU_CORE_B85 || MCU_CORE_B87))
	usb_set_pin_en();
	sleep_ms(1000);
	printf("\n Driver version: %2x \n Copyright (c) %d Telink semiconductor (%s) Ltd, Co \n",0xa001,2019,"shanghai");
#else
	printf("\n Driver version: %2x \n Copyright (c) %d Telink semiconductor (%s) Ltd, Co \n",0xa001,2019,"shanghai");
#endif

}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	sleep_ms(1000);
    gpio_toggle(LED1);
    gpio_toggle(LED2);
    gpio_toggle(LED3);
    gpio_toggle(LED4);

	printf(" Hello world! \n");

    // test sprintf 
	int hex_test=0xab;
	char hex_buff[3];//The minimum length is set to 3 bytes ('a'+ 'b'+'\0').
	sprintf(hex_buff,"%x",hex_test);

	int num_test=123;
	char num_buff[4];//The minimum length is set to 4 bytes ('1'+'2'+'3'+'\0').
	sprintf(num_buff,"%d",num_test);

	const char* string_test = "ABCD";
	char string_buff[5];//The minimum length is set to 5 bytes ('A'+'B'+'C'+'D'+'\0').
	sprintf(string_buff,"%s",string_test);

	printf("hex_test=%s , num_test=%s , string_test=%s \n",hex_buff,num_buff,string_buff);


}





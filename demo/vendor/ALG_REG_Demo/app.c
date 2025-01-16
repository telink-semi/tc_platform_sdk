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

unsigned char analog_write_byte_test(void);
unsigned char analog_write_buff_test(void);

unsigned char normal_tx_buffer[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
unsigned char normal_rx_buffer[8] = {0x00};
unsigned char normal_rx_buffer1[8] = {0x00};
unsigned short normal_rx_hword[2] = {0x0000};
unsigned int normal_rx_word  __attribute__((aligned(4)))= 0x00000000;

volatile unsigned short NN;

unsigned char test_result = 0;

unsigned char analog_write_byte_test(void)
{
    unsigned char write_data[4] = {0x11, 0x22, 0x33, 0x44};
    analog_write(PM_ANA_REG_WD_CLR_BUF0, write_data[0]);
    analog_write(PM_ANA_REG_WD_CLR_BUF0+1, write_data[1]);
    analog_write(PM_ANA_REG_WD_CLR_BUF0+2, write_data[2]);
    analog_write(PM_ANA_REG_WD_CLR_BUF0+3, write_data[3]);

    normal_rx_buffer[0] = analog_read(PM_ANA_REG_WD_CLR_BUF0);
    normal_rx_buffer[1] = analog_read(PM_ANA_REG_WD_CLR_BUF0+1);
    normal_rx_buffer[2] = analog_read(PM_ANA_REG_WD_CLR_BUF0+2);
    normal_rx_buffer[3] = analog_read(PM_ANA_REG_WD_CLR_BUF0+3);

    if(memcmp((const void *)write_data, (const void *)normal_rx_buffer, 4))
    {
        return 0;
    }
    return 1;
}

void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO); //set as GPIO
    gpio_set_func(LED2, AS_GPIO); //set as GPIO

    gpio_set_output_en(LED1, 1); //enable output
    gpio_set_output_en(LED2, 1); //enable output

    gpio_set_input_en(LED1, 0); //disable input
    gpio_set_input_en(LED2, 0); //disable input

    gpio_write(LED1, 0); //LED OFF
    gpio_write(LED2, 0); //LED OFF

    test_result = analog_write_byte_test();
    if(test_result == 0)
    {
        while(1){}
    }

}

void main_loop(void)
{
    while(1)
    {
        gpio_toggle(LED1);
        gpio_toggle(LED2);
        sleep_ms(500);
    }
}

/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for Telink MCU
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
#include "random.h"

#if (MCU_CORE_TC123X)
#define SRAM_START         (0x840000)
#define SRAM_END           (0x850000)
#endif

#define SRAM_TEST_START    (SRAM_START + 6 * 1024)
#define SRAM_TEST_END      (SRAM_END   - 1 * 1024)

void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_output_en(LED1, 1); //enable output
    gpio_set_input_en(LED1, 0);  //disable input
    gpio_write(LED1, 0);         //LED OFF
    random_generator_init();
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	unsigned int addr = SRAM_TEST_START;
	unsigned char data = 1;
	printf(" Hello world! \n");
    //write data
    for(; addr < SRAM_TEST_END; addr++)
    {
    	REG_ADDR8(addr) = data;
    	data++;
    	if (data == 0) data++;
    }
    //check data
    data = 1;
    for(addr = SRAM_TEST_START; addr < SRAM_TEST_END; addr++)
    {
    	if (REG_ADDR8(addr) != data)
    	{
    		printf("data check err0, addr 0x%x, read_data 0x%x, write_data 0x%x!\n",addr,REG_ADDR8(addr),data);
    		while(1);
    	}
    	data++;
    	if (data == 0)
        {
    		data++;
    	}
    }
    for(addr = SRAM_TEST_START; addr < SRAM_TEST_END; addr++)
    {
    	data = rand_uchar();
    	REG_ADDR8(addr) = data;
    	if (REG_ADDR8(addr) != data)
    	{
    		printf("data check err1, addr 0x%x, read_data 0x%x, write_data 0x%x!\n",addr,REG_ADDR8(addr),data);
    		while(1);
    	}
    }
    sleep_ms(1000);
	gpio_toggle(LED1);
}

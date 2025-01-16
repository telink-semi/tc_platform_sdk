/********************************************************************************************************
 * @file    gpio_default.h
 *
 * @brief   This is the header file for TC321X
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
#pragma once






#ifndef PA0_INPUT_ENABLE
#define PA0_INPUT_ENABLE	0
#endif
#ifndef PA1_INPUT_ENABLE
#define PA1_INPUT_ENABLE	0   //USB
#endif
#ifndef PA2_INPUT_ENABLE
#define PA2_INPUT_ENABLE	0  //USB
#endif
#ifndef PA3_INPUT_ENABLE
#define PA3_INPUT_ENABLE	1   //SWS
#endif
#ifndef PA4_INPUT_ENABLE
#define PA4_INPUT_ENABLE	0
#endif
#ifndef PA5_INPUT_ENABLE
#define PA5_INPUT_ENABLE	0
#endif
#ifndef PA6_INPUT_ENABLE
#define PA6_INPUT_ENABLE	0
#endif
#ifndef PA7_INPUT_ENABLE
#define PA7_INPUT_ENABLE	0
#endif
#ifndef PA0_OUTPUT_ENABLE
#define PA0_OUTPUT_ENABLE	0
#endif
#ifndef PA1_OUTPUT_ENABLE
#define PA1_OUTPUT_ENABLE	0
#endif
#ifndef PA2_OUTPUT_ENABLE
#define PA2_OUTPUT_ENABLE	0
#endif
#ifndef PA3_OUTPUT_ENABLE
#define PA3_OUTPUT_ENABLE	0
#endif
#ifndef PA4_OUTPUT_ENABLE
#define PA4_OUTPUT_ENABLE	0
#endif
#ifndef PA5_OUTPUT_ENABLE
#define PA5_OUTPUT_ENABLE	0
#endif
#ifndef PA6_OUTPUT_ENABLE
#define PA6_OUTPUT_ENABLE	0
#endif
#ifndef PA7_OUTPUT_ENABLE
#define PA7_OUTPUT_ENABLE	0
#endif
#ifndef PA0_DATA_STRENGTH_DS0
#define PA0_DATA_STRENGTH_DS0	1
#endif
#ifndef PA1_DATA_STRENGTH_DS0
#define PA1_DATA_STRENGTH_DS0	1
#endif
#ifndef PA2_DATA_STRENGTH_DS0
#define PA2_DATA_STRENGTH_DS0	1
#endif
#ifndef PA3_DATA_STRENGTH_DS0
#define PA3_DATA_STRENGTH_DS0	1
#endif
#ifndef PA4_DATA_STRENGTH_DS0
#define PA4_DATA_STRENGTH_DS0	1
#endif
#ifndef PA5_DATA_STRENGTH_DS0
#define PA5_DATA_STRENGTH_DS0	1
#endif
#ifndef PA6_DATA_STRENGTH_DS0
#define PA6_DATA_STRENGTH_DS0	1
#endif
#ifndef PA7_DATA_STRENGTH_DS0
#define PA7_DATA_STRENGTH_DS0	1
#endif
#ifndef PA0_DATA_STRENGTH_DS1
#define PA0_DATA_STRENGTH_DS1	1
#endif
#ifndef PA1_DATA_STRENGTH_DS1
#define PA1_DATA_STRENGTH_DS1	1
#endif
#ifndef PA2_DATA_STRENGTH_DS1
#define PA2_DATA_STRENGTH_DS1	1
#endif
#ifndef PA3_DATA_STRENGTH_DS1
#define PA3_DATA_STRENGTH_DS1	1
#endif
#ifndef PA4_DATA_STRENGTH_DS1
#define PA4_DATA_STRENGTH_DS1	1
#endif
#ifndef PA5_DATA_STRENGTH_DS1
#define PA5_DATA_STRENGTH_DS1	1
#endif
#ifndef PA6_DATA_STRENGTH_DS1
#define PA6_DATA_STRENGTH_DS1	1
#endif
#ifndef PA7_DATA_STRENGTH_DS1
#define PA7_DATA_STRENGTH_DS1	1
#endif
#ifndef PA0_DATA_OUT
#define PA0_DATA_OUT	0
#endif
#ifndef PA1_DATA_OUT
#define PA1_DATA_OUT	0
#endif
#ifndef PA2_DATA_OUT
#define PA2_DATA_OUT	0
#endif
#ifndef PA3_DATA_OUT
#define PA3_DATA_OUT	0
#endif
#ifndef PA4_DATA_OUT
#define PA4_DATA_OUT	0
#endif
#ifndef PA5_DATA_OUT
#define PA5_DATA_OUT	0
#endif
#ifndef PA6_DATA_OUT
#define PA6_DATA_OUT	0
#endif
#ifndef PA7_DATA_OUT
#define PA7_DATA_OUT	0
#endif
#ifndef PA0_FUNC
#define PA0_FUNC	AS_GPIO
#endif
#ifndef PA1_FUNC
#define PA1_FUNC	AS_GPIO
#endif
#ifndef PA2_FUNC
#define PA2_FUNC	AS_GPIO
#endif
#ifndef PA3_FUNC
#define PA3_FUNC	SWS_IO
#endif
#ifndef PA4_FUNC
#define PA4_FUNC	AS_GPIO
#endif
#ifndef PA5_FUNC
#define PA5_FUNC	AS_GPIO
#endif
#ifndef PA6_FUNC
#define PA6_FUNC	AS_GPIO
#endif
#ifndef PA7_FUNC
#define PA7_FUNC	AS_GPIO
#endif
#ifndef PULL_WAKEUP_SRC_PA0
#define PULL_WAKEUP_SRC_PA0	0
#endif
#ifndef PULL_WAKEUP_SRC_PA1
#define PULL_WAKEUP_SRC_PA1	0
#endif
#ifndef PULL_WAKEUP_SRC_PA2
#define PULL_WAKEUP_SRC_PA2	0
#endif
#ifndef PULL_WAKEUP_SRC_PA3
#define PULL_WAKEUP_SRC_PA3	 PM_PIN_PULLUP_1M	//sws pullup
#endif
#ifndef PULL_WAKEUP_SRC_PA4
#define PULL_WAKEUP_SRC_PA4	0
#endif
#ifndef PULL_WAKEUP_SRC_PA5
#define PULL_WAKEUP_SRC_PA5	0
#endif
#ifndef PULL_WAKEUP_SRC_PA6
#define PULL_WAKEUP_SRC_PA6	0
#endif
#ifndef PULL_WAKEUP_SRC_PA7
#define PULL_WAKEUP_SRC_PA7	0
#endif

//////////////////////////////////////////////////
#ifndef PB0_INPUT_ENABLE
#define PB0_INPUT_ENABLE	0    //SPI
#endif
#ifndef PB1_INPUT_ENABLE
#define PB1_INPUT_ENABLE	0    //SPI
#endif
#ifndef PB2_INPUT_ENABLE
#define PB2_INPUT_ENABLE	0
#endif
#ifndef PB3_INPUT_ENABLE
#define PB3_INPUT_ENABLE	0    //SPI
#endif
#ifndef PB4_INPUT_ENABLE
#define PB4_INPUT_ENABLE	0
#endif
#ifndef PB5_INPUT_ENABLE
#define PB5_INPUT_ENABLE	0
#endif
#ifndef PB6_INPUT_ENABLE
#define PB6_INPUT_ENABLE	0
#endif
#ifndef PB7_INPUT_ENABLE
#define PB7_INPUT_ENABLE	0
#endif
#ifndef PB0_OUTPUT_ENABLE
#define PB0_OUTPUT_ENABLE	0
#endif
#ifndef PB1_OUTPUT_ENABLE
#define PB1_OUTPUT_ENABLE	0
#endif
#ifndef PB2_OUTPUT_ENABLE
#define PB2_OUTPUT_ENABLE	0
#endif
#ifndef PB3_OUTPUT_ENABLE
#define PB3_OUTPUT_ENABLE	0
#endif
#ifndef PB4_OUTPUT_ENABLE
#define PB4_OUTPUT_ENABLE	0
#endif
#ifndef PB5_OUTPUT_ENABLE
#define PB5_OUTPUT_ENABLE	0
#endif
#ifndef PB6_OUTPUT_ENABLE
#define PB6_OUTPUT_ENABLE	0
#endif
#ifndef PB7_OUTPUT_ENABLE
#define PB7_OUTPUT_ENABLE	0
#endif
#ifndef PB0_DATA_STRENGTH_DS0
#define PB0_DATA_STRENGTH_DS0	1
#endif
#ifndef PB1_DATA_STRENGTH_DS0
#define PB1_DATA_STRENGTH_DS0	1
#endif
#ifndef PB2_DATA_STRENGTH_DS0
#define PB2_DATA_STRENGTH_DS0	1
#endif
#ifndef PB3_DATA_STRENGTH_DS0
#define PB3_DATA_STRENGTH_DS0	1
#endif
#ifndef PB4_DATA_STRENGTH_DS0
#define PB4_DATA_STRENGTH_DS0	1
#endif
#ifndef PB5_DATA_STRENGTH_DS0
#define PB5_DATA_STRENGTH_DS0	1
#endif
#ifndef PB6_DATA_STRENGTH_DS0
#define PB6_DATA_STRENGTH_DS0	1
#endif
#ifndef PB7_DATA_STRENGTH_DS0
#define PB7_DATA_STRENGTH_DS0	1
#endif
#ifndef PB0_DATA_STRENGTH_DS1
#define PB0_DATA_STRENGTH_DS1	1
#endif
#ifndef PB1_DATA_STRENGTH_DS1
#define PB1_DATA_STRENGTH_DS1	1
#endif
#ifndef PB2_DATA_STRENGTH_DS1
#define PB2_DATA_STRENGTH_DS1	1
#endif
#ifndef PB3_DATA_STRENGTH_DS1
#define PB3_DATA_STRENGTH_DS1	1
#endif
#ifndef PB4_DATA_STRENGTH_DS1
#define PB4_DATA_STRENGTH_DS1	1
#endif
#ifndef PB5_DATA_STRENGTH_DS1
#define PB5_DATA_STRENGTH_DS1	1
#endif
#ifndef PB6_DATA_STRENGTH_DS1
#define PB6_DATA_STRENGTH_DS1	1
#endif
#ifndef PB7_DATA_STRENGTH_DS1
#define PB7_DATA_STRENGTH_DS1	1
#endif
#ifndef PB0_DATA_OUT
#define PB0_DATA_OUT	0
#endif
#ifndef PB1_DATA_OUT
#define PB1_DATA_OUT	0
#endif
#ifndef PB2_DATA_OUT
#define PB2_DATA_OUT	0
#endif
#ifndef PB3_DATA_OUT
#define PB3_DATA_OUT	0
#endif
#ifndef PB4_DATA_OUT
#define PB4_DATA_OUT	0
#endif
#ifndef PB5_DATA_OUT
#define PB5_DATA_OUT	0
#endif
#ifndef PB6_DATA_OUT
#define PB6_DATA_OUT	0
#endif
#ifndef PB7_DATA_OUT
#define PB7_DATA_OUT	0
#endif
#ifndef PB0_FUNC
#define PB0_FUNC	AS_GPIO
#endif
#ifndef PB1_FUNC
#define PB1_FUNC	AS_GPIO
#endif
#ifndef PB2_FUNC
#define PB2_FUNC	AS_GPIO
#endif
#ifndef PB3_FUNC
#define PB3_FUNC	AS_GPIO
#endif
#ifndef PB4_FUNC
#define PB4_FUNC	AS_GPIO
#endif
#ifndef PB5_FUNC
#define PB5_FUNC	AS_GPIO
#endif
#ifndef PB6_FUNC
#define PB6_FUNC	AS_GPIO
#endif
#ifndef PB7_FUNC
#define PB7_FUNC	AS_GPIO
#endif
#ifndef PULL_WAKEUP_SRC_PB0
#define PULL_WAKEUP_SRC_PB0	0
#endif
#ifndef PULL_WAKEUP_SRC_PB1
#define PULL_WAKEUP_SRC_PB1	0
#endif
#ifndef PULL_WAKEUP_SRC_PB2
#define PULL_WAKEUP_SRC_PB2	0
#endif
#ifndef PULL_WAKEUP_SRC_PB3
#define PULL_WAKEUP_SRC_PB3	0
#endif
#ifndef PULL_WAKEUP_SRC_PB4
#define PULL_WAKEUP_SRC_PB4	0
#endif
#ifndef PULL_WAKEUP_SRC_PB5
#define PULL_WAKEUP_SRC_PB5	0
#endif
#ifndef PULL_WAKEUP_SRC_PB6
#define PULL_WAKEUP_SRC_PB6	0
#endif
#ifndef PULL_WAKEUP_SRC_PB7
#define PULL_WAKEUP_SRC_PB7	0
#endif

//////////////////////////////////////////////////
#ifndef PC0_INPUT_ENABLE
#define PC0_INPUT_ENABLE	0
#endif
#ifndef PC1_INPUT_ENABLE
#define PC1_INPUT_ENABLE	0
#endif
#ifndef PC2_INPUT_ENABLE
#define PC2_INPUT_ENABLE	0
#endif
#ifndef PC3_INPUT_ENABLE
#define PC3_INPUT_ENABLE	0
#endif
#ifndef PC4_INPUT_ENABLE
#define PC4_INPUT_ENABLE	0
#endif
#ifndef PC5_INPUT_ENABLE
#define PC5_INPUT_ENABLE	0
#endif
#ifndef PC6_INPUT_ENABLE
#define PC6_INPUT_ENABLE	0
#endif
#ifndef PC7_INPUT_ENABLE
#define PC7_INPUT_ENABLE	0
#endif
#ifndef PC0_OUTPUT_ENABLE
#define PC0_OUTPUT_ENABLE	0
#endif
#ifndef PC1_OUTPUT_ENABLE
#define PC1_OUTPUT_ENABLE	0
#endif
#ifndef PC2_OUTPUT_ENABLE
#define PC2_OUTPUT_ENABLE	0
#endif
#ifndef PC3_OUTPUT_ENABLE
#define PC3_OUTPUT_ENABLE	0
#endif
#ifndef PC4_OUTPUT_ENABLE
#define PC4_OUTPUT_ENABLE	0
#endif
#ifndef PC5_OUTPUT_ENABLE
#define PC5_OUTPUT_ENABLE	0
#endif
#ifndef PC6_OUTPUT_ENABLE
#define PC6_OUTPUT_ENABLE	0
#endif
#ifndef PC7_OUTPUT_ENABLE
#define PC7_OUTPUT_ENABLE	0
#endif
#ifndef PC0_DATA_STRENGTH_DS0
#define PC0_DATA_STRENGTH_DS0	1
#endif
#ifndef PC1_DATA_STRENGTH_DS0
#define PC1_DATA_STRENGTH_DS0	1
#endif
#ifndef PC2_DATA_STRENGTH_DS0
#define PC2_DATA_STRENGTH_DS0	1
#endif
#ifndef PC3_DATA_STRENGTH_DS0
#define PC3_DATA_STRENGTH_DS0	1
#endif
#ifndef PC4_DATA_STRENGTH_DS0
#define PC4_DATA_STRENGTH_DS0	1
#endif
#ifndef PC5_DATA_STRENGTH_DS0
#define PC5_DATA_STRENGTH_DS0	1
#endif
#ifndef PC6_DATA_STRENGTH_DS0
#define PC6_DATA_STRENGTH_DS0	1
#endif
#ifndef PC7_DATA_STRENGTH_DS0
#define PC7_DATA_STRENGTH_DS0	1
#endif
#ifndef PC0_DATA_STRENGTH_DS1
#define PC0_DATA_STRENGTH_DS1	1
#endif
#ifndef PC1_DATA_STRENGTH_DS1
#define PC1_DATA_STRENGTH_DS1	1
#endif
#ifndef PC2_DATA_STRENGTH_DS1
#define PC2_DATA_STRENGTH_DS1	1
#endif
#ifndef PC3_DATA_STRENGTH_DS1
#define PC3_DATA_STRENGTH_DS1	1
#endif
#ifndef PC4_DATA_STRENGTH_DS1
#define PC4_DATA_STRENGTH_DS1	1
#endif
#ifndef PC5_DATA_STRENGTH_DS1
#define PC5_DATA_STRENGTH_DS1	1
#endif
#ifndef PC6_DATA_STRENGTH_DS1
#define PC6_DATA_STRENGTH_DS1	1
#endif
#ifndef PC7_DATA_STRENGTH_DS1
#define PC7_DATA_STRENGTH_DS1	1
#endif
#ifndef PC0_DATA_OUT
#define PC0_DATA_OUT	0
#endif
#ifndef PC1_DATA_OUT
#define PC1_DATA_OUT	0
#endif
#ifndef PC2_DATA_OUT
#define PC2_DATA_OUT	0
#endif
#ifndef PC3_DATA_OUT
#define PC3_DATA_OUT	0
#endif
#ifndef PC4_DATA_OUT
#define PC4_DATA_OUT	0
#endif
#ifndef PC5_DATA_OUT
#define PC5_DATA_OUT	0
#endif
#ifndef PC6_DATA_OUT
#define PC6_DATA_OUT	0
#endif
#ifndef PC7_DATA_OUT
#define PC7_DATA_OUT	0
#endif
#ifndef PC0_FUNC
#define PC0_FUNC	AS_GPIO
#endif
#ifndef PC1_FUNC
#define PC1_FUNC	AS_GPIO
#endif
#ifndef PC2_FUNC
#define PC2_FUNC	AS_GPIO
#endif
#ifndef PC3_FUNC
#define PC3_FUNC	AS_GPIO
#endif
#ifndef PC4_FUNC
#define PC4_FUNC	AS_GPIO
#endif
#ifndef PC5_FUNC
#define PC5_FUNC	AS_GPIO
#endif
#ifndef PC6_FUNC
#define PC6_FUNC	AS_GPIO
#endif
#ifndef PC7_FUNC
#define PC7_FUNC	AS_GPIO
#endif
#ifndef PULL_WAKEUP_SRC_PC0
#define PULL_WAKEUP_SRC_PC0	0
#endif
#ifndef PULL_WAKEUP_SRC_PC1
#define PULL_WAKEUP_SRC_PC1	0
#endif
#ifndef PULL_WAKEUP_SRC_PC2
#define PULL_WAKEUP_SRC_PC2	0
#endif
#ifndef PULL_WAKEUP_SRC_PC3
#define PULL_WAKEUP_SRC_PC3	0
#endif
#ifndef PULL_WAKEUP_SRC_PC4
#define PULL_WAKEUP_SRC_PC4	0
#endif
#ifndef PULL_WAKEUP_SRC_PC5
#define PULL_WAKEUP_SRC_PC5	0
#endif
#ifndef PULL_WAKEUP_SRC_PC6
#define PULL_WAKEUP_SRC_PC6	0
#endif
#ifndef PULL_WAKEUP_SRC_PC7
#define PULL_WAKEUP_SRC_PC7	0
#endif

//////////////////////////////////////////////////
#ifndef PD0_INPUT_ENABLE
#define PD0_INPUT_ENABLE	0
#endif
#ifndef PD1_INPUT_ENABLE
#define PD1_INPUT_ENABLE	0
#endif
#ifndef PD2_INPUT_ENABLE
#define PD2_INPUT_ENABLE	0
#endif
#ifndef PD3_INPUT_ENABLE
#define PD3_INPUT_ENABLE	0
#endif
#ifndef PD4_INPUT_ENABLE
#define PD4_INPUT_ENABLE	0
#endif
#ifndef PD5_INPUT_ENABLE
#define PD5_INPUT_ENABLE	0
#endif
#ifndef PD6_INPUT_ENABLE
#define PD6_INPUT_ENABLE	0
#endif
#ifndef PD7_INPUT_ENABLE
#define PD7_INPUT_ENABLE	0
#endif
#ifndef PD0_OUTPUT_ENABLE
#define PD0_OUTPUT_ENABLE	0
#endif
#ifndef PD1_OUTPUT_ENABLE
#define PD1_OUTPUT_ENABLE	0
#endif
#ifndef PD2_OUTPUT_ENABLE
#define PD2_OUTPUT_ENABLE	0
#endif
#ifndef PD3_OUTPUT_ENABLE
#define PD3_OUTPUT_ENABLE	0
#endif
#ifndef PD4_OUTPUT_ENABLE
#define PD4_OUTPUT_ENABLE	0
#endif
#ifndef PD5_OUTPUT_ENABLE
#define PD5_OUTPUT_ENABLE	0
#endif
#ifndef PD6_OUTPUT_ENABLE
#define PD6_OUTPUT_ENABLE	0
#endif
#ifndef PD7_OUTPUT_ENABLE
#define PD7_OUTPUT_ENABLE	0
#endif
#ifndef PD0_DATA_STRENGTH_DS0
#define PD0_DATA_STRENGTH_DS0	1
#endif
#ifndef PD1_DATA_STRENGTH_DS0
#define PD1_DATA_STRENGTH_DS0	1
#endif
#ifndef PD2_DATA_STRENGTH_DS0
#define PD2_DATA_STRENGTH_DS0	1
#endif
#ifndef PD3_DATA_STRENGTH_DS0
#define PD3_DATA_STRENGTH_DS0	1
#endif
#ifndef PD4_DATA_STRENGTH_DS0
#define PD4_DATA_STRENGTH_DS0	1
#endif
#ifndef PD5_DATA_STRENGTH_DS0
#define PD5_DATA_STRENGTH_DS0	1
#endif
#ifndef PD6_DATA_STRENGTH_DS0
#define PD6_DATA_STRENGTH_DS0	1
#endif
#ifndef PD7_DATA_STRENGTH_DS0
#define PD7_DATA_STRENGTH_DS0	1
#ifndef PD0_DATA_STRENGTH_DS1
#define PD0_DATA_STRENGTH_DS1	1
#endif
#ifndef PD1_DATA_STRENGTH_DS1
#define PD1_DATA_STRENGTH_DS1	1
#endif
#ifndef PD2_DATA_STRENGTH_DS1
#define PD2_DATA_STRENGTH_DS1	1
#endif
#ifndef PD3_DATA_STRENGTH_DS1
#define PD3_DATA_STRENGTH_DS1	1
#endif
#ifndef PD4_DATA_STRENGTH_DS1
#define PD4_DATA_STRENGTH_DS1	1
#endif
#ifndef PD5_DATA_STRENGTH_DS1
#define PD5_DATA_STRENGTH_DS1	1
#endif
#ifndef PD6_DATA_STRENGTH_DS1
#define PD6_DATA_STRENGTH_DS1	1
#endif
#ifndef PD7_DATA_STRENGTH_DS1
#define PD7_DATA_STRENGTH_DS1	1
#endif
#ifndef PD0_DATA_OUT
#define PD0_DATA_OUT	0
#endif
#ifndef PD1_DATA_OUT
#define PD1_DATA_OUT	0
#endif
#ifndef PD2_DATA_OUT
#define PD2_DATA_OUT	0
#endif
#ifndef PD3_DATA_OUT
#define PD3_DATA_OUT	0
#endif
#ifndef PD4_DATA_OUT
#define PD4_DATA_OUT	0
#endif
#ifndef PD5_DATA_OUT
#define PD5_DATA_OUT	0
#endif
#ifndef PD6_DATA_OUT
#define PD6_DATA_OUT	0
#endif
#ifndef PD7_DATA_OUT
#define PD7_DATA_OUT	0
#endif
#ifndef PD0_FUNC
#define PD0_FUNC	AS_GPIO
#endif
#ifndef PD1_FUNC
#define PD1_FUNC	AS_GPIO
#endif
#ifndef PD2_FUNC
#define PD2_FUNC	AS_GPIO
#endif
#ifndef PD3_FUNC
#define PD3_FUNC	AS_GPIO
#endif
#ifndef PD4_FUNC
#define PD4_FUNC	AS_GPIO
#endif
#ifndef PD5_FUNC
#define PD5_FUNC	AS_GPIO
#endif
#ifndef PD6_FUNC
#define PD6_FUNC	AS_GPIO
#endif
#ifndef PD7_FUNC
#define PD7_FUNC	AS_GPIO
#endif
#ifndef PULL_WAKEUP_SRC_PD0
#define PULL_WAKEUP_SRC_PD0	0
#endif
#ifndef PULL_WAKEUP_SRC_PD1
#define PULL_WAKEUP_SRC_PD1	0
#endif
#ifndef PULL_WAKEUP_SRC_PD2
#define PULL_WAKEUP_SRC_PD2	0
#endif
#ifndef PULL_WAKEUP_SRC_PD3
#define PULL_WAKEUP_SRC_PD3	0
#endif
#ifndef PULL_WAKEUP_SRC_PD4
#define PULL_WAKEUP_SRC_PD4	0
#endif
#ifndef PULL_WAKEUP_SRC_PD5
#define PULL_WAKEUP_SRC_PD5	0
#endif
#ifndef PULL_WAKEUP_SRC_PD6
#define PULL_WAKEUP_SRC_PD6	0
#endif
#ifndef PULL_WAKEUP_SRC_PD7
#define PULL_WAKEUP_SRC_PD7	0
#endif

//////////////////////////////////////////////////
#ifndef PE0_INPUT_ENABLE
#define PE0_INPUT_ENABLE	1
#endif
#ifndef PE1_INPUT_ENABLE
#define PE1_INPUT_ENABLE	1
#endif

#endif
#ifndef PE0_OUTPUT_ENABLE
#define PE0_OUTPUT_ENABLE	0
#endif
#ifndef PE1_OUTPUT_ENABLE
#define PE1_OUTPUT_ENABLE	0
#endif
#ifndef PE0_DATA_STRENGTH_DS0
#define PE0_DATA_STRENGTH_DS0	1
#endif
#ifndef PE1_DATA_STRENGTH_DS0
#define PE1_DATA_STRENGTH_DS0	1
#endif
#ifndef PE0_DATA_STRENGTH_DS1
#define PE0_DATA_STRENGTH_DS1	1
#endif
#ifndef PE1_DATA_STRENGTH_DS1
#define PE1_DATA_STRENGTH_DS1	1
#endif


#ifndef PE0_DATA_OUT
#define PE0_DATA_OUT	0
#endif
#ifndef PE1_DATA_OUT
#define PE1_DATA_OUT	0
#endif

#ifndef PE0_FUNC
#define PE0_FUNC	AS_GPIO
#endif
#ifndef PE1_FUNC
#define PE1_FUNC	AS_GPIO
#endif

#ifndef PF0_OUTPUT_ENABLE
#define PF0_OUTPUT_ENABLE	0
#endif
#ifndef PF1_OUTPUT_ENABLE
#define PF1_OUTPUT_ENABLE	0
#endif
#ifndef PF2_OUTPUT_ENABLE
#define PF2_OUTPUT_ENABLE	0
#endif
#ifndef PF3_OUTPUT_ENABLE
#define PF3_OUTPUT_ENABLE	0
#endif

#ifndef PF0_INPUT_ENABLE
#define PF0_INPUT_ENABLE	1 //MSPI
#endif
#ifndef PF1_INPUT_ENABLE
#define PF1_INPUT_ENABLE	1 //MSPI
#endif
#ifndef PF2_INPUT_ENABLE
#define PF2_INPUT_ENABLE	1 //MSPI
#endif
#ifndef PF3_INPUT_ENABLE
#define PF3_INPUT_ENABLE	1 //MSPI
#endif
#ifndef PF0_DATA_STRENGTH_DS0
#define PF0_DATA_STRENGTH_DS0	1
#endif
#ifndef PF1_DATA_STRENGTH_DS0
#define PF1_DATA_STRENGTH_DS0	1
#endif
#ifndef PF2_DATA_STRENGTH_DS0
#define PF2_DATA_STRENGTH_DS0	1
#endif
#ifndef PF3_DATA_STRENGTH_DS0
#define PF3_DATA_STRENGTH_DS0	1
#endif
#ifndef PF0_DATA_STRENGTH_DS1
#define PF0_DATA_STRENGTH_DS1	1
#endif
#ifndef PF1_DATA_STRENGTH_DS1
#define PF1_DATA_STRENGTH_DS1	1
#endif
#ifndef PF2_DATA_STRENGTH_DS1
#define PF2_DATA_STRENGTH_DS1	1
#endif
#ifndef PF3_DATA_STRENGTH_DS1
#define PF3_DATA_STRENGTH_DS1	1
#endif

#ifndef PF0_DATA_OUT
#define PF0_DATA_OUT		0
#endif
#ifndef PF1_DATA_OUT
#define PF1_DATA_OUT		0
#endif
#ifndef PF2_DATA_OUT
#define PF2_DATA_OUT		0
#endif
#ifndef PF3_DATA_OUT
#define PF3_DATA_OUT		0
#endif

#ifndef PF0_FUNC
#define PF0_FUNC	MSPI_MOSI
#endif
#ifndef PF1_FUNC
#define PF1_FUNC	MSPI_MCLK
#endif
#ifndef PF2_FUNC
#define PF2_FUNC	MSPI_MSCN
#endif
#ifndef PF3_FUNC
#define PF3_FUNC	MSPI_MISO
#endif

#ifndef PULL_WAKEUP_SRC_PF0
#define PULL_WAKEUP_SRC_PF0	  0
#endif
#ifndef PULL_WAKEUP_SRC_PF1
#define PULL_WAKEUP_SRC_PF1	  0
#endif

/**
 *  @brief  Define GPIO setting
 */

#define GPIO_PA0_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x03;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA0_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA0_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA0_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x03;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA0_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val ^=  0x01;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)

#define GPIO_PA1_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x0C;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA1_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA1_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA1_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x0C;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA1_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val ^=  0x04;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)


#define GPIO_PA2_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x30;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA2_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA2_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA2_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x30;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA2_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val ^=  0x10;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)


#define GPIO_PA3_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0xC0;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA3_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA3_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA3_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val |=  0xC0;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)
#define GPIO_PA3_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0e_pa0_pa3_pull);  val ^=  0x40;   				analog_write(areg_0e_pa0_pa3_pull, val); }while(0)

#define GPIO_PA4_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x03;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA4_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA4_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA4_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x03;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA4_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val ^=  0x01;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)


#define GPIO_PA5_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x0C;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA5_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA5_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA5_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x0C;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA5_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val ^=  0x04;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)

#define GPIO_PA6_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x30;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA6_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA6_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA6_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x30;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA6_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val ^=  0x10;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)

#define GPIO_PA7_PULL_FLOAT			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0xC0;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA7_PULL_UP_1M			do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA7_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA7_PULL_UP_10K		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val |=  0xC0;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)
#define GPIO_PA7_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_0f_pa4_pa7_pull);  val ^=  0x40;   				analog_write(areg_0f_pa4_pa7_pull, val); }while(0)






#define GPIO_PB0_PULL_FLOAT			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x03;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB0_PULL_UP_1M			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB0_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB0_PULL_UP_10K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x03;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB0_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val ^=  0x01;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)



#define GPIO_PB1_PULL_FLOAT			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x0C;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB1_PULL_UP_1M			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB1_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB1_PULL_UP_10K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x0C;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB1_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val ^=  0x04;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)


#define GPIO_PB2_PULL_FLOAT			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x30;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB2_PULL_UP_1M			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB2_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB2_PULL_UP_10K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x30;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB2_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val ^=  0x10;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)


#define GPIO_PB3_PULL_FLOAT			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0xC0;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB3_PULL_UP_1M			do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB3_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB3_PULL_UP_10K		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val |=  0xC0;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)
#define GPIO_PB3_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_10_pb0_pb3_pull);  val ^=  0x40;   				analog_write(areg_10_pb0_pb3_pull, val); }while(0)

#define GPIO_PB4_PULL_FLOAT			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x03;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB4_PULL_UP_1M			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB4_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB4_PULL_UP_10K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x03;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB4_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val ^=  0x01;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)


#define GPIO_PB5_PULL_FLOAT			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x0C;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB5_PULL_UP_1M			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB5_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB5_PULL_UP_10K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x0C;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB5_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val ^=  0x04;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)

#define GPIO_PB6_PULL_FLOAT			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x30;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB6_PULL_UP_1M			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB6_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB6_PULL_UP_10K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x30;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB6_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val ^=  0x10;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)

#define GPIO_PB7_PULL_FLOAT			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0xC0;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB7_PULL_UP_1M			do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB7_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB7_PULL_UP_10K		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val |=  0xC0;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)
#define GPIO_PB7_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_11_pb4_pb7_pull);  val ^=  0x40;   				analog_write(areg_11_pb4_pb7_pull, val); }while(0)



#define GPIO_PC0_PULL_FLOAT			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x03;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC0_PULL_UP_1M			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC0_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC0_PULL_UP_10K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x03;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC0_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val ^=  0x01;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)



#define GPIO_PC1_PULL_FLOAT			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x0C;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC1_PULL_UP_1M			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC1_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC1_PULL_UP_10K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x0C;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC1_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val ^=  0x04;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)


#define GPIO_PC2_PULL_FLOAT			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x30;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC2_PULL_UP_1M			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC2_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC2_PULL_UP_10K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x30;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC2_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val ^=  0x10;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)


#define GPIO_PC3_PULL_FLOAT			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0xC0;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC3_PULL_UP_1M			do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC3_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC3_PULL_UP_10K		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val |=  0xC0;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)
#define GPIO_PC3_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_12_pc0_pc3_pull);  val ^=  0x40;   				analog_write(areg_12_pc0_pc3_pull, val); }while(0)

#define GPIO_PC4_PULL_FLOAT			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x03;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC4_PULL_UP_1M			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC4_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC4_PULL_UP_10K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x03;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC4_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val ^=  0x01;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)


#define GPIO_PC5_PULL_FLOAT			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x0C;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC5_PULL_UP_1M			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC5_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC5_PULL_UP_10K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x0C;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC5_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val ^=  0x04;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)

#define GPIO_PC6_PULL_FLOAT			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x30;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC6_PULL_UP_1M			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC6_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC6_PULL_UP_10K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x30;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC6_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val ^=  0x10;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)

#define GPIO_PC7_PULL_FLOAT			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0xC0;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC7_PULL_UP_1M			do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC7_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC7_PULL_UP_10K		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val |=  0xC0;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)
#define GPIO_PC7_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_13_pc4_pc7_pull);  val ^=  0x40;   				analog_write(areg_13_pc4_pc7_pull, val); }while(0)



#define GPIO_PD0_PULL_FLOAT			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x03;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD0_PULL_UP_1M			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD0_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD0_PULL_UP_10K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x03;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD0_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val ^=  0x01;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)



#define GPIO_PD1_PULL_FLOAT			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x0C;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD1_PULL_UP_1M			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD1_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD1_PULL_UP_10K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x0C;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD1_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val ^=  0x04;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)


#define GPIO_PD2_PULL_FLOAT			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x30;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD2_PULL_UP_1M			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD2_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD2_PULL_UP_10K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x30;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD2_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val ^=  0x10;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)


#define GPIO_PD3_PULL_FLOAT			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0xC0;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD3_PULL_UP_1M			do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD3_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD3_PULL_UP_10K		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val |=  0xC0;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)
#define GPIO_PD3_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_14_pd0_pd3_pull);  val ^=  0x40;   				analog_write(areg_14_pd0_pd3_pull, val); }while(0)

#define GPIO_PD4_PULL_FLOAT			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x03;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD4_PULL_UP_1M			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD4_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD4_PULL_UP_10K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x03;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD4_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val ^=  0x01;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)


#define GPIO_PD5_PULL_FLOAT			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x0C;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD5_PULL_UP_1M			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD5_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD5_PULL_UP_10K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x0C;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD5_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val ^=  0x04;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)

#define GPIO_PD6_PULL_FLOAT			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x30;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD6_PULL_UP_1M			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x20;	val |=  0x10;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD6_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x20;	val &= ~0x10;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD6_PULL_UP_10K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x30;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD6_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val ^=  0x10;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)

#define GPIO_PD7_PULL_FLOAT			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0xC0;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD7_PULL_UP_1M			do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val &= ~0x80;	val |=  0x40;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD7_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0x80;	val &= ~0x40;   analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD7_PULL_UP_10K		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val |=  0xC0;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)
#define GPIO_PD7_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_15_pd4_pd7_pull);  val ^=  0x40;   				analog_write(areg_15_pd4_pd7_pull, val); }while(0)

#define GPIO_PE0_PULL_FLOAT			do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val &= ~0x03;   				analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE0_PULL_UP_1M			do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val &= ~0x02;	val |=  0x01;   analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE0_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val |=  0x02;	val &= ~0x01;   analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE0_PULL_UP_10K		do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val |=  0x03;   				analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE0_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val ^=  0x01;   				analog_write(areg_16_pe0_pe1_pull, val); }while(0)

#define GPIO_PE1_PULL_FLOAT			do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val &= ~0x0C;   				analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE1_PULL_UP_1M			do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val &= ~0x08;	val |=  0x04;   analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE1_PULL_DOWN_100K		do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val |=  0x08;	val &= ~0x04;   analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE1_PULL_UP_10K		do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val |=  0x0C;   				analog_write(areg_16_pe0_pe1_pull, val); }while(0)
#define GPIO_PE1_PULL_TOGGLE		do{ unsigned char val = analog_read(areg_16_pe0_pe1_pull);  val ^=  0x04;   				analog_write(areg_16_pe0_pe1_pull, val); }while(0)


#define GPIO_PA0_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x01    )
#define GPIO_PA0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x01    )
#define GPIO_PA0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x01    )
#define GPIO_PA1_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x02    )
#define GPIO_PA1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x02    )
#define GPIO_PA1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x02    )
#define GPIO_PA2_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x04    )
#define GPIO_PA2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x04    )
#define GPIO_PA2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x04    )
#define GPIO_PA3_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x08    )
#define GPIO_PA3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x08    )
#define GPIO_PA3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x08    )
#define GPIO_PA4_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x10    )
#define GPIO_PA4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x10    )
#define GPIO_PA4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x10    )
#define GPIO_PA5_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x20    )
#define GPIO_PA5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x20    )
#define GPIO_PA5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x20    )
#define GPIO_PA6_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x40    )
#define GPIO_PA6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x40    )
#define GPIO_PA6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x40    )
#define GPIO_PA7_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c1) |= 0x80    )
#define GPIO_PA7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c0) |= 0x80    )
#define GPIO_PA7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c2) ^= 0x80    )

#define GPIO_PB0_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x01    )
#define GPIO_PB0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x01    )
#define GPIO_PB0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x01    )
#define GPIO_PB1_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x02    )
#define GPIO_PB1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x02    )
#define GPIO_PB1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x02    )
#define GPIO_PB2_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x04    )
#define GPIO_PB2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x04    )
#define GPIO_PB2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x04    )
#define GPIO_PB3_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x08    )
#define GPIO_PB3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x08    )
#define GPIO_PB3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x08    )
#define GPIO_PB4_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x10    )
#define GPIO_PB4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x10    )
#define GPIO_PB4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x10    )
#define GPIO_PB5_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x20    )
#define GPIO_PB5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x20    )
#define GPIO_PB5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x20    )
#define GPIO_PB6_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x40    )
#define GPIO_PB6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x40    )
#define GPIO_PB6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x40    )
#define GPIO_PB7_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c5) |= 0x80    )
#define GPIO_PB7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c4) |= 0x80    )
#define GPIO_PB7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005c6) ^= 0x80    )

#define GPIO_PC0_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x01    )
#define GPIO_PC0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x01    )
#define GPIO_PC0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x01    )
#define GPIO_PC1_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x02    )
#define GPIO_PC1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x02    )
#define GPIO_PC1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x02    )
#define GPIO_PC2_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x04    )
#define GPIO_PC2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x04    )
#define GPIO_PC2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x04    )
#define GPIO_PC3_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x08    )
#define GPIO_PC3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x08    )
#define GPIO_PC3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x08    )
#define GPIO_PC4_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x10    )
#define GPIO_PC4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x10    )
#define GPIO_PC4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x10    )
#define GPIO_PC5_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x20    )
#define GPIO_PC5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x20    )
#define GPIO_PC5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x20    )
#define GPIO_PC6_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x40    )
#define GPIO_PC6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x40    )
#define GPIO_PC6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x40    )
#define GPIO_PC7_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005c9) |= 0x80    )
#define GPIO_PC7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005c8) |= 0x80    )
#define GPIO_PC7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ca) ^= 0x80    )

#define GPIO_PD0_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x01    )
#define GPIO_PD0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x01    )
#define GPIO_PD0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x01    )
#define GPIO_PD1_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x02    )
#define GPIO_PD1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x02    )
#define GPIO_PD1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x02    )
#define GPIO_PD2_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x04    )
#define GPIO_PD2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x04    )
#define GPIO_PD2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x04    )
#define GPIO_PD3_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x08    )
#define GPIO_PD3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x08    )
#define GPIO_PD3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x08    )
#define GPIO_PD4_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x10    )
#define GPIO_PD4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x10    )
#define GPIO_PD4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x10    )
#define GPIO_PD5_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x20    )
#define GPIO_PD5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x20    )
#define GPIO_PD5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x20    )
#define GPIO_PD6_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x40    )
#define GPIO_PD6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x40    )
#define GPIO_PD6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x40    )
#define GPIO_PD7_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005cd) |= 0x80    )
#define GPIO_PD7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005cc) |= 0x80    )
#define GPIO_PD7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005ce) ^= 0x80    )

#define GPIO_PE0_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d1) |= 0x01    )
#define GPIO_PE0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d0) |= 0x01    )
#define GPIO_PE0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d2) ^= 0x01    )
#define GPIO_PE1_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d1) |= 0x02    )
#define GPIO_PE1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d0) |= 0x02    )
#define GPIO_PE1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d2) ^= 0x02    )

#define GPIO_PF0_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d5) |= 0x01    )
#define GPIO_PF0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d4) |= 0x01    )
#define GPIO_PF0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d6) ^= 0x01    )
#define GPIO_PF1_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d5) |= 0x02    )
#define GPIO_PF1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d4) |= 0x02    )
#define GPIO_PF1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d6) ^= 0x02    )
#define GPIO_PF2_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d5) |= 0x04    )
#define GPIO_PF2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d4) |= 0x04    )
#define GPIO_PF2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d6) ^= 0x04    )
#define GPIO_PF3_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d5) |= 0x08    )
#define GPIO_PF3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d4) |= 0x08    )
#define GPIO_PF3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d6) ^= 0x08    )
#define GPIO_PF4_OUTPUT_LOW			( (*(volatile unsigned char *)0x8005d5) |= 0x10    )
#define GPIO_PF4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x8005d4) |= 0x10    )
#define GPIO_PF4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x8005d6) ^= 0x10    )

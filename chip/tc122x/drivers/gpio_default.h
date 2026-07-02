/********************************************************************************************************
 * @file    gpio_default.h
 *
 * @brief   This is the header file for TC122X
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
#pragma once

#define FLASH_BIN_ON        1


#ifndef PA0_INPUT_ENABLE
#define PA0_INPUT_ENABLE	0
#endif
#ifndef PA1_INPUT_ENABLE
#define PA1_INPUT_ENABLE	0
#endif
#ifndef PA2_INPUT_ENABLE
#define PA2_INPUT_ENABLE	0
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
#ifndef PA0_DATA_STRENGTH
#define PA0_DATA_STRENGTH	1
#endif
#ifndef PA1_DATA_STRENGTH
#define PA1_DATA_STRENGTH	1
#endif
#ifndef PA2_DATA_STRENGTH
#define PA2_DATA_STRENGTH	1
#endif
#ifndef PA3_DATA_STRENGTH
#define PA3_DATA_STRENGTH	1
#endif
#ifndef PA4_DATA_STRENGTH
#define PA4_DATA_STRENGTH	1
#endif
#ifndef PA5_DATA_STRENGTH
#define PA5_DATA_STRENGTH	1
#endif
#ifndef PA6_DATA_STRENGTH
#define PA6_DATA_STRENGTH	1
#endif
#ifndef PA7_DATA_STRENGTH
#define PA7_DATA_STRENGTH	1
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
#define PA3_FUNC	AS_SWIRE
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
#define PULL_WAKEUP_SRC_PA0	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PA1
#define PULL_WAKEUP_SRC_PA1	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PA2
#define PULL_WAKEUP_SRC_PA2	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PA3
#define PULL_WAKEUP_SRC_PA3	 PM_PIN_PULLUP_1M	//sws pullup
#endif
#ifndef PULL_WAKEUP_SRC_PA4
#define PULL_WAKEUP_SRC_PA4	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PA5
#define PULL_WAKEUP_SRC_PA5	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PA6
#define PULL_WAKEUP_SRC_PA6	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PA7
#define PULL_WAKEUP_SRC_PA7	PM_PIN_PULLDOWN_100K
#endif

//////////////////////////////////////////////////
#ifndef PB0_INPUT_ENABLE
#define PB0_INPUT_ENABLE	0
#endif
#ifndef PB1_INPUT_ENABLE
#define PB1_INPUT_ENABLE	0
#endif
#ifndef PB2_INPUT_ENABLE
#define PB2_INPUT_ENABLE	0
#endif
#ifndef PB3_INPUT_ENABLE
#define PB3_INPUT_ENABLE	0    //MSPI
#endif
#ifndef PB4_INPUT_ENABLE
#define PB4_INPUT_ENABLE	0    //MSPI
#endif
#ifndef PB5_INPUT_ENABLE
#define PB5_INPUT_ENABLE	0    //MSPI
#endif
#ifndef PB6_INPUT_ENABLE
#define PB6_INPUT_ENABLE	0    //MSPI
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
#ifndef PB0_DATA_STRENGTH
#define PB0_DATA_STRENGTH	1
#endif
#ifndef PB1_DATA_STRENGTH
#define PB1_DATA_STRENGTH	1
#endif
#ifndef PB2_DATA_STRENGTH
#define PB2_DATA_STRENGTH	1
#endif
#ifndef PB3_DATA_STRENGTH
#define PB3_DATA_STRENGTH	1
#endif
#ifndef PB4_DATA_STRENGTH
#define PB4_DATA_STRENGTH	1
#endif
#ifndef PB5_DATA_STRENGTH
#define PB5_DATA_STRENGTH	1
#endif
#ifndef PB6_DATA_STRENGTH
#define PB6_DATA_STRENGTH	1
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
#define PB3_FUNC	AS_MSPI
#endif
#ifndef PB4_FUNC
#define PB4_FUNC	AS_MSPI
#endif
#ifndef PB5_FUNC
#define PB5_FUNC	AS_MSPI
#endif
#ifndef PB6_FUNC
#define PB6_FUNC	AS_MSPI
#endif
#ifndef PULL_WAKEUP_SRC_PB0
#define PULL_WAKEUP_SRC_PB0	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PB1
#define PULL_WAKEUP_SRC_PB1	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PB2
#define PULL_WAKEUP_SRC_PB2	PM_PIN_PULLDOWN_100K
#endif
#if (FLASH_BIN_ON == 0)
#ifndef PULL_WAKEUP_SRC_PB3
#define PULL_WAKEUP_SRC_PB3	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PB4
#define PULL_WAKEUP_SRC_PB4	PM_PIN_PULLDOWN_100K
#endif
#ifndef PULL_WAKEUP_SRC_PB5
#define PULL_WAKEUP_SRC_PB5	PM_PIN_PULLUP_10K
#endif
#ifndef PULL_WAKEUP_SRC_PB6
#define PULL_WAKEUP_SRC_PB6	PM_PIN_PULLDOWN_100K
#endif
#else
#ifndef PULL_WAKEUP_SRC_PB3
#define PULL_WAKEUP_SRC_PB3	PM_PIN_UP_DOWN_FLOAT
#endif
#ifndef PULL_WAKEUP_SRC_PB4
#define PULL_WAKEUP_SRC_PB4	PM_PIN_UP_DOWN_FLOAT
#endif
#ifndef PULL_WAKEUP_SRC_PB5
#define PULL_WAKEUP_SRC_PB5	PM_PIN_PULLUP_10K
#endif
#ifndef PULL_WAKEUP_SRC_PB6
#define PULL_WAKEUP_SRC_PB6	PM_PIN_UP_DOWN_FLOAT
#endif
#endif


#define GPIO_PA0_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x01 )
#define GPIO_PA0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x01 )
#define GPIO_PA0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x01 )
#define GPIO_PA1_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x02 )
#define GPIO_PA1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x02 )
#define GPIO_PA1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x02 )
#define GPIO_PA2_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x04 )
#define GPIO_PA2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x04 )
#define GPIO_PA2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x04 )
#define GPIO_PA3_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x08 )
#define GPIO_PA3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x08 )
#define GPIO_PA3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x08 )
#define GPIO_PA4_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x10 )
#define GPIO_PA4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x10 )
#define GPIO_PA4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x10 )
#define GPIO_PA5_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x20 )
#define GPIO_PA5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x20 )
#define GPIO_PA5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x20 )
#define GPIO_PA6_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x40 )
#define GPIO_PA6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x40 )
#define GPIO_PA6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x40 )
#define GPIO_PA7_OUTPUT_LOW			( (*(volatile unsigned char *)0x800521) |= 0x80 )
#define GPIO_PA7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800520) |= 0x80 )
#define GPIO_PA7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800522) |= 0x80 )

#define GPIO_PB0_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x01 )
#define GPIO_PB0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x01 )
#define GPIO_PB0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x01 )
#define GPIO_PB1_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x02 )
#define GPIO_PB1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x02 )
#define GPIO_PB1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x02 )
#define GPIO_PB2_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x04 )
#define GPIO_PB2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x04 )
#define GPIO_PB2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x04 )
#define GPIO_PB3_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x08 )
#define GPIO_PB3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x08 )
#define GPIO_PB3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x08 )
#define GPIO_PB4_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x10 )
#define GPIO_PB4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x10 )
#define GPIO_PB4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x10 )
#define GPIO_PB5_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x20 )
#define GPIO_PB5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x20 )
#define GPIO_PB5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x20 )
#define GPIO_PB6_OUTPUT_LOW			( (*(volatile unsigned char *)0x800525) |= 0x40 )
#define GPIO_PB6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800524) |= 0x40 )
#define GPIO_PB6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800526) |= 0x40 )

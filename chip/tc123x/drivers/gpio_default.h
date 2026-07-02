/********************************************************************************************************
 * @file    gpio_default.h
 *
 * @brief   This is the header file for TC123X
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




#ifndef PA0_INPUT_ENABLE
#define PA0_INPUT_ENABLE	0
#endif
#ifndef PA1_INPUT_ENABLE
#define PA1_INPUT_ENABLE	0 //A0 chip:INPUT_ENABLE always disable for PA1, other chip: defined by user.
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
#define PA1_OUTPUT_ENABLE	0 //A0 chip:OUTPUT_ENABLE always disable for PA1, other chip: defined by user.
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
#define PA1_DATA_STRENGTH	1 //A0 chip:DATA_STRENGTH default value, other chip: defined by user.
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
#define PA1_DATA_OUT	0 //A0 chip:DATA_OUT default value, other chip: defined by user.
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
#define PA1_FUNC	AS_GPIO //A0 chip:AS_GPIO FUNC, other chip: defined by user.
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
#define PULL_WAKEUP_SRC_PA0	0
#endif
#ifndef PULL_WAKEUP_SRC_PA1
#define PULL_WAKEUP_SRC_PA1	PM_PIN_PULLUP_1M //A0 chip:PM_PIN_PULLUP_1M pullup, other chip: defined by user.
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
#define PB0_INPUT_ENABLE	0
#endif
#ifndef PB1_INPUT_ENABLE
#define PB1_INPUT_ENABLE	0
#endif
#ifndef PB2_INPUT_ENABLE
#define PB2_INPUT_ENABLE	0
#endif
#ifndef PB3_INPUT_ENABLE
#define PB3_INPUT_ENABLE	0    
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
#ifndef PB7_DATA_STRENGTH
#define PB7_DATA_STRENGTH	1
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
#define PC2_INPUT_ENABLE	1    //SPI
#endif
#ifndef PC3_INPUT_ENABLE
#define PC3_INPUT_ENABLE	1    //SPI
#endif
#ifndef PC4_INPUT_ENABLE
#define PC4_INPUT_ENABLE	1    //SPI
#endif
#ifndef PC5_INPUT_ENABLE
#define PC5_INPUT_ENABLE	1    //SPI
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

#ifndef PC0_DATA_STRENGTH
#define PC0_DATA_STRENGTH	1
#endif
#ifndef PC1_DATA_STRENGTH
#define PC1_DATA_STRENGTH	1
#endif
#ifndef PC2_DATA_STRENGTH
#define PC2_DATA_STRENGTH	1
#endif
#ifndef PC3_DATA_STRENGTH
#define PC3_DATA_STRENGTH	1
#endif
#ifndef PC4_DATA_STRENGTH
#define PC4_DATA_STRENGTH	1
#endif
#ifndef PC5_DATA_STRENGTH
#define PC5_DATA_STRENGTH	1
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
#ifndef PC0_FUNC
#define PC0_FUNC	AS_GPIO
#endif
#ifndef PC1_FUNC
#define PC1_FUNC	AS_GPIO
#endif
#ifndef PC2_FUNC
#define PC2_FUNC	AS_MSPI
#endif
#ifndef PC3_FUNC
#define PC3_FUNC	AS_MSPI
#endif
#ifndef PC4_FUNC
#define PC4_FUNC	AS_MSPI
#endif
#ifndef PC5_FUNC
#define PC5_FUNC	AS_MSPI
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

#define GPIO_PA0_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x01 )
#define GPIO_PA0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x01 )
#define GPIO_PA0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x01 )
#define GPIO_PA1_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x02 )
#define GPIO_PA1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x02 )
#define GPIO_PA1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x02 )
#define GPIO_PA2_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x04 )
#define GPIO_PA2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x04 )
#define GPIO_PA2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x04 )
#define GPIO_PA3_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x08 )
#define GPIO_PA3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x08 )
#define GPIO_PA3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x08 )
#define GPIO_PA4_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x10 )
#define GPIO_PA4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x10 )
#define GPIO_PA4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x10 )
#define GPIO_PA5_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x20 )
#define GPIO_PA5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x20 )
#define GPIO_PA5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x20 )
#define GPIO_PA6_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x40 )
#define GPIO_PA6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x40 )
#define GPIO_PA6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x40 )
#define GPIO_PA7_OUTPUT_LOW			( (*(volatile unsigned char *)0x80052d) = 0x80 )
#define GPIO_PA7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x80052c) = 0x80 )
#define GPIO_PA7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x80052e) = 0x80 )


#define GPIO_PB0_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x01 )
#define GPIO_PB0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x01 )
#define GPIO_PB0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x01 )
#define GPIO_PB1_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x02 )
#define GPIO_PB1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x02 )
#define GPIO_PB1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x02 )
#define GPIO_PB2_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x04 )
#define GPIO_PB2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x04 )
#define GPIO_PB2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x04 )
#define GPIO_PB3_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x08 )
#define GPIO_PB3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x08 )
#define GPIO_PB3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x08 )

#define GPIO_PB4_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x10 )
#define GPIO_PB4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x10 )
#define GPIO_PB4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x10 )

#define GPIO_PB5_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x20 )
#define GPIO_PB5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x20 )
#define GPIO_PB5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x20 )

#define GPIO_PB6_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x40 )
#define GPIO_PB6_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x40 )
#define GPIO_PB6_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x40 )

#define GPIO_PB7_OUTPUT_LOW			( (*(volatile unsigned char *)0x800531) = 0x80 )
#define GPIO_PB7_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800530) = 0x80 )
#define GPIO_PB7_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800532) = 0x80 )


#define GPIO_PC0_OUTPUT_LOW			( (*(volatile unsigned char *)0x800535) = 0x01 )
#define GPIO_PC0_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800534) = 0x01 )
#define GPIO_PC0_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800536) = 0x01 )

#define GPIO_PC1_OUTPUT_LOW			( (*(volatile unsigned char *)0x800535) = 0x02 )
#define GPIO_PC1_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800534) = 0x02 )
#define GPIO_PC1_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800536) = 0x02 )

#define GPIO_PC2_OUTPUT_LOW			( (*(volatile unsigned char *)0x800535) = 0x04 )
#define GPIO_PC2_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800534) = 0x04 )
#define GPIO_PC2_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800536) = 0x04 )

#define GPIO_PC3_OUTPUT_LOW			( (*(volatile unsigned char *)0x800535) = 0x08 )
#define GPIO_PC3_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800534) = 0x08 )
#define GPIO_PC3_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800536) = 0x08 )

#define GPIO_PC4_OUTPUT_LOW			( (*(volatile unsigned char *)0x800535) = 0x10 )
#define GPIO_PC4_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800534) = 0x10 )
#define GPIO_PC4_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800536) = 0x10 )

#define GPIO_PC5_OUTPUT_LOW			( (*(volatile unsigned char *)0x800535) = 0x20 )
#define GPIO_PC5_OUTPUT_HIGH		( (*(volatile unsigned char *)0x800534) = 0x20 )
#define GPIO_PC5_OUTPUT_TOGGLE		( (*(volatile unsigned char *)0x800536) = 0x20 )

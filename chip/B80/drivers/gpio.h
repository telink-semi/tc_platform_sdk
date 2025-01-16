/********************************************************************************************************
 * @file    gpio.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "register.h"
#include "gpio.h"
#include "gpio_default.h"
#include "analog.h"
#include "usbhw.h"
#include "types.h"

/* For compatibility, usb_set_pin_en() is equivalent to usb_set_pin(1), configure the usb pin and enable the dp_through_swire function.*/
#define usb_set_pin_en()    usb_set_pin(1)

/*
    gpio lookup table
    -support  -not support DF- (support and default)
 					 PA0 PA1 PA2 PA3 PA4 PA5 PA6 PA7 PB0 PB1 PB2 PB3 PB4 PB5 PB6 PB7 PC0 PC1 PC2 PC3 PC4 PC5 PC6 PC7 PD0 PD1 PD2 PD3 PD4 PD5 PD6 PD7 PE0 PE1 PE2 PE3 PF0 PF1
    WIFI_DENY_I		     	  	     	  	  	  	  	  	  	  	  	           	  	              	  	  	  	  	  	  	     	  	     	  	  	  	  
	BLE_STATUS		     	  	     	  	  	  	  	  	  	  	  	           	  	              	  	  	     	  	  	     	  	  	  	  	  	  	  
	BLE_ACTIVITY         	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	     	  	  	  	  	  	  	  
	SPI_CSN              	  	        	  	     DF  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	SPI_CLK			     	  	     	  	  	  	  	  DF  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	SPI_MOSI_IO0	     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	     DF  
	SPI_MISO_IO1	     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  DF
	SWM				     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	TX_CYC2PA            	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	RX_CYC2LNA           	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	ANT_SEL2             	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	ANT_SEL1             	  	     	     	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	ANT_SEL0             	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	UART_RTX             	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	CLK_7816             	  	           	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	I2C_SDA              	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	I2C_SCL              	  	     	     	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	UART_RX              	  	     	  	  	  	  	     	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	UART_TX              	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	UART_RTS             	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	UART_CTS             	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM5_N               	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM4_N               	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM3_N               	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM2_N               	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM1_N			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM0_N			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM5			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM4			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM3			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM1			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	PWM0			     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	DBG				     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	KEY_SCAN		     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	DM		  		     DF  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	DP		  		     	  DF     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	SWS		  		     	     DF  	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	SPI_WP_IO2	  	     	  	     	     	  	  	  	  	  DF              	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
	SPI_HOLD_IO3	     	  	     	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	     DF  	  	  	  	  	  	  	  	  
	MSPI_MOSI		     	        	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	     DF  	  	  	  	  
	MSPI_MCLK		     	        	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  DF  	  	  	  
	MSPI_MSCN		     	        	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	  DF  	  	  
	MSPI_MISO		     	        	  	  	  	  	  	  	  	  	           	  	           	  	  	  	  	  	  	  	  	  	  	  	  	     DF  	  
*/
/**
 *  @brief  Define GPIO types.
 * 	@note	the following two point need to noticed when using PB0, PB1, PB3, PD4 and PF0 GPIO ports:
 *  		1. These pins are not recommend to use as wake-up source;
 *  		2. Since these pins are output functions by default, even if they are configured with pull-up/pull-down retention,
 *  		when deep/deep Retention is invoked, they can't maintain high/low level and an abnormal level will occur.
 *  		Therefore, these pins can't be used in applications where a certain level state needs to be maintained all the time.
 */
typedef enum{
		GPIO_GROUPA    = 0x000,
		GPIO_GROUPB    = 0x100,
		GPIO_GROUPC    = 0x200,
		GPIO_GROUPD    = 0x300,
		GPIO_GROUPE    = 0x400,
		GPIO_GROUPF    = 0x500,

	    GPIO_PA0 = GPIO_GROUPA | BIT(0),
		GPIO_PA1 = GPIO_GROUPA | BIT(1),     GPIO_DM=GPIO_PA1, // only support  UART_RX  DM
		GPIO_PA2 = GPIO_GROUPA | BIT(2),     GPIO_DP=GPIO_PA2, // only support  UART_TX  DP
		GPIO_PA3 = GPIO_GROUPA | BIT(3),     GPIO_SWS=GPIO_PA3,// only support  SWS
		GPIO_PA4 = GPIO_GROUPA | BIT(4),
		GPIO_PA5 = GPIO_GROUPA | BIT(5),
		GPIO_PA6 = GPIO_GROUPA | BIT(6),
		GPIO_PA7 = GPIO_GROUPA | BIT(7),
		GPIOA_ALL = GPIO_GROUPA | 0x00ff,

		GPIO_PB0 = GPIO_GROUPB | BIT(0),    GPIO_SPI_CN=GPIO_PB0, // only support SPI_CSN  DBG  KEY_SCAN
		GPIO_PB1 = GPIO_GROUPB | BIT(1),    GPIO_SPI_CK=GPIO_PB1, // only support SPI_CLK  KEY_SCAN
		GPIO_PB2 = GPIO_GROUPB | BIT(2),
		GPIO_PB3 = GPIO_GROUPB | BIT(3),    GPIO_SPI_IO2=GPIO_PB3,// only support SPI_WP_IO2  KEY_SCAN
		GPIO_PB4 = GPIO_GROUPB | BIT(4),
		GPIO_PB5 = GPIO_GROUPB | BIT(5),
		GPIO_PB6 = GPIO_GROUPB | BIT(6),
		GPIO_PB7 = GPIO_GROUPB | BIT(7),

		GPIO_PC0 = GPIO_GROUPC | BIT(0),
		GPIO_PC1 = GPIO_GROUPC | BIT(1),
		GPIO_PC2 = GPIO_GROUPC | BIT(2),
		GPIO_PC3 = GPIO_GROUPC | BIT(3),
		GPIO_PC4 = GPIO_GROUPC | BIT(4),
		GPIO_PC5 = GPIO_GROUPC | BIT(5),
		GPIO_PC6 = GPIO_GROUPC | BIT(6),
		GPIO_PC7 = GPIO_GROUPC | BIT(7),

		GPIO_PD0 = GPIO_GROUPD | BIT(0),
		GPIO_PD1 = GPIO_GROUPD | BIT(1),
		GPIO_PD2 = GPIO_GROUPD | BIT(2),
		GPIO_PD3 = GPIO_GROUPD | BIT(3),
		GPIO_PD4 = GPIO_GROUPD | BIT(4),   GPIO_SPI_IO3=GPIO_PD4, // only support  SPI_HOLD_IO3  KEY_SCAN
		GPIO_PD5 = GPIO_GROUPD | BIT(5),
		GPIO_PD6 = GPIO_GROUPD | BIT(6),
		GPIO_PD7 = GPIO_GROUPD | BIT(7),

		GPIO_PE0 = GPIO_GROUPE | BIT(0),  GPIO_MSPI_MISO=GPIO_PE0, // only support MSPI_MOSI
		GPIO_PE1 = GPIO_GROUPE | BIT(1),  GPIO_MSPI_MCLK=GPIO_PE1, // only support MSPI_MCLK
		GPIO_PE2 = GPIO_GROUPE | BIT(2),  GPIO_MSPI_MSCN=GPIO_PE2, // only support MSPI_MSCN
		GPIO_PE3 = GPIO_GROUPE | BIT(3),  GPIO_MSPI_MOSI=GPIO_PE3, // only support MSPI_MISO

		GPIO_PF1 = GPIO_GROUPF | BIT(1),  GPIO_SPI_MISO=GPIO_PF1,  // only support SPI_MOSI_IO0  KEY_SCAN
		GPIO_PF0 = GPIO_GROUPF | BIT(0),  GPIO_SPI_MOSI=GPIO_PF0,  // only support SPI_MISO_IO1  KEY_SCAN


		GPIO_ALL = 0x600,

		GPIO_NONE_PIN =0xfff,
}GPIO_PinTypeDef;

/**
 *  @brief  Define GPIO function mux types
 */

typedef enum{
	    AS_GPIO 	   =	0xff,

		DM 			   = 	0,
		DP 			   = 	0,
		SWS 		   = 	0,
		SPI_WP_IO2 	   = 	0,
		SPI_HOLD_IO3   = 	0,
		MSPI_MOSI	   = 	0,
		MSPI_MCLK 	   = 	0,
		MSPI_MSCN 	   = 	0,
		MSPI_MISO	   = 	0,

        KSCAN          =	1,
		DBG            =	2,

		PWM0           =	3,
		PWM1           =	4,
		PWM2           =	5,
		PWM3           =	6,
		PWM4           =	7,
		PWM5           =	8,

		PWM0_N         =	9,
		PWM1_N         =	10,
		PWM2_N         =	11,
		PWM3_N         =	12,
		PWM4_N         =	13,
		PWM5_N         =	14,

		UART_CTS_I     =	15,
		UART_RTS       =	16,
		UART_TX        =	17,
		UART_RX_I      =	18,

		I2C_SCL        =	19,
		I2C_SDA        =	20,

		CLK_7816       =	21,
		UART_RTX       =	22,

		ATSEL_0        =	23,
		ATSEL_1        =	24,
		ATSEL_2        =	25,

		RX_CYC2LNA     =	26,
		TX_CYC2PA      =	27,

		SWM	           =	28,

		SPI_MISO_IO1   =	29,
		SPI_MOSI_IO0   =	30,
		SPI_CLK        =	31,
		SPI_CSN        =	32,

		BLE_ACTIVITY   =	33,
		BLE_STATUS     =	34,
		WIFI_DENY_I    =	35,
#if (MCU_CORE_B80B)
		UART1_CTS_I    =    36,
		UART1_RTS      =    37,
		UART1_TX       =    38,
		UART1_RX_I     =    39,
	    CLK_78161      =    40,
		UART1_RTX      =    41,

		DBG_OTP_PCLK   =    42,
		DBG_OTP_PCE    =    42,
		DBG_OTP0_PPROG =    42,
		DBG_OTP0_PWE   =    42,
		DBG_OTP_PAS    =    42,
		DBG_OTP_PTR    =    42,
		DBG_OTP_PDSTB  =    42,
		DBG_OTP_PLDO   =    42,
		DBG_OTP_PTM0_3 =    42,//DBG_OTP_PTM0~DBG_OTP_PTM3
		DBG_OTP_PA0_11 =    42,//DBG_OTP_PA0~DBG_OTP_PA11

		DBG_OTP_PAIO0  =    42,//DBG_OTP_PAIO0

	    DBG_OTP_PAIO1_5  =    43,//DBG_OTP_PAIO1~DBG_OTP_PAIO5
		DBG_OTP_DAT0_19  =    43,//DBG_OTP_DAT0~DBG_OTP_DAT19
		DBG_OTP_DAT20_31 =    44,//DBG_OTP_DAT20~DBG_OTP_DAT31
		DBG_OTP0_DAT0_12 =    44,//DBG_OTP0_DAT0~DBG_OTP0_DAT12
		DBG_OTP0_DAT13_31=    45,//DBG_OTP0_DAT13~DBG_OTP0_DAT31
#endif
}gpio_func_e;

typedef enum{
	Level_Low=0,
	Level_High =1,
}GPIO_LevelTypeDef;

/**
 *  @brief  Define rising/falling types
 */
typedef enum{
	POL_RISING   = 0,  pol_rising = 0,
	POL_FALLING  = 1,  pol_falling = 1,
}GPIO_PolTypeDef;


/**
 *  @brief  Define src irq types
 */
typedef enum{
	 SRC_IRQ_RISING_EDGE=0,
	 SRC_IRQ_FALLING_EDGE ,
	 SRC_IRQ_HIGH_LEVEL,
	 SRC_IRQ_LOW_LEVEL,
} gpio_src_irq_trigger_type_e;

#if (MCU_CORE_B80B)
/**
 *  @brief  Define new irq risc number
 */
typedef enum{
    RISC3 = 3,
    RISC4 = 4,
    RISC5 = 5,
    RISC6 = 6,
    RISC7 = 7,
}gpio_irq_new_risc;
#endif

/**
 *  @brief  Define pull up or down types
 */
typedef enum {
	PM_PIN_UP_DOWN_FLOAT    = 0,
	PM_PIN_PULLUP_1M     	= 1,
	PM_PIN_PULLDOWN_100K  	= 2,
	PM_PIN_PULLUP_10K 		= 3,
}GPIO_PullTypeDef;


typedef enum{
		GPIO_GROUP_A    = 0,
		GPIO_GROUP_B    = 1,
		GPIO_GROUP_C    = 2,
		GPIO_GROUP_D    = 3,
		GPIO_GROUP_F    = 4,
}gpio_group_e;

/*
 *  @brief define gpio irq status types
 */
typedef enum{
	GPIO_IRQ_GPIO_STATUS           =   	BIT(18),
	GPIO_IRQ_GPIO2RISC0_STATUS     =    BIT(21),
	GPIO_IRQ_GPIO2RISC1_STATUS     =    BIT(22),
	GPIO_IRQ_GPIO2RISC2_STATUS     =    BIT(23),
}gpio_irq_status_e;

/*
 *  @brief define gpio irq mask types
 */
typedef enum{
	GPIO_IRQ_MASK_GPIO       = 			BIT(18),
	GPIO_IRQ_MASK_GPIO2RISC0 = 			BIT(21),
	GPIO_IRQ_MASK_GPIO2RISC1 = 			BIT(22),
	GPIO_IRQ_MASK_GPIO2RISC2 = 			BIT(23),
}gpio_irq_mask_e;

#if (MCU_CORE_B80)
/*
 *  @brief define gpio group irq types
 */
typedef enum{
	GPIO_GROUP_IRQ0                     = BIT(0),
	GPIO_GROUP_IRQ1                     = BIT(1),
	GPIO_GROUP_IRQ2                     = BIT(2),
	GPIO_GROUP_IRQ3                     = BIT(3),
	GPIO_GROUP_IRQ4                     = BIT(4),
	GPIO_GROUP_IRQ5                     = BIT(5),
	GPIO_GROUP_IRQ6                     = BIT(6),
	GPIO_GROUP_IRQ7                     = BIT(7),
}gpio_group_irq_e;
#elif (MCU_CORE_B80B)
typedef enum{
	GPIO_NEW_RISC3_IRQ                     = BIT(3),
	GPIO_NEW_RISC4_IRQ                     = BIT(4),
	GPIO_NEW_RISC5_IRQ                     = BIT(5),
	GPIO_NEW_RISC6_IRQ                     = BIT(6),
	GPIO_NEW_RISC7_IRQ                     = BIT(7),
}gpio_new_risc_irq_e;


#endif
/**
 * @brief      This function servers to initialization all gpio.
 * @param[in]  en  -  if mcu wake up from deep retention mode, it is NOT necessary to reset analog register
 * @return     none.
 */
/**Processing methods of unused GPIO
 * Set it to high resistance state and set it to open pull-up or pull-down resistance to
 * let it be in the determined state.When GPIO uses internal pull-up or pull-down resistance,
 * do not use pull-up or pull-down resistance on the board in the process of practical
 * application because it may have the risk of electric leakage .
 */
void gpio_init(int anaRes_init_en);


 /**
  * @brief      This function servers to set the GPIO's function.
  * @param[in]  pin - the special pin.
  * @param[in]  func - the function of GPIO.
  * @return     none.
  */
 /**Steps to set GPIO as a multiplexing function is as follows.
  * Step 1: Set GPIO as a multiplexing function.
  * Step 2: Disable GPIO function.
  * NOTE: Failure to follow the above steps may result in risks.
  */
 void gpio_set_func(GPIO_PinTypeDef pin, gpio_func_e func);

/**
 * @brief      This function set the output function of a pin.
 * @param[in]  pin - the pin needs to set the output function
 * @param[in]  value - enable or disable the pin's output function(0: disable, 1: enable)
 * @return     none
 */
static inline void gpio_set_output_en(GPIO_PinTypeDef pin, unsigned int value)
{
	unsigned char	bit = pin & 0xff;
	if(!value){
		BM_SET(reg_gpio_oen(pin), bit);
	}else{
		BM_CLR(reg_gpio_oen(pin), bit);
	}
}

/**
 * @brief      This function set the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function
 * @param[in]  value - enable or disable the pin's input function(0: disable, 1: enable)
 * @return     none
 */
void gpio_set_input_en(GPIO_PinTypeDef pin, unsigned int value);

/**
 * @brief      This function determines whether the output function of a pin is enabled.
 * @param[in]  pin - the pin needs to determine whether its output function is enabled.
 * @return     1: the pin's output function is enabled ;
 *             0: the pin's output function is disabled
 */
static inline int gpio_is_output_en(GPIO_PinTypeDef pin)
{
	return !BM_IS_SET(reg_gpio_oen(pin), pin & 0xff);
}

/**
 * @brief     This function determines whether the input function of a pin is enabled.
 * @param[in] pin - the pin needs to determine whether its input function is enabled.
 * @return    1: the pin's input function is enabled ;
 *            0: the pin's input function is disabled
 */

static inline int gpio_is_input_en(GPIO_PinTypeDef pin)
{
	return BM_IS_SET(reg_gpio_ie(pin), pin & 0xff);
}

/**
 * @brief     This function set the pin's output level.
 * @param[in] pin - the pin needs to set its output level
 * @param[in] value - value of the output level(1: high 0: low)
 * @return    none
 */
#if (MCU_CORE_B80)
static inline void gpio_write(GPIO_PinTypeDef pin, unsigned int value)
{
	unsigned char	bit = pin & 0xff;
	if(value){
		BM_SET(reg_gpio_out(pin), bit);
	}else{
		BM_CLR(reg_gpio_out(pin), bit);
	}
}
#elif (MCU_CORE_B80B)
static inline void gpio_write(GPIO_PinTypeDef pin, unsigned int value)
{
    unsigned char   bit = pin & 0xff;
    if(value)
    {
        reg_gpio_out_set(pin) = bit;
    }
    else{
        reg_gpio_out_clear(pin) = bit;
    }
}
#endif 

/**
 * @brief     This function set the pin toggle.
 * @param[in] pin - the pin needs to toggle
 * @return    none
 */
#if (MCU_CORE_B80)
static inline void gpio_toggle(GPIO_PinTypeDef pin)
{
	reg_gpio_out(pin) ^= (pin & 0xFF);
}
#elif (MCU_CORE_B80B)
static inline void gpio_toggle(GPIO_PinTypeDef pin)
{
    reg_gpio_out_toggle(pin) =pin & 0xff;
}
#endif

/**
 * @brief     This function read the pin's input/output level.
 * @param[in] pin - the pin needs to read its level
 * @return    the pin's level(1: high 0: low)
 */
static inline _Bool gpio_read(GPIO_PinTypeDef pin)
{
	return BM_IS_SET(reg_gpio_in(pin), pin & 0xff);
}
/**
 * @brief     This function read a pin's cache from the buffer.
 * @param[in] pin - the pin needs to read.
 * @param[in] p - the buffer from which to read the pin's level.
 * @return    the state of the pin.
 */
static inline unsigned int gpio_read_cache(GPIO_PinTypeDef pin, unsigned char *p)
{
	return p[pin>>8] & (pin & 0xff);
}

/**
 * @brief      This function read all the pins' input level.
 * @param[out] p - the buffer used to store all the pins' input level
 * @return     none
 */
static inline void gpio_read_all(unsigned char *p)
{
	p[0] = REG_ADDR8(0x500);
	p[1] = REG_ADDR8(0x508);
	p[2] = REG_ADDR8(0x510);
	p[3] = REG_ADDR8(0x518);
}


/**
 * @brief      This function serves to get gpio irq status.
 * @param[in]  status  - the pin needs to disable its IRQ.
 * @return     1:the interrupt status type is 1, 0: the interrupt status type is 0..
 */
static inline int gpio_get_irq_status(gpio_irq_status_e status)
{
	return (reg_irq_src & status);
}


/**
 * @brief      This function serves to clr gpio irq status.
 * @param[in]  status  - the pin needs to disable its IRQ.
 * @return     none.
 */
static inline void gpio_clr_irq_status(gpio_irq_status_e status)
{
	reg_irq_src=status;
}

/**
 * @brief      This function serves to enable gpio irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_irq_mask(gpio_irq_mask_e mask)
{
	BM_SET(reg_irq_mask, mask);
}

/**
 * @brief      This function serves to disable gpio irq mask function.
 *             if disable gpio interrupt,choose disable gpio mask , use interface gpio_clr_irq_mask.
 * @return     none.
 */
static inline void gpio_clr_irq_mask(gpio_irq_mask_e mask)
{
	BM_CLR(reg_irq_mask, mask);
}
#if (MCU_CORE_B80)
/**
 * @brief      This function serves to clr gpio group irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline char gpio_get_group_irq_status(gpio_group_irq_e status)
{
	return (reg_gpio_irq_from_pad & status);
}

/**
 * @brief      This function serves to clr gpio group irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline void gpio_clr_group_irq_status(gpio_group_irq_e status)
{
	reg_gpio_irq_from_pad = status;
}

/**
 * @brief      This function serves to enable gpio irq group mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_group_irq_mask(gpio_group_irq_e mask)
{
   BM_SET(reg_gpio_irq_pad_mask, mask);
}

/**
 * @brief      This function serves to disable gpio irq group mask function.
 *             if disable gpio src irq interrupt,can choose disable gpio_src_irq mask.
 * @return     none.
 */
static inline void gpio_clr_group_irq_mask(gpio_group_irq_e mask)
{
	BM_CLR(reg_gpio_irq_pad_mask, mask);
}
#elif (MCU_CORE_B80B)
/**
 * @brief      This function serves to clr new_risc irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline char gpio_get_new_risc_irq_status(gpio_new_risc_irq_e status)
{
	return (reg_gpio_irq_from_pad & status);
}

/**
 * @brief      This function serves to clr new_risc irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline void gpio_clr_new_risc_irq_status(gpio_new_risc_irq_e status)
{
	reg_gpio_irq_from_pad = status;
}

/**
 * @brief      This function serves to enable new_risc irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_new_risc_irq_mask(gpio_new_risc_irq_e mask)
{
   BM_SET(reg_gpio_irq_pad_mask, mask);
}

/**
 * @brief      This function serves to disable  new_risc irq mask function.
 *             if disable gpio interrupt,can choose disable this mask.
 * @return     none.
 */
static inline void gpio_clr_new_risc_irq_mask(gpio_new_risc_irq_e mask)
{
	BM_CLR(reg_gpio_irq_pad_mask, mask);
}
#endif
/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  value - the level of driving strength(1: strong 0: poor)
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin, unsigned int value);

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor, GPIOE group is not supported
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 * @note	* @note	the following two point need to noticed when using PB0, PB1, PB3, PD4 and PF0 GPIO ports:
 *  		1. These pins are not recommend to use as wake-up source;
 *  		2. Since these pins are output functions by default, even if they are configured with pull-up/pull-down retention,
 *  		when deep/deep Retention is invoked, they can't maintain high/low level and an abnormal level will occur.
 *  		Therefore, these pins can't be used in applications where a certain level state needs to be maintained all the time.
 */

void gpio_setup_up_down_resistor(GPIO_PinTypeDef gpio, GPIO_PullTypeDef up_down);

/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO, GPIOE group is not included in GPIO_ALL
 * @return     none.
 */
void gpio_shutdown(GPIO_PinTypeDef pin);

/**
 * @brief     This function set pin's 30k pull-up register.
 * @param[in] pin - the pin needs to set its pull-up register.
 * @return    none.
 * @attention This function sets the digital pull-up, it will not work after entering low power consumption.
 */
void gpio_set_pullup_res_30k(GPIO_PinTypeDef pin);

/**
 * @brief     This function set a pin's gpio irq interrupt,if need disable gpio interrupt,choose disable gpio mask,use interface gpio_clr_irq_mask.
 * @param[in] pin - the pin needs to enable its IRQ
 * @param[in] falling - value of the edge polarity(1: falling edge 0: rising edge)
 * @return    none
 */
static inline void gpio_set_interrupt(GPIO_PinTypeDef pin, GPIO_PolTypeDef falling)
{
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq_wakeup_en(pin), bit);

	reg_gpio_wakeup_irq |= FLD_GPIO_CORE_INTERRUPT_EN;
	if(falling == POL_FALLING){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,bit);
	}
	else if(falling == POL_RISING){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,bit);
	}
    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src = FLD_IRQ_GPIO_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1:enable 0:disable.
 * @return    none
 */
static inline void gpio_en_interrupt(GPIO_PinTypeDef pin, int en)   // reg_irq_mask: FLD_IRQ_GPIO_EN
{
	unsigned char	bit = pin & 0xff;
	if(en){
		BM_SET(reg_gpio_irq_wakeup_en(pin), bit);
	}
	else{
		BM_CLR(reg_gpio_irq_wakeup_en(pin), bit);
	}
}

/**
 * @brief     This function set a pin's gpio gpio2risc0 interrupt,if need disable gpio interrupt,choose disable gpio mask,use interface gpio_clr_irq_mask.
 * @param[in] pin - the pin needs to enable its IRQ
 * @param[in] falling - value of the edge polarity(1: falling edge 0: rising edge)
 * @return    none
 */
static inline void gpio_set_interrupt_risc0(GPIO_PinTypeDef pin, GPIO_PolTypeDef falling){
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc0_en(pin), bit);
	if(falling == POL_FALLING){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,bit);
	}
	else if(falling == POL_RISING){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,bit);
	}
/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_RISC0_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc0(GPIO_PinTypeDef pin, int en)  // reg_irq_mask: FLD_IRQ_GPIO_RISC0_EN
{
	unsigned char	bit = pin & 0xff;
	if(en){
		BM_SET(reg_gpio_irq_risc0_en(pin), bit);
	}
	else{
		BM_CLR(reg_gpio_irq_risc0_en(pin), bit);
	}
}

/**
 * @brief     This function set a pin's gpio gpio2risc1 interrupt,if need disable gpio interrupt,choose disable gpio mask,use interface gpio_clr_irq_mask.
 * @param[in] pin - the pin needs to enable its IRQ
 * @param[in] falling - value of the edge polarity(1: falling edge 0: rising edge)
 * @return    none
 */
static inline void gpio_set_interrupt_risc1(GPIO_PinTypeDef pin, GPIO_PolTypeDef falling)
{
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc1_en(pin), bit);

	if(falling == POL_FALLING){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,bit);
	}
	else if(falling == POL_RISING){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,bit);
	}
/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_RISC1_EN;
}


/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc1(GPIO_PinTypeDef pin, int en)  // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
{
	unsigned char	bit = pin & 0xff;
	if(en){
		BM_SET(reg_gpio_irq_risc1_en(pin), bit);
	}
	else{
		BM_CLR(reg_gpio_irq_risc1_en(pin), bit);
	}
}

/**
 * @brief     This function set a pin's gpio gpio2risc2 interrupt,if need disable gpio interrupt,choose disable gpio mask,use interface gpio_clr_irq_mask.
 * @param[in] pin - the pin needs to enable its IRQ
 * @param[in] falling - value of the edge polarity(1: falling edge 0: rising edge)
 * @return    none
 */
static inline void gpio_set_interrupt_risc2(GPIO_PinTypeDef pin, GPIO_PolTypeDef falling)
{
    unsigned char   bit = pin & 0xff;
    BM_SET(reg_gpio_irq_risc2_en(pin), bit);

    if(falling == POL_FALLING){
        BM_SET(reg_gpio_pol(pin), bit);
        BM_CLR(reg_gpio_irq_lvl,bit);
    }
    else if(falling == POL_RISING){
        BM_CLR(reg_gpio_pol(pin), bit);
        BM_CLR(reg_gpio_irq_lvl,bit);
    }
/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
    reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;
    reg_irq_mask |= FLD_IRQ_GPIO_RISC2_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc2(GPIO_PinTypeDef pin, int en)  // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
{
    unsigned char   bit = pin & 0xff;
    if(en){
        BM_SET(reg_gpio_irq_risc2_en(pin), bit);
    }
    else{
        BM_CLR(reg_gpio_irq_risc2_en(pin), bit);
    }
}

/**
 * @brief      This function enables or disables the internal pull-up resistor
 *             of DP pin of USB interface
 * @param[in]  En - enables or disables the internal pull-up resistor(1: enable 0: disable)
 * @return     none
 */
static inline void usb_dp_pullup_en (int en)
{
	unsigned char dat = analog_read(0x0b);
	if (en) {
		dat = dat | BIT(7);
	}
	else
	{
		dat = dat & 0x7f ;
	}

	analog_write (0x0b, dat);
}

/**
 * @brief      This function serves to set GPIO MUX function as DP and DM pin of USB.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 * @note       1. Configure usb_set_pin(0) , there are some risks, please refer to the startup.S file about DP_THROUGH_SWIRE_DIS
 *                for detailed description (by default dp_through_swire is disabled). Configure usb_set_pin(1) to enable dp_through_swire again.
 *             2. When dp_through_swire is enabled, Swire and USB applications do not affect each other.
 */
static inline void usb_set_pin(bool dp_through_swire)
{
    gpio_set_func(GPIO_PA2, 0); // DP
    gpio_set_func(GPIO_PA1, 0); // DM
    gpio_set_input_en(GPIO_PA2 | GPIO_PA1, 1); // DP/DM must set input enable
    usb_dp_pullup_en(1);
    /*                                      Note
     * If you want to enable the dp_through_swire function, there are the following considerations:
     * 1.configure dp_through_swire_en(1).
     * 2.keep DM high (external hardware burning EVK has pull-up function, no software configuration is needed).
     */
    dp_through_swire_en(dp_through_swire);
}

#if(MCU_CORE_B80)
/**
 * @brief     This function select the irq group source.
 * @param[in] group - gpio irq group,include group A,B,C,D,E,F.
 * @note      after you choose the gpio_group,you should set the pin's irq one by one,which correspond to the function "gpio_set_src_irq()".
 * @return    none.
 */
static inline void gpio_set_src_irq_group(gpio_group_e group)
{
	reg_gpio_irq_sel = group;
}
/**
 * @brief     This function set a pin's IRQ.
 * @param[in] pin 			- the pin needs to enable its IRQ.
 * @param[in] falling  - gpio interrupt type.
 * 							  0: rising edge.
 * 							  1: falling edge.
 * 							  2: high level.
 * 							  3: low level
 * @note      if you want to use this irq,you should select irq_group first,which correspond to the function "gpio_set_src_irq_group()".
 * @return    none.
 */
static inline void gpio_set_src_irq(GPIO_PinTypeDef pin, gpio_src_irq_trigger_type_e falling)
{
	    unsigned char	bit = pin & 0xff;

		switch(falling)
		{
		case SRC_IRQ_RISING_EDGE:
			BM_CLR(reg_gpio_pol(pin), bit);
			BM_CLR(reg_gpio_irq_lvl,bit);
		break;
		case SRC_IRQ_FALLING_EDGE:
			BM_SET(reg_gpio_pol(pin), bit);
			BM_CLR(reg_gpio_irq_lvl,bit);
		break;
		case SRC_IRQ_HIGH_LEVEL:
			BM_CLR(reg_gpio_pol(pin), bit);
			BM_SET(reg_gpio_irq_lvl,bit);
		break;
		case SRC_IRQ_LOW_LEVEL:
			BM_SET(reg_gpio_pol(pin), bit);
			BM_SET(reg_gpio_irq_lvl,bit);
		break;
		}
	   /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_gpio_irq_from_pad |=bit;//must clear, or it will cause to unexpected interrupt.
		reg_gpio_irq_pad_mask |=bit;
		reg_irq_mask |= FLD_IRQ_GPIO_NEW_EN;
}

#elif (MCU_CORE_B80B)
/**
 * @brief     This function select the irq type.
 * @param[in] pin     -  the pin needs to enables its IRQ function.
 * @param[in] falling -  value of the edge polarity(1: falling edge 0: rising edge)
 * @param[in] risc    -  new irq risc number(risc3~risc7)
 * @return    none.
 */
static inline void gpio_set_interrupt_new_risc(GPIO_PinTypeDef pin, gpio_src_irq_trigger_type_e falling, gpio_irq_new_risc risc)
{
    unsigned char	bit = pin & 0xff;
    BM_SET(reg_gpio_irq_risc3_risc7_en(pin,(risc-3)), bit);

        switch(falling)
        {
        case SRC_IRQ_RISING_EDGE:
            BM_CLR(reg_gpio_pol(pin), bit);
            BM_CLR(reg_gpio_irq_lvl,bit);
        break;
        case SRC_IRQ_FALLING_EDGE:
            BM_SET(reg_gpio_pol(pin), bit);
            BM_CLR(reg_gpio_irq_lvl,bit);
        break;
        case SRC_IRQ_HIGH_LEVEL:
            BM_CLR(reg_gpio_pol(pin), bit);
            BM_SET(reg_gpio_irq_lvl,bit);
        break;
        case SRC_IRQ_LOW_LEVEL:
            BM_SET(reg_gpio_pol(pin), bit);
            BM_SET(reg_gpio_irq_lvl,bit);
        break;
        }
        /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
        gpio_clr_new_risc_irq_status(BIT(risc));//must clear, or it will cause to unexpected interrupt.
        gpio_set_new_risc_irq_mask(BIT(risc));
        reg_irq_mask |= FLD_IRQ_GPIO_NEW_EN;
}
#endif




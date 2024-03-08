/********************************************************************************************************
 * @file	gpio.h
 *
 * @brief	This is the header file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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


/**
 *  @brief  Define GPIO types. 
 *  @note	the following two points need to noticed when using PA2 port and GPIOC groups(PC groups for A0,A1 chip):
 *  		1. This pin is not recommend to use as wake-up source;
 *  		2. Since this pin is output function by default, even if it is configured with pull-up/pull-down retention,
 *  		when deep/deep Retention is invoked, it can't maintain high/low level and an abnormal level will occur.
 *  		Therefore, this pin can't be used in applications where a certain level state needs to be maintained all the time.
 */

typedef enum{
		GPIO_GROUPA    = 0x000,
		GPIO_GROUPB    = 0x100,
		GPIO_GROUPC    = 0x200,
		GPIO_GROUPD    = 0x300,
		GPIO_GROUPE    = 0x400,

	    GPIO_PA0 = GPIO_GROUPA | BIT(0),//Because the IO of A0 has internal up, so its can't use for pull_down/pull_up function.This BUG will be fixed in next chip version. 
		GPIO_PA1 = GPIO_GROUPA | BIT(1),
		GPIO_PA2 = GPIO_GROUPA | BIT(2),
		GPIO_PA3 = GPIO_GROUPA | BIT(3),
		GPIO_PA4 = GPIO_GROUPA | BIT(4),
		GPIO_PA5 = GPIO_GROUPA | BIT(5),
		GPIO_PA6 = GPIO_GROUPA | BIT(6),
		GPIO_PA7 = GPIO_GROUPA | BIT(7), 	GPIO_SWS=GPIO_PA7,
		GPIOA_ALL = GPIO_GROUPA | 0x00ff,

		GPIO_PB0 = GPIO_GROUPB | BIT(0),
		GPIO_PB1 = GPIO_GROUPB | BIT(1),
		GPIO_PB2 = GPIO_GROUPB | BIT(2),
		GPIO_PB3 = GPIO_GROUPB | BIT(3),
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

		GPIO_PD0 = GPIO_GROUPD | BIT(0),//Because the IO of D0 has internal up, so its can't use for pull_down/pull_up function.This BUG will be fixed in next chip version. 
		GPIO_PD1 = GPIO_GROUPD | BIT(1),//Because the IO of D1 has internal up, so its can't use for pull_down/pull_up function.This BUG will be fixed in next chip version. 
		GPIO_PD2 = GPIO_GROUPD | BIT(2),
		GPIO_PD3 = GPIO_GROUPD | BIT(3),
		GPIO_PD4 = GPIO_GROUPD | BIT(4),
		GPIO_PD5 = GPIO_GROUPD | BIT(5),
		GPIO_PD6 = GPIO_GROUPD | BIT(6),
		GPIO_PD7 = GPIO_GROUPD | BIT(7),//Because the IO of D7 has internal up, so its can't use for pull_down/pull_up function.This BUG will be fixed in next chip version. 

		GPIO_PE0 = GPIO_GROUPE | BIT(0),  GPIO_MISO=GPIO_PE0,
		GPIO_PE1 = GPIO_GROUPE | BIT(1),  GPIO_MCLK=GPIO_PE1,
		GPIO_PE2 = GPIO_GROUPE | BIT(2),  GPIO_MSCN=GPIO_PE2,
		GPIO_PE3 = GPIO_GROUPE | BIT(3),  GPIO_MOSI=GPIO_PE3,

		GPIO_ALL = 0x500,
}GPIO_PinTypeDef;

/**
 *  @brief  Define GPIO Function types
 */

typedef enum{
	AS_GPIO 	=  0xff,
	AS_MSPI 	=  0xfe,
	AS_SWIRE	=  0xfd,
/**************SPI**************/
	PA0_SPI_DI      = 0,
	PA1_SPI_CN      = 0,
	PA2_SPI_DO      = 0,
	PA4_SPI_CK      = BIT(1),
	PA5_SPI_CK      = 0,
	PA6_SPI_DI      = BIT(1),
	PB0_SPI_DO      = BIT(1)|BIT(0),
	PB1_SPI_DO      = BIT(1),
	PB5_SPI_DO      = BIT(0),
	PB6_SPI_DI      = BIT(0),
	PB7_SPI_CN      = BIT(0),
	PD0_SPI_CK      = BIT(1),
	PD1_SPI_CN      = BIT(1),
	PD3_SPI_DO      = BIT(0),
	PD4_SPI_DI      = BIT(1)|BIT(0),

/**************I2C**************/
	PA0_I2C_SDA     = BIT(0),
	PA1_I2C_SCL     = BIT(0),
	PA2_I2C_SDA     = BIT(0),
	PA4_I2C_SCL     = BIT(1)|BIT(0),
	PA5_I2C_SDA     = BIT(0),
	PB0_I2C_SCL     = BIT(0),
	PB3_I2C_SDA     = BIT(0),
	PB3_I2C_SCL     = BIT(2),
	PB5_I2C_SDA     = BIT(1),
	PB7_I2C_SCL     = BIT(1),
	PD6_I2C_SCL     = BIT(1),
	PD7_I2C_SDA     = BIT(1),
/**************UART**************/
    PA0_UART_RX     = BIT(1),
    PA1_UART_TX     = BIT(1),
    PA2_UART_RTS    = BIT(1),
    PA3_UART_CTS    = BIT(1),
    PA5_UART_RX     = BIT(1),
    PB0_UART_TX     = BIT(1),
    PB0_UART_RX     = BIT(2),
    PB1_UART_RX     = BIT(0),
    PB2_UART_7816_CLK = BIT(0),
    PB3_UART_RX     = BIT(1),
    PB3_UART_TX     = BIT(0)|BIT(1),
    PB3_UART_CTS    = BIT(2)|BIT(0),
    PB4_UART_TX     = BIT(0),
    PB4_UART_CTS    = BIT(0)|BIT(1),
    PB4_UART_RX     = BIT(2),
    PB4_UART_RTS    = BIT(2)|BIT(0),
    PB5_UART_RTS    = BIT(0)|BIT(1),
    PB6_UART_TX     = BIT(1),
    PB6_UART_CTS    = BIT(0)|BIT(1),
    PB6_UART_7816_CLK = BIT(3)|BIT(0),
    PC0_UART_RX     = BIT(0),
    PC0_UART_TX     = BIT(1),
    PC0_UART_RTS    = BIT(1)|BIT(0),
    PC0_UART_CTS    = BIT(2),
    PC2_UART_CTS    = BIT(0),
    PC2_UART_RTS    = BIT(1),
    PC2_UART_RX     = BIT(0)|BIT(1),
    PD2_UART_RTS    = BIT(0),
    PD2_UART_CTS    = BIT(1),
    PD3_UART_RX     = BIT(1),
    PD3_UART_RTS    = BIT(0)|BIT(1),
    PD4_UART_TX     = BIT(1),
    PD5_UART_RTS    = BIT(1),
/**************PWM**************/
    PA0_PWM0_N      = BIT(0)|BIT(1),
    PA1_PWM0        = BIT(0)|BIT(1),
    PA1_PWM1        = BIT(2),
    PA2_PWM1        = BIT(0)|BIT(1),
    PA3_PWM1_N      = BIT(0)|BIT(1),
    PA5_PWM1        = BIT(0)|BIT(1),
    PA5_PWM2        = BIT(2),
    PA6_PWM2_N      = BIT(0)|BIT(1),
    PB4_PWM0        = BIT(1),
    PB6_PWM2        = BIT(2),
    PB6_PWM1        = BIT(2)|BIT(0),
    PB6_PWM0_N      = BIT(2)|BIT(1),
    PB6_PWM0        = BIT(2)|BIT(1)|BIT(0),
    PB6_PWM1_N      = BIT(3),
    PB7_PWM2_N      = BIT(1)|BIT(0),
    PC0_PWM0        = BIT(2)|BIT(0),
    PC0_PWM1        = BIT(2)|BIT(1),
    PC2_PWM0        = BIT(2),
    PC2_PWM1        = BIT(2)|BIT(0),
    PC2_PWM2        = BIT(2)|BIT(1),
    PD0_PWM2        = BIT(0),
    PD1_PWM2_N      = BIT(0),
    PD2_PWM0        = BIT(0)|BIT(1),
    PD2_PWM1        = BIT(2),
    PD2_PWM2        = BIT(2)|BIT(0),
    PD2_PWM0_N      = BIT(2)|BIT(1),
    PD3_PWM0        = BIT(2),
    PD3_PWM1        = BIT(2)|BIT(0),
    PD3_PWM2        = BIT(2)|BIT(1),
    PD3_PWM0_N      = BIT(2)|BIT(1)|BIT(0),
    PD3_PWM1_N      = BIT(3),
    PD3_PWM2_N      = BIT(3)|BIT(0),
    PD4_PWM0_N      = BIT(0),
    PD5_PWM0        = BIT(0),
    PD5_PWM1        = BIT(1)|BIT(0),
    PD5_PWM2        = BIT(2),
    PD5_PWM0_N      = BIT(2)|BIT(0),
    PD5_PWM1_N      = BIT(2)|BIT(1),
    PD6_PWM1        = BIT(0),
    PD7_PWM1_N      = BIT(0),
/**************PTA**************/
    PA3_BLE_PRIORITY= BIT(0),
    PB0_BLE_PRIORITY= BIT(2)|BIT(0),
    PA3_BLE_ACTIVE  = BIT(0),
    PA4_BLE_STATUS  = BIT(0),
    PA6_WLAN_DENY   = BIT(0),
    PB0_BLE_ACTIVE  = BIT(2)|BIT(0),
    PB3_BLE_STATUS  = BIT(2)|BIT(1),
    PB4_WLAN_DENY   = BIT(2)|BIT(1),

/**************RF**************/
    PB2_RX_CYC2LNA  = BIT(1)|BIT(0),
    PB3_TX_CYC2PA   = BIT(2)|BIT(1)|BIT(0),
    PD0_RX_CYC2LNA  = BIT(1)|BIT(0),
    PD1_TX_CYC2PA   = BIT(1)|BIT(0),
/**************ANT**************/
    PB0_ANT_SEL0    = BIT(2)|BIT(1),
    PB1_ANT_SEL1    = BIT(1)|BIT(0),
    PB2_ANT_SEL2    = BIT(1),
    PD5_ANT_SEL0    = BIT(2)|BIT(1)|BIT(0),
    PD6_ANT_SEL1    = BIT(1)|BIT(0),
    PD7_ANT_SEL2    = BIT(1)|BIT(0),
/**************RESERVED**************/
    PA5_SWM         = BIT(2)|BIT(0),
    PA6_SWM         = BIT(0),

}GPIO_FuncTypeDef;


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
 *  @brief  Define pull up or down types
 */
typedef enum {
	PM_PIN_UP_DOWN_FLOAT    = 0,
	PM_PIN_PULLUP_1M     	= 1,
	PM_PIN_PULLDOWN_100K  	= 2,
	PM_PIN_PULLUP_10K 		= 3,
}GPIO_PullTypeDef;


/*
 *  @brief define gpio irq status types
 */
typedef enum{
	GPIO_IRQ_GPIO_STATUS           =   	BIT(18),
	GPIO_IRQ_GPIO2RISC0_STATUS     =    BIT(21),
	GPIO_IRQ_GPIO2RISC1_STATUS     =    BIT(22),
}gpio_irq_status_e;

/*
 *  @brief define gpio irq mask types
 */
typedef enum{
	GPIO_IRQ_MASK_GPIO       = 			BIT(18),
	GPIO_IRQ_MASK_GPIO2RISC0 = 			BIT(21),
	GPIO_IRQ_MASK_GPIO2RISC1 = 			BIT(22),

}gpio_irq_mask_e;
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
 * @brief      This function servers to set the GPIO's MUX function.
 * @param[in]  pin - the special pin.
 * @param[in]  func - the function of GPIO.
 * @return     none.
 */
 void gpio_set_mux(GPIO_PinTypeDef pin, GPIO_FuncTypeDef func);

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
void gpio_set_func(GPIO_PinTypeDef pin, GPIO_FuncTypeDef func);

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
static inline void gpio_write(GPIO_PinTypeDef pin, unsigned int value)
{
	unsigned char	bit = pin & 0xff;
	if(value){
		BM_SET(reg_gpio_out(pin), bit);
	}else{
		BM_CLR(reg_gpio_out(pin), bit);
	}
}

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
	p[0] = REG_ADDR8(0x580);
	p[1] = REG_ADDR8(0x588);
	p[2] = REG_ADDR8(0x590);
	p[3] = REG_ADDR8(0x598);
}

/**
 * @brief     This function set the pin toggle.
 * @param[in] pin - the pin needs to toggle
 * @return    none
 */
static inline void gpio_toggle(GPIO_PinTypeDef pin)
{
	reg_gpio_out(pin) ^= (pin & 0xFF);
}

/**
 * @brief      This function serves to get gpio irq status.
 * @param[in]  status  - the pin needs to disable its IRQ.
 * @return     1:the interrupt status type is 1, 0: the interrupt status type is 0..
 */
static inline char gpio_get_irq_status(gpio_irq_status_e status)
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

/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  value - the level of driving strength(1: strong 0: poor)
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin, unsigned int value);

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */

void gpio_setup_up_down_resistor(GPIO_PinTypeDef gpio, GPIO_PullTypeDef up_down);

/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO, GPIOE group is not included in GPIO_ALL
 * @return     none.
 */
void gpio_shutdown(GPIO_PinTypeDef pin);

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
	if(falling){
		BM_SET(reg_gpio_pol(pin), bit);
	}else{
		BM_CLR(reg_gpio_pol(pin), bit);
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
	if(falling){
		BM_SET(reg_gpio_pol(pin), bit);
	}else{
		BM_CLR(reg_gpio_pol(pin), bit);
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

	if(falling){
		BM_SET(reg_gpio_pol(pin), bit);
	}else{
		BM_CLR(reg_gpio_pol(pin), bit);
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


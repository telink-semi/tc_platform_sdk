/********************************************************************************************************
 * @file    gpio.h
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

#include "register.h"
#include "gpio_default.h"
#include "analog.h"
#include "types.h"

/**
 *  @brief  Define GPIO types.
 * 	@note	the following two point need to noticed when using PB0, PB1, PB3, PD4 and PF0 GPIO ports:
 *  		1. These pins are not recommend to use as wake-up source;
 *  		2. Since these pins are output functions by default, even if they are configured with pull-up/pull-down retention,
 *  		when deep/deep Retention is invoked, they can't maintain high/low level and an abnormal level will occur.
 *  		Therefore, these pins can't be used in applications where a certain level state needs to be maintained all the time.
 */
typedef enum
{
	GPIO_GROUPA = 0x000,
	GPIO_GROUPB = 0x100,
	GPIO_GROUPC = 0x200,

	GPIO_PA0 = GPIO_GROUPA | BIT(0),
    /* A0 chip: PA1 is connected to CS inside the chip and configured as a pull-up function for CS.
     * PA1 is no longer open to the public.
     */
	GPIO_PA1 = GPIO_GROUPA | BIT(1),
	GPIO_PA2 = GPIO_GROUPA | BIT(2),
	GPIO_PA3 = GPIO_GROUPA | BIT(3),
	GPIO_SWS = GPIO_PA3, // only support  SWS
	GPIO_PA4 = GPIO_GROUPA | BIT(4),
	GPIO_PA5 = GPIO_GROUPA | BIT(5),
	GPIO_PA6 = GPIO_GROUPA | BIT(6),
	GPIO_PA7 = GPIO_GROUPA | BIT(7),
	GPIOA_ALL = GPIO_GROUPA | 0x00ff,

	GPIO_PB0 = GPIO_GROUPB | BIT(0),
	GPIO_PB1 = GPIO_GROUPB | BIT(1),
	GPIO_PB2 = GPIO_GROUPB | BIT(2),
	GPIO_PB3 = GPIO_GROUPB | BIT(3),
	GPIO_PB4 = GPIO_GROUPB | BIT(4),
	GPIO_PB5 = GPIO_GROUPB | BIT(5),
	GPIO_PB6 = GPIO_GROUPB | BIT(6),
	GPIO_PB7 = GPIO_GROUPB | BIT(7),
	GPIOB_ALL = GPIO_GROUPB | 0x00ff,

	GPIO_PC0 = GPIO_GROUPC | BIT(0),
	GPIO_PC1 = GPIO_GROUPC | BIT(1),
	GPIO_PC2 = GPIO_GROUPC | BIT(2),
	GPIO_PC3 = GPIO_GROUPC | BIT(3),
	GPIO_PC4 = GPIO_GROUPC | BIT(4),
	GPIO_PC5 = GPIO_GROUPC | BIT(5),
	GPIOC_ALL = GPIO_GROUPC | 0x003f,

    GPIO_ALL = 0x300,
	GPIO_NONE_PIN = 0xfff,
} GPIO_PinTypeDef;

/**
 *  @brief  Define GPIO function mux types
 */
typedef enum
{
	PWM0		=1,
	PWM1		=2,
	PWM2		=3,
	PWM3		=4,
	PWM4        =5,
	PWM5        =6,
	UART_CTS_I  =7,
	UART_RTS    =8,
	UART_TX     =9,
	UART_RX_I   =10,
	I2C_SCL     =11,
	I2C_SDA     =12,
	SPI_DI_IO   =13,
	SPI_DO_IO   =14,
	SPI_CK_IO   =15,
	SPI_CN_IO   =16,
	UART1_CTS_I =17,
	UART1_RTS   =18,
	UART1_TX    =19,
	UART1_RX_I  =20,
	IR_LEARN_I  =21,

	AS_SWIRE    =0,
	AS_MSPI     =0,
	AS_PROBE_CLK=0,

	AS_GPIO     =0xff,
} gpio_func_e;

/**
 *  @brief  Define rising/falling types
 */
typedef enum
{
	POL_RISING = 0,
	pol_rising = 0,
	POL_FALLING = 1,
	pol_falling = 1,
	POL_HIGH,//RISC2 NOT SUPPORT
	POL_LOW,//RISC2 NOT SUPPORT
} GPIO_PolTypeDef;

typedef enum
{
	GPIO_GROUP_A = 0,
	GPIO_GROUP_B = 1,
} gpio_group_e;

/*
 *  @brief define gpio irq status types
 */
typedef enum
{
	GPIO_IRQ_GPIO2RISC0_STATUS     =    BIT(21)|BIT(14),
	GPIO_IRQ_GPIO2RISC1_STATUS     =    BIT(22)|BIT(15),
	GPIO_IRQ_GPIO2RISC2_STATUS     =    BIT(23),//RISC2 NOT SUPPORT LEVEL IRQ
} gpio_irq_status_e;

typedef enum
{
	GPIO_RISC0_IRQ_MASK = BIT(0),
	GPIO_RISC1_IRQ_MASK = BIT(1),
	GPIO_RISC2_IRQ_MASK = BIT(2),
} gpio_irq_mask_e;

typedef enum
{
    PROB_CLK_32K  = 0,
	PROB_CLK_7816 = 1,
	PROB_CLK_SYS  = 2,
}prob_clock_src_e;

/**
 *  @brief  Define pull up or down types
 */
typedef enum {
	PM_PIN_UP_DOWN_FLOAT    = 0,
	PM_PIN_PULLUP_1M     	= 1,
	PM_PIN_PULLDOWN_100K  	= 2,
	PM_PIN_PULLUP_10K 		= 3,
}GPIO_PullTypeDef;

/**
 * @brief      This function servers to set the GPIO's function.
 * @param[in]  pin - the special pin.
 * @param[in]  func - the function of GPIO.
 * @return     none.
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
	unsigned char bit = pin & 0xff;
	if (!value)
	{
		BM_SET(reg_gpio_oe(pin), bit);
	}else
	{
		BM_CLR(reg_gpio_oe(pin), bit);
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
	return !BM_IS_SET(reg_gpio_oe(pin), pin & 0xff);
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
	unsigned char bit = pin & 0xff;
	if (value)
	{
		reg_gpio_out_set(pin) = bit;
	}
	else
	{
		reg_gpio_out_clear(pin) = bit;
	}
}

/**
 * @brief     This function set the pin toggle.
 * @param[in] pin - the pin needs to toggle
 * @return    none
 */
static inline void gpio_toggle(GPIO_PinTypeDef pin)
{
	reg_gpio_out_toggle(pin) = pin & 0xff;
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
	return p[pin >> 8] & (pin & 0xff);
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
	reg_irq_src = status;//mcu layer irq
	reg_gpio_irq_status = status>>21;//gpio layer irq
}

/**
 * @brief      This function serves to clr gpio irq level status.
 * @param[in]  status  - the pin needs to disable its IRQ.
 * @return     none.
 */
static inline void gpio_clr_irq_lvl_status(gpio_irq_status_e status)
{
	reg_irq_src = status;//mcu layer irq
	reg_gpio_irq_status = (status>>14 & 0X07);//gpio layer irq
}

/**
 * @brief      This function serves to enable gpio irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_irq_mask(gpio_irq_status_e mask)
{
	BM_SET(reg_irq_mask, mask);
}

/**
 * @brief      This function serves to disable gpio irq mask function.
 *             if disable gpio interrupt,choose disable gpio mask , use interface gpio_clr_irq_mask.
 * @return     none.
 */
static inline void gpio_clr_irq_mask(gpio_irq_status_e mask)
{
	BM_CLR(reg_irq_mask, mask);
}

/**
 * @brief      This function serves to enable new_risk irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_new_irq_mask(gpio_irq_mask_e mask)
{
	BM_SET(reg_gpio_irq_pad_mask, mask);
}

/**
 * @brief      This function serves to disable  new_risc irq mask function.
 *             if disable gpio interrupt,can choose disable this mask.
 * @return     none.
 */
static inline void gpio_clr_new_irq_mask(gpio_irq_mask_e mask)
{
	BM_CLR(reg_gpio_irq_pad_mask, mask);
}

/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  en - 1: enable driving strength, 0:disable driving strength
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin,  unsigned int en);


/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO, GPIOE group is not included in GPIO_ALL
 * @return     none.
 */
void gpio_shutdown(GPIO_PinTypeDef pin);

static inline void gpio_set_interrupt_risc0(GPIO_PinTypeDef pin, GPIO_PolTypeDef trigger)
{
	unsigned char	bit = pin & 0xff;

	if(trigger == POL_FALLING){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(0));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EDGE_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC0_EDGE_EN;
	}
	else if(trigger == POL_RISING){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(0));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EDGE_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC0_EDGE_EN;
	}
	else if(trigger == POL_HIGH){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_SET(reg_gpio_irq_lvl,BIT(0));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC0_LVL_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC0_LVL_EN;
	}
	else if(trigger == POL_LOW){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_SET(reg_gpio_irq_lvl,BIT(0));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC0_LVL_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC0_LVL_EN;
	}
	BM_SET(reg_gpio_irq0_en(pin), bit);//reg_gpio_irq0_en must be configured after reg_gpio_pol to avoid unexpected interrupt.(Confirmed by Qiangkai)

	reg_gpio_irq_status = FLD_IRQ_RISC0;// must clear, or it will cause to unexpected interrupt.
	gpio_set_new_irq_mask(GPIO_RISC0_IRQ_MASK);
}

static inline void gpio_set_interrupt_risc1(GPIO_PinTypeDef pin, GPIO_PolTypeDef trigger)
{
	unsigned char	bit = pin & 0xff;

	if(trigger == POL_FALLING){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(1));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EDGE_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC1_EDGE_EN;
	}
	else if(trigger == POL_RISING){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(1));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EDGE_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC1_EDGE_EN;
	}
	else if(trigger == POL_HIGH){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_SET(reg_gpio_irq_lvl,BIT(1));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC1_LVL_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC1_LVL_EN;
	}
	else if(trigger == POL_LOW){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_SET(reg_gpio_irq_lvl,BIT(1));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC1_LVL_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC1_LVL_EN;
	}
	BM_SET(reg_gpio_irq1_en(pin), bit);//reg_gpio_irq1_en must be configured after reg_gpio_pol to avoid unexpected interrupt.(Confirmed by Qiangkai)

	reg_gpio_irq_status = FLD_IRQ_RISC1;// must clear, or it will cause to unexpected interrupt.
	gpio_set_new_irq_mask(GPIO_RISC1_IRQ_MASK);
}


static inline void gpio_set_interrupt_risc2(GPIO_PinTypeDef pin, GPIO_PolTypeDef trigger)
{
	unsigned char	bit = pin & 0xff;

	if(trigger == POL_FALLING){
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(2));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EDGE_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC2_EDGE_EN;
	}
	else if(trigger == POL_RISING){
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(2));
	    /*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EDGE_EN;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC2_EDGE_EN;
	}
	BM_SET(reg_gpio_irq2_en(pin), bit);//reg_gpio_irq2_en must be configured after reg_gpio_pol(Confirmed by Qiangkai)

	reg_gpio_irq_status = FLD_IRQ_RISC2;// must clear, or it will cause to unexpected interrupt.
	gpio_set_new_irq_mask(GPIO_RISC2_IRQ_MASK);
}

/**
 * @brief       This function is used to enable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_en(void)
{
	write_reg8(0x511, read_reg8(0x511) | 0x3c);
}

/**
 * @brief       This function is used to disable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_dis(void)
{
	 write_reg8(0x511, read_reg8(0x511) & (~0x3c)); //MSPI ie disable
}

/**
 * @brief      This function servers to initialization all gpio.
 * @param[in]  en  -  if mcu wake up from deep retention mode, determine whether it is NOT necessary to reset analog register
 *                    1: set analog register
 *                    0: not set analog register
 * @return     none.
 */
void gpio_init(int anaRes_init_en);

/**
 * @brief     This function performs to probe clock to IO.
 * @param[in] pin - the pin to probe clock, only support PA2.
 * @param[in] sel_clk - the clock source which you want to probe.
 * @return    none.
 */
void gpio_set_probe_clk_function(prob_clock_src_e sel_clk);

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */
void gpio_setup_up_down_resistor(GPIO_PinTypeDef gpio, GPIO_PullTypeDef up_down);

/**
 * @brief     This function set pin's 30k pull-up register.
 * @param[in] pin - the pin needs to set its pull-up register.
 * @param[in] value - enable or disable the pin's input function(0: disable, 1: enable)
 * @return    none.
 * @attention This function sets the digital pull-up, it will not work after entering low power consumption.
 */
void gpio_set_pullup_res_30k_en(GPIO_PinTypeDef pin,unsigned int value);

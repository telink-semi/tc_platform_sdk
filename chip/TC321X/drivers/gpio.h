/********************************************************************************************************
 * @file    gpio.h
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
	GPIO_GROUPD = 0x300,
	GPIO_GROUPE = 0x400,
	GPIO_GROUPF = 0x500,

	GPIO_PA0 = GPIO_GROUPA | BIT(0),
	GPIO_PA1 = GPIO_GROUPA | BIT(1),
	GPIO_PA2 = GPIO_GROUPA | BIT(2),
	GPIO_PA3 = GPIO_GROUPA | BIT(3),
	GPIO_SWS = GPIO_PA3,            //only support  SWS
	GPIO_PA4 = GPIO_GROUPA | BIT(4),
	GPIO_PA5 = GPIO_GROUPA | BIT(5),
	GPIO_PA6 = GPIO_GROUPA | BIT(6),
	GPIO_PA7 = GPIO_GROUPA | BIT(7),
	GPIOA_ALL = GPIO_GROUPA | 0x00ff,

	GPIO_PB0 = GPIO_GROUPB | BIT(0),
	GPIO_PB1 = GPIO_GROUPB | BIT(1),//only support SPI_CLK/KEY_SCAN
	GPIO_PB2 = GPIO_GROUPB | BIT(2),
	GPIO_PB3 = GPIO_GROUPB | BIT(3),//only support UART2_RTX_IO/KEY_SCAN
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
	GPIO_PD4 = GPIO_GROUPD | BIT(4),//only support IR_LEARN_I/KEY_SCAN
	GPIO_PD5 = GPIO_GROUPD | BIT(5),
	GPIO_PD6 = GPIO_GROUPD | BIT(6),
	GPIO_PD7 = GPIO_GROUPD | BIT(7),

	GPIO_PE0 = GPIO_GROUPE | BIT(0),//only support SPI_MOSI_IO/UART2_RX_I/CLK_7816/UART_RTX_IO/IR_LEARN_I/MIC_DAT
	GPIO_PE1 = GPIO_GROUPE | BIT(1),//only support SPI_MISO_IO/UART2_TX/CLK_7816/UART_RTX_IO/IR_LEARN_I/MIC_CLK

	GPIO_PF0 = GPIO_GROUPF | BIT(0),
	GPIO_MSPI_MISI = GPIO_PF0,      //only support MSPI_MOSI
	GPIO_PF1 = GPIO_GROUPF | BIT(1),
	GPIO_MSPI_MCLK = GPIO_PF1,      //only support MSPI_MCLK
	GPIO_PF2 = GPIO_GROUPF | BIT(2),
	GPIO_MSPI_MSCN = GPIO_PF2,      //only support MSPI_MSCN
	GPIO_PF3 = GPIO_GROUPF | BIT(3),
	GPIO_MSPI_MOSO = GPIO_PF3,      //only support MSPI_MISO

	GPIO_ALL = 0x600,

	GPIO_NONE_PIN = 0x00,
} GPIO_PinTypeDef;

/**
 *  @brief  Define GPIO function mux types
 */

typedef enum
{
	AS_GPIO = 0xff,
	PA_KS0_IO = 1,
	DBG0_IO = 2,
	PWM0 = 3,
	PWM1 = 4,
	PWM2 = 5,
	PWM3 = 6,
	PWM4 = 7,
	PWM5 = 8,
	UART_CTS_I = 9,
	UART_RTS = 10,
	UART_TX = 11,
	UART_RX_I = 12,
	I2C_SCL_IO = 13,
	I2C_SDA_IO = 14,
	RX_CYC2LNA = 15,
	TX_CYC2PA = 16,
	SPI_MISO_IO = 17,
	SPI_MOSI_IO = 18,
	SPI_CK_IO = 19,
	SPI_CN_IO = 20,
	UART2_TX = 21,
	UART2_RX_I =21,
	UART1_RX_I = 22,
	UART1_TX = 22,
	WIFI_DENY_I = 23,
	SDM_P0 = 24,
	SDM_N0 = 25,
	SDM_P1 = 26,
	SDM_N1 = 27,
	I2S_CK = 28,
	I2S_LR = 29,
	I2S_DO = 30,
	i2S_DI_I = 31,
	UART_PE_RTX_IO = 3, UART_RTX_IO = 15, UART1_RTX_IO = 16, UART2_RTX_IO = 17,BLE_ACTIVITY =23,BLE_STATUS=23,KSCAN = 0, SWS_IO =0 ,UART1_PA2_TX=0, UART1_PA1_RX, UART2_PB3_RTX_IO = 0, CLK_7816 = 0 ,MSPI_MISO=0,MSPI_MSCN=0,MSPI_MCLK=0,MSPI_MOSI=0,
	SPI_PB0_CN_IO=0,SPI_PB1_CK_IO=0,SPI_PE0_MOSI_IO=0,SPI_PE1_MIS0_IO=0,
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
} GPIO_PolTypeDef;

/**
 *  @brief  Define src irq types
 */
typedef enum
{
	SRC_IRQ_RISING_EDGE = 0,
	SRC_IRQ_FALLING_EDGE,
	SRC_IRQ_HIGH_LEVEL,
	SRC_IRQ_LOW_LEVEL,
} gpio_src_irq_trigger_type_e;

/**
 *  @brief  Define new irq risc number
 */
typedef enum
{
	RISC0 = 0,
	RISC1 = 1,
	RISC2 = 2,
	RISC3 = 3,
} gpio_irq_new_risc;

/**
 *  @brief  Define pull up or down types
 */
typedef enum
{
	DRV_STRENGTH_2MA = 0,
	DRV_STRENGTH_4MA = 1,
	DRV_STRENGTH_8MA = 2,
	DRV_STRENGTH_12MA = 3,
} GPIO_Drv_Strength;

/**
 *  @brief  Define pull up or down types
 */
typedef enum
{
	PM_PIN_UP_DOWN_FLOAT = 0,
	PM_PIN_PULLUP_1M = 1,
	PM_PIN_PULLDOWN_100K = 2,
	PM_PIN_PULLUP_10K = 3,
} GPIO_PullTypeDef;

typedef enum
{
	GPIO_GROUP_A = 0,
	GPIO_GROUP_B = 1,
	GPIO_GROUP_C = 2,
	GPIO_GROUP_D = 3,
	GPIO_GROUP_F = 4,
} gpio_group_e;

/*
 *  @brief define gpio irq status types
 */
typedef enum
{
	GPIO_IRQ_GPIO2RISC0_STATUS = BIT(21),
	GPIO_IRQ_GPIO2RISC1_STATUS = BIT(22),
	GPIO_IRQ_GPIO2RISC2_STATUS = BIT(23),
	GPIO_IRQ_GPIO2RISC3_STATUS = BIT(18) | BIT(15),
} gpio_irq_status_e;

typedef enum
{
	GPIO_NEW_RISC0_IRQ = BIT(1),
	GPIO_NEW_RISC1_IRQ = BIT(2),
	GPIO_NEW_RISC2_IRQ = BIT(3),
	GPIO_NEW_RISC3_IRQ = BIT(4),
} gpio_risc_irq_e;

typedef enum
{
    PROB_CLK_7816,
    PROB_CLK_32K,
    PROB_CLK_SYS,
    PROB_CLK_24M_RC,
    PROB_CLK_24M_XTAL,
    PROB_CLK_HS,
    PROB_CLK_STIMER,
    PROB_CLK_78161=8,
}prob_clock_src_e;


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
	unsigned char bit = pin & 0xff;
	if (value){
		BM_SET(reg_gpio_oe(pin), bit);
	}else{
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

static inline __attribute__((always_inline)) void gpio_write(GPIO_PinTypeDef pin, unsigned int value)
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
	reg_irq_src = status;
	reg_gpio_irq_status = (GPIO_IRQ_GPIO2RISC3_STATUS == status)?(BIT(RISC3)):(status>>21);
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
 * @brief      This function serves to clr new_risc irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline void gpio_clr_risc_irq_status(gpio_risc_irq_e status)
{
	reg_gpio_irq_status = status;
}

/**
 * @brief      This function serves to enable new_risc irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_risc_irq_mask(gpio_risc_irq_e mask)
{
	BM_SET(reg_gpio_irq_pad_mask, mask);
}

/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  value - the level of driving strength(1: strong 0: poor)
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin, GPIO_Drv_Strength value);

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
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength.
 * 			   Each call to this function can set only one pin.
 * @param[in]  value - the level of driving strength.
 * @note        | DS0 | DS1 | Drv Strength |
				| --- | --- | ------------ |
				| 0   | 0   | 2mA          |
				| 0   | 1   | 4mA          |
				| 1   | 0   | 8mA          |
				| 1   | 1   | 12mA         |
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin, GPIO_Drv_Strength value);

/**
 * @brief     This function set a pin's gpio gpio2risc0 interrupt,if need disable gpio interrupt,choose disable gpio mask,use interface gpio_clr_irq_mask.
 * @param[in] pin - the pin needs to enable its IRQ
 * @param[in] falling - value of the edge polarity(1: falling edge 0: rising edge)
 * @return    none
 */
static inline void gpio_set_interrupt_risc0(GPIO_PinTypeDef pin, GPIO_PolTypeDef falling)
{
	unsigned char bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc0_en(pin), bit);
	if (falling == POL_FALLING)
	{
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC0));
	}
	else if (falling == POL_RISING)
	{
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC0));
	}

	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	gpio_clr_risc_irq_status(BIT(RISC0)); // must clear, or it will cause to unexpected interrupt.
	gpio_set_risc_irq_mask(BIT(RISC0));

	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src |= FLD_IRQ_GPIO_RISC0_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_RISC0_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc0(GPIO_PinTypeDef pin, int en) // reg_irq_mask: FLD_IRQ_GPIO_RISC0_EN
{
	unsigned char bit = pin & 0xff;
	if (en)
	{
		BM_SET(reg_gpio_irq_risc0_en(pin), bit);
	}
	else
	{
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
	unsigned char bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc1_en(pin), bit);

	if (falling == POL_FALLING)
	{
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC1));
	}
	else if (falling == POL_RISING)
	{
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC1));
	}

	gpio_clr_risc_irq_status(BIT(RISC1)); // must clear, or it will cause to unexpected interrupt.
	gpio_set_risc_irq_mask(BIT(RISC1));
	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src |= FLD_IRQ_GPIO_RISC1_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_RISC1_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc1(GPIO_PinTypeDef pin, int en) // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
{
	unsigned char bit = pin & 0xff;
	if (en)
	{
		BM_SET(reg_gpio_irq_risc1_en(pin), bit);
	}
	else
	{
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
	unsigned char bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc2_en(pin), bit);

	if (falling == POL_FALLING)
	{
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC2));
	}
	else if (falling == POL_RISING)
	{
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC2));
	}

	gpio_clr_risc_irq_status(BIT(RISC2)); // must clear, or it will cause to unexpected interrupt.
	gpio_set_risc_irq_mask(BIT(RISC2));
	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src |= FLD_IRQ_GPIO_RISC2_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_RISC2_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc2(GPIO_PinTypeDef pin, int en) // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
{
	unsigned char bit = pin & 0xff;
	if (en)
	{
		BM_SET(reg_gpio_irq_risc2_en(pin), bit);
	}
	else
	{
		BM_CLR(reg_gpio_irq_risc2_en(pin), bit);
	}
}

/**
 * @brief     This function set a pin's gpio gpio2risc3 interrupt,if need disable gpio interrupt,choose disable gpio mask,use interface gpio_clr_irq_mask.
 * @param[in] pin - the pin needs to enable its IRQ
 * @param[in] falling - value of the edge polarity(1: falling edge 0: rising edge)
 * @return    none
 */
static inline void gpio_set_interrupt_risc3(GPIO_PinTypeDef pin, GPIO_PolTypeDef falling)
{
	unsigned char bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc3_en(pin), bit);

	if (falling == POL_FALLING)
	{
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC3));
	}
	else if (falling == POL_RISING)
	{
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl, BIT(RISC3));
	}

	gpio_clr_risc_irq_status(BIT(RISC3)); // must clear, or it will cause to unexpected interrupt.
	gpio_set_risc_irq_mask(BIT(RISC3));
	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src |= FLD_IRQ_GPIO_RISC3_EN;
	reg_irq_mask |= FLD_IRQ_GPIO_RISC3_EN;
}

/**
 * @brief     This function enables a pin's IRQ function.
 * @param[in] pin - the pin needs to enables its IRQ function.
 * @param[in] en - 1 enable. 0 disable.
 * @return    none
 */
static inline void gpio_en_interrupt_risc3(GPIO_PinTypeDef pin, int en) // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
{
	unsigned char bit = pin & 0xff;
	if (en)
	{
		BM_SET(reg_gpio_irq_risc3_en(pin), bit);
	}
	else
	{
		BM_CLR(reg_gpio_irq_risc3_en(pin), bit);
	}
}

/**
 * @brief       This function is used to enable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_en(void)
{
	write_reg8(0x529, 0x0f);
}

/**
 * @brief       This function is used to disable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_dis(void)
{
	 write_reg8(0x529, 0x00); //MSPI ie disable
}

/**
 * @brief     This function performs to probe clock to IO.
 * @param[in] pin - the pin to probe clock, only support PB[0]/PD0/PE[1:0].
 * @param[in] sel_clk - the clock source which you want to probe.
 * @return    none.
 */
void gpio_set_probe_clk_function(GPIO_PinTypeDef pin, prob_clock_src_e sel_clk);

/********************************************************************************************************
 * @file    timer.c
 *
 * @brief   This is the source file for B85
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
#include "timer.h"

/**
 * @brief     This function performs to set sleep us.
 * @param[in] us - microseconds need to delay.
 * @return    none
 */

_attribute_ram_code_sec_noinline_ void sleep_us(unsigned long us)
{
	unsigned long t = clock_time();
	while(!clock_time_exceed(t, us)){
	}
}

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer0.
 * @param[in] pin - select pin for timer0.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer0_gpio_init(GPIO_PinTypeDef pin, GPIO_PolTypeDef pol)
{
	/*
		initiate gpio trigger and gpio width mode of timer0,it is not necessary to turn on the gpio risc0 mask,
		otherwise, gpio interrupt will be generated, resulting in continuous in-out interrupt,changed by shuaixing,confirmed by jianzhi 20210902.
	 */
	gpio_set_output_en(pin, 0); //disable output
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc0_en(pin), bit);
	gpio_set_input_en(pin ,1);//enable input
	if(pol==POL_FALLING)
	{
		gpio_setup_up_down_resistor(pin,PM_PIN_PULLUP_10K);
		BM_SET(reg_gpio_pol(pin), bit);
	}
	else if(pol==POL_RISING)
	{
		gpio_setup_up_down_resistor(pin,PM_PIN_PULLDOWN_100K);
		BM_CLR(reg_gpio_pol(pin), bit);
	}
	gpio_set_func(pin ,AS_GPIO);
	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;
}
/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer1.
 * @param[in] pin - select pin for timer1.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer1_gpio_init(GPIO_PinTypeDef pin,GPIO_PolTypeDef pol)
{
	/*
		initiate gpio trigger and gpio width mode of timer1,it is not necessary to turn on the gpio risc1 mask,
		otherwise, gpio interrupt will be generated, resulting in continuous in-out interrupt,changed by shuaixing,confirmed by jianzhi 20210902.
	 */
	gpio_set_output_en(pin, 0); //disable output
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc1_en(pin), bit);
	gpio_set_input_en(pin ,1);//enable input
	if(pol==POL_FALLING)
	{
		gpio_setup_up_down_resistor(pin,PM_PIN_PULLUP_10K);
		BM_SET(reg_gpio_pol(pin), bit);
	}
	else if(pol==POL_RISING)
	{
		gpio_setup_up_down_resistor(pin,PM_PIN_PULLDOWN_100K);
		BM_CLR(reg_gpio_pol(pin), bit);
	}
	gpio_set_func(pin ,AS_GPIO);
	/*clear gpio interrupt source (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
    reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;

}
/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer2.
 * @param[in] pin - select pin for timer2.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer2_gpio_init(GPIO_PinTypeDef pin,GPIO_PolTypeDef pol)
{
	gpio_set_output_en(pin, 0); //disable output
	unsigned char bit = pin & 0xff;
	BM_SET(reg_gpio_irq_risc2_en(pin), bit);
	gpio_set_input_en(pin ,1);//enable input
	if(pol==POL_FALLING)
	{
		gpio_setup_up_down_resistor(pin,PM_PIN_PULLUP_10K);
		BM_SET(reg_gpio_pol(pin), bit);
	}
	else if(pol==POL_RISING)
	{
		gpio_setup_up_down_resistor(pin,PM_PIN_PULLDOWN_100K);
		BM_CLR(reg_gpio_pol(pin), bit);
	}
	gpio_set_func(pin ,AS_GPIO);
}
/**
 * @brief     set mode, initial tick and capture of timer0.
 * @param[in] mode - select mode for timer0.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
void timer0_set_mode(TIMER_ModeTypeDef mode,unsigned int init_tick, unsigned int cap_tick)
{
	switch(mode)
	{
		case TIMER_MODE_SYSCLK:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick;
			reg_tmr0_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_SYSCLK<<1);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick;
			reg_tmr0_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_GPIO_TRIGGER<<1);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR0;
			reg_tmr_ctrl16 &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_GPIO_WIDTH<<1);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick; 		//clear counter
			reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_TICK<<1);
			break;
		}
		default: break;
	}
}
/**
 * @brief     set mode, initial tick and capture of timer1.
 * @param[in] mode - select mode for timer1.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
void timer1_set_mode(TIMER_ModeTypeDef mode,unsigned int init_tick, unsigned int cap_tick)
{
	switch(mode)
	{
		case TIMER_MODE_SYSCLK:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick;
			reg_tmr1_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_SYSCLK<<4);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick;
			reg_tmr1_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_GPIO_TRIGGER<<4);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR1;
			reg_tmr_ctrl16 &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_GPIO_WIDTH<<4);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick; //clear counter
			reg_tmr_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_TICK<<4);
			break;
		}
		default: break;
	}
}
/**
 * @brief     set mode, initial tick and capture of timer2.
 * @param[in] mode - select mode for timer2.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
void timer2_set_mode(TIMER_ModeTypeDef mode,unsigned int init_tick, unsigned int cap_tick)
{
	switch(mode)
	{
		case TIMER_MODE_SYSCLK:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick;
			reg_tmr2_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR2; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_SYSCLK<<7);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick;
			reg_tmr2_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR2; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_GPIO_TRIGGER<<7);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR2;
			reg_tmr_ctrl16 &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_GPIO_WIDTH<<7);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick; //clear counter
			reg_tmr_sta = FLD_TMR_STA_TMR2; //clear irq status
			reg_tmr_ctrl16 &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl16 |= (TIMER_MODE_TICK<<7);
			break;
		}
		default: break;
	}
}
/**
 * @brief     the specified timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(TIMER_TypeDef type)
{
	switch(type)
	{
		case TIMER0:
			reg_tmr_ctrl16 |= FLD_TMR0_EN;
			break;
		case TIMER1:
			reg_tmr_ctrl16 |= FLD_TMR1_EN;
			break;
		case TIMER2:
			reg_tmr_ctrl16 |= FLD_TMR2_EN;
			break;
		default:
			break;
	}
}
/**
 * @brief     the specified timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
void timer_stop(TIMER_TypeDef type)
{
	switch(type)
	{
		case TIMER0:
			reg_tmr_ctrl16 &= (~FLD_TMR0_EN);
			break;
		case TIMER1:
			reg_tmr_ctrl16 &= (~FLD_TMR1_EN);
			break;
		case TIMER2:
			reg_tmr_ctrl16 &= (~FLD_TMR2_EN);
			break;
		default:
			break;
	}
}



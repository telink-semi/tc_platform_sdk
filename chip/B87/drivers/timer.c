/********************************************************************************************************
 * @file	timer.c
 *
 * @brief	This is the source file for b87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	/*clear gpio interrupt sorce (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src |= FLD_IRQ_GPIO_RISC0_EN;
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
	/*clear gpio interrupt sorce (after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src |= FLD_IRQ_GPIO_RISC1_EN;

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
			reg_tmr_ctrl &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_SYSCLK<<1);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick;
			reg_tmr0_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr_ctrl &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_GPIO_TRIGGER<<1);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR0;
			reg_tmr_ctrl &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_GPIO_WIDTH<<1);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick = init_tick; 		//clear counter
			reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr_ctrl &= (~FLD_TMR0_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_TICK<<1);
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
			reg_tmr_ctrl &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_SYSCLK<<4);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick;
			reg_tmr1_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr_ctrl &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_GPIO_TRIGGER<<4);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR1;
			reg_tmr_ctrl &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_GPIO_WIDTH<<4);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick = init_tick; //clear counter
			reg_tmr_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr_ctrl &= (~FLD_TMR1_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_TICK<<4);
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
			reg_tmr_ctrl &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_SYSCLK<<7);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick;
			reg_tmr2_capt = cap_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR2; //clear irq status
			reg_tmr_ctrl &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_GPIO_TRIGGER<<7);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick;
			reg_tmr_sta = FLD_TMR_STA_TMR2;
			reg_tmr_ctrl &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_GPIO_WIDTH<<7);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR2_EN;
			reg_tmr2_tick = init_tick; //clear counter
			reg_tmr_sta = FLD_TMR_STA_TMR2; //clear irq status
			reg_tmr_ctrl &= (~FLD_TMR2_MODE);
			reg_tmr_ctrl |= (TIMER_MODE_TICK<<7);
			break;
		}
		default: break;
	}
}
/**
 * @brief     the specifed timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(TIMER_TypeDef type)
{
	switch(type)
	{
		case TIMER0:
			reg_tmr_ctrl |= FLD_TMR0_EN;
			break;
		case TIMER1:
			reg_tmr_ctrl |= FLD_TMR1_EN;
			break;
		case TIMER2:
			reg_tmr_ctrl |= FLD_TMR2_EN;
			break;
		default:
			break;
	}
}
/**
 * @brief     the specifed timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
void timer_stop(TIMER_TypeDef type)
{
	switch(type)
	{
		case TIMER0:
			reg_tmr_ctrl &= (~FLD_TMR0_EN);
			break;
		case TIMER1:
			reg_tmr_ctrl &= (~FLD_TMR1_EN);
			break;
		case TIMER2:
			reg_tmr_ctrl &= (~FLD_TMR2_EN);
			break;
		default:
			break;
	}
}



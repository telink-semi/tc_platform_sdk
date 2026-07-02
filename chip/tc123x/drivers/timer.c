/********************************************************************************************************
 * @file    timer.c
 *
 * @brief   This is the source file for TC123X
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
#include "timer.h"


/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer0.
 * @param[in] pin - select pin for timer0.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer0_gpio_init(GPIO_PinTypeDef pin, GPIO_PolTypeDef pol)
{
	/*
		initiate gpio trigger and gpio width mode of timer0,it is not necessary to turn on the gpio irq0 mask,
		otherwise, gpio interrupt will be generated, resulting in continuous in-out interrupt,changed by shuaixing,confirmed by jianzhi 20210902.
	 */
	gpio_set_output_en(pin, 0); //disable output
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq0_en(pin), bit);
	BM_SET(reg_gpio_risc_en, FLD_RISC0_EN);
	gpio_set_input_en(pin ,1);//enable input
	if(pol==POL_FALLING)
	{
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(0));
	}
	else if(pol==POL_RISING)
	{
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(0));
	}
	gpio_set_func(pin ,AS_GPIO);
	/*clear gpio interrupt source(after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src = FLD_IRQ_GPIO_RISC0_EDGE_EN;
}

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer1.
 * @param[in] pin - select pin for timer1.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer1_gpio_init(GPIO_PinTypeDef pin, GPIO_PolTypeDef pol)
{
	/*
		initiate gpio trigger and gpio width mode of timer1,it is not necessary to turn on the gpio irq1 mask,
		otherwise, gpio interrupt will be generated, resulting in continuous in-out interrupt,changed by shuaixing,confirmed by jianzhi 20210902.
	 */
	gpio_set_output_en(pin, 0); //disable output
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_irq1_en(pin), bit);
	BM_SET(reg_gpio_risc_en, FLD_RISC1_EN);
	gpio_set_input_en(pin ,1);//enable input
	if(pol==POL_FALLING)
	{
		BM_SET(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(0));
	}
	else if(pol==POL_RISING)
	{
		BM_CLR(reg_gpio_pol(pin), bit);
		BM_CLR(reg_gpio_irq_lvl,BIT(0));
	}
	gpio_set_func(pin ,AS_GPIO);
	/*clear gpio interrupt source(after setting gpio polarity,before enable interrupt)to avoid unexpected interrupt. confirm by minghai*/
	reg_irq_src = FLD_IRQ_GPIO_RISC1_EDGE_EN;
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

			reg_tmr0_tick0 = init_tick;
			reg_tmr0_tick1 = init_tick>>8;
			reg_tmr0_tick2 = init_tick>>16;

			reg_tmr0_capt0 = cap_tick;
			reg_tmr0_capt1 = cap_tick>>8;
			reg_tmr0_capt2 = cap_tick>>16;

			reg_tmr0_sta = FLD_TMR_STA_TMR0;//clear irq status
			reg_tmr0_ctrl8 = (reg_tmr0_ctrl8 & (~FLD_TMR0_MODE)) | (TIMER_MODE_SYSCLK<<1);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;
			reg_tmr0_tick0 = init_tick;
			reg_tmr0_tick1 = init_tick>>8;
			reg_tmr0_tick2 = init_tick>>16;

			reg_tmr0_capt0 = cap_tick;
			reg_tmr0_capt1 = cap_tick>>8;
			reg_tmr0_capt2 = cap_tick>>16;

			reg_tmr0_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr0_ctrl8 = (reg_tmr0_ctrl8 & (~FLD_TMR0_MODE)) | (TIMER_MODE_GPIO_TRIGGER<<1);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;

			reg_tmr0_tick0 = init_tick;
			reg_tmr0_tick1 = init_tick>>8;
			reg_tmr0_tick2 = init_tick>>16;

			reg_tmr0_sta = FLD_TMR_STA_TMR0;//clear irq status
			reg_tmr0_ctrl8 = (reg_tmr0_ctrl8 & (~FLD_TMR0_MODE)) | (TIMER_MODE_GPIO_WIDTH<<1);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR0_EN;

			reg_tmr0_tick0 = init_tick;
			reg_tmr0_tick1 = init_tick>>8;
			reg_tmr0_tick2 = init_tick>>16;

			reg_tmr0_sta = FLD_TMR_STA_TMR0; //clear irq status
			reg_tmr0_ctrl8 = (reg_tmr0_ctrl8 & (~FLD_TMR0_MODE)) | (TIMER_MODE_TICK<<1);
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

			reg_tmr1_tick0 = init_tick;
			reg_tmr1_tick1 = init_tick>>8;
			reg_tmr1_tick2 = init_tick>>16;

			reg_tmr1_capt0 = cap_tick;
			reg_tmr1_capt1 = cap_tick>>8;
			reg_tmr1_capt2 = cap_tick>>16;

			reg_tmr1_sta = FLD_TMR_STA_TMR1;//clear irq status
			reg_tmr1_ctrl8 = (reg_tmr1_ctrl8 & (~FLD_TMR1_MODE)) | (TIMER_MODE_SYSCLK<<1);
			break;
		}
		case TIMER_MODE_GPIO_TRIGGER:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;
			reg_tmr1_tick0 = init_tick;
			reg_tmr1_tick1 = init_tick>>8;
			reg_tmr1_tick2 = init_tick>>16;

			reg_tmr1_capt0 = cap_tick;
			reg_tmr1_capt1 = cap_tick>>8;
			reg_tmr1_capt2 = cap_tick>>16;

			reg_tmr1_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr1_ctrl8 = (reg_tmr1_ctrl8 & (~FLD_TMR1_MODE)) | (TIMER_MODE_GPIO_TRIGGER<<1);
			break;
		}
		case TIMER_MODE_GPIO_WIDTH:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;

			reg_tmr1_tick0 = init_tick;
			reg_tmr1_tick1 = init_tick>>8;
			reg_tmr1_tick2 = init_tick>>16;

			reg_tmr1_sta = FLD_TMR_STA_TMR1;//clear irq status
			reg_tmr1_ctrl8 = (reg_tmr1_ctrl8 & (~FLD_TMR1_MODE)) | (TIMER_MODE_GPIO_WIDTH<<1);
			break;
		}
		case TIMER_MODE_TICK:
		{
			reg_irq_mask |= FLD_IRQ_TMR1_EN;

			reg_tmr1_tick0 = init_tick;
			reg_tmr1_tick1 = init_tick>>8;
			reg_tmr1_tick2 = init_tick>>16;

			reg_tmr1_sta = FLD_TMR_STA_TMR1; //clear irq status
			reg_tmr1_ctrl8 = (reg_tmr1_ctrl8 & (~FLD_TMR1_MODE)) | (TIMER_MODE_TICK<<1);
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
			reg_tmr0_ctrl8 |= FLD_TMR0_EN;
			break;
		case TIMER1:
			reg_tmr1_ctrl8 |= FLD_TMR1_EN;
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
			reg_tmr0_ctrl8 &= (~FLD_TMR0_EN);
			break;
		case TIMER1:
			reg_tmr1_ctrl8 &= (~FLD_TMR1_EN);
			break;
		default:
			break;
	}
}



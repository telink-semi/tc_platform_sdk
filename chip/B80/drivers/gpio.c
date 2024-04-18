/********************************************************************************************************
 * @file    gpio.c
 *
 * @brief   This is the source file for B80
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
#include "bsp.h"
#include "compiler.h"
#include "register.h"
#include "analog.h"
#include "gpio.h"

/**
 * @brief      This function servers to initiate pull up-down resistor of all gpio,Except GPIOE group.
 * @param[in]  none
 * @return     none.
 */
static void gpio_analog_resistance_init(void)
{
	analog_write (0x0e,  PULL_WAKEUP_SRC_PA0 |
						(PULL_WAKEUP_SRC_PA1<<2) |
						(PULL_WAKEUP_SRC_PA2<<4) |
						(PULL_WAKEUP_SRC_PA3<<6));

	analog_write (0x0f,  PULL_WAKEUP_SRC_PA4 |
						(PULL_WAKEUP_SRC_PA5<<2) |
						(PULL_WAKEUP_SRC_PA6<<4) |
						(PULL_WAKEUP_SRC_PA7<<6));

	analog_write (0x10,  PULL_WAKEUP_SRC_PB0 |
						(PULL_WAKEUP_SRC_PB1<<2) |
						(PULL_WAKEUP_SRC_PB2<<4) |
						(PULL_WAKEUP_SRC_PB3<<6));

	analog_write (0x11,  PULL_WAKEUP_SRC_PB4 |
						(PULL_WAKEUP_SRC_PB5<<2) |
						(PULL_WAKEUP_SRC_PB6<<4) |
						(PULL_WAKEUP_SRC_PB7<<6));

	analog_write (0x12,  PULL_WAKEUP_SRC_PC0 |
						(PULL_WAKEUP_SRC_PC1<<2) |
						(PULL_WAKEUP_SRC_PC2<<4) |
						(PULL_WAKEUP_SRC_PC3<<6));

	analog_write (0x13,  PULL_WAKEUP_SRC_PC4 |
						(PULL_WAKEUP_SRC_PC5<<2) |
						(PULL_WAKEUP_SRC_PC6<<4) |
						(PULL_WAKEUP_SRC_PC7<<6));

	analog_write (0x14,  PULL_WAKEUP_SRC_PD0 |
						(PULL_WAKEUP_SRC_PD1<<2) |
						(PULL_WAKEUP_SRC_PD2<<4) |
						(PULL_WAKEUP_SRC_PD3<<6));

	analog_write (0x15,  PULL_WAKEUP_SRC_PD4 |
						(PULL_WAKEUP_SRC_PD5<<2) |
						(PULL_WAKEUP_SRC_PD6<<4) |
						(PULL_WAKEUP_SRC_PD7<<6));

	analog_write(0x16,PULL_WAKEUP_SRC_PF0 |(PULL_WAKEUP_SRC_PF1<<2));
}
/**
 * @brief      This function servers to initialization all gpio.
 * @param[in]  en  -  if mcu wake up from deep retention mode, determine whether it is NOT necessary to reset analog register
 *                    1: set analog register
 *                    0: not set analog register
 * @return     none.
 */
/**Processing methods of unused GPIO
 * Set it to high resistance state and set it to open pull-up or pull-down resistance to
 * let it be in the determined state.When GPIO uses internal pull-up or pull-down resistance,
 * do not use pull-up or pull-down resistance on the board in the process of practical
 * application because it may have the risk of electric leakage .
 */
#if (BLC_PM_DEEP_RETENTION_MODE_EN)
_attribute_ram_code_sec_noinline_
#endif
void gpio_init(int anaRes_init_en)
{
	reg_gpio_pa_setting1 =
		(PA0_INPUT_ENABLE<<8) 	| (PA1_INPUT_ENABLE<<9)	| (PA2_INPUT_ENABLE<<10)	| (PA3_INPUT_ENABLE<<11) |
		(PA4_INPUT_ENABLE<<12)	| (PA5_INPUT_ENABLE<<13)	| (PA6_INPUT_ENABLE<<14)	| (PA7_INPUT_ENABLE<<15) |
		((PA0_OUTPUT_ENABLE?0:1)<<16)	| ((PA1_OUTPUT_ENABLE?0:1)<<17) | ((PA2_OUTPUT_ENABLE?0:1)<<18)	| ((PA3_OUTPUT_ENABLE?0:1)<<19) |
		((PA4_OUTPUT_ENABLE?0:1)<<20)	| ((PA5_OUTPUT_ENABLE?0:1)<<21) | ((PA6_OUTPUT_ENABLE?0:1)<<22)	| ((PA7_OUTPUT_ENABLE?0:1)<<23) |
		(PA0_DATA_OUT<<24)	| (PA1_DATA_OUT<<25)	| (PA2_DATA_OUT<<26)	| (PA3_DATA_OUT<<27) |
		(PA4_DATA_OUT<<28)	| (PA5_DATA_OUT<<29)	| (PA6_DATA_OUT<<30)	| (PA7_DATA_OUT<<31) ;
	reg_gpio_pa_setting2 =
		(PA0_DATA_STRENGTH<<8)		| (PA1_DATA_STRENGTH<<9)| (PA2_DATA_STRENGTH<<10)	| (PA3_DATA_STRENGTH<<11) |
		(PA4_DATA_STRENGTH<<12)	| (PA5_DATA_STRENGTH<<13)	| (PA6_DATA_STRENGTH<<14)	| (PA7_DATA_STRENGTH<<15) |
		(PA0_FUNC==AS_GPIO ? BIT(16):0)	| (PA1_FUNC==AS_GPIO ? BIT(17):0)| (PA2_FUNC==AS_GPIO ? BIT(18):0)| (PA3_FUNC==AS_GPIO ? BIT(19):0) |
		(PA4_FUNC==AS_GPIO ? BIT(20):0)	| (PA5_FUNC==AS_GPIO ? BIT(21):0)| (PA6_FUNC==AS_GPIO ? BIT(22):0)| (PA7_FUNC==AS_GPIO ? BIT(23):0);

	//PB group
	reg_gpio_pb_setting1 =
		(PB0_INPUT_ENABLE<<8) 	| (PB1_INPUT_ENABLE<<9)	| (PB2_INPUT_ENABLE<<10)	| (PB3_INPUT_ENABLE<<11) |
		(PB4_INPUT_ENABLE<<12)	| (PB5_INPUT_ENABLE<<13)	| (PB6_INPUT_ENABLE<<14)	| (PB7_INPUT_ENABLE<<15) |
		((PB0_OUTPUT_ENABLE?0:1)<<16)	| ((PB1_OUTPUT_ENABLE?0:1)<<17) | ((PB2_OUTPUT_ENABLE?0:1)<<18)	| ((PB3_OUTPUT_ENABLE?0:1)<<19) |
		((PB4_OUTPUT_ENABLE?0:1)<<20)	| ((PB5_OUTPUT_ENABLE?0:1)<<21) | ((PB6_OUTPUT_ENABLE?0:1)<<22)	| ((PB7_OUTPUT_ENABLE?0:1)<<23) |
		(PB0_DATA_OUT<<24)	| (PB1_DATA_OUT<<25)	| (PB2_DATA_OUT<<26)	| (PB3_DATA_OUT<<27) |
		(PB4_DATA_OUT<<28)	| (PB5_DATA_OUT<<29)	| (PB6_DATA_OUT<<30)	| (PB7_DATA_OUT<<31) ;
	reg_gpio_pb_setting2 =
		(PB0_DATA_STRENGTH<<8)		| (PB1_DATA_STRENGTH<<9)| (PB2_DATA_STRENGTH<<10)	| (PB3_DATA_STRENGTH<<11) |
		(PB4_DATA_STRENGTH<<12)	| (PB5_DATA_STRENGTH<<13)	| (PB6_DATA_STRENGTH<<14)	| (PB7_DATA_STRENGTH<<15) |
		(PB0_FUNC==AS_GPIO ? BIT(16):0)	| (PB1_FUNC==AS_GPIO ? BIT(17):0)| (PB2_FUNC==AS_GPIO ? BIT(18):0)| (PB3_FUNC==AS_GPIO ? BIT(19):0) |
		(PB4_FUNC==AS_GPIO ? BIT(20):0)	| (PB5_FUNC==AS_GPIO ? BIT(21):0)| (PB6_FUNC==AS_GPIO ? BIT(22):0)| (PB7_FUNC==AS_GPIO ? BIT(23):0);

	//PC group
		//ie
		analog_write(areg_gpio_pc_ie, 	(PC0_INPUT_ENABLE<<0) 	| (PC1_INPUT_ENABLE<<1)	| (PC2_INPUT_ENABLE<<2)	| (PC3_INPUT_ENABLE<<3) |
										(PC4_INPUT_ENABLE<<4)	| (PC5_INPUT_ENABLE<<5) | (PC6_INPUT_ENABLE<<6)	| (PC7_INPUT_ENABLE<<7) );

		//oen
		reg_gpio_pc_oen =
			((PC0_OUTPUT_ENABLE?0:1)<<0)	| ((PC1_OUTPUT_ENABLE?0:1)<<1) | ((PC2_OUTPUT_ENABLE?0:1)<<2)	| ((PC3_OUTPUT_ENABLE?0:1)<<3) |
			((PC4_OUTPUT_ENABLE?0:1)<<4)	| ((PC5_OUTPUT_ENABLE?0:1)<<5) | ((PC6_OUTPUT_ENABLE?0:1)<<6)	| ((PC7_OUTPUT_ENABLE?0:1)<<7);
		//dataO
		reg_gpio_pc_out =
			(PC0_DATA_OUT<<0)	| (PC1_DATA_OUT<<1)	| (PC2_DATA_OUT<<2)	| (PC3_DATA_OUT<<3) |
			(PC4_DATA_OUT<<4)	| (PC5_DATA_OUT<<5)	| (PC6_DATA_OUT<<6)	| (PC7_DATA_OUT<<7) ;

		//ds
		analog_write(areg_gpio_pc_ds, 	(PC0_DATA_STRENGTH<<0) 	| (PC1_DATA_STRENGTH<<1)  | (PC2_DATA_STRENGTH<<2)	| (PC3_DATA_STRENGTH<<3) |
										(PC4_DATA_STRENGTH<<4)	| (PC5_DATA_STRENGTH<<5)  | (PC6_DATA_STRENGTH<<6)	| (PC7_DATA_STRENGTH<<7) );

		reg_gpio_pc_gpio =
			(PC0_FUNC==AS_GPIO ? BIT(0):0)	| (PC1_FUNC==AS_GPIO ? BIT(1):0)| (PC2_FUNC==AS_GPIO ? BIT(2):0)| (PC3_FUNC==AS_GPIO ? BIT(3):0) |
			(PC4_FUNC==AS_GPIO ? BIT(4):0)	| (PC5_FUNC==AS_GPIO ? BIT(5):0)| (PC6_FUNC==AS_GPIO ? BIT(6):0)| (PC7_FUNC==AS_GPIO ? BIT(7):0);

	//PD group
	reg_gpio_pd_setting1 =
		(PD0_INPUT_ENABLE<<8) 	| (PD1_INPUT_ENABLE<<9)	| (PD2_INPUT_ENABLE<<10)	| (PD3_INPUT_ENABLE<<11) |
		(PD4_INPUT_ENABLE<<12)	| (PD5_INPUT_ENABLE<<13)| (PD6_INPUT_ENABLE<<14)	| (PD7_INPUT_ENABLE<<15) |
		((PD0_OUTPUT_ENABLE?0:1)<<16)	| ((PD1_OUTPUT_ENABLE?0:1)<<17) | ((PD2_OUTPUT_ENABLE?0:1)<<18)	| ((PD3_OUTPUT_ENABLE?0:1)<<19) |
		((PD4_OUTPUT_ENABLE?0:1)<<20)	| ((PD5_OUTPUT_ENABLE?0:1)<<21) | ((PD6_OUTPUT_ENABLE?0:1)<<22)	| ((PD7_OUTPUT_ENABLE?0:1)<<23) |
		(PD0_DATA_OUT<<24)	| (PD1_DATA_OUT<<25)	| (PD2_DATA_OUT<<26)	| (PD3_DATA_OUT<<27) |
		(PD4_DATA_OUT<<28)	| (PD5_DATA_OUT<<29)	| (PD6_DATA_OUT<<30)	| (PD7_DATA_OUT<<31) ;
	reg_gpio_pd_setting2 =
		(PD0_DATA_STRENGTH<<8)	| (PD1_DATA_STRENGTH<<9)	| (PD2_DATA_STRENGTH<<10)	| (PD3_DATA_STRENGTH<<11) |
		(PD4_DATA_STRENGTH<<12)	| (PD5_DATA_STRENGTH<<13)	| (PD6_DATA_STRENGTH<<14)	| (PD7_DATA_STRENGTH<<15) |
		(PD0_FUNC==AS_GPIO ? BIT(16):0)	| (PD1_FUNC==AS_GPIO ? BIT(17):0)| (PD2_FUNC==AS_GPIO ? BIT(18):0)| (PD3_FUNC==AS_GPIO ? BIT(19):0) |
		(PD4_FUNC==AS_GPIO ? BIT(20):0)	| (PD5_FUNC==AS_GPIO ? BIT(21):0)| (PD6_FUNC==AS_GPIO ? BIT(22):0)| (PD7_FUNC==AS_GPIO ? BIT(23):0);


	//PE group
	reg_gpio_pe_ie = (PE0_INPUT_ENABLE<<0)	| (PE1_INPUT_ENABLE<<1)| (PE2_INPUT_ENABLE<<2)	| (PE3_INPUT_ENABLE<<3);
	reg_gpio_pe_oen = ((PE0_OUTPUT_ENABLE?0:1)<<0)	| ((PE1_OUTPUT_ENABLE?0:1)<<1) | ((PE2_OUTPUT_ENABLE?0:1)<<2)	| ((PE3_OUTPUT_ENABLE?0:1)<<3);
	reg_gpio_pe_out = (PE0_DATA_OUT<<0)	| (PE1_DATA_OUT<<1)	| (PE2_DATA_OUT<<2)	| (PE3_DATA_OUT<<3);
	reg_gpio_pe_ds = (PE0_DATA_STRENGTH<<0)	| (PE1_DATA_STRENGTH<<1)	| (PE2_DATA_STRENGTH<<2)	| (PE3_DATA_STRENGTH<<3);
	reg_gpio_pe_gpio = (PE0_FUNC==AS_GPIO ? BIT(0):0)	| (PE1_FUNC==AS_GPIO ? BIT(1):0)| (PE2_FUNC==AS_GPIO ? BIT(2):0)| (PE3_FUNC==AS_GPIO ? BIT(3):0);

	reg_gpio_pf_setting1 =
		(PF0_INPUT_ENABLE<<8)|(PF1_INPUT_ENABLE<<9)|((PF0_OUTPUT_ENABLE?0:1)<<16)|((PF1_OUTPUT_ENABLE?0:1)<<17)|(PF0_DATA_OUT<<24)|(PF1_DATA_OUT<<25);
	reg_gpio_pf_setting2 =
		(PF0_DATA_STRENGTH<<8)|(PF1_DATA_STRENGTH<<9)|(PF0_FUNC==AS_GPIO ? BIT(16):0)|(PF1_FUNC==AS_GPIO ? BIT(17):0);

	if(anaRes_init_en)
	{
		gpio_analog_resistance_init();
	}
}


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
void gpio_set_func(GPIO_PinTypeDef pin, gpio_func_e func)
{
	unsigned char	bit = pin & 0xff;
	if(func == AS_GPIO)
	{
		BM_SET(reg_gpio_func(pin), bit);
	}
	else
	{
		reg_gpio_func_mux(pin)=func;
		BM_CLR(reg_gpio_func(pin), bit);
	}
}
/**
 * @brief      This function set the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function
 * @param[in]  value - enable or disable the pin's input function(0: disable, 1: enable)
 * @return     none
 */
void gpio_set_input_en(GPIO_PinTypeDef pin, unsigned int value)
{
	unsigned char	bit = pin & 0xff;
	unsigned short group = pin & 0xf00;

	if(group == GPIO_GROUPC)
	{
	    if(value)
	    {
	    	analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie)|bit);
	    }
	    else
	    {
	    	analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie)&(~bit));
	    }
	}
	else
	{
		if(value)
		{
			BM_SET(reg_gpio_ie(pin), bit);
		}
		else
		{
			BM_CLR(reg_gpio_ie(pin), bit);
		}
	}
}

/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  value - the level of driving strength(1: strong 0: poor)
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin, unsigned int value)
{
	unsigned char bit = pin & 0xff;
	unsigned short group = pin & 0xf00;

	if(group == GPIO_GROUPC)
	{
	    if(value)
	    {
	    	analog_write(areg_gpio_pc_ds, analog_read(areg_gpio_pc_ds)|bit);
	    }
	    else
	    {
	    	analog_write(areg_gpio_pc_ds, analog_read(areg_gpio_pc_ds)&(~bit));
	    }
	}
	else
    {
    	if(value)
    	{
    		BM_SET(reg_gpio_ds(pin), bit);
    	}
    	else
    	{
    		BM_CLR(reg_gpio_ds(pin), bit);
    	}
    }
}

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor, GPIOE group is not supported
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 * @note	the following two point need to noticed when using PB0, PB1, PB3, PD4 and PF0 GPIO ports:
 *  		1. These pins are not recommend to use as wake-up source;
 *  		2. Since these pins are output functions by default, even if they are configured with pull-up/pull-down retention,
 *  		when deep/deep Retention is invoked, they can't maintain high/low level and an abnormal level will occur.
 *  		Therefore, these pins can't be used in applications where a certain level state needs to be maintained all the time.
 */
void gpio_setup_up_down_resistor(GPIO_PinTypeDef gpio, GPIO_PullTypeDef up_down)
{
	unsigned char r_val = up_down & 0x03;

	unsigned char base_ana_reg = 0x0e + ((gpio >> 8) << 1) + ( (gpio & 0xf0) ? 1 : 0 );  //group = gpio>>8;
	unsigned char shift_num, mask_not;

	if(GPIO_GROUPF == (gpio&0xf00))
	{
		base_ana_reg = 0x16;
	}

	if(gpio & 0x11){
		shift_num = 0;
		mask_not = 0xfc;
	}
	else if(gpio & 0x22){
		shift_num = 2;
		mask_not = 0xf3;
	}
	else if(gpio & 0x44){
		shift_num = 4;
		mask_not = 0xcf;
	}
	else if(gpio & 0x88){
		shift_num = 6;
		mask_not = 0x3f;
	}
	else{
		return;
	}
	analog_write(base_ana_reg, (analog_read(base_ana_reg) & mask_not) | (r_val << shift_num));
}

/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO, GPIOE group is not included in GPIO_ALL
 * @return     none.
 */
void gpio_shutdown(GPIO_PinTypeDef pin)
{
		unsigned short group = pin & 0xf00;
		unsigned char bit = pin & 0xff;
		switch(group)
		{
			case GPIO_GROUPA:
				reg_gpio_pa_out &= (~bit);
				reg_gpio_pa_oen |= bit;
				reg_gpio_pa_gpio |= (bit&0xf7);
				reg_gpio_pa_ie &= ((~bit)|0x08);
				break;
			case GPIO_GROUPB:
				reg_gpio_pb_out &= (~bit);
				reg_gpio_pb_oen |= bit;
				reg_gpio_pb_gpio |= bit;
				reg_gpio_pb_ie &= (~bit);
				break;
			case GPIO_GROUPC:
				reg_gpio_pc_out &= (~bit);
				reg_gpio_pc_oen |= bit;
				reg_gpio_pc_gpio |= bit;
				analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie) & (~bit));
				break;
			case GPIO_GROUPD:
				reg_gpio_pd_out &= (~bit);
				reg_gpio_pd_oen |= bit;
				reg_gpio_pd_gpio |= bit;
				reg_gpio_pd_ie &= (~bit);
				break;
			case GPIO_GROUPE:
				reg_gpio_pe_out &= (~bit);
				reg_gpio_pe_oen |= bit;
				reg_gpio_pe_gpio |= bit;
				reg_gpio_pe_ie &= (~bit);
				break;
			case GPIO_GROUPF:
				reg_gpio_pf_out &= (~bit);
				reg_gpio_pf_oen |= bit;
				reg_gpio_pf_gpio |= bit;
				reg_gpio_pf_ie &= (~bit);
				break;
			case GPIO_ALL:
			{
				//set low level
				reg_gpio_pa_out = 0x00;
				reg_gpio_pb_out = 0x00;
				reg_gpio_pc_out = 0x00;
				reg_gpio_pd_out = 0x00;
				reg_gpio_pf_out = 0x00;

				//output disable
				reg_gpio_pa_oen = 0xff;
				reg_gpio_pb_oen = 0xff;
				reg_gpio_pc_oen = 0xff;
				reg_gpio_pd_oen = 0xff;
				reg_gpio_pf_oen = 0xff;

				//dataO = 0
				reg_gpio_pa_gpio = 0xf7;
				reg_gpio_pb_gpio = 0xff;
				reg_gpio_pc_gpio = 0xff;
				reg_gpio_pd_gpio = 0xff;
				reg_gpio_pf_gpio = 0xff;

				//ie = 0
				reg_gpio_pa_ie = 0x08;			  //SWS   501<3>
				reg_gpio_pb_ie = 0x00;
				analog_write(areg_gpio_pc_ie, 0);
				reg_gpio_pd_ie = 0x00;
				reg_gpio_pf_ie = 0x00;
			}
		}
}


/**
 * @brief     This function set pin's 30k pull-up register.
 * @param[in] pin - the pin needs to set its pull-up register.
 * @return    none.
 * @attention This function sets the digital pull-up, it will not work after entering low power consumption.
 */
#if (MCU_CORE_B80)
void gpio_set_pullup_res_30k(GPIO_PinTypeDef pin){
	unsigned char	bit = pin & 0xff;
	unsigned short group = pin & 0xf00;

	if(group==GPIO_GROUPC)
	{
		analog_write(areg_gpio_pc_pe, analog_read(areg_gpio_pc_pe) | bit);
	}
	else
	{
		BM_SET(reg_gpio_oen(pin),bit);
		BM_SET(reg_gpio_out(pin),bit);
	}
}
#elif (MCU_CORE_B80B)
void gpio_set_pullup_res_30k(GPIO_PinTypeDef pin){
    unsigned char	bit = pin & 0xff;
    unsigned short group = pin & 0xf00;

    if(group==GPIO_GROUPC){
        analog_write(areg_gpio_pc_pe, analog_read(areg_gpio_pc_pe) | bit);
    }
    else
    {
        BM_SET(reg_gpio_pe(pin),bit);
    }
}
#endif


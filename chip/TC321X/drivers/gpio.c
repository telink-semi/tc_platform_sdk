/********************************************************************************************************
 * @file    gpio.c
 *
 * @brief   This is the source file for TC321X
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
	analog_write (0x17,  PULL_WAKEUP_SRC_PA0 |
						(PULL_WAKEUP_SRC_PA1<<2) |
						(PULL_WAKEUP_SRC_PA2<<4) |
						(PULL_WAKEUP_SRC_PA3<<6));

	analog_write (0x18,  PULL_WAKEUP_SRC_PA4 |
						(PULL_WAKEUP_SRC_PA5<<2) |
						(PULL_WAKEUP_SRC_PA6<<4) |
						(PULL_WAKEUP_SRC_PA7<<6));

	analog_write (0x19,  PULL_WAKEUP_SRC_PB0 |
						(PULL_WAKEUP_SRC_PB1<<2) |
						(PULL_WAKEUP_SRC_PB2<<4) |
						(PULL_WAKEUP_SRC_PB3<<6));

	analog_write (0x1a,  PULL_WAKEUP_SRC_PB4 |
						(PULL_WAKEUP_SRC_PB5<<2) |
						(PULL_WAKEUP_SRC_PB6<<4) |
						(PULL_WAKEUP_SRC_PB7<<6));

	analog_write (0x1b,  PULL_WAKEUP_SRC_PC0 |
						(PULL_WAKEUP_SRC_PC1<<2) |
						(PULL_WAKEUP_SRC_PC2<<4) |
						(PULL_WAKEUP_SRC_PC3<<6));

	analog_write (0x1c,  PULL_WAKEUP_SRC_PC4 |
						(PULL_WAKEUP_SRC_PC5<<2) |
						(PULL_WAKEUP_SRC_PC6<<4) |
						(PULL_WAKEUP_SRC_PC7<<6));

	analog_write (0x1d,  PULL_WAKEUP_SRC_PD0 |
						(PULL_WAKEUP_SRC_PD1<<2) |
						(PULL_WAKEUP_SRC_PD2<<4) |
						(PULL_WAKEUP_SRC_PD3<<6));

	analog_write (0x1e,  PULL_WAKEUP_SRC_PD4 |
						(PULL_WAKEUP_SRC_PD5<<2) |
						(PULL_WAKEUP_SRC_PD6<<4) |
						(PULL_WAKEUP_SRC_PD7<<6));
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
		((PA0_OUTPUT_ENABLE?1:0)<<16)	| ((PA1_OUTPUT_ENABLE?1:0)<<17) | ((PA2_OUTPUT_ENABLE?1:0)<<18)	| ((PA3_OUTPUT_ENABLE?1:0)<<19) |
		((PA4_OUTPUT_ENABLE?1:0)<<20)	| ((PA5_OUTPUT_ENABLE?1:0)<<21) | ((PA6_OUTPUT_ENABLE?1:0)<<22)	| ((PA7_OUTPUT_ENABLE?1:0)<<23);

	reg_gpio_pa_setting2 =
		(PA0_DATA_STRENGTH_DS0)		| (PA1_DATA_STRENGTH_DS0<<1)| (PA2_DATA_STRENGTH_DS0<<2)	| (PA3_DATA_STRENGTH_DS0<<3) |
		(PA4_DATA_STRENGTH_DS0<<4)	| (PA5_DATA_STRENGTH_DS0<<5)	| (PA6_DATA_STRENGTH_DS0<<6)	| (PA7_DATA_STRENGTH_DS0<<7) |
		(PA0_DATA_STRENGTH_DS1<<8)	| (PA1_DATA_STRENGTH_DS1<<9)| (PA2_DATA_STRENGTH_DS1<<10)	| (PA3_DATA_STRENGTH_DS1<<11) |
		(PA4_DATA_STRENGTH_DS1<<12)	| (PA5_DATA_STRENGTH_DS1<<13)	| (PA6_DATA_STRENGTH_DS1<<14)	| (PA7_DATA_STRENGTH_DS1<<15) |
		(PA0_FUNC==AS_GPIO ? BIT(16):0)	| (PA1_FUNC==AS_GPIO ? BIT(17):0)| (PA2_FUNC==AS_GPIO ? BIT(18):0)| (PA3_FUNC==AS_GPIO ? BIT(19):0) |
		(PA4_FUNC==AS_GPIO ? BIT(20):0)	| (PA5_FUNC==AS_GPIO ? BIT(21):0)| (PA6_FUNC==AS_GPIO ? BIT(22):0)| (PA7_FUNC==AS_GPIO ? BIT(23):0);
	reg_gpio_pa_out_set = 		(PA0_DATA_OUT)	| (PA1_DATA_OUT<<1)	| (PA2_DATA_OUT<<2)	| (PA3_DATA_OUT<<3) |
							(PA4_DATA_OUT<<4)	| (PA5_DATA_OUT<<5)	| (PA6_DATA_OUT<<6)	| (PA7_DATA_OUT<<7) ;
	//PB group
	reg_gpio_pb_setting1 =
		(PB0_INPUT_ENABLE<<8) 	| (PB1_INPUT_ENABLE<<9)	| (PB2_INPUT_ENABLE<<10)	| (PB3_INPUT_ENABLE<<11) |
		((PB0_OUTPUT_ENABLE?1:0)<<16)	| ((PB1_OUTPUT_ENABLE?1:0)<<17) | ((PB2_OUTPUT_ENABLE?1:0)<<18)	| ((PB3_OUTPUT_ENABLE?1:0)<<19) |
		((PB4_OUTPUT_ENABLE?1:0)<<20)	| ((PB5_OUTPUT_ENABLE?1:0)<<21) | ((PB6_OUTPUT_ENABLE?1:0)<<22)	| ((PB7_OUTPUT_ENABLE?1:0)<<23);
	//input
	analog_write(areg_gpio_pb_ie, 	(PB4_INPUT_ENABLE<<4)	| (PB5_INPUT_ENABLE<<5) | (PB6_INPUT_ENABLE<<6)	| (PB7_INPUT_ENABLE<<7));	

	reg_gpio_pb_setting2 =
		(PB0_DATA_STRENGTH_DS0 )		| (PB1_DATA_STRENGTH_DS0<<1)| (PB2_DATA_STRENGTH_DS0<<2)	| (PB3_DATA_STRENGTH_DS0<<3) |
	    (PB0_DATA_STRENGTH_DS1<<8)		| (PB1_DATA_STRENGTH_DS1<<9)| (PB2_DATA_STRENGTH_DS1<<10)	| (PB3_DATA_STRENGTH_DS1<<11) |
		(PB0_FUNC==AS_GPIO ? BIT(16):0)	| (PB1_FUNC==AS_GPIO ? BIT(17):0)| (PB2_FUNC==AS_GPIO ? BIT(18):0)| (PB3_FUNC==AS_GPIO ? BIT(19):0) |
		(PB4_FUNC==AS_GPIO ? BIT(20):0)	| (PB5_FUNC==AS_GPIO ? BIT(21):0)| (PB6_FUNC==AS_GPIO ? BIT(22):0)| (PB7_FUNC==AS_GPIO ? BIT(23):0);
	//ds0
	analog_write(areg_gpio_pb_ds0, (PB4_DATA_STRENGTH_DS0<<4)	| (PB5_DATA_STRENGTH_DS0<<5)  | (PB6_DATA_STRENGTH_DS0<<6)	| (PB7_DATA_STRENGTH_DS0<<7) );
	//ds1
	analog_write(areg_gpio_pb_ds1, (PB4_DATA_STRENGTH_DS1<<4)	| (PB5_DATA_STRENGTH_DS1<<5)  | (PB6_DATA_STRENGTH_DS1<<6)	| (PB7_DATA_STRENGTH_DS1<<7) );
	reg_gpio_pb_out_set = 		(PB0_DATA_OUT)	| (PB1_DATA_OUT<<1)	| (PB2_DATA_OUT<<2)	| (PB3_DATA_OUT<<3) |
							(PB4_DATA_OUT<<4)	| (PB5_DATA_OUT<<5)	| (PB6_DATA_OUT<<6)	| (PB7_DATA_OUT<<7) ;

	//PC group
	//ie
	analog_write(areg_gpio_pc_ie, (PC0_INPUT_ENABLE<<0) | (PC1_INPUT_ENABLE<<1)	| (PC2_INPUT_ENABLE<<2)	| (PC3_INPUT_ENABLE<<3) |
		(PC4_INPUT_ENABLE<<4)	| (PC5_INPUT_ENABLE<<5) | (PC6_INPUT_ENABLE<<6)	| (PC7_INPUT_ENABLE<<7) );
	//oen
	reg_gpio_pc_oe =
		((PC0_OUTPUT_ENABLE?1:0)<<0)	| ((PC1_OUTPUT_ENABLE?1:0)<<1) | ((PC2_OUTPUT_ENABLE?1:0)<<2)	| ((PC3_OUTPUT_ENABLE?1:0)<<3) |
		((PC4_OUTPUT_ENABLE?1:0)<<4)	| ((PC5_OUTPUT_ENABLE?1:0)<<5) | ((PC6_OUTPUT_ENABLE?1:0)<<6)	| ((PC7_OUTPUT_ENABLE?1:0)<<7);
	//ds
	analog_write(areg_gpio_pc_ds0, 	(PC0_DATA_STRENGTH_DS0<<0) 	| (PC1_DATA_STRENGTH_DS0<<1)  | (PC2_DATA_STRENGTH_DS0<<2)	| (PC3_DATA_STRENGTH_DS0<<3) |
									(PC4_DATA_STRENGTH_DS0<<4)	| (PC5_DATA_STRENGTH_DS0<<5)  | (PC6_DATA_STRENGTH_DS0<<6)	| (PC7_DATA_STRENGTH_DS0<<7) );
	analog_write(areg_gpio_pc_ds1, 	(PC0_DATA_STRENGTH_DS1<<0) 	| (PC1_DATA_STRENGTH_DS1<<1)  | (PC2_DATA_STRENGTH_DS1<<2)	| (PC3_DATA_STRENGTH_DS1<<3) |
									(PC4_DATA_STRENGTH_DS1<<4)	| (PC5_DATA_STRENGTH_DS1<<5)  | (PC6_DATA_STRENGTH_DS1<<6)	| (PC7_DATA_STRENGTH_DS1<<7) );
	
	reg_gpio_pc_gpio =
		(PC0_FUNC==AS_GPIO ? BIT(0):0)	| (PC1_FUNC==AS_GPIO ? BIT(1):0)| (PC2_FUNC==AS_GPIO ? BIT(2):0)| (PC3_FUNC==AS_GPIO ? BIT(3):0) |
		(PC4_FUNC==AS_GPIO ? BIT(4):0)	| (PC5_FUNC==AS_GPIO ? BIT(5):0)| (PC6_FUNC==AS_GPIO ? BIT(6):0)| (PC7_FUNC==AS_GPIO ? BIT(7):0);

	reg_gpio_pc_out_set = 		(PC0_DATA_OUT)	| (PC1_DATA_OUT<<1)	| (PC2_DATA_OUT<<2)	| (PC3_DATA_OUT<<3) |
		(PC4_DATA_OUT<<4)	| (PC5_DATA_OUT<<5)	| (PC6_DATA_OUT<<6)	| (PC7_DATA_OUT<<7) ;

	//PD group
	reg_gpio_pd_setting1 =
		(PD0_INPUT_ENABLE<<8) 	| (PD1_INPUT_ENABLE<<9)	| (PD2_INPUT_ENABLE<<10)	| (PD3_INPUT_ENABLE<<11) |
		(PD4_INPUT_ENABLE<<12)	| (PD5_INPUT_ENABLE<<13)| (PD6_INPUT_ENABLE<<14)	| (PD7_INPUT_ENABLE<<15) |
		((PD0_OUTPUT_ENABLE?1:0)<<16)	| ((PD1_OUTPUT_ENABLE?1:0)<<17) | ((PD2_OUTPUT_ENABLE?1:0)<<18)	| ((PD3_OUTPUT_ENABLE?1:0)<<19) |
		((PD4_OUTPUT_ENABLE?1:0)<<20)	| ((PD5_OUTPUT_ENABLE?1:0)<<21) | ((PD6_OUTPUT_ENABLE?1:0)<<22)	| ((PD7_OUTPUT_ENABLE?1:0)<<23);

	reg_gpio_pd_setting2 =
		(PD0_DATA_STRENGTH_DS0) 	| (PD1_DATA_STRENGTH_DS0<<1)	| (PD2_DATA_STRENGTH_DS0<<2)	| (PD3_DATA_STRENGTH_DS0<<3) |
		(PD4_DATA_STRENGTH_DS0<<4)	| (PD5_DATA_STRENGTH_DS0<<5)	| (PD6_DATA_STRENGTH_DS0<<6)	| (PD7_DATA_STRENGTH_DS0<<7) |
		(PD0_DATA_STRENGTH_DS1<<8)	| (PD1_DATA_STRENGTH_DS1<<9)	| (PD2_DATA_STRENGTH_DS1<<10)	| (PD3_DATA_STRENGTH_DS1<<11) |
		(PD4_DATA_STRENGTH_DS1<<12)	| (PD5_DATA_STRENGTH_DS1<<13)	| (PD6_DATA_STRENGTH_DS1<<14)	| (PD7_DATA_STRENGTH_DS1<<15) |
		(PD0_FUNC==AS_GPIO ? BIT(16):0)	| (PD1_FUNC==AS_GPIO ? BIT(17):0)| (PD2_FUNC==AS_GPIO ? BIT(18):0)| (PD3_FUNC==AS_GPIO ? BIT(19):0) |
		(PD4_FUNC==AS_GPIO ? BIT(20):0)	| (PD5_FUNC==AS_GPIO ? BIT(21):0)| (PD6_FUNC==AS_GPIO ? BIT(22):0)| (PD7_FUNC==AS_GPIO ? BIT(23):0);

	reg_gpio_pd_out_set = 		(PD0_DATA_OUT)	| (PD1_DATA_OUT<<1)	| (PD2_DATA_OUT<<2)	| (PD3_DATA_OUT<<3) |
							(PD4_DATA_OUT<<4)	| (PD5_DATA_OUT<<5)	| (PD6_DATA_OUT<<6)	| (PD7_DATA_OUT<<7) ;
	//PE group
	reg_gpio_pe_ie = (PE0_INPUT_ENABLE<<0) | (PE1_INPUT_ENABLE<<1);
	reg_gpio_pe_oe = ((PE0_OUTPUT_ENABLE?1:0)<<0)	| ((PE1_OUTPUT_ENABLE?1:0)<<1);
	reg_gpio_pe_ds0 = (PE0_DATA_STRENGTH_DS0<<0)	| (PE1_DATA_STRENGTH_DS0<<1);
	reg_gpio_pe_ds1 = (PE0_DATA_STRENGTH_DS1<<0)	| (PE1_DATA_STRENGTH_DS1<<1);
	reg_gpio_pe_gpio = (PE0_FUNC==AS_GPIO ? BIT(0):0)	| (PE1_FUNC==AS_GPIO ? BIT(1):0);
	reg_gpio_pe_out_set = (PE0_DATA_OUT)	| (PE1_DATA_OUT<<1);

	reg_gpio_pf_setting1 =
		(PF0_INPUT_ENABLE<<8)|(PF1_INPUT_ENABLE<<9)|(PF2_INPUT_ENABLE<<10)|(PF3_INPUT_ENABLE<<11)
		|((PF0_OUTPUT_ENABLE?1:0)<<16)|((PF1_OUTPUT_ENABLE?1:0)<<17)|(PF0_DATA_OUT<<24)|(PF1_DATA_OUT<<25)|((PF2_OUTPUT_ENABLE?1:0)<<26)|((PF3_OUTPUT_ENABLE?1:0)<<27);
	reg_gpio_pf_setting2 =
		(PF0_DATA_STRENGTH_DS0<<8)|(PF1_DATA_STRENGTH_DS0<<9)|(PF2_DATA_STRENGTH_DS0<<9)|(PF3_DATA_STRENGTH_DS0<<9)|
	    (PF0_FUNC==AS_GPIO ? BIT(16):0)	| (PF1_FUNC==AS_GPIO ? BIT(17):0)| (PF2_FUNC==AS_GPIO ? BIT(18):0)| (PF3_FUNC==AS_GPIO ? BIT(19):0);
	reg_gpio_pf_out_set = 		(PF0_DATA_OUT)	| (PF1_DATA_OUT<<1)	| (PF2_DATA_OUT<<2)	| (PF3_DATA_OUT<<3);
	
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
	else if(pin == GPIO_PB4 || pin == GPIO_PB5 || pin == GPIO_PB6 || pin == GPIO_PB7)
	{
		if(value)
	    {
	    	analog_write(areg_gpio_pb_ie, analog_read(areg_gpio_pb_ie)|bit);
	    }
	    else	
	    {
	    	analog_write(areg_gpio_pb_ie, analog_read(areg_gpio_pb_ie)&(~bit));
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
void gpio_set_data_strength(GPIO_PinTypeDef pin, GPIO_Drv_Strength value)
{
	unsigned char bit = pin & 0xff;
	unsigned short group = pin & 0xf00;

    if((group == GPIO_GROUPC)||(pin == GPIO_PB4 || pin == GPIO_PB5 || pin == GPIO_PB6 || pin == GPIO_PB7))
    {
        if(DRV_STRENGTH_2MA == value)
        {
			analog_write(areg_gpio_ds0(group), analog_read(areg_gpio_ds0(group))&(~bit));
			analog_write(areg_gpio_ds1(group), analog_read(areg_gpio_ds1(group))&(~bit));
        }
        else if(DRV_STRENGTH_4MA == value)
        {
			analog_write(areg_gpio_ds0(group), analog_read(areg_gpio_ds0(group))&(~bit));
			analog_write(areg_gpio_ds1(group), analog_read(areg_gpio_ds1(group))|bit);
        }
        else if(DRV_STRENGTH_8MA == value)
        {
			analog_write(areg_gpio_ds0(group), analog_read(areg_gpio_ds0(group))|bit);
			analog_write(areg_gpio_ds1(group), analog_read(areg_gpio_pc_ds1)&(~bit));
        }
            else if(DRV_STRENGTH_12MA == value)
        {
			analog_write(areg_gpio_ds0(group), analog_read(areg_gpio_ds0(group))|bit);
			analog_write(areg_gpio_ds1(group), analog_read(areg_gpio_ds1(group))|bit);
        }
    }
	else
    {
	    if(DRV_STRENGTH_2MA == value)
    	{
    		BM_CLR(reg_gpio_ds0(pin), bit);
    		BM_CLR(reg_gpio_ds1(pin), bit);
    	}
	    else if(DRV_STRENGTH_4MA == value)
    	{
    		BM_CLR(reg_gpio_ds0(pin), bit);
    		BM_SET(reg_gpio_ds1(pin), bit);
    	}
	    else if(DRV_STRENGTH_8MA == value)
		{
    		BM_SET(reg_gpio_ds0(pin), bit);
			BM_CLR(reg_gpio_ds1(pin), bit);
		}
	    else if(DRV_STRENGTH_12MA == value)
		{
			BM_SET(reg_gpio_ds0(pin), bit);
    		BM_SET(reg_gpio_ds1(pin), bit);
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

	unsigned char base_ana_reg = 0x17 + ((gpio >> 8) << 1) + ( (gpio & 0xf0) ? 1 : 0 );  //group = gpio>>8;
	unsigned char shift_num, mask_not;

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
				reg_gpio_pa_out_clear |= bit;
				reg_gpio_pa_oe &= ~(bit);
				reg_gpio_pa_gpio |= (bit&0xf7);
				reg_gpio_pa_ie &= ((~bit)|0x08);
				break;
			case GPIO_GROUPB:
				reg_gpio_pb_out_clear |= bit;
				reg_gpio_pb_oe &= ~(bit);
				reg_gpio_pb_gpio |= bit;
				if(pin & 0x0f){
					analog_write(areg_gpio_pb_ie, analog_read(areg_gpio_pb_ie) & (~bit));
				}else{
					reg_gpio_pb_ie &= (~bit);
				}
				break;
			case GPIO_GROUPC:
				reg_gpio_pc_out_clear |= bit;
				reg_gpio_pc_oe &= ~(bit);
				reg_gpio_pc_gpio |= bit;
				analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie) & (~bit));
				break;
			case GPIO_GROUPD:
				reg_gpio_pd_out_clear |= bit;
				reg_gpio_pd_oe &= ~(bit);
				reg_gpio_pd_gpio |= bit;
				reg_gpio_pd_ie &= (~bit);
				break;
			case GPIO_GROUPE:
				reg_gpio_pe_out_clear |= bit;
				reg_gpio_pe_oe &= ~(bit);
				reg_gpio_pe_gpio |= bit;
				reg_gpio_pe_ie &= (~bit);
				break;
			case GPIO_ALL:
			{
				//set low level
				reg_gpio_pa_out_clear = 0xff;
				reg_gpio_pb_out_clear = 0xff;
				reg_gpio_pc_out_clear = 0xff;
				reg_gpio_pd_out_clear = 0xff;
				reg_gpio_pe_out_clear = 0x03;


				//output disable
				reg_gpio_pa_oe = 0x00;
				reg_gpio_pb_oe = 0x00;
				reg_gpio_pc_oe = 0x00;
				reg_gpio_pd_oe = 0x00;
				reg_gpio_pe_oe = 0x00;

				//dataO = 0
				reg_gpio_pa_gpio = 0xf7;
				reg_gpio_pb_gpio = 0xff;
				reg_gpio_pc_gpio = 0xff;
				reg_gpio_pd_gpio = 0xff;
				reg_gpio_pe_gpio = 0x03;

				//ie = 0
				reg_gpio_pa_ie = 0x08;			  //SWS   501<3>
				reg_gpio_pb_ie = 0x00;
				analog_write(areg_gpio_pb_ie, 0);
				analog_write(areg_gpio_pc_ie, 0);
				reg_gpio_pd_ie = 0x00;
				reg_gpio_pe_ie = 0x00;
			}
		}
}

/**
 * @brief     This function performs to probe clock to IO.
 * @param[in] pin - the pin to probe clock, only support PB[0]/PD0/PE[1:0].
 * @param[in] sel_clk - the clock source which you want to probe.
 * @return    none.
 */
void gpio_set_probe_clk_function(GPIO_PinTypeDef pin, prob_clock_src_e sel_clk)
{
	write_reg8(0x75, ((read_reg8(0x75) & 0xf8) | sel_clk));	//0:clk_7816,  1:clk32k,   2:clk_sys,      3:rc24m
												  	    //4:xtl24m,    5:clkpll,   6:clk_stimer,
	BM_CLR(reg_gpio_func(pin), (pin&0xff));

	if (pin == GPIO_PB0) {
		reg_gpio_func_mux(pin) = 19;// PB0 as CLK_7816
	} else if (pin == GPIO_PD0) {
		reg_gpio_func_mux(pin) = 0;// PD0 as CLK_7816
	} else if ((pin == GPIO_PE0) || (pin == GPIO_PE1)) {
		reg_gpio_func_mux(pin) = 2;// PE0/PE1 as CLK_7816
	}	
}


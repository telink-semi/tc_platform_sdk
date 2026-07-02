/********************************************************************************************************
 * @file    gpio.c
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
#include "bsp.h"
#include "compiler.h"
#include "register.h"
#include "analog.h"
#include "gpio.h"
#include "lib/include/sys.h"

/**
 * @brief      This function servers to set the GPIO's function.
 * @param[in]  pin - the special pin.
 * @param[in]  func - the function of GPIO.
 * @return     none.
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

	if(value)
	{
		if(pin == GPIO_PA6 || pin == GPIO_PA7){
			analog_write(areg_gpio_pa6to7_pb0to2_ie, analog_read(areg_gpio_pa6to7_pb0to2_ie)| (bit>>6));
		}
		else if(pin == GPIO_PB0 || pin == GPIO_PB1 || pin == GPIO_PB2){
			analog_write(areg_gpio_pa6to7_pb0to2_ie, analog_read(areg_gpio_pa6to7_pb0to2_ie)| (bit<<2));
		}
		else{
			BM_SET(reg_gpio_ie(pin),bit);
		}
	}
	else{
		if(pin == GPIO_PA6 || pin == GPIO_PA7){
			analog_write(areg_gpio_pa6to7_pb0to2_ie, analog_read(areg_gpio_pa6to7_pb0to2_ie) &(~(bit>>6)));
		}
		else if(pin == GPIO_PB0 || pin == GPIO_PB1 || pin == GPIO_PB2){
			analog_write(areg_gpio_pa6to7_pb0to2_ie, analog_read(areg_gpio_pa6to7_pb0to2_ie) &(~(bit<<2)));
		}
		else{
			BM_CLR(reg_gpio_ie(pin),bit);
		}
	}
}

/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  en - 1: enable driving strength, 0:disable driving strength
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin,  unsigned int en)
{
	unsigned char bit = pin & 0xff;

	if(en)
	{
		if(pin == GPIO_PA6 || pin == GPIO_PA7){

			analog_write(areg_gpio_pa6to7_pb0to2_ds, analog_read(areg_gpio_pa6to7_pb0to2_ds)| (bit>>6));
		}
		else if(pin == GPIO_PB0 || pin == GPIO_PB1 || pin == GPIO_PB2){
			analog_write(areg_gpio_pa6to7_pb0to2_ds, analog_read(areg_gpio_pa6to7_pb0to2_ds)| (bit<<2));
		}
		else{
			BM_SET(reg_gpio_ds(pin),bit);
		}
	}
	else{
		if(pin == GPIO_PA6 || pin == GPIO_PA7){

			analog_write(areg_gpio_pa6to7_pb0to2_ds, analog_read(areg_gpio_pa6to7_pb0to2_ds) &(~(bit>>6)));
		}
		else if(pin == GPIO_PB0 || pin == GPIO_PB1 || pin == GPIO_PB2){
			analog_write(areg_gpio_pa6to7_pb0to2_ds, analog_read(areg_gpio_pa6to7_pb0to2_ds) &(~(bit<<2)));
		}
		else{
			BM_CLR(reg_gpio_ds(pin),bit);
		}
	}
}

/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO,PB0 is not included in GPIO_ALL
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
			reg_gpio_pa_oe |= bit;
			reg_gpio_pa_gpio |= (bit&0xf7);
			reg_gpio_pa_ie &= ((~bit)|0x08);
			break;

		case GPIO_GROUPB:
			reg_gpio_pb_out_clear |= bit;
			reg_gpio_pb_oe |= bit;
			reg_gpio_pb_gpio |= bit;
			reg_gpio_pb_ie &= (~bit);
			break;

		case GPIO_GROUPC: 
            reg_gpio_pc_out_clear |= bit; 
            reg_gpio_pc_oe |= bit;
            reg_gpio_pc_gpio |= bit; 
            reg_gpio_pc_ie &= (~bit);    
			break;

		case GPIO_ALL:

			//set low level
			reg_gpio_pa_out_clear = 0xf7;//PA3 is SWS
			reg_gpio_pb_out_clear = 0xff;
			reg_gpio_pc_out_clear = 0x03;//PC2-5 is MSPI

			//output disable
			reg_gpio_pa_oe = 0xff;//PA3 is SWS
			reg_gpio_pb_oe = 0xff;
			reg_gpio_pc_oe = 0x03;//PC2-5 is MSPI

			//as gpio
			reg_gpio_pa_gpio = 0xf7;//PA3 is SWS
			reg_gpio_pb_gpio = 0xff;
			reg_gpio_pc_gpio = 0x03;//PC2-5 is MSPI

			//disable input
			reg_gpio_pa_ie = 0x08;//SWS input is enabled
            reg_gpio_pb_ie = 0x00;
			reg_gpio_pc_ie = 0x3C;

            /* A0 chip: PA1 used as flash cs 1M pull-up function, PA3(SWS) 1M pull-up,
             * PC2~PC5(MSPI) have no pull-down or pull up function, others pull-down.
             */
            if (g_chip_version == CHIP_VERSION_A0)
            {
                analog_write(0x18, 0x66);
            }
            else
            {
                analog_write(0x18, 0x6a);
            }
            analog_write(0x19, 0xaa);
            analog_write(0x1a, 0xaa);
            analog_write(0x1b, 0xaa);
			analog_write(0x1c, 0x0a);

			break;
			
		default:break;
	}
}
/**
 * @brief      This function servers to initiate pull up-down resistor of all gpio.
 * @param[in]  none
 * @return     none.
 */
static void gpio_analog_resistance_init(void)
{
	analog_write (0x18,  PULL_WAKEUP_SRC_PA0 |
						(PULL_WAKEUP_SRC_PA1<<2) |
						(PULL_WAKEUP_SRC_PA2<<4) |
						(PULL_WAKEUP_SRC_PA3<<6));

	analog_write (0x19,  PULL_WAKEUP_SRC_PA4 |
						(PULL_WAKEUP_SRC_PA5<<2) |
						(PULL_WAKEUP_SRC_PA6<<4) |
						(PULL_WAKEUP_SRC_PA7<<6));


	analog_write (0x1a,  PULL_WAKEUP_SRC_PB0 |
						(PULL_WAKEUP_SRC_PB1<<2) |
						(PULL_WAKEUP_SRC_PB2<<4) |
						(PULL_WAKEUP_SRC_PB3<<6));

	analog_write (0x1b,  PULL_WAKEUP_SRC_PB4 |
						(PULL_WAKEUP_SRC_PB5<<2) |
						(PULL_WAKEUP_SRC_PB6<<4) |
						(PULL_WAKEUP_SRC_PB7<<6));
						
    analog_write (0x1C,  (analog_read(0x1C) & 0xf0)| PULL_WAKEUP_SRC_PC0 |
                                            (PULL_WAKEUP_SRC_PC1<<2));
}

/**
 * @brief      This function servers to initialization all gpio.
 * @param[in]  en  -  if mcu wake up from deep retention mode, determine whether it is NOT necessary to reset analog register
 *                    1: set analog register
 *                    0: not set analog register
 * @return     none.
 */
#if (BLC_PM_DEEP_RETENTION_MODE_EN)
_attribute_ram_code_sec_noinline_
#endif
void gpio_init(int anaRes_init_en)
{
    //PA group
	reg_gpio_pa_setting1 =
		(PA0_INPUT_ENABLE<<8) 	| (PA1_INPUT_ENABLE<<9)	| (PA2_INPUT_ENABLE<<10)	| (PA3_INPUT_ENABLE<<11) |
		(PA4_INPUT_ENABLE<<12)	| (PA5_INPUT_ENABLE<<13) |
		((PA0_OUTPUT_ENABLE?0:1)<<16)	| ((PA1_OUTPUT_ENABLE?0:1)<<17) | ((PA2_OUTPUT_ENABLE?0:1)<<18)	| ((PA3_OUTPUT_ENABLE?0:1)<<19) |
		((PA4_OUTPUT_ENABLE?0:1)<<20)	| ((PA5_OUTPUT_ENABLE?0:1)<<21) | ((PA6_OUTPUT_ENABLE?0:1)<<22)	| ((PA7_OUTPUT_ENABLE?0:1)<<23) ;
	reg_gpio_pa_ds=
			(PA0_DATA_STRENGTH)		| (PA1_DATA_STRENGTH<<1)| (PA2_DATA_STRENGTH<<2)	| (PA3_DATA_STRENGTH<<3) |
			(PA4_DATA_STRENGTH<<4)	| (PA5_DATA_STRENGTH<<5);
	reg_gpio_pa_gpio =
		(PA0_FUNC==AS_GPIO ? BIT(0):0)	| (PA1_FUNC==AS_GPIO ? BIT(1):0)| (PA2_FUNC==AS_GPIO ? BIT(2):0)| (PA3_FUNC==AS_GPIO ? BIT(3):0) |
	    (PA4_FUNC==AS_GPIO ? BIT(4):0)	| (PA5_FUNC==AS_GPIO ? BIT(5):0)| (PA6_FUNC==AS_GPIO ? BIT(6):0)| (PA7_FUNC==AS_GPIO ? BIT(7):0);

    reg_gpio_pa_out_set_clear = ((PA0_DATA_OUT) ? BIT(0): BIT(8)) |((PA1_DATA_OUT) ? BIT(1): BIT(9)) |((PA2_DATA_OUT) ? BIT(2): BIT(10))|((PA3_DATA_OUT) ? BIT(3): BIT(11))|
                                ((PA4_DATA_OUT) ? BIT(4): BIT(12))|((PA5_DATA_OUT) ? BIT(5): BIT(13))|((PA6_DATA_OUT) ? BIT(6): BIT(14))|((PA7_DATA_OUT) ? BIT(7): BIT(15));
    //PB group
	reg_gpio_pb_setting1 =
		(PB3_INPUT_ENABLE<<11) |
		(PB4_INPUT_ENABLE<<12)	| (PB5_INPUT_ENABLE<<13) | (PB6_INPUT_ENABLE<<14) | (PB7_INPUT_ENABLE<<15) |
		((PB0_OUTPUT_ENABLE?0:1)<<16)	| ((PB1_OUTPUT_ENABLE?0:1)<<17) | ((PB2_OUTPUT_ENABLE?0:1)<<18)	| ((PB3_OUTPUT_ENABLE?0:1)<<19) |
		((PB4_OUTPUT_ENABLE?0:1)<<20)	| ((PB5_OUTPUT_ENABLE?0:1)<<21) | ((PB6_OUTPUT_ENABLE?0:1)<<22) | ((PB7_OUTPUT_ENABLE?0:1)<<23) ;
	reg_gpio_pb_ds=
			(PB3_DATA_STRENGTH<<3) |
			(PB4_DATA_STRENGTH<<4)	| (PB5_DATA_STRENGTH<<5) | (PB6_DATA_STRENGTH<<6) | (PB7_DATA_STRENGTH<<7);
	reg_gpio_pb_gpio =
		(PB0_FUNC==AS_GPIO ? BIT(0):0)	| (PB1_FUNC==AS_GPIO ? BIT(1):0)| (PB2_FUNC==AS_GPIO ? BIT(2):0)| (PB3_FUNC==AS_GPIO ? BIT(3):0) |
	    (PB4_FUNC==AS_GPIO ? BIT(4):0)	| (PB5_FUNC==AS_GPIO ? BIT(5):0)| (PB6_FUNC==AS_GPIO ? BIT(6):0)| (PB7_FUNC==AS_GPIO ? BIT(7):0);

    reg_gpio_pb_out_set_clear = ((PB0_DATA_OUT) ? BIT(0): BIT(8)) |((PB1_DATA_OUT) ? BIT(1): BIT(9)) |((PB2_DATA_OUT) ? BIT(2): BIT(10))|((PB3_DATA_OUT) ? BIT(3): BIT(11))|
                                ((PB4_DATA_OUT) ? BIT(4): BIT(12))|((PB5_DATA_OUT) ? BIT(5): BIT(13))|((PB6_DATA_OUT) ? BIT(6): BIT(14))|((PB7_DATA_OUT) ? BIT(7): BIT(15));


	reg_gpio_pc_setting1 =
	(PC0_INPUT_ENABLE<<8) 	| (PC1_INPUT_ENABLE<<9)	 | (PC2_INPUT_ENABLE<<10)	| (PC3_INPUT_ENABLE<<11) |
	(PC4_INPUT_ENABLE<<12)	| (PC5_INPUT_ENABLE<<13) |
	((PC0_OUTPUT_ENABLE?0:1)<<16)	| ((PC1_OUTPUT_ENABLE?0:1)<<17) | ((PC2_OUTPUT_ENABLE?0:1)<<18)	| ((PC3_OUTPUT_ENABLE?0:1)<<19) |
	((PC4_OUTPUT_ENABLE?0:1)<<20)	| ((PC5_OUTPUT_ENABLE?0:1)<<21)  ;
	reg_gpio_pc_ds=
			(PC0_DATA_STRENGTH)		| (PC1_DATA_STRENGTH<<1) | (PC2_DATA_STRENGTH<<2) | (PC3_DATA_STRENGTH<<3) |
			(PC4_DATA_STRENGTH<<4)	| (PC5_DATA_STRENGTH<<5);
	reg_gpio_pc_gpio =
		(PC0_FUNC==AS_GPIO ? BIT(0):0)	| (PC1_FUNC==AS_GPIO ? BIT(1):0)| (PC2_FUNC==AS_GPIO ? BIT(2):0)| (PC3_FUNC==AS_GPIO ? BIT(3):0) |
	    (PC4_FUNC==AS_GPIO ? BIT(4):0)	| (PC5_FUNC==AS_GPIO ? BIT(5):0);

    reg_gpio_pc_out_set_clear = ((PC0_DATA_OUT) ? BIT(0): BIT(8)) |((PC1_DATA_OUT) ? BIT(1): BIT(9)) |((PC2_DATA_OUT) ? BIT(2): BIT(10))|((PC3_DATA_OUT) ? BIT(3): BIT(11))|
                                ((PC4_DATA_OUT) ? BIT(4): BIT(12))|((PC5_DATA_OUT) ? BIT(5): BIT(13));

									//PA6-7,PB0-2
	analog_write(areg_gpio_pa6to7_pb0to2_ie, (PA6_INPUT_ENABLE<<0) 	| (PA7_INPUT_ENABLE<<1)	| (PB0_INPUT_ENABLE<<2)	| (PB1_INPUT_ENABLE<<3) | (PB2_INPUT_ENABLE<<4));
	analog_write(areg_gpio_pa6to7_pb0to2_ds, (PA6_DATA_STRENGTH<<0) | (PA6_DATA_STRENGTH<<1) | (PB0_DATA_STRENGTH<<2) | (PB1_DATA_STRENGTH<<3) | (PB2_DATA_STRENGTH<<4));

	if(anaRes_init_en)
	{
		gpio_analog_resistance_init();
	}
}

/**
 * @brief     This function performs to probe clock to IO.
 * @param[in] pin - the pin to probe clock, only support PA2.
 * @param[in] sel_clk - the clock source which you want to probe.
 * @return    none.
 */
void gpio_set_probe_clk_function(prob_clock_src_e sel_clk)
{
	gpio_set_func(GPIO_PC1,AS_PROBE_CLK);
	write_reg8(0x65, ((read_reg8(0x65) & 0xfc) | sel_clk));	//0:clk_32k, 1:clk_7816, 2:clk_sys
}

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */
void gpio_setup_up_down_resistor(GPIO_PinTypeDef gpio, GPIO_PullTypeDef up_down)
{
	unsigned char r_val = up_down & 0x03;

	unsigned char base_ana_reg = 0x18 + ((gpio >> 8) << 1) + ( (gpio & 0xf0) ? 1 : 0 );  //group = gpio>>8;
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
 * @brief     This function set pin's 30k pull-up register.
 * @param[in] pin - the pin needs to set its pull-up register.
 * @param[in]  value - enable or disable the pin's input function(0: disable, 1: enable)
 * @return    none.
 * @attention This function sets the digital pull-up, it will not work after entering low power consumption.
 */
void gpio_set_pullup_res_30k_en(GPIO_PinTypeDef pin,unsigned int value){

	unsigned char bit = pin & 0xff;

	if(value)
	{
		if(pin == GPIO_PA6 || pin == GPIO_PA7){
			analog_write(areg_gpio_pa6to7_pb0to2_pd, analog_read(areg_gpio_pa6to7_pb0to2_pd)| (bit>>6));
		}
		else if(pin == GPIO_PB0 || pin == GPIO_PB1 || pin == GPIO_PB2){
			analog_write(areg_gpio_pa6to7_pb0to2_pd, analog_read(areg_gpio_pa6to7_pb0to2_pd)| (bit<<2));
		}
		else{
			BM_SET(reg_gpio_pe(pin),bit);
		}
	}
	else{
		if(pin == GPIO_PA6 || pin == GPIO_PA7){

			analog_write(areg_gpio_pa6to7_pb0to2_pd, analog_read(areg_gpio_pa6to7_pb0to2_pd) &(~(bit>>6)));
		}
		else if(pin == GPIO_PB0 || pin == GPIO_PB1 || pin == GPIO_PB2){
			analog_write(areg_gpio_pa6to7_pb0to2_pd, analog_read(areg_gpio_pa6to7_pb0to2_pd) &(~(bit<<2)));
		}
		else{
			BM_CLR(reg_gpio_pe(pin),bit);
		}
	}
}

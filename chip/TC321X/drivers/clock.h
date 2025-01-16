/********************************************************************************************************
 * @file    clock.h
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


#include "compiler.h"
#include "register.h"
#include "gpio.h"


#define 	_ASM_NOP_			asm("tnop")

/**
 * @brief  instruction delay.
 */
#define		CLOCK_DLY_1_CYC    _ASM_NOP_
#define		CLOCK_DLY_2_CYC    _ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_3_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_4_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_5_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_6_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_7_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_8_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_9_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_10_CYC   _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_



extern unsigned char system_clk_type;

/**
 * @brief 	Power type for different application
 */
typedef enum{
	LDO_MODE 		=0x40,	//LDO mode
	DCDC_LDO_MODE	=0x41,	//DCDC_LDO mode
}POWER_MODE_TypeDef;

/**
 * @brief:	External 24M crystal using internal or external capacitors
 * @note:	If the software configuration and hardware board does not match,
 *          it may lead to the following problems:
 *          crystal clock frequency is not allowed,  slow crystal vibration caused by the chip reset, etc.
 */
typedef enum{
	INTERNAL_CAP_XTAL24M = 0, /**<    Use the chip's internal crystal capacitors,
	                             <p>  hardware boards can not have 24M crystal matching capacitors */
	EXTERNAL_CAP_XTAL24M = 1, /**<    Use an external crystal capacitor,
	                             <p>  the hardware board needs to have a matching capacitor for the 24M crystal,
	                             <p>  the program will turn off the chip's internal capacitor */
}cap_typedef_e;



typedef enum{
	SYS_CLK_12M_Crystal = 0x44,
	SYS_CLK_16M_Crystal = 0x43,
	SYS_CLK_24M_Crystal = 0x42,
	SYS_CLK_32M_Crystal = 0x60,
	SYS_CLK_48M_Crystal = 0x20,


	SYS_CLK_24M_RC 	 = 0x00,

}SYS_CLK_TypeDef;


/**
 * @brief 32K clock type.
 * @note  this chip do not support 32k xtal, here is to keep compatibility.
 */
typedef enum{
	CLK_32K_RC   =0,
	//CLK_32K_XTAL =1,
}CLK_32K_TypeDef;

typedef enum{
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
 * @brief       This function to select the system clock source.
 * @param[in]   SYS_CLK - the clock source of the system clock.
 * @note		1. Do not switch the clock during the DMA sending and receiving process because during the clock switching process, 
 * 					the system clock will be suspended for a period of time, which may cause data loss.
 * 				2. When this function called after power on or deep sleep wakeup, it will perform 24m rc calibration. 
 * 					The usage rules of 24m rc, please refer to the rc_24m_cal() for details.
 * 					If do not want this logic, you can check the usage and precautions of clock_init_calib_24m_rc_cfg().
 */
void clock_init(SYS_CLK_TypeDef SYS_CLK);

/**
 * @brief       This function to get the system clock source.
 * @param[in]   none
 * @return      system clock source
 */

static inline int clock_get_system_clk(void)
{
	return system_clk_type;
}

/**
 * @brief       This function use to restore all previously saved clock configurations.
 * @return      none.
 * @note        This function needs to be used in conjunction with clock_save_clock_config().
 */
static _always_inline void clock_restore_clock_config(unsigned char cfg)
{
	write_reg8 (0x066, cfg);      //restore system clock
}

/**
 * @brief   This function serves to set 32k clock source.
 * @return  none.
 */
void clock_32k_init (CLK_32K_TypeDef src);

/**
 * @brief     This function performs to select 24M as the system clock source.
 * @param[in] none.
 * @return    none.
 * @note	  During the first power-on, after the xtal is stable (cpu_wakeup_init()), it is necessary to calibrate the 24m rc as soon as possible 
 * 				to prevent some unknown problems caused by a large frequency deviation of the RC clock.
 *            1. If the sleep function is not used and the accuracy of 24m rc is not high, then there is no need for regular calibration.
 *            2. If the sleep wake-up function is required, it is necessary to calibrate the 24m rc before the first sleep, otherwise it may cause the 
 * 					oscillator to fail to start after waking up.The recommended interval for regular calibration is 10 seconds. 
 *            3. If the 24m rc is more accurate, the oscillator will start up faster after waking up. If it is not accurate, the oscillator may not start
 * 					up after waking up.Therefore, regular calibration is needed to prevent the impact of temperature changes.
 */
void rc_24m_cal (void);


/**
 * @brief     This function performs to select 32K as the system clock source.
 * @param[in] none.
 * @return    none.
 * @note	  1. If a more accurate 32K RC timing is required, then to prevent temperature effects, calibration can be performed regularly.
 * 			  2. If it is to ensure accurate sleep time, then the 32K RC calibration is not necessary. Although sleep time is measured by 32K RC, 
 * 				    sleep time is obtained through tracking way and will not affected by 32K RC deviation. So in this scenario, it is necessary to 
 * 				    calibrate once when power-on (to prevent significant frequency deviation caused by 32K RC), and regular calibration is not necessary.
 */
void rc_32k_cal (void);

/**
 * @brief     This function performs to probe clock to IO.
 * @param[in] src - the clock source which you want to probe.
 * @param[in] pin - the pin to probe clock, only support PB[0]/PD0/PE[1:0].
 * @return    none.
 */
void clock_prob(prob_clock_src_e src, GPIO_PinTypeDef pin);

/**
 * @brief 	  This function performs to configure whether to calibrate the 24m rc in the clock_init() when power-on or wakeup from deep sleep mode.
 * 				If wakeup from deep retention sleep mode will not calibrate.
 * @param[in] calib_flag - Choose whether to calibrate the 24m rc or not.
 * 						1 - calibrate; 0 - not calibrate
 * @return	  none
 * @note	  This function will not take effect until it is called before clock_init(). 
 */
void clock_init_calib_24m_rc_cfg(char calib_flag);

/**
 * @brief  This function serves to set the 32k tick.
 * @param  none.
 * @return none.
 */
_attribute_ram_code_sec_noinline_ void cpu_set_32k_tick(unsigned int tick);

/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */
_attribute_ram_code_sec_noinline_ unsigned int pm_get_32k_tick(void);

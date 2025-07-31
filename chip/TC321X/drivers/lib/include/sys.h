/********************************************************************************************************
 * @file    sys.h
 *
 * @brief   This is the header file for TC321X
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

#include "compiler.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/**
 * @brief instruction delay
 */

#define     _ASM_NOP_			asm("tnop")

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
#define		CLOCK_DLY_20_CYC   CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC
#define		CLOCK_DLY_40_CYC   CLOCK_DLY_20_CYC;CLOCK_DLY_20_CYC
#define		CLOCK_DLY_80_CYC   CLOCK_DLY_40_CYC;CLOCK_DLY_40_CYC


/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 * @brief:  External 24M crystal using internal or external capacitors
 * @note:   If the software configuration and hardware board does not match,
 *          it may lead to the following problems:
 *          crystal clock frequency is not allowed,  slow crystal vibration caused by the chip reset, etc.
 */
typedef enum
{
    INTERNAL_CAP_XTAL24M = 0, /**<    Use the chip's internal crystal capacitors,
                                 <p>  hardware boards can not have 24M crystal matching capacitors */
    EXTERNAL_CAP_XTAL24M = 1, /**<    Use an external crystal capacitor,
                                 <p>  the hardware board needs to have a matching capacitor for the 24M crystal,
                                 <p>  the program will turn off the chip's internal capacitor */
}cap_typedef_e;


/**
 * @brief   Power type for different application
 */
typedef enum
{
	LDO_MODE 		=0x40,	//LDO mode
	DCDC_LDO_MODE	=0x41,	//DCDC_LDO mode
}POWER_MODE_TypeDef;

/**
 * @brief   chip version.
 * @note    this value should confirm when chip reversion.
 */
typedef enum
{
    CHIP_VERSION_A0 = 0x00,
} sys_chip_version_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

extern unsigned char g_chip_version;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief   This function serves to reboot chip.
 * @param   none.
 * @return  none.
 */
void start_reboot(void);

/**
 * @brief   This function serves to initialize MCU
 * @param   xtal - set crystal for different application.
 * @return  none
 * @note    1. In version A0, the chip cannot be lower than 2.2V when it is powered on for the first time.
 *              After calling this function, g_chip_version is the version number recorded.
 *          2. For crystal oscillators with very slow start-up or poor quality, after calling this function,
 *              a reboot will be triggered (whether a reboot has occurred can be judged by using DEEP_ANA_REG0[bit1]).
 *              For the case where the crystal oscillator used is very slow to start, you can call the pm_set_wakeup_time_param()
 *              to adjust the waiting time for the crystal oscillator to start before calling the cpu_wakeup_init().
 *              When this time is adjusted to meet the crystal oscillator requirements, it will not reboot.
 *          3. When this function called after power on or deep sleep wakeup, it will cost about 6~7ms for perform 32k RC calibration.
 *              If do not want this logic, you can check the usage and precautions of cpu_wakeup_init_calib_32k_rc_cfg().
 */
void cpu_wakeup_init(POWER_MODE_TypeDef power_mode, cap_typedef_e cap);

/**
 * @brief     this function servers to manual set crystal.
 * @param[in] none.
 * @return    none.
 * @note      This function can only used when cclk is 24M RC cause the function execution process will power down the 24M crystal.
 */
_attribute_ram_code_sec_noinline_ void crystal_manual_settle(void);

/**
 * @brief     This function performs to configure whether to calibrate the 32k rc in the cpu_wakeup_init() when power-on or wakeup from deep sleep mode.If wakeup from deep retention sleep mode will not calibrate.
 * @param[in] calib_flag - Choose whether to calibrate the 32k rc or not.
 *                      1 - calibrate; 0 - not calibrate
 * @return    none
 * @note      This function will not take effect until it is called before cpu_wakeup_init().
 */
void cpu_wakeup_init_calib_32k_rc_cfg(char calib_flag);

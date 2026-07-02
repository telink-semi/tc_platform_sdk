/********************************************************************************************************
 * @file    pm_internal.h
 *
 * @brief   This is the header file for TC122X
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
#include "compiler.h"
#include "analog.h"
#include "lib/include/pm/pm.h"
#include "driver_func_cfg.h"

/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/*
 * @note    This is for internal stability debugging use only.
 */
#define PM_DEBUG                 0
//1 PB2, 2 PA2
#define PM_DEBUG_PAD             GPIO_PA2
#define PM_SUSPEND_WHILE_DEBUG   0
#define PM_SUSPEND_WHILE_DEBUG_2 0
#define PM_START_CODE_DEBUG      0
#define PM_XTAL_READY_DEBUG      0
#define PM_XTAL_ONCE_DEBUG       0
#define PM_XTAL_READY_TIME       0
#define PM_MANUAL_SETTLE_DEBUG   0

extern unsigned char g_pm_system_reboot_event;
extern _attribute_data_retention_ unsigned int   g_tracking_32k_stimer_tick;
extern _attribute_data_retention_ unsigned int   g_current_stimer_tick;
extern _attribute_data_retention_ unsigned int   g_current_32k_tick;
extern _attribute_data_retention_ unsigned char  g_pm_long_suspend;
extern _attribute_data_retention_ unsigned char  g_pb_pupd_reg[3];

#if (PM_DEBUG)
extern volatile unsigned char debug_pm_info;
extern volatile unsigned int debug_ana_32k_tick;
extern volatile unsigned int debug_ana_tick_reset;
extern volatile unsigned int debug_sleep_32k_cur;
extern volatile unsigned int debug_tick_32k_cur;
extern volatile unsigned char debug_min_wakeup_src;
extern volatile unsigned char debug_sleep_start_wakeup_src0;
extern volatile unsigned char debug_sleep_start_wakeup_src1;
extern volatile unsigned char debug_sleep_start_wakeup_src2;
extern volatile unsigned int debug_min_stimer_tick;
extern volatile unsigned int debug_sleep_start_cur_tick;
extern volatile unsigned int debug_sleep_start_set_tick;
extern volatile unsigned int debug_sleep_wakeup_return;
extern volatile unsigned long long debug_while_7d_tick_1;
extern volatile unsigned long long debug_while_7d_tick_2;
extern volatile unsigned long long debug_while_7d_tick_3;
extern volatile unsigned long long debug_min_code_tick_1;
extern volatile unsigned long long debug_min_code_tick_2;
extern volatile unsigned long long debug_min_code_tick_3;
extern volatile unsigned char debug_ana_reg[128];
extern volatile unsigned char debug_xtal_num;
#endif


/**
 * @brief trim dig LDO
 *
 */
typedef enum
{
    DIG_LDO_TRIM_1P001V = 0,
	DIG_LDO_TRIM_1P051V,
	DIG_LDO_TRIM_1P098V,
	DIG_LDO_TRIM_1P150V,
	DIG_LDO_TRIM_1P198V, //default
	DIG_LDO_TRIM_1P250V,
	DIG_LDO_TRIM_1P295V,
	DIG_LDO_TRIM_1P343V,
} pm_dig_ldo_trim_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum
{
    RET_LDO_TRIM_0P812V = 0,
	RET_LDO_TRIM_0P877V,
	RET_LDO_TRIM_0P942V,
	RET_LDO_TRIM_1P008V,
	RET_LDO_TRIM_1P073V, //default
	RET_LDO_TRIM_1P138V,
	RET_LDO_TRIM_1P200V,

	RET_LDO_TRIM_0P552V,
	RET_LDO_TRIM_0P617V,
	RET_LDO_TRIM_0P682V,
	RET_LDO_TRIM_0P747V,
} pm_ret_ldo_trim_e;

/**
 * @brief       This function serves to reboot system.
 * @return      none
 */
_always_inline void sys_reset_all(void)
{
#if (PM_DEBUG)
    while(1){}
#endif
    reg_pwdn_en = 0x20;
}

/**
 * @brief       This function serves to trim dig LDO voltage
 * @param[in]   dig_ldo_trim - dig LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_dig_ldo_voltage(pm_dig_ldo_trim_e dig_ldo_trim)
{
    analog_write(areg_0x03, (analog_read(areg_0x03) & 0xf8) | (dig_ldo_trim));
}

/**
 * @brief       This function serves to trim dig LDO voltage
 * @param[in]   dig_ldo_trim - dig LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_ret_ldo_voltage(pm_ret_ldo_trim_e ret_ldo_trim)
{
    analog_write(areg_0x00, (analog_read(areg_0x00) & 0xf0) | ret_ldo_trim);
}

/**
 * @brief   This function is used to enable retention LDO.
 * @return  none.
 */
static _always_inline void pm_enable_ret_ldo(void)
{
    analog_write(areg_0x05, analog_read(areg_0x05) & ~(FLD_PD_RET_LDO));
}

/**
 * @brief   This function is used to disable retention LDO.
 * @return  none.
 */
static _always_inline void pm_disable_ret_ldo(void)
{
    analog_write(areg_0x05, analog_read(areg_0x05) | FLD_PD_RET_LDO);
}

/**
 * @brief   This function is used to enable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_enable_spd_ldo(void)
{
    analog_write(areg_0x05, analog_read(areg_0x05) & ~(FLD_PD_SPD_LDO));
}

/**
 * @brief   This function is used to disable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_disable_spd_ldo(void)
{
    analog_write(areg_0x05, analog_read(areg_0x05) | FLD_PD_SPD_LDO);
}

/**
 * @brief       This function configures the values of r_delay cycle.
 * @param[in]   r_delay - r_delay cycle.
 * @return      none.
 */
static _always_inline void pm_set_delay_cycle(unsigned char r_delay)
{
    /**
     *  timer wake up: (n*2) 32k cycle
     *  pad wake up  : (n*2-1) ~ (n*2)32k cycle
     */
    analog_write(0x20, r_delay);
}

/**
 * @brief       This function is used to set reboot reason.
 * @return      none.
 */
static _always_inline void pm_set_reboot_reason(pm_sw_reboot_reason_e reboot_reason)
{
    analog_write(PM_ANA_REG_POWER_ON_CLR_BUF0, REBOOT_OR_POWERON_FLAG | (reboot_reason << 1));
}

/**
 * @brief       This function serves to get 32k set tick.
 * @return      32k set tick.
 */
static _always_inline unsigned int  pm_get_32k_set_tick(void)
{
    return (analog_read(0x30)<<24)|(analog_read(0x2f)<<16)|(analog_read(0x2e)<<8)|(analog_read(0x2d));
}
/**
 * @brief       This function serves to get 32k current tick.
 * @return      32k set tick.
 */
static _always_inline unsigned int  pm_get_32k_cur_tick(void)
{
    return (analog_read(0x2b)<<24)|(analog_read(0x2a)<<16)|(analog_read(0x29)<<8)|(analog_read(0x28));
}

#if !FLASH_SDK_ENABLE
/**
 * @brief       This function is used to determine the stability of the crystal oscillator.
 *              To judge the stability of the crystal oscillator, xo_ready_ana is invalid, and use an alternative solution to judge.
 *              Alternative principle: Because the clock source of the stimer is the crystal oscillator,
 *              if the crystal oscillator does not vibrate, the tick value of the stimer does not increase or increases very slowly (when there is interference).
 *              So first use 24M RC to run the program and wait for a fixed time, calculate the number of ticks that the stimer should increase during this time,
 *              and then read the tick value actually increased by the stimer.
 *              When it reaches 50% of the calculated value, it proves that the crystal oscillator has started.
 *              If it is not reached for a long time, the system will reboot.
 * @attention   This function can only be called with the 24M clock configuration.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_wait_xtal_ready(void);

/**
 * @brief   This function serves to recover system timer from tick of internal 32k RC.
 *          The code is placed in the ram code section, in order to shorten the time.
 * @return  none.
 */
_attribute_ram_code_sec_noinline_ void pm_stimer_recover_32k_rc(void);

/**
 * @brief       this function serves to start sleep mode.
 * @param[in]   sleep_mode  - sleep mode type select.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_sleep_start(unsigned char sleep_mode);
#endif

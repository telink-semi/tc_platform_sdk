/********************************************************************************************************
 * @file    pm_internal.h
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

#include "register.h"
#include "compiler.h"
#include "analog.h"
#include "lib/include/pm/pm.h"
#include "lib/include/stimer.h"

extern unsigned char  g_24m_rc_is_used;

/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/*
 * @note    This is for internal stability debugging use only.
 */
#define PM_DEBUG                 0
//1 PB4, 2 PB5
#define PM_DEBUG_PAD             GPIO_PB5
#define PM_SUSPEND_WHILE_DEBUG   0
#define PM_SUSPEND_WHILE_DEBUG_2 0
#define PM_MIN_CODE_DEBUG        0
#define PM_START_CODE_DEBUG      0
#define PM_XTAL_READY_DEBUG      0
#define PM_XTAL_ONCE_DEBUG       0
#define PM_XTAL_READY_TIME       0
#define PM_MANUAL_SETTLE_DEBUG   0

extern _attribute_data_retention_ unsigned int   g_tracking_32k_stimer_tick;
extern _attribute_data_retention_ unsigned int   g_current_stimer_tick;
extern _attribute_data_retention_ unsigned int   g_current_32k_tick;
extern _attribute_data_retention_ unsigned char  g_pm_suspend_power_cfg;
extern _attribute_data_retention_ unsigned char  g_pm_long_suspend;
extern _attribute_data_retention_ unsigned short g_pm_multi_addr;

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
 * @brief trim suspend LDO
 *
 */
typedef enum
{
    SPD_LDO_TRIM_0P80V = 0,
    SPD_LDO_TRIM_0P85V,
    SPD_LDO_TRIM_0P90V,
    SPD_LDO_TRIM_0P95V,
    SPD_LDO_TRIM_1P00V,
    SPD_LDO_TRIM_1P05V,
    SPD_LDO_TRIM_1P10V,
    SPD_LDO_TRIM_1P15V,
} pm_spd_ldo_trim_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum
{
    RET_LDO_TRIM_0P60V = 0,
    RET_LDO_TRIM_0P65V,
    RET_LDO_TRIM_0P70V,
    RET_LDO_TRIM_0P75V,
    RET_LDO_TRIM_0P80V,
    RET_LDO_TRIM_0P85V,
    RET_LDO_TRIM_0P90V,
    RET_LDO_TRIM_0P95V,
} pm_ret_ldo_trim_e;

extern volatile unsigned int __attribute__((section(".reboot_data"))) g_reboot_flag;
extern volatile unsigned char __attribute__((section(".reboot_data"))) g_reboot_ana_3a;
extern volatile unsigned char __attribute__((section(".reboot_data"))) g_reboot_ana_3b;
extern volatile unsigned char __attribute__((section(".reboot_data"))) g_reboot_ana_3c;

/**
 * @brief LDO 1.25V output trim definition
 * @note The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 * @note Step is 15mV(0-8)/23mV(8-15), 0x08 is the default value (1.25V).
 */
typedef enum {
    PM_LDO_1P25_VOLTAGE_1V127 = 0x00, /**< LDO 1.25V output 1.127V */
    PM_LDO_1P25_VOLTAGE_1V142 = 0x01, /**< LDO 1.25V output 1.142V */
    PM_LDO_1P25_VOLTAGE_1V157 = 0x02, /**< LDO 1.25V output 1.157V */
    PM_LDO_1P25_VOLTAGE_1V172 = 0x03, /**< LDO 1.25V output 1.172V */
    PM_LDO_1P25_VOLTAGE_1V187 = 0x04, /**< LDO 1.25V output 1.187V */
    PM_LDO_1P25_VOLTAGE_1V202 = 0x05, /**< LDO 1.25V output 1.202V */
    PM_LDO_1P25_VOLTAGE_1V217 = 0x06, /**< LDO 1.25V output 1.217V */
    PM_LDO_1P25_VOLTAGE_1V232 = 0x07, /**< LDO 1.25V output 1.232V */
    PM_LDO_1P25_VOLTAGE_1V250 = 0x08, /**< LDO 1.25V output 1.250V (default) */
    PM_LDO_1P25_VOLTAGE_1V273 = 0x09, /**< LDO 1.25V output 1.273V */
    PM_LDO_1P25_VOLTAGE_1V296 = 0x0A, /**< LDO 1.25V output 1.296V */
    PM_LDO_1P25_VOLTAGE_1V319 = 0x0B, /**< LDO 1.25V output 1.319V */
    PM_LDO_1P25_VOLTAGE_1V342 = 0x0C, /**< LDO 1.25V output 1.342V */
    PM_LDO_1P25_VOLTAGE_1V365 = 0x0D, /**< LDO 1.25V output 1.365V */
    PM_LDO_1P25_VOLTAGE_1V388 = 0x0E, /**< LDO 1.25V output 1.388V */
    PM_LDO_1P25_VOLTAGE_1V411 = 0x0F, /**< LDO 1.25V output 1.411V */
} pm_ldo_1p25_voltage_e;

/**
 * @brief LDO 1.8V output trim definition
 * @note The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 * @note Step is 20mV(0-8)/30mV(8-15), 0x08 is the default value (1.8V).
 */
typedef enum {
    PM_LDO_1P8_VOLTAGE_1V640 = 0x00, /**< LDO 1.8V output 1.640V */
    PM_LDO_1P8_VOLTAGE_1V660 = 0x01, /**< LDO 1.8V output 1.660V */
    PM_LDO_1P8_VOLTAGE_1V680 = 0x02, /**< LDO 1.8V output 1.680V */
    PM_LDO_1P8_VOLTAGE_1V700 = 0x03, /**< LDO 1.8V output 1.700V */
    PM_LDO_1P8_VOLTAGE_1V720 = 0x04, /**< LDO 1.8V output 1.720V */
    PM_LDO_1P8_VOLTAGE_1V740 = 0x05, /**< LDO 1.8V output 1.740V */
    PM_LDO_1P8_VOLTAGE_1V760 = 0x06, /**< LDO 1.8V output 1.760V */
    PM_LDO_1P8_VOLTAGE_1V780 = 0x07, /**< LDO 1.8V output 1.780V */
    PM_LDO_1P8_VOLTAGE_1V800 = 0x08, /**< LDO 1.8V output 1.800V (default) */
    PM_LDO_1P8_VOLTAGE_1V830 = 0x09, /**< LDO 1.8V output 1.830V */
    PM_LDO_1P8_VOLTAGE_1V860 = 0x0A, /**< LDO 1.8V output 1.860V */
    PM_LDO_1P8_VOLTAGE_1V890 = 0x0B, /**< LDO 1.8V output 1.890V */
    PM_LDO_1P8_VOLTAGE_1V920 = 0x0C, /**< LDO 1.8V output 1.920V */
    PM_LDO_1P8_VOLTAGE_1V950 = 0x0D, /**< LDO 1.8V output 1.950V */
    PM_LDO_1P8_VOLTAGE_1V980 = 0x0E, /**< LDO 1.8V output 1.980V */
    PM_LDO_1P8_VOLTAGE_2V010 = 0x0F, /**< LDO 1.8V output 2.010V */
} pm_ldo_1p8_voltage_e;

/**
 * @brief DCDC 1.25V output trim definition
 * @note The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 * @note Step is 25mV, 0x08 is the default value (1.251V).
 */
typedef enum {
    PM_DCDC_1P25_VOLTAGE_1V051 = 0x00, /**< DCDC 1.25V output 1.051V */
    PM_DCDC_1P25_VOLTAGE_1V076 = 0x01, /**< DCDC 1.25V output 1.076V */
    PM_DCDC_1P25_VOLTAGE_1V101 = 0x02, /**< DCDC 1.25V output 1.101V */
    PM_DCDC_1P25_VOLTAGE_1V126 = 0x03, /**< DCDC 1.25V output 1.126V */
    PM_DCDC_1P25_VOLTAGE_1V151 = 0x04, /**< DCDC 1.25V output 1.151V */
    PM_DCDC_1P25_VOLTAGE_1V176 = 0x05, /**< DCDC 1.25V output 1.176V */
    PM_DCDC_1P25_VOLTAGE_1V201 = 0x06, /**< DCDC 1.25V output 1.201V */
    PM_DCDC_1P25_VOLTAGE_1V226 = 0x07, /**< DCDC 1.25V output 1.226V */
    PM_DCDC_1P25_VOLTAGE_1V251 = 0x08, /**< DCDC 1.25V output 1.251V (default) */
    PM_DCDC_1P25_VOLTAGE_1V275 = 0x09, /**< DCDC 1.25V output 1.275V */
    PM_DCDC_1P25_VOLTAGE_1V300 = 0x0A, /**< DCDC 1.25V output 1.300V */
    PM_DCDC_1P25_VOLTAGE_1V325 = 0x0B, /**< DCDC 1.25V output 1.325V */
    PM_DCDC_1P25_VOLTAGE_1V350 = 0x0C, /**< DCDC 1.25V output 1.350V */
    PM_DCDC_1P25_VOLTAGE_1V375 = 0x0D, /**< DCDC 1.25V output 1.375V */
    PM_DCDC_1P25_VOLTAGE_1V400 = 0x0E, /**< DCDC 1.25V output 1.400V */
    PM_DCDC_1P25_VOLTAGE_1V425 = 0x0F, /**< DCDC 1.25V output 1.425V */
} pm_dcdc_1p25_voltage_e;

/**
 * @brief VDDDEC output trim definition
 * @note The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 * @note Step is 25mV, 0x06 is the normal working value (1.0V), 0x0A is the default value (1.0V).
 */
typedef enum {
    PM_VDDDEC_VOLTAGE_0V750 = 0x00, /**< VDDDEC output 0.750V */
    PM_VDDDEC_VOLTAGE_0V775 = 0x01, /**< VDDDEC output 0.775V */
    PM_VDDDEC_VOLTAGE_0V800 = 0x02, /**< VDDDEC output 0.800V */
    PM_VDDDEC_VOLTAGE_0V825 = 0x03, /**< VDDDEC output 0.825V */
    PM_VDDDEC_VOLTAGE_0V850 = 0x04, /**< VDDDEC output 0.850V */
    PM_VDDDEC_VOLTAGE_0V875 = 0x05, /**< VDDDEC output 0.875V */
    PM_VDDDEC_VOLTAGE_0V900 = 0x06, /**< VDDDEC output 0.900V */
    PM_VDDDEC_VOLTAGE_0V925 = 0x07, /**< VDDDEC output 0.925V */
    PM_VDDDEC_VOLTAGE_0V950 = 0x08, /**< VDDDEC output 0.950V */
    PM_VDDDEC_VOLTAGE_0V975 = 0x09, /**< VDDDEC output 0.975V */
    PM_VDDDEC_VOLTAGE_1V000 = 0x0A, /**< VDDDEC output 1.000V (default) */
    PM_VDDDEC_VOLTAGE_1V025 = 0x0B, /**< VDDDEC output 1.025V */
    PM_VDDDEC_VOLTAGE_1V050 = 0x0C, /**< VDDDEC output 1.050V */
    PM_VDDDEC_VOLTAGE_1V075 = 0x0D, /**< VDDDEC output 1.075V */
    PM_VDDDEC_VOLTAGE_1V100 = 0x0E, /**< VDDDEC output 1.100V */
    PM_VDDDEC_VOLTAGE_1V125 = 0x0F, /**< VDDDEC output 1.125V */
} pm_vdddec_voltage_e;

/**
 * @brief       This function serves to reboot system.
 * @return      none
 */
_always_inline void sys_reset_all(void)
{
#if (PM_DEBUG)
    while(1){}
#endif

	g_reboot_flag = 0x544c4e4b;
	g_reboot_ana_3a =analog_read(0x3a);
	g_reboot_ana_3b =analog_read(0x3b);
	g_reboot_ana_3c =analog_read(0x3c);
    reg_pwdn_en = 0x20;
}

/**
 * @brief       This function serves to trim suspend LDO voltage
 * @param[in]   spd_ldo_trim - suspend LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_spd_ldo_voltage(pm_spd_ldo_trim_e spd_ldo_trim)
{
    analog_write(areg_aon_0x0e, (analog_read(areg_aon_0x0e) & 0xf8) | spd_ldo_trim);
}

/**
 * @brief       This function serves to trim deep retention LDO voltage
 * @param[in]   ret_ldo_trim - deep retention LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_ret_ldo_voltage(pm_ret_ldo_trim_e ret_ldo_trim)
{
    analog_write(areg_aon_0x0f, (analog_read(areg_aon_0x0f) & 0xf8) | ret_ldo_trim);
}

/**
 * @brief   This function is used to enable native LDO.
 * @return  none.
 */
static _always_inline void pm_enable_native_ldo(void)
{
    analog_write(0x0b, (analog_read(0x0b) & ~(FLD_PD_NTV_1P25 | FLD_PD_NTV_1P8)));
}

/**
 * @brief   This function is used to disable native LDO.
 * @return  none.
 */
static _always_inline void pm_disable_native_ldo(void)
{
    analog_write(0x0b, (analog_read(0x0b) | (FLD_PD_NTV_1P25 | FLD_PD_NTV_1P8)));
}

/**
 * @brief   This function is used to enable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_enable_spd_ldo(void)
{
    analog_write(areg_aon_0x06, analog_read(areg_aon_0x06) & ~(FLD_PD_SPD_LDO));
}

/**
 * @brief   This function is used to disable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_disable_spd_ldo(void)
{
    analog_write(areg_aon_0x06, analog_read(areg_aon_0x06) | FLD_PD_SPD_LDO);
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
//    analog_write(0x3d, xtal_delay);  //the xtal delay is fixed and has a size of 1.
    analog_write(0x3e, r_delay);
}

/**
 * @brief       This function is used to set reboot reason.
 * @return      none.
 */
static _always_inline void pm_set_reboot_reason(pm_sw_reboot_reason_e reboot_reason)
{
    analog_write(PM_ANA_REG_POWER_ON_CLR_BUF0, REBOOT_FLAG | (reboot_reason << 1));
}

/**
 * @brief       This function is used to bypass ldo_1p8v enable.
 * @return      none.
 */
static _always_inline void pm_bypass_ldo_1p8_enable(void)
{
    analog_write(areg_aon_0x02, (analog_read(areg_aon_0x02) | FLD_EN_BYPASS_LDO_1P8V));
}

/**
 * @brief       This function is used to bypass ldo_lp8v disable.
 * @return      none.
 */
static _always_inline void pm_bypass_ldo_1p8_disable(void)
{
    analog_write(areg_aon_0x02, (analog_read(areg_aon_0x02) & (~FLD_EN_BYPASS_LDO_1P8V)));
}

/**
 * @brief       This function serves to get 32k set tick.
 * @return      32k set tick.
 */
static _always_inline unsigned int  pm_get_32k_set_tick(void)
{
    return (analog_read(0x68)<<24)|(analog_read(0x67)<<16)|(analog_read(0x66)<<8)|(analog_read(0x65));
}
/**
 * @brief       This function serves to get 32k current tick.
 * @return      32k set tick.
 */
static _always_inline unsigned int  pm_get_32k_cur_tick(void)
{
    return (analog_read(0x63)<<24)|(analog_read(0x62)<<16)|(analog_read(0x61)<<8)|(analog_read(0x60));
}

/**
 * @brief       This function is used to power up 24m rc.
 *              [DRIV-1966]The power consumption of 24m rc is 400uA in DCDC mode.
 * @return      none.
 */
static _always_inline void pm_24mrc_power_up(void)
{
    if (!g_24m_rc_is_used) {
    	analog_write(areg_aon_0x05, analog_read(areg_aon_0x05) & ~(BIT(2))); //power on 24M RC

        /*
         * the calibration of 24m RC should wait for 1us if just power it up.
         * (added by jilong.liu, confirmed by yangya at 20240805)
        */
    	sleep_us(2);
    }
}

/**
 * @brief       This function is used to power down 24m rc.
 *              [DRIV-1966]The power consumption of 24m rc is 400uA in DCDC mode.
 * @return      none.
 * @note        In the following case, please make sure the 24m rc can not be power down.
 *              1. Doing clock switch
 *              2. XTAL start up
 *              3. Doing digital module power switch
 *              4. Enter sleep.
 */
static _always_inline void pm_24mrc_power_down_if_unused(void)
{
    if (!g_24m_rc_is_used) {
    	analog_write(areg_aon_0x05, analog_read(areg_aon_0x05) | BIT(2)); //power down 24M RC
    }
}

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
 * @brief     This function serves to clear cache tag.
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void cache_tag_clr();

/**
 * @brief       this function serves to start sleep mode.
 * @param[in]   sleep_mode  - sleep mode type select.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_sleep_start(unsigned char sleep_mode);

/**
 * @brief      This function serves to set DCDC 1.25V.
 * @param[in]  voltage - dcdc 1.25V setting gear.
 * @return     none.
 */
static _always_inline void pm_set_dcdc_1p25(pm_dcdc_1p25_voltage_e voltage)
{
    analog_write(0x0c, (analog_read(0x0c) & 0x0f) | (voltage << 4));
}

/**
 * @brief      This function serves to set LDO 1.25V.
 * @param[in]  voltage - ldo 1.25V setting gear.
 * @return     none.
 */
static _always_inline void pm_set_ldo_1p25(pm_ldo_1p25_voltage_e voltage)
{
	analog_write(0x09, (analog_read(0x09) & 0xf0) | voltage);
}

/**
 * @brief      This function serves to set LDO 1.8V.
 * @param[in]  voltage - ldo 1.8V setting gear.
 * @return     none.
 */
static _always_inline void pm_set_ldo_1p8(pm_ldo_1p8_voltage_e voltage)
{
	analog_write(0x09, (analog_read(0x09) & 0x0f) | (voltage << 4));
}

/**
 * @brief      This function serves to set VDDDEC.
 * @param[in]  voltage - vdddec setting gear.
 * @return     none.
 */
static _always_inline void pm_set_vdddec(pm_vdddec_voltage_e voltage)
{
	analog_write(0x0f, (analog_read(0x0f) & 0x0f) | (voltage << 4));
}

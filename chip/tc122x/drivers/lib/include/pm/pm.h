/********************************************************************************************************
 * @file    pm.h
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

#include "gpio.h"
#include "lib/include/pm/pm_internal.h"
#include "driver_func_cfg.h"


/**
 * @brief these analog register can store data in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by watchdog, software reboot (sys_reboot()), RESET Pin, power cycle, 32k watchdog.
 */
#define PM_ANA_REG_WD_CLR_BUF1       0x36 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF2       0x37 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF3       0x38 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF4       0x39 // initial value 0x00.

/**
 * @brief analog register below can store information when MCU in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by power cycle, 32k watchdog, RESET Pin.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF1 0x3b // initial value 0x00.
#define PM_ANA_REG_POWER_ON_CLR_BUF2 0x3c // initial value 0xff.


//ana3b system used, user can not use
#define SYS_DEEP_ANA_REG             PM_ANA_REG_POWER_ON_CLR_BUF1
#define WAKEUP_STATUS_TIMER_CORE     ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD      ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

/**
 * @brief   gpio wakeup level definition
 */
typedef enum
{
    Level_Low  = 0,
    Level_High = 1,
}GPIO_LevelTypeDef;

/**
 * @brief   wakeup tick type definition
 */
typedef enum
{
    PM_TICK_STIMER     = 0, // 24M
    PM_TICK_32K        = 1,
}pm_wakeup_tick_type_e;

/**
 * @brief   sleep mode.
 */
typedef enum
{
    //available mode for customer
    SUSPEND_MODE                    = 0x00,
    DEEPSLEEP_MODE                  = 0x02, //when use deep mode pad wakeup(low or high level), if the high(low) level always in the pad,
                                            //system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20.
                                            //deep retention also had this issue, but not to reboot.
    DEEPSLEEP_MODE_RET_SRAM_LOW8K   = 0x03, //for boot from sram

    //not available mode
    DEEPSLEEP_RETENTION_FLAG        = 0x01,
}SleepMode_TypeDef;

/**
 * @brief   available wake-up source for customer
 */
typedef enum
{
    PM_WAKEUP_PAD           = FLD_WAKEUP_PAD_EN,
    PM_WAKEUP_TIMER         = FLD_WAKEUP_TIMER_EN,
    PM_WAKEUP_CORE          = FLD_WAKEUP_CORE_EN,
}SleepWakeupSrc_TypeDef;

/**
 * @brief   wake up status
 */
typedef enum
{
    WAKEUP_STATUS_PAD            = FLD_WAKEUP_STATUS_PAD,
    WAKEUP_STATUS_TIMER          = FLD_WAKEUP_STATUS_TIMER,
    WAKEUP_STATUS_CORE           = FLD_WAKEUP_STATUS_CORE,
    WAKEUP_STATUS_ALL            = FLD_WAKEUP_STATUS_ALL,
    WAKEUP_STATUS_INUSE_ALL      = FLD_WAKEUP_STATUS_INUSE_ALL,

    STATUS_GPIO_ERR_NO_ENTER_PM  = BIT(8), /**<Bit8 is used to determine whether the wake source is normal.*/
    STATUS_EXCEED_MAX            = BIT(27),
    STATUS_EXCEED_MIN            = BIT(28),
    STATUS_CLEAR_FAIL            = BIT(29),
    STATUS_ENTER_SUSPEND         = BIT(30),
}pm_suspend_wakeup_status_e;

/**
 * @brief   mcu status
 */
typedef enum
{
    MCU_POWER_ON                 = BIT(0), /**< power on, vbus detect or reset pin */
    //BIT(1) RSVD
    MCU_SW_REBOOT_BACK           = BIT(2), /**< Clear the watchdog status flag in time, otherwise, the system reboot may be wrongly judged as the watchdog.*/
    MCU_DEEPRET_BACK             = BIT(3),
    MCU_DEEP_BACK                = BIT(4),
    //BIT(5) RSVD
    MCU_HW_REBOOT_32K_WATCHDOG   = BIT(6), /**< - When the 32k watchdog status is set to 1, if it is not cleared:
                                              - power cyele/vbus detect/reset pin come back, the status is lost;
                                              - but software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the status remains;
                                              */
} pm_mcu_status;

/**
 * @brief power sel
 *
 */
typedef enum
{
    PM_POWER_UP         = 0,
    PM_POWER_DOWN       = 1,
}pm_power_sel_e;

/**
 * @brief   deep sleep wake up by external xtal
 */
typedef struct{
    unsigned char ext_cap_en;    //24xtal  cap
    unsigned char pad32k_en;
    unsigned char rsvd0;
    unsigned char rsvd;
}misc_para_t;
extern  _attribute_aligned_(4) misc_para_t              blt_miscParam;


typedef int (*suspend_handler_t)(void);
extern  suspend_handler_t        func_before_suspend;

/**
 * @brief   early wakeup time
 */
typedef struct
{
    unsigned short suspend_early_wakeup_time_us;  /**< suspend_early_wakeup_time_us = deep_ret_r_delay_us + xtal_stable_time + early_time*/
    unsigned short deep_ret_early_wakeup_time_us; /**< deep_ret_early_wakeup_time_us = deep_ret_r_delay_us + early_time*/
    unsigned short deep_early_wakeup_time_us;     /**< deep_early_wakeup_time_us = suspend_ret_r_delay_us*/
    unsigned short sleep_min_time_us;             /**< sleep_min_time_us = suspend_early_wakeup_time_us + 200*/
} pm_early_wakeup_time_us_s;

extern volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;

/**
 * @brief   hardware delay time
 */
typedef struct
{
    unsigned short deep_r_delay_cycle;           /**< hardware delay time ,deep_ret_r_delay_us = deep_r_delay_cycle * 1/16k */
    unsigned short suspend_ret_r_delay_cycle;    /**< hardware delay time ,suspend_ret_r_delay_us = suspend_ret_r_delay_cycle * 1/16k */
} pm_r_delay_cycle_s;

extern volatile pm_r_delay_cycle_s g_pm_r_delay_cycle;

/**
 * @brief   sleep wake up status
 */
typedef struct
{
    unsigned char is_deepretn_back;
    unsigned char is_pad_wakeup;
    unsigned char wakeup_src;
    unsigned char mcu_status;
}pm_para_t;
extern _attribute_aligned_(4) pm_para_t pmParam;

/**
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_deepRetentionWakeup(void)
{
    return pmParam.is_deepretn_back;
}

/**
 * @brief      This function serves to determine whether mcu is waked up by pad.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_deepPadWakeup(void)
{
    return pmParam.is_pad_wakeup;
}

/**
 * @brief       This function serves to get wake up source.
 * @return      wake up source.
 * @note        After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
 *              whether the wake source in use has been cleared, because some of the wake sources
 *              that are not in use may have been set up.
 */
static _always_inline pm_wakeup_status_e pm_get_wakeup_src(void)
{
    return ((pm_wakeup_status_e)(analog_read(areg_0x2c)&FLD_WAKEUP_STATUS_ALL));
}

/**
 * @brief       This function serves to clear the wake up bit.
 * @param[in]   status  - the interrupt status that needs to be cleared.
 * @return      none.
 * @note        To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
 *              instead of FLD_WAKEUP_STATUS_INUSE_ALL.
 */
static _always_inline void pm_clr_irq_status(pm_wakeup_status_e status)
{
    analog_write(areg_0x2c, status);
}

/**
 * @brief       This function serves to set the wakeup source.
 * @param[in]   wakeup_src  - wake up source select.
 * @return      none.
 */
static _always_inline void pm_set_wakeup_src(SleepWakeupSrc_TypeDef wakeup_src)
{
    analog_write(areg_0x25, (analog_read(areg_0x25) & 0xf8) | wakeup_src);
}

#if !FLASH_SDK_ENABLE
/**
 * @brief       This function configures a GPIO pin as the wake up pin.
 * @param[in]   pin - the pins can be set to all GPIO except PB0, PB1, PB3, PD4, PF0 and GPIOE groups.
 * @param[in]   pol - the wake up polarity of the pad pin(0: low-level wake up, 1: high-level wake up).
 * @param[in]   en  - enable or disable the wake up function for the pan pin(1: enable, 0: disable).
 * @return      none.
 */
void cpu_set_gpio_wakeup(GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en);
#endif

/**
 * @brief       This function configures pm wake up time parameter.
 * @param[in]   param - deep/suspend/deep_retention r_delay time.(default value: suspend/deep_ret=3, deep=11)
 * @return      none.
 * @note        Those parameters will be lost after reboot or deep sleep, so it required to be reconfigured.
 */
void pm_set_wakeup_time_param(pm_r_delay_cycle_s param);

/**
 * @brief       This function is used in applications where the crystal oscillator is relatively slow to start.
 *              When the start-up time is very slow, you can call this function to avoid restarting caused
 *              by insufficient crystal oscillator time (it is recommended to leave a certain margin when setting).
 * @param[in]   delay_us - This time setting is related to the parameter nopnum, which is about the execution time of the for loop
 *                          in the ramcode(default value: 200).
 * @param[in]   loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: 10).
 * @param[in]   nopnum - The number of for loops used to wait for the crystal oscillator to stabilize after suspend wakes up.
 *                       for(i = 0; i < nopnum; i++){ asm("tnop"); }(default value: Flash=250, OTP=Flash-60).
 * @return      none.
 * @note        Those parameters will be lost after reboot or deep sleep, so it required to be reconfigured.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum, unsigned int nopnum);

/**
 * @brief       This function will put the cpu into the stall state, and then wake up by the specified wakeup source.
 *              All interrupt sources can wake the CPU from stall mode.
 *              Depending on the configuration, the execution flow after waking up will be different:
 *              If the bit corresponding to the wake-up source in the register reg_irq_mask is enabled and the total interrupt is turned on,
 *              the CPU will be interrupted first after waking up from the stall state, and then continue to execute.
 *              If the bit corresponding to the wake-up source in the register reg_irq_mask is disabled,
 *              the CPU will continue to execute after waking up from the stall state.
 *              No matter which execution flow is taken after wake-up, the interrupt flag corresponding to the wake-up source needs to be clear after wake-up.
 * @param[in]   irq_mask - interrupt source for wake up.
 * @return      none.
 */
void cpu_stall_wakeup(irq_list_e irq_mask);

#if !FLASH_SDK_ENABLE
/**
 * @brief      This function serves to set the working mode of MCU based on 32k rc,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select,if only KEY_SCAN is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K watchdog will be turned off inside the function.
 * @param[in]  wakeup_tick_type - tick type select. Use 32K tick count for long-term sleep and 16M tick count for short-term sleep.
 * @param[in]  wakeup_tick - The tick value at the time of wake-up.
                             If the wakeup_tick_type is PM_TICK_STIMER, then wakeup_tick is converted to 16M. The range of tick that can be set is approximately:
                             current tick value + (18352~0xe0000000), and the corresponding sleep time is approximately: 2ms~234.88s.It cannot go to sleep normally when it exceeds this range.
                             If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K. The range of tick that can be set is approximately:
                             64~0xffffffff, and the corresponding sleep time is approximately: 2ms~37hours.It cannot go to sleep normally when it exceeds this range.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);

#define cpu_sleep_wakeup(sleep_mode, wakeup_src, wakeup_tick)  cpu_sleep_wakeup_32k_rc(sleep_mode, wakeup_src, PM_TICK_STIMER, wakeup_tick)

/**
 * @brief   This function serves to recover system timer from tick of internal 32k RC.
 *          The code is placed in the ram code section, in order to shorten the time.
 * @return  none.
 */
_attribute_ram_code_sec_noinline_ void pm_stimer_recover_32k_rc(void);


/**
 * @brief       This function serves to update wakeup status.
 * @param[in]   clr_en  - Whether to set the value of the status register to a fixed value.
 *                        If the interface is called twice, the first time it is not modified, clr_en=0;
 *                        if the interface is called once, it is modified, clr_en=1.
 * @return      none.
 * @note        After calling this interface, it is necessary to clear the flag of the timer watchdog or the 32k watchdog.
 *              Otherwise, if the flag remains set, it may affect the next judgment.
 *              After calling this interface, other states are set to fixed values.
 *              Therefore, this interface cannot be called twice,
 *              and if it is called twice, the state will be fixed to one state, not the correct state.
 */
_attribute_ram_code_sec_noinline_ void pm_update_status_info(unsigned char clr_en);

/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/
/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/
/**
 * @brief       When an error occurs, such as the crystal does not vibrate properly, the corresponding recording and reset operations are performed.
 * @param[in]   reboot_reason  - The bit to be configured in the power on buffer.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_sys_reboot_with_reason(pm_sw_reboot_reason_e reboot_reason);
#endif

/********************************************************************************************************
 * @file    stimer.h
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
#ifndef _STIMER_H_
#define _STIMER_H_

#include "compiler.h"
#include "register.h"

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

#define	    sys_tick_per_us   				16
#define		CLOCK_SYS_CLOCK_1250US			(1250 * sys_tick_per_us)

/**
 * @brief   system Timer : 16Mhz, Constant
 */
enum{
	CLOCK_16M_SYS_TIMER_CLK_1S  = sys_tick_per_us*1000*1000,
	CLOCK_16M_SYS_TIMER_CLK_1MS = sys_tick_per_us*1000,
	CLOCK_16M_SYS_TIMER_CLK_1US = sys_tick_per_us,
	CLOCK_SYS_TIMER_CLK_1S  = CLOCK_16M_SYS_TIMER_CLK_1S,
	CLOCK_SYS_TIMER_CLK_1MS = CLOCK_16M_SYS_TIMER_CLK_1MS,
	CLOCK_SYS_TIMER_CLK_1US = CLOCK_16M_SYS_TIMER_CLK_1US,
};


/**
 * @brief   This enumeration defines how many 32k ticks are reached, update the tick value of the increased system timer into the register.
 *          //32k_cnt = 2^(16-(cal_32k_mode>>4)), system_timer_cnt = 750*32k_cnt.(system timer is 24M).
 */
typedef enum
{
    STIMER_TRACK_32KCNT_2     = 0xf0, /* 2 32k ticks, corresponds 0x5dc system timer ticks. */
    STIMER_TRACK_32KCNT_4     = 0xe0, /* 4 32k ticks, corresponds 0xbb8 system timer ticks. */
    STIMER_TRACK_32KCNT_8     = 0xd0, /* 8 32k ticks, corresponds 0x1770 system timer ticks. */
    STIMER_TRACK_32KCNT_16    = 0xc0, /* 16 32k ticks, corresponds 0x2ee0 system timer ticks. */
    STIMER_TRACK_32KCNT_32    = 0xb0, /* 32 32k ticks, corresponds 0x5dc0 system timer ticks. */
    STIMER_TRACK_32KCNT_64    = 0xa0, /* 64 32k ticks, corresponds 0xbb80 system timer ticks. */
    STIMER_TRACK_32KCNT_128   = 0x90, /* 128 32k ticks, corresponds 0x17700 system timer ticks. */
    STIMER_TRACK_32KCNT_256   = 0x80, /* 256 32k ticks, corresponds 0x2ee00 system timer ticks. */
    STIMER_TRACK_32KCNT_512   = 0x70, /* 512 32k ticks, corresponds 0x5dc00 system timer ticks. */
    STIMER_TRACK_32KCNT_1024  = 0x60, /* 1024 32k ticks, corresponds 0xbb800 system timer ticks. */
    STIMER_TRACK_32KCNT_2048  = 0x50, /* 2048 32k ticks, corresponds 0x177000 system timer ticks. */
    STIMER_TRACK_32KCNT_4096  = 0x40, /* 4096 32k ticks, corresponds 0x2ee000 system timer ticks. */
    STIMER_TRACK_32KCNT_8192  = 0x30, /* 8192 32k ticks, corresponds 0x5dc000 system timer ticks. */
    STIMER_TRACK_32KCNT_16384 = 0x20, /* 16384 32k ticks, corresponds 0xbb8000 system timer ticks. */
    STIMER_TRACK_32KCNT_32768 = 0x10, /* 32768 32k ticks, corresponds 0x1770000 system timer ticks. */
    STIMER_TRACK_32KCNT_65536 = 0x00, /* 65536 32k ticks, corresponds 0x2ee0000 system timer ticks. */
} stimer_track_cnt_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern unsigned int g_track_32kcnt;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief       This function servers to set system timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static _always_inline void stimer_set_irq_mask(stimer_irq_mask_e mask)
{
    reg_system_irq_mask |= mask;
}

/**
 * @brief       This function servers to clear system timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static _always_inline void stimer_clr_irq_mask(stimer_irq_mask_e mask)
{
    reg_system_irq_mask &= (~mask);
}

/**
 * @brief       This function servers to clear system timer irq status.
 * @param[in]   status - the irq status.
 * @return      none.
 */
static _always_inline void stimer_clr_irq_status(stimer_irq_status_e status)
{
	reg_irq_src = (FLD_IRQ_SYSTEM_TIMER);
}

/**
 * @brief       This function servers to get system timer irq status.
 * @param[in]   status - the irq status.
 * @retval      non-zero -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static _always_inline unsigned char stimer_get_irq_status(stimer_irq_status_e status)
{
	return (reg_irq_src & FLD_IRQ_SYSTEM_TIMER) == FLD_IRQ_SYSTEM_TIMER;
}

/**
 * @brief       This function servers to set tick irq capture.
 * @param[in]   tick - the value of irq tick.
 * @return      none.
 */
static _always_inline void stimer_set_capture_tick(unsigned int tick)
{
	//0x744 bit[2:0] is always 0, to ensure system timer can be trigger correctly, bit[2:0] of the setting tick should
	//set to 0 to match the value of 0x744
	reg_system_irq_level = (tick & (~0x07));
}

/**
 * @brief       This function is used to set the tick value of the system timer.
 * @param[in]   tick - the value of system timer tick.
 * @return      none.
 */
static _always_inline void stimer_set_tick(unsigned int tick)
{
    reg_system_tick = (tick);
}

/**
 * @brief     This function performs to get system timer tick.
 * @param[in] none.
 * @return    system timer tick value.
 */
static _always_inline unsigned int clock_time(void)
{
    return reg_system_tick;
}

/**
 * @brief     This function serves to set timeout by us.
 * @param[in] ref  - reference tick of system timer .
 * @param[in] us   - count by us.
 * @return    true - timeout, false - not timeout
 */
static _always_inline unsigned int clock_time_exceed(unsigned int ref, unsigned int us)
{
    return ((unsigned int)(clock_time() - ref) > us * sys_tick_per_us);
}

/**
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void sleep_us(unsigned int microsec);

/**
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void sleep_ms(unsigned int millisec);

#define ClockTime			clock_time
#define WaitUs				sleep_us
#define WaitMs(t)			sleep_us((t)*1000)

/**********************************************************************************************************************
 *                          Internal interface, provided only for driver use.                                         *
 *********************************************************************************************************************/

/**
 * @brief   This function is used to enable system timer in manual mode.
 * @return  none.
 */
static _always_inline void stimer_enable_in_manual_mode(void)
{
    reg_system_ctrl |= FLD_SYSTEM_TIMER_MANUAL_EN;
}

/**
 * @brief   This function is used to enable 32k tracing.
 * @return  none.
 */
static _always_inline void stimer_32k_tracking_enable(void)
{
    reg_system_ctrl |= FLD_SYSTEM_32K_CAL_EN;
}

/**
 * @brief   This function is used to disable 32k tracing.
 * @return  none.
 */
static _always_inline void stimer_32k_tracking_disable(void)
{
    reg_system_ctrl &= ~(FLD_SYSTEM_32K_CAL_EN);
}

/**
 * @brief   This interface is used to configure the 32k track mode.
 *          (2^(16-(cnt>>4)))cycles of 32k clock indicates the tick number of the system timer.
 * @return  none.
 */
static _always_inline void stimer_set_32k_track_cnt(stimer_track_cnt_e cnt)
{
    reg_system_ctrl = (reg_system_ctrl & 0x0f) | cnt;
    g_track_32kcnt  = (1 << (16 - (cnt >> 4)));
}

/**
 * @brief   This interface is used to obtain 32k track count.
 * @return  32k track count.
 */
static _always_inline unsigned char stimer_get_32k_track_cnt(void)
{
    return g_track_32kcnt;
}

/**
 * @brief     This function performs to get the number of cycles of the RC 32k clock converted to system timer clock.
 * @return    none.
 */
static _always_inline unsigned int stimer_get_tracking_32k_value(void)
{
    return reg_system_32k_tick_cal;
}

/**
 * @brief     This interface is used to configure the write mode of the system timer to the 32k timer.
 * @return    none.
 */
static _always_inline void stimer_set_32k_write_mode(void)
{
    reg_system_ctrl |= FLD_SYSTEM_32K_WR_EN;
}

/**
 * @brief     This interface is used to configure the read mode of the system timer to the 32k timer.
 * @return    none.
 */
static _always_inline void stimer_set_32k_read_mode(void)
{
    reg_system_ctrl &= ~FLD_SYSTEM_32K_WR_EN;
}

/**
 * @brief     This interface is used to wait for the completion of reading 32k timers using system timers.
 * @return    none.
 */
static _always_inline void stimer_wait_read_32k_done(void)
{
    while(reg_system_status & FLD_SYSTEM_32K_TIMER_BUSY);
}

/**
 * @brief     This interface is used to configure the 32k tick value to be written.
 * @return    none.
 */
static _always_inline void stimer_set_32k_tick(unsigned int tick)
{
	reg_system_32k_tick_wt = tick;
}

/**
 * @brief     This interface is used to trigger the system timer to write the 32k timer.
 * @return    none.
 */
static _always_inline void stimer_set_32k_tick_write_trig(void)
{
	reg_system_status = FLD_SYSTEM_CMD_SYNC;
}

/**
 * @brief     This interface is used to wait for the completion of writing 32k timers using system timers.
 * @return    none.
 */
static _always_inline void stimer_wait_write_32k_done(void)
{
    while(reg_system_status & FLD_SYSTEM_RD_BUSY);
}

/**
 * @brief     This function is used to start the system timer.
 * @param[in] tick    - The initial value of the tick at startup.
 * @return    none.
 */
static _always_inline void stimer_enable(unsigned int tick)
{
	stimer_set_tick(tick);
	stimer_enable_in_manual_mode();
}

/**
 * @brief     This function is used to stop the system timer.
 * @return    none.
 */
static _always_inline void stimer_disable(void)
{
	reg_system_ctrl &= ~(FLD_SYSTEM_TIMER_MANUAL_EN);
}

#endif

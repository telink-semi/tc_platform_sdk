/********************************************************************************************************
 * @file    watchdog.h
 *
 * @brief   This is the header file for B87
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/**
 * @brief     This function set the feed dog capture value (capture_tick), the clock source is the system clock.
 * 			  When this capture value is reached, the chip will restart. The actual capture value is only high 14Bits will work,
 * 			  so there will be some deviation from the set value, the deviation can be calculated by the following principle,
 * 			  the actual capture value is: capture_tick = (period_ms*tick_per_ms) & 0xfffc0000.
 * @param[in] period_ms - feeding period time, the unit is ms
 * @param[in] tick_per_ms - tick value required for 1ms under system clock timing
 * @return    none
 */
extern void wd_set_interval_ms(unsigned int period_ms,unsigned long int tick_per_ms);

/**
 * @brief     start watchdog. ie enable watchdog
 * @param[in] none
 * @return    none
 */
static inline void wd_start(void){
	BM_SET(reg_tmr_ctrl8, FLD_TMR2_EN);
	BM_SET(reg_wd_ctrl1, FLD_WD_EN);
}
/**
 * @brief     stop watchdog. ie disable watchdog
 * @param[in] none
 * @return    none
 */
static inline void wd_stop(void){
	BM_CLR(reg_wd_ctrl1, FLD_WD_EN);
}

/**
 * @brief     clear watchdog.
 * @param[in] none
 * @return    none
 */
static inline void wd_clear(void)
{
	reg_tmr_sta = FLD_TMR_STA_WD;
}


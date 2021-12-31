/********************************************************************************************************
 * @file	watchdog.h
 *
 * @brief	This is the header file for b87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#pragma once


#include "register.h"

/**
 * @brief     This function set the feed dog capture value (capture_tick), the clock source is the system clock.
 * 			  When this capture value is reached, the chip will restart. The actual capture value is only high 14Bits will work,
 * 			  so there will be some deviation from the set value, the deviation can be calculated by the following principle,
 * 			  the actual capture value is: capture_tick = (period_ms*tick_per_ms) & 0xfff30000.
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
	BM_SET(reg_tmr_ctrl, FLD_TMR2_EN);
	BM_SET(reg_tmr_ctrl, FLD_TMR_WD_EN);
}
/**
 * @brief     stop watchdog. ie disable watchdog
 * @param[in] none
 * @return    none
 */
static inline void wd_stop(void){
	BM_CLR(reg_tmr_ctrl, FLD_TMR_WD_EN);
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


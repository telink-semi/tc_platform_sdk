/********************************************************************************************************
 * @file    watchdog.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver Group
 * @date    2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
void wd_set_interval_ms(unsigned int period_ms,unsigned long int tick_per_ms)
{
	static unsigned short tmp_period_ms = 0;
	tmp_period_ms = (period_ms*tick_per_ms>>18);
	reg_tmr2_tick = 0x00000000;    //reset tick register
	reg_tmr_ctrl=(((reg_tmr_ctrl&(~FLD_TMR_WD_CAPT))|((tmp_period_ms<<9)&FLD_TMR_WD_CAPT))&0x00ffffff);//set the capture register
}


/********************************************************************************************************
 * @file    sys_clock.h
 *
 * @brief   This is the header file for B85m
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
#ifndef __SYS_CLOCK_H_
#define __SYS_CLOCK_H_

#if (MCU_CORE_B89)
#if (CLOCK_SYS_CLOCK_HZ == 12000000)
#define SYS_CLK SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 16000000)
#define SYS_CLK SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
#define SYS_CLK SYS_CLK_24M_Crystal
#endif
#else
#if (CLOCK_SYS_CLOCK_HZ == 12000000)
#define SYS_CLK SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 16000000)
#define SYS_CLK SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
#define SYS_CLK SYS_CLK_24M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
#define SYS_CLK SYS_CLK_32M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
#define SYS_CLK SYS_CLK_48M_Crystal
#endif
#endif

enum
{
    CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
    CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
    CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};

#endif /* end of __SYS_CLOCK_H_. */

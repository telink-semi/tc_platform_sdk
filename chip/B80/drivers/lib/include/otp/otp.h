/********************************************************************************************************
 * @file    otp.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef _OTP_H_
#define _OTP_H_

#include "bsp.h"
#include "timer.h"
#include "clock.h"
#include "lib/include/otp/otp_base.h"

#define OTP_PCE_AUTO_MODE_EN				 1/**< 0: otp_pce_auto_mode is close,set to 0 only for internal debugging*/
											  /**< 1:otp_pce_auto_mode is open */

//delay times in spec
#define Tms			1        //ptm  -->  pce = 1,   Tms>1(ns)
#define Tmh			1        //ptm  -->  pce = 0,   Tmh>1(ns)
#define Tcsp		20       //pce = 1 ---> pprog=1,  10 < Tcsp < 100(us)
#define Tpps		10       //pprog=1 ---> pwe=1,  5 < Tpps < 20(us)
#define Tpw			15       //pwe high time,  10 < Tpw < 20(us)
#define Tpwi		4        //pwe high time,  1 < Tpwi < 5(us)
#define Tpph		10       //pwe=0 ---> pprog=0,  5 < Tpph < 20(us)
#define Tcs			15       //pwe=1 ---> enable,  Tcs > 10(us)
#define Tpls        15       //ldo setup time,Tpls > 10(us)
#define Tsas        5        //deep standby to active mode setup time, Tsas >2(us)

typedef enum{
    OTP_PTM_PROG = 0x02,
    OTP_PTM_READ = 0x00,
    OTP_PTM_INIT_MARGIN_READ = 0x01,
    OTP_PTM_PGM_MARGIN_READ = 0x04,
}OTP_PtmTypeDef;

#if (MCU_CORE_B80)
/**
 * @brief     This function servers to waiting for pce timeout.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_pce_timeout_exceed(void);
#elif (MCU_CORE_B80B)
/**
 * @brief      This function serves to disable global irq and wait otp done.
 * @return     none 
 */
_attribute_ram_code_sec_noinline_ void otp_dis_irq_and_wait_done(void);
#endif

/**
 * @brief      This function serves to enable the otp test area,only for internal testing,there are two operating units, each of which is 16 words,one start at address 0x00,
 *             the other the otp start at address 0x4000, test area address is independent of the real otp address,if use the otp test area,need to use this function.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_en(void);
/**
 * @brief      This function serves to disable the otp test area,only for internal testing,if do not use the otp test area,use this function,
 *             will transition from test area to operating on real the otp.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_dis(void);

/**
 * @brief      This function serves to read data from OTP memory.
 * @param[in]  ptm_mode - read mode.
 * @param[in]  addr     - the address of the data,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  word_len - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff     - data buff.
 * @return     none
 */
_attribute_ram_code_sec_ void otp_read_cycle(OTP_PtmTypeDef ptm_mode,unsigned int addr, unsigned int word_len, unsigned int *buff);

#if (MCU_CORE_B80)
/*
 * @brief     This function is a common sequence used by these interfaces:otp_write32/otp_read_cycle/otp_set_active_mode.
 * @param[in] ptm_mode - ptm type.
 * @return    none
 */
_attribute_ram_code_sec_ void otp_start(OTP_PtmTypeDef ptm_mode);
#elif (MCU_CORE_B80B)
/**
 * @brief      This function serves to set otp ptm read mode.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_ static inline void otp_ptm_read_mode(void)
{
    /* set ptm read */
    reg_otp_ctrl1 &= (~FLD_OTP_PTM);
    reg_otp_ctrl1 |= OTP_PTM_READ;
}
#endif

#if (MCU_CORE_B80)
/**
 * @brief      This function serves to preparations after otp software operation.
 * @param[in]  pce_flag - pce auto mode flag,from the return value of otp_auto_pce_disable function.
 * @return     none
 */
_attribute_ram_code_sec_ void otp_auto_pce_restore();
#elif (MCU_CORE_B80B)
/**
 * @brief      This function serves to set otp read mode and restore global irq.
 * @return     none 
 */
_attribute_ram_code_sec_ void otp_restore_irq_and_set_read_mode(void);
#endif

#include "lib/include/otp/otp_read.h"
#include "lib/include/otp/otp_write.h"
#include "lib/include/otp/otp_margin_read.h"
#include "lib/include/otp/otp_pce_timeout.h"


#endif /* _OTP_H_ */

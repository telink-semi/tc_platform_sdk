/********************************************************************************************************
 * @file	otp.h
 *
 * @brief	This is the header file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "driver_func_cfg.h"
#include "otp/otp_base.h"

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

/**
 * @brief     This function servers to enable pce auto mode,after enable pce auto mode,
 *            If the time from the last instruction fetching to the next instruction fetching exceeds the set timeout time,
 *            the pce will be pulled down. If the pce is pulled down, the hardware will automatically pull up when the instruction fetching,
 *            and clk will not work until the tsc time. This mechanism can save power consumption but reduce efficiency.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_auto_pce_enable();


#if OTP_ALL_FUNCTION_EN
/**
 * @brief    1. This function servers to set otp pce timeout time, the optimal time cannot be set according to the system clock,
 *              because the time between two otp fetch commands cannot be determined,otp fetch commands will only be sent when the cache miss occurs,
 *              but when the miss is not known,so this time is self-determining based on power consumption and efficiency.
 *           2. due to otp setup logic problems, tcs and timeout have a certain relationship: tcs>timeout,the minimum value of tcs is 10us,
 *              which has been set as the optimal under different system clocks,so the relationship between tcs and timeout is shown as follows:
 *               * | :-------------- | :--------------- | :------------------  |
 *                 |    SYS_CLK      |    tcs_config    |   timeout_config     |
 *                 |   4M_Crystal    |     0x01         |     = 0x00 (15.75us) |
 *                 |   6M_Crystal    |     0x01         |     = 0x00 (10.5us)  |
 *                 |   8M_Crystal    |     0x01         |     = 0x00 (7.88us)  |
 *                 |   12M_Crystal   |     0x02         |     < 0x02 (10.5us)  |
 *                 |   16M_Crystal   |     0x02         |     < 0x02 (7.88us)  |
 *                 |   24M_Crystal   |     0x04         |     < 0x04 (10.5us)  |
 * @param[in] timeout_config -range:0-3,timeout_time = (timeout_config(bit[2:3])+1)*0x3f(system clock cycle),the default value is 0;
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_timeout(unsigned char timeout_config);
#endif

/**
 * @brief      This function serves to otp set deep standby mode,if code run in flash,otp is idle,can enter deep to save current.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_deep_standby_mode(void);
/**
 * @brief      This function serves to otp set active mode,if otp is in deep mode,need to operate on the otp,set active mode.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_active_mode(void);

/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 *             voltage: otp write: 2.25V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 * @param[in]  addr - the address of the data,it has to be a multiple of 4,the OTP memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     0 :it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             1 :it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_write(unsigned int addr,unsigned int word_len,unsigned int *buff);

/**
 * @brief      This function serves to read data from OTP memory,belong to otp normal read.
 *             otp has three kinds of read mode,in general,just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,need to use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

#if OTP_ALL_FUNCTION_EN
/**
 * @brief      This function serves to read data from OTP memory,belong to otp pgm marginread.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,need to use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_pgm_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);
/**
 * @brief      This function serves to read data from OTP memory,belong to otp initial margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,need to use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_initial_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);
#endif



#endif /* _OTP_H_ */

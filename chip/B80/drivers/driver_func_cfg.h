/********************************************************************************************************
 * @file	driver_func_cfg.h
 *
 * @brief	This is the header file for b80
 *
 * @author	Driver Group
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef DRIVER_FUNCTION_CONFIG_H_
#define DRIVER_FUNCTION_CONFIG_H_

//When pengcheng.xu tested the SRAM program, he found that the suspend sleep current was too large when the program was downloaded immediately after being active;
//after debugging, it was found that the MCU was stalled because the chip boot did not finish running, and the otp was not turned off, so the sleep current was too large.
//If it is a ram program SRAM_OTP_FLASH_HANDLE needs to be set to 1, then the driver processing is:
//in active mode, the power of the OTP flash is present to ensure that the otp flash function can be used normally,
//but the current at this time will be higher (Compared to the normal OTP or flash program), the specific data is as follows:
//In sleep mode, the otp will enter deep, and the flash will be powered off to ensure that the sleep current is normal
/////////////////////////////////////////////////////////////////////////////////////////////
// 		  				otp active mode	    otp deep standby mode
// open flash ldo 			6.80mA					5.35mA
// close flash ldo			4.44mA					2.99mA
#ifndef SRAM_OTP_FLASH_HANDLE
#define SRAM_OTP_FLASH_HANDLE				0
#endif

/**********************************************************************************************************************
 *   If you need to optimize the code size, you can modify the following configuration according to the situation.
 *   Note:
 *	 -Before modifying, please confirm whether the function is needed, in case the needed function is deleted by mistake.
 *	 -If you modify the following configuration options, please regenerate the library file
 *********************************************************************************************************************/
#ifndef ONLY_SUPPORT_OTP_PROGRAM
#define ONLY_SUPPORT_OTP_PROGRAM			0/**< 0:support otp program and flash program */
											 /**< 1:only support otp program */
#endif

#ifndef PM_32K_EXTERNAL_XTAL_EN
#define PM_32K_EXTERNAL_XTAL_EN				1/**< 0:only support 32K internal Crystal */
											 /**< 1:support 32K internal and external Crystal */
#endif

#ifndef PM_RETENTION_FUNCTION_EN
#define PM_RETENTION_FUNCTION_EN			1/**< 0:only Support sleep modes:suspend/deep */
											 /**< 1:support all sleep modes:suspend/deep/deep retention */
#endif

#ifndef OTP_ALL_FUNCTION_EN
#define OTP_ALL_FUNCTION_EN					1/**< 0:not support otp_set_auto_pce_timeout/otp_pgm_margin_read/otp_initial_margin_read */
											 /**< 1:support all functions of OTP */
#endif



#endif /* DRIVER_FUNCTION_CONFIG_H_ */

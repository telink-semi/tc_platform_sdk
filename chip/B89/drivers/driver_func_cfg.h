/********************************************************************************************************
 * @file	driver_func_cfg.h
 *
 * @brief	This is the header file for B89
 *
 * @author	Driver Group
 * @date	2021
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
#ifndef DRIVER_FUNCTION_CONFIG_H_
#define DRIVER_FUNCTION_CONFIG_H_


//TODO:has not yet been tested.
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
//#ifndef SRAM_OTP_FLASH_HANDLE
//#define SRAM_OTP_FLASH_HANDLE				0
//#endif

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


#endif /* DRIVER_FUNCTION_CONFIG_H_ */

/********************************************************************************************************
 * @file    driver_func_cfg.h
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
#ifndef DRIVER_FUNCTION_CONFIG_H_
#define DRIVER_FUNCTION_CONFIG_H_
/**
 * @brief	The program is not burned into OTP/Flash, but is directly downloaded into sram through the tool for execution (that is, the scene that the jig and BDT tool will use).
 * 			The reasons are as follows:
 * 			When using the tool download the program to the sram, the tool will first send the instruction to stall the mcu, and then load the program to the sram for execution.
 * 			The stall operation is very fast, so it may stop in the boot program of the chip.
 *
 * 			This may will cause the following effects:
 * 			 1. OTP is not turned off (will cause the current to increase);
 * 			 2. The flag bit identification of the otp program or the flash program is incorrect, resulting in abnormal flash function, so added the processing of SRAM_OTP_FLASH_HANDLE;
 * 			In order to ensure that the following functions of the jig program are normal, sleep current test and OTP/flash programming, the processing method here is as follows:
 * 			 1. Ensure that both flash and OTP are in a working state during initialization.
 * 			 2. Flash and OTP will be turned off before sleep, and turned on after suspend wake up, this will introduce another problem, that is, the working current will be larger,
 * 			 but the jig application does not care about the working current, so we ignore this effect.
 * 			 When the program is running, the specific current data when OTP and FLASH are turned off and turned on are as follows:
 *                      otp active mode            otp deep standby mode
 *  open flash ldo          6.80mA                          5.35mA
 *  close flash ldo         4.44mA                          2.99mA
 * @note
 *        - This macro takes effect in the C source file(Defined in TC32 Compiler not in TC32 Assembler).
 *        - Since the SRAM program is only for debugging purposes, the low power consumption function may not be realized.
 *        - If the SDK project uses this macro instead of the source project, in addition to the above problems, \n
 *          the current in the low-power part will be high (since the sdk uses this macro just to do debugging and won't use the low-power, the program doesn't deal with this scenario)
 * 
 */
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


#endif /* DRIVER_FUNCTION_CONFIG_H_ */

/********************************************************************************************************
 * @file    cstartup_funcs.h
 *
 * @brief   This is the header file for TC321X
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
#ifndef _BOOTLOADER_
#define _BOOTLOADER_


/**
 * @brief		This function serves to copy ram bin from flash to sram.
 * @return		none.
 */
__attribute__((section(".cstartup_ram_funcs"))) void copy_bincode();

/**
 * @brief		This function serves to copy ram code from flash to sram.
 * @return		none.
 */
__attribute__((section(".cstartup_ram_funcs"))) void copy_ramcode();

#endif /* _BOOTLOADER_ */

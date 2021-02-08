/********************************************************************************************************
 * @file	flash_mid13325e.h
 *
 * @brief	This is the header file for b85m
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef __MID13325E_H__
#define __MID13325E_H__

/*
 * @brief     MID = 0x13325e Flash include ZB25WD40B.
 */

#include "compiler.h"


/**
 * @brief     define the section of the protected memory area which is read-only and unalterable.
 */
typedef enum{
	FLASH_LOCK_NONE_MID13325E		=	0x00,
	FLASH_LOCK_LOW_504K_MID13325E	=	0x04,	//000000h-07DFFFh
	FLASH_LOCK_LOW_496K_MID13325E	=	0x08,	//000000h-07BFFFh
	FLASH_LOCK_LOW_480K_MID13325E	=	0x0c,	//000000h-077FFFh
	FLASH_LOCK_LOW_448K_MID13325E	=	0x10,	//000000h-06FFFFh
	FLASH_LOCK_LOW_384K_MID13325E	=	0x14,	//000000h-05FFFFh
	FLASH_LOCK_LOW_256K_MID13325E	=	0x18,	//000000h-03FFFFh
	FLASH_LOCK_ALL_512K_MID13325E	=	0x1c,	//000000h-07FFFFh
}mid13325e_lock_block_e;

/**
 * @brief     the range of bits to be modified when writing status.
 */
typedef enum{
	FLASH_WRITE_STATUS_BP_MID13325E	=	0x1c,
}mid13325e_write_status_bit_e;


/**
 * @brief 		This function reads the status of flash.
 * @return 		the value of status.
 */
_attribute_ram_code_ unsigned char flash_read_status_mid13325e(void);

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	data	- the value of status.
 * @param[in]  	bit		- the range of bits to be modified when writing status.
 * @return 		none.
 */
_attribute_ram_code_ void flash_write_status_mid13325e(unsigned char data, mid13325e_write_status_bit_e bit);

/**
 * @brief 		This function serves to set the protection area of the flash.
 * @param[in]   data	- refer to the protection area definition in the .h file.
 * @return 		none.
 */
_attribute_ram_code_ void flash_lock_mid13325e(mid13325e_lock_block_e data);

/**
 * @brief 		This function serves to flash release protection.
 * @return 		none.
 */
_attribute_ram_code_ void flash_unlock_mid13325e(void);


#endif


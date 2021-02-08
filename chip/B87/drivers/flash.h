/********************************************************************************************************
 * @file	flash.h
 *
 * @brief	This is the header file for b87
 *
 * @author	Driver Group
 * @date	2019
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
#pragma once

#include "compiler.h"

/**
 * @brief     flash command definition
 */
enum{
	FLASH_WRITE_STATUS_CMD		=	0x01,
	FLASH_WRITE_CMD				=	0x02,
	FLASH_READ_CMD				=	0x03,
	FLASH_WRITE_DISABLE_CMD 	= 	0x04,
	FLASH_READ_STATUS_CMD		=	0x05,
	FLASH_WRITE_ENABLE_CMD 		= 	0x06,
	FLASH_SECT_ERASE_CMD		=	0x20,
	FLASH_GD_PUYA_READ_UID_CMD	=	0x4B,	//Flash Type = GD/PUYA
	FLASH_32KBLK_ERASE_CMD		=	0x52,
	FLASH_XTX_READ_UID_CMD		=	0x5A,	//Flash Type = XTX
	FLASH_CHIP_ERASE_CMD		=	0x60,   //or 0xc7
	FLASH_PAGE_ERASE_CMD		=	0x81,   //caution: only P25Q40L support this function
	FLASH_64KBLK_ERASE_CMD		=	0xD8,
	FLASH_POWER_DOWN			=	0xB9,
	FLASH_POWER_DOWN_RELEASE	=	0xAB,
	FLASH_GET_JEDEC_ID			=	0x9F,

};

/**
 * @brief     flash type definition
 */
 
typedef enum{
	FLASH_TYPE_GD = 0 ,
	FLASH_TYPE_XTX,
	FLASH_TYPE_PUYA
}Flash_TypeDef;

/**
 * @brief     flash capacity definition
 * Call flash_read_mid function to get the size of flash capacity.
 * Example is as follows:
 * unsigned char temp_buf[4];
 * flash_read_mid(temp_buf);
 * The value of temp_buf[2] reflects flash capacity.
 */
typedef enum {
    FLASH_SIZE_64K     = 0x10,
    FLASH_SIZE_128K    = 0x11,
    FLASH_SIZE_256K    = 0x12,
    FLASH_SIZE_512K    = 0x13,
    FLASH_SIZE_1M      = 0x14,
    FLASH_SIZE_2M      = 0x15,
    FLASH_SIZE_4M      = 0x16,
    FLASH_SIZE_8M      = 0x17,
} Flash_CapacityDef;

/**
 * @brief This function serves to erase a sector.
 * @param[in]   addr the start address of the sector needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_sector(unsigned long addr);

/**
 * @brief This function writes the buffer's content to the flash.
 * @param[in]   addr - the start address of the area.
 * @param[in]   len  - the length(in byte) of content needs to write into the flash.
 * @param[in]   buf  - the start address of the content needs to write into.
 * @return      none
 * @note        the funciton support cross-page writing,which means the len of buf can bigger than 256.
 */
_attribute_ram_code_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief This function reads the content from a page to the buf.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to read out from the page
 * @param[out]  buf the start address of the buffer
 * @return none
 */
_attribute_ram_code_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf);
/**
 * @brief This function write the status of flash.
 * @param[in]  the value of status
 * @return status
 */
void flash_read_mid(unsigned char *buf);
/**
 * @brief This function write the status of flash.
 * @param[in]  the value of status
 * @return status
 */
unsigned char flash_write_status(unsigned char data);
/* according to your appliaction */
#if 0
/**
 * @brief     This function serves to erase a page(256 bytes).
 * @param[in] addr - the start address of the page needs to erase.
 * @return    none
 * @note      only 8359 support
 */
void flash_erase_page(unsigned int addr);

/**
 * @brief This function serves to erase a block(32k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return none
 */
void flash_erase_32kblock(unsigned int addr);

/**
 * @brief This function serves to erase a block(64k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return none
 */
void flash_erase_64kblock(unsigned int addr);

/**
 * @brief     This function serves to erase a page(256 bytes).
 * @param[in] addr - the start address of the page needs to erase.
 * @return    none
 */
void flash_erase_chip(void);



/**
 * @brief This function reads the status of flash.
 * @param[in]  none
 * @return none
 */
unsigned char flash_read_status(void);

/**
 * @brief  	Deep Power Down mode to put the device in the lowest consumption mode
 * 			it can be used as an extra software protection mechanism,while the device
 * 			is not in active use,since in the mode,  all write,Program and Erase commands
 * 			are ignored,except the Release from Deep Power-Down and Read Device ID(RDI)
 * 			command.This release the device from this mode
 * @param[in] none
 * @return none.
 */
void flash_deep_powerdown(void);

/**
 * @brief		The Release from Power-Down or High Performance Mode/Device ID command is a
 * 				Multi-purpose command.it can be used to release the device from the power-Down
 * 				State or High Performance Mode or obtain the devices electronic identification
 * 				(ID)number.Release from Power-Down will take the time duration of tRES1 before
 * 				the device will resume normal operation and other command are accepted.The CS#
 * 				pin must remain high during the tRES1(8us) time duration.
 * @param[in]   none
 * @return      none.
 */
void flash_release_deep_powerdown(void);


/**
 * @brief	  This function serves to read UID of flash
 * @param[in] idcmd - different flash vendor have different read-uid command
 *                    GD/PUYA:0x4B; XTX: 0x5A
 * @param[in] buf   - store UID of flash
 * @return    none.
 */
void flash_read_uid(unsigned char idcmd,unsigned char *buf);
/**
 * @brief 		 This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]   flash_mid - Flash Manufacturer ID
 * @param[out]   flash_uid - Flash Unique ID
 * @return       0:error 1:ok

 */
int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid);
/**
 * @brief This function serves to protect data for flash.
 * @param[in]   type - flash type include GD,Puya and XTX
 * @param[in]   data - refer to Driver API Doc.
 * @return none
 */
void flash_lock(Flash_TypeDef type , unsigned short data);

/**
 * @brief This function serves to protect data for flash.
 * @param[in]   type - flash type include GD,Puya and XTX
 * @return none
 */
void flash_unlock(Flash_TypeDef type);
#endif

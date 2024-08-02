/********************************************************************************************************
 * @file    flash_mid114485.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef __MID114485_H__
#define __MID114485_H__

/*
 * @brief     MID = 0x114485 Flash include P25D09U.
 */


/**
 * @brief   define the section of the protected memory area which is read-only and unalterable.
 * @note    when data protection, only enumeration items in mid114485_lock_block_e can be selected,
 *          to ensure that the values returned by the interface flash_get_lock_block_mid114485 are in the mid114485_lock_block_e.
 */
typedef enum{
	FLASH_LOCK_NONE_MID114485		=	0x00,
	FLASH_LOCK_UP_64K_MID114485		=	0x04,	//010000h-01FFFFh
	FLASH_LOCK_LOW_64K_MID114485	=	0x24,	//000000h-00FFFFh
	FLASH_LOCK_UP_4K_MID114485		=	0x44,	//01F000h-01FFFFh
	FLASH_LOCK_UP_8K_MID114485		=	0x48,	//01E000h-01FFFFh
	FLASH_LOCK_UP_16K_MID114485		=	0x4c,	//01C000h-01FFFFh
	FLASH_LOCK_UP_32K_MID114485		=	0x50,	//018000h-01FFFFh
	FLASH_LOCK_LOW_4K_MID114485		=	0x64,	//000000h-000FFFh
	FLASH_LOCK_LOW_8K_MID114485		=	0x68,	//000000h-001FFFh
	FLASH_LOCK_LOW_16K_MID114485	=	0x6c,	//000000h-003FFFh
	FLASH_LOCK_LOW_32K_MID114485	=	0x70,	//000000h-007FFFh
	FLASH_LOCK_ALL_128K_MID114485	=	0x7c,	//000000h-01FFFFh
}mid114485_lock_block_e;

/**
 * @brief     the range of bits to be modified when writing status.
 */
typedef enum{
	FLASH_WRITE_STATUS_BP_MID114485	=	0x7c,
}mid114485_write_status_bit_e;


/**
 * @brief 		This function reads the status of flash.
 * @return 		the value of status.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned char flash_read_status_mid114485(void);

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	data	- the value of status.
 * @param[in]  	mask    - mid114485_write_status_bit_e
 * @return 		1: success, 0: error, 2: parameter error.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned char flash_write_status_mid114485(unsigned char data, unsigned char mask);

/**
 * @brief 		This function serves to set the protection area of the flash.
 * @param[in]   data	- refer to the mid114485_lock_block_e protection area definition in the .h file.
 * @return 		1: success, 0: error, 2: parameter error.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned char flash_lock_mid114485(unsigned int data);

/**
 * @brief 		This function serves to flash release protection.
 * @return 		1: success, 0: error, 2: parameter error.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned char flash_unlock_mid114485(void);

/**
 * @brief 		This function serves to get the protection area of the flash.
 * @return 		mid114485_lock_block_e
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned int flash_get_lock_block_mid114485(void);
#endif


/********************************************************************************************************
 * @file    flash_mid1360b3.h
 *
 * @brief   This is the header file for TC123X
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef __MID1360B3_H__
#define __MID1360B3_H__

/*
 * @brief     MID = 0x1360b3 Flash include UC25WD40IB (UCUN).
 */


/**
 * @brief   define the section of the protected memory area which is read-only and unalterable.
 * @note    when data protection, only enumeration items in mid1360b3_lock_block_e can be selected,
 *          to ensure that the values returned by the interface flash_get_lock_block_mid1360b3 are in the mid1360b3_lock_block_e.
 */
typedef enum{
	FLASH_LOCK_NONE_MID1360B3		=	0x0000,
	FLASH_LOCK_LOW_504K_MID1360B3	=	0x0004,	//000000h-07DFFFh
	FLASH_LOCK_LOW_496K_MID1360B3	=	0x0008,	//000000h-07BFFFH
	FLASH_LOCK_LOW_480K_MID1360B3	=	0x000c,	//000000h-077FFFH
	FLASH_LOCK_LOW_448K_MID1360B3	=	0x0010,	//000000h-06FFFFH
	FLASH_LOCK_LOW_384K_MID1360B3	=	0x0014,	//000000h-05FFFFH
	FLASH_LOCK_LOW_256K_MID1360B3	=	0x0018,	//000000h-03FFFFH
	FLASH_LOCK_ALL_512K_MID1360B3	=	0x001c,	//000000h-07FFFFH
}mid1360b3_lock_block_e;

/**
 * @brief     The starting address of the Security Registers.
 */
typedef enum{
	FLASH_OTP_0x001000_256B_MID1360B3	=	0x001000,	//001000h-0010FFh
	FLASH_OTP_0x002000_256B_MID1360B3	=	0x002000,	//002000h-0020FFh
}mid1360b3_otp_block_e;

/**
 * @brief     the lock area of the Security Registers.
 * @note      LB1 (S11)=1 locks Security Register 1, LB2 (S12)=1 locks Security Register 2.
 */
typedef enum{
	FLASH_LOCK_OTP_0x001000_256B_MID1360B3	=	0x0800,	//001000h-0010FFh (LB1 in Status-2)
	FLASH_LOCK_OTP_0x002000_256B_MID1360B3	=	0x1000,	//002000h-0020FFh (LB2 in Status-2)
}mid1360b3_lock_otp_e;

/**
 * @brief     the range of bits to be modified when writing status.
 */
typedef enum{
	FLASH_WRITE_STATUS_BP_MID1360B3		=	0x001c,	// BP2:BP0
	FLASH_WRITE_STATUS_OTP_MID1360B3	=	0x1800,	// LB2:LB1
}mid1360b3_write_status_bit_e;


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
unsigned short flash_read_status_mid1360b3(void);

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	data	- the value of status.
 * @param[in]  	mask    - mid1360b3_write_status_bit_e
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
unsigned char flash_write_status_mid1360b3(unsigned short data, unsigned int mask);

/**
 * @brief 		This function serves to set the protection area of the flash.
 * @param[in]   data	- refer to the mid1360b3_lock_block_e protection area definition in the .h file.
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
unsigned char flash_lock_mid1360b3(unsigned int data);

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
unsigned char flash_unlock_mid1360b3(void);

/**
 * @brief 		This function serves to get the protection area of the flash.
 * @return 		mid1360b3_lock_block_e
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
unsigned int flash_get_lock_block_mid1360b3(void);

/**
 * @brief 		This function serves to read data from the Security Registers of the flash.
 * @param[in]   addr	- the start address of the Security Registers.
 *						the address of the  Security Registers #1 0x001000-0x0010ff
 *						the address of the  Security Registers #2 0x002000-0x0020ff
 * @param[in]   len		- the length of the content to be read.
 * @param[out]  buf		- the starting address of the content to be read.
 * @return 		none.
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
void flash_read_otp_mid1360b3(unsigned long addr, unsigned long len, unsigned char* buf);

/**
 * @brief 		This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   addr	- the start address of the Security Registers.
 *						the address of the  Security Registers #1 0x001000-0x0010ff
 *						the address of the  Security Registers #2 0x002000-0x0020ff
 * @param[in]   len		- the length of content to be written.
 * @param[in]   buf		- the starting address of the content to be written.
 * @return 		none.
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
void flash_write_otp_mid1360b3(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief 		This function serves to erase the data of the Security Registers that you choose.
 * 				You can erase 256-byte one time.
 * @param[in]   addr	- the address that you want to erase.
 *						the address of the  Security Registers #1 0x001000-0x0010ff
 *						the address of the  Security Registers #2 0x002000-0x0020ff
 * @return 		none.
 * @note		Even you choose the middle area of the Security Registers,it will erase the whole area.
 *
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
void flash_erase_otp_mid1360b3(mid1360b3_otp_block_e addr);

/**
 * @brief 		This function serves to provide the write protect control to the Security Registers.
 * @param[in]   data	- the lock area of the Security Registers.
 * @return 		1: success, 0: error, 2: parameter error.
 * @note		once they are set to 1, the Security Registers will become read-only permanently,
 * 				you can't write or erase this area.
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned char flash_lock_otp_mid1360b3(mid1360b3_lock_otp_e data);
#endif

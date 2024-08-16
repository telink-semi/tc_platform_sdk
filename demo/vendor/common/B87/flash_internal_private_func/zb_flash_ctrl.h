/********************************************************************************************************
 * @file    zb_flash_ctrl.h
 *
 * @brief   This is the header file for Telink MCU
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
#ifndef _ZB_CAM_MODIFY_H_
#define _ZB_CAM_MODIFY_H_

#include "driver.h"
#include "spi_i.h"

typedef enum{
	ZB_FLASH_CAM6_0x00 	=	0x00,   // zb flash reset voltage set to 1.0V, in this case,flash current will increase about 1uA.
	ZB_FLASH_CAM6_0x10 	=	0x10,	// zb flash reset voltage set to 0.5V.
}zb_flash_cam6_e;

/**
 * @brief		This function serves to modify zb flash reset voltage(cam[6]).cam[6] default value is 0x10,when flash voltage is less than 0.5V,
 * 				the flash will reset.but in some application solution,the voltage is not decrease less than 0.5 and increase soon,in this case the
 * 				flash will power on fail,need to change the reset voltage to 1.0V(change the cam[6] to 0x00),but the flash standby current will increase
 * 				about 1uA.
 * @param[in]	cam_6  - the cam[6] value which you want to modify.
 * @return		0 - it is zb flash and modify success;
 * 				1 - it is zb flash and modify fail;
 * 				2 - it is not zb flash or it is zb flash but can not use this api to modify cam value;
 * 				0xff - flash is error, the cam value is change to 0x00.
 * @note		The irq will disable to avoid interrupt in the process of this api.
 * 				This process need keep the chip power on, if power down in this process will cause flash error
 * 				and can not use it in low voltage status.
 * 				The best usage is:call this api to change cam value after power on and system init,if change success,write
 * 				a flag to flash to ensure the api only call once.
 *@attention	The return value equal 0 just represent that this configuration is modify success,you need to power down the flash power and power on again
 *				to make it take effect.
 */
_attribute_ram_code_sec_noinline_ unsigned char zb_cam_modify(zb_flash_cam6_e cam_6);

/**
 * @brief		This function serves to check the zb flash cam value.
 * @param[in]	cam6_check_en - whether to check the cam6, 1 - enable, 0 - disable.
 * @param[in]	cam_6 - the cam6 value, if you enable cam6_check, this value will compare with the cam6 that read from flash.
 * @return		0 - it is zb flash and cam value is the setting value;
 * 				1 - it is zb flash and cam value is error;
 * 				2 - it is not zb flash or it is zb flash but can not use this api to modify cam value;
 */
_attribute_ram_code_sec_noinline_ unsigned char zb_cam_check(unsigned char cam6_check_en, zb_flash_cam6_e cam_6);

#endif /* _ZB_CAM_MODIFY_H_ */

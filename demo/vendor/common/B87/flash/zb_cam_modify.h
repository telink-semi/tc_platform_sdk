/********************************************************************************************************
 * @file	zb_cam_modify.h
 *
 * @brief	This is the header file for b85m
 *
 * @author	Driver Group
 * @date	2018
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

#endif /* _ZB_CAM_MODIFY_H_ */

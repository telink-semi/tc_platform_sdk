/********************************************************************************************************
 * @file    calibration.c
 *
 * @brief   This is the source file for TC1211
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
#include "calibration.h"
#include "driver.h"
#include "types.h"

/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in efuse,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * 				(reduce frequency offset.)
 * @return      1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_read_efuse_value_calib(void)
{
	unsigned char frequency_offset_value = 0;
	efuse_read(12, &frequency_offset_value, 1); //bit96->bit101, The default value for efuse is 0. 
	/*In the efuse, 6 bits store the frequency_offset value, defaulting to 0. 
	  If 0 is considered valid, the software cannot distinguish between an unwritten value and a calibration value of 0, making it unable to decide whether to use it. 
	  After discussion with Wenfeng, it was decided to treat 0 as an invalid value in the current scenario. 
	  If the calibration value is 0, the chip is flagged as defective (though this is highly unlikely). 
	  If the calibration value in efuse is unwritten, the program defaults to using 0x100000 without calibration.
	  This note also applies to the fixture! 
	  changed by kun.he, confirmed by wenfeng.lou; 20250211.
	*/
	if(0x0 != (0x3f&frequency_offset_value))
	{
		rf_update_internal_cap(frequency_offset_value);
		return 1;
	}
	return 0;
}
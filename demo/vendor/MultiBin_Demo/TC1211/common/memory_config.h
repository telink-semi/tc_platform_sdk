/********************************************************************************************************
 * @file    memory_config.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/*
 * memory_config.h
 *
 *  Created on: 2024年9月5日
 *      Author: Admin
 */

#ifndef VENDOR_COMMON_MEMORY_CONFIG_H_
#define VENDOR_COMMON_MEMORY_CONFIG_H_


#define  DRIVER_FUNC_LIST_ADDR_OFFSET               0x28
#define  DRIVER_RETENTION_DATA_LMA_START            0x2c
#define  DRIVER_RETENTION_DATA_VMA_START            0x30
#define  DRIVER_RETENTION_DATA_VMA_END              0x34
#define  DRIVER_DATA_LMA_START_ADDR_OFFSET          0x38
#define  DRIVER_DATA_VMA_START_ADDR_OFFSET          0x3c
#define  DRIVER_DATA_VMA_END_ADDR_OFFSET            0x40
#define  DRIVER_BSS_VMA_START_ADDR_OFFSET           0x44
#define  DRIVER_BSS_VMA_END_ADDR_OFFSET             0x48
#define  DRIVER_SRAM_USAGE_SIZE_ADDR_OFFSET         0x4c


//The sdk adjusts the size according to the actual situation
#define  DRIVER_ROM_START_ADDR           0x14000
#define  DRIVER_SRAM_START_ADDR          0x841000
#define  DRIVER_RAM_MAX_SIZE             0x800
#define  DRIVER_ROM_MAX_SIZE             0x2000



#endif /* VENDOR_COMMON_MEMORY_CONFIG_H_ */

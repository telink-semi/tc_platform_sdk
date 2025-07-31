/********************************************************************************************************
 * @file    efuse.h
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
#ifndef EFUSE_H_
#define EFUSE_H_

#define SC_BASE  0x40
/*******************************       efuse registers: 0x40     ******************************/
#define BIT7   BIT(7)
#define reg_efuse_decrypt_status		REG_ADDR8(SC_BASE + 0x07)
#define reg_efuse_addr		     		REG_ADDR8(SC_BASE + 0x08)
#define reg_efuse_wrdata     			REG_ADDR32(SC_BASE + 0x0C)
#define reg_efuse_ctlr     			    REG_ADDR8(SC_BASE + 0x09)
#define reg_rst_en                      REG_ADDR32(SC_BASE + 0x20)

enum{
    EFUSE_WRITE_EN               = BIT(0),
    EFUSE_WRITE_TRIG             = BIT(1),
    EFUSE_BUSY                   = BIT(7),
}efuse_ctrl;

enum{
    EFUSE_KEY_CYC_VLD               = BIT(0),
    EFUSE_RAMCRC_ERROR             = BIT(1),
    EFUSE_CLCK_EN                = BIT(2),
}efuse_status;

typedef enum{
    EFUSE_BLOCK0                = 0,
    EFUSE_BLOCK1                = 1,
    EFUSE_BLOCK2                = 2,
    EFUSE_BLOCK3                = 3,
}efuse_block;

/**
 * @brief       This function servers to read efuse.
 * @param[in]   block    - the read address must align word (4bytes): 0,1,2,3...
 * @return      data     - efuse data .
 */
unsigned int efuse_read(efuse_block block);
/**
 * @brief       This function servers to write efuse.
 * @param[in]   block    - the read address must align word (4bytes): 0,1,2,3...
 * @param[out]  data    - the pointer to the data for read. It's best to align 4 bytes.
 * @return      none
 */
void efuse_write(efuse_block block, unsigned int data);

#endif /* EFUSE_H_ */

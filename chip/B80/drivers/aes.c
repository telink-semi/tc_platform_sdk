/********************************************************************************************************
 * @file    aes.c
 *
 * @brief   This is the source file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "aes.h"
#include "bsp.h"
#include "clock.h"
#include "register.h"
#include "timer.h"
/**
 * @brief       This function servers to perform aes_128 encryption for 16-Byte input data
 *              with specific 16-Byte key
 * @param[in]   Key the pointer to the 16-Byte Key
 * @param[in]   Data the pointer to the 16-Byte plain text
 * @param[out]  Result the pointer to the encryption result cipher text
 * @return      0: success;
 *              not 0: failure
 */
int aes_encrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result)
{
    unsigned int tmp = 0;
    unsigned char *p = Data;
    unsigned char i = 0;

    //trig encrypt operation
    reg_aes_ctrl &= (~FLD_AES_CTRL_CODEC_TRIG);

    //set the key
    for (i = 0; i < 16; i++) {
    	reg_aes_key(i) = Key[i];
    }

    //feed the data
    while (reg_aes_ctrl & FLD_AES_CTRL_DATA_FEED) {
        tmp = p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
        reg_aes_data = tmp;
        p += 4;
    }

    //wait for aes ready
    while ((reg_aes_ctrl & FLD_AES_CTRL_CODEC_FINISHED) == 0);

    //read out the result
    p = Result;
    for (i = 0; i < 4; i++) {
        tmp = reg_aes_data;
        *p++ = tmp & 0xff;
        *p++ = (tmp>>8) & 0xff;
        *p++ = (tmp>>16) & 0xff;
        *p++ = (tmp>>24) & 0xff;
    }
    return 0;
}


/**
 * @brief       This function servers to perform aes_128 decryption for 16-Byte input data
 *              with specific 16-Byte key
 * @param[in]   Key the pointer to the 16-Byte Key
 * @param[in]   Data the pointer to the 16-Byte cipher text
 * @param[out]  Result the pointer to the decryption result plain text
 * @return      0: success;
 *              not 0: failure
 */
int aes_decrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result)
{
    unsigned int tmp = 0;
    unsigned char *p = Data;
    unsigned char i = 0;

    //trig decrypt operation
    reg_aes_ctrl |= FLD_AES_CTRL_CODEC_TRIG;

    //set the key
    for (i = 0; i < 16; i++) {
    	reg_aes_key(i) = Key[i];
    }

    //feed the data
    while (reg_aes_ctrl & FLD_AES_CTRL_DATA_FEED) {
        tmp = p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24);
        reg_aes_data = tmp;
        p += 4;
    }

    //wait for aes ready
    while ((reg_aes_ctrl & FLD_AES_CTRL_CODEC_FINISHED) == 0);

    //read out the result
    p = Result;
    for (i = 0; i < 4; i++) {
        tmp = reg_aes_data;
        *p++ = tmp & 0xff;
        *p++ = (tmp>>8) & 0xff;
        *p++ = (tmp>>16) & 0xff;
        *p++ = (tmp>>24) & 0xff;
    }
    return 0;
}

/**
 * @brief       This function servers to perform aes_128 encryption
 *              with specific 16-Byte key in DMA MODE
 * @param[in]   Key :the pointer to the 16-Byte Key
 * @param[in]   Data :the pointer to the 16-Byte plain text
 * @param[in]   DataSize: the length of Data
 * @param[out]  ResultSize: the length of Result
 * @param[out]  Result: the pointer to the encryption cipher text
 * @return      0: success;
 *              not 0: failure
 */
int  aes_dma_encrypt(unsigned char *Key,unsigned int *Data,unsigned short DataSize, unsigned int *Result,unsigned short ResultSize)
{
	write_reg8(0xc10,((unsigned int) Result)&0xff);//set memory address low byte
	write_reg8(0xc11, (((unsigned int)Result)>>8)&0xff);//set memory address high byte
	reg_dma4_addrHi=0x04;
	reg_dma4_size =  ResultSize/16;//set memory size
	reg_dma4_mode = 0x01;

	write_reg8(0xc14,((unsigned int)Data)&0xff);//set memory address low byte
	write_reg8(0xc15, (((unsigned int)Data)>>8)&0xff);//set memory address high byte
	reg_dma5_addrHi=0x04;
	reg_dma5_size =DataSize/ 16;//set memory size
	reg_dma5_mode = 0x00;

	reg_aes_ctrl &= (~FLD_AES_CTRL_CODEC_TRIG);//trig encrypt operation

	for (unsigned char i = 0; i < 16; i++) {
		reg_aes_key(i) = Key[i];  //set the key
	}

	reg_dma_chn_en|=0x30;//enable aes dma channel
	reg_dma_tx_rdy0|=(FLD_DMA_CHN_AES_IN|FLD_DMA_CHN_AES_OUT);

	while ( reg_dma_tx_rdy0 & FLD_DMA_CHN_AES_OUT);
	return 0;
}

/**
 * @brief       This function servers to perform aes_128  decryption
 *              with specific 16-Byte key in DMA MODE
 * @param[in]   Key :the pointer to the 16-Byte Key
 * @param[in]   Data :the pointer to the 16-Byte cipher text
 * @param[in]   DataSize: the length of Data
 * @param[out]  ResultSize: the length of Result
 * @param[out]  Result: the pointer to the decryption result plain text
 * @return      0: success;
 *              not 0: failure
 */
int aes_dma_decrypt(unsigned char *Key,unsigned int *Data,unsigned short DataSize, unsigned int *Result,unsigned short ResultSize)
{
	write_reg8(0xc10,((unsigned int) Result)&0xff);//set memory address low byte
	write_reg8(0xc11, (((unsigned int)Result)>>8)&0xff);//set memory address high byte
	reg_dma4_addrHi=0x04;
	reg_dma4_size =  ResultSize/16;//set memory size
	reg_dma4_mode = 0x01;

	write_reg8(0xc14,((unsigned int)Data)&0xff);//set memory address low byte
	write_reg8(0xc15, (((unsigned int)Data)>>8)&0xff);//set memory address high byte
	reg_dma5_addrHi=0x04;
	reg_dma5_size =DataSize/ 16;//set memory size
	reg_dma5_mode = 0x00;

	reg_aes_ctrl |=FLD_AES_CTRL_CODEC_TRIG;

	for (unsigned char i = 0; i < 16; i++) {
		reg_aes_key(i) = Key[i];  //set the key
	}

	reg_dma_chn_en|=0x30;//enable aes dma channel
	reg_dma_tx_rdy0|=(FLD_DMA_CHN_AES_IN|FLD_DMA_CHN_AES_OUT);

	while ( reg_dma_tx_rdy0 & FLD_DMA_CHN_AES_OUT);
	return 0;
}


/********************************************************************************************************
 * @file	aes.c
 *
 * @brief	This is the source file for b85
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

	while(reg_dma_tx_rdy0 & FLD_DMA_CHN_AES_OUT);  //wait dma done
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

	while(reg_dma_tx_rdy0 & FLD_DMA_CHN_AES_OUT);  //wait dma done
	return 0;
}


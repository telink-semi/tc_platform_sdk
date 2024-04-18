/********************************************************************************************************
 * @file    aes.h
 *
 * @brief   This is the header file for B87
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef AES_H_
#define AES_H_
#include "compiler.h"
/**
 * @brief       This function servers to perform aes_128 encryption for 16-Byte input data
 *              with specific 16-Byte key
 * @param[in]   Key the pointer to the 16-Byte Key
 * @param[in]   Data the pointer to the 16-Byte plain text
 * @param[out]  Result the pointer to the encryption result cipher text
 * @return      0: success;
 *              not 0: failure
 */
extern int aes_encrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result);

#define  AES_Encrypt  aes_encrypt

/**
 * @brief       This function servers to perform aes_128 decryption for 16-Byte input data
 *              with specific 16-Byte key
 * @param[in]   Key the pointer to the 16-Byte Key
 * @param[in]   Data the pointer to the 16-Byte cipher text
 * @param[out]  Result the pointer to the decryption result plain text
 * @return      0: success;
 *              not 0: failure
 */
extern int aes_decrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result);

#define  AES_Decrypt  aes_decrypt

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
extern int  aes_dma_encrypt(unsigned char *Key,unsigned int *Data,unsigned short DataSize, unsigned int *Result,unsigned short ResultSize);
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
extern int  aes_dma_decrypt(unsigned char *Key,unsigned int *Data,unsigned short DataSize, unsigned int *Result,unsigned short ResultSize);


#endif /* AES_H_ */

/********************************************************************************************************
 * @file    random.c
 *
 * @brief   This is the source file for TC321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#include "random.h"

/**
 * @brief     This function performs to get one unsigned char random number
 * @param[in] none.
 * @return    the value of one random number.
 */
unsigned char rand_uchar(void)
{
	unsigned char data = reg_trng_data;
	return data;
}

/**
 * @brief     This function performs to get one unsigned short random number
 * @param[in] none.
 * @return    the value of one random number.
 */
unsigned short rand_ushort(void)
{
	unsigned short data = reg_trng_data;
	data = (data<<8) | reg_trng_data;
	return data;
}

/**
 * @brief     This function performs to get one unsigned int random number
 * @param[in] none.
 * @return    the value of one random number.
 */
unsigned int rand(void)
{
	unsigned int data = reg_trng_data;
	data = (data<<8) | reg_trng_data;
	data = (data<<8) | reg_trng_data;
	data = (data<<8) | reg_trng_data;
	return data;
}

/**
 * @brief     This function performs to get a serial of random number.
 * @param[in]  len- the length of random number
 * @param[in]  data - the first address of buffer store random number in
 * @return    the result of a serial of random number.
 */
void generateRandomNum(int len, unsigned char *data)
{
    for (int i = 0; i < len; i++)
    {
        data[i] = reg_trng_data;
    }
}

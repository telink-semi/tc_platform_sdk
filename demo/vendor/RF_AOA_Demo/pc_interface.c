/********************************************************************************************************
 * @file    pc_interface.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "driver.h"
#include "pc_interface.h"
#define    IO_BASE_ADDR    0x840000

unsigned char __attribute__ ((aligned (4))) commandBuff[commandBuffCnt][commandBuffSize];
unsigned char __attribute__ ((aligned (4))) resultBuff[resultBuffCnt][resultBuffSize];
/**
 * @brief      	This function is used to initiate the buffer of command
 * @param[in]  	BufAddr 	- set the address of buffer of command from the upper monitor
 * @param[in]  	BufSize		- set the size of address of buffer of command from the upper monitor
 * @param[in]	BufCnt		- set layers of fifo
 * @return     	none
 */
void ParaBuf_Init(unsigned short BufAddr, unsigned char BufSize, unsigned char BufCnt)
{
	WRITE_REG16(PARA_BUF_ADDR, BufAddr);
	WRITE_REG8(PARA_BUF_SIZE, BufSize);
	WRITE_REG8(PARA_BUF_CNT, BufCnt);
	WRITE_REG8(PARA_BUF_WPTR, 0);
	WRITE_REG8(PARA_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to get the command from the upper monitor
 * @param[in]  	none
 * @return     	none
 */
unsigned int is_ParaBuf_HaveData(void)
{
    unsigned char wptr = READ_REG8(PARA_BUF_WPTR);
    unsigned char rptr = READ_REG8(PARA_BUF_RPTR);
    if (wptr == rptr) {  //if buf is empty, return 0
        return 0;
    }
    else {
    	return 1;
    }
}
/**
 * @brief      	This function is used to read command from the upper monitor
 * @param[in]  	*pDestBuf	-save the command to the specified location waiting to be executed
 * @param[in]  	len			-set the length of command waiting to read
 * @return     	none
 */
void ParaBuf_Read(unsigned char *pDestBuf, unsigned char len)
{
    unsigned char rptr = READ_REG8(PARA_BUF_RPTR);
    unsigned short BufAddr = READ_REG16(PARA_BUF_ADDR);
    unsigned char BufSize = READ_REG8(PARA_BUF_SIZE);
    unsigned char BufCnt = READ_REG8(PARA_BUF_CNT);
    unsigned long ReadAddr = IO_BASE_ADDR + BufAddr + BufSize*rptr;

    int i = 0;
    for (i=0; i<len; i++) {
        pDestBuf[i] = READ_REG8(ReadAddr+i);
    }

    //adjust rptr
    WRITE_REG8(PARA_BUF_RPTR, (rptr+1)%BufCnt);
}

/**
 * @brief      	This function is used to initiate the result buffer
 * @param[in]  	BufAddr		-save the result to the specified location waiting to be read by the upper monitor
 * @param[in]  	BufSize		-set the length of result buffer
 * @param[in]  	BufCnt		-set the layers of fifo to save result data waiting to be read by the upper monitor
 * @return     	none
 */
void ResuBuf_Init(unsigned short BufAddr, unsigned char BufSize, unsigned char BufCnt)
{
	WRITE_REG16(RESU_BUF_ADDR, BufAddr);
    WRITE_REG8(RESU_BUF_SIZE, BufSize);
    WRITE_REG8(RESU_BUF_CNT, BufCnt);
    WRITE_REG8(RESU_BUF_WPTR, 0);
    WRITE_REG8(RESU_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to clear the result buffer
 * @param[in]  	none
 * @return     	none
 */
void Result_Buff_Clear(void)
{
    WRITE_REG8(RESU_BUF_WPTR, 0);
    WRITE_REG8(RESU_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to write result waiting to be read by the upper monitor
 * @param[in]  	*pSrcBuf	-save the result to the specified location waiting to be read
 * @param[in]  	len			-set the length of result waiting to read
 * @return     	none
 */
unsigned int ResuBuf_Write(unsigned char *pSrcBuf, unsigned char len)
{
    unsigned char wptr = READ_REG8(RESU_BUF_WPTR); 	//0
    unsigned char rptr = READ_REG8(RESU_BUF_RPTR);	//0
    unsigned char BufCnt = READ_REG8(RESU_BUF_CNT); //3

    if (((wptr+1)%BufCnt) == rptr) { //if ResuBuf is full, do nothing and return 0
        return 0;
    }
    else {
        unsigned short BufAddr = READ_REG16(RESU_BUF_ADDR);
        unsigned char BufSize = READ_REG8(RESU_BUF_SIZE);
        unsigned long WriteAddr = IO_BASE_ADDR + BufAddr + BufSize*wptr;

        int i = 0;
	    for (i=0; i<len; i++) {
	    	WRITE_REG8(WriteAddr+i, pSrcBuf[i]);
	    }

        //adjust wptr
	    WRITE_REG8(RESU_BUF_WPTR, (wptr+1)%BufCnt);

        return i;
    }
}

unsigned int Get_Next_Packet(void)
{
    unsigned char wptr = READ_REG8(RESU_BUF_WPTR); 	//0
    unsigned char rptr = READ_REG8(RESU_BUF_RPTR);	//0
    unsigned char BufCnt = READ_REG8(RESU_BUF_CNT); //8

    if (((wptr+1)%BufCnt) == rptr)
    { //if ResuBuf is full, do nothing and return 0
        return 0;
    }

    else
    {
        //adjust wptr
        WRITE_REG8(RESU_BUF_WPTR, (wptr+1)%BufCnt);

        unsigned short BufAddr = READ_REG16(RESU_BUF_ADDR);
        unsigned char BufSize = READ_REG8(RESU_BUF_SIZE);
        unsigned int WriteAddr = BufAddr + BufSize*wptr;

	    return WriteAddr;
    }
}



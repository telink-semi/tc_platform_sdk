/********************************************************************************************************
 * @file	pc_interface.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
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



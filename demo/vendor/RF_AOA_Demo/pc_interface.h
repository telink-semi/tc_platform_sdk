/********************************************************************************************************
 * @file    pc_interface.h
 *
 * @brief   This is the header file for Telink MCU
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
#ifndef PC_INTERFACE_H
#define PC_INTERFACE_H

/****************************************************************************************************************************
 * Address allocation:
 * 		0x840004    0x840005    0x840006    0x840007    0x840008    0x840009
 * 		ParaAddr_L  ParaAddr_H  fifo_size   fifo_cnt    wptr        rptr
 * 		0x84000a    0x84000b    0x84000c    0x84000d    0x84000e    0x84000f
 * 		ResAddr_L   ResAddr_H   fifo_size   fifo_cnt    wptr        rptr
 * Note:
 * -1.this block of address is very safe,and it would not be interrupted because of unreasonable definition
 * by programmer or automatic allocation mechanism in processing of program. It is why we use this part of address!
 *
 * -2.a block of continuous address is created in ram,which is divided into two parts :one part is used to receive command
 * from the upper monitor,another part is used to send result waited to read by the upper monitor
 *
 * -3.In order to advance the speed of the whole system,we consider to use 5 layers of fifo	to upload or download data,
 * every layer of fifo has 16 bytes.
 *
****************************************************************************************************************************/
#define    PARA_BUF_ADDR    0x840004
#define    PARA_BUF_SIZE    0x840006
#define    PARA_BUF_CNT     0x840007
#define    PARA_BUF_WPTR    0x840008
#define    PARA_BUF_RPTR    0x840009

#define    RESU_BUF_ADDR    0x84000a
#define    RESU_BUF_SIZE    0x84000c
#define    RESU_BUF_CNT     0x84000d
#define    RESU_BUF_WPTR    0x84000e
#define    RESU_BUF_RPTR    0x84000f

#define    commandBuffSize   32
#define    commandBuffCnt    4
#define    resultBuffSize    255
#define    resultBuffCnt     8
enum{
	NO_PROJECT 			= 0x00,
	ADC_PROJECT 		= 0x01,
	AUDIO_PROJECT 		= 0x02,
	AES_PROJECT 		= 0x03,
	GPIO_PROJECT 		= 0x04,
	FLASH_PROJECT 		= 0x05,
	PWM_PROJECT 		= 0x06,
	TIMER_PROJECT 		= 0x07,
	PM_PROJECT 			= 0x08,
	UART_PROJECT 		= 0x09,
	I2C_PROJECT 		= 10,
	SPI_PROJECT 		= 11,
	RF_PROJECT 			= 12,
//	PM_PROJECT 			= 13,
	USB_PROJECT			= 14,
	s7816_PROJECT		= 15,
};
extern unsigned char commandBuff[commandBuffCnt][commandBuffSize];
extern unsigned char resultBuff[resultBuffCnt][resultBuffSize];
extern unsigned char AllModuleResultBuff[resultBuffSize];
extern unsigned char ModuleResultBuff[resultBuffSize];
/**
 * @brief      	This function is used to initiate the buffer of command
 * @param[in]  	BufAddr 	- set the address of buffer of command from the upper monitor
 * @param[in]  	BufSize		- set the size of address of buffer of command from the upper monitor
 * @param[in]	BufCnt		- set layers of fifo
 * @return     	none
 */
extern void ParaBuf_Init(unsigned short BufAddr, unsigned char BufSize, unsigned char BufCnt);
/**
 * @brief      	This function is used to get the command from the upper monitor
 * @param[in]  	none
 * @return     	none
 */
extern unsigned int is_ParaBuf_HaveData(void);
/**
 * @brief      	This function is used to read command from the upper monitor
 * @param[in]  	*pDestBuf	-save the command to the specified location waiting to be executed
 * @param[in]  	len			-set the length of command waiting to read
 * @return     	none
 */
extern void ParaBuf_Read(unsigned char *pDestBuf, unsigned char len);
/**
 * @brief      	This function is used to initiate the result buffer
 * @param[in]  	BufAddr		-save the result to the specified location waiting to be read by the upper monitor
 * @param[in]  	BufSize		-set the length of result buffer
 * @param[in]  	BufCnt		-set the layers of fifo to save result data waiting to be read by the upper monitor
 * @return     	none
 */
extern void ResuBuf_Init(unsigned short BufAddr, unsigned char BufSize, unsigned char BufCnt);
/**
 * @brief      	This function is used to clear the result buffer
 * @param[in]  	none
 * @return     	none
 */
extern void Result_Buff_Clear(void);
/**
 * @brief      	This function is used to write result waiting to be read by the upper monitor
 * @param[in]  	*pSrcBuf	-save the result to the specified location waiting to be read
 * @param[in]  	len			-set the length of result waiting to read
 * @return     	none
 */
extern unsigned int ResuBuf_Write(unsigned char *pSrcBuf, unsigned char len);

extern unsigned int Get_Next_Packet(void);

#endif

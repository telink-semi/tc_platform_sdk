/********************************************************************************************************
 * @file	pc_interface.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "pc_interface.h"

volatile unsigned char par_addr[20]__attribute__((aligned(4)));

/**
 * @brief      	This function is used to initiate the buffer of command.
 * @param[in]  	buf_addr - set the address of buffer of command from the upper monitor.
 * @param[in]  	buf_size - set the size of address of buffer of command from the upper monitor.
 * @param[in]	buf_cnt  - set layers of fifo.
 * @return     	none
 */
void para_buff_init(unsigned int buf_addr, unsigned char buf_size, unsigned char buf_cnt)
{
	write_data_word(PARA_BUF_CHECK,(unsigned int)PARA_BUF_CHECK);
	write_data_word(PARA_BUF_ADDR, buf_addr);
	write_data_byte(PARA_BUF_SIZE, buf_size);
	write_data_byte(PARA_BUF_CNT, buf_cnt);
	write_data_byte(PARA_BUF_WPTR, 0);
	write_data_byte(PARA_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to get the command from the upper monitor
 * @param[in]  	none
 * @return     	none
 */
unsigned int para_buff_have_data(void)
{
    unsigned char wptr = read_data_byte(PARA_BUF_WPTR);
    unsigned char rptr = read_data_byte(PARA_BUF_RPTR);
    if (wptr == rptr) { // if buffer is empty, return 0
        return 0;
    }
    else {
    	return 1;
    }
}
/**
 * @brief      	This function is used to read command from the upper monitor.
 * @param[in]  	*dest_buf - save the command to the specified location waiting to be executed.
 * @param[in]  	len       - set the length of command waiting to read.
 * @return     	none
 */
int para_buff_read(unsigned char *dest_buf, unsigned char len)
{
    unsigned char rptr = read_data_byte(PARA_BUF_RPTR);
    unsigned int  buf_addr = read_data_word(PARA_BUF_ADDR);
    unsigned char buf_size = read_data_byte(PARA_BUF_SIZE);
    unsigned char buf_cnt = read_data_byte(PARA_BUF_CNT);
    unsigned int  read_addr = buf_addr + buf_size*rptr;
    unsigned char i = 0;
    unsigned short crc;

    if(len > buf_size) len = buf_size;
    for (i = 0; i < len; i++) {
    	dest_buf[i] = read_data_byte(read_addr+i);
     }
    autotest_package_t_ptr para = (autotest_package_t_ptr)dest_buf;
    write_data_byte(PARA_BUF_RPTR, (rptr+1)%buf_cnt);
    if((para->payload_length+4) <= COMMAND_BUFF_SIZE){
    	crc = crc_16(dest_buf,para->payload_length);
    	if((para->crc[0] == (crc&0xff) && (para->crc[1] == ((crc>>8)&0xff)))) {
    		return 1;//CRC correct
    	}
    	else{
    		return -1;//CRC error
    	}
    }
    return 0;
}
/**
 * @brief      	This function is used to initiate the result buffer.
 * @param[in]  	buf_addr - save the result to the specified location waiting to be read by the upper monitor.
 * @param[in]  	buf_size - set the length of result buffer.
 * @param[in]  	buf_cnt  - set the layers of fifo to save result data waiting to be read by the upper monitor.
 * @return     	none
 */
void result_buff_init(unsigned int buf_addr, unsigned char buf_size, unsigned char buf_cnt)
{
	write_data_word(PARA_BASE_ADDR,(unsigned int)par_addr);
	write_data_word(RESU_BUF_ADDR, buf_addr);
    write_data_byte(RESU_BUF_SIZE, buf_size);
    write_data_byte(RESU_BUF_CNT, buf_cnt);
    write_data_byte(RESU_BUF_WPTR, 0);
    write_data_byte(RESU_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to clear the result buffer
 * @param[in]  	none
 * @return     	none
 */
void result_buff_clear(void)
{
    write_data_byte(RESU_BUF_WPTR, 0);
    write_data_byte(RESU_BUF_RPTR, 0);
}

/**
 * @brief      	This function is used to write result waiting to be read by the upper monitor.
 * @param[in]  	*src_buf - save the result to the specified location waiting to be read.
 * @param[in]  	len      - set the length of result waiting to read.
 * @return     	none
 */
int result_buff_write(unsigned char* data, unsigned char length)
{
	unsigned char wptr = read_data_byte(RESU_BUF_WPTR);
	unsigned char rptr = read_data_byte(RESU_BUF_RPTR);
	unsigned char buf_cnt = read_data_byte(RESU_BUF_CNT);
	unsigned char result_buff_size = read_data_byte(RESU_BUF_SIZE);
	unsigned char temp_buff[result_buff_size];
	if(length > (result_buff_size - 4)) return -1;

	autotest_package_t_ptr para = (autotest_package_t_ptr)temp_buff;
	memcpy(para->payload,data,length);
	para->payload_length = length;

	unsigned short crc16 =  crc_16(temp_buff,length);
	para->crc[0] = crc16&0xff;
	para->crc[1] = crc16>>8;

    length = RESULT_BUFF_SIZE;

    if (((wptr+1)%buf_cnt) == rptr) {
        return 0;
    }
    else {
        unsigned int buf_addr    = read_data_word(RESU_BUF_ADDR);
        unsigned char buf_size   = read_data_byte(RESU_BUF_SIZE);
        unsigned int write_addr  = buf_addr + buf_size*wptr;
        unsigned char i = 0;
	    for (i = 0; i < length; i++) {
	    	write_data_byte(write_addr+i, temp_buff[i]);
	    }
	    write_data_byte(RESU_BUF_WPTR, (wptr+1)%buf_cnt);
        return i;
    }
}

/**
 * @brief     This function performs to calculate cyclic redundancy check code.
 * @param[in] data   - data.
 * @param[in] length - the length of data.
 * @return    crc    - calculate cyclic redundancy
 */
unsigned short crc_16(unsigned char *data,unsigned int length)
{
	unsigned int pos = 0;

    unsigned short crc = 0xFFFF;
    for (pos = 0; pos < length; pos++)
    {
        crc ^= (unsigned short)data[pos];
        for (int i = 8; i != 0; i--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief   This function performs to send command to automatic test tool.
 * @param[in] data   - command data waiting to be sent.
 * @param[in] length - the length of data.
 * @return    none
 */
void respond_data(unsigned char* data, unsigned char length)
{
	unsigned char result_buff_size = read_data_byte(RESU_BUF_SIZE);
	if(length > (result_buff_size - 2)) return;
	unsigned char temp_para[result_buff_size];
	unsigned short crc16 =  crc_16(data,length);
	for(unsigned char i = 0; i < length; i++){
		temp_para[i] = data[i];
	}
	temp_para[length]   =  crc16>>8;
	temp_para[length+1] =  crc16&0xff;
	result_buff_write(temp_para,length+2);
}
/**
 * @brief   This function performs to send command name to automatic test tool.
 * @param[in] dut_cmd - command name waiting to be sent.
 * @return    none
 */
void respond_name(unsigned short dut_cmd)
{
	unsigned char temp_para[3];
	temp_para[0]  =  (dut_cmd>>8)&0xFF;
	temp_para[1]  =  dut_cmd&0xFF;
	temp_para[2]  =  0x03;
	respond_data(temp_para,0x03);
}

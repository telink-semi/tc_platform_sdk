/********************************************************************************************************
 * @file    config_obj.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifdef RF_CERTIFICATION_CGF_EN
    #include "config_obj.h"
    #include "flash.h"
    #include <string.h>

volatile __attribute__((section(".rf_certification_cfg"))) unsigned int config_data_sec[CONFIG_OBJ_MAX_SIZE] = {
    CONFIG_CHECK_VAL_BEGIN_U32,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    CONFIG_CHECK_VAL_END_U32};

static unsigned char s_config_data_flag = 0;
    #if defined(MCU_STARTUP_FLASH)
extern unsigned long _RF_CERTIFICATION_CFG_ADDR_OFFSET;
    #endif

/**
 * @brief This function initializes a configuration object located in config_data_sec.
 */
void config_obj_init(void)
{
    #if defined(MCU_STARTUP_FLASH)
    flash_read_page((unsigned long)&_RF_CERTIFICATION_CFG_ADDR_OFFSET, sizeof(unsigned int) * CONFIG_OBJ_MAX_SIZE, (unsigned char *)&config_data_sec);
    #endif
    // Cast the memory block `config_data_sec` to a pointer to tlk_config_obj_ptr
    tlk_config_obj_ptr ptr = (tlk_config_obj_ptr)config_data_sec;

    // Check the integrity of the configuration object using check values
    if (ptr->check_val_begin != CONFIG_CHECK_VAL_BEGIN_U32 || ptr->check_val_end != CONFIG_CHECK_VAL_END_U32) {
        // Set error flag if check values do not match expected values
        s_config_data_flag = 1;
        return;
    }

    // Calculate the number of '1' bits in the configuration data
    volatile unsigned int *data     = (volatile unsigned int *)config_data_sec + 2;
    unsigned int           bit1_cnt = 0;
    for (int i = 0; i < CONFIG_OBJ_MAX_SIZE - 3; i++) {
        for (int j = 0; j < 32; j++) {
            bit1_cnt += ((data[i] >> j) & 1);
        }
    }

    // Check if the calculated bit count matches ptr->bit1_cnt and is not zero
    if (ptr->bit1_cnt != bit1_cnt || (bit1_cnt == 0)) {
        // Set error flag if bit count is incorrect or zero
        s_config_data_flag = 2;
        return;
    }

    // Check offset and size of pointers in the configuration content
    for (unsigned int i = 0; i < ptr->pointer_cnt; i++) {
        // Check if the sum of size and offset exceeds the maximum allowed size
        if (ptr->content.pointer[i].size + ptr->content.pointer[i].offset > ((CONFIG_OBJ_MAX_SIZE - 1) * 4)) {
            // Set error flag if offset + size exceeds the maximum size limit
            s_config_data_flag = 3;
            break;
        }

        // Check if the offsets of consecutive pointers are in ascending order
        if (i > 0) {
            if (ptr->content.pointer[i - 1].size + ptr->content.pointer[i - 1].offset > ptr->content.pointer[i].offset) {
                // Set error flag if offsets are not in ascending order
                s_config_data_flag = 4;
                break;
            }
        }
    }

    // Return after all checks are performed
    return;
}

/**
 * @brief  This function is used to read a specific type of data from the configuration data and copy it to a specified buffer.
 * @param type - The type of configuration data to retrieve
 *        buffer - Pointer to the buffer where data will be stored
 *        buffer_max_size - Maximum capacity of the buffer
 * @return Returns whether the operation was successful; 1 for success, 0 for failure
 */
unsigned char get_config_data(tlk_config_type_e type, unsigned char *buffer, unsigned short buffer_max_size)
{
    if (s_config_data_flag != 0) {
        return 0;
    }

    unsigned char      ret = 0;
    unsigned short     size;
    tlk_config_obj_ptr ptr = (tlk_config_obj_ptr)config_data_sec;
    for (unsigned int i = 0; i < ptr->pointer_cnt; i++) {
        if (ptr->content.data[i] == 0) {
            continue;
        }

        if (ptr->content.pointer[i].type == type) {
            size                         = ((ptr->content.pointer[i].size <= buffer_max_size) ? ptr->content.pointer[i].size : buffer_max_size);
            volatile unsigned char *src  = (volatile unsigned char *)config_data_sec + ptr->content.pointer[i].offset;
            unsigned char          *dest = buffer;

            for (unsigned short j = 0; j < size; ++j) {
                dest[j] = src[j];
            }
            //            memcpy(buffer, ((unsigned char volatile*)config_data_sec) + ptr->content.pointer[i].offset, size);
            ret = 1;
            break;
        }
    }

    return ret;
}

/**
 * @brief This function is used to read the offset of a specific type of configuration data.
 * @param type The type of configuration data whose offset is to be retrieved
 * @return Returns the offset of the specified type; 0 if not found or on error
 */
unsigned short get_config_offset(tlk_config_type_e type)
{
    if (s_config_data_flag != 0) {
        return 0;
    }

    unsigned short     ret = 0;
    tlk_config_obj_ptr ptr = (tlk_config_obj_ptr)config_data_sec;
    for (unsigned int i = 0; i < ptr->pointer_cnt; i++) {
        if (ptr->content.data[i] == 0) {
            continue;
        }

        if (ptr->content.pointer[i].type == type) {
            ret = ptr->content.pointer[i].offset;
            break;
        }
    }

    return ret;
}

/**
 * @brief This function is used to read a byte from the configuration data at the specified offset.
 * @param offset The offset within the configuration data to retrieve the byte from
 * @return Returns the byte at the specified offset
 */
unsigned char get_config_data_byte(unsigned short offset)
{
    volatile unsigned char *data = (volatile unsigned char *)config_data_sec;
    return data[offset];
}

/**
 * @brief This function is used to read a half word (16 bits) from the configuration data at the specified offset.
 * @param offset The offset within the configuration data to retrieve the half word from
 * @return Returns the half word (16-bit) at the specified offset
 */
unsigned short get_config_data_half_word(unsigned short offset)
{
    volatile unsigned char *data = (volatile unsigned char *)config_data_sec;
    return data[offset] | (data[offset + 1] << 8);
}

/**
 * @brief This function is used to read a word (32 bits) from the configuration data at the specified offset.
 * @param offset The offset within the configuration data to retrieve the word from
 * @return Returns the word (32-bit) at the specified offset
 */
unsigned int get_config_data_word(unsigned short offset)
{
    volatile unsigned char *data = (volatile unsigned char *)config_data_sec;
    return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24);
}

#endif /*SUPPORT_USER_CONFIG*/

/********************************************************************************************************
 * @file    config_obj.h
 *
 * @brief   This is the header file for Telink MCU
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
#ifndef TLK_CONFIG_OBJ_H
#define TLK_CONFIG_OBJ_H

#ifdef RF_CERTIFICATION_CGF_EN

    #define CONFIG_OBJ_MAX_SIZE        64

    #define CONFIG_CHECK_VAL_BEGIN_U32 (('c' << 0) | ('f' << 8) | ('g' << 16) | ('{' << 24))
    #define CONFIG_CHECK_VAL_END_U32   (('c' << 8) | ('f' << 16) | ('g' << 24) | ('}' << 0))

typedef enum
{
    TLK_CONFIG_TYPE_BQB = 0x10,
    TLK_CONFIG_TYPE_EMI = 0x11,
    TLK_CONFIG_TYPE_PA  = 0x12,
} tlk_config_type_e;

/*
 * @brief Structure for BQB configuration.
 */
typedef struct
{
    unsigned int  access_code;
    unsigned int  baud_rate;
    unsigned char uart_tx;
    unsigned char uart_rx;
    unsigned char cal_pos;
    unsigned char power_mode;
    unsigned char io_voltage;
    unsigned char power_index;
    unsigned char power_slice;
    unsigned char voltage_type;
    unsigned char cap;
    unsigned char swire_through_usb_en;
    unsigned char pa_en;
    unsigned char power_slice_en;

} __attribute__((packed)) bqb_config_data_t;

/*
 * @brief Structure for EMI configuration.
 */
typedef struct
{
    unsigned char power_mode;
    unsigned char io_voltage;
    unsigned char cap;
    unsigned char cal_pos;
    unsigned int  access_code;
    unsigned char swire_through_usb_en;
    unsigned char pa_en;
} __attribute__((packed)) emi_config_data_t;

typedef struct
{
    unsigned int type   : 8;
    unsigned int offset : 12;
    unsigned int size   : 12;
} __attribute__((packed)) tlk_config_pointer_t;

typedef struct
{
    unsigned int check_val_begin;
    unsigned int bit1_cnt; //all bit1 counts in config_obj except check_val_begin, check_val_end and bit1_cnt
    unsigned int pointer_cnt;

    union
    {
        tlk_config_pointer_t pointer[CONFIG_OBJ_MAX_SIZE - 4];
        unsigned int         data[CONFIG_OBJ_MAX_SIZE - 4];
    } content;

    unsigned int check_val_end;
} __attribute__((packed)) tlk_config_obj;

typedef volatile tlk_config_obj *tlk_config_obj_ptr;

extern void           config_obj_init(void);
extern unsigned char  get_config_data(tlk_config_type_e type, unsigned char *buffer, unsigned short buffer_max_size);
extern unsigned short get_config_offset(tlk_config_type_e type);
extern unsigned char  get_config_data_byte(unsigned short offset);
extern unsigned short get_config_data_half_word(unsigned short offset);
extern unsigned int   get_config_data_word(unsigned short offset);

#endif /*SUPPORT_USER_CONFIG*/

#endif /*TLK_CONFIG_OBJ_H*/

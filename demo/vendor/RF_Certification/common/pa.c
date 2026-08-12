/********************************************************************************************************
 * @file    pa.c
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
#include "pa.h"
#include "gpio.h"
#include "flash.h"
#include "config_obj.h"
#include "../common/rf_certification.h"

#if (PA_SUPPORT_USR_CONFIG)
/**
 * @brief    Enumeration type of PA operation.
 */
typedef enum
{
    PA_SETTING_OPT_OUTPUT = 0xa5,
} pa_setting_opt_e;

/**
 * @brief    Structure used to store an operation instruction.
 */
typedef union
{
    unsigned int cmd;

    struct
    {
        unsigned char state; //PA_SETTING_STATE
        unsigned char opt;   //PA_SETTING_OPT
        unsigned char para0; //PA_GPIO_PIN
        unsigned char para1; //PA_GPIO_PIN_STATE
    } data;
} pa_setting_cmd_t, *pa_setting_cmd_t_ptr;

/**
 * @brief    Structure used to store PA Setting information.
 */
typedef struct
{
    unsigned int addr;
    unsigned int bit1_cnt;
    unsigned int cmd_cnt;
} pa_setting_data_t, *pa_setting_data_t_ptr;

/**
 * @brief        This function serves to read data saved in binary file.
 * @param[in]   addr   - Position in binary file.
 * @return         Four-byte result data.
 */
static inline unsigned int read_data32(unsigned int addr)
{
    return get_config_data_word(addr & 0xffff);
}

/**
 * @brief        This function serves to execute PA operations according to the cmd.
 * @param[in]   cmd      - PA operation command.
 * @return         none
 */
static inline void cmd_operation(pa_setting_cmd_t cmd)
{
    unsigned short pin = get_pin(cmd.data.para0);
    switch (cmd.data.opt) {
    case PA_SETTING_OPT_OUTPUT:
        gpio_function_en(pin);
        gpio_input_dis(pin);
        gpio_output_en(pin);
        if (cmd.data.para1) {
            gpio_set_high_level(pin);
        } else {
            gpio_set_low_level(pin);
        }
        break;
    default:
        break;
    }
}
#endif


/**
 * @brief    Structure used to store PA operation information.
 */
typedef struct
{
#if (PA_SUPPORT_USR_CONFIG)
    pa_setting_data_t pa_setting_data;
#endif
    unsigned char pa_en;
    unsigned char bypass_en;
} pa_operation_t, *pa_operation_t_ptr;

/**
 * @brief    Local variable used to hold data related to the PA operation.
 */
#if (PA_SUPPORT_USR_CONFIG)
static pa_operation_t s_pa_operation = {
    {0, 0, 0},
    0,
    0
};
#else
static pa_operation_t s_pa_operation = {0, 0};
#endif


#if (PA_SUPPORT_USR_CONFIG)
/**
 * @brief        This function serves to initialize PA setting.
 * @param[in]   addr       - Address to save PA related operations.
 * @param[in]   bypass_en  - To set PA bypass mode, 0:disable 1:enable.
 * @return         none
 */
void pa_setting_init(unsigned int addr, unsigned char bypass_en)
{
    s_pa_operation.pa_setting_data.addr = read_data32(addr);

    s_pa_operation.bypass_en = bypass_en;

    //check address
    if (s_pa_operation.pa_setting_data.addr != addr) {
        s_pa_operation.pa_en = 0;
        return;
    }


    s_pa_operation.pa_setting_data.bit1_cnt = read_data32(addr + 4);
    s_pa_operation.pa_setting_data.cmd_cnt  = read_data32(addr + 8);

    //check bit1 count
    unsigned int data_check;
    unsigned int bit1_cnt = 0;
    unsigned int data_len = 1 + s_pa_operation.pa_setting_data.cmd_cnt;
    for (unsigned int i = 0; i < data_len; i++) {
        data_check = read_data32(s_pa_operation.pa_setting_data.addr + 8 + 4 * i);
        for (unsigned char j = 0; j < 32; j++) {
            bit1_cnt += (data_check >> j) & 1;
        }
    }

    if (bit1_cnt != s_pa_operation.pa_setting_data.bit1_cnt) {
        s_pa_operation.pa_en = 0;
        return;
    }

    s_pa_operation.pa_en = 1;
}
#else
/**
 * @brief        This function serves to initialize PA setting.
 * @param[in]   pa_en  - To set PA enable, 0:disable 1:enable.
 * @param[in]   bypass_en  - To set PA bypass mode, 0:disable 1:enable.
 * @return         none
 */
void pa_setting_init(unsigned char pa_en, unsigned char bypass_en)
{
    s_pa_operation.pa_en     = pa_en;
    s_pa_operation.bypass_en = bypass_en;

    //to do:To use the PA function, enable related PA control pins
}
#endif


/**
 * @brief        This function serves to execute PA operations according to the state.
 * @param[in]   state      - PA working state.
 * @return         none
 */
void pa_operation(pa_setting_state_e state)
{
    if (!s_pa_operation.pa_en) {
        return;
    }
#if (PA_SUPPORT_USR_CONFIG)
    if (s_pa_operation.bypass_en) {
        state = PA_SETTING_STATE_BYPASS;
    }

    pa_setting_cmd_t cmd;
    for (unsigned int i = 0; i < s_pa_operation.pa_setting_data.cmd_cnt; i++) {
        cmd.cmd = read_data32(s_pa_operation.pa_setting_data.addr + 12 + 4 * i);

        if (state == cmd.data.state) {
            cmd_operation(cmd);
        }
    }

#else
    if (s_pa_operation.bypass_en) {
        //to do:To use the PA function, enable related PA control pins
    } else {
        switch (state) {
        case PA_SETTING_STATE_INIT:
            //to do:To use the PA function, enable related PA control pins
            break;
        case PA_SETTING_STATE_TX:
            //to do:To use the PA function, enable related PA control pins
            break;
        case PA_SETTING_STATE_RX:
            //to do:To use the PA function, enable related PA control pins
            break;
        default:
            break;
        }
    }
#endif
}

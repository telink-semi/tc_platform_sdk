/********************************************************************************************************
 * @file    hal_flash.c
 *
 * @brief   This is the source file for B85m
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "hal_flash.h"


extern const flash_hal_handler_t flash_list[];
extern const unsigned int FLASH_CNT;
flash_hal_handler_t g_flash_handler = {0};
/*
 * 1. Whether hal_flash_init parameter handler->list is NULL or not, as long as the default configuration of flash_list is successfully configured,
 *    g_mid_matched is true, which does not affect the use of hal_flash_4line_en/hal_flash_4line_dis function;
 * 2. hal_flash_init return value is given to the upper layer (flash_init_list/flash_list) flash mid whether the match is successful return status,
 *    the upper layer according to the actual situation to do the logical analysis and processing;
 */
bool g_mid_matched = false;
/**
 * @brief       This function is used to initializing flash configurations.
 *              it must be called before the hal_xx function can be called.
 * @param[in]   handler - provide member structures that can be modified by upper-level concerns,
 *              When the bin protection area uses the default size, then handler->list is NULL,
 *              if the customer needs to modify the size of the protection area, enable FLASH_PROTECT_MODIFY_CONFIG and modify it through flash_init_list.
 * @return      0:flash_init mid matches successfully, and handler(flash_init_list) mid matches successfully when FLASH_PROTECT_MODIFY_CONFIG is enabled;
 *              1:flash_init mid matches fail;
 *              2:handler(flash_init_list) mid matches fail;
 */
unsigned char  hal_flash_init(flash_hal_user_handler_t* handler){
    g_flash_handler.mid = flash_read_mid();
    for(unsigned int i = 0; i < FLASH_CNT; i++)
    {
        if(g_flash_handler.mid == flash_list[i].mid)
        {
            g_mid_matched = true;
            g_flash_handler.get_lock= flash_list[i].get_lock;
            g_flash_handler.unlock =  flash_list[i].unlock;
            g_flash_handler.lock_func = flash_list[i].lock_func;
            g_flash_handler.lock_size = flash_list[i].lock_size;
            break;
        }
    }
    if(g_mid_matched == false){
        return 1;
    }else if( handler->list != NULL){
        for(unsigned int j = 0; j < (handler->flash_cnt); j++)
        {
            if(g_flash_handler.mid == handler->list[j].mid)
            {
                g_flash_handler.lock_size =  handler->list[j].block_size;
                return 0;
            }
        }
        return 2;
    }else{
        return 0;
    }
}

/**
 * @brief       This function is used to lock a flash through lock_fp,
 *              if the flash is not locked, it is locked, if the flash is locked, it is not executed.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_lock(void)
{
    if(g_mid_matched)
    {
        if(0 == g_flash_handler.get_lock())
        {

            return g_flash_handler.lock_func(g_flash_handler.lock_size);

        }
        return 1;
    }
    return 3;

}

/**
 * @brief       This function is used to unlock a flash through lock_fp,
 *              if the flash is locked, it is unlocked, if the flash is unlocked, it is not executed.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_unlock(void)
{
    if(g_mid_matched)
    {
        if(0 != g_flash_handler.get_lock())
        {
            return g_flash_handler.unlock();
        }
        return 1;

    }
    return 3;
}

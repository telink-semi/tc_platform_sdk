/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for Telink MCU
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
#include "app_config.h"

extern void user_init(void);
extern void main_loop (void);

/**
 * @brief       This function serves to handle the interrupt of MCU
 * @param[in]   none
 * @return      none
 */
#if (AUDIO_MODE == BUFF_TO_LINEOUT)
extern void* buff;
#endif
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
#if (AUDIO_MODE == BUFF_TO_LINEOUT)

    if(timer_get_interrupt_status(TMR_STA_TMR1) & FLD_TMR_STA_TMR1)
    {
        timer_clear_interrupt_status(TMR_STA_TMR1); //clear irq status
        audio_dfifo_write_data(buff, 48);
    }
#endif
}

/**
 * @brief       This is main function
 * @param[in]   none
 * @return      none
 */
int main(void)
{
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    while(1)
    {
        main_loop();
    }
    return 0;
}

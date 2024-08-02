/********************************************************************************************************
 * @file	common.c
 *
 * @brief	This is the header file for B85m
 *
 * @author	Driver Group
 * @date	2024
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
#include "common.h"
#include "driver.h"

/*
 * @note The flash protection size has been allocated by default, refer to the comment FLASH_PROTECT_MODIFY_CONFIG for details.
 */
flash_user_defined_list_t flash_init_list[] = {

#if (MCU_CORE_B80)
    // 128K
    {0x1160c8, FLASH_LOCK_LOW_64K_MID1160C8},
    {0x114485, FLASH_LOCK_LOW_64K_MID114485},
    {0x1164c8, FLASH_LOCK_LOW_64K_MID1164C8},
    // 512K
    {0x1360c8, FLASH_LOCK_LOW_256K_MID1360C8},
    {0x136085, FLASH_LOCK_LOW_256K_MID136085},
    {0x13325e, FLASH_LOCK_LOW_256K_MID13325E},
#elif (MCU_CORE_B80B)
    // 128K
    {0x114485, FLASH_LOCK_LOW_64K_MID114485},
    {0x1164c8, FLASH_LOCK_LOW_64K_MID1164C8},
    // 512K
    {0x136085, FLASH_LOCK_LOW_256K_MID136085},

#elif (MCU_CORE_B85)
    // 64K
    {0x1060C8, FLASH_LOCK_LOW_32K_MID1060C8},
    // 512K
    {0x1360C8, FLASH_LOCK_LOW_256K_MID1360C8},
    {0x13325E, FLASH_LOCK_LOW_256K_MID13325E},
    {0x134051, FLASH_LOCK_LOW_256K_MID134051},
    {0x136085, FLASH_LOCK_LOW_256K_MID136085},
    {0x1360EB, FLASH_LOCK_LOW_256K_MID1360EB},
    // 1M
    {0x14325E, FLASH_LOCK_LOW_768K_MID14325E},
    {0x1460C8, FLASH_LOCK_LOW_768K_MID1460C8},
    {0x011460C8, FLASH_LOCK_LOW_512K_MID011460C8},
#elif (MCU_CORE_B87)
    // 128K
    {0x1160C8, FLASH_LOCK_LOW_64K_MID1160C8},
    {0x11325E, FLASH_LOCK_LOW_64K_MID11325E},
    // 512K
    {0x1360C8, FLASH_LOCK_LOW_256K_MID1360C8},
    {0x13325E, FLASH_LOCK_LOW_256K_MID13325E},
    // 1M
    {0x146085, FLASH_LOCK_LOW_512K_MID146085},
    {0x1460C8, FLASH_LOCK_LOW_768K_MID1460C8},
    {0x14325E, FLASH_LOCK_LOW_768K_MID14325E},
    // 2M
    {0x1570CD, FLASH_LOCK_LOW_1M_MID1570CD},

#elif (MCU_CORE_B89)
    // 512K
    {0x1360C8, FLASH_LOCK_LOW_256K_MID1360C8},
#else
    {0, 0}
#endif
};
#if FLASH_PROTECT_MODIFY_CONFIG
#define list_fp flash_init_list
#else
#define list_fp NULL
#endif
flash_hal_user_handler_t flash_handler = {
    .list = list_fp,
    .flash_cnt = (sizeof(flash_init_list) / sizeof(flash_user_defined_list_t)),
};

void flash_init(unsigned char flash_protect_en)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_output_en(LED1, 1); // enable output
    unsigned char flash_init_flag = hal_flash_init(&flash_handler);
    if (flash_init_flag != 0)
    {
        gpio_write(LED1, 1);
        while (1);
    }
    if (flash_protect_en)
    {
        unsigned char lock_flag = hal_flash_lock();

        if (!(lock_flag == 1))
        {
            gpio_write(LED1, 1);
            while (1);
        }
    }
}

void platform_init(unsigned char flash_protect_en)
{
#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89)
    cpu_wakeup_init(INTERNAL_CAP_XTAL24M);
#elif (MCU_CORE_B85)
    cpu_wakeup_init();
#elif (MCU_CORE_B87)
    cpu_wakeup_init(LDO_MODE, INTERNAL_CAP_XTAL24M);
#endif

#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_B89)
    wd_32k_stop();
#endif

#if (MCU_CORE_B85) || (MCU_CORE_B87)
    // Note: This function must be called, otherwise an abnormal situation may occur.
    // Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
    user_read_flash_value_calib();
#elif (MCU_CORE_B89)
    // Note: This function must be called, otherwise an abnormal situation may occur.
    // Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
    user_read_otp_value_calib();
#elif (MCU_CORE_B80 || MCU_CORE_B80B)
    // Note: This function must be called, otherwise an abnormal situation may occur.
    // Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
#if (PACKAGE_TYPE == OTP_PACKAGE)
    user_read_otp_value_calib();
#elif (PACKAGE_TYPE == FLASH_PACKAGE)
    user_read_flash_value_calib();
#endif

#endif
    gpio_init(0);
    // Note: This is to set SWS pull. If SWS is not set up, SWS will be floating, causing abnormal sleep currents of suspend,
    // there may be the risk of sws miswriting the chip registers or sram causing a crash.
    gpio_setup_up_down_resistor(GPIO_SWS, PM_PIN_PULLUP_1M);

    /**
    ===============================================================================
                    ##### driver sdk firmware protection #####
    ===============================================================================
    Flash write protection must be added, the size of the protected area is decided according to the application bin file,
    the principle is as follows:
    1.The program area is protected (to prevent the program area from being erased by mistake);
    2.the program will modify the data area is not protected (if write-protected, each time before erasing the need to unprotect,
        so that there is a risk that the status register of the flash has been mistakenly rewritten);

    @note if flash protection fails, LED1 lights up long, and keeps while.
    ===============================================================================
    */
    flash_init(flash_protect_en);
}

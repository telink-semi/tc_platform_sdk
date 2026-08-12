/********************************************************************************************************
 * @file    app_autotest.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#if(PM_MODE==PM_AUDO_TEST)

#if defined(MCU_CORE_TC122X)
    #define SRAM_BASE_ADDR                      0x40000
#elif defined(MCU_CORE_TC321X)||defined(MCU_CORE_TC123X)
    #define SRAM_BASE_ADDR                      0x43000
#endif

#define PIN_ADDR                                (SRAM_BASE_ADDR + 0x04) //pA0-PB2
#define SLEEP_MODE_ADDR                         (SRAM_BASE_ADDR + 0x05) //00:suspend 01:deep ret 02:deep
#define TEST_COMMAND_ADDR                       (SRAM_BASE_ADDR + 0x06)
#define RUN_STATUE_ADDR                         (SRAM_BASE_ADDR + 0x07)

#define PULLDOWN_100K_WAKEUP_LEVEL_HIGH         00
#define PULLUP_1M_WAKEUP_LEVEL_LOW              01
#define PULLUP_10K_WAKEUP_LEVEL_LOW             02
#define TEST_MODE                               PULLDOWN_100K_WAKEUP_LEVEL_HIGH

void pm_pin_sleep_wakeup_test(unsigned int pin, unsigned char sleep_mode);

/**
 * @brief   The pm test command and function
 */
typedef struct
{
    unsigned char cmd_id;                       /**< pm sleep command id */
    void (*func)(unsigned int, unsigned char);  /**< pm sleep function */
} test_list_t;

unsigned int  g_pin                             = 0;
unsigned char g_sleep_mode                      = 0;
unsigned char g_cmd_now                         = 0;
unsigned char g_run                             = 0;

volatile unsigned int g_debug_pin_flag          = 0x0;

#if defined(MCU_CORE_TC122X)
#define BUFF_PIN_LEN                            10
unsigned short gpio_buff_test_code[BUFF_PIN_LEN]={GPIO_PA0,GPIO_PA1,GPIO_PA2,GPIO_PA4,GPIO_PA5,GPIO_PA6,GPIO_PA7,
        GPIO_PB0,GPIO_PB1,GPIO_PB2};
#elif defined(MCU_CORE_TC321X)
#define BUFF_PIN_LEN                            33
unsigned short gpio_buff_test_code[BUFF_PIN_LEN]={GPIO_PA0,GPIO_PA1,GPIO_PA2,GPIO_PA4,GPIO_PA5,GPIO_PA6,GPIO_PA7,
        GPIO_PB0,GPIO_PB1,GPIO_PB2,GPIO_PB3,GPIO_PB4,GPIO_PB5,GPIO_PB6,GPIO_PB7,
		GPIO_PC0,GPIO_PC1,GPIO_PC2,GPIO_PC3,GPIO_PC4,GPIO_PC5,GPIO_PC6,GPIO_PC7,
		GPIO_PD0,GPIO_PD1,GPIO_PD2,GPIO_PD3,GPIO_PD4,GPIO_PD5,GPIO_PD6,GPIO_PD7,
		GPIO_PE0,GPIO_PE1};
#elif defined(MCU_CORE_TC123X)
#define BUFF_PIN_LEN                            17
const GPIO_PinTypeDef gpio_buff_test_code[BUFF_PIN_LEN]={GPIO_PA0,GPIO_PA1,GPIO_PA2,GPIO_PA4,GPIO_PA5,GPIO_PA6,GPIO_PA7,
        GPIO_PB0,GPIO_PB1,GPIO_PB2,GPIO_PB3,GPIO_PB4,GPIO_PB5,GPIO_PB6,GPIO_PB7,
		GPIO_PC0,GPIO_PC1};
#endif

/**
 * @brief   Init the structure of the pm test command and function
 */
test_list_t ate_list[] = {
    {0x01, pm_pin_sleep_wakeup_test},
};

void gpio_set_pin_config(void)
{
    for(unsigned char i=0;i<BUFF_PIN_LEN;i++){
#if (TEST_MODE == PULLDOWN_100K_WAKEUP_LEVEL_HIGH)
    	gpio_setup_up_down_resistor(gpio_buff_test_code[i],PM_PIN_PULLDOWN_100K);
#elif (TEST_MODE == PULLUP_1M_WAKEUP_LEVEL_LOW)
        gpio_setup_up_down_resistor(gpio_buff_test_code[i],PM_PIN_PULLUP_1M);
#elif (TEST_MODE == PULLUP_10K_WAKEUP_LEVEL_LOW)
        gpio_setup_up_down_resistor(gpio_buff_test_code[i], PM_PIN_PULLUP_10K);
#endif
    }
}

/**
 * @brief       This function serves to pm Init
 * @return      none
 */
void pm_init(void)
{
    gpio_set_pin_config();
}

/**
 * @brief       This function serves to pm CarryOnly
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return      none
 */

void pm_pin_sleep_wakeup_test(unsigned int pin, unsigned char sleep_mode)
{
	if(pin < 8) {
		pin = (GPIO_PinTypeDef)(GPIO_GROUPA | BIT(pin%8));
	} else if(pin >= 8 && pin < 16) {
		pin = (GPIO_PinTypeDef)(GPIO_GROUPB | BIT(pin%8));
	}

#if(MCU_CORE_TC123X)||(MCU_CORE_TC321X)
	else if(pin >= 16 && pin < 24) {
		pin = (GPIO_PinTypeDef)(GPIO_GROUPC | BIT(pin%8));
	}
#endif
#if(MCU_CORE_TC321X)
	else if(pin >= 24 && pin < 32) {
		pin = (GPIO_PinTypeDef)(GPIO_GROUPD | BIT(pin%8));
	} else if(pin >= 32 && pin < 40) {
		pin = (GPIO_PinTypeDef)(GPIO_GROUPE | BIT(pin%8));
	}
#endif

#if (TEST_MODE == PULLDOWN_100K_WAKEUP_LEVEL_HIGH)
    cpu_set_gpio_wakeup(pin, Level_High, 1);
#elif (TEST_MODE == PULLUP_1M_WAKEUP_LEVEL_LOW)
    cpu_set_gpio_wakeup(pin, Level_Low, 1);
#elif (TEST_MODE == PULLUP_10K_WAKEUP_LEVEL_LOW)
    cpu_set_gpio_wakeup(pin, Level_Low, 1);
#endif

    if(sleep_mode == 0x00) {
        sleep_mode = SUSPEND_MODE;
        cpu_sleep_wakeup(sleep_mode , PM_WAKEUP_PAD, 0);
    } else if(sleep_mode == 0x01) {
#if(MCU_CORE_TC321X)||(MCU_CORE_TC123X)
        sleep_mode = DEEPSLEEP_MODE_RET_SRAM_LOW64K;
#else
        sleep_mode = DEEPSLEEP_MODE_RET_SRAM_LOW8K;
#endif
        cpu_sleep_wakeup(sleep_mode, PM_WAKEUP_PAD, 0);
    } else if(sleep_mode == 0x02) {
        sleep_mode = DEEPSLEEP_MODE;
        cpu_sleep_wakeup(sleep_mode, PM_WAKEUP_PAD, 0);
    }
    g_debug_pin_flag = 0xff09;
}

/**
 * @brief     This function serves to pm ServiceLoop
 * @return    none
 */
void pm_serviceloop(void)
{
    unsigned char i = 0;

    write_reg8(RUN_STATUE_ADDR, 0);
    while (1) {
        g_run = read_reg8(RUN_STATUE_ADDR); // get the run state!
        if (g_run != 1) continue;

        g_pin           = read_reg8(PIN_ADDR);
        g_sleep_mode    = read_reg8(SLEEP_MODE_ADDR);
        g_cmd_now       = read_reg8(TEST_COMMAND_ADDR); // get the command!

//        printf("g_run:0x%x g_cmd_now:%x g_pin:%x g_sleep_mode:%x \r\n", g_run, g_cmd_now, g_pin, g_sleep_mode);

        for (i = 0; i < sizeof(ate_list) / sizeof(test_list_t); i++) {
            if (g_cmd_now != ate_list[i].cmd_id) continue;

            ate_list[i].func(g_pin, g_sleep_mode);
        }

        g_run = 0;
        write_reg8(RUN_STATUE_ADDR, g_run);
    }
}

/**
 * @brief       This function serves to User Init
 * @return      none
 */
void user_init(void)
{
	gpio_shutdown(GPIO_ALL);
    pm_init();
}

/**
 * @brief       This function serves to main
 * @return      none
 */
void main_loop(void)
{
    pm_serviceloop();
}
#endif

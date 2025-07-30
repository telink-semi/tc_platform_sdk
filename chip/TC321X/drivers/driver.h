/********************************************************************************************************
 * @file    driver.h
 *
 * @brief   This is the header file for TC321X
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
#pragma once
#include "lib/include/emi.h"
#include "lib/include/pm/pm.h"
#include "lib/include/pm/pm_internal.h"
#include "lib/include/rf_drv.h"
#include "lib/include/sys.h"
#include "lib/include/stimer.h"
#include "lib/include/efuse.h"
#include "lib/include/sd_adc.h"

#include "clock.h"
#include "bsp.h"
#include "aes.h"
#include "analog.h"
#include "audio.h"
#include "compiler.h"
#include "gpio_default.h"
#include "gpio.h"
#include "pwm.h"
#include "irq.h"
#include "flash.h"
#include "i2c.h"
#include "spi.h"
#include "uart.h"
#include "watchdog.h"
#include "random.h"
#include "register.h"
#include "dma.h"
#include "timer.h"
#include "s7816.h"
#include "printf.h"
#include "ir_learn.h"
#include "qdec.h"
#include "keyscan.h"
#include "flash/flash_type.h"
#include "sdk_version.h"
#include "flash/flash_common.h"

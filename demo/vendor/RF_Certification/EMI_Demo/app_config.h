/********************************************************************************************************
 * @file    app_config.h
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
#pragma once
#include "driver.h"
#include "common.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define ACCESS_CODE              0x29417671
#define SWITCH_POWER_MODE        0x40 /**< Power mode of the chip.(LDO_LDO:0x40;DCDC_LDO:0x41;DCDC_DCDC:0x43)*/
#define SWITCH_INTERNAL_CAP      0    /**< Capacitor selection: 0 for internal cap, 1 for external cap. */
#define SWITCH_CALI_POSITION     0    /**< Calibration position: 0 for FLASH, 1 for OTP. */
#define SWITCH_GPIO_VOLTAGE      0    /**< The configuration of GPIO voltage. (0:GPIO voltage 3.3V 1:GPIO voltage 1.8V)*/
#define PA_FUNCTION_ENABLE       0    /**< PA enable flag (0: disabled,1: enabled). */
#define PA_BYPASS_ENABLE         0    /**< PA bypass enable flag (0: disabled,1: enabled).*/
#define SWIRE_THROUGH_USB_ENABLE 0    /**< Enable Swire through USB(0: disabled,1: enabled) */
#define POWER_SLICE_EN           0    /**< Power slice enable flag (0: disabled,1: enabled)*/

#define EMI_TX_PACKET_CNT        0    /**< Tx send packet quantity mode selection. (0:Unlimit,1:1000)*/
#define EMI_RUN_STATE            1    /**< Current operational state. */
#define EMI_TEST_CMD_NOW         1    /**< Current test command being executed. */
#define EMI_POWER_LEVEL          0    /**< If POWER_SLICE_EN is 0:tx power level;If POWER_SLICE_EN is 1: tx power slice. */
#define EMI_RF_CHN               2    /**< RF channel number.Default: 2 (Frequency channel 2402 MHz) */
#define EMI_RF_MODE              0    /**< RF mode setting.(0:BLE2M  1:BLE1M  2:ZIGBEE 250K  3:BLE500K  4:BLE125K  5:Private 1M  6:Private 2M) */
#define EMI_HOPPING_EN           0    /**< Frequency hopping enable flag. (0:disable, 1:enable) */
#define EMI_ADAPTIVE_EN          0    /**< Adaptive test enable flag. (0:disable, 1:enable)*/
#define EMI_PKT_TONE_EN          0    /**< Packet tone enable flag.   (0:disable, 1:enable)*/
#define EMI_CD_TIME_UNIT         0    /**< Time unit selection flag for tx carrier mode.(0:ms) */
#define EMI_PKT_DUTY_CYCLE       50   /**< TX Packet duty cycle setting.Default: 50 (50% duty cycle) */
#define EMI_PKT_TYPE             0    /**< The type of data sent (0:pbrs9  1:0x0f  2:0x55  3:0xaa  4:0xf0  5:0x00  6:0xff) */
#define EMI_TX_PAYLOAD_LEN       37   /**< Length of transmitted payload. */
#define EMI_PKT_TONE_DLY         40   /**< Duration of tone in packet tone mode,Unit in microseconds. */

/**
 * @brief Define system clock
 */
#if(TEST_DEMO==DUT_8278_FLASH)
#define CLOCK_SYS_CLOCK_HZ 32000000
#else
#define CLOCK_SYS_CLOCK_HZ 24000000
#endif

/**
 * @note The sys_clock.h file depends on the definition of CLOCK_SYS_CLOCK_HZ so CLOCK_SYS_CLOCK_HZ must be placed before #include "sys_clock.h" without reversing the order.
 */
#include "sys_clock.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

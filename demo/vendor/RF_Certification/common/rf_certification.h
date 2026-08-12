/********************************************************************************************************
 * @file    rf_certification.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "common.h"
#include "calibration.h"
#include "pa.h"
#include "config_obj.h"
#include "lib/include/emi_cert.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
{
#endif


    /**
 * @brief Structure to store EMI configuration.
 */
    typedef struct
    {
        unsigned int access_code;               /**< Access code. */
#ifdef RF_CERTIFICATION_CGF_EN
        unsigned short pa_addr;                 /**< PA Configuration Address. */
#endif
        unsigned char cal_pos;                  /**< Calibration position: 0 for FLASH, 1 for OTP. */
        unsigned char power_mode;               /**< Power mode of the chip. */
        unsigned char io_voltage;               /**< The configuration of GPIO voltage. (0:GPIO voltage 3.3V 1:GPIO voltage 1.8V)*/
        unsigned char pa_en                : 1; /**< PA enable flag (0: disabled,1: enabled). */
        unsigned char pa_bypass_en         : 1; /**< PA bypass enable flag (0: disabled,1: enabled).*/
        unsigned char swire_through_usb_en : 1; /**< Enable Swire through USB(0: disabled,1: enabled) */
        unsigned char cap                  : 1; /**< Capacitor selection: 0 for internal cap, 1 for external cap. */
    } __attribute__((packed)) emi_setting_t;

    /**
 * @brief This structure defines various parameters related to EMI test
 */
    typedef struct
    {
        signed char   g_rssi_val;           /**< Received Signal Strength Indicator (RSSI) value. */
        unsigned char g_tx_packet_cnt;      /**< Tx send packet quantity mode selection. 0:Unlimit,1:1000*/
        unsigned char g_run_state;          /**< Current operational state. */
        unsigned char g_test_cmd_now;       /**< Current test command being executed. */
        unsigned char g_power_level;        /**< If g_power_slice_en is 0:tx power level;If g_power_slice_en is 1: tx power slice. */
        unsigned char g_chn;                /**< RF channel number.Default: 2 (Frequency channel 2402 MHz) */
        unsigned char g_rf_mode;            /**< RF mode setting.(0:BLE2M  1:BLE1M  2:ZIGBEE 250K  3:BLE500K  4:BLE125K  5:Private 1M  6:Private 2M) */
        unsigned char g_hop_en         : 1; /**< Frequency hopping enable flag. */
        unsigned char g_adaptive_en    : 1; /**< Adaptive test enable flag. (0: disabled,1: enabled)*/
        unsigned char g_pkt_tone_en    : 1; /**< Packet tone enable flag. (0: disabled,1: enabled)*/
        unsigned char g_power_slice_en : 1; /**< Power slice enable flag (0: disabled,1: enabled)*/
        unsigned char g_cd_time_unit   : 1; /**< Time unit selection flag for tx carrier mode.(0:ms) */
        unsigned char                  : 3; /**< Reserved bits. */

        union
        {
            unsigned int g_rx_packet_num; /**< Number of received packets. */

            struct
            {
                unsigned char g_pkt_duty_cycle; /**< TX Packet duty cycle setting.Default: 50 (50% duty cycle) */
                unsigned char g_pkt_type;       /**< The type of data sent (0:pbrs9  1:0x0f  2:0x55  3:0xaa  4:0xf0  5:0x00  6:0xff)*/
                unsigned char g_tx_payload_len; /**< Length of transmitted payload. */
                unsigned char g_pkt_tone_delay; /**< Duration of tone in packet tone mode,Unit in microseconds. */
            } tx_cfg;
        } rx_cfg;
    } __attribute__((packed)) emi_cmd_t;

    /**
 * @brief Structure to store BQB configuration.
 */
    typedef struct
    {
        unsigned int   access_code;         /**< Access code. */
        unsigned int   baud_rate;           /**< Baud rate for UART communication. */
        unsigned short uart_tx;             /**< UART TX pin configuration. */
        unsigned short uart_rx;             /**< UART RX pin configuration. */
#ifdef RF_CERTIFICATION_CGF_EN
        unsigned short pa_addr;             /**< PA Configuration Address. */
#endif
        unsigned char cal_pos;              /**< Calibration position: 0 for FLASH, 1 for OTP. */
        unsigned char power_mode;           /**< Power mode of the chip.(LDO_LDO:0x00;DCDC_LDO:0x01;DCDC_DCDC:0x03)*/
        unsigned char io_voltage;           /**< The configuration of GPIO voltage. (0:GPIO voltage 3.3V 1:GPIO voltage 1.8V)*/
        unsigned char power;                /**< RF transmission power level in dBm. */
        unsigned char voltage_type;         /**< RF tx power voltage type (1:vbat,0:vant) */
        unsigned char cap;                  /**< Capacitor selection: 0 for internal cap, 1 for external cap. */
        unsigned char swire_through_usb_en; /**< Enable Swire through USB(0: disabled,1: enabled) */
        unsigned char pa_en          : 1;   /**< PA enable flag (0: disabled,1: enabled). */
        unsigned char pa_bypass_en   : 1;   /**< PA bypass enable flag (0: disabled,1: enabled).*/
        unsigned char power_slice_en : 1;   /**< Power slice enable flag (0: disabled,1: enabled)*/
        unsigned char                : 1;

    } __attribute__((packed)) bqb_setting_t;

/**
 * @brief        This function is used to convert the GPIO PIN data in ASCII form (unsigned char) into the enumeration value of the gpio pin (unsigned short).
 * @param[in]    value  - the GPIO PIN data in ASCII(unsigned char).
 * @return       Result of actual pin value.
 * @note         Example of value: PB1 value = (unsigned char)(('B'-'A')<<3 )|('1'-'0')
 */
#define get_pin(value) (((unsigned short)((value) >> 3) << 8) | BIT((value) & 0x07))
#define gpio_function_en(pin)			gpio_set_func((pin), AS_GPIO)
#define gpio_output_en(pin)				gpio_set_output_en((pin), 1)
#define gpio_output_dis(pin)			gpio_set_output_en((pin), 0)
#define gpio_input_en(pin)				gpio_set_input_en((pin), 1)
#define gpio_input_dis(pin)				gpio_set_input_en((pin), 0)
#define gpio_set_low_level(pin)			gpio_write((pin), 0)
#define gpio_set_high_level(pin)		gpio_write((pin), 1)

#if defined(MCU_CORE_TL721X)
    #define rf_stimer_get_tick()     rf_bb_timer_get_tick()
    #define RF_SYSTEM_TIMER_TICK_1US BB_TIMER_TICK_1US
    #define RF_SYSTEM_TIMER_TICK_1MS BB_TIMER_TICK_1MS
#else
    #define rf_stimer_get_tick()     stimer_get_tick()
    #define RF_SYSTEM_TIMER_TICK_1US SYSTEM_TIMER_TICK_1US
    #define RF_SYSTEM_TIMER_TICK_1MS SYSTEM_TIMER_TICK_1MS
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

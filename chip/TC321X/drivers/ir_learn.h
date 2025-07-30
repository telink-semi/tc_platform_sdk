/********************************************************************************************************
 * @file    ir_learn.h
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
#ifndef IR_LEARN_H_
#define IR_LEARN_H_

#include "gpio.h"
#include "register.h"
#include "irq.h"
#include "pwm.h"
#include "lib/include/sys.h"

extern unsigned char g_chip_version;
/**
 * @brief ir_learn capture mode
 */
typedef enum
{
    RISING_EDGE_START_CNT = 0,       /**<
                                        After enabling the ir_learning module, wait for the first rising edge tick to start counting.
                                        */
    IL_EN_HIGH_STATUS_START_CNT = 1, /**<
                                        After the module is enabled, the software specifies to enter the high state, and the tick starts counting immediately.
                                        */
    FALLING_EDGE_START_CNT = 2,      /**<
                                        After enabling the ir_learning module, wait for the first falling edge tick to start counting.
                                        */
    IL_EN_LOW_STATUS_START_CNT = 3,  /**<
                                        After the module is enabled, the software specifies to enter the low state, and the tick starts counting immediately.
                                        */
} ir_learn_start_cnt_mode_e;

/**
 *   ir learn interrupt
 */
typedef enum{
    IR_LEARN_HIGH_IRQ       = BIT(0), /**<
                                       The high interrupt is triggered when the logical state of the module changes from high to low.
                                       If you want to generate an interrupt, you need to enable the mask.
                                       If you only want to query the corresponding flag, you do not need to enable the mask.
                                       */

    IR_LEARN_CYCLE_IRQ      = BIT(1),/**<
                                       The cycle interrupt is triggered when the logical state of the module changes from low to high.
                                       If you want to generate an interrupt, you need to enable the mask.
	                                   If you only want to query the corresponding flag, you do not need to enable the mask.
	                                   */

    IR_LEARN_TIMEOUT_IRQ    = BIT(2),/**<
                                       The timeout interrupt is triggered when the tick count reaches the set timeout threshold.
                                       If you want to generate an interrupt, you need to enable the mask.
	                                   If you only want to query the corresponding flag, you do not need to enable the mask.
	                                   */
} ir_learn_irq_e;

/**
 * @brief ir_learn rx mode.
 */
typedef enum
{
    ANALOG_RX_MODE, /**< Note that the A0 chip version analog RX loses the first triggered edge. */
    DIGITAL_RX_MODE,
} ir_learn_rx_e;

/**
 * @brief ir_learn tx mode.
 */
typedef enum
{
    DIGITAL_TX_MODE,
    ANALOG_TX_MODE,
} ir_learn_tx_e;

/**
 * ir learn timeout tick trig level,
 * note: This tick value is converted to time according to the system clock
 * example: (system clock is 16M) TIMEOUT_TRIG_LEVEL_6 : 0x003fff -> (DEC:16383 / 16 =  1023 us)
 */
typedef enum{
    TICK_VALUE_3,           // tick value: 0x000003
    TICK_VALUE_15,          // tick value: 0x00000f
    TICK_VALUE_63,          // tick value: 0x00003f
    TICK_VALUE_255,         // tick value: 0x0000ff
    TICK_VALUE_1023,        // tick value: 0x0003ff
    TICK_VALUE_4095,        // tick value: 0x000fff
    TICK_VALUE_16383,       // tick value: 0x003fff
    TICK_VALUE_65535,       // tick value: 0x00ffff
    TICK_VALUE_262143,      // tick value: 0x03ffff
    TICK_VALUE_1048575,     // tick value: 0x0fffff
    TICK_VALUE_4194303,     // tick value: 0x3fffff
} ir_learn_timeout_tick_e;

/**
 * @brief ir_learn rx struct.
 */
typedef struct
{
    ir_learn_start_cnt_mode_e cnt_mode;
    ir_learn_rx_e rx_mode;
    ir_learn_timeout_tick_e timeout_cnt;
} ir_learn_rx_t;

/**
 * @brief ir_learn tx struct.
 */
typedef struct
{
    ir_learn_tx_e tx_mode;
} ir_learn_tx_t;

typedef enum{
    IR_RX_PA1  = GPIO_PA1,
    IR_RX_PA2  = GPIO_PA2,
    IR_RX_PB0  = GPIO_PB0,
    IR_RX_PC2  = GPIO_PC2,
    IR_RX_PD4  = GPIO_PD4,
    IR_RX_PE0  = GPIO_PE0,
    IR_RX_PE1  = GPIO_PE1,
}ir_learn_rx_pin_e;

/**
 * @brief      This function is used to set rx pin of the ir_learn
 * @param[in]  ir_rx_pin       - the rx pin of ir_learn
 * @param[in]  pull_type       - digital rx pin pull up or pull down.
 * @return     none
 */
void ir_learn_set_dig_rx_pin(ir_learn_rx_pin_e ir_rx_pin, GPIO_PullTypeDef pull_type);

/**
 * @brief      This function servers to set ir_learn tx mode.
 * @param[in]  tx_mode - ir_learn tx mode.
 * @return     none
 */
void ir_learn_tx_mode(ir_learn_tx_e tx_mode);

/**
 * @brief      This function servers to set ir_learn rx mode.
 * @param[in]  rx_mode - ir_learn rx mode.
 * @return     none
 */
void ir_learn_rx_mode(ir_learn_rx_e rx_mode);

/**
 * @brief      This function servers to initial ir_learn tx.
 * @param[in]  ir_learn_tx - the pointer of ir_learn_tx_t struct.
 * @return     none
 */
void ir_learn_tx_init(ir_learn_tx_t *ir_learn_tx);

/**
 * @brief      This function servers to initial ir_learn rx.
 * @param[in]  ir_learn_rx - the pointer of ir_learn_rx_t struct.
 * @return     none
 */
void ir_learn_rx_init(ir_learn_rx_t *ir_learn_rx);

/**
 *@brief       This function is used to enable the ir_learn function
 *@return      none
 */
static inline void ir_learn_en(void)
{
    reg_il_run |= BIT(0);
}

/**
 *@brief       This function is used to disable the ir_learn function
 *@return      none
 *@note        After the module is disable, the tick count value, high register, cycle register will be reset
 */
static inline void ir_learn_dis(void)
{
    reg_il_run &= ~BIT(0);
}


/**
 *@brief       This function serves to set the mask of ir learn irq
 *@param[in]   mask - variable of enum to select the mask source.
 *@return      none
 */
static inline void ir_learn_set_irq_mask(ir_learn_irq_e mask)
{
    BM_SET(reg_il_mask_0,mask);
}

/**
 *@brief        This function serves to clr the mask of ir learn irq
 *@param[in]    mask - variable of enum to select the mask source.
 *@return       none
 */
static inline void ir_learn_clr_irq_mask(ir_learn_irq_e mask)
{
    BM_CLR(reg_il_mask_0,mask);
}

/**
 * @brief     This function servers to get the ir learn interrupt status.
 * @param[in] status  - variable of enum to select the ir learn interrupt source.
 * @return    ir learn irq status.
 */
static inline unsigned char ir_learn_get_irq_status(ir_learn_irq_e status)
{
    return (reg_il_mask_1 & status);
}

/**
 * @brief     This function servers to clear the ir learn interrupt.
 * @param[in] status  - variable of enum to select the ir learn interrupt source.
 * @return    none.
 */
static inline void ir_learn_clr_irq_status(ir_learn_irq_e status)
{
    reg_il_mask_1 = status;
}

/**
 * @brief       This function servers to read the tick value of the high register
 * @param[in]   none
 * @return      The tick value of the high level duration stored in high register.
 * @note        data width - 24bit
 */
static inline unsigned int ir_learn_get_high(void)
{
    return reg_il_high_0;
}

/**
 * @brief       This function servers to read the tick value of the cycle register
 * @param[in]   none
 * @return      The tick value of the cycle in the cycle register.
 * @note        data width - 24bit
 */
static inline unsigned int ir_learn_get_cycle(void)
{
    return reg_il_cycle_0;
}

/**
 * @brief      This function servers to enable ir_learn analog tx.
 * @return     none
 * @note
 *             - If you disable the analog tx function before going to sleep, you need to enable the analog tx function after waking up from sleep.
 */
static inline void ir_learn_ana_tx_en(void)
{
    analog_write(0x11, (analog_read(0x11) | 0x80));
}

/**
 * @brief      This function servers to disable ir_learn analog tx.
 * @return     none
 * @note
 *             - If the analog tx function is enabled, you need to disable it before going to sleep to save power consumption.
 */
static inline void ir_learn_ana_tx_dis(void)
{
    analog_write(0x11, (analog_read(0x11) & 0x7f));
}

/**
 * @brief      This function servers to enable ir_learn analog rx.
 * @return     none
 * @note
 */
static inline void ir_learn_ana_rx_en(void)
{
    analog_write(0x0f, (analog_read(0x0f) | 0x08));
    if (g_chip_version != CHIP_VERSION_A0) {
    	analog_write(0x14, (analog_read(0x14) | 0x08));
        sleep_us(1);
        analog_write(0x14, (analog_read(0x14) & 0xf7));
    }
}

/**
 * @brief      This function servers to disable ir_learn analog rx.
 * @return     none
 * @note
 */
static inline void ir_learn_ana_rx_dis(void)
{
    analog_write(0x0f, (analog_read(0x0f) & 0xf7));
}


#endif /* IR_LEARN_H_ */

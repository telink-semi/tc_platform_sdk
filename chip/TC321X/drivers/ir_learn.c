/********************************************************************************************************
 * @file    ir_learn.c
 *
 * @brief   This is the source file for TC321X
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
#include "ir_learn.h"
#include "pwm.h"
/**
 * @brief      This function is used to set rx pin of the ir_learn
 * @param[in]  ir_rx_pin       - the rx pin of ir_learn
 * @param[in]  pull_type       - digital rx pin pull up or pull down.
 * @return     none
 */
void ir_learn_set_dig_rx_pin(ir_learn_rx_pin_e ir_rx_pin, GPIO_PullTypeDef pull_type)
{
    gpio_set_output_en(ir_rx_pin,0);
    gpio_set_input_en(ir_rx_pin,1);
    gpio_set_func(ir_rx_pin,DBG0_IO);
    gpio_setup_up_down_resistor((ir_learn_rx_pin_e)ir_rx_pin, pull_type);
}

/**
 * @brief      This function servers to set ir_learn tx mode.
 * @param[in]  tx_mode - ir_learn tx mode.
 * @return     none
 */
void ir_learn_tx_mode(ir_learn_tx_e tx_mode)
{
    switch (tx_mode)
    {
    case DIGITAL_TX_MODE:
        ir_learn_ana_tx_dis();
        pwm_set_pwm0_output_to_ana_ir_dis();
        break;
    case ANALOG_TX_MODE:
        ir_learn_ana_tx_en();
        pwm_set_pwm0_output_to_ana_ir_en();
        break;
    default:
        break;
    }
}


/**
 * @brief      This function servers to set ir_learn rx mode.
 * @param[in]  rx_mode - ir_learn rx mode.
 * @return     none
 */
void ir_learn_rx_mode(ir_learn_rx_e rx_mode)
{
    switch (rx_mode)
    {
    case DIGITAL_RX_MODE:
        ir_learn_ana_rx_dis();
        break;
    case ANALOG_RX_MODE:
        ir_learn_ana_rx_en();
        analog_write(0x14, 0x34);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function servers to initial ir_learn tx.
 * @param[in]  ir_learn_tx - the pointer of ir_learn_tx_t struct.
 * @return     none
 */
void ir_learn_tx_init(ir_learn_tx_t *ir_learn_tx)
{
    ir_learn_tx_mode(ir_learn_tx->tx_mode);
}

/**
 * @brief      This function servers to initial ir_learn rx.
 * @param[in]  ir_learn_rx - the pointer of ir_learn_rx_t struct.
 * @return     none
 */
void ir_learn_rx_init(ir_learn_rx_t *ir_learn_rx)
{
    unsigned short ir_dma_send_buff_rev[3]={0x0002, 0x0000, 0x8001};

    ir_learn_rx_mode(ir_learn_rx->rx_mode); /* TODO: A2 chip version need software trig timing to fix the first edge loss issue. */
    reg_il_cfg0 = MASK_VAL(FLD_IL_MODE, ir_learn_rx->cnt_mode, FLD_IL_INPUT_SEL, ir_learn_rx->rx_mode);
    reg_il_cfg1 = (reg_il_cfg1 & (~FLD_IL_TIMEOUT)) |MASK_VAL(FLD_IL_TIMEOUT, ir_learn_rx->timeout_cnt);

    if (g_chip_version == CHIP_VERSION_A0) {
        pwm_set_mode(PWM0_ID, PWM_IR_DMA_FIFO_MODE);
        pwm_set_cycle_and_duty(PWM0_ID, 100 * 24, 50 * 24);
        pwm_set_dma_address(&ir_dma_send_buff_rev);
        ir_learn_ana_tx_en();
        pwm_set_pwm0_output_to_ana_ir_en();
        pwm_start_dma_ir_sending();
        sleep_us(20);
        ir_learn_ana_tx_dis();
    }
}

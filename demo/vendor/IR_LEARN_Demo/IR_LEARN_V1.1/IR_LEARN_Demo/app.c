/********************************************************************************************************
 * @file    app.c
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

#define PWM_PIN         GPIO_PC0
#define GPIO_Type       PWM0

#define IRLEARN_RX_PIN  IR_RX_PA1
#define BUFF_DATA_LEN   16
volatile unsigned short ir_dma_send_buff[BUFF_DATA_LEN]={0};
volatile unsigned short il_wave_send_buff[BUFF_DATA_LEN]={0};
volatile unsigned int il_wave_receive_buff[BUFF_DATA_LEN]={0};

volatile unsigned int il_index = 0;
volatile unsigned int ir_irq_cnt = 0;

void pwm_send_init(void)
{
    pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);
    pwm_set_mode(PWM0_ID, PWM_IR_DMA_FIFO_MODE);
    pwm_set_cycle_and_duty(PWM0_ID, 100 * CLOCK_SYS_CLOCK_1US, 50 * CLOCK_SYS_CLOCK_1US);
    pwm_set_pwm0_shadow_cycle_and_duty(120 * CLOCK_SYS_CLOCK_1US, 80 * CLOCK_SYS_CLOCK_1US);

    unsigned char index=2;
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 2);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_NORMAL, 3);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_NORMAL, 1);

    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 2);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 1);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(0, PWM0_PULSE_SHADOW, 3);
    ir_dma_send_buff[index++]= pwm_config_dma_fifo_waveform(1, PWM0_PULSE_SHADOW, 2);

    /**
     * buffer[0] is cmp time, buffer[1] is cycle time, and so on.
     * Fill the actual data sent by PWM into the array ir_dma_send_buff.
     * The purpose of this is to facilitate comparison with the data received in the array g_il_wave_receive_buff.
     */
    il_wave_send_buff[0] = 50 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[1] = 100 * CLOCK_SYS_CLOCK_1US * 2;
    il_wave_send_buff[2] = 50 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[3] = 100 * CLOCK_SYS_CLOCK_1US * 3;
    il_wave_send_buff[4] = 50 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[5] = 100 * CLOCK_SYS_CLOCK_1US * 4;
    il_wave_send_buff[6] = 50 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[7] = 100 * CLOCK_SYS_CLOCK_1US;

    il_wave_send_buff[8] = 80 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[9] = 120 * CLOCK_SYS_CLOCK_1US * 2;
    il_wave_send_buff[10] = 80 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[11] = 120 * CLOCK_SYS_CLOCK_1US * 3;
    il_wave_send_buff[12] = 80 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[13] = 120 * CLOCK_SYS_CLOCK_1US * 4;
    il_wave_send_buff[14] = 80 * CLOCK_SYS_CLOCK_1US;
    il_wave_send_buff[15] = 120 * CLOCK_SYS_CLOCK_1US;

    unsigned int length = index*2 - 4;//The first four bytes are data length bytes, not included in the actual length to be sent
    unsigned char* buff = (unsigned char*)&ir_dma_send_buff[0];
    buff[0]= length&0xff;
    buff[1]= (length>>8)&0xff;
    buff[2]= (length>>16)&0xff;
    buff[3]= (length>>24)&0xff;
    pwm_set_dma_address(&ir_dma_send_buff);
}


void user_init(void)
{
    gpio_set_func(LED1, AS_GPIO);
    gpio_set_func(LED2, AS_GPIO);
    gpio_set_func(LED3, AS_GPIO);
    gpio_set_func(LED4, AS_GPIO);

    gpio_set_output_en(LED1, 1);
    gpio_set_output_en(LED2, 1);
    gpio_set_output_en(LED3, 1);
    gpio_set_output_en(LED4, 1);

    gpio_set_input_en(LED1, 0);
    gpio_set_input_en(LED2, 0);
    gpio_set_input_en(LED3, 0);
    gpio_set_input_en(LED4, 0);

#if (IR_LEARN_MODE == IR_DIGITAL_TX_MODE)
    /* pwm config. */
    pwm_send_init();
    gpio_set_func(PWM_PIN, GPIO_Type);

    /* interrupt config. */
    irq_set_mask(FLD_IRQ_SW_PWM_EN);
    pwm_set_interrupt_enable(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);
    irq_enable();

    /* dma config. */
    pwm_start_dma_ir_sending();

#elif (IR_LEARN_MODE == IR_DIGITAL_RX_MODE)
    /* irlearn config. */
    ir_learn_rx_t ir_learn_rx = {
        .cnt_mode      = RISING_EDGE_START_CNT,
        .rx_mode       = DIGITAL_RX_MODE,
        .timeout_cnt   = TICK_VALUE_65535,
    };
    ir_learn_rx_init(&ir_learn_rx);
    ir_learn_set_dig_rx_pin(IRLEARN_RX_PIN, PM_PIN_PULLDOWN_100K);
    ir_learn_en();

    /* interrupt config. */
    irq_set_mask(FLD_IRQ_IR_LEARN_EN);
    ir_learn_set_irq_mask(IR_LEARN_CYCLE_IRQ|IR_LEARN_TIMEOUT_IRQ);
    irq_enable();

#elif (IR_LEARN_MODE == IR_ANALOG_TX_MODE)
    /* pwm config. */
    pwm_send_init();
    gpio_set_func(PWM_PIN, GPIO_Type);

    /* irlearn config. */
    ir_learn_tx_t ir_learn_tx = {
        .tx_mode = ANALOG_TX_MODE,
    };
    ir_learn_tx_init(&ir_learn_tx);

    /* interrupt config. */
    pwm_set_interrupt_enable(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);
    irq_set_mask(FLD_IRQ_SW_PWM_EN);

    /* dma config. */
    pwm_start_dma_ir_sending();

#elif (IR_LEARN_MODE == IR_ANALOG_RX_MODE)
    /* irlearn config. */
    ir_learn_rx_t ir_learn_rx = {
        .cnt_mode      = RISING_EDGE_START_CNT,
        .rx_mode       = ANALOG_RX_MODE,
        .timeout_cnt   = TICK_VALUE_65535,
    };
    if (g_chip_version == CHIP_VERSION_A0) {
        ir_learn_rx.cnt_mode = FALLING_EDGE_START_CNT;
    }
    ir_learn_rx_init(&ir_learn_rx);
    ir_learn_en();
    /* interrupt config. */
    irq_set_mask(FLD_IRQ_IR_LEARN_EN);
    ir_learn_set_irq_mask(IR_LEARN_CYCLE_IRQ|IR_LEARN_TIMEOUT_IRQ);
    irq_enable();

#if (IR_LEARN_PM_FUNCTION_EN)
    blc_pm_select_internal_32k_crystal();
    ir_learn_ana_rx_dis();
    cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 500*CLOCK_16M_SYS_TIMER_CLK_1MS);
    ir_learn_ana_rx_en();
#endif /* IR_LEARN_PM_FUNCTION_EN */

#endif

}


/**
 * @brief       This function serves to handle the interrupt of MCU
 * @return      none
 */
#if (IR_LEARN_MODE == IR_DIGITAL_RX_MODE)||(IR_LEARN_MODE == IR_ANALOG_RX_MODE)
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
    if( ir_learn_get_irq_status(IR_LEARN_CYCLE_IRQ))
    {
        ir_irq_cnt++;
        il_wave_receive_buff[il_index] = ir_learn_get_high();
        il_wave_receive_buff[il_index+1] = ir_learn_get_cycle();
        ir_learn_clr_irq_status(IR_LEARN_CYCLE_IRQ);
        ir_learn_clr_irq_status(IR_LEARN_HIGH_IRQ);
        gpio_toggle(LED3);
        il_index += 2;
    }
    if( ir_learn_get_irq_status(IR_LEARN_TIMEOUT_IRQ))
    {
    	/**
    	 * When a timeout interrupt occurs, there is still the last piece of data in the high register.
    	 * Therefore, in the timeout interrupt, we check the high interrupt to retrieve the last value from the high register.
    	 */
        if(ir_learn_get_irq_status(IR_LEARN_HIGH_IRQ))
        {
            il_wave_receive_buff[il_index] = ir_learn_get_high();
            ir_learn_clr_irq_status(IR_LEARN_HIGH_IRQ);
            gpio_toggle(LED4);
            il_index += 1;
        }
        ir_learn_clr_irq_status(IR_LEARN_TIMEOUT_IRQ);
    }
}
#elif (IR_LEARN_MODE == IR_DIGITAL_TX_MODE)||(IR_LEARN_MODE == IR_ANALOG_TX_MODE)
_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
    if(pwm_get_interrupt_status(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE)){
        pwm_clear_interrupt_status(PWM_IRQ_PWM0_IR_DMA_FIFO_DONE);
        gpio_toggle(LED2);
    }
}
#endif

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////

void main_loop(void)
{
    sleep_ms(500);
    gpio_toggle(LED1);
}

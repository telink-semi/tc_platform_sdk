/********************************************************************************************************
 * @file    app.c
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

volatile unsigned int cur_32k_tick[200];
volatile unsigned int cur_32k_tick_step[200];
volatile unsigned int cur_32k_tick_step_err;
volatile unsigned int stimer_track_32k_value;
volatile unsigned int stimer_tick_low3bit;
volatile unsigned int cur_32k_track[200];
volatile unsigned int cur_32k_tick_track_err;
volatile unsigned int stimer_cnt = 0;
volatile unsigned int stimer_irq_cnt = 0;

void user_init(void)
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1, AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1, 0); //disable input
	gpio_write(LED1, 0); //LED OFF

#if !((MCU_CORE_TC1211)||(MCU_CORE_TC123X))
	gpio_set_func(LED2, AS_GPIO);
	gpio_set_output_en(LED2, 1); //enable output
	gpio_set_input_en(LED2, 0); //disable input
	gpio_write(LED2, 0); //LED OFF

	gpio_set_func(LED3, AS_GPIO);
	gpio_set_output_en(LED3, 1); //enable output
	gpio_set_input_en(LED3, 0); //disable input
	gpio_write(LED3, 0); //LED OFF

	gpio_set_func(LED4, AS_GPIO);
	gpio_set_output_en(LED4, 1); //enable output
	gpio_set_input_en(LED4, 0); //disable input
	gpio_write(LED4, 0); //LED OFF
#endif

#if (STIMER_MODE == STIMER_IRQ)
	stimer_set_capture_tick(clock_time() + 100*CLOCK_SYS_CLOCK_1MS);
    stimer_set_irq_mask(FLD_SYSTEM_IRQ_MASK);
#if(MCU_CORE_TC321X)
    irq_set_mask(FLD_IRQ_SYSTEM_TIMER);
#else
    irq_set_mask(FLD_IRQ_SYSTEM_TIMER_EDG_EN);
#endif
    irq_enable();

#elif (STIMER_MODE == STIMER_GET_32K_TICK)
    clock_32k_init(CLK_32K_RC);
    rc_32k_cal();

#elif (STIMER_MODE == STIMER_SET_32K_TICK_LVL)
    cpu_set_32k_tick(pm_get_32k_tick() + 32000); //1s
    pm_set_wakeup_src(PM_WAKEUP_TIMER);
    pm_clr_irq_status(WAKEUP_STATUS_ALL);
    irq_set_mask(FLD_IRQ_PM_EN);
    irq_enable();

#elif (STIMER_MODE == STIMER_TRACK_32K_TICK)
    clock_32k_init(CLK_32K_RC);
    rc_32k_cal();
    stimer_set_32k_track_cnt(STIMER_TRACK_32KCNT_16);
    stimer_track_32k_value = CLOCK_SYS_CLOCK_1MS / 32 * g_track_32kcnt;
    sleep_ms(1000);
    stimer_set_irq_mask(FLD_SYSTEM_IRQ_32K_CAL_MASK);
#if !(MCU_CORE_TC321X)
    irq_set_mask(FLD_IRQ_SYSTEM_TIMER_LVL_EN);
#endif
    irq_enable();
#elif (STIMER_MODE == STIMER_TIMEOUT_IRQ)
	stimer_set_capture_tick(clock_time() + 1000*CLOCK_SYS_CLOCK_1MS);
    stimer_set_irq_mask(FLD_SYSTEM_IRQ_MASK);
#if(MCU_CORE_TC321X)
#else
    irq_set_mask(FLD_IRQ_SYSTEM_TIMER_EDG_EN);
#endif
    irq_enable();
#endif

}

void main_loop(void)
{
#if (STIMER_MODE == STIMER_DELAY)
    sleep_ms(500);
    gpio_toggle(LED1);
#if !((MCU_CORE_TC1211)||(MCU_CORE_TC123X))
    gpio_toggle(LED2);
    gpio_toggle(LED3);
    gpio_toggle(LED4);
#endif

#elif (STIMER_MODE == STIMER_GET_32K_TICK)
    cur_32k_tick[0] = pm_get_32k_tick();
    for (int i = 1; i < 200; i++) {
        sleep_ms(100);
        cur_32k_tick[i]      = pm_get_32k_tick();
        cur_32k_tick_step[i] = cur_32k_tick[i] - cur_32k_tick[i - 1];
        if ((cur_32k_tick_step[i] < 0xc7f) || (cur_32k_tick_step[i] > 0xc83)) //0xc81,500ppm
        {
            cur_32k_tick_step_err = i;
            gpio_write(LED1, 1);
            while (1)
                ;
        }
    }

#elif (STIMER_MODE == STIMER_GET_TICK)
    if ((clock_time() & (0x07)) != 0x00) {
        stimer_tick_low3bit = 1;
        gpio_write(LED1, 1);
        while (1)
            ;
    }
#elif (STIMER_MODE == STIMER_TIMEOUT_IRQ)
    sleep_ms(600);
    stimer_set_capture_tick(clock_time() - 10*CLOCK_SYS_CLOCK_1MS);
#endif

}

#if (STIMER_MODE == STIMER_IRQ)
_attribute_ram_code_sec_ void irq_handler(void)
{
	if(stimer_get_irq_status())
    {
    	stimer_irq_cnt++;
        gpio_toggle(LED1);
        stimer_clr_irq_status();
        stimer_set_capture_tick(clock_time() + 100*CLOCK_SYS_CLOCK_1MS);
    }
}

#elif (STIMER_MODE == STIMER_SET_32K_TICK_LVL)
_attribute_ram_code_sec_ void irq_handler(void)
{
    if(irq_get_src() & FLD_IRQ_PM_EN)
    {
    	if(pm_get_wakeup_src()&PM_WAKEUP_TIMER)
    	{
			stimer_irq_cnt++;
			gpio_toggle(LED1);
			pm_clr_irq_status(PM_WAKEUP_TIMER);
			cpu_set_32k_tick(pm_get_32k_tick() + 32000);//1s
    	}
    	irq_clr_sel_src(FLD_IRQ_PM_EN);
    }
}

#elif (STIMER_MODE == STIMER_TRACK_32K_TICK)
_attribute_ram_code_sec_ void irq_handler(void)
{
    if (stimer_get_lev_irq_status(FLD_SYSTEM_32K_CAL_IRQ))
    {
        gpio_toggle(LED1);
        stimer_clr_lev_irq_status(FLD_SYSTEM_32K_CAL_IRQ);
        if (stimer_cnt < 200) {
            cur_32k_track[stimer_cnt] = stimer_get_tracking_32k_value();
            if ((cur_32k_track[stimer_cnt] < (stimer_track_32k_value - 0x06)) || (cur_32k_track[stimer_cnt] > (stimer_track_32k_value + 0x06))) //500ppm
            {
                cur_32k_tick_track_err = stimer_cnt;
                gpio_write(LED1, 1);
            }
        }
        stimer_cnt++;
    }
}
#elif (STIMER_MODE == STIMER_TIMEOUT_IRQ)
_attribute_ram_code_sec_ void irq_handler(void)
{
	if(stimer_get_irq_status())
    {
    	stimer_irq_cnt++;
        gpio_toggle(LED1);
        stimer_clr_irq_status();
        stimer_set_capture_tick(clock_time() + 1000*CLOCK_SYS_CLOCK_1MS);
    }
}
#else
_attribute_ram_code_sec_ void irq_handler(void)
{

}

#endif

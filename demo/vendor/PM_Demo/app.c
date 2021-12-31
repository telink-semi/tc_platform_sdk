/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "app_config.h"

#define WAKEUP_PAD						GPIO_PA0
#define CURRENT_TEST	     			1
#define CRC_OK			     			1
#define	MDEC_MATCH_VALUE				0x02
unsigned char 	dat[5]={0};

unsigned short read_data_time_sustain[24] = {0};	//the size of the array can be set according to the actual situation.

void user_init()
{

	sleep_ms(1000);
#if CURRENT_TEST
	gpio_shutdown(GPIO_ALL);

#else
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On
#endif

#if(PM_MODE==IDLE_TIMER_WAKEUP)



#elif(PM_MODE==SUSPEND_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

#elif(PM_MODE==SUSPEND_MDEC_WAKEUP)
	mdec_init(GPIO_PB7_MDEC);
	mdec_reset();
	cpu_set_mdec_value_wakeup(MDEC_MATCH_VALUE);
	cpu_sleep_wakeup(SUSPEND_MODE , PM_WAKEUP_MDEC,0);

#elif(PM_MODE==SUSPEND_CORE_WAKEUP)
	usb_set_pin_en();
	pm_set_suspend_power_cfg(PM_POWER_USB, 1);

#elif(PM_MODE==DEEP_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_PAD, 0);

#elif(PM_MODE==DEEP_32K_RC_WAKEUP||PM_MODE==DEEP_32K_XTAL_WAKEUP)

    cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_TIMER,(clock_time() + 1000*CLOCK_16M_SYS_TIMER_CLK_1MS));

#elif(PM_MODE==DEEP_MDEC_WAKEUP)
	if((analog_read(0x44) & WAKEUP_STATUS_MDEC) && (CRC_OK == mdec_read_dat(dat)))
	{
		gpio_toggle(LED2);
		sleep_ms(2000);
	}
	mdec_init(GPIO_PB7_MDEC);
	mdec_reset();
	cpu_set_mdec_value_wakeup(MDEC_MATCH_VALUE);
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_MDEC,0);

#elif(PM_MODE==DEEP_DEBOUNCE_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	pm_set_debounce_count(0x0f);
	if(pm_get_wakeup_src() & WAKEUP_STATUS_DEBOUNCE){
		unsigned int k = 0;
		do{
			if(!pm_debounce_fifo_empty()){
				read_data_time_sustain[k] = pm_debounce_read_data();
				k++;
			}
		}while(pm_debounce_data_overflow() == 0);
	}

	cpu_set_gpio_wakeup(DEBOUNCE_WAKEUP_PA1, Level_High, 1);	//Set the wake-up pin, wake-up polarity and whether to enable
	gpio_setup_up_down_resistor(DEBOUNCE_WAKEUP_PA1, PM_PIN_PULLDOWN_100K);	//Set the pull-up and pull-up mode of the wake-up pin
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_DEBOUNCE, 0);	//Put the chip into low power mode

#elif(PM_MODE==DEEP_RET_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_PAD, 0);

#elif(PM_MODE==DEEP_RET_32K_RC_WAKEUP||PM_MODE==DEEP_RET_32K_XTAL_WAKEUP)

	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_TIMER,(clock_time() + 4000*CLOCK_16M_SYS_TIMER_CLK_1MS));

#elif(PM_MODE==DEEP_RET_MDEC_WAKEUP)
	if((analog_read(0x44) & WAKEUP_STATUS_MDEC) && (CRC_OK == mdec_read_dat(dat)))
	{
		gpio_toggle(LED2);
		sleep_ms(2000);
	}
	mdec_init(GPIO_PB7_MDEC);
	mdec_reset();
	cpu_set_mdec_value_wakeup(MDEC_MATCH_VALUE);
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_MDEC,0);

#elif(PM_MODE==DEEP_RET_DEBOUNCE_WAKEUP)

	pm_set_debounce_count(0x0f);
	if(pm_get_wakeup_src() & WAKEUP_STATUS_DEBOUNCE){
		unsigned int k = 0;
		do{
			if(!pm_debounce_fifo_empty()){
				read_data_time_sustain[k] = pm_debounce_read_data();
				k++;
			}
		}while(pm_debounce_data_overflow() == 0);
	}

	cpu_set_gpio_wakeup(DEBOUNCE_WAKEUP_PA1, Level_High, 1);	//Set the wake-up pin, wake-up polarity and whether to enable
	gpio_setup_up_down_resistor(DEBOUNCE_WAKEUP_PA1, PM_PIN_PULLDOWN_100K);	//Set the pull-up and pull-up mode of the wake-up pin
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_DEBOUNCE, 0);	//Put the chip into low power mode

#elif(PM_MODE==SHUTDOWN_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

	cpu_sleep_wakeup(SHUTDOWN_MODE , PM_WAKEUP_SHUTDOWN, 0);

#endif

}

unsigned int tick_suspend_interval = 0;
/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{

#if(PM_MODE==IDLE_TIMER_WAKEUP)

	cpu_stall_wakeup_by_timer0(3000*CLOCK_SYS_CLOCK_1MS);

#elif(PM_MODE==SUSPEND_PAD_WAKEUP)

	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD, 0);

#elif(PM_MODE==SUSPEND_32K_RC_WAKEUP||PM_MODE==SUSPEND_32K_XTAL_WAKEUP)

	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 1000*CLOCK_16M_SYS_TIMER_CLK_1MS);

#elif(PM_MODE==SUSPEND_MDEC_WAKEUP)
	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_MDEC, 0);
	if((CRC_OK == mdec_read_dat(dat)))//(analog_read(0x44) & WAKEUP_STATUS_MDEC) &&
	{
		gpio_toggle(LED2);
		sleep_ms(2000);
	}
	mdec_reset();

#elif(PM_MODE==SUSPEND_CORE_WAKEUP)
	if(clock_time_exceed (tick_suspend_interval, 300000))
	{
		gpio_toggle(LED2);
		cpu_sleep_wakeup (SUSPEND_MODE, PM_WAKEUP_CORE, 0);
		tick_suspend_interval = clock_time()|1;
	}

#endif

#if CURRENT_TEST
	sleep_ms(1000);
#else
	gpio_write(LED1, 1);
	sleep_ms(100);
	gpio_write(LED1, 0);
	sleep_ms(100);

#endif
}




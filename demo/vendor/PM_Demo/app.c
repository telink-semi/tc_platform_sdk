/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#define WAKEUP_PAD						GPIO_PA0
#define CURRENT_TEST	     			1
#define CRC_OK			     			1
#define	MDEC_MATCH_VALUE				0x02
unsigned char 	dat[5]={0};

unsigned short read_data_time_sustain[24] = {0};	//the size of the array can be set according to the actual situation.

#define TX								1
#define RX								2
#define RF_TX_RX_MODE					TX
#if (MCU_CORE_B85)
#define RF_POWER						RF_POWER_P10p46dBm
#elif(MCU_CORE_B87)
#define RF_POWER						RF_POWER_P11p26dBm
#elif(MCU_CORE_B89)
#define RF_POWER						RF_POWER_INDEX_P4p98dBm
#elif(MCU_CORE_B80)
#define RF_POWER						RF_POWER_P11p46dBm
#endif
#define RF_FREQ							17
#define ACCESS_CODE						0x29417671
#define TX_INTERVAL_MS					1
volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;
unsigned char  rx_packet[64]  __attribute__ ((aligned (4)));
unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) = {0x23,0x00,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

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
	gpio_set_func(LED2,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2,0);			//disable input
	gpio_write(LED2, 0);              	//LED On
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

    cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_TIMER, (clock_time() + 1000*CLOCK_16M_SYS_TIMER_CLK_1MS));

#elif(PM_MODE==DEEP_LONG_32K_RC_WAKEUP||PM_MODE==DEEP_LONG_32K_XTAL_WAKEUP)

    cpu_long_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_TIMER, 500*CLOCK_32K_SYS_TIMER_CLK_1MS);

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

	unsigned short front_level_flag = 0;
	unsigned short front_time_data = 0;

	unsigned short next_fifo_data = 0;
	unsigned short next_level_flag = 0;
	unsigned short next_time_data = 0;

	unsigned short total_data_time = 0;

	pm_set_debounce_count(0x10);	//set debounce_count to determine the width of filter.
	if(pm_get_wakeup_src() & WAKEUP_STATUS_DEBOUNCE){	//determine whether the debounce is awakened.
		unsigned int k = 0;
		do{
			if(!pm_debounce_fifo_empty()){	//after determining that debounce is awakened, determine whether the FIFO is empty, that is, whether there is any readable data.

				//the data read in fifo is processed. If high level (low level) is continuously read,
				//it means that this series of data all belong to the same high level (low level), and several times need to be added together.
				if(k == 0){
					read_data_time_sustain[k] = pm_debounce_read_data();
					k++;
				}else{
					front_time_data = read_data_time_sustain[k-1] & 0x7fff;
					front_level_flag = read_data_time_sustain[k-1] & 0x8000;

					next_fifo_data = pm_debounce_read_data();
					next_time_data = next_fifo_data & 0x7fff;
					next_level_flag = next_fifo_data & 0x8000;

					if(front_level_flag == next_level_flag){
						total_data_time = front_time_data + next_time_data;
						read_data_time_sustain[k-1] = total_data_time + front_level_flag;
					}else{
						read_data_time_sustain[k] = next_fifo_data;
						k++;
					}
				}
			}
		}while(pm_debounce_data_overflow() == 0);	//determine whether the read value overflows.
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

	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K, PM_WAKEUP_TIMER, (clock_time() + 4000*CLOCK_16M_SYS_TIMER_CLK_1MS));

#elif(PM_MODE==DEEP_RET_LONG_32K_RC_WAKEUP||PM_MODE==DEEP_RET_LONG_32K_XTAL_WAKEUP)

    cpu_long_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K, PM_WAKEUP_TIMER, 500*CLOCK_32K_SYS_TIMER_CLK_1MS);

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

	unsigned short front_level_flag = 0;
	unsigned short front_time_data = 0;

	unsigned short next_fifo_data = 0;
	unsigned short next_level_flag = 0;
	unsigned short next_time_data = 0;

	unsigned short total_data_time = 0;

	pm_set_debounce_count(0x10);	//set debounce_count to determine the width of filter.
	if(pm_get_wakeup_src() & WAKEUP_STATUS_DEBOUNCE){	//determine whether the debounce is awakened.
		unsigned int k = 0;
		do{
			if(!pm_debounce_fifo_empty()){	//after determining that debounce is awakened, determine whether the FIFO is empty, that is, whether there is any readable data.

				//the data read in fifo is processed. If high level (low level) is continuously read,
				//it means that this series of data all belong to the same high level (low level), and several times need to be added together.
				if(k == 0){	//store the first read data in read_data_time_sustain[0].
					read_data_time_sustain[k] = pm_debounce_read_data();
					k++;
				}else{
					//extract the level data and time data of the previous data.
					front_time_data = read_data_time_sustain[k-1] & 0x7fff;
					front_level_flag = read_data_time_sustain[k-1] & 0x8000;

					//read the latter data, and extract the level data and time data of the latter data.
					next_fifo_data = pm_debounce_read_data();
					next_time_data = next_fifo_data & 0x7fff;
					next_level_flag = next_fifo_data & 0x8000;

					if(front_level_flag == next_level_flag){	//if the two data are at the same level, the sum of the two time values is stored in read_data_time_sustain[k-1].
						total_data_time = front_time_data + next_time_data;
						read_data_time_sustain[k-1] = total_data_time + front_level_flag;
					}else{	//if the two data levels are not the same, the latter time data is stored in read_data_time_sustain[k].
						read_data_time_sustain[k] = next_fifo_data;
						k++;
					}
				}
			}
		}while(pm_debounce_data_overflow() == 0);	//determine whether the read value overflows.
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

#if(PM_MODE == IDLE_TIMER_WAKEUP)
	//After stall wakes up, it does not enter the interrupt, but only continues to execute, and the interrupt mask needs to be cleared.
	timer0_set_mode(TIMER_MODE_SYSCLK, 0, 2*CLOCK_SYS_CLOCK_1S);
	timer_clr_irq_mask(TMR_STA_TMR0);		//Clear the interrupt mask.
	timer_start(TIMER0);
	cpu_stall_wakeup(FLD_IRQ_TMR0_EN);
	timer_clear_interrupt_status(FLD_TMR_STA_TMR0);	//Clear the interrupt status.
	timer_stop(TIMER0);

#elif(PM_MODE == IDLE_STIMER_WAKEUP)
	stimer_set_capture_tick(clock_time() + CLOCK_16M_SYS_TIMER_CLK_1S);
	stimer_enable();
	cpu_stall_wakeup(FLD_IRQ_SYSTEM_TIMER);
	stimer_clr_irq_status();				//Clear the interrupt status.
	stimer_disable();

#elif(PM_MODE == IDLE_RF_WAKEUP)
#if (MCU_CORE_B89 || MCU_CORE_B80)
	rf_mode_init();
	rf_set_ble_1M_mode();
#endif
	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off_auto_mode();
	rf_set_ble_channel(RF_FREQ);
    irq_disable();
    irq_clr_src();
    rf_irq_disable(FLD_RF_IRQ_ALL);
    rf_irq_enable(FLD_RF_IRQ_TX|FLD_RF_IRQ_RX|FLD_RF_IRQ_RX_TIMEOUT);
    rf_access_code_comm(ACCESS_CODE);

#if(RF_TX_RX_MODE == TX)
	sleep_ms(1);
	rf_start_stx (ble_tx_packet, clock_time() + 16*1000*TX_INTERVAL_MS);
	cpu_stall_wakeup(FLD_IRQ_ZB_RT_EN);
	rf_tx_finish_clear_flag();				//Clear the interrupt status.
	sleep_ms(500);
#elif(RF_TX_RX_MODE == RX)
	rf_rx_buffer_set(rx_packet,64, 0);
	rf_start_srx(clock_time() + 100*16);
	cpu_stall_wakeup(FLD_IRQ_ZB_RT_EN);
	rf_rx_finish_clear_flag();				//Clear the interrupt status.
	sleep_ms(1500);
#endif

#elif(PM_MODE == IDLE_PAD_WAKEUP)
	//After stall wakes up, the advanced interrupt continues to execute, and the interrupt mask and total interrupt need to be opened.
	gpio_set_func(GPIO_PB1 ,AS_GPIO);
	gpio_set_output_en(GPIO_PB1, 0);
	gpio_set_input_en(GPIO_PB1 ,1);
	gpio_setup_up_down_resistor(GPIO_PB1, PM_PIN_PULLUP_10K);
	gpio_set_interrupt(GPIO_PB1, POL_FALLING);
	irq_enable();		//Turn on the total interrupt.
	cpu_stall_wakeup(FLD_IRQ_GPIO_EN);
	gpio_clr_irq_status(GPIO_IRQ_MASK_GPIO);//Clear the interrupt status.

#elif(PM_MODE==SUSPEND_PAD_WAKEUP)

	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD, 0);

#elif(PM_MODE==SUSPEND_32K_RC_WAKEUP||PM_MODE==SUSPEND_32K_XTAL_WAKEUP)

	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 500*CLOCK_16M_SYS_TIMER_CLK_1MS);

#elif(PM_MODE==SUSPEND_LONG_32K_RC_WAKEUP||PM_MODE==SUSPEND_LONG_32K_XTAL_WAKEUP)

	cpu_long_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, 500*CLOCK_32K_SYS_TIMER_CLK_1MS);

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




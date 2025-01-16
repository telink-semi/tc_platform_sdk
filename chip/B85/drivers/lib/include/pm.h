/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for B85
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

#include "bsp.h"
#include "gpio.h"
#include "flash.h"
#include "clock.h"
#define PM_DEBUG							0

#if(PM_DEBUG)
volatile unsigned char debug_pm_info;
volatile unsigned int ana_32k_tick;
#endif

#define PM_LONG_SUSPEND_EN					1

#ifndef PM_TIM_RECOVER_MODE
#define PM_TIM_RECOVER_MODE			    	0 //this function only support for interface of cpu_sleep_wakeup_32k_rc and cpu_sleep_wakeup_32k_xtal
#endif

#define PM_XTAL_DELAY_DURATION      		500
//#define EARLYWAKEUP_TIME_US_DEEP    		1240
//#define EARLYWAKEUP_TIME_US_SUSPEND 		1190
//#define EMPTYRUN_TIME_US       	    	1500

//#define PM_DCDC_DELAY_DURATION      		1000

#define EARLYWAKEUP_TIME_MS_DEEP	2
#define EARLYWAKEUP_TIME_MS_SUSPEND	1
#define	tick_32k_tick_per_ms		32
#define PM_EMPTYRUN_TIME_MS			2



#define PM_LONG_SLEEP_WAKEUP_EN			    0 //if user need to make MCU sleep for a long time that is more than 268s, this macro need to be enabled and use "pm_long_sleep_wakeup" function
#define SYS_CLK_48MRC_EN				    0 //if user take 48M RC as system clock, this macro need to be enabled

/**
 * @brief analog register below can store information when MCU in deepsleep mode
 * 	      store your information in these ana_regs before deepsleep by calling analog_write function
 * 	      when MCU wakeup from deepsleep, read the information by by calling analog_read function
 * 	      Reset these analog registers only by power cycle
 */

#define DEEP_ANA_REG0                       0x3a //initial value =0x00 [Bit1] The crystal oscillator failed to start normally.The customer cannot change!
#define DEEP_ANA_REG1                       0x3b //initial value =0x00
#define DEEP_ANA_REG2                       0x3c //initial value =0x00

/**
 * @brief these analog register can store data in deepsleep mode or deepsleep with SRAM retention mode.
 * 	      Reset these analog registers by watchdog, chip reset, RESET Pin, power cycle
 */

#define DEEP_ANA_REG6                       0x35 //initial value =0x00
#define DEEP_ANA_REG7                       0x36 //initial value =0x00
#define DEEP_ANA_REG8                       0x37 //initial value =0x00
#define DEEP_ANA_REG9                       0x38 //initial value =0x00
#define DEEP_ANA_REG10                      0x39 //initial value =0x00


#define SYS_NEED_REINIT_EXT32K			    BIT(0)


//ana3c system used, user can not use
#define SYS_DEEP_ANA_REG 					DEEP_ANA_REG2
#define WAKEUP_STATUS_TIMER_CORE     	    ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD		        ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

//API changed, compatible for the old SDK version user, define this macro.
#define cpu_long_sleep_wakeup				cpu_long_sleep_wakeup_32k_rc

/**
 * @brief sleep mode.
 * @note	After entering suspend mode,the registers of linklayer and modem return to default values,so the
 * 			functions in rf.h need to be reconfigured after suspend. (The registers with base address 0x400,
 * 			0xf00,0x1200 need to be reconfigured.)
 */
typedef enum {
	//available mode for customer
	SUSPEND_MODE						= 0,

	DEEPSLEEP_MODE						= 0x80,
	DEEPSLEEP_MODE_RET_SRAM_LOW8K		= 0x61,  //for boot from sram
	DEEPSLEEP_MODE_RET_SRAM_LOW16K  	= 0x43,  //for boot from sram
	DEEPSLEEP_MODE_RET_SRAM_LOW32K  	= 0x07,  //for boot from sram

	SHUTDOWN_MODE						= 0xFF,

	//not available mode
	DEEPSLEEP_RETENTION_FLAG			= 0x7F,
}SleepMode_TypeDef;


/**
 * @brief   wakeup source
 */

typedef enum {
	 //available wake-up source for customer
	 PM_WAKEUP_PAD   		= BIT(4),
	 PM_WAKEUP_CORE  		= BIT(5),
	 PM_WAKEUP_TIMER 		= BIT(6),
	 PM_WAKEUP_COMPARATOR 	= BIT(7), /**<
	 	 	 	 	 	 	 	 	 	There are two things to note when using LPC wake up:
										1.After the LPC is configured, you need to wait 100 microseconds before you can go to sleep because the LPC need 1-2 32k tick to calculate the result.
										  If you enter the sleep function at this time, you may not be able to sleep normally because the data in the result register is abnormal.

										2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV, otherwise can not enter sleep normally, crash occurs.
	  	  	  	  	  	  	  	  	  	 */
	 //not available wake-up source for customer
	 PM_TIM_RECOVER_START   = BIT(14),
	 PM_TIM_RECOVER_END     = BIT(15),
}SleepWakeupSrc_TypeDef;

/**
 * @brief   wakeup status
 */

enum {
	 WAKEUP_STATUS_COMPARATOR  		= BIT(0),
	 WAKEUP_STATUS_TIMER  			= BIT(1),
	 WAKEUP_STATUS_CORE 			= BIT(2),
	 WAKEUP_STATUS_PAD    			= BIT(3),
     WAKEUP_STATUS_WD    			= BIT(6),
     
	 WAKEUP_STATUS_INUSE_ALL        = 0x0f,

	 STATUS_GPIO_ERR_NO_ENTER_PM  	= BIT(8),/**<Bit8 is used to determine whether the wake source is normal.*/
	 
	 STATUS_ENTER_SUSPEND  			= BIT(30),
};

/**
 * @brief	pm power whether to power down definition
 */
typedef enum {
	 PM_POWER_BASEBAND  	= BIT(0),	//whether to power on the BASEBAND before suspend.
	 PM_POWER_USB  			= BIT(1),	//whether to power on the USB before suspend.
	 PM_POWER_AUDIO  		= BIT(2),	//whether to power on the AUDIO before suspend.
}pm_suspend_power_cfg_e;

/**
 * @brief   deepsleep wakeup by external xtal
 */
typedef struct{
	unsigned char ext_cap_en;    //24xtal  cap
	unsigned char pad32k_en;
	unsigned char pm_enter_en;
}misc_para_t;

extern misc_para_t 				blt_miscParam;

/**
 * @brief   deepsleep wakeup status
 */
typedef struct{
	unsigned char is_deepretn_back;
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;
}pm_para_t;

extern pm_para_t	pmParam;

/**
 * @brief	early wakeup time
 */
typedef struct {
	unsigned short  suspend;	/*< suspend_early_wakeup_time_us >*/
	unsigned short  deep_ret;	/*< deep_ret_early_wakeup_time_us >*/
	unsigned short  deep;		/*< deep_early_wakeup_time_us >*/
	unsigned short  min;		/*< sleep_min_time_us >*/
}pm_early_wakeup_time_us_s;

extern volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;

/**
 * @brief	hardware delay time
 */
typedef struct {
	unsigned short  deep_r_delay_us ;			/**< hardware delay time, deep_r_delay_us = (deep_r_delay_cycle) * 1/16k */
	unsigned short  suspend_ret_r_delay_us ;		/**< hardware delay time, suspend_ret_r_delay_us = (suspend_ret_r_delay_cycle) * 1/16k */
}pm_r_delay_us_s;

extern volatile pm_r_delay_us_s g_pm_r_delay_us;


#if (PM_TIM_RECOVER_MODE)

typedef struct{
	unsigned int   tick_sysClk;
	unsigned int   tick_32k;
	unsigned char  recover_flag;
}pm_tim_recover_t;

extern pm_tim_recover_t			pm_timRecover;
#endif


typedef int (*suspend_handler_t)(void);
extern  suspend_handler_t 		 func_before_suspend;

void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func );


/**
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_deepRetentionWakeup(void)
{
	return pmParam.is_deepretn_back;
}

/**
 * @brief      This function serves to determine whether mcu is waked up by pad.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_deepPadWakeup(void)
{
	return pmParam.is_pad_wakeup;
}

/**
 * @brief      This function serves to get the source of wake-up.
 * @param[in]  none.
 * @return     wakeup source.
 */
static inline int pm_get_wakeup_src(void)
{
	return pmParam.wakeup_src;
}

/**
 * @brief		This function serves to set baseband/usb/audio power on/off before suspend sleep,If power
 * 				on this module,the suspend current will increase;power down this module will save current,
 * 				but you need to re-init this module after suspend wakeup.All power down default to save
 * 				current.
 * @param[in]	value - whether to power on/off the baseband/usb/audio.
 * @param[in]	on_off - select power on or off. 0 - power off, other value - power on; .
 * @return		none.
 */
void pm_set_suspend_power_cfg(pm_suspend_power_cfg_e value, unsigned char on_off);

/**
 * @brief   This function serves to wake up cpu from stall mode by timer0.
 * @param   tick - capture value of timer0.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer0(unsigned int tick);

/**
 * @brief   This function serves to wake up cpu from stall mode by timer1.
 * @param   tick - capture value of timer1.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer1(unsigned int tick);

/**
 * @brief   This function serves to wake up cpu from stall mode by timer2.
 * @param   tick - capture value of timer2.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer2(unsigned int tick);

/**
 * @brief   This function serves to wake up cpu from stall mode by timer1 or RF TX done irq.
 * @param   WakeupSrc  - timer1.
 * @param   IntervalUs - capture value of timer1.
 * @param   sysclktick - tick value of per us based on system clock.
 * @return  none.
 */
unsigned int cpu_stall(int WakeupSrc, unsigned int IntervalUs,unsigned int sysclktick);

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  pin - the pin needs to be configured as wakeup pin
 * @param[in]  pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup)
 * @param[in]  en  - enable or disable the wakeup function for the pan pin(1: Enable, 0: Disable)
 * @return     none
 */
void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en);


/**
 * @brief   This function serves to reboot chip.
 * @param   none.
 * @return  none.
 */

void start_reboot(void);

/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  tick of 32k .
 */

extern unsigned int pm_get_32k_tick(void);

/**
 * @brief   This function serves to initialize MCU
 * @param   none
 * @return  none
 * @note	When this function called after power on or deep sleep wakeup, it will cost about 6~7ms for perform 32k RC calibration. 
 * 			If do not want this logic, you can check the usage and precautions of cpu_wakeup_init_calib_32k_rc_cfg().
 */
_attribute_ram_code_sec_noinline_ void cpu_wakeup_init(void);

/**
 * @brief 	  This function performs to configure whether to calibrate the 32k rc in the cpu_wakeup_init() when power-on or wakeup from deep sleep mode.If wakeup from deep retention sleep mode will not calibrate.
 * @param[in] calib_flag - Choose whether to calibrate the 32k rc or not.
 * 						1 - calibrate; 0 - not calibrate
 * @return	  none
 * @note	  This function will not take effect until it is called before cpu_wakeup_init(). 
 */
void cpu_wakeup_init_calib_32k_rc_cfg(char calib_flag);

/**
 * @brief   This function serves to recover system timer from tick of internal 32k RC.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_rc(unsigned int now_tick_32k);

/**
 * @brief   This function serves to recover system timer from tick of external 32k crystal.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k);


typedef unsigned int (*pm_tim_recover_handler_t)(unsigned int);

extern  pm_tim_recover_handler_t pm_tim_recover;


/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function servers to wake up the cpu from sleep mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the 32k tick which you want to sleep.(32*1000 -> 1s)
 * @return     indicate whether the cpu is wake up successful.
 */
int cpu_long_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function servers to wake up the cpu from sleep mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the 32k tick which you want to sleep.(32768 -> 1s)
 * 							 Note that the frequency of the external 32k crystal is 32768, not 32000. The sleep tick value is calculated based on 32768 ticks being 1s.
 * @return     indicate whether the cpu is wake up successful.
 */
int cpu_long_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

typedef int (*cpu_pm_handler_t)(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

extern 	cpu_pm_handler_t  		 cpu_sleep_wakeup;

/**
 * @brief      This function serves to determine whether wake up source is internal 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_internal_32k_crystal(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_rc;
	pm_tim_recover  	 	= pm_tim_recover_32k_rc;

	blt_miscParam.pm_enter_en 	= 1; // allow enter pm, 32k rc does not need to wait for 32k clk to be stable
}

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_external_32k_crystal(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_xtal;
	pm_tim_recover		 	= pm_tim_recover_32k_xtal;

	blt_miscParam.pad32k_en 	= 1; // set '1': 32k clk src use external 32k crystal
}

/**********************************  Internal APIs (not for user)***************************************************/
extern  unsigned char 		    tl_multi_addr;
extern  unsigned char 		    tl_24mrc_cal;
extern 	unsigned short 			tick_32k_calib;
extern  unsigned int 			tick_cur;
extern  unsigned int 			tick_32k_cur;

void sleep_start(void);

unsigned int  pm_get_info0(void);

unsigned int  pm_get_info1(void);
/**
 * @brief     this function servers to get calibration value from EFUSE.
 * There is only the reference voltage value sampled by gpio in the efuse of B85.
 * @param[in] none
 * @return    data - calibration value.If data is 0, there is no calibration value in efuse.
 */
unsigned short efuse_get_adc_calib_value(void);

void soft_reboot_dly13ms_use24mRC(void);

void check_32k_clk_stable(void);

/**
 * @brief		This function serves to set flash voltage vdd_f.TO ensure the vdd_f is enough to supply the flash,need to calibration the vdd_f.
 * @param[in]	voltage - the vdd_f need to set.The voltage is range from 0 to 0x07.
 * @return		none.
 */
void pm_set_vdd_f(Flash_VoltageDef voltage);

/**
 * @brief		This function is used to configure the early wake-up time.
 * @param[in]	param - deep/suspend/deep_retention r_delay time.(default value: suspend/deep_ret=16, deep=24)
 * @return		none.
 */
void pm_set_wakeup_time_param(pm_r_delay_us_s param);

/**
 * @brief		This function is used in applications where the crystal oscillator is relatively slow to start.
 * 				When the start-up time is very slow, you can call this function to avoid restarting caused
 * 				by insufficient crystal oscillator time (it is recommended to leave a certain margin when setting).
 * @param[in]	delay_us - This time setting is related to the parameter nopnum, which is about the execution time of the for loop
 * 							in the ramcode(default value: 135).
 * @param[in]	loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: loopnum=10).
 * @param[in]	nopnum - The number of for loops used to wait for the crystal oscillator to stabilize after suspend wakes up.
 * 						 for(i = 0; i < nopnum; i++){ asm("tnop"); }(default value: Flash=200).
 * @return		none.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum, unsigned int nopnum);

/**
 * @brief   	This function is used to determine the stability of the crystal oscillator.
 * 				To judge the stability of the crystal oscillator, xo_ready_ana is invalid, and use an alternative solution to judge.
 * 				Alternative principle: Because the clock source of the stimer is the crystal oscillator,
 * 				if the crystal oscillator does not vibrate, the tick value of the stimer does not increase or increases very slowly (when there is interference).
 * 				So first use 24M RC to run the program and wait for a fixed time, calculate the number of ticks that the stimer should increase during this time,
 * 				and then read the tick value actually increased by the stimer.
 * 				When it reaches 50% of the calculated value, it proves that the crystal oscillator has started.
 * 				If it is not reached for a long time, the system will reboot.
 * @return  	none.
 */
_attribute_ram_code_sec_noinline_ void pm_wait_xtal_ready(void);

#if PM_LONG_SLEEP_WAKEUP_EN
/**
 * @brief      This function servers to wake up the cpu from sleep mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  SleepDurationUs - the time of sleep.
 * @return     indicate whether the cpu is wake up successful.
 */
int pm_long_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int  SleepDurationUs);


#endif

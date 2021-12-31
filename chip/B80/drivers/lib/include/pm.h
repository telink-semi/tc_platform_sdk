/********************************************************************************************************
 * @file	pm.h
 *
 * @brief	This is the header file for b80
 *
 * @author	Driver Group
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "bsp.h"
#include "gpio.h"
#include "clock.h"
#include "flash.h"
#define PM_DEBUG							0


#if(PM_DEBUG)
volatile unsigned char debug_pm_info;
volatile unsigned int ana_32k_tick;
#endif

#define PM_LONG_SUSPEND_EN					1

#ifndef PM_TIM_RECOVER_MODE
#define PM_TIM_RECOVER_MODE			    	0
#endif


#define XTAL_READY_CHECK_TIMING_OPTIMIZE	1

#define RAM_CRC_EN							0		//if use RAM_CRC func, retention ldo will turn down to 0.6V in A1, A0 is 0.8V.

//when timer wakeup,the DCDC delay time is accurate,but other wake-up sources wake up,
//this time is ((PM_DCDC_DELAY_CYCLE+1)*2-1)*32us ~ (PM_DCDC_DELAY_CYCLE+1)*2*32us
#define PM_DCDC_DELAY_DURATION     					250   // delay_time_us = (PM_DCDC_DELAY_CYCLE+1)*2*32us
												  // 2 * 1/16k = 125 uS, 3 * 1/16k = 187.5 uS  4*1/16k = 250 uS

#define PM_XTAL_MANUAL_MODE_DELAY		    200  //150  200

#if(PM_DCDC_DELAY_DURATION == 62)
#define PM_DCDC_DELAY_CYCLE		0
#elif(PM_DCDC_DELAY_DURATION == 125)
#define PM_DCDC_DELAY_CYCLE		1
#elif(PM_DCDC_DELAY_DURATION == 187)
#define PM_DCDC_DELAY_CYCLE		2
#elif(PM_DCDC_DELAY_DURATION == 250)
#define PM_DCDC_DELAY_CYCLE		3
#endif

#define EARLYWAKEUP_TIME_US_SUSPEND 		(PM_DCDC_DELAY_DURATION + PM_XTAL_MANUAL_MODE_DELAY + 200)  //100: code running time margin//154  //175
#define EARLYWAKEUP_TIME_US_DEEP_RET    	(PM_DCDC_DELAY_DURATION + 90)//(PM_DCDC_DELAY_DURATION + 32)
//#define EARLYWAKEUP_TIME_US_DEEP	    	(PM_DCDC_DELAY_DURATION + 32 + ((SOFT_START_DLY)*62))
#define EMPTYRUN_TIME_US       	    		(EARLYWAKEUP_TIME_US_SUSPEND + 200)

#define EARLYWAKEUP_TIME			19
#define	tick_32k_tick_per_ms		32
#define PM_EMPTYRUN_TIME_US			25




/**
 * @brief analog register below can store information when MCU in deepsleep mode
 * 	      store your information in these ana_regs before deepsleep by calling analog_write function
 * 	      when MCU wakeup from deepsleep, read the information by by calling analog_read function
 * 	      Reset these analog registers only by power cycle
 */

#define DEEP_ANA_REG0                       0x3a //initial value =0x00
#define DEEP_ANA_REG1                       0x3b //initial value =0x00
#define DEEP_ANA_REG2                       0x3c //initial value =0x0f

/**
 * @brief these analog register can store data in deepsleep mode or deepsleep with SRAM retention mode.
 * 	      Reset these analog registers by watchdog, chip reset, RESET Pin, power cycle
 */

#define DEEP_ANA_REG6                       0x35 //initial value =0x20
#define DEEP_ANA_REG7                       0x36 //initial value =0x00
#define DEEP_ANA_REG8                       0x37 //initial value =0x00
#define DEEP_ANA_REG9                       0x38 //initial value =0x00
#define DEEP_ANA_REG10                      0x39 //initial value =0xff


#define SYS_NEED_REINIT_EXT32K			    BIT(0)


//ana3b system used, user can not use
#define SYS_DEEP_ANA_REG 					DEEP_ANA_REG1
#define WAKEUP_STATUS_TIMER_CORE     	    ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD		        ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

/**
 * @brief sleep mode.
 * @note	After entering suspend mode,the registers of linklayer and modem return to default values,so the
 * 			functions in rf.h need to be reconfigured after suspend. (The registers with base address 0x400,
 * 			0xf00,0x1200 need to be reconfigured.)
 */
typedef enum {
	//available mode for customer
	SUSPEND_MODE						= 0x00,

	DEEPSLEEP_MODE						= 0x30,		//when use deep mode pad wakeup(low or high level), if the high(low) level always in
													//the pad, system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20
													//deep retention also had this issue, but not to reboot.

	SHUTDOWN_MODE						= 0x40,

	DEEPSLEEP_MODE_RET_SRAM_LOW16K  	= 0x01,  //for boot from sram   different with vulture.

	//not available mode
	DEEPSLEEP_RETENTION_FLAG			= 0x0F,
}SleepMode_TypeDef;


/**
 * @brief   wakeup source
 */
typedef enum {
	 //available wake-up source for customer
	 PM_WAKEUP_PAD   			= BIT(0),
	 PM_WAKEUP_CORE_USB  		= (BIT(1) | (BIT(2)<<8)),
	 PM_WAKEUP_CORE_KEY_SCAN  	= (BIT(1) | (BIT(1)<<8)),
	 PM_WAKEUP_TIMER 			= BIT(2),

	 //not available wake-up source for customer
	 PM_TIM_RECOVER_START   = BIT(14),
	 PM_TIM_RECOVER_END     = BIT(15),
}SleepWakeupSrc_TypeDef;

/**
 * @brief   wakeup status
 */
enum {
	 WAKEUP_STATUS_PAD  			= BIT(0),
	 WAKEUP_STATUS_CORE  			= BIT(1),
	 WAKEUP_STATUS_TIMER 			= BIT(2),

	 STATUS_GPIO_ERR_NO_ENTER_PM  	= BIT(7),
	 STATUS_ENTER_SUSPEND  			= BIT(30),
};

/**
 * @brief	pm power weather to power down definition
 */
typedef enum {
	 PM_POWER_BASEBAND  	= BIT(0),	//weather to power on the BASEBAND before suspend.
	 PM_POWER_USB  			= BIT(1),	//weather to power on the USB before suspend.
}pm_suspend_power_cfg_e;


/**
 * @brief   deepsleep wakeup by external xtal
 */
typedef struct{
	unsigned char ext_cap_en;    //24xtal  cap
	unsigned char pad32k_en;
	unsigned char pm_enter_en;
	unsigned char rsvd;
}misc_para_t;

extern  _attribute_aligned_(4) misc_para_t 				blt_miscParam;

/**
 * @brief   deepsleep wakeup status
 */
typedef struct{
	unsigned char is_deepretn_back;
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;
	unsigned char rsvd;
}pm_para_t;

extern _attribute_aligned_(4) pm_para_t	pmParam;

#if (PM_TIM_RECOVER_MODE)

typedef struct{
	unsigned int   tick_sysClk;
	unsigned int   tick_32k;
	unsigned int   recover_flag;
}pm_tim_recover_t;

extern _attribute_aligned_(4) pm_tim_recover_t			pm_timRecover;
#endif


typedef int (*suspend_handler_t)(void);
extern  suspend_handler_t 		 func_before_suspend;

typedef void (*check_32k_clk_handler_t)(void);
extern  check_32k_clk_handler_t  pm_check_32k_clk_stable;


/**
 * @brief      This function serves to change the timing of enable ram crc.
 * @param[in]  none.
 * @return     none.
 */
extern unsigned int RAM_CRC_EN_16KRAM_TIME;
extern unsigned int RAM_CRC_EN_32KRAM_TIME;
static inline void ram_crc_en_timing(unsigned int RAM_CRC_16K_Timing, unsigned int RAM_CRC_32K_Timing)
{
	RAM_CRC_EN_16KRAM_TIME = RAM_CRC_16K_Timing;
	RAM_CRC_EN_32KRAM_TIME = RAM_CRC_32K_Timing;
}


/**
 * @brief      This function serves to change the timing of soft start delay.
 * @param[in]  none.
 * @return     none.
 */
extern unsigned char SOFT_START_DLY;
extern unsigned int EARLYWAKEUP_TIME_US_DEEP;
static inline void soft_start_dly_time(unsigned char soft_start_time)
{
	SOFT_START_DLY = soft_start_time;
	EARLYWAKEUP_TIME_US_DEEP = PM_DCDC_DELAY_DURATION + 90 + ((SOFT_START_DLY)*62);
}

/**
 * @brief     this function servers to wait bbpll clock lock
 * @param[in] none
 * @return    none
 */
void pm_wait_bbpll_done(void);

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
 * @brief		This function serves to set baseband/usb power on/off before suspend sleep,If power
 * 				on this module,the suspend curent will increase;power down this module will save current,
 * 				but you need to re-init this module after suspend wakeup.All power down default to save
 * 				current.
 * @param[in]	value - weather to power on/off the baseband/usb.
 * @param[in]	on_off - select power on or off. 0 - power off; other value - power on;.
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
 * @param   power mode- set the power mode(LOD mode, DCDC mode, DCDC_LDO mode)
 * @param   xtal- set this parameter based on external crystal
 * @return  none
 */
void cpu_wakeup_init(XTAL_TypeDef xtal) ;

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
 * @brief      This function serves to set the working mode of MCU based on 32k rc,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 				This chip use 1.5V power supply,the 32k rc ppm about 2000,if need the accuracy higher,need use software to improve it. 
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

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

extern void check_32k_clk_stable(void);

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_external_32k_crystal(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_xtal;
	pm_check_32k_clk_stable = check_32k_clk_stable;
	pm_tim_recover		 	= pm_tim_recover_32k_xtal;

	blt_miscParam.pad32k_en 	= 1; // set '1': 32k clk src use external 32k crystal
}
/**
 * @brief		This function serves to set flash voltage vdd_f.TO ensure the vdd_f is enough to supply the flash,need to calibration the vdd_f.
 * @param[in]	voltage - the vdd_f need to set.The voltage is range from 0 to 0x07.
 * @return		none.
 */
void pm_set_vdd_f(Flash_VoltageDef voltage_ldo);

/**********************************  Internal APIs (not for user)***************************************************/
extern  unsigned short 		    tl_multi_addr;
extern  unsigned char 		    tl_24mrc_cal;
extern 	unsigned int 			tick_32k_calib;
extern  unsigned int 			tick_cur;
extern  unsigned int 			tick_32k_cur;
extern  unsigned char       	pm_long_suspend;

void sleep_start(void);


//unsigned int cpu_get_32k_tick(void);

void cpu_set_32k_tick(unsigned int tick);

void soft_reboot_dly13ms_use24mRC(void);

void pm_set_32k_watchdog_interval(unsigned int interval_offset);




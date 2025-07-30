/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "clock.h"
#include "flash.h"
#define PM_DEBUG							0

#if(PM_DEBUG)
volatile unsigned char debug_pm_info;
volatile unsigned int ana_32k_tick;
#endif

#define PM_LONG_SUSPEND_EN					1

#define RAM_CRC_EN							0		//if use RAM_CRC func, retention ldo will turn down to 0.6V in A1, A0 is 0.8V.

#define EARLYWAKEUP_TIME			19
#define	tick_32k_tick_per_ms		32
#define PM_EMPTYRUN_TIME_US			25




/**
 * @brief analog register below can store information when MCU in deepsleep mode
 * 	      store your information in these ana_regs before deepsleep by calling analog_write function
 * 	      when MCU wakeup from deepsleep, read the information by by calling analog_read function
 * 	      Reset these analog registers only by power cycle
 */

#define DEEP_ANA_REG0                       0x3a //initial value =0x00	[Bit1] The crystal oscillator failed to start normally.The customer cannot change!
                                                 //                     [Bit2] The PLL failed to start normally.The customer cannot change!
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
//#define SYS_DEEP_ANA_REG 					DEEP_ANA_REG1
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

	 WAKEUP_STATUS_INUSE_ALL        = 0x07,

	 STATUS_GPIO_ERR_NO_ENTER_PM  	= BIT(8), /**<Bit8 is used to determine whether the wake source is normal.*/
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
 * @brief	wakeup tick type definition
 */
typedef enum {
	 PM_TICK_STIMER_16M		= 0,
	 PM_TICK_32K			= 1,
}pm_wakeup_tick_type_e;

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
	unsigned short  deep_r_delay_cycle ;			/**< hardware delay time, deep_r_delay_us = (deep_r_delay_cycle+1) * 1/16k */
	unsigned short  suspend_ret_r_delay_cycle ;		/**< hardware delay time, suspend_ret_r_delay_us = (suspend_ret_r_delay_cycle+1) * 1/16k */
}pm_r_delay_cycle_s;

extern volatile pm_r_delay_cycle_s g_pm_r_delay_cycle;

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


/**
 * @brief	vdd_1v2 voltage definition
 */
typedef enum {
	 VDD_1V2_1V35   = 0x07,
	 VDD_1V2_1V3    = 0x06,
	 VDD_1V2_1V25   = 0x05,
	 VDD_1V2_1V2    = 0x04,
	 VDD_1V2_1V15   = 0x03,
	 VDD_1V2_1V1    = 0x02,
	 VDD_1V2_1V05   = 0x01,
	 VDD_1V2_1V0    = 0x00,
}pm_vdd_1v2_voltage_e;

extern _attribute_aligned_(4) pm_para_t	pmParam;


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
 * @brief     this function servers to wait bbpll clock lock.
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void pm_wait_bbpll_done(void);

/**
 * @brief     this function servers to wait bbpll clock lock.
 * @param[in] num - the number of PLL tests.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void pm_bbpll_zero_tolerance_check(unsigned int num);

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
 * 				on this module,the suspend current will increase;power down this module will save current,
 * 				but you need to re-init this module after suspend wakeup.All power down default to save
 * 				current.
 * @param[in]	value - weather to power on/off the baseband/usb.
 * @param[in]	on_off - select power on or off. 0 - power off; other value - power on;.
 * @return		none.
 */
void pm_set_suspend_power_cfg(pm_suspend_power_cfg_e value, unsigned char on_off);

/**
 * @brief		This function will put the cpu into the stall state, and then wake up by the specified wakeup source.
 * 				All interrupt sources can wake the CPU from stall mode.
 * 				Depending on the configuration, the execution flow after waking up will be different:
 * 				If the bit corresponding to the wake-up source in the register reg_irq_mask is enabled and the total interrupt is turned on,
 * 				the CPU will be interrupted first after waking up from the stall state, and then continue to execute.
 * 				If the bit corresponding to the wake-up source in the register reg_irq_mask is disabled,
 * 				the CPU will continue to execute after waking up from the stall state.
 * 				No matter which execution flow is taken after wake-up, the interrupt flag corresponding to the wake-up source needs to be clear after wake-up.
 * @param[in]	irq_mask - interrupt source for wake up.
 * @return		none.
 */
void cpu_stall_wakeup(irq_list_e irq_mask);

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  pin - the pins can be set to all GPIO except PB0, PB1, PB3, PD4, PF0 and GPIOE groups.
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
 * @param   xtal - set crystal for different application.
 * @return  none
 * @note	1. In version A0, the chip cannot be lower than 2.2V when it is powered on for the first time.
 * 				After calling this function, g_chip_version is the version number recorded.
 * 			2. For crystal oscillators with very slow start-up or poor quality, after calling this function,
 * 				a reboot will be triggered (whether a reboot has occurred can be judged by using DEEP_ANA_REG0[bit1]).
 * 				For the case where the crystal oscillator used is very slow to start, you can call the pm_set_wakeup_time_param()
 * 			 	to adjust the waiting time for the crystal oscillator to start before calling the cpu_wakeup_init().
 * 			 	When this time is adjusted to meet the crystal oscillator requirements, it will not reboot.
 * 			3. When this function called after power on or deep sleep wakeup, it will cost about 6~7ms for perform 32k RC calibration. 
 * 				If do not want this logic, you can check the usage and precautions of cpu_wakeup_init_calib_32k_rc_cfg().
 */
void cpu_wakeup_init(cap_typedef_e cap);

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
 * @brief      This function serves to set the working mode of MCU based on 32k rc,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select,if only KEY_SCAN is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K watchdog will be turned off inside the function.
 * @param[in]  wakeup_tick_type	- tick type select. Use 32K tick count for long-term sleep and 16M tick count for short-term sleep.
 * @param[in]  wakeup_tick - The tick value at the time of wake-up.
							 If the wakeup_tick_type is PM_TICK_STIMER_16M, then wakeup_tick is converted to 16M. The range of tick that can be set is approximately:
							 current tick value + (18352~0xe0000000), and the corresponding sleep time is approximately: 2ms~234.88s.It cannot go to sleep normally when it exceeds this range.
							 If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K. The range of tick that can be set is approximately:
							 64~0xffffffff, and the corresponding sleep time is approximately: 2ms~37hours.It cannot go to sleep normally when it exceeds this range.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select, and if only KEY_SCAN is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K watchdog will be turned off inside the function..
 * @param[in]  wakeup_tick_type	- tick type select. Use 32K tick count for long-term sleep and 16M tick count for short-term sleep.
 * @param[in]  wakeup_tick - The tick value at the time of wake-up.
							 If the wakeup_tick_type is PM_TICK_STIMER_16M, then wakeup_tick is converted to 16M. The range of tick that can be set is approximately:
							 current tick value + (18352~0xe0000000), and the corresponding sleep time is approximately: 2ms~234.88s.It cannot go to sleep normally when it exceeds this range.
							 If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K. The range of tick that can be set is approximately:
							 64~0xffffffff, and the corresponding sleep time is approximately: 2ms~37hours.It cannot go to sleep normally when it exceeds this range.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);

typedef int (*cpu_pm_handler_t)(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);

extern 	cpu_pm_handler_t  		 cpu_sleep_wakeup_and_longsleep;

#define cpu_sleep_wakeup(sleep_mode, wakeup_src, wakeup_tick)  cpu_sleep_wakeup_and_longsleep(sleep_mode, wakeup_src, PM_TICK_STIMER_16M, wakeup_tick)
#define cpu_long_sleep_wakeup(sleep_mode, wakeup_src, wakeup_tick)  cpu_sleep_wakeup_and_longsleep(sleep_mode, wakeup_src, PM_TICK_32K, wakeup_tick)

/**
 * @brief      This function serves to determine whether wake up source is internal 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_internal_32k_crystal(void)
{
	cpu_sleep_wakeup_and_longsleep	= cpu_sleep_wakeup_32k_rc;
	pm_tim_recover					= pm_tim_recover_32k_rc;
	blt_miscParam.pm_enter_en		= 1; // allow enter pm, 32k rc does not need to wait for 32k clk to be stable
}

extern void check_32k_clk_stable(void);

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_external_32k_crystal(void)
{
	cpu_sleep_wakeup_and_longsleep	= cpu_sleep_wakeup_32k_xtal;
	pm_check_32k_clk_stable			= check_32k_clk_stable;
	pm_tim_recover					= pm_tim_recover_32k_xtal;
	blt_miscParam.pad32k_en			= 1; // set '1': 32k clk src use external 32k crystal
}
/**
 * @brief		This function serves to set flash voltage vdd_f.TO ensure the vdd_f is enough to supply the flash,need to calibration the vdd_f.
 * @param[in]	voltage - the vdd_f need to set.The voltage is range from 0 to 0x07.
 * @return		none.
 */
void pm_set_vdd_f(Flash_VoltageDef voltage_ldo);

#if 0
/*
 * Eaglet A0~A3 have the following issues (eaglet A4 and eaglet-b do not have): Operating OTP in RF rx state may result in errors. 
 * The initial solution was:
 * 1. All programs running in OTP can avoid this issue
 * 2. If the program is relatively large and still need to run the OTP program, then need to open 32K wd and trim the calibration value from 1.2V to ATE (calibrated according to 1.15V)
 * So in the driver SDK v1.5.0, it was processed according to the above scheme.
 * Subsequently, it was discovered that lowering the voltage by 1.2V actually carries risks, as the digital function may not work properly. 
 * Therefore, the final conclusion is that this interface is not allowed to be called in any usage scenario, so it has been removed here.
 * (added by jilong.liu, confirmed by kaixin.chen at 20240307)
 */
/**
 * @brief		This function serves to set dig_ldo vdd_1v2.
 *              for otp products, if all codes cannot be executed in ram code, there is a risk of crash. need to enable 32K watchdog and trim vDD1V2 voltage to reduce the risk.
 * @param[in]	vdd_1v2_voltage - the vdd_1v2 need to set.
 * @return		none.
 */
void pm_set_vdd_1v2(pm_vdd_1v2_voltage_e vdd_1v2_voltage);
#endif

/**
 * @brief		This function is used to configure the early wake-up time.
 * @param[in]	param - deep/suspend/deep_retention r_delay time.(default value: suspend/deep_ret=7, deep=15)
 * @return		none.
 */
void pm_set_wakeup_time_param(pm_r_delay_cycle_s param);

/**
 * @brief		This function is used in applications where the crystal oscillator is relatively slow to start.
 * 				When the start-up time is very slow, you can call this function to avoid restarting caused
 * 				by insufficient crystal oscillator time (it is recommended to leave a certain margin when setting).
 * @param[in]	delay_us - This time setting is related to the parameter nopnum, which is about the execution time of the for loop
 * 							in the ramcode(default value: 200).
 * @param[in]	loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: 10).
 * @param[in]	nopnum - The number of for loops used to wait for the crystal oscillator to stabilize after suspend wakes up.
 * 						 for(i = 0; i < nopnum; i++){ asm("tnop"); }(default value: Flash=250, OTP=Flash-60).
 * @return		none.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum, unsigned int nopnum);

/**********************************  Internal APIs (not for user)***************************************************/
extern  unsigned short 		    tl_multi_addr;
extern  unsigned char 		    tl_24mrc_cal;
extern 	unsigned int 			tick_32k_calib;
extern  unsigned int 			tick_cur;
extern  unsigned int 			tick_32k_cur;
extern  unsigned char       	pm_long_suspend;

void sleep_start(void);


void cpu_set_32k_tick(unsigned int tick);

void soft_reboot_dly13ms_use24mRC(void);

/**
 * @brief	  This function serves to clear cache tag.
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void cache_tag_clr();

#if SRAM_OTP_FLASH_HANDLE
#include "lib/include/otp/otp_base.h"
/**
 * @brief   This function services to power on flash and OTP and indicates that the program is not an OTP program.
 * @return  none
 * @note
 *          - When compile source code for SRAM program, this function does not call.
 *          - When compile SDK code for SRAM program, this function is called in cpu_wakeup_init().
 */
extern unsigned char otp_program_flag;
static inline void sram_program_handler(void)
{
    analog_write(0x05, analog_read(0x05) & ~(BIT(5))); /* <5>:Power down of Flash LDO, 1: Power down  0: Power up */
    otp_set_active_mode();
    otp_program_flag = 0; /* Indicates the program is not an OTP program */
}
#else
/**
 * @brief   This function services to power on flash and OTP and indicates that the program is not an OTP program.
 * @return  none
 * @note
 *          - When compile source code for SRAM program, this function does not call.
 *          - When compile SDK code for SRAM program, this function is called in cpu_wakeup_init().
 */
__attribute__((weak)) void sram_program_handler(void);
#endif

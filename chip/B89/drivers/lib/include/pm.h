/********************************************************************************************************
 * @file	pm.h
 *
 * @brief	This is the header file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#define PM_TIM_RECOVER_MODE			    	0
#endif

#define PM_OTP_SET_DIF_VERSION			    0		//if the chip is A0, set PM_OTP_SET_DIF_VERSION = 1.

#define XTAL_READY_CHECK_TIMING_OPTIMIZE	1

#define RAM_CRC_EN							0		//if use RAM_CRC func, retention ldo will turn down to 0.6V in A1, A0 is 0.8V.


#define PM_DCDC_DELAY_DURATION     			6000   // delay_time_us = (PM_DCDC_DELAY_CYCLE+1)*2*32us
#define PM_XTAL_DELAY_DURATION				6000

#define PM_DCDC_DELAY_DURATION_SUSPEND     	187   // delay_time_us = (PM_DCDC_DELAY_CYCLE+1)*2*32us
#define PM_XTAL_DELAY_DURATION_SUSPEND		187

#define PM_XTAL_MANUAL_MODE_DELAY		    200  //150  200

#define EARLYWAKEUP_TIME_US_SUSPEND 		(PM_DCDC_DELAY_DURATION_SUSPEND + PM_XTAL_MANUAL_MODE_DELAY + 210 )  //100: code running time margin//154  //175
#define EARLYWAKEUP_TIME_US_DEEP_RET    	(PM_DCDC_DELAY_DURATION + 176)//(PM_DCDC_DELAY_DURATION + 112)
#define EMPTYRUN_TIME_US_SUSPEND       	    (EARLYWAKEUP_TIME_US_SUSPEND + 200)
#define EMPTYRUN_TIME_US_DEEP       	    (EARLYWAKEUP_TIME_US_DEEP_RET)

#define EARLYWAKEUP_TIME_US_DEEP_RET_NO_DCDC	(PM_DCDC_DELAY_DURATION_SUSPEND)
#define EMPTYRUN_TIME_US_DEEP_RET_NO_DCDC		(EARLYWAKEUP_TIME_US_DEEP_RET_NO_DCDC + 32)

#define EARLYWAKEUP_TIME			19
#define	tick_32k_tick_per_ms		32
#define PM_EMPTYRUN_TIME_US			25



#define PM_LONG_SLEEP_WAKEUP_EN			    0 //if user need to make MCU sleep for a long time that is more than 268s, this macro need to be enabled and use "pm_long_sleep_wakeup" function

/**
 * @brief analog register below can store information when MCU in deepsleep mode
 * 	      store your information in these ana_regs before deepsleep by calling analog_write function
 * 	      when MCU wakeup from deepsleep, read the information by by calling analog_read function
 * 	      Reset these analog registers only by power cycle
 */

#define DEEP_ANA_REG0                       0x3a //initial value =0x00   [Bit0] is already occupied. The customer cannot change!
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
#define DEEP_ANA_REG10                      0x39 //initial value =0xff  [Bit0] is already occupied. The customer cannot change!


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

	DEEPSLEEP_MODE_RET_SRAM_LOW16K  	= 0x23,  //for boot from sram   different with vulture.
	DEEPSLEEP_MODE_RET_SRAM_LOW32K  	= 0x03,  //for boot from sram

	//not available mode
	DEEPSLEEP_RETENTION_FLAG			= 0x0F,
}SleepMode_TypeDef;


/**
 * @brief   wakeup source
 */
typedef enum {
	 //available wake-up source for customer
	 PM_WAKEUP_SHUTDOWN     = BIT(1),
	 PM_WAKEUP_PAD   		= BIT(3),
	 PM_WAKEUP_CORE  		= BIT(4),
	 PM_WAKEUP_TIMER 		= BIT(5),
	 PM_WAKEUP_DEBOUNCE 	= BIT(7),

	 //not available wake-up source for customer
	 PM_TIM_RECOVER_START   = BIT(14),
	 PM_TIM_RECOVER_END     = BIT(15),
}SleepWakeupSrc_TypeDef;

/**
 * @brief   wakeup status
 */
enum {
	 WAKEUP_STATUS_TIMER  			= BIT(1),
	 WAKEUP_STATUS_CORE 			= BIT(2),
	 WAKEUP_STATUS_PAD    			= BIT(3),
	 WAKEUP_STATUS_DEBOUNCE    		= BIT(4),

	 STATUS_GPIO_ERR_NO_ENTER_PM  	= BIT(8),/**<Bit8 is used to determine whether the wake source is normal.*/
	 STATUS_ENTER_SUSPEND  			= BIT(30),
};

/**
 * @brief	the mcu status when run init.
 */
typedef enum{
	MCU_STATUS_POWER_ON  		= BIT(0),
	MCU_STATUS_REBOOT_BACK    	= BIT(1),	//the user will not see the reboot status.
	MCU_STATUS_DEEPRET_BACK  	= BIT(2),
	MCU_STATUS_DEEP_BACK		= BIT(3),
}pm_mcu_status;


/**
 * @brief   debounce wakeup pad
 */
enum {
	 DEBOUNCE_WAKEUP_PA1  		= GPIO_PA1,
	 DEBOUNCE_WAKEUP_PD3  		= GPIO_PD3,
};
/**
 * @brief  otp dcdc output
 */
typedef enum {
    DCDC_2P5V = 0,
    DCDC_3P0V,
} DcdcOutput_TypeDef;

/**
 * @brief  1P2 power sel
 */
typedef enum {
    DCDC_OUTPUT_SEL 			= 0,   	/*< VBAT burst to 2.5V to supply power for RF_LDO, analog_ldo, digital_ldo and SRAM_ldo */
    VBAT_SEL					= 1,	/*< Use VBAT to supply power for RF_LDO, analog_ldo, digital_ldo and SRAM_ldo */
} PowerSel_TypeDef;

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
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;
	unsigned char mcu_status;
	unsigned char rsvd;
}pm_para_t;

/**
 * @brief	pm power weather to power down definition
 */
typedef enum {
	 PM_POWER_BASEBAND  	= BIT(0),	//weather to power on the BASEBAND before suspend.
}pm_suspend_power_cfg_e;

extern _attribute_aligned_(4) pm_para_t	pmParam;

extern _attribute_data_retention_  unsigned char 		pm_1p2_power_sel_vbat;

extern _attribute_data_retention_	unsigned char 		g_chip_version;

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


void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func );


/**
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_deepRetentionWakeup(void)
{
	return (pmParam.mcu_status == MCU_STATUS_DEEPRET_BACK);
}

/**
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_deepWakeup(void)
{
	return (pmParam.mcu_status == MCU_STATUS_DEEP_BACK);
}

/**
 * @brief      This function serves to determine whether mcu is power on.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_powerOn(void)
{
	return (pmParam.mcu_status == MCU_STATUS_POWER_ON);
}

/**
 * @brief      This function serves to determine whether mcu is reboot back.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_rebootBack(void)
{
	return (pmParam.mcu_status == MCU_STATUS_REBOOT_BACK);
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
 * @brief      This function serves to set debounce count value.
 * @param[in]  debounce_count - number of filtered clocks under 32k, ranges from 1 to 32, which is 31.25~1000us.
 * @return     none.
 */
static inline void pm_set_debounce_count(unsigned char debounce_count)
{
	analog_write(0x2e,(analog_read(0x2e) & 0xe0) | (debounce_count - 1));	//ana_2e<4:0> pad_debounce_filter: set the debounce filter 32k clock cycle.
}

/**
 * @brief      This function serves to determine whether fifo is empty.
 * @param[in]  none.
 * @return     1 - empty , 0 - not empty.
 */
static inline unsigned char pm_debounce_fifo_empty(void)
{
	return (BIT(5) == (analog_read(0x48) & BIT(5)));	//ana_48<5> fifo_empty
}

/**
 * @brief      This function serves to determine whether the recorded 32k tick value overflows, when the level duration exceeds 1023.979ms,
 * 			   the data will overflow.
 * @param[in]  none.
 * @return     1 - overflow, 0 - not overflow.
 */
static inline unsigned char pm_debounce_data_overflow(void)
{
	return (BIT(6) == (analog_read(0x48) & BIT(6)));	//ana_48<6> indicator_counter_overflow
}

/**
 * @brief		This function serves to set baseband power on/off before suspend sleep,If power
 * 				on this module,the suspend current will increase;power down this module will save current,
 * 				but you need to re-init this module after suspend wakeup.All power down default to save
 * 				current.
 * @param[in]	value - weather to power on/off the baseband.
 * @param[in]	on_off - select power on or off, 0 - power off; other value - power on.
 * @return		none.
 */
void pm_set_suspend_power_cfg(pm_suspend_power_cfg_e value, unsigned char on_off);

/**
 * @brief      This function serves to get a valid level of 32k tick value, and there is a FIFO inside the chip with a size of 16 Half Words,
 * 			   and the highest bit of data in FIFO represents what level data is there. If the FIFO is full, the following data will be lost,
 * 			   and only the previously stored data will be retained.
 * @param[in]  none.
 * @return     data[14:0]: 32k tick value, ranges from 0 to 32767, which is 0~1023.979ms.
 * 			   data[15]: the data level polarity, 0 - low level, 1 - high level.
 */
unsigned short pm_debounce_read_data(void);

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
 * @note	1. The 1p2 power(include RF_LDO, analog_ldo, digital_ldo and SRAM_ldo) default use dcdc mode to supply power.
 * 				Call the API of pm_set_1p2_power_sel() can change the power mode, but need to caution that:
 *				Application need to detect the battery voltage,if VBAT is bigger than 1.2V, you can use all two mode, but:
 *				Use DCDC mode:tx current will increase than VBAT mode.
 *				Use VBAT mode:the tx power will decreased with the VBAT voltage.
 *				if the VBAT voltage is less than 1.2V, must use DCDC mode, otherwise the chip function will abnormal.
 * 			2. When this function called after power on or deep sleep wakeup, it will cost about 6~7ms for perform 32k RC calibration. 
 * 				If do not want this logic, you can check the usage and precautions of cpu_wakeup_init_calib_32k_rc_cfg().
 */
void cpu_wakeup_init(XTAL_TypeDef xtal) ;

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
 				This chip use 1.5V power supply,the 32k rc ppm about 2000,if need the accuracy higher,need use software to improve it. 
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select,if only DEBOUNCE is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K watchdog will be turned off inside the function.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select,if only DEBOUNCE is set as the wake-up source in sleep mode (there is no Timer wake-up source), the 32K watchdog will be turned off inside the function..
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
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
 * @brief      This function serves to switch dcdc.
 * @param[in]  dcdc_output type
 * @return     none
 */
void pm_set_dcdc_output(DcdcOutput_TypeDef volt);

/**********************************  Internal APIs (not for user)***************************************************/
extern  unsigned char 		    tl_multi_addr_L;
extern  unsigned char 		    tl_multi_addr_H;
extern  unsigned char 		    tl_24mrc_cal;
extern 	unsigned int 			tick_32k_calib;
extern  unsigned int 			tick_cur;
extern  unsigned int 			tick_32k_cur;
extern  unsigned char       	pm_long_suspend;

/**
 * @brief     this function servers to start sleep mode.
 * @param[in] none
 * @return    none
 */
void sleep_start(void);


/**
 * @brief  This function serves to set the 32k tick.
 * @param  tick - the tick need to set.
 * @return none.
 */
void cpu_set_32k_tick(unsigned int tick);

/**
 * @brief   	This function serves to delay 13ms based on 24M rc clk.
 * @param[in]   none.
 * @return  	none.
 */
void soft_reboot_dly13ms_use24mRC(void);

/**
 * @brief		This function serves to set flash voltage vdd_f.
 * @param[in]	voltage - the vdd_f need to set.
 * @return		none.
 */
void pm_set_vdd_f(Flash_VoltageDef voltage);

/**
 * @brief		This function serves to set the voltage of DCDC 2V5.
 * @param[in]	vol - the dcdc 2p5 need to set.
 * @return		none.
 */
void pm_set_dcdc_2p5(unsigned char vol);

/**
 * @brief		This function serves to set the power sel of 1p2,include RF_LDO, analog_ldo, digital_ldo and SRAM_ldo.
 * @param[in]	power_sel - 0:DCDC_OUTPUT_SEL, 1:VBAT_SEL.
 * @return		none.
 * @note		When the VBAT voltage > 1.2V,we can use the VBAT to supply the power for 1p2,in this mode,the rf power will
 * 				decrease as the VBAT voltage decrease.
 * 				When the VBAT voltage < 1.2V,we can use the DCDC mode to supply the power for 1p2,but the rf tx current will increase.
 *
 *				Application need to detect the battery voltage,if VBAT is bigger than 1.2V, you can use all two mode,but:
 *					Use DCDC mode:tx current will increase than VBAT mode.
 *					Use VBAT mode:the tx power will decrease with the VBAT voltage.
 *				if the VBAT voltage is less than 1.2V,must use DCDC mode,otherwise the chip function will abnormal.
 *				if call this API to change the 1p2 power mode,you should to re_call the rf_set_power_level_index() to set the rf power.
 */
void pm_set_1p2_power_sel(PowerSel_TypeDef power_sel);

/**
 * @brief		This function provides the function to turn off boost dcdc. When boost dcdc is turned off,
 * 				the voltage of the GPIO port on the chip's OTP module will drop to the VBAT voltage.
 * @param[in]	none.
 * @return		none.
 */
void pm_boost_dcdc_dis(void);

/**
 * @brief		This function provides the function to turn on boost dcdc.
 * @param[in]	none.
 * @return		none.
 */
void pm_boost_dcdc_en(void);

/**
 * @brief		This function serves to get the status of boost dcdc.
 * @return		status of boost dcdc, 1 - boost dcdc is open, 0 - boost dcdc is close.
 */
unsigned char pm_get_boost_dcdc_status(void);

/**
 * @brief		This function provides the function to turn on boost dcdc.
 * @param[in]	rst_time_value - set the value of reset time.
 * @return		none.
 */
void pm_set_rst_time(unsigned int rst_time_value);

/**
 * @brief		This function saves ram cipher encryption keys before deep retention.
 * @param[in]	none.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void pm_cipher_key_save(void);

/**
 * @brief		This function restores ram cipher encryption keys after deep retention.
 * @param[in]	none.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void pm_cipher_key_recover(void);

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



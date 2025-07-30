/********************************************************************************************************
 * @file    sd_adc.h
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
#pragma once
#include "driver.h"

/**
 * Since the voltage at the input P and N terminals, after voltage divider, must be <= 1.2V,
 * the detection ranges of the different modes are as follows:
    +--------------+---------+-----------------+
    | Sample Mode  | Divider | Detection Range |
    +--------------+---------+-----------------+
    |   GPIO_MODE  |   off   |    -1.2-1.2V    |
    |              |   1/2   |    -2.4-2.4V    |
    |              |   1/4   |    -3.6-3.6V    |
    |              |   1/8   |    -3.6-3.6V    |
    +--------------+---------+-----------------+
    |   VBAT_MODE  |   1/2   |     2.0-2.4V    |
    |              |   1/4   |     2.0-3.6V    |
    |              |   1/8   |     2.0-3.6V    |
    +--------------+---------+-----------------+
    |   TEMP_MODE  |    \    |  -40-85Celsius  |
    +--------------+---------+-----------------+

    -# Using the SD ADC after sleep wakeup:
       -# Before entering suspend, need to call sd_adc_power_off(SD_ADC_SAMPLE_MODE), after wake up from suspend, then call sd_adc_power_on(SD_ADC_SAMPLE_MODE), and wait for >160us(when the C10 capacitor on the development board is 10nF), then call sd_adc_sample_start() to start sampling.
       -# After waking up in DEEP/DEEP RETENTION mode, all sd adc registers need to be reconfigured and need to wait for >160us(when the C10 capacitor on the development board is 10nF) after sd_adc_power_on(SD_ADC_SAMPLE_MODE).

    -# Precautions for switching between different sampling modes are as follows:
      -# Since the internal filter needs to be reset after switching modes, the first 4 codes are anomalous and need to be discarded when starting sampling.
      -# The interfaces that need to be called to switch to GPIO mode are: sd_adc_temp_sensor_power_off(), sd_adc_vbat_mode_dis(), sd_adc_gpio_pin_init(),sd_adc_set_gpio_divider().
      -# The interfaces that need to be called to switch to VBAT mode are: sd_adc_temp_sensor_power_off(),sd_adc_vbat_mode_en(), sd_adc_set_vbat_divider().
      -# The interfaces that need to be called to switch to TEMP mode are: sd_adc_vbat_mode_dis(),sd_adc_set_diff_input(), sd_adc_temp_sensor_power_on().

    -# The interfaces that need to be called to switch between different sampling pins in gpio mode are: sd_adc_gpio_pin_init().
 */

volatile unsigned char g_sd_adc_divider;
volatile unsigned short g_sd_adc_downsample_rate;

/**
 * @note Different models of chips support different resolutions, including 12bit and 16bit.
 * The 12-bit chip only support the sampling clock for 1M,
 * The 16-bit chip support the sampling clock for 1M/2M,
 * chip specific support for which resolution, please refer to the datasheet.
 */
#define SD_ADC_SAPMPLE_CLK_1M_DIV(sys_clk)     (((sys_clk)/2/(1000000))-1)
#define SD_ADC_SAPMPLE_CLK_2M_DIV(sys_clk)     (((sys_clk)/2/(2000000))-1)

/**
 *  @brief  Define SD ADC VMID power switch state
 */
typedef enum
{
    SD_ADC_VMID_POWER_DOWN,
    SD_ADC_VMID_POWER_ON,
} sd_adc_vmid_power_switch_e;

typedef enum{
	SD_ADC_SAMPLE_MODE,
	SD_ADC_AUDIO_MODE,
}sd_adc_mode_e;

/**
 * @brief dc op mode enum
 * @note  only support two modes in TC321X
 */
typedef enum{
    ONE_DC_CHANNEL = 1,
    AUDIO_ONLY = 4,
}sd_dc_op_mode_e;

/**
 * @brief sd_adc positive input pin type
 * |               |              |
 * | :------------ | :----------- |
 * |    <15:12>    |    <11:0>    |
 * | sd_adc channel|    gpio pin  |
 */
typedef enum{
    SD_ADC_GPIO_PB0P = GPIO_PB0 | (0x0<<12),
    SD_ADC_GPIO_PB1P = GPIO_PB1 | (0x1<<12),
    SD_ADC_GPIO_PB2P = GPIO_PB2 | (0x2<<12),
    SD_ADC_GPIO_PB3P = GPIO_PB3 | (0x3<<12),
    SD_ADC_GPIO_PB4P = GPIO_PB4 | (0x4<<12),
    SD_ADC_GPIO_PB5P = GPIO_PB5 | (0x5<<12),
    SD_ADC_GPIO_PB6P = GPIO_PB6 | (0x6<<12),
    SD_ADC_GPIO_PB7P = GPIO_PB7 | (0x7<<12),
    SD_ADC_GPIO_PD0P = GPIO_PD0 | (0x8<<12),
    SD_ADC_GPIO_PD1P = GPIO_PD1 | (0x9<<12),
    SD_ADC_TEMP_SENSOR_P = 0 | (0xb<<12),
}sd_adc_p_input_pin_def_e;

/**
 * @brief sd_adc negative input pin type
 * |               |              |
 * | :------------ | :----------- |
 * |     <15:12>   |    <11:0>    |
 * | sd_adc channel|    gpio pin  |
 */
typedef enum{
    SD_ADC_GPIO_PB0N = GPIO_PB0 | (0x0<<12),
    SD_ADC_GPIO_PB1N = GPIO_PB1 | (0x1<<12),
    SD_ADC_GPIO_PB2N = GPIO_PB2 | (0x2<<12),
    SD_ADC_GPIO_PB3N = GPIO_PB3 | (0x3<<12),
    SD_ADC_GPIO_PB4N = GPIO_PB4 | (0x4<<12),
    SD_ADC_GPIO_PB5N = GPIO_PB5 | (0x5<<12),
    SD_ADC_GPIO_PB6N = GPIO_PB6 | (0x6<<12),
    SD_ADC_GPIO_PB7N = GPIO_PB7 | (0x7<<12),
    SD_ADC_GPIO_PD0N = GPIO_PD0 | (0x8<<12),
    SD_ADC_GPIO_PD1N = GPIO_PD1 | (0x9<<12),
    SD_ADC_GNDN = 0 | (0xb<<12),
}sd_adc_n_input_pin_def_e;

/**
 * @brief sd_adc downsample rate type.
 * @note  Different models of chips support different resolutions, including 12bit and 16bit.
 * The 12-bit chip only support the downsampling rate of 64,
 * The 16-bit chip support the downsampling rate of 64/128/256,
 * chip specific support for which resolution, please refer to the datasheet.
 */
typedef enum{
    SD_ADC_DOWNSAMPLE_RATE_64  = 64,
    SD_ADC_DOWNSAMPLE_RATE_128 = 128,
    SD_ADC_DOWNSAMPLE_RATE_256 = 256,
}sd_adc_downsample_rate_e;

/**
 * @brief sd_adc vbat channel divider
 */
typedef enum{
    SD_ADC_VBAT_DIV_1F4 = 0x01,
    SD_ADC_VBAT_DIV_1F2 = 0x02,
}sd_adc_vbat_div_e;

/**
 * @brief sd_adc positive input channel divider
 */
typedef enum{
    SD_ADC_GPIO_P_CHN_DIV_1F4 = 0x01,
    SD_ADC_GPIO_P_CHN_DIV_1F2 = 0x02,
    SD_ADC_GPIO_P_CHN_DIV_OFF = 0x03,
}sd_adc_gpio_p_chn_div_e;

/**
 * @brief sd_adc negative input channel divider
 */
typedef enum{
    SD_ADC_GPIO_N_CHN_DIV_1F4 = 0x01,
    SD_ADC_GPIO_N_CHN_DIV_1F2 = 0x02,
    SD_ADC_GPIO_N_CHN_DIV_OFF = 0x03,
}sd_adc_gpio_n_chn_div_e;

/**
 * @brief sd_adc gpio mode configuration
 * @note Different models of chips support different resolutions, including 12bit and 16bit.
 * The 12-bit chip only support the sampling clock for 1M, downsampling rate of 64,
 * The 16-bit chip support the sampling clock for 1M/2M, downsampling rate of 64/128/256,
 * chip specific support for which resolution, please refer to the datasheet.
 */
typedef struct{
    sd_adc_p_input_pin_def_e input_p;
    sd_adc_n_input_pin_def_e input_n;
    sd_adc_gpio_p_chn_div_e p_div;
    sd_adc_gpio_n_chn_div_e n_div;
    unsigned char clk_div;
    sd_adc_downsample_rate_e downsample_rate;
}sd_adc_gpio_cfg_t;

typedef enum{
    SD_ADC_VOLTAGE_10X_MV,
	SD_ADC_VOLTAGE_MV,
    TEMP_VALUE,
}sd_adc_result_type_e;

/**
 *  @brief  Define SD ADC VMID power switch state
 */
typedef enum
{
    SD_ADC_VMID_VREF,
    SD_ADC_VBG_VREF,
} sd_adc_vref_e;

/**********************************************************************************************************************
 *                                                  SD_ADC only interface                                                 *
 **********************************************************************************************************************/
/**
 * @brief This function is used to calibrate sd adc sample voltage for single GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_single_gpio_calib_vref(unsigned short vref, signed short offset);

/**
 * @brief This function is used to calibrate sd adc sample voltage for VABT.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short vref, signed short offset);

/**
 * @brief This function is used to calibrate sd adc sample voltage for GPIO differential.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_diff_gpio_calib_vref(unsigned short vref, signed short offset);

/**
 * @brief      This function servers to clear the write pointer of fifo.
 * @param[in]  none
 * @return     none
 */
static inline void sd_adc_clr_fifo_wptr(void)
{
	reg_dfifo_wptr2 = 0;
}
/**
 * @brief      This function serves to set the positive and negative input gpio.
 * @param[in]  p_ain - sd_adc_p_input_pin_def_e
 * @param[in]  n_ain - sd_adc_n_input_pin_def_e
 * @return none
 */
static inline void sd_adc_set_diff_input(sd_adc_p_input_pin_def_e p_ain, sd_adc_n_input_pin_def_e n_ain)
{
    analog_write(areg_sel_ana_input, (n_ain>>12) | (p_ain>>12)<<4);
}

/**
 * @brief     This function servers to set the SD_ADC reference voltage.
 * @param[in] vref - sd_adc_vref_e
 * @return    none
 * @note	  The A0 version chip is unable to utilize this interface.
 */
static inline void sd_adc_select_vref(sd_adc_vref_e vref)
{
    if (g_chip_version != CHIP_VERSION_A0) {

    	if(vref == SD_ADC_VBG_VREF)
    	{
    		analog_write(0x08, analog_read(0x08) | BIT(3));
    	}else
    	{
    		analog_write(0x08, analog_read(0x08) & (~BIT(3)));
    	}
    }
}



/**
 * @brief       This function serves to set the interrupt trigger threshold.
 * @param[in]   threshold - interrupt trigger threshold(unit: word)
 * @return      none
 * @note        -# Interrupt trigger condition: fifo data count > threshold, so the threshold is at least 1 word less than the depth, otherwise the interrupt will not be triggered.
 *              -# threshold must be multiple of 4 and max is 1020.
 */
static inline void sd_adc_set_irq_trig_thres(unsigned int threshold)
{
    reg_dfifon2 = (threshold/4 - 1); //threshold = (reg_dfifon2<<2)+3+1
}

/**
 * @brief      This function serves to set the destination address of the SD_ADC rx fifo and depth.
 * @param[in]  sd_adc_data_buf -Pointer to sd_adc_data_buf
 * @param[in]  depth - rx fifo depth(unit: word)
 * @return     none
 * @note       -# depth must be 2^n (n ranges from 3 to 10): 8/16/32/64/128/256/512/1024, otherwise the threshold trigger interrupt will not be accurate.
 *             -# Interrupt trigger condition: fifo data count > threshold, so the threshold is at least 1 word less than the depth, otherwise the interrupt will not be triggered.
 *             -# The length of sd_adc_data_buf must be >= depth, otherwise there is a risk of array overflow.
 *
 * How SD_ADC Sampling Data storage to buffer works:
 * -# From the buffer first address to start receiving data, data is greater than the threshold, irq status will be set to 1, if the interrupt is enabled,
 *    at the same time will trigger an interrupt, at this time you need to call sd_adc_sample_stop() function to stop the sampling, read out the data in the buffer,
 *    and processed to complete.Then call sd_adc_sample_start() to start the next sampling, or stop the sampling, modify the buffer address, and then start the sampling.
 * -# If the sampling is not stopped or the data in the buffer is not taken out in time, the data will continue to be stored in the buffer
 *    until the fifo depth is reached and the data will be overwritten cyclically from the first address of the buffer.
 */
static inline void sd_adc_set_rx_fifo(unsigned int *sd_adc_data_buf, unsigned int depth)
{
    reg_dfifo_ba2 = (unsigned int)sd_adc_data_buf;
    reg_dfifo_aidx2 = ((reg_dfifo_aidx2 & (~FLD_R_AIDX2)) | ((unsigned int)sd_adc_data_buf >> 16));
    reg_dfifo_depth = (depth/4 - 1); //depth = (reg_dfifo_depth<<2)+3+1
    sd_adc_clr_fifo_wptr();
}

/**
 * @brief      This function serves to enable SD_ADC sample data to be written to sram.
 * @param[in]  none
 * @return     none
 */
static inline void sd_adc_data_to_sram_en(void)
{
    reg_dfifo_dc_mode |= FLD_R_EN_AIN2;
}

/**
 * @brief     This function servers to set SD_ADC irq mask.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_set_irq_mask(void)
{
    reg_dfifo_dc_mode |= FLD_R_MASK;
}

/**
 * @brief     This function servers to clear SD_ADC irq mask.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_clr_irq_mask(void)
{
    reg_dfifo_dc_mode &= ~FLD_R_MASK;
}

/**
 * @brief     This function servers to power on temperature sensor.
 * @param[in] none
 * @return    none
 * @note      The A0 version of the chip temperature sensor is no longer open for use.
 */
static inline void sd_adc_temp_sensor_power_on(void)
{
    analog_write(0x06,analog_read(0x06) & (~BIT(2)));
    if (g_chip_version != CHIP_VERSION_A0)
    {
       analog_write(0x08,analog_read(0x08) | BIT(4));  //Set to 1 to select CTAT(a voltage complementary to absolute temperature)
    }
}

/**
 * @brief      This function servers to power off temperature sensor.
 * @param[in]  none
 * @return     none
 * @note       The A0 version of the chip temperature sensor is no longer open for use.
 */
static inline void sd_adc_temp_sensor_power_off(void)
{
    analog_write(0x06,analog_read(0x06) | BIT(2));
    if (g_chip_version != CHIP_VERSION_A0)
    {
       analog_write(0x08,analog_read(0x08) & (~BIT(4)));
    }
}

/**
 * @brief      This function servers to get sd_adc irq status.
 * @param[in]  none
 * @return     irq status
 * @note       Whether irq mask are set or not, irq status is set to 1 when fifo data count > threshold,
 *             and irq status is automatically set to 0 when fifo data count <= threshold.
 */
static inline unsigned char sd_adc_get_irq_status(void)
{
	return reg_irq_fifo & FLD_FIFO2_MID;
}

/**
 * @brief     This function servers to power on SD_ADC.
 * @param[in] none
 * @return    none
 * @note      the first 4 sampling data anomaly after each calling sd_adc_sample_start().
 */
static inline void sd_adc_sample_start(void)
{
	sd_adc_clr_fifo_wptr();//Make sure that the write pointer is cleared before starting sampling.
    reg_op_mode = (reg_op_mode & (~FLD_OP_MODE)) | 0x01;
}
/**
 * @brief     This function servers to power off SD_ADC.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_sample_stop(void)
{
    reg_op_mode = reg_op_mode & (~FLD_OP_MODE);
}
/**
 * @brief      This function is used to initialize the positive and negative channels for gpio sampling.
 * @param[in]  p_pin - sd_adc_p_input_pin_def_e
 * @param[in]  n_pin - sd_adc_n_input_pin_def_e
 * @return     none
 */
void sd_adc_gpio_pin_init(sd_adc_p_input_pin_def_e p_pin,sd_adc_n_input_pin_def_e n_pin);

/**
 * @brief      This function serves to select Vbat voltage division.
 * @param[in]  div - sd_adc_vbat_div_e.
 * @return     none
*/
void sd_adc_set_vbat_divider(sd_adc_vbat_div_e div);

/**
 * @brief      This function serves to select the positive and negative voltage divider of the gpio.
 * @param[in]  div - sd_adc_vbat_div_e.
 * @return     none
*/
void sd_adc_set_gpio_divider(sd_adc_gpio_p_chn_div_e p_div,sd_adc_gpio_n_chn_div_e n_div);

/**
 * @brief      This function serves to enable vbat function.
 * @param[in]  none
 * @return     none
 */
void sd_adc_vbat_mode_en(void);

/**
 * @brief      This function serves to disable vbat function.
 * @param[in]  none
 * @return     none
 */
void sd_adc_vbat_mode_dis(void);

/**
 * @brief      This function is used to initialize the SD_ADC.
 * @param[in]  none
 * @return     none
 */
void sd_adc_init(void);

/**
 * @brief      This function is used to initialize the SD_ADC for gpio sampling.
 * @param[in]  cfg -Pointer to configure the GPIO channel structure.
 * @return     none
 * @note       If you switch to gpio mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_gpio_sample_init(sd_adc_gpio_cfg_t *cfg);

/**
 * @brief      This function is used to initialize the SD_ADC for vbat sampling.
 * @param[in]  clk_div       - system clock/sd_adc sample clock scale
 * @param[in]  div             - sd_adc_vbat_div_e.
 * @param[in]  downsample_rate - sd_adc_downsample_rate_e
 * @return     none
 * @note       If you switch to vbat mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_vbat_sample_init(unsigned char clk_div, sd_adc_vbat_div_e div, sd_adc_downsample_rate_e downsample_rate);

/**
 * @brief      This function is used to initialize the SD_ADC for Temperature Sensor sampling.
 * @param[in]  clk_div       - system clock/sd_adc sample clock scale
 * @param[in]  downsample_rate -sd_adc_downsample_rate_e
 * @return     none
 * @note       If you switch to temp mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_temp_init(unsigned char clk_div, sd_adc_downsample_rate_e downsample_rate);

/**
 * @brief       This function is used to convert SD_ADC sample code to a voltage.
 * @param[in]   sd_adc_code    - the sd_adc sample code.
 * @param[in]   type       - sd_adc_result_type_e.
 * @return      sd_adc_vol_mv  - the average value of adc voltage value.
 */
signed int sd_adc_calculate_voltage(signed int sd_adc_code,sd_adc_result_type_e type);

/**
 * @brief       This function is used to convert SD_ADC sample code to celsius value.
 * @param[in]   sd_adc_code    - the adc sample code.
 * @return      Celsius value.
 */
signed short sd_adc_calculate_temperature(signed int sd_adc_code);

/**********************************************************************************************************************
 *                                         Audio and SD_ADC common interface                                              *
 **********************************************************************************************************************/
/**
 * @brief     This function servers to power on SD_ADC.
 * @param[in] mode -sd_adc_mode_e
 * @return    none
 * @note      -# After sd_adc_power_on(SD_ADC_SAMPLE_MODE), must wait >160us(when the C10 capacitor on the development board is 10nF) for VMID power to stabilize, otherwise there will be hundreds of sample data will be abnormal.
 *            -# After each call to sd_adc_power_on(SD_ADC_SAMPLE_MODE) and wait >160us(when the C10 capacitor on the development board is 10nF), the first 4 codes sampled by sd_adc may be abnormal and need to be discarded.
 *
 */
void sd_adc_power_on(sd_adc_mode_e mode);

/**
 * @brief     This function servers to power off SD_ADC.
 * @param[in] mode -sd_adc_mode_e
 * @return    none
 * @note      -# sd_adc_power_off must be called before entering pm sleep to prevent abnormal sampling of sd adc after PM wakes up.
 */
void sd_adc_power_off(sd_adc_mode_e mode);

/**
 * @brief This function serves to set SD_ADC op mode.
 * @param[in]  mode - sd_dc_op_mode_e
 * @return none
 */
static inline void sd_adc_set_op_mode(sd_dc_op_mode_e mode)
{
	reg_op_mode = MASK_VAL(FLD_OP_MODE, mode);
}

/**
 * @brief       This function is used to enable the data weighted average algorithm function to improve SD ADC performance.
 * @param[in]   none
 * @return      none
 */
static inline void sd_adc_data_weighted_average_en(void)
{
    analog_write(areg_0x8c, analog_read(areg_0x8c) | FLD_L_DEM_EN);
}

/**
 * @brief      This function servers to clear sd_adc irq status.
 * @param[in]  none
 * @return     none
 */
static inline void sd_adc_clr_irq_status(void)
{
    /**
     * The wptr must be cleared to 0 immediately after entering the interrupt,
     * otherwise the interrupt will be triggered all the time because of the wptr > trigger threshold value.
     */
	sd_adc_clr_fifo_wptr();
	sd_adc_sample_stop();
	reg_irq_fifo |= FLD_R_IRQ;
}

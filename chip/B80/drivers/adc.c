/********************************************************************************************************
 * @file    adc.c
 *
 * @brief   This is the source file for B80
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
#include "adc.h"
#include "analog.h"
#include "clock.h"
#include "dfifo.h"
#include "timer.h"
#include "flash.h"
#include "lib/include/pm.h"
/**
 * Note: When the reference voltage is configured to 1.2V, the calculated ADC voltage value is closest to the actual voltage value using 1175 as the coefficient default.
 * 1175 is the value obtained by ATE through big data statistics, which is more in line with most chips than 1200.
 */
_attribute_data_retention_
volatile unsigned short g_adc_vref = 1175;//ADC calibration value voltage (unit:mV).
_attribute_data_retention_
volatile signed char g_adc_vref_offset = 0;//ADC calibration value voltage offset (unit:mV).
_attribute_data_retention_
unsigned short g_adc_gpio_calib_vref = 1175;//ADC gpio calibration value voltage (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_
signed char g_adc_gpio_calib_vref_offset = 0;//ADC gpio calibration value voltage offset (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_
unsigned short g_adc_vbat_calib_vref = 1175;//ADC vbat calibration value voltage (unit:mV)(used for internal voltage sample).
_attribute_data_retention_
signed char g_adc_vbat_calib_vref_offset = 0;//ADC vbat calibration value voltage offset (unit:mV)(used for vbat voltage sample).
volatile unsigned short	adc_code;
unsigned char   adc_pre_scale;
unsigned char   adc_vbat_divider;

/**
 * @brief This function is used for IO port configuration of ADC IO port voltage sampling.
 *        This interface can be used to switch sampling IO without reinitializing the ADC.
 * @param[in]  pin - adc_input_pin_def_e
 * @return none
 */
void adc_base_pin_init(adc_input_pin_def_e pin)
{
	//ADC GPIO Init
	gpio_set_func(pin&0xfff, AS_GPIO);
	gpio_set_input_en(pin&0xfff,0);
	gpio_set_output_en(pin&0xfff,0);
	gpio_write(pin&0xfff,0);
	adc_set_ain_chn_misc(pin >> 12, GND);
}


/**
 * @brief This function serves to set the channel reference voltage.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @return none
 */
void adc_set_ref_voltage(ADC_RefVolTypeDef v_ref)
{
	//any two channel can not be used at the same time
	adc_set_vref(v_ref);
#if ADC_INTER_TEST
	if(v_ref == ADC_VREF_1P2V)
	{
		//Vref buffer bias current trimming: 		150%
		//Comparator preamp bias current trimming:  100%
		analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(0xC0)) | 0x3d );
	}
	else
	{
		//Vref buffer bias current trimming: 		100%
		//Comparator preamp bias current trimming:  100%
		analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(0xC0)) | 0x15 );
		adc_vref_cfg.adc_vref=900;// v_ref=ADC_VREF_0P9V,
	}
#else
	//Vref buffer bias current trimming: 		150%
	//Comparator preamp bias current trimming:  100%
	analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(0xC0)) | 0x3d );
#endif
}


/**
 * @brief This function serves to set input channel in differential_mode.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  InPCH - enum variable of ADC analog positive input channel.
 * @param[in]  InNCH - enum variable of ADC analog negative input channel.
 * @return none
 */
void adc_set_ain_channel_differential_mode(ADC_InputPchTypeDef InPCH,ADC_InputNchTypeDef InNCH)
{
	adc_set_ain_chn_misc(InPCH, InNCH);
	adc_set_input_mode_chn_misc(DIFFERENTIAL_MODE);
}

/**
 * @brief This function serves to set pre_scaling.
 * @param[in]  v_scl - enum variable of ADC pre_scaling factor.
 * @return none
 */
void adc_set_ain_pre_scaler(ADC_PreScalingTypeDef v_scl)
{
	analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(~FLD_SEL_AIN_SCALE)) | (v_scl<<6) );
	adc_pre_scale = 1<<(unsigned char)v_scl;
}

/**
 * @brief This function serves to ADC init.
 * @param[in]   none
 * @return none
 */
void adc_init(void){
	/******power off sar adc********/
	adc_power_on_sar_adc(0);
	/****** sar adc Reset ********/
	//reset whole digital adc module
	adc_reset_adc_module();
	/******enable signal of 24M clock to sar adc********/
	adc_enable_clk_24m_to_sar_adc(1);
	/******set adc clk as 4MHz******/
	adc_set_sample_clk(5);

	dfifo_disable_dfifo2();//disable misc channel data dfifo

	//
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);//enable the mic channel and set max_state_cnt

	adc_set_input_mode_chn_misc(DIFFERENTIAL_MODE);

	adc_set_resolution_chn_misc(RES14);//set resolution

	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref,

	adc_set_state_length(240, 10);
	adc_set_tsample_cycle_chn_misc(SAMPLING_CYCLES_6);
}

/**
 * @brief This function is used to calib ADC 1.2V vref for GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_gpio_calib_vref(unsigned short vref,signed char offset)
{
	g_adc_gpio_calib_vref = vref;
	g_adc_gpio_calib_vref_offset = offset;
}
/**
 * @brief This function is used to calib ADC 1.2V vref for Vbat.
 * @param[in] vref - Vbat channel sampling calibration value.
 * @param[in] offset - Vbat channel sampling two-point calibration value offset.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short vref,signed char offset)
{
	g_adc_vbat_calib_vref = vref;
	g_adc_vbat_calib_vref_offset = offset;
}

/**
 * @brief This function is used for ADC configuration of ADC IO voltage sampling.
 * @param[in]   pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_init(adc_input_pin_def_e pin)
{
	g_adc_vref = g_adc_gpio_calib_vref;//set adc_vref as adc_gpio_calib_vref
	g_adc_vref_offset = g_adc_gpio_calib_vref_offset;//set adc_vref_offset as adc_gpio_calib_vref_offset
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,
	adc_base_pin_init(pin);
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);//adc scaling factor is 1/8
}


/**
 * @brief This function servers to test ADC temp.When the reference voltage is set to 1.2V, and
 * at the same time, the division factor is set to 1 the most accurate.
 * @param[in]  none.
 * @return     none.
 */
void adc_temp_init(void)
{

	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_set_ain_chn_misc(TEMPERATURE_SENSOR_P_EE, TEMPERATURE_SENSOR_N_EE);

	adc_set_ain_pre_scaler(ADC_PRESCALER_1);//adc scaling factor is 1 or 1/8

	//enable temperature sensor
	analog_write(0x00, (analog_read(0x00)&0xef));

}


/**
 * @brief This function is used for ADC configuration of ADC supply voltage sampling.
 * @param[in]    pin - GPIO_PinTypeDef
 * @return none
 */
void adc_vbat_channel_init(void)
{
	/**
		 * Add Vref calibrate operation.
		 * add by jiarong.20220418.
	*/
	g_adc_vref = g_adc_vbat_calib_vref;//set adc_vref as adc_vbat_calib_vref
	g_adc_vref_offset = g_adc_vbat_calib_vref_offset;//set adc_vref_offset as adc_vbat_calib_vref_offset
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_1F4);

	adc_set_ain_chn_misc(VBAT, GND);

	//set Analog input pre-scaling
	adc_set_ain_pre_scaler(ADC_PRESCALER_1);

}


#define ADC_SAMPLE_NUM		8 //(4 or 8)

/**
 * @brief This function serves to set adc sampling and get results.
 * @param[in]  none.
 * @return the result of sampling.
 */
unsigned int adc_sample_and_get_result(void)
{
	unsigned short temp;
	volatile signed int adc_data_buf[ADC_SAMPLE_NUM];  //size must 16 byte aligned(16/32/64...)

	int i,j;
	unsigned int  adc_vol_mv = 0;
	unsigned short adc_sample[ADC_SAMPLE_NUM] = {0};
	unsigned short  adc_result=0;

	adc_reset_adc_module();

	for(i=0;i<ADC_SAMPLE_NUM;i++){   	//dfifo data clear
		adc_data_buf[i] = 0;
	}
	//dfifo setting will lose in suspend/deep, so we need config it every time
	adc_config_misc_channel_buf((unsigned short *)adc_data_buf, ADC_SAMPLE_NUM<<2);  //size: ADC_SAMPLE_NUM*4
	dfifo_enable_dfifo2();

	unsigned int t0 = clock_time();
	while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
//////////////// get adc sample data and sort these data ////////////////
	for(i=0;i<ADC_SAMPLE_NUM;i++){
		/*wait for new adc sample data, When the data is not zero and more than 1.5 sampling times (when the data is zero),
	 The default data is already ready.*/
		while((!adc_data_buf[i])&&(!clock_time_exceed(t0,25)));
		 t0 = clock_time();
		if(adc_data_buf[i] & BIT(13)){  //14 bit resolution, BIT(13) is sign bit, 1 means negative voltage in differential_mode
			adc_sample[i] = 0;
		}
		else{
			adc_sample[i] = ((unsigned short)adc_data_buf[i] & 0x1FFF);  //BIT(12..0) is valid adc result
		}

		//insert sort
		if(i){
			if(adc_sample[i] < adc_sample[i-1]){
				temp = adc_sample[i];
				adc_sample[i] = adc_sample[i-1];
				for(j=i-1;j>=0 && adc_sample[j] > temp;j--){
					adc_sample[j+1] = adc_sample[j];
				}
				adc_sample[j+1] = temp;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////////
	dfifo_disable_dfifo2();   //misc channel data dfifo disable
///// get average value from raw data(abandon some small and big data ), then filter with history data //////
#if (ADC_SAMPLE_NUM == 4)  	//use middle 2 data (index: 1,2)
	unsigned int adc_average = (adc_sample[1] + adc_sample[2])/2;
#elif(ADC_SAMPLE_NUM == 8) 	//use middle 4 data (index: 2,3,4,5)
	unsigned int adc_average = (adc_sample[2] + adc_sample[3] + adc_sample[4] + adc_sample[5])/4;
#endif
	adc_code=adc_result = adc_average;

	//When the code value is 0, the returned voltage value should be 0.
	if(adc_result == 0){
		return 0;
	}
	else{
		//////////////// adc sample data convert to voltage(mv) ////////////////
		//                          (Vref, adc_pre_scale)   (BIT<12~0> valid data)
		//			 =  adc_result * Vref * adc_pre_scale / 0x2000 + offset
		//           =  adc_result * Vref*adc_pre_scale >>13 + offset
		adc_vol_mv  = ((adc_vbat_divider*adc_result*adc_pre_scale*g_adc_vref)>>13) + g_adc_vref_offset;
	}
	return adc_vol_mv;
}

/**
 * @brief      This function serves to set adc sampling and get results in manual mode for Base and Vbat mode.
 *             In base mode just PB2 PB3 PB4 PC4 can get the right value!If you want to get the sampling results twice in succession,
 *             Must ensure that the sampling interval is more than 2 times the sampling period.
 * @param[in]  none.
 * @return the result of sampling.
 */
unsigned short adc_sample_and_get_result_manual_mode(void)
{
	volatile unsigned char adc_misc_data_L;
	volatile unsigned char adc_misc_data_H;
	volatile unsigned short adc_misc_data;

	analog_write(adc_data_sample_control,analog_read(adc_data_sample_control) | NOT_SAMPLE_ADC_DATA);
	adc_misc_data_L = analog_read(areg_adc_misc_l);
	adc_misc_data_H = analog_read(areg_adc_misc_h);
	analog_write(adc_data_sample_control,analog_read(adc_data_sample_control) & (~NOT_SAMPLE_ADC_DATA));

	adc_misc_data = (adc_misc_data_H<<8 | adc_misc_data_L);

	if(adc_misc_data & BIT(13)){
		adc_misc_data=0;
	}
	else{
		adc_misc_data &= 0x1FFF;
	}

	return adc_misc_data;
}


/**
 * @brief This function serves to get adc sampling temperature results.
 * @param[in]  none.
 * @return the result of temperature.
 */
/********************************************************************************************
 	adc_vol_mv = (883.98-((adc_result * 1200)>>13))/1.4286-40;
			   = (883.98-((adc_result * 1200)>>13))/(14286/10000)-40;
			   = (8839800-((adc_result * 1200)>>13)*10000)/14286-40;
			   = 619 - (adc_result * 1200)>>13)/14286-40;
			   = 619 - (adc_result * 840)>>13)-40;
			   = 579 - (adc_result * 840)>>13);
 ********************************************************************************************/
signed short adc_temp_result(void)
{
	signed short  adc_temp_value = 0;

	adc_sample_and_get_result();

	adc_temp_value = (signed short)(579-((adc_code * 840)>>13));

	return adc_temp_value;
}







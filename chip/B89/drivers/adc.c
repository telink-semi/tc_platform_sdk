/********************************************************************************************************
 * @file	adc.c
 *
 * @brief	This is the source file for B89
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
#include "adc.h"
#include "analog.h"
#include "clock.h"
#include "dfifo.h"
#include "timer.h"
#include "lib/include/pm.h"
/**
 * Note: When the reference voltage is configured to 1.2V, the calculated ADC voltage value is closest to the actual voltage value using 1175 as the coefficient default.
 * 1175 is the value obtained by ATE through big data statistics, which is more in line with most chips than 1200.
 */
_attribute_data_retention_
adc_vref_ctr_t adc_vref_cfg = {
	.adc_vref 		= 1175, //default ADC ref voltage (unit:mV)
	.adc_calib_en	= 1, 	//default disable
};

volatile unsigned char adc_sample_num = 8;

volatile unsigned short	adc_code;
unsigned char   adc_pre_scale;
unsigned char   adc_vbat_divider;
unsigned short adc_ref_vol[2] 	= {600,900};
GPIO_PinTypeDef ADC_GPIO_tab[10] = {
		GPIO_PB0,GPIO_PB1,
		GPIO_PB2,GPIO_PB3,
		GPIO_PB4,GPIO_PB5,
		GPIO_PB6,GPIO_PB7,
		GPIO_PC4,GPIO_PC5
};

/**
 * @brief This function is used for IO port configuration of ADC IO port voltage sampling.
 *        This interface can be used to switch sampling IO without reinitializing the ADC.
 * @param[in]  pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_pin_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_num=0;
	//ADC GPIO Init
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,0);
	gpio_write(pin,0);

	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_num = i+1;
			break;
		}
	}
	adc_set_ain_channel_differential_mode(gpio_num, GND);
}


/**
 * @brief This function serves to set the channel reference voltage.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @return none
 */
void adc_set_ref_voltage(ADC_RefVolTypeDef v_ref)
{
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
		//0.9V and 1.2V are used as reference voltages, GPIO sampling/VBAT Channel/Temp sampling have been tested, and 0.6V has not been tested.
		adc_vref_cfg.adc_vref=adc_ref_vol[v_ref];
	}
#else
	//Vref buffer bias current trimming: 		150%
	//Comparator preamp bias current trimming:  100%
	analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(0xC0)) | 0x3d );
#endif
}

/**
 * @brief This function serves to set resolution.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  v_res - enum variable of ADC resolution.
 * @return none
 */
void adc_set_resolution(ADC_ResTypeDef v_res)
{
	adc_set_resolution_chn_misc(v_res);
}

/**
 * @brief This function serves to set sample_cycle.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  adcST - enum variable of ADC Sampling cycles.
 * @return none
 */
void adc_set_tsample_cycle(ADC_SampCycTypeDef adcST)
{
	adc_set_tsample_cycle_chn_misc(adcST);
}

/**
 * @brief This function serves to set input_mode.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  m_input - enum variable of ADC channel input mode.
 * @return none
 */
void adc_set_input_mode(ADC_InputModeTypeDef m_input)
{
	adc_set_input_mode_chn_misc(m_input);
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
	//setting adc_sel_atb ,if stat is 0,clear adc_sel_atb,else set adc_sel_atb[0]if(stat)
	unsigned char tmp;
	if(v_scl)
	{
		//ana_F9<4> must be 1
		tmp = analog_read(0xF9);
		tmp = tmp|0x10;                    //open tmp = tmp|0x10;
		analog_write (0xF9, tmp);
	}
	else
	{
		//ana_F9 <4> <5> must be 0
		tmp = analog_read(0xF9);
		tmp = tmp&0xcf;
		analog_write (0xF9, tmp);
	}
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

}

/**
 * @brief This function is used for ADC configuration of ADC IO voltage sampling.
 * @param[in]   pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_init(GPIO_PinTypeDef pin)
{
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);//enable the mic channel and set max_state_cnt
	adc_set_state_length(240, 10);  	//set R_max_mc=240,R_max_s=10
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_base_pin_init(pin);
	adc_set_resolution(RES14);
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);
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

	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);//enable the mic channel and set max_state_cnt
	adc_set_state_length(240, 10);  	//set R_max_mc=240,R_max_s=10

	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_set_ain_channel_differential_mode(TEMPERATURE_SENSOR_P_EE, TEMPERATURE_SENSOR_N_EE);
	adc_set_resolution(RES14);
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);
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
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);
	adc_set_state_length(240, 10);  	//set R_max_mc,R_max_c,R_max_s

	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_1F3);

	adc_set_ain_channel_differential_mode(VBAT, GND);
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref

	adc_set_resolution(RES14);//set resolution
	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);

	//set Analog input pre-scaling
	adc_set_ain_pre_scaler(ADC_PRESCALER_1);

}

/**
 * @brief 		This function serves to set adc sampling number.
 * 				The recommended number of samples is 8 normally.If the number of samples is less than 8, it will cause insufficient ADC measurement accuracy.
 * 				The number of samples must be 64 during DCDC calibration.If the number of samples is less than 64, it will cause insufficient DCDC calibration accuracy.
 * 				And the size of the array is also restricted in the API(adc_sample_and_get_result).
 * 				The default number of samples is 8.
 * 				After calibrating the DCDC, you need to call the interface once,set adc_sample_num to 8;
 * @param[in]  	sample_num - the ADC sample number.
 * @return 		none.
 */
void adc_set_sample_num(unsigned char sample_num)
{
	if(sample_num > 64){
		adc_sample_num = 64;
	}
	else{
		adc_sample_num = sample_num;
	}
}

/**
 * @brief 		This function serves to set adc sampling and get results.
 * @param[in]  	none.
 * @return 		the result of sampling.
 */
unsigned int adc_sample_and_get_result(void)
{
	unsigned short temp;
	volatile signed int adc_data_buf[64];  //size must 16 byte aligned(16/32/64...)

	int i,j;
	unsigned int  adc_vol_mv = 0;
	unsigned short adc_sample[64] = {0};
	unsigned short  adc_result=0;

	adc_reset_adc_module();

	for(i=0;i<adc_sample_num;i++){   	//dfifo data clear
		adc_data_buf[i] = 0;
	}
	//dfifo setting will lose in suspend/deep, so we need config it every time
	adc_config_misc_channel_buf((unsigned short *)adc_data_buf, adc_sample_num<<2);  //size: adc_sample_num*4
	dfifo_enable_dfifo2();

	unsigned int t0 = clock_time();
	//if the sampling rate is not 96k,the waiting time must be modified.
	while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us).
//////////////// get adc sample data and sort these data ////////////////
	for(i=0;i<adc_sample_num;i++){
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
	unsigned int adc_average = 0;
	//get average value from raw data(abandon 1/4 small and 1/4 big data)
	for (i = adc_sample_num>>2; i < (adc_sample_num - (adc_sample_num>>2)); i++)
	{
		adc_average += adc_sample[i]/(adc_sample_num>>1);
	}
	adc_code=adc_result = adc_average;

	 //////////////// adc sample data convert to voltage(mv) ////////////////
	//                          (Vref, adc_pre_scale)   (BIT<12~0> valid data)
	//			 =  adc_result * Vref * adc_pre_scale / 0x2000
	//           =  adc_result * Vref*adc_pre_scale >>13
	adc_vol_mv  = (adc_vbat_divider*adc_result*adc_pre_scale*adc_vref_cfg.adc_vref)>>13;

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
 Temperature and adc_code are linearly related. From the test results of three chips between -40~130 (Celsius) got an average relationship:
 			adc_vol_mv = 508 - ((adc_result * 767)>>13);
 ********************************************************************************************/
signed short adc_temp_result(void)
{
	signed short  adc_temp_value = 0;

	adc_sample_and_get_result();

	adc_temp_value = (signed short)(508 - ((adc_code * 767)>>13));

	return adc_temp_value;
}
/**
 * @brief      This function serves to calibrate the output voltage of DCDC between 2.45v and 2.55v.
 * 				This interface is only called once in pm.c.This interface will modify the ADC configuration.
 * @param[in]  none.
 * @return voltage - DCDC voltage value after calibration.
 */
unsigned short adc_set_dcdc_2p5v(void)
{
//1.ADC init
	adc_init();
//2.initialize dcdc sampling channel
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);
	adc_set_state_length(240, 10);  	//set R_max_mc,R_max_c,R_max_s
	//set Vbat divider select,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);
	//set channel mode and channel
	//The PIN that can be used for ADC sampling is PC[5:4] and PB[7:0]
	//It is best to choose a PIN that is not encapsulated.
	gpio_set_func(GPIO_PC5, AS_GPIO);
	gpio_set_input_en(GPIO_PC5,0);
	gpio_set_output_en(GPIO_PC5,1);
	gpio_write(GPIO_PC5,1);
	adc_set_ain_channel_differential_mode(10, GND);
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref
	adc_set_resolution(RES14);//set resolution
	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);
	//set Analog input pre-scaling and
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);

//3.power on adc
	adc_power_on_sar_adc(1);		//After setting the ADC parameters, turn on the ADC power supply control bit

/* The total sampling time must be greater than the period of the DCDC harmonic.
 * The frequency of DCDC harmonics increases as the load increases.
 * The sampling times cannot be less than 64 times during DCDC calibration.*/
//4.set sample number to 64
	adc_set_sample_num(64);

//5.calibrate DCDC to 2p5v
	//DCDC analog register 0x09[6:4] Default 100(2.5v),Each unit represents 50mv
	unsigned short dcdc_output_mv_max = 2550;//2.55v
	unsigned short dcdc_output_mv_min = 2450;//2.45v
	unsigned short dcdc_output_mv_2p5 = 2500;//2.5v
	unsigned short voltage;
	unsigned char cnt = 0;
	signed char calib_value = (analog_read(0x09) & 0x70)>>4;
	while(1){
		//This cycle can be calibrated up to 6 times.Avoid endless loops.
		if(cnt > 6){
			return voltage;
		}
		//DCDC calibration is successful or no need to calibrate DCDC, return the voltage value of DCDC.
		voltage = adc_sample_and_get_result();
		if((voltage >= dcdc_output_mv_min) && (voltage <= dcdc_output_mv_max)){
			return voltage;
		}
		else if(voltage < dcdc_output_mv_min){
			//The DCDC calibration has reached the maximum value, and the DCDC voltage value is returned
				if(7 == calib_value){
					return voltage;
				}
				//DCDC first calibration
				if(0 == cnt){
					calib_value  += ((dcdc_output_mv_2p5 - voltage)/50);
				}else{
					calib_value  += 1;
				}
				if(calib_value > 7){
					calib_value = 7;
				}
		}
		else if(voltage > dcdc_output_mv_max){
			//The DCDC calibration has reached the minimum value, and the DCDC voltage value is returned
				if(0 == calib_value){
					return voltage;
				}
				//DCDC first calibration
				if(0 == cnt){
					calib_value  -= ((voltage - dcdc_output_mv_2p5)/50);
				}else{
					calib_value  -= 1;
				}
				if(calib_value < 0){
					calib_value = 0;
				}
		}
			pm_set_dcdc_2p5(calib_value);
			cnt++;
		}
//6.power down adc
	adc_power_on_sar_adc(0);		//After calibrate DCDC, shut down the ADC power supply control bit

//7.set to the default value of 8
	adc_set_sample_num(8);
}

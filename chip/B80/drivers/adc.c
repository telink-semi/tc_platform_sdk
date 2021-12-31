/********************************************************************************************************
 * @file	adc.c
 *
 * @brief	This is the source file for b80
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
#include "adc.h"
#include "analog.h"
#include "clock.h"
#include "dfifo.h"
#include "timer.h"
#include "flash.h"
#include "lib/include/pm.h"
_attribute_data_retention_
adc_vref_ctr_t adc_vref_cfg = {
	.adc_vref 		= 1175, //default ADC ref voltage (unit:mV)
	.adc_calib_en	= 1, 	//default disable
};
volatile unsigned short	adc_code;
unsigned char   adc_pre_scale;
unsigned char   adc_vbat_divider;

/**
 * @brief This function is used for IO port configuration of ADC IO port voltage sampling.
 * @param[in]  pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_pin_init(GPIO_PinTypeDef pin)
{
	//ADC GPIO Init
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,0);
	gpio_write(pin,0);
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
 * @brief This function is used for ADC configuration of ADC IO voltage sampling.
 * @param[in]   pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_init(adc_input_pin_def_e pin)
{
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_base_pin_init(pin&0xfff);		//ADC GPIO Init

	adc_set_ain_chn_misc(pin >> 12, GND);
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

	adc_set_ain_chn_misc(TEMSENSORP_EE, TEMSENSORN_EE);

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







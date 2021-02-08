/********************************************************************************************************
 * @file	adc.c
 *
 * @brief	This is the source file for b87
 *
 * @author	Driver Group
 * @date	2019
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
#include "adc.h"
#include "analog.h"
#include "clock.h"
#include "dfifo.h"
#include "timer.h"
#include "flash.h"
#include "pm.h"
_attribute_data_retention_
adc_vref_ctr_t adc_vref_cfg = {
	.adc_vref 		= 1175, //default ADC ref voltage (unit:mV)
	.adc_calib_en	= 1, 	//default disable
};
_attribute_data_retention_
unsigned short adc_gpio_calib_vref = 1175;//ADC gpio calibration value voltage (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_
unsigned short adc_vbat_calib_vref = 1175;//ADC vbat calibration value voltage (unit:mV)(used for internal voltage sample).
volatile unsigned short	adc_code;
unsigned char   adc_pre_scale;
unsigned char   adc_vbat_divider;
GPIO_PinTypeDef ADC_GPIO_tab[10] = {
		GPIO_PB0,GPIO_PB1,
		GPIO_PB2,GPIO_PB3,
		GPIO_PB4,GPIO_PB5,
		GPIO_PB6,GPIO_PB7,
		GPIO_PC4,GPIO_PC5
};

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
 * @brief This function is used for IO port configuration of ADC supply voltage sampling.
 * @param[in]  pin - GPIO_PinTypeDef
 * @return none
 */
void adc_vbat_pin_init(GPIO_PinTypeDef pin)
{
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,1);
	gpio_write(pin,1);
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
 * @brief This function is used to calib ADC 1.2V vref.
 * @param[in] none
 * @return none
 */
/********************************************************************************************
	There have two kind of calibration value of ADC 1.2V vref in flash,and one calibration value in Efuse.
	Two kind of ADC calibration value in flash are adc_vbat_calib_vref(used for internal voltage sample)
	and adc_gpio_calib_vref(used for gpio voltage sample).The ADC calibration value in Efuse is
	adc_vbat_calib_vref(used for internal voltage sample).
	The priority of adc_gpio_calib_vref is: Flash > Default(1175mV).
	The priority of adc_vbat_calib_vref is: Flash > Efuse >Default(1175mV).
	The limit error scope of Flash adc_vbat_calib_vref is 10mV, Flash adc_gpio_calib_vref is 30mV.
********************************************************************************************/
void adc_update_1p2_vref_calib_value(void)
{
	unsigned char flash_mid[3];
	unsigned char flash_uid[16];
	unsigned char flash_mid_sure = 0;
	unsigned int adc_vref_calib_flash_addr = 0;
	unsigned char adc_vref_calib_value_rd[4] = {0};
	unsigned char calib_vref_ok = 1;

	if (adc_vref_cfg.adc_calib_en == 1)
	{
		/******check for flash mid********/
		flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);
		/******find the address of adc calibration value in flash********/
		if ((flash_mid_sure == 1))
		{
			switch (flash_mid[2])
			{
				case 0x10:
					adc_vref_calib_flash_addr = 0xe0c0;
					break;
				case 0x11:
					adc_vref_calib_flash_addr = 0x1e0c0;
					break;
				case 0x13:
					adc_vref_calib_flash_addr = 0x770c0;
					break;
				case 0x14:
					adc_vref_calib_flash_addr = 0xfe0c0;
					break;
			}
			if (adc_vref_calib_flash_addr != 0)
			{
				/******read 4 byte calibration value from flash********/
				flash_read_page(adc_vref_calib_flash_addr, 4, adc_vref_calib_value_rd);
				if ((adc_vref_calib_value_rd[0] != 0xff) || (adc_vref_calib_value_rd[1] != 0xff ))//if there has calibration value.
				{
					/******Method of calculating calibration Flash_gpio_Vref value: ********/
					/******Vref = [1175 +First_Byte-255+Second_Byte] mV =  [920 + First_Byte + Second_Byte] mV  ********/
					adc_gpio_calib_vref = 920 + adc_vref_calib_value_rd[0] + adc_vref_calib_value_rd[1];
				}
				else
				{
					calib_vref_ok = 0;
				}

				if ((adc_vref_calib_value_rd[2] != 0xff) || (adc_vref_calib_value_rd[3]  != 0xff ))//if there has calibration value.
				{
					/******Method of calculating calibration Flash_vbat_Vref value: ********/
					/******Vref = [1175 +First_Byte-255+Second_Byte] mV =  [920 + First_Byte + Second_Byte] mV  ********/
					adc_vbat_calib_vref = 920 + adc_vref_calib_value_rd[2] + adc_vref_calib_value_rd[3];
				}
				else
				{
					calib_vref_ok = 0;
				}
				/******Check the calibration value whether is correct********/
				if ((adc_vbat_calib_vref < 1047) || (adc_vbat_calib_vref > 1302) || (adc_gpio_calib_vref < 1047) || (adc_gpio_calib_vref > 1302))
				{
					calib_vref_ok = 0;
				}
			}
			else
			{
				calib_vref_ok = 0;
			}
		}
		else
		{
			calib_vref_ok = 0;
		}
		/******if flash do not exist the calibration value or the value is incorrect or check mid fail,use the Efuse calibration value********/
		if (calib_vref_ok == 0)
		{
			adc_gpio_calib_vref = 1175;
			//////////////////// get Efuse bit32~63 info ////////////////////
			unsigned int  efuse_32to63bit_info = pm_get_info1();
			if((efuse_32to63bit_info&0xff))//if there has calibration value.
			{
				unsigned short adc_vref_calib_idx = efuse_32to63bit_info & 0xff; //ADC Ref: efuse bit32~bit39 8bits.
				adc_vbat_calib_vref = 1047 + adc_vref_calib_idx ; //ADC ref voltage: g_adc_vref (unit: mV)
			}
		}
	}
}

/**
 * @brief This function is used for ADC configuration of ADC IO voltage sampling.
 * @param[in]   pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_num=0;
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);//enable the mic channel and set max_state_cnt
	adc_set_state_length(240, 10);  	//set R_max_mc=240,R_max_s=10
	/**
		 * Add Vref calibrate operation.
		 * add by chaofan.20201029.
	*/
	adc_vref_cfg.adc_vref = adc_gpio_calib_vref;//set adc_vref as adc_gpio_calib_vref
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_base_pin_init(pin);		//ADC GPIO Init

	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_num = i+1;
			break;
		}
	}
	adc_set_ain_channel_differential_mode(gpio_num, GND);
	adc_set_resolution(RES14);
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);//adc scaling factor is 1/8
}


#if ADC_OLD_TEMP_TEST
/**
 * @brief This function servers to test ADC temp.When the reference voltage is set to 1.2V, and
 * at the same time, the division factor is set to 1 the most accurate.
 * @param[in]  none.
 * @return     none.
 */
void adc_old_temp_init(void)
{

	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);//enable the mic channel and set max_state_cnt
	adc_set_state_length(240, 10);  	//set R_max_mc=240,R_max_s=10
	/**
		 * Add Vref calibrate operation.
		 * add by chaofan.20201029.
	*/
	adc_vref_cfg.adc_vref = adc_vbat_calib_vref;//set adc_vref as adc_vbat_calib_vref
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_set_ain_channel_differential_mode(TEMSENSORP, TEMSENSORN);
	adc_set_resolution(RES14);
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);
	adc_set_ain_pre_scaler(ADC_PRESCALER_1);//adc scaling factor is 1 or 1/8

	//enable temperature sensor
	analog_write(0x06, (analog_read(0x06)&0xfb));

}
#endif


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
	/**
		 * Add Vref calibrate operation.
		 * add by chaofan.20201029.
	*/
	adc_vref_cfg.adc_vref = adc_vbat_calib_vref;//set adc_vref as adc_vbat_calib_vref
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,

	adc_set_ain_channel_differential_mode(TEMSENSORP_EE, TEMSENSORN_EE);
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
void adc_vbat_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_no=0;
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);
	adc_set_state_length(240, 10);  	//set R_max_mc,R_max_c,R_max_s
	/**
		 * Add Vref calibrate operation.
		 * add by chaofan.20201029.
	*/
	adc_vref_cfg.adc_vref = adc_gpio_calib_vref;//set adc_vref as adc_gpio_calib_vref
	//set Vbat divider select,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);
	//set channel mode and channel
	adc_vbat_pin_init(pin);
	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_no = i+1;
			break;
		}
	}
	adc_set_ain_channel_differential_mode(gpio_no, GND);
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref
	adc_set_resolution(RES14);//set resolution
	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);

	//set Analog input pre-scaling and
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);
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
	/**
		 * Add Vref calibrate operation.
		 * add by chaofan. 20201029.
	 */
	adc_vref_cfg.adc_vref = adc_vbat_calib_vref;//set adc_vref as adc_vbat_calib_vref
	//set Vbat divider select,
	/**
		 * Change the ADC_VBAT_DIVIDER as 1F3 and ADC_PRESCALER as 1.
		 * Because ATE calibrate the Vbat Vref with ADC_VBAT_DIVIDER_1F3 and ADC_PRESCALER_1.
		 * It may be inaccurate if you use other parameters
		 * changed by chaofan. confirmed by zhengting. 20201029.
	 */
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_1F3);

	adc_set_ain_channel_differential_mode(VBAT, GND);
	adc_set_ref_voltage(ADC_VREF_1P2V);//set channel Vref

	adc_set_resolution(RES14);//set resolution
	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(SAMPLING_CYCLES_6);

	//set Analog input pre-scaling
	adc_set_ain_pre_scaler(ADC_PRESCALER_1);

}


#define ADC_SAMPLE_NUM		8 //8 /4

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
	/**
		 * move the "2 sample cycle" wait operation after adc dfifo enable,
		 * otherwise may have data lose due to the adc and dfifo mechanism.
		 * changed by chaofan.20201029.
	 */
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
	/**
		 * move the return value adc_misc_data to the function end.Otherwise may don't do the return operation.
		 * changed by chaofan.20201029.
	 */
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







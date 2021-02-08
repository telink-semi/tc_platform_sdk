/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
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
#include "app_config.h"

volatile unsigned short base_val;
volatile unsigned short vbat_val;
volatile signed short temp_new_val;
volatile unsigned short vbat_channel_val;
volatile unsigned short adc_manual_val;
volatile unsigned int rns_val;

void user_init()
{

#if(ADC_MODE==ADC_RNG_MODE)
	 random_generator_init();
#else
	adc_init();

	#if(ADC_MODE==ADC_BASE_MODE)
		adc_base_init(GPIO_PB0);
	 adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);//ADC pre_scaling default value is ADC_PRESCALER_1F8, it can change after adc_base_init().
	#elif (ADC_MODE==ADC_VBAT_MODE)
		adc_vbat_init(GPIO_PB0);
    #elif (ADC_MODE==ADC_VBAT_CHANNEL_MODE)
		adc_vbat_channel_init();
    #elif (ADC_MODE==ADC_TEMP_MODE_EE)
		adc_temp_init();
	#endif

	adc_power_on_sar_adc(1);		//After setting the ADC parameters, turn on the ADC power supply control bit

#endif

}


void main_loop (void)
{

#if(ADC_MODE==ADC_RNG_MODE)
	rns_val = rand();

#else

	#if(ADC_MODE==ADC_BASE_MODE)
		base_val = adc_sample_and_get_result();
	#elif (ADC_MODE==ADC_VBAT_MODE)
		vbat_val = adc_sample_and_get_result();
    #elif (ADC_MODE==ADC_VBAT_CHANNEL_MODE)
		vbat_channel_val = adc_sample_and_get_result();
    #elif (ADC_MODE==ADC_TEMP_MODE_EE)
		temp_new_val = adc_temp_result();
	#endif

    #if(MANNUAL_MODE_GET_ADC_SAMPLE_RESULT==1)
		adc_manual_val = adc_sample_and_get_result_manual_mode();
    #endif

#endif

}



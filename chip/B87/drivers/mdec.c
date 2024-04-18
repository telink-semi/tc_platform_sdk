/********************************************************************************************************
 * @file    mdec.c
 *
 * @brief   This is the source file for B87
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "mdec.h"

/**
 * @brief This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]    pin - MDEC_PinTypeDef. In order to distinguish which pin the data is input from, 
                 only one input pin can be selected one time.
 * @return none
 */
void mdec_init(MDEC_PinTypeDef pin)
{
	analog_write(0x2d, analog_read(0x2d) & (~BIT(7)));    //turn to 32K_RC
	rc_32k_cal();

    if(pin==GPIO_PA0_MDEC){
		analog_write(mdec_rst_addr,(analog_read(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PA0 );//A0
	}
    if(pin==GPIO_PB7_MDEC){
	    analog_write(mdec_rst_addr,(analog_read(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PB7 );//B7
    }
    if(pin==GPIO_PC4_MDEC){
    	analog_write(mdec_rst_addr,(analog_read(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PC4 );//C4
    }
    if(pin==GPIO_PD0_MDEC){
    	analog_write(mdec_rst_addr,(analog_read(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PD0 );//D0
   }
}

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat)
{

	unsigned char m0,m1,m2,data_crc;

	dat[0]=analog_read(0x51); //0x51
	dat[1]=analog_read(0x52); //0x52
	dat[2]=analog_read(0x53); //0x53
	dat[3]=analog_read(0x54); //0x54
	dat[4]=analog_read(0x55); //0x55

	m0= ((dat[0]>>5)<<4);
	m1= dat[0]&0x07;
	m2= m0+m1;
	data_crc=(((m2+dat[1])^dat[2])+dat[3])^0xa5;
	write_reg32(reg_irq_src, read_reg32(reg_irq_src) | FLD_IRQ_PM_EN);
	if(data_crc==dat[4]){
		return 1;
	}
	return 0;
}












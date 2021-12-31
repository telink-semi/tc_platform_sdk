/********************************************************************************************************
 * @file	mdec.c
 *
 * @brief	This is the source file for b87
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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












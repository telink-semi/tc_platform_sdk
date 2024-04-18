/********************************************************************************************************
 * @file    qdec.c
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
#include "qdec.h"

/**
 * @brief      This function servers to set input port.
 * @param[in]  chn_a - input types of a channel.
 * @param[in]  chn_b - input types of b channel.
 * @return     none.
 */
void qdec_set_pin(QDEC_InputAchTypeDef chn_a,QDEC_InputBchTypeDef chn_b)
{
	reg_qdec_channel_a = chn_a;
	reg_qdec_channel_b = chn_b;
}

/**
 * @brief      This function servers to set qdec mode,qdec mode:common mode and double accuracy mode.
 *             common mode:the qdec counter value is increased/decreased by 1 only when the same rising/falling edges are detected from the two phase signals.
 *             double accuracy mode:the qdec counter value is increased/decreased by 1 on each rising/falling edge of the two phase signals;
 *             the counter value will be increased/decreased by 2 for one wheel rolling.
 * @param[in]  mode - mode type to select.
 * @return     none.
 */
void qdec_set_mode(qdec_mode_e mode)
{
	reg_qdec_mode= (reg_qdec_mode&0xfe)|mode;
}

/**
 * @brief      This function servers to initials qedc source clock.
 * @param[in]  none.
 * @return     none.
 */
void qdec_clk_en(void)
{
	rc_32k_cal();
	BM_SET(reg_clk_en0,FLD_CLK0_QDEC_EN);
}

/**
 * @brief      This function servers to read hardware counting value,
 *             After reading the value, if there is no input to channels A and B, then reading the value is 0 through this function,
 *             If the count is not read, the qdec read real time counting value increases or decreases with the direction of the wheel according to the qdec mode.
 * @param[in]  none.
 * @return     hardware counting value.
 */
signed char qdec_get_count_value(void)
{
	//before reading the hardware counting value,write 1 to reg_qdec_load.
	reg_qdec_load |= FLD_QDEC_COUNT0_RELOAD;
	return reg_qdec_count0;

}

/**
 * @brief      This function servers to reset qdec and the qdec counter value is cleared zero.
 * @param[in]  none.
 * @return     none.
 */
void qdec_reset(void)
{
	reg_rst0 &= ~(FLD_RST0_QDEC);
	reg_rst0 |=  FLD_RST0_QDEC;
}

/**
 * @brief      This function servers to set hardware debouncing.
 * @param[in]  thrsh - any signal with width lower than the threshold will be regarded as jitter,
 *             effective signals input from Channel A and B should contain high/low level with width more than the threshold.
 * @return     none.
 */
void qdec_set_debouncing(qdec_thrsh_e thrsh)
{
	//the threshold: 2^(n+1) *clk_32kHz *3 (n=0xd1[2:0]))
	reg_qdec_dbntime = ((reg_qdec_dbntime &(~FLD_QDEC_DBNTIME))|(thrsh&0x07));
}





/********************************************************************************************************
 * @file    pa.h
 *
 * @brief   This is the header file for Telink MCU
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef PA_H_
#define PA_H_

#ifdef RF_CERTIFICATION_CGF_EN
    #define PA_SUPPORT_USR_CONFIG 1
#else
    #define PA_SUPPORT_USR_CONFIG 0
#endif

/**
 * @brief        Enumeration type of PA state.
 */
typedef enum
{
    PA_SETTING_STATE_INIT   = 0x01,
    PA_SETTING_STATE_TX     = 0x02,
    PA_SETTING_STATE_RX     = 0x03,
    PA_SETTING_STATE_BYPASS = 0x04,
} pa_setting_state_e;

#if (PA_SUPPORT_USR_CONFIG)

/**
 * @brief        This function serves to initialize PA setting.
 * @param[in]   addr       - Address to save PA related operations.
 * @param[in]   bypass_en  - To set PA bypass mode, 0:disable 1:enable.
 * @return         none
 */
extern void pa_setting_init(unsigned int addr, unsigned char bypass_en);

#else

/**
 * @brief        This function serves to initialize PA setting.
 * @param[in]   pa_en  - To set PA enable, 0:disable 1:enable.
 * @param[in]   bypass_en  - To set PA bypass mode, 0:disable 1:enable.
 * @return         none
 */
extern void pa_setting_init(unsigned char pa_en, unsigned char bypass_en);

#endif

/**
 * @brief        This function serves to execute PA operations according to the state.
 * @param[in]   state      - PA working state.
 * @return         none
 */
extern void pa_operation(pa_setting_state_e state);


#endif /* PA_H_ */

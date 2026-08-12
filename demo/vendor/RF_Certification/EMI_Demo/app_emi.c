/********************************************************************************************************
 * @file    app_emi.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "app_config.h"
#include "calibration.h"
#include "../common/rf_certification.h"

#if defined(MCU_CORE_B80)
    #define SRAM_BASE_ADDR 0x840000
#elif defined(MCU_CORE_B80B)
    #define SRAM_BASE_ADDR 0x840000
#elif defined(MCU_CORE_B85)
    #define SRAM_BASE_ADDR 0x840000
#elif defined(MCU_CORE_B87)
    #define SRAM_BASE_ADDR 0x840000
#elif defined(MCU_CORE_TC321X)
    #define SRAM_BASE_ADDR 0x840000
#elif defined(MCU_CORE_TC122X)
    #define SRAM_BASE_ADDR 0x840000
#else
    #define SRAM_BASE_ADDR 0x00000000
#endif

#define RSSI_ADDR              (SRAM_BASE_ADDR + 0x4)
#define TX_PACKET_MODE_ADDR    (SRAM_BASE_ADDR + 0x5)
#define RUN_STATUE_ADDR        (SRAM_BASE_ADDR + 0x6)
#define TEST_COMMAND_ADDR      (SRAM_BASE_ADDR + 0x7)
#define POWER_ADDR             (SRAM_BASE_ADDR + 0x8)
#define CHANNEL_ADDR           (SRAM_BASE_ADDR + 0x9)
#define RF_MODE_ADDR           (SRAM_BASE_ADDR + 0xa)
#define EMI_CONFIG_FLAG_ADDR   (SRAM_BASE_ADDR + 0xb)
#define RX_PACKET_NUM_ADDR     (SRAM_BASE_ADDR + 0xc)

#define MAX_RF_CHANNEL         40
#define MAX_RF_TONE_CHANNEL    72
#define MAX_NOISE_VALUE        -70
#define READ_RSSI_TIMES        100
#define RSSI_COPENSATION_VALUE 0

emi_setting_t g_emi_setting = {
    .access_code = ACCESS_CODE,
#ifdef RF_CERTIFICATION_CGF_EN
    .pa_addr = 0,
#endif
    .cal_pos              = SWITCH_CALI_POSITION,
    .power_mode           = SWITCH_POWER_MODE,
    .io_voltage           = SWITCH_GPIO_VOLTAGE,
    .pa_en                = PA_FUNCTION_ENABLE,
    .pa_bypass_en         = PA_BYPASS_ENABLE,
    .swire_through_usb_en = SWIRE_THROUGH_USB_ENABLE,
    .cap                  = SWITCH_INTERNAL_CAP,
};

emi_cmd_t emi_cmd_now =
    {
        .g_rssi_val                     = 0,
        .g_tx_packet_cnt                = EMI_TX_PACKET_CNT,
        .g_run_state                    = EMI_RUN_STATE,
        .g_test_cmd_now                 = EMI_TEST_CMD_NOW,
        .g_power_level                  = EMI_POWER_LEVEL,
        .g_chn                          = EMI_RF_CHN,
        .g_rf_mode                      = EMI_RF_MODE,
        .g_hop_en                       = EMI_HOPPING_EN,
        .g_adaptive_en                  = EMI_ADAPTIVE_EN,
        .g_pkt_tone_en                  = EMI_PKT_TONE_EN,
        .g_power_slice_en               = POWER_SLICE_EN,
        .g_cd_time_unit                 = EMI_CD_TIME_UNIT,
        .rx_cfg.tx_cfg.g_pkt_duty_cycle = EMI_PKT_DUTY_CYCLE,
        .rx_cfg.tx_cfg.g_pkt_type       = EMI_PKT_TYPE,
        .rx_cfg.tx_cfg.g_tx_payload_len = EMI_TX_PAYLOAD_LEN,
        .rx_cfg.tx_cfg.g_pkt_tone_delay = EMI_PKT_TONE_DLY,
};

extern const RF_ModeTypeDef rf_mode_list[10];

/**
 * @brief       This function serves to read the calibration value from flash.
 * @param[in]   none
 * @return      none
 * @note       Attention:For TL751X and TL7518 chips, this function call must be made after rf_rode_init
 */
void read_emi_calibration(void)
{
    if (!g_emi_setting.cap) {
        if (g_emi_setting.cal_pos == 1) {
            //OTP:need to be added.
#if defined(MCU_CORE_B80) || defined(MCU_CORE_B80B) || defined(MCU_CORE_B89)
            user_read_otp_value_calib();
#endif
        } else {
#if defined(MCU_CORE_TC1211) || defined(MCU_CORE_TC122X)
        	user_read_efuse_value_calib();
#else
        	user_read_flash_value_calib();
#endif
        }
    }
    else{
        rf_turn_off_internal_cap();
    }
}

/**
 * @brief   The emi test command and function
 */
typedef struct
{
    unsigned char cmd_id; /**< emi command id */
    void (*func)(void);   /**< emi function */
} test_list_t;

/**
 * @brief   Initialize the frequency hopping channels for RF communication.
 */
const unsigned char rf_chn_hopping[MAX_RF_CHANNEL] = {
	48,  4, 66, 62, 44, 12, 38, 16 ,26, 20,
	22, 68, 18, 28,	42, 32, 34, 36, 14,	40,
	30, 54, 46,  2,	50, 52, 80, 56, 78,	74,
	 8, 64,  6, 24,	70, 72, 60, 76, 58, 10,
};

/**
 * @brief   Initialize the tone hopping channels for RF communication.
 */
const unsigned char rf_tone_chn_hopping[MAX_RF_TONE_CHANNEL] = {
    4,  5,  6,  7,  8,  9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
    57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
    67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
    77, 78
};

void emicarrieronly(void);
void emirx(void);
void emi_tx_burst(void);
void emi_tx_continue(void);
#if defined(MCU_CORE_B92)||defined(MCU_CORE_TL721X)||defined(MCU_CORE_TL321X)||defined(MCU_CORE_TL322X)
void rf_emi_tx_current_test(void);
void rf_emi_rx_current_test(void);
#endif

/**
 * @brief   Init the structure of the emi test command and function
 */

test_list_t ate_list[] = {
    {0x01, emicarrieronly        },
    {0x02, emi_tx_continue       },
    {0x03, emirx                 },
    {0x04, emi_tx_burst          },
#if defined(MCU_CORE_B92)||defined(MCU_CORE_TL721X)||defined(MCU_CORE_TL321X)||defined(MCU_CORE_TL322X)
    {0x10, rf_emi_tx_current_test},
    {0x11, rf_emi_rx_current_test}
#endif
};


static signed char    rssi_noise = -110;
static unsigned short rssi_cnt   = 0;

/**
 * @brief      This function is used to get the RSSI value in the ambient noise
 * @return     RSSI values in ambient noise
 */
signed char get_noise_value()
{
	signed char rtn;
	unsigned char ss = (read_reg8(0x448) & 0x30) >> 4;
	if(((read_reg8(0xf20)&BIT(0)) == 0) && ((ss == 0) || (ss == 1)))
	{
		if(rssi_cnt >= READ_RSSI_TIMES)
		{
//			last_rssi = rssi_noise;
			rssi_noise = -110;
			rssi_cnt = 0;
		}

		signed char rssi_temp;
		for(int i = 0; i < 50; i++)
		{
			sleep_us(5);
			rssi_temp = rf_rssi_get_154();
			if (rssi_temp > rssi_noise)
				rssi_noise = rssi_temp;
		}
		rssi_cnt++;
#if DEBUG
		if(debug_index >= 100)
			debug_index = 0;
		debug[debug_index].rssi = rssi_noise + COPENSATION_VALUE;
		debug[debug_index].tick = clock_time();
		debug_index++;
#endif
	}
	else if(read_reg8(0xf20)&BIT(0))
	{
		rf_rx_finish_clear_flag();
	}

	rtn = rssi_noise;// changed by Pengcheng 20201222, for calculating the energy value of white noise
	return rtn + RSSI_COPENSATION_VALUE;
}

/**
 * @brief        This function serves to EMI TxPrbs
 * @param[in]    none
 * @return       none
 */
void emi_tx_burst(void)
{
    pa_operation(PA_SETTING_STATE_TX);
    unsigned int tx_num = 0;
    if (emi_cmd_now.g_power_slice_en) {
    	/* Notice: Set bit 6 of the power table enumeration value to 1 to indicate that power is being configured using the "slice" method;
    	 *         this approach addresses the inconsistency between the power tables of Raptor and Tercel.
    	 *         Since this value is accessed using a data-AND operation within the function, it does not affect other chips.
    	 * */
        emi_cfg_param.power_level = emi_cmd_now.g_power_level | BIT(6);
    } else {
        emi_cfg_param.power_level = rf_power_Level_list[emi_cmd_now.g_power_level];
    }
    if (emi_cmd_now.g_adaptive_en) {
        rf_emi_rx_setup_v1();

    } else {
       rf_emi_tx_burst_setup_v1();
    }
    while (((read_reg8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state) && ((read_reg8(TEST_COMMAND_ADDR)) == emi_cmd_now.g_test_cmd_now) && ((read_reg8(POWER_ADDR)) == emi_cmd_now.g_power_level) && ((read_reg8(CHANNEL_ADDR)) == emi_cmd_now.g_chn) && ((read_reg8(RF_MODE_ADDR)) == emi_cmd_now.g_rf_mode)) {
        if (emi_cmd_now.g_adaptive_en) {
            if (get_noise_value() < MAX_NOISE_VALUE) {
                rf_emi_stop();
                rf_emi_tx_burst_setup_v1();
                rf_emi_tx_burst_loop_v1();
                rf_set_tx_rx_off_auto_mode();
                rf_emi_stop();
                rf_emi_rx_setup_v1();
            }
        } else {
            rf_emi_tx_burst_loop_v1();
        }

        if (emi_cmd_now.g_tx_packet_cnt) {
            tx_num++;
            if (tx_num >= 1000) {
                break;
            }
        }
#if defined(MCU_CORE_B92)
        if (emi_cmd_now.g_pkt_tone_en) {
            rf_set_tx_modulation_index(RF_MI_P0p00);
            rf_set_power_level_singletone(emi_cfg_param.power_level);
            delay_us(emi_cmd_now.rx_cfg.tx_cfg.g_pkt_tone_delay);
            rf_set_power_off_singletone();
            rf_set_tx_modulation_index(RF_MI_P0p50);
        }
#endif
    }
    rf_emi_stop();
}

/**
 * @brief        This function serves to EMI continue data
 * @param[in]    none.
 * @return       none
 */
void emi_tx_continue(void)
{
    pa_operation(PA_SETTING_STATE_TX);
    unsigned int  t0 = reg_system_tick, chnidx = 1;
    unsigned char power      = 0;
    unsigned int  pulse_time = 0;
    if (emi_cmd_now.g_power_slice_en) {
    	/* Notice: Set bit 6 of the power table enumeration value to 1 to indicate that power is being configured using the "slice" method;
    	 *         this approach addresses the inconsistency between the power tables of Raptor and Tercel.
    	 *         Since this value is accessed using a data-AND operation within the function, it does not affect other chips.
    	 * */
        power = emi_cmd_now.g_power_level | BIT(6);
    } else {
        power = rf_power_Level_list[emi_cmd_now.g_power_level];
    }
    emi_cmd_now.g_hop_en       = read_reg8(EMI_CONFIG_FLAG_ADDR) & 0x01;
    emi_cmd_now.g_cd_time_unit = (read_reg8(EMI_CONFIG_FLAG_ADDR) & 0x10) >> 4;
    rf_emi_tx_continue_setup(emi_cfg_param.rf_mode, power, emi_cmd_now.g_chn, emi_cmd_now.rx_cfg.tx_cfg.g_pkt_type);

    while (((read_reg8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state) && ((read_reg8(TEST_COMMAND_ADDR)) == emi_cmd_now.g_test_cmd_now) && ((read_reg8(POWER_ADDR)) == emi_cmd_now.g_power_level) && ((read_reg8(CHANNEL_ADDR)) == emi_cmd_now.g_chn) && ((read_reg8(RF_MODE_ADDR)) == emi_cmd_now.g_rf_mode)) {
        if (emi_cmd_now.g_hop_en) {
            pulse_time = (emi_cmd_now.rx_cfg.tx_cfg.g_tx_payload_len) | (emi_cmd_now.rx_cfg.tx_cfg.g_pkt_tone_delay << 8); //Multiplexed as pulse time
            if (emi_cmd_now.g_cd_time_unit == 0) {
                pulse_time = pulse_time * 1000;
            }

            while (!clock_time_exceed(t0, pulse_time)) {
                rf_continue_mode_run(); // run 10ms
            }
            rf_set_tx_rx_off();
            sleep_us(((pulse_time / emi_cmd_now.rx_cfg.tx_cfg.g_pkt_duty_cycle) * 100) - pulse_time);
            t0 = reg_system_tick;
            rf_emi_tx_continue_setup(emi_cmd_now.g_rf_mode, power, rf_chn_hopping[chnidx - 1], emi_cmd_now.rx_cfg.tx_cfg.g_pkt_type);
            (chnidx >= MAX_RF_CHANNEL) ? (chnidx = 1) : (chnidx++);
        } else {
            rf_continue_mode_run();
        }
    }
	rf_emi_stop();
	dma_set_buff_size(DMA3_RF_TX,0x60);
	dma_reset();
	reset_baseband();
}

#if defined(MCU_CORE_B92)||defined(MCU_CORE_TL721X)||defined(MCU_CORE_TL321X)||defined(MCU_CORE_TL322X)
/**
 * @brief      This function serves to test RF tx current
 * @param[in]  none
 * @return     none
 */
void rf_emi_tx_current_test(void)
{
    extern unsigned char g_single_tong_freqoffset;
    unsigned char        power = 0;
    rf_mode_init();
#if defined(MCU_CORE_TL322X)
    if((emi_cmd_now.g_rf_mode==RF_MODE_BLE_4M_NO_PN)||(emi_cmd_now.g_rf_mode==RF_MODE_BLE_6M_NO_PN)) {
        PLL_192M_D25F_48M_HCLK_N22_24M_PCLK_12M_MSPI_48M;
        rf_modem_rate_mode(RF_48M_MODEM_RATE);
    }else{
        rf_modem_rate_mode(RF_24M_MODEM_RATE);
    }
    rf_modem_hp_path(1);
#endif
    if (emi_cmd_now.g_power_slice_en) {
        power = emi_cmd_now.g_power_level;
    } else {
        power = rf_power_Level_list[emi_cmd_now.g_power_level];

    }
    g_single_tong_freqoffset = 1;
    rf_set_chn(emi_cmd_now.g_chn);
    g_single_tong_freqoffset = 0;
    rf_set_power_level_singletone(power);
#if defined(MCU_CORE_TL721X)
        if(emi_cmd_now.g_power_level>RF_POWER_INDEX_N49p00dBm){
            pm_set_vdd0p94(CAL_0P94V_TO_1P05V);
        }
#elif defined(MCU_CORE_TL322X)
        if(emi_cmd_now.g_power_level>RF_POWER_INDEX_N40p00dBm){
            rf_set_vant_power_trim_level(RF_VANT_HIGH_POWER);
        }
#endif
    rf_set_txmode();
    gpio_shutdown(GPIO_ALL);
    rf_current_test_cfg();
    while (1);
}

/**
 * @brief      This function serves to test RF rx current
 * @param[in]  none
 * @return     none
 */
void rf_emi_rx_current_test(void)
{
#if defined(MCU_CORE_TL322X)
    if((emi_cmd_now.g_rf_mode==RF_MODE_BLE_4M_NO_PN)||(emi_cmd_now.g_rf_mode==RF_MODE_BLE_6M_NO_PN)) {
        PLL_192M_D25F_48M_HCLK_N22_24M_PCLK_12M_MSPI_48M;
        rf_modem_hp_path(1);
        if(emi_cmd_now.g_rf_mode==RF_MODE_BLE_4M_NO_PN){
            rf_emi_rx_setup(RF_MODE_BLE_4M_NO_PN, emi_cmd_now.g_chn);
        }else{
            rf_emi_rx_setup(RF_MODE_BLE_6M_NO_PN, emi_cmd_now.g_chn);
        }
    }else{
        rf_emi_rx_setup(RF_MODE_BLE_1M_NO_PN, emi_cmd_now.g_chn);
    }
    rf_modem_hp_path(1);
#else
    rf_emi_rx_setup(RF_MODE_BLE_1M_NO_PN, emi_cmd_now.g_chn);
#endif
    gpio_shutdown(GPIO_ALL);
    rf_current_test_cfg();
    while (1);
}
#endif

/**
 * @brief        This function serves to EMI CarryOnly
 * @param[in]    none
 * @return       none
 */
void emicarrieronly(void)
{
    pa_operation(PA_SETTING_STATE_TX);
    unsigned int  chnidx     = 1;
    unsigned int  pulse_time = 0;
    unsigned char power      = 0;
    if (emi_cmd_now.g_power_slice_en) {
    	/* Notice: Set bit 6 of the power table enumeration value to 1 to indicate that power is being configured using the "slice" method;
    	 *         this approach addresses the inconsistency between the power tables of Raptor and Tercel.
    	 *         Since this value is accessed using a data-AND operation within the function, it does not affect other chips.
    	 * */
        power = emi_cmd_now.g_power_level | BIT(6);
    } else {
        power = rf_power_Level_list[emi_cmd_now.g_power_level];
    }

    emi_cmd_now.g_hop_en       = read_reg8(EMI_CONFIG_FLAG_ADDR) & 0x01;
    emi_cmd_now.g_cd_time_unit = (read_reg8(EMI_CONFIG_FLAG_ADDR) & 0x10) >> 4;
    if (emi_cmd_now.g_hop_en != 1) {
    	rf_emi_single_tone(power, emi_cmd_now.g_chn);
    }
    while (((read_reg8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state) && ((read_reg8(TEST_COMMAND_ADDR)) == emi_cmd_now.g_test_cmd_now) && ((read_reg8(POWER_ADDR)) == emi_cmd_now.g_power_level) && ((read_reg8(CHANNEL_ADDR)) == emi_cmd_now.g_chn) && ((read_reg8(RF_MODE_ADDR)) == emi_cmd_now.g_rf_mode)) {
        if (emi_cmd_now.g_hop_en == 1) {
            pulse_time = (emi_cmd_now.rx_cfg.tx_cfg.g_tx_payload_len) | (emi_cmd_now.rx_cfg.tx_cfg.g_pkt_tone_delay << 8);
            if (emi_cmd_now.g_cd_time_unit == 0) {
                pulse_time = pulse_time * 1000;
            }

            rf_emi_single_tone(power, rf_tone_chn_hopping[chnidx - 1]);
            sleep_us(pulse_time);
            rf_set_tx_rx_off();
            sleep_us(((pulse_time / emi_cmd_now.rx_cfg.tx_cfg.g_pkt_duty_cycle) * 100) - pulse_time);
            (chnidx >= MAX_RF_TONE_CHANNEL) ? (chnidx = 1) : (chnidx++);
        }
    }
	rf_emi_stop();
	dma_set_buff_size(DMA3_RF_TX,0x60);
	dma_reset();
	reset_baseband();
}

/**
 * @brief        This function serves to EMI Rx
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr        - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @param[in]   pkt_type    - The type of data sent.
 * @return         none
 */
void emirx(void)
{
    pa_operation(PA_SETTING_STATE_RX);
    rf_emi_rx_setup_v1();
    emi_cmd_now.g_rssi_val             = 0;
    emi_cmd_now.rx_cfg.g_rx_packet_num = 0;
    write_reg8(RSSI_ADDR, 0);
    write_reg32(RX_PACKET_NUM_ADDR, 0);
    while (((read_reg8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state) && ((read_reg8(TEST_COMMAND_ADDR)) == emi_cmd_now.g_test_cmd_now) && ((read_reg8(POWER_ADDR)) == emi_cmd_now.g_power_level) && ((read_reg8(CHANNEL_ADDR)) == emi_cmd_now.g_chn) && ((read_reg8(RF_MODE_ADDR)) == emi_cmd_now.g_rf_mode)) {
        rf_emi_rx_loop_v1();
        if (rf_emi_get_rxpkt_cnt_v1() != read_reg32(RX_PACKET_NUM_ADDR)) {
        	write_reg8(RSSI_ADDR, rf_emi_get_rssi_avg_v1());
        	write_reg32(RX_PACKET_NUM_ADDR, rf_emi_get_rxpkt_cnt_v1());
        }
    }

    rf_emi_stop();
}

/**
 * @brief      This function serves to EMI ServiceLoop
 * @return       none
 */
void emi_serviceloop(void)
{
    unsigned char i             = 0;
    unsigned int  pa_first_flag = 0;
    emi_cmd_t    *cmd_ptr       = (emi_cmd_t *)(RSSI_ADDR);
    while (1) {
        emi_cmd_now.g_run_state = read_reg8(RUN_STATUE_ADDR); // get the run state!

        if (emi_cmd_now.g_run_state != 0) {
            pa_first_flag++;
            memcpy(&emi_cmd_now, cmd_ptr, sizeof(emi_cmd_t));
            emi_cfg_param.rf_chn             = emi_cmd_now.g_chn;
            emi_cfg_param.pkt_type           = emi_cmd_now.rx_cfg.tx_cfg.g_pkt_type;
            emi_cfg_param.emi_tx_payload_len = emi_cmd_now.rx_cfg.tx_cfg.g_tx_payload_len;
            emi_cfg_param.emi_pkt_duty_cycle = emi_cmd_now.rx_cfg.tx_cfg.g_pkt_duty_cycle;
            emi_cfg_param.emi_access_code    = g_emi_setting.access_code;
            emi_cfg_param.rf_mode            = rf_mode_list[emi_cmd_now.g_rf_mode];
            emi_cfg_param.power_level        = emi_cmd_now.g_power_level;
            pa_operation(PA_SETTING_STATE_INIT);
            if ((pa_first_flag == 1) && g_emi_setting.pa_en) {
                if (emi_cmd_now.g_power_slice_en) {
                    emi_cmd_now.g_power_level = 0;
                } else {
                    emi_cmd_now.g_power_level = (sizeof(rf_power_Level_list) / sizeof(rf_power_Level_list[0]));
                }
            }

            for (i = 0; i < sizeof(ate_list) / sizeof(test_list_t); i++) {
                if (emi_cmd_now.g_test_cmd_now == ate_list[i].cmd_id) {
                    ate_list[i].func();
                }
            }
            emi_cmd_now.g_run_state = 0;
            write_reg8(RUN_STATUE_ADDR, emi_cmd_now.g_run_state);
            pa_first_flag = 0xf0;
        }
    }
}

/**
 * @brief        This function serves to pa init.
 * @return         none
 */
void emi_pa_init(void)
{
    if (g_emi_setting.pa_en) {
#ifdef RF_CERTIFICATION_CGF_EN
        g_emi_setting.pa_addr = get_config_offset(TLK_CONFIG_TYPE_PA);
        if (g_emi_setting.pa_addr) {
            pa_setting_init(g_emi_setting.pa_addr, g_emi_setting.pa_bypass_en);
        } else {
            g_emi_setting.pa_en = 0;
        }
#else
        pa_setting_init(g_emi_setting.pa_en, g_emi_setting.pa_bypass_en);
#endif
    }
}

/**
 * @brief        This function serves to User Init
 * @return         none
 */
void user_init(void)
{
#if (!defined(MCU_CORE_TC122X))
	blc_pm_select_internal_32k_crystal();
#endif

#if (MCU_CORE_B85) || (MCU_CORE_B87)
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp
#endif

#if defined(MCU_CORE_B85)
	cpu_wakeup_init();
#elif defined(MCU_CORE_B87) || defined(MCU_CORE_TC321X)
	cpu_wakeup_init(LDO_MODE, INTERNAL_CAP_XTAL24M);
#elif defined(MCU_CORE_B89) || defined(MCU_CORE_B80) || defined(MCU_CORE_B80B) || defined(MCU_CORE_TC1211) || defined(MCU_CORE_TC122X)
	cpu_wakeup_init(INTERNAL_CAP_XTAL24M);
#endif

#ifdef RF_CERTIFICATION_CGF_EN

    config_obj_init();

    emi_config_data_t emi_cfg_data;
    if (get_config_data(TLK_CONFIG_TYPE_EMI, (unsigned char *)&emi_cfg_data, sizeof(emi_cfg_data))) {
        g_emi_setting.cap     = emi_cfg_data.cap;
        g_emi_setting.cal_pos = emi_cfg_data.cal_pos;

        switch (emi_cfg_data.power_mode) {
        case 0:
            g_emi_setting.power_mode = 0x40; //B91:LDO_1P4_LDO_1P8; B92:LDO_1P4_LDO_2P0; TL721X:LDO_0P94_LDO_1P8; TL321X:LDO_1P25_LDO_1P8
            break;
        case 1:
            g_emi_setting.power_mode = 0x41; //B91:DCDC_1P4_LDO_1P8; B92:DCDC_1P4_LDO_2P0; TL721X:DCDC_0P94_LDO_1P8; TL321X:DCDC_1P25_LDO_1P8
            break;
        case 2:
            g_emi_setting.power_mode = 0x43; //B91:DCDC_1P4_DCDC_1P8; B92:DCDC_1P4_DCDC_2P0; TL721X:DCDC_0P94_DCDC_1P8; TL321X:DCDC_1P25_DCDC_1P8
            break;
        default:
            g_emi_setting.power_mode = 0x40;
            break;
        }

        g_emi_setting.swire_through_usb_en = emi_cfg_data.swire_through_usb_en;
        g_emi_setting.pa_en                = emi_cfg_data.pa_en;
        g_emi_setting.access_code          = emi_cfg_data.access_code;
    }

#endif /*SUPPORT_USER_CONFIG*/

#if defined(MCU_CORE_B85)
	cpu_wakeup_init();
#elif defined(MCU_CORE_B87)
	cpu_wakeup_init(g_emi_setting.power_mode, g_emi_setting.cap);
#elif defined(MCU_CORE_TC321X)
	cpu_wakeup_init(g_emi_setting.power_mode, g_emi_setting.cap);
#elif defined(MCU_CORE_B89)
	cpu_wakeup_init(INTERNAL_CAP_XTAL24M);
#elif defined(MCU_CORE_B80)
	cpu_wakeup_init(g_emi_setting.cap);
#elif defined(MCU_CORE_B80B)
	cpu_wakeup_init(g_emi_setting.cap);
#elif defined(MCU_CORE_TC1211)
	cpu_wakeup_init(g_emi_setting.cap);
#elif defined(MCU_CORE_TC122X)
	cpu_wakeup_init(g_emi_setting.cap);
#endif

#if(MCU_CORE_B80 || MCU_CORE_B80B ||MCU_CORE_B89 || MCU_CORE_TC321X || MCU_CORE_TC1211||MCU_CORE_TC122X)
	wd_32k_stop();
#endif
#if (MCU_CORE_B85) || (MCU_CORE_B87)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif (MCU_CORE_B89)
	//Note: This function must be called, otherwise an abnormal situation may occur.
	//Called immediately after cpu_wakeup_init, set in other positions, some calibration values may not take effect.
	user_read_otp_value_calib();
#elif (MCU_CORE_TC1211||MCU_CORE_TC122X)
	user_read_efuse_value_calib();
#endif

#if MCU_CORE_TC1211
    gpio_digital_pullup_en(GPIO_SWS,1);
#else
    // Note: This is to set SWS pull. If SWS is not set up, SWS will be floating, causing abnormal sleep currents of suspend,
    // there may be the risk of sws miswriting the chip registers or sram causing a crash.
    gpio_setup_up_down_resistor(GPIO_SWS, PM_PIN_PULLUP_1M);
#endif

/**
	===============================================================================
						##### driver sdk firmware protection #####
	===============================================================================
	Flash write protection must be added, the size of the protected area is decided according to the application bin file,
	the principle is as follows:
	1.The program area is protected (to prevent the program area from being erased by mistake);
	2.the program will modify the data area is not protected (if write-protected, each time before erasing the need to unprotect,
	  so that there is a risk that the status register of the flash has been mistakenly rewritten);

	@note if flash protection fails, LED1 lights up long, and keeps while.
	===============================================================================
*/
#if !(MCU_CORE_TC1211||MCU_CORE_TC122X)
	flash_init(0);
#endif

    CLOCK_INIT;

#if (MCU_CORE_B85 || MCU_CORE_B87)
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);
	gpio_init(!deepRetWakeUp);
#elif(MCU_CORE_B89 || MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC1211||MCU_CORE_TC122X)
	rf_mode_init();
	rf_set_ble_1M_NO_PN_mode();
#endif
	read_emi_calibration();
	emi_pa_init();

    if (g_emi_setting.swire_through_usb_en) {
#if !defined(MCU_CORE_B89) && !defined(MCU_CORE_TC321X) && !defined(MCU_CORE_TC122X)
        usb_set_pin(1); //add for chips only support swire function of through-usb
#endif
#if defined(MCU_CORE_B85) || defined(MCU_CORE_B87)
        usb_set_pin(1);
        gpio_setup_up_down_resistor(GPIO_PA5, PM_PIN_PULLUP_10K);
#endif
    }

    rf_access_code_comm(g_emi_setting.access_code); // access code
    unsigned char *dest = (unsigned char *)RSSI_ADDR;
    unsigned char *src = (unsigned char *)&emi_cmd_now;
    unsigned int size = sizeof(emi_cmd_t);

    for (unsigned int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

void main_loop(void)
{
	emi_serviceloop();
}

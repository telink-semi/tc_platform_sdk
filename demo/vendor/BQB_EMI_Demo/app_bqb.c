/********************************************************************************************************
 * @file    app_bqb.c
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
#include "BQB/bqb.h"

#if(TEST_DEMO==BQB_DEMO)

#define read_config(addr) (*(volatile unsigned char*)(0x840000 | (addr)))
#define write_config(addr,v) 	(*(volatile unsigned char*)(0x840000 | (addr)) = (unsigned char)(v))

#if (RF_FAST_SETTLE)&&(defined(MCU_CORE_TC321X)||defined(MCU_CORE_TC123X))
_attribute_data_retention_ rf_fast_settle_t fs_cv_1m;
_attribute_data_retention_ rf_fast_settle_t fs_cv_2m;
/**
 *  @brief      This function is used to get the calibration value of rf tx/rx fast settle
 *  @param[in]  tx_settle_us    After adjusting the timing sequence, the time required for tx to settle.
 *  @param[in]  rx_settle_us    After adjusting the timing sequence, the time required for rx to settle.
 *  @param[in]  fs_cv           Fast settle related calibration value storage variable address.
 *  @return     none
 */
void rf_fast_settle_get_val(rf_tx_fast_settle_time_e tx_settle_us, rf_rx_fast_settle_time_e rx_settle_us, rf_fast_settle_t *fs_cv)
{
    //tx
    rf_set_tx_rx_off(); //STOP_RF_STATE_MACHINE;
    rf_clr_irq_status(FLD_RF_IRQ_ALL);
    rf_set_tx_settle_time(TX_SETTLE_TIME);        //adjust TX settle time

    for (unsigned char f_chn = 0; f_chn <= 80; f_chn++) {
        rf_set_channel(f_chn,0);
        rf_fcal_debug();
        rf_set_txmode();
        sleep_us(TX_SETTLE_TIME); //Wait for calibration to stabilize
        rf_tx_fast_settle_get_cal_val(tx_settle_us, f_chn, fs_cv);

        rf_set_tx_rx_off(); //STOP_RF_STATE_MACHINE;
        rf_clr_irq_status(FLD_RF_IRQ_ALL);
    }

    //rx
    rf_set_rx_settle_time(RX_SETTLE_TIME); //adjust RX settle time
    for (unsigned char f_chn = 0; f_chn <= 80; f_chn++) {
        rf_set_channel(f_chn,0);
        rf_fcal_debug();
        rf_set_rxmode();
        sleep_us(RX_SETTLE_TIME); //Wait for the rx packetization action to complete
        rf_rx_fast_settle_get_cal_val(rx_settle_us, f_chn, fs_cv);

        rf_set_tx_rx_off(); //STOP_RF_STATE_MACHINE;
        rf_clr_irq_status(FLD_RF_IRQ_ALL);
    }
}

void fs_get_value(void)
{
    rf_set_ble_1M_NO_PN_mode();
    rf_fast_settle_get_val(TX_SETTLE_TIME_21US, RX_SETTLE_TIME_21US, &fs_cv_1m);

    rf_set_ble_2M_NO_PN_mode();
    rf_fast_settle_get_val(TX_SETTLE_TIME_21US, RX_SETTLE_TIME_21US, &fs_cv_2m);
}

#endif

#if SUPPORT_CONFIGURATION


void rd_usr_definition(unsigned char _s)
{
	if(_s)
	{
		usr_config.uart_tx = read_config(CONFIGURATION_ADDR_UART_TX);
		usr_config.uart_rx = read_config(CONFIGURATION_ADDR_UART_RX);
		usr_config.pa_tx = read_config(CONFIGURATION_ADDR_PA_TX);
		usr_config.pa_rx = read_config(CONFIGURATION_ADDR_PA_RX);
		unsigned char tmp = read_config(CONFIGURATION_ADDR_CAP);
		usr_config.cap = (tmp >> 7) & 0x01;
		usr_config.cal_pos = tmp & 0x03;
		usr_config.power_mode = (tmp >> 5) & 0x03;
		usr_config.power = read_config(CONFIGURATION_ADDR_POWER);
	}
	else
	{
		usr_config.flash = (flash_read_mid() >> 16) & 0xff;
	}
}
#if (MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X|| MCU_CORE_TC123X)
void get_uart_port(GPIO_PinTypeDef* bqb_uart_tx_port, GPIO_PinTypeDef* bqb_uart_rx_port)
#else
void get_uart_port(UART_TxPinDef* bqb_uart_tx_port, UART_RxPinDef* bqb_uart_rx_port)
#endif
{
	if(usr_config.uart_tx == usr_config.uart_rx) return;
	*bqb_uart_tx_port = get_pin(usr_config.uart_tx);
	*bqb_uart_rx_port = get_pin(usr_config.uart_rx);
}
#endif

void read_bqb_calibration()
{
	unsigned char chnidx = 0xff;
#if SUPPORT_CONFIGURATION
	if(usr_config.cap)
	{
		rf_turn_off_internal_cap();
	}
	else
	{
		if(usr_config.cal_pos == 1)//OTP
		{
#if (MCU_CORE_B80 || MCU_CORE_B80B|| MCU_CORE_B89)
			extern unsigned char otp_program_flag;
			unsigned int temp;
			if(otp_program_flag != 1)
			{
				otp_set_active_mode();
			}
			otp_read(OTP_CAP_VALUE_ADDR, 1, &temp);
			if(otp_program_flag != 1)
			{
				otp_set_deep_standby_mode();
			}
			chnidx = temp & 0xff;
#endif
		}
		else if(usr_config.cal_pos == 2)//SRAM
		{
			chnidx = read_config(CAP_CALIBRATION_SRAM);
		}
		else//FLASH
		{
			#if (!MCU_CORE_TC122X)
			if(usr_config.flash == FLASH_SIZE_2M)
			{
				flash_read_page(FLASH_CAP_VALUE_ADDR_2M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_1M)
			{
				flash_read_page(FLASH_CAP_VALUE_ADDR_1M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_512K)
			{
				flash_read_page(FLASH_CAP_VALUE_ADDR_512K, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_128K || usr_config.flash == FLASH_SIZE_256K)
			{
				flash_read_page(FLASH_CAP_VALUE_ADDR_128K, 1, &chnidx);
			}
			else
			{
				flash_read_page(FLASH_CAP_VALUE_ADDR_64K, 1, &chnidx);
			}
			#endif
		}

		if(chnidx!=0xff)
		{
			rf_update_internal_cap(chnidx);
		}
	}
#else
	#if(SWITCH_INTERNAL_CAP)
		#if SWITCH_CALIBRATION_POSITION
			#if (!MCU_CORE_TC122X && !MCU_CORE_TC123X)
			flash_read_page(CAP_SET_FLASH_ADDR, 1, &chnidx);
			#endif
		#else
			chnidx = read_config(CAP_CALIBRATION_SRAM);
		#endif
			if(chnidx!=0xff)
			{
				rf_update_internal_cap(chnidx);
			}

	#else
			rf_turn_off_internal_cap();
	#endif
#endif
}


void user_init(void)
{
#if DEBUG_FLAG
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);	 //disable input
	gpio_write(LED1, 0);         //LED On
#endif

#if(MCU_CORE_B80 || MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X|| MCU_CORE_TC123X)
	GPIO_PinTypeDef bqb_uart_tx_port = BQB_UART_TX_PORT;
	GPIO_PinTypeDef bqb_uart_rx_port = BQB_UART_RX_PORT;
#else
	UART_TxPinDef bqb_uart_tx_port = BQB_UART_TX_PORT;
	UART_RxPinDef bqb_uart_rx_port = BQB_UART_RX_PORT;
#endif
#if SUPPORT_CONFIGURATION
	if(usr_config.cal_pos == 0)
	{
		rd_usr_definition(0);
	}
	get_uart_port(&bqb_uart_tx_port, &bqb_uart_rx_port);
#endif
#if(MCU_CORE_B80B || MCU_CORE_TC321X || MCU_CORE_TC122X|| MCU_CORE_TC123X)
	uart_gpio_set(UART_MODULE_SEL,bqb_uart_tx_port, bqb_uart_rx_port);// uart tx/rx pin set
	uart_reset(UART_MODULE_SEL);  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
	uart_init_baudrate(UART_MODULE_SEL,BQB_UART_BAUD,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);
	uart_dma_enable(UART_MODULE_SEL,0, 0);
	irq_disable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);
	uart_irq_enable(UART_MODULE_SEL,0,0);   //uart RX irq enable
	uart_ndma_irq_triglevel(UART_MODULE_SEL,0,0);	//set the trig level. 1 indicate one byte will occur interrupt
#else
	uart_gpio_set(bqb_uart_tx_port, bqb_uart_rx_port);// uart tx/rx pin set
	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
	uart_init_baudrate(BQB_UART_BAUD,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);
	uart_dma_enable(0, 0);
	irq_disable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);
	uart_irq_enable(0,0);   //uart RX irq enable
	uart_ndma_irq_triglevel(0,0);	//set the trig level. 1 indicate one byte will occur interrupt
#endif
	/* for SRAM calibration */
	write_config(CAP_CALIBRATION_SRAM, 0xff);
	read_bqb_calibration();
	/*DP through sws*/
#if(MCU_CORE_B85||MCU_CORE_B87)
	usb_set_pin(1);
	gpio_setup_up_down_resistor(GPIO_PA5, PM_PIN_PULLUP_10K);
#endif
	bqbtest_init();
	//SET PA PORT
	bqb_pa_init();

#if (RF_FAST_SETTLE)&&(defined(MCU_CORE_TC321X)||defined(MCU_CORE_TC123X))
	fs_get_value();
    rf_fast_settle_set_val(&fs_cv_1m);
    if(-1 == rf_fast_settle_config()){
        while(1);
    }
    rf_tx_fast_settle_en();
    rf_rx_fast_settle_en();
    rf_set_tx_settle_time(21);
    rf_set_rx_settle_time(21);
#endif
}

void main_loop(void)
{
	bqb_serviceloop();
}
#endif

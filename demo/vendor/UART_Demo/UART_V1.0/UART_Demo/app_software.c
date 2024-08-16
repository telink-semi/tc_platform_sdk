/********************************************************************************************************
 * @file    app_software.c
 *
 * @brief   This is the source file for Telink MCU
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "string.h"

#if (UART_MODE == UART_SOFTWARE_RX)
/********************************************************************************************************
 *	Data processing process:
 *	1.Set UART_RX pin to GPIO function when initialized and set it to falling edge interrupt
 *	2.when data is received:
 *   1). First, enable the GPIO falling edge interrupt of the UART_RX pin, the start bit of the UART data will
 *       trigger the falling edge interrupt, and execute in the interrupt:Turn off the GPIO interrupt
 *       and set a hardware timer whose timing period is equal to uart Sampling period
 *       (time to send 1 bit + offset at baud rate), then start timer sampling;
 *   2). When the sampling timer interrupt arrives,read the high and low state of UART_RX for sampling,
 *       and stores each 8bit data received as a Byte;turns on the timeout detection (using the same timer,
 *		 the timing period is the set maximum interval between two Bytes in the same frame),
 *       and enables the GPIO falling edge interrupt again
 *   3). If a timeout is detected,find the filling place of the next frame of data and give the data pointer, 
 *		 and close the timer.
 *       If the GPIO falling edge interrupt of the UART_RX pin is triggered before the timeout period arrives,
 *       it is judged that this frame of data has not been received, and the operation of step 1 is performed.
 *	3.Function soft_uart_rx_data_process is used to demonstrate the user's processing of data
 *******************************************************************************************************/
typedef void (*soft_uart_rx_timer_def)(TIMER_ModeTypeDef,unsigned int, unsigned int);
#define  SOFT_UART_TIMER_INIT_FUN(SOFT_UART_TIMER_NUM)	 timer##SOFT_UART_TIMER_NUM##_set_mode
#define  SOFT_UART_REC_LEN_MAX			 200	// the maximum length of data that can be received in one buffer of data
#define  SOFT_UART_REC_GROUP_MAX		 2		// maximum number of data pens saved

#define  SOFT_UART_PROCESSIN 			 1	   // select whether to process received data in main_loop
#define  SOFT_UART_PRINT_ALL 			 1	   // select whether to print all data in one buffer
#define  SOFT_UART_DELAYING   			 0     // used to simulate background blocking
volatile char g_soft_uart_rec_data[SOFT_UART_REC_GROUP_MAX][SOFT_UART_REC_LEN_MAX];

/**
 *  @brief  Define soft uart receive buffer states.
 */
typedef enum{
	SOFT_UART_DATA_IS_EMPTY_REC  = 0, // data has been cleared or is being received
 	SOFT_UART_DATA_IS_WAIT   	 = 1, // data is received and waiting to be processed
}soft_uart_rx_buff_state_e;

/**
 *  @brief  The structure of the management software UART.
 */
typedef struct{
	unsigned int    baud;		  // the baud rate of the UART signal
	unsigned short  timer_period; // the sampling period(the time to send 1bit at this baud rate + offset)
	/*
	 * The value can be positive or negative [-128,127]
	 * If the baud rate of the sending end is lower than the standard value, set it to a higher value.
	 * If the baud rate of the sending end is higher than the standard value, set it to a lower (negative) value
	*/
	signed char     baud_offset;
	unsigned short  rec_length[SOFT_UART_REC_GROUP_MAX];// received data lengths
	unsigned short  rec_length_max; // the maximum length of data that can be received in one buffer of data
    soft_uart_rx_timer_def rx_timer; // timer setting function
	GPIO_PinTypeDef	rx_pin; // UART signal receiving pin
	char *data_pointer;		// the base address of the current buffer
	unsigned char index;    // the index value of the current buffer
	unsigned char bit_num;  // the bit number of the currently received byte
	unsigned char temp_byte;// currently received bytes
	unsigned char timeout:4;// the periodic period (the time to send g_soft_uart_rec.timeout bits at this baud rate),the value range is[2,15]
	unsigned char timer_num:2;// the hardware timer label used, optional value is 0 1 2
	/*
	 *  timer usage flag
	 *  0: the purpose of the timer is for data sampling
	 *  1: the purpose of the timer is timeout detection
	 *  2: the timer state as idle (if the user does not want to continue to receive data, then the timer can be used for other things)
	*/
	unsigned char timer_application:2;
	unsigned char data_over_flag[SOFT_UART_REC_GROUP_MAX];// buffer overflow flag
	soft_uart_rx_buff_state_e sotf_uart_rx_buff_state[SOFT_UART_REC_GROUP_MAX];// buffers status bit
}soft_uart_rec_st;

volatile soft_uart_rec_st  g_soft_uart_rec; // global variable

// interrupt handlers must be placed in ram_code section
_attribute_ram_code_sec_noinline_ void irq_handler(void){
	if((reg_irq_src & FLD_IRQ_GPIO_EN)==FLD_IRQ_GPIO_EN){
		reg_irq_src = FLD_IRQ_GPIO_EN; // clear the relevant irq
		gpio_en_interrupt(g_soft_uart_rec.rx_pin,0);
		// set the sampling period (the time to send 1bit at this baud rate + offset)
		g_soft_uart_rec.rx_timer(TIMER_MODE_SYSCLK,0,g_soft_uart_rec.timer_period);
		timer_start(g_soft_uart_rec.timer_num); //start the timer
		g_soft_uart_rec.timer_application = 0; // The purpose of the timer is for data sampling
	}
	if(timer_get_interrupt_status(FLD_TMR_STA_TMR0)){
		timer_clear_interrupt_status(FLD_TMR_STA_TMR0);// clear irq status
		if(0 == g_soft_uart_rec.timer_application){  // If the purpose of the timer is data sampling
 		    if(SOFT_UART_DATA_IS_EMPTY_REC == g_soft_uart_rec.sotf_uart_rx_buff_state[g_soft_uart_rec.index]){
			    if(1 == gpio_read(g_soft_uart_rec.rx_pin)){
					g_soft_uart_rec.temp_byte |= BIT(g_soft_uart_rec.bit_num);//continuously sample eight bits of data
				}
            }
			g_soft_uart_rec.bit_num++;
			if(8 == g_soft_uart_rec.bit_num){//received a Byte data
				/* set the periodic period (the time to send g_soft_uart_rec.timeout bits at this baud rate) */
				g_soft_uart_rec.rx_timer(TIMER_MODE_SYSCLK,0,(g_soft_uart_rec.timeout*CLOCK_SYS_CLOCK_HZ)/g_soft_uart_rec.baud);
				timer_start(g_soft_uart_rec.timer_num);// start the timer
				g_soft_uart_rec.timer_application = 1; // The purpose of the timer is timeout detection
				g_soft_uart_rec.bit_num = 0;
				if(g_soft_uart_rec.rec_length[g_soft_uart_rec.index] < g_soft_uart_rec.rec_length_max){// store the data at the current buff
					g_soft_uart_rec.data_pointer[g_soft_uart_rec.rec_length[g_soft_uart_rec.index]] = g_soft_uart_rec.temp_byte;
				}
				g_soft_uart_rec.temp_byte = 0;

				g_soft_uart_rec.rec_length[g_soft_uart_rec.index]++;
				if(g_soft_uart_rec.rec_length[g_soft_uart_rec.index] > g_soft_uart_rec.rec_length_max){
					g_soft_uart_rec.data_over_flag[g_soft_uart_rec.index] = 1; // indicates that this buffer has overflowed
					g_soft_uart_rec.rec_length[g_soft_uart_rec.index] = g_soft_uart_rec.rec_length_max;
				}
				gpio_set_interrupt(g_soft_uart_rec.rx_pin, POL_FALLING); // configuration falling edge interruption
			}
		}
		else if(1 == g_soft_uart_rec.timer_application){ // if the purpose of the timer is timeout detection
			// data filling ends and waits to process this buffer of data
			g_soft_uart_rec.sotf_uart_rx_buff_state[g_soft_uart_rec.index]  = SOFT_UART_DATA_IS_WAIT;
		    g_soft_uart_rec.bit_num   =  0;
		    for(unsigned char i = 0; i < SOFT_UART_REC_GROUP_MAX; i++){ // find the next buffer to fill
		    	unsigned char next_node = (g_soft_uart_rec.index+1)%SOFT_UART_REC_GROUP_MAX;
			    if(SOFT_UART_DATA_IS_EMPTY_REC == g_soft_uart_rec.sotf_uart_rx_buff_state[i]){
				    g_soft_uart_rec.index = next_node;
				    g_soft_uart_rec.data_pointer  =  (char*)(g_soft_uart_rec_data[g_soft_uart_rec.index]);//set the base address of the current buffer
				    break;
			    }
		    }
		    timer_stop(g_soft_uart_rec.timer_num);// stop the timer
			/*
				mark the timer state as idle (if the user does not want to continue to
				receive data, then the timer can be used for other things)
			*/
		    g_soft_uart_rec.timer_application = 2;
 		}
	}
}
/**
 * @brief     This function is to configure the software UART receive function
 * @param[in] rx_pin - UART signal receiving pin
 * @param[in] baud - the baud rate of the UART signal,the value range is[400,19200] under the 24M system clock
 * @return    none
 *
 * notes1:
 *   The default is hardware timer 0. If you want to change to hardware timer 1 or 2,
 *   you need to modify the values of the two variables g_soft_uart_rec.rx_timer and g_soft_uart_rec.timer_num together
 *
 * notes2: 
 *  The following parameters can be modified by the user according to the actual situation
 * 	1) g_soft_uart_rec.baud_offset: refer to note3
 * 	2) g_soft_uart_rec.timer_num: the hardware timer label used, optional value is 0 1 2
 * 	3) g_soft_uart_rec.rx_timer:  timer setting function
 * 	4) g_soft_uart_rec.timeout:   the periodic period (the time to send g_soft_uart_rec.timeout bits at this baud rate),the value range is[2,15]
 *
 * notes3: 
 *  g_soft_uart_rec.baud_offset:
 *  1).The value of g_soft_uart_rec.baud_offset can be positive or negative [-128,127]
 *  If the baud rate of the sending end is lower than the standard value, set it to a higher value.
 *  If the baud rate of the sending end is higher than the standard value, set it to a lower (negative) value
 *
 *  2).Under the UART demo of B87, use ndma mode, 50 is the step value for setting the baud rate of B87 UART,
 *  and connect the UART_TX pin of B87 to the UART_RX pin of B80 for testing.
 *  Both the B87 and B80 system clocks are 24MHZ, and the UART_RX baud rate is set to 9600,
 *  Set different offset values, the minimum and maximum baud rate of the sender when it can communicate normally:
 *		baud_offset		minimum baud rate		maximum baud rate
 *			0				  9450					10400
 *		    50				  9300					10200
 *		   100				  9100					10000
 *		   120				  9050					9800
 */
void soft_uart_rx_init(GPIO_PinTypeDef rx_pin,unsigned int  baud){
    g_soft_uart_rec.index 			  =  0; // the index value of the current buffer
	g_soft_uart_rec.data_pointer	  =  (char*)g_soft_uart_rec_data; // the base address of the current buffer
	g_soft_uart_rec.bit_num  		  =  0;  // the bit number of the currently received byte
	for(unsigned int once_loop = 0; once_loop < SOFT_UART_REC_GROUP_MAX; once_loop++){// mark all buffer status as acceptable data
		g_soft_uart_rec.rec_length[once_loop] 	  = 0;
		g_soft_uart_rec.sotf_uart_rx_buff_state[once_loop]  = SOFT_UART_DATA_IS_EMPTY_REC;
		g_soft_uart_rec.data_over_flag[once_loop] = 0; // buffer overflow status cleared
	}
	g_soft_uart_rec.temp_byte = 0;	// currently received bytes
	g_soft_uart_rec.rec_length_max    =  SOFT_UART_REC_LEN_MAX; // the maximum length of data that can be received in one buffer of data
	g_soft_uart_rec.rx_pin		 	  =  rx_pin;  // UART signal receiving pin
	g_soft_uart_rec.baud 			  =  baud; // the baud rate of the UART signal

	g_soft_uart_rec.baud_offset  	  =  0;
	// the sampling period(the time to send 1bit at this baud rate + offset)
	g_soft_uart_rec.timer_period      =  CLOCK_SYS_CLOCK_HZ/g_soft_uart_rec.baud + g_soft_uart_rec.baud_offset;
	/*
	 * The default is hardware timer 0. If you want to change to hardware timer 1 or 2,
	 * you need to modify the values of the two variables g_soft_uart_rec.rx_timer and g_soft_uart_rec.timer_num together
	*/
	g_soft_uart_rec.timer_num		  =  0;// the hardware timer label used, optional value is 0 1 2
	g_soft_uart_rec.rx_timer 		  =  SOFT_UART_TIMER_INIT_FUN(0); // timer setting function
	g_soft_uart_rec.timeout 		  =  4;//the periodic period (the time to send g_soft_uart_rec.timeout bits at this baud rate),the value range is[2,15]
	/*
	 *  timer usage flag
	 *  0: the purpose of the timer is for data sampling
	 *  1: the purpose of the timer is timeout detection
	 *  2: the timer state as idle (if the user does not want to continue to receive data, then the timer can be used for other things)
	*/
	g_soft_uart_rec.timer_application = 0;
 	memset((char*)g_soft_uart_rec_data,0,SOFT_UART_REC_GROUP_MAX*SOFT_UART_REC_LEN_MAX); // buffers clear
    gpio_set_func(g_soft_uart_rec.rx_pin ,AS_GPIO);
	gpio_set_output_en(g_soft_uart_rec.rx_pin, 0);
	gpio_set_input_en(g_soft_uart_rec.rx_pin ,1);
	gpio_setup_up_down_resistor(g_soft_uart_rec.rx_pin,PM_PIN_PULLUP_10K);
}

/**
 * @brief     This function turns on the UART receiver function
 * @return    none
 * notes: 
 *       When this function is used for the first time, it must be initialized in the following order:  
 *       1, sofT_UART_rx_init
 *       2, soft_UARt_rx_en (when needed, call it), then if you want to disable this function, 
 *          you can use soft_UARt_rx_dis to implement, when the function is enabled again, call soft_UARt_rx_en
 */
void soft_uart_rx_enable(void){
	gpio_set_interrupt(g_soft_uart_rec.rx_pin, POL_FALLING);
	irq_enable();
}

/**
 * @brief     This function turns off the UART receiver function
 * @return    none
 * notes: 
 *       When this function is used for the first time, it must be initialized in the following order:  
 *       1, sofT_UART_rx_init
 *       2, soft_UARt_rx_en (when needed, call it), then if you want to disable this function, 
 *          you can use soft_UARt_rx_dis to implement, when the function is enabled again, call soft_UARt_rx_en
 */
void soft_uart_rx_dis(void){
    gpio_en_interrupt(g_soft_uart_rec.rx_pin,0);
	timer_stop(g_soft_uart_rec.timer_num);
	/*
		mark the timer state as idle (if the user does not want to continue to
		receive data, then the timer can be used for other things)
	*/
	g_soft_uart_rec.timer_application = 2;
}

/**
 * @brief     This function is to print the data received by the software UART
 * @param[in] par - buffer data index value stored in buffer
 * @return    none
 */
void soft_uart_rx_callback(unsigned char par){
#if(SOFT_UART_DELAYING == 1)
        sleep_ms(2000);
#endif
    /*
     * print the following data:
     * buffer data index value stored in buffer
     * the length of this buffer of data
     * the ASCII decimal value of the first byte of this buffer of data
     * the ASCII decimal value of the last byte of this buffer of data
     * */
    printf("%d-%d-%d-%d",par,g_soft_uart_rec.rec_length[par],g_soft_uart_rec_data[par][0],g_soft_uart_rec_data[par][g_soft_uart_rec.rec_length[par]-1]);
	/* If this buffer of data is marked as overflow, print the character N */
    if(1 == g_soft_uart_rec.data_over_flag[par]){
        g_soft_uart_rec.data_over_flag[par] = 0;
		printf("N");
    }
#if(SOFT_UART_PRINT_ALL == 1) //  select whether to print all data in one buffer
	    printf("\r\n");
	    for(unsigned int p = 0; p < g_soft_uart_rec.rec_length[par]; p++){
		    printf("%d:%d\r\n",p, g_soft_uart_rec_data[par][p]);
	    }
#endif
	printf("\r\n");
}

/**
 * @brief     This function is used to process the data received by the UART
 * @param[in] _cb - callback function for processing data
 * @return    none
 */
void soft_uart_rx_data_process(void (*_cb)(unsigned char)){
	for(unsigned char i = 0; i < SOFT_UART_REC_GROUP_MAX; i++){// scan all buffers
	    if(SOFT_UART_DATA_IS_WAIT == g_soft_uart_rec.sotf_uart_rx_buff_state[i]){// If the buffer is marked as waiting to be processed
	    	_cb(i); // process this buffer of data
			memset((char*)g_soft_uart_rec_data[i],0,g_soft_uart_rec.rec_length[i]);// buffer clear
			g_soft_uart_rec.rec_length[g_soft_uart_rec.index] = 0;	// receive length clear
			g_soft_uart_rec.sotf_uart_rx_buff_state[i]  = SOFT_UART_DATA_IS_EMPTY_REC; // mark this buffer of data as empty
		}
	}
}

void user_init(void){
	sleep_ms(500);
	soft_uart_rx_init(GPIO_PA4,9600); // configure GPIO_PA4 as a software UART receive pin,baud rate 9600
	soft_uart_rx_enable();			  // enable software UART receive function
}

void main_loop (void){
#if(SOFT_UART_PROCESSIN == 1) // select whether to process received data in main_loop
	soft_uart_rx_data_process(soft_uart_rx_callback);
#endif
}

#endif


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
#include "audio_data.h"
#define AUDIO_RATE_VAL			AUDIO_16K   //This configure must be consistent with usb_desc.c.

#define	MIC_BUFFER_SIZE			4096
volatile signed short MicBuf[MIC_BUFFER_SIZE>>1];
volatile signed short MicBuf_f[MIC_BUFFER_SIZE>>1];
volatile signed short MicBuf2[MIC_BUFFER_SIZE>>1];

#define FLASH_READ_ADDR			0x080000
#define FLASH_WRITE_ADDR		0x080000
#define FLASH_BUFF_LEN			256
#define RECORD_BTN				GPIO_PD0
#define PLAYER_BTN				GPIO_PD1

#define HALF_PTR_SIZE			0x800     // 16k is ok without flash write operation
#define QUARTER_PTR_SIZE		0x400     // 16k is ok without flash write operation, 8k is ok with flash write operation
#define ONE_EIGHTH_PTR_SIZE		0x200     // 16k is ok without flash write operation, 8k is ok with flash write operation
#define ONE_SIXTEENTH_PTR_SIZE	0x100     // 16k is ok without flash write operation

#define BUFF_SIZE				QUARTER_PTR_SIZE
#define BUFF_LEN				(0x1000/BUFF_SIZE)
#define AMIC_BIAS_PIN			     GPIO_PC4

volatile unsigned short rptr;
volatile unsigned char cnt;
volatile unsigned int n;
volatile unsigned int m;
# if(AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ)
//* Points within our sound sample.
 const short *buffer = NULL;
//* Number of octets left in the sample buffer.
 volatile unsigned int remaining = 0;
//*Number of half fifo length .
unsigned short half_buff_length=0;
#endif
void user_init()
{
#if (AUDIO_MODE ==AUDIO_AMIC_TO_SDM)
	//PC0 for BIAS setting.
	gpio_set_func(AMIC_BIAS_PIN, AS_GPIO);
	gpio_set_output_en(AMIC_BIAS_PIN, 1); 		//enable output
	gpio_set_input_en(AMIC_BIAS_PIN ,0);			//disable input
	gpio_write(AMIC_BIAS_PIN, 1);              	//BIAS OUTPUT 1
	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
#if(MCU_CORE_B85)
	//if use two amic, need two BIAS
	gpio_set_func(GPIO_PC5, AS_GPIO);
	gpio_set_output_en(GPIO_PC5, 1); 		//enable output
	gpio_set_input_en(GPIO_PC5 ,0);			//disable input
	gpio_write(GPIO_PC5, 1);              	//BIAS OUTPUT 1
	audio_set_amic_mode(AUDIO_AMIC_STEREO_MODE);
	audio_amic_init(AUDIO_RATE_VAL);
#elif(MCU_CORE_B87)
	audio_set_mute_pga(0);  ////enable audio need follow this step: 1 enable bias; 2 disable mute_pga;
    audio_amic_init(AUDIO_RATE_VAL);							  //3 init; 4 delay about 17ms; 5 enable mute_pga.
    sleep_ms(17);
    audio_set_mute_pga(1);
#endif


#if(MCU_CORE_B85)
    //SDM has Dual output, need 4pin(PB4/PB5/PB6/PB7),default single output use PB6/PB7, call it if want to use Dual output
	audio_set_sdm_output_mode(AUDIO_SDM_DUAL_OUTPUT);
	audio_set_sdm_output(AMIC, AUDIO_RATE_VAL,1);

#elif(MCU_CORE_B87)
    audio_set_sdm_output(GPIO_PB6_PB7,AMIC,AUDIO_RATE_VAL,1);
#endif


#elif (AUDIO_MODE ==AUDIO_DMIC_TO_SDM)
	gpio_set_func(GPIO_PA0, AS_DMIC);
	gpio_set_func(GPIO_PA1, AS_DMIC);
	gpio_set_input_en(GPIO_PA0, 1);
	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_dmic_init(AUDIO_RATE_VAL);
#if(MCU_CORE_B85)
	audio_set_sdm_output(DMIC,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,DMIC,AUDIO_RATE_VAL,1);
#endif
#elif AUDIO_MODE == AUDIO_AMIC_TO_CODEC
#if(MCU_CORE_B85)
	audio_set_codec(I2C_GPIO_GROUP_A3A4, CODEC_MODE_I2S_TO_HEADPHONE_LINEOUT,CLOCK_SYS_CLOCK_HZ);
#elif(MCU_CORE_B87)
	audio_set_codec(I2C_GPIO_SDA_A3,I2C_GPIO_SCL_A4, CODEC_MODE_I2S_TO_HEADPHONE_LINEOUT,CLOCK_SYS_CLOCK_HZ);
#endif
	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);
	audio_set_i2s_output(BUF_IN,AUDIO_RATE_VAL);

#elif(AUDIO_MODE == AUDIO_BUF_TO_SDM)
	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_buff_init(AUDIO_RATE_VAL);
#if(MCU_CORE_B85)
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,BUF_IN,AUDIO_RATE_VAL,1);
#endif
	/*set store space as 16k*/
	write_reg8(0x40004,32);

#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF)
	gpio_set_func(AMIC_BIAS_PIN, AS_GPIO);
	gpio_set_output_en(AMIC_BIAS_PIN, 1); 		//enable output
	gpio_set_input_en(AMIC_BIAS_PIN ,0);			//disable input
	gpio_write(AMIC_BIAS_PIN, 1);              	//BIAS OUTPUT 1
	/* set fifo0 as input */
	dfifo_set_dfifo1((unsigned short*)MicBuf2,MIC_BUFFER_SIZE);
#if(MCU_CORE_B85)
	audio_set_amic_mode(AUDIO_AMIC_MONO_MODE);
	audio_amic_init(AUDIO_RATE_VAL);
#elif(MCU_CORE_B87)
	audio_set_mute_pga(0);
	audio_amic_init(AUDIO_RATE_VAL);
	sleep_ms(17);
	audio_set_mute_pga(1);
#endif
	/*set store space as 16k*/
	write_reg8(0x40004,32);
#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF_TO_SDM)
	gpio_set_func(RECORD_BTN ,AS_GPIO);
	gpio_set_output_en(RECORD_BTN, 0); 			//enable output
	gpio_set_input_en(RECORD_BTN ,1);				//disable input
	gpio_setup_up_down_resistor(RECORD_BTN, PM_PIN_PULLUP_10K);

	gpio_set_func(PLAYER_BTN ,AS_GPIO);
	gpio_set_output_en(PLAYER_BTN, 0); 			//enable output
	gpio_set_input_en(PLAYER_BTN ,1);				//disable input
	gpio_setup_up_down_resistor(PLAYER_BTN, PM_PIN_PULLUP_10K);

	gpio_set_func(AMIC_BIAS_PIN, AS_GPIO);
	gpio_set_output_en(AMIC_BIAS_PIN, 1); 		//enable output
	gpio_set_input_en(AMIC_BIAS_PIN ,0);			//disable input
	gpio_write(AMIC_BIAS_PIN, 1);              	//BIAS OUTPUT 1

	/* set fifo1 as input */
	dfifo_set_dfifo1((unsigned short*)MicBuf2,MIC_BUFFER_SIZE);
	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
#elif (AUDIO_MODE ==AUDIO_AMIC_TO_SDM_BIDIR_SIGFIFO)
	//PC4 for BIAS setting
	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);

	audio_amic_init(AUDIO_RATE_VAL);
#if(MCU_CORE_B85)
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,BUF_IN,AUDIO_RATE_VAL,1);
#endif
	reg_dfifo_mode &= (~FLD_AUD_DFIFO0_OUT);
	write_reg8(0x40004,32);

#elif(AUDIO_MODE == AUDIO_AMIC_TO_SDM_BIDIR_BIDFIFO)

	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	/* set fifo1 as input */
	dfifo_set_dfifo1((unsigned short*)MicBuf2,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);
	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
#if(MCU_CORE_B85)
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,BUF_IN,AUDIO_RATE_VAL,1);
#endif
	reg_dfifo_mode &= (~FLD_AUD_DFIFO0_OUT);
#elif(AUDIO_CODEC_TO_CODEC&&(MCU_CORE_B85))
	audio_set_codec(I2C_GPIO_GROUP_A3A4, CODEC_MODE_LINE_IN_TO_LINEOUT_I2S,CLOCK_SYS_CLOCK_HZ);
	audio_i2s_init();
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	dfifo_set_dfifo1((unsigned short*)MicBuf_f,MIC_BUFFER_SIZE);
	audio_set_i2s_output(I2S_IN,AUDIO_32K);
#elif ((AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ))
    /*
     *set timer interval,it equals approximately the time of sdm play half depth of fifo.
     *for example :AUDIO_RATE_VAL=16K, fifo depth=4096 bytes,  the time of sdm play half depth of fifo(2048 bytes) is 64 ms(2048*(1/(16*2))).
     */
	timer1_set_mode(TIMER_MODE_SYSCLK,0,((1000000/(16))/(0x1000/MIC_BUFFER_SIZE)) * CLOCK_SYS_CLOCK_1US);//set timer62.5ms<64ms
	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_buff_init(AUDIO_RATE_VAL);
#if(MCU_CORE_B85)
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,BUF_IN,AUDIO_RATE_VAL,1);
#endif
	irq_enable();//enable global interrupt

	half_buff_length=MIC_BUFFER_SIZE>>1;
	buffer = bump;
	remaining = sizeof(bump) / sizeof(bump[0]);
	unsigned int block_len = (remaining <(MIC_BUFFER_SIZE>>2)) ? remaining :(MIC_BUFFER_SIZE>>2);
	timer_start(TIMER1);//trigger timer.
	audio_rx_data_from_sample_buff(buffer, block_len);////first write half number of dfifo0 depth( recommend set 4K),
	buffer += block_len;
	remaining -= block_len;	write_reg8(0x40004,32);

#endif
}



void main_loop (void)
{

#if(AUDIO_MODE == AUDIO_BUF_TO_SDM)

	m=0;
	while(m<read_reg8(0x40004))
	{
		/* load 4k audio data from flash to buffer*/
		for(unsigned int i=0;i<(MIC_BUFFER_SIZE/FLASH_BUFF_LEN);i++)
		{
			flash_read_page(FLASH_READ_ADDR+ m*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf_f+((FLASH_BUFF_LEN*i)/2)));
		}
		audio_rx_data_from_buff((signed char*)MicBuf_f,MIC_BUFFER_SIZE);
		while(reg_dfifo0_wptr < reg_dfifo0_rptr);
		m++;
	}

#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF)
	n=0;
	while(n<read_reg8(0x40004))
	{
		while(reg_dfifo1_wptr < (0x800+rptr));
		rptr=0x800;
		flash_erase_sector(FLASH_WRITE_ADDR + n*0x1000);
		for(unsigned int i=0;i<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);i++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+((FLASH_BUFF_LEN*i)/2)));
		}
		while(reg_dfifo1_wptr > rptr);
		rptr=0x00;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*8+(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+FLASH_BUFF_LEN*4+((FLASH_BUFF_LEN*j)/2)));
		}
		n++;
	}


#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF_TO_SDM)
	if(gpio_read(RECORD_BTN)==0)
	{
		sleep_ms(100);
		n=0;
		audio_amic_init(AUDIO_RATE_VAL);

		while(gpio_read(RECORD_BTN)==0)
		{
			while(reg_dfifo1_wptr < (0x200+rptr));
			rptr=0x200;
			flash_erase_sector(FLASH_WRITE_ADDR+n*0x1000);
			for(unsigned int i=0;i<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);i++)
			{
				flash_write_page(FLASH_WRITE_ADDR + n*0x1000 + (FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+((FLASH_BUFF_LEN*i)/2)));
			}
			while(reg_dfifo1_wptr > rptr);
			rptr=0x00;
			for(unsigned int j=0;j<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);j++)
			{
				flash_write_page(FLASH_WRITE_ADDR + n*0x1000 + FLASH_BUFF_LEN*8+(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+FLASH_BUFF_LEN*4+((FLASH_BUFF_LEN*j)/2)));
			}
			n++;
			if(n>32)break;
		}

	}

	if(gpio_read(PLAYER_BTN)==0)
	{
		sleep_ms(100);
		m=0;

		audio_buff_init(AUDIO_RATE_VAL);

#if(MCU_CORE_B85)
	audio_set_sdm_output(AMIC,AUDIO_RATE_VAL,1);
#elif(MCU_CORE_B87)
	audio_set_sdm_output(GPIO_PB6_PB7,AMIC,AUDIO_RATE_VAL,1);
#endif

		while((n>=m)&&(gpio_read(PLAYER_BTN)==0))
		{
			for(unsigned int i=0;i<(MIC_BUFFER_SIZE/FLASH_BUFF_LEN);i++)
			{
				flash_read_page(FLASH_READ_ADDR + m*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf_f+((FLASH_BUFF_LEN*i)/2)));
			}
			audio_rx_data_from_buff((signed char*)MicBuf_f,MIC_BUFFER_SIZE);
			while(reg_dfifo0_wptr < reg_dfifo0_rptr);
			m++;
		}
	}
#elif(AUDIO_MODE == AUDIO_AMIC_TO_SDM_BIDIR_SIGFIFO)
	/* 1.record amic data to buffer0 */
	n=0;
	while(n<read_reg8(0x40004))
	{
		while(reg_dfifo0_wptr < (0x400+rptr));
		rptr=0x400;
		flash_erase_sector(FLASH_WRITE_ADDR + n*0x1000);
		for(unsigned int i=0;i<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);i++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+((FLASH_BUFF_LEN*i)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+((FLASH_BUFF_LEN*i)/2)));
		}
		reg_dfifo_mode |= FLD_AUD_DFIFO0_OUT;
		while(reg_dfifo0_wptr < (0x400+rptr));
		rptr=0x800;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*4 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*2 + ((FLASH_BUFF_LEN*j)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +FLASH_BUFF_LEN*4 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*2 + ((FLASH_BUFF_LEN*j)/2)));
		}
		while(reg_dfifo0_wptr < (0x400+rptr));
		rptr=0xc00;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*8 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*4 + ((FLASH_BUFF_LEN*j)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +FLASH_BUFF_LEN*8 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*4 + ((FLASH_BUFF_LEN*j)/2)));
		}
		while(reg_dfifo0_wptr > rptr);
		rptr=0x00;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*12 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*6 + ((FLASH_BUFF_LEN*j)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +FLASH_BUFF_LEN*12 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*6 + ((FLASH_BUFF_LEN*j)/2)));
		}
		n++;
	}

#elif(AUDIO_MODE == AUDIO_AMIC_TO_SDM_BIDIR_BIDFIFO)
	n=0;
	while(n<read_reg8(0x40004))
	{
		for(unsigned int j=0;j<BUFF_LEN;j++)
		{
			if(j==(BUFF_LEN-1))
			{
				while(reg_dfifo1_wptr > rptr);
				rptr=0;
			}
			else
			{
				while(reg_dfifo1_wptr < (BUFF_SIZE+rptr));
				rptr=BUFF_SIZE*(j+1);
			}

			if(j==0)flash_erase_sector(FLASH_WRITE_ADDR + n*0x1000);
			for(unsigned int i=0;i<((MIC_BUFFER_SIZE/BUFF_LEN)/FLASH_BUFF_LEN);i++)
			{
				/* operation of flash  will lead to delay of read ptr and let sdm play with missing data, you should control it by some ways*/
#if(AUDIO_RATE_VAL==AUDIO_8K)
				/* user can write data to flash to delay rptr */
				flash_write_page(FLASH_WRITE_ADDR + n*0x1000 + BUFF_SIZE*j + (FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+(BUFF_SIZE*j/2)+((FLASH_BUFF_LEN*i)/2)));
#elif(AUDIO_RATE_VAL==AUDIO_16K)
                /* user should avoid writing data to flash to delay rptr*/
#endif
				flash_read_page(FLASH_READ_ADDR+ n*0x1000 + BUFF_SIZE*j + (FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+(BUFF_SIZE*j/2)+((FLASH_BUFF_LEN*i)/2)));
			}
			if(j==15)
			{
				reg_dfifo0_wptr=0;
			}
			else
			{
				reg_dfifo0_wptr = BUFF_SIZE*(j+1);
			}
			if(j==0) reg_dfifo_mode |= FLD_AUD_DFIFO0_OUT;
		}
		n++;
	}

#endif
}



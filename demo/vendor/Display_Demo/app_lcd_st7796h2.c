/********************************************************************************************************
 * @file	lcd_st7796h2.c
 *
 * @brief	This is the source file for B91
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
#include "app_config.h"
#if (SPI_DISPLAY_SELECT == SPI_LCD)
#include "pic.h"

#define SPI_CLK 				4000000

#define LCD_RESET_PIN			GPIO_PB0

#define	LCD_1DATA_LANE			1
#define	LCD_2DATA_LANE			2
#define LCD_DATA_LANE_MODE		LCD_2DATA_LANE

#define LCD_DMA_EN   			0

 spi_pin_config_t spi_pin_config = {

	.spi_clk_pin	   = GPIO_PA0,
	.spi_csn_pin 	   = GPIO_PA4,
	.spi_mosi_io0_pin  = GPIO_PB2,
	.spi_miso_io1_pin  = GPIO_PB5,//3line mode set none
	.spi_wp_io2_pin    = 0,//set quad mode otherwise set none,only PB3 support
	.spi_hold_io3_pin  = 0,//set quad mode otherwise set none, only PD4 support

 };

spi_config_t spi_lcd_protocol_config = {
	.spi_io_mode	  = SPI_SINGLE_MODE,
	.spi_dummy_cnt   = 0,
	.spi_cmd_en 	  = 0,
	.spi_addr_en 	  = 0,
	.spi_addr_len 	  = 0,//when spi_addr_en = false,invalid set.
	.spi_cmd_fmt_en  = 0,//when spi_cmd_en = false,invalid set.
	.spi_addr_fmt_en = 0,//when spi_addr_en = false,invalid set.
};

#if  RGB_SELECT==RGB565
#define RED    0x07E0
#define GREEN  0x001F
#define BLUE   0xF800
#define WHITE  0xFFFF
#define BLACK  0x0000
#elif RGB_SELECT==RGB888
#define RED    0x0000FF
#define GREEN  0x00FF00
#define BLUE   0xFF0000
#define WHITE  0xFFFFFF
#define BLACK  0x000000
#elif RGB_SELECT==RGB666
#define RED    0x0000FC
#define GREEN  0x00FC00
#define BLUE   0xFC0000
#define WHITE  0xFCFCFC
#define BLACK  0x000000
#endif

/**
 * @brief    This function serves to reset lcd.
 * @return   none
 */
void lcd_reset()
{
	gpio_write(LCD_RESET_PIN,0);
	sleep_ms(120);
	gpio_write(LCD_RESET_PIN,1);
	sleep_ms(120);
}


/**
 * @brief    This function serves to write one byte cmd to lcd.
 * @param[in] cmd - cmd byte.
 * @return   none
 */
void lcd_write_cmd(unsigned char cmd)
{
	spi_set_3line_dcx_cmd();
	spi_master_write(&cmd,1);
}
/**
 * @brief    This function serves to write one byte data to lcd.
 * @param[in] data - data byte.
 * @return   none
 */
void lcd_write_data(unsigned char data)
{
	spi_set_3line_dcx_data();
	spi_master_write(&data,1);
}


/**
 * @brief     	This function serves to normal write data repeatedly.
 * @param[in] 	data 		- the pointer to the data for write.
 * @param[in] 	len 		- write length.
 * @param[in] 	repeat_time - number of times to write data repeatedly.
 * @return  	none
 */
void spi_master_write_repeat(unsigned char *data, unsigned int len, unsigned int repeat_time)
{
	unsigned int i = 0, j = 0,k = 0;
	spi_tx_dma_dis();
	spi_tx_fifo_clr();
	spi_tx_cnt(len*repeat_time);
	spi_set_transmode( SPI_MODE_WRITE_ONLY);
	spi_master_set_cmd(0x00);//when  cmd  disable that  will not sent cmd,just trigger spi send .
	for (i = 0; i < repeat_time; i++)
	{
		for (j = 0; j < len; j++,k++)
			{
				while (reg_spi_fifo_state & FLD_SPI_TXF_FULL);
				reg_spi_wr_rd_data(k%4) = data[j];
			}
	}
	while (spi_is_busy());
}





/**
 * @brief    This function serves to write operating memory address and space size to lcd.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @return   none
 */
void lcd_set_address(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta)
{
	unsigned char x_h,x_l,xe_h,xe_l;
	unsigned char y_h,y_l,ye_h,ye_l;
	x_h = x>>8;
	x_l = x&0xff;
	xe_h = (x_delta+x-1)>>8;
	xe_l = (x_delta+x-1)&0xff;

	y_h = y>>8;
	y_l = y&0xff;
	ye_h = (y_delta+y-1)>>8;
	ye_l = (y_delta+y-1)&0xff;

	while (spi_is_busy());
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_CLOSE);
	spi_set_normal_mode();

	lcd_write_cmd(0x11);//Sleep out
	lcd_write_cmd(0x2A);
	lcd_write_data(x_h);
	lcd_write_data(x_l);
	lcd_write_data(xe_h);
	lcd_write_data(xe_l);

	lcd_write_cmd(0x2B);
	lcd_write_data(y_h);
	lcd_write_data(y_l);
	lcd_write_data(ye_h);
	lcd_write_data(ye_l);

	lcd_write_cmd(0x2c);//enable write
}

/**
 * @brief    This function serves to fill a rectangular space of lcd with single color.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @param[in] color - the single color.
 * @return   none
 */
void lcd_fill_rectangle(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,unsigned int color)
{
	unsigned int display_size;
	lcd_set_address(x, x_delta, y, y_delta);
	display_size = x_delta * y_delta;
	spi_set_3line_dcx_data();

#if LCD_DATA_LANE_MODE == LCD_2DATA_LANE
#if RGB_SELECT==RGB565
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_RGB565);
#elif RGB_SELECT==RGB666
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_RGB666);
#elif RGB_SELECT==RGB888
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_RGB888);
#endif
	spi_set_dual_mode();
#endif

#if RGB_SELECT==RGB565
	spi_master_write_repeat((unsigned char*)&color, 2, display_size);
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	spi_master_write_repeat((unsigned char*)&color, 3, display_size);
#endif
}

/**
 * @brief    This function serves to draw a 1*1 point in lcd with single color.
 * @param[in] x - abscissa of address.
 * @param[in] y - ordinate of address.
 * @param[in] color - the single color.
 * @return   none
 */
void lcd_draw_point(unsigned short x,unsigned short y,unsigned int color)
{
	lcd_fill_rectangle(x, 1, y, 1,color);
}

/**
 * @brief     	This function servers to config lcd.
 * @return  	none
 * @attention 	The configuration process comes from the screen manufacturer,
 * 				 and the specific meaning can be compared with the screen manual.
 */
void lcd_config(void)
{
	lcd_reset();

	lcd_write_cmd(0x11);//Sleep out
	sleep_ms(120);       //ms

	lcd_write_cmd(0xF0);// Command Set Control
	lcd_write_data(0xC3);//Enable command2 part1

	lcd_write_cmd(0xF0);// Command Set Control
	lcd_write_data(0x96);//Enable command2 part2

	lcd_write_cmd(0x36);// Memory Access Control
	lcd_write_data(0x48);

	lcd_write_cmd(0x3A);  //Interface Pixel Format
#if  RGB_SELECT==RGB565
	lcd_write_data(0x05);	//RGB565
#elif RGB_SELECT==RGB666
	lcd_write_data(0x06);  //RGB666
#elif RGB_SELECT==RGB888
	lcd_write_data(0x07);  //RGB888
#endif
	lcd_write_cmd(0xB4);//Display Inversion Control
	lcd_write_data(0x00); //Column inversion

	lcd_write_cmd(0xB1);//Frame Rate Control (In Normal Mode/Full Colors)
	lcd_write_data(0xA0);
	lcd_write_data(0x08);

	lcd_write_cmd(0xB5);//Blanking Porch Control
	lcd_write_data(0x02);//VFP
	lcd_write_data(0x02);//VBP
	lcd_write_data(0x00);//reserved
	lcd_write_data(0x04);//HBP

	lcd_write_cmd(0xB6);// Display Function Control
	lcd_write_data(0x00); //Normal
	lcd_write_data(0x02); //Normal 0x00
	lcd_write_data(0x3B);// Sets the max number of lines to drive the LCD:8*(0x3b+1) = 480 line.

	lcd_write_cmd(0xB9);// Mode Selection
	lcd_write_data(0x00); //Digital gamma function off, dithering function control off.


#if  LCD_DATA_LANE_MODE == LCD_2DATA_LANE
	lcd_write_data(0xE1);//LCD_2DATA_LANE enable 0x01
#elif LCD_DATA_LANE_MODE == LCD_1DATA_LANE
	lcd_write_data(0xE0);// 2-Lane SPI function disable 0x00
#endif

	lcd_write_cmd(0xC0);//Power Control 1
	lcd_write_data(0xF0); // AVDD:6.8V, AVCL: -5.0
	lcd_write_data(0x00); // VGH:12.541, VGL:-7.158

	lcd_write_cmd(0xC1);//Power Control 2
	lcd_write_data(0x1F); //VOP=5.1

	lcd_write_cmd(0xC2);//Power Control 3
	lcd_write_data(0xA7);//SOP:0x02,GOP:0x03

	lcd_write_cmd(0xC5);//VCOM Control(VCOM: a power supply for the TFT-LCD common electrode)
	lcd_write_data(0x15); //VCOM: 0.825

	lcd_write_cmd(0xE8);//Display Output Ctrl Adjust
	lcd_write_data(0x40);
	lcd_write_data(0x8A);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x29);// Set Source equalizing period time. (0x9*1.5 + 9)us
	lcd_write_data(0x19);//	Gate timing Control G_START:Tclk*0x19
	lcd_write_data(0xA5);// G_EQ: Gate driver EQ function ON. Gate timing Control G_END:Tclk*0x25
	lcd_write_data(0x33);

	lcd_write_cmd(0xE0);// Positive Gamma Control, adjust the gamma characteristics of the TFT panel.
	lcd_write_data(0xF0);
	lcd_write_data(0x10);
	lcd_write_data(0x16);
	lcd_write_data(0x0C);
	lcd_write_data(0x0A);
	lcd_write_data(0x15);
	lcd_write_data(0x3A);
	lcd_write_data(0x43);
	lcd_write_data(0x4E);
	lcd_write_data(0x18);
	lcd_write_data(0x14);
	lcd_write_data(0x13);
	lcd_write_data(0x2E);
	lcd_write_data(0x33);

	lcd_write_cmd(0xE1);//Negative Gamma Control,adjust the gamma characteristics of the TFT panel.
	lcd_write_data(0xF0);
	lcd_write_data(0x10);
	lcd_write_data(0x16);
	lcd_write_data(0x0C);
	lcd_write_data(0x0A);
	lcd_write_data(0x15);
	lcd_write_data(0x3A);
	lcd_write_data(0x43);
	lcd_write_data(0x4E);
	lcd_write_data(0x18);
	lcd_write_data(0x14);
	lcd_write_data(0x13);
	lcd_write_data(0x2E);
	lcd_write_data(0x33);

	lcd_write_cmd(0xF0);//Command Set Control
	lcd_write_data(0x3C);//Disable command2 part1

	lcd_write_cmd(0xF0);//Command Set Control

	lcd_write_data(0x69);//Disable command2 part2

	lcd_write_cmd(0x29);//Display on

	lcd_write_cmd(0x21);//Display Inversion On

	lcd_write_cmd(0x35);//Tearing Effect setting

}

/**
 * @brief    This function serves to show a rectangular picture in lcd.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of picture.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of picture.
 * @param[in] pic_addr - address of picture buffer.
 * @return   none
 */
void lcd_show_picture(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,  unsigned char *pic_addr)
{
	unsigned int display_size;
	lcd_set_address(x, x_delta, y, y_delta);
	spi_set_3line_dcx_data();

#if  LCD_DATA_LANE_MODE == LCD_2DATA_LANE
#if RGB_SELECT==RGB565
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_RGB565);
#elif RGB_SELECT==RGB666
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_RGB666);
#elif RGB_SELECT==RGB888
	spi_set_panel_2data_lane_mode(SPI_2DATA_LANE_RGB888);
#endif
	spi_set_dual_mode();
#endif
#if RGB_SELECT==RGB565
	display_size=2*x_delta * y_delta;
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	display_size=3*x_delta * y_delta;
#endif
	spi_master_write_plus_fetch_flash(0x00,0x00,(unsigned char*)pic_addr, display_size, SPI_MODE_WRITE_ONLY );

}

void user_init()
{
	gpio_set_output_en(LCD_RESET_PIN,1);
	gpio_set_input_en(LCD_RESET_PIN,0);
	gpio_write(LCD_RESET_PIN,1);
	gpio_set_func(LCD_RESET_PIN,AS_GPIO);

	spi_master_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*SPI_CLK)-1),SPI_MODE0);
	spi_set_pin(&spi_pin_config);
	spi_master_config_plus(&spi_lcd_protocol_config);
	spi_3line_dcx_en();
	lcd_config();
	//show a picture in lcd
#if RGB_SELECT==RGB565
	lcd_show_picture(0,320,0,380,(unsigned char *)telink_log_565);
#elif RGB_SELECT==RGB666
	lcd_show_picture(0,320,0,380,(unsigned char *)telink_log_666);
#elif RGB_SELECT==RGB888
	lcd_show_picture(0,320,0,380,(unsigned char *)telink_log_888);
#endif
	//fill a rectangle with red color in lcd
	lcd_fill_rectangle(150,100,300,30,RED);
}
volatile unsigned short point_x=150;
volatile unsigned short point_y=0;


void main_loop (void)
{
	if(point_y != 280)
	{
		//draw a line with blue color in lcd
		lcd_draw_point(point_x,point_y,RED);
		point_y ++;
	}
}
#endif

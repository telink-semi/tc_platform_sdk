/********************************************************************************************************
 * @file	app_oled_rm69330.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
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
#if (SPI_DISPLAY_SELECT == SPI_OLED)
#include "app_oled_rm69330.h"
#include "pic.h"

#define SPI_CLK	    					  		1000000

#define OLED_RESET_PIN			GPIO_PB0

#define	OLED_1DATA_LANE			 1
#define	OLED_4DATA_LANE			 2
#define OLED_DATA_LANE_MODE	  OLED_4DATA_LANE

typedef enum{
	OLED_SINGLE_WRITE_CMD = 0x02,
	OLED_SINGLE_CMD_QUAD_ADDR_WRITE_CMD = 0x12,
	OLED_SINGLE_CMD_ADDR_QUAD_WRITE_CMD = 0x32,
}oled_cmd_e;

spi_pin_config_t spi_pin_config = {
	.spi_clk_pin	   = GPIO_PA0,
	.spi_csn_pin 	   = GPIO_PA4,
	.spi_mosi_io0_pin  = GPIO_PB2,
	.spi_miso_io1_pin  = GPIO_PB4,//3line mode set none
	.spi_wp_io2_pin    = GPIO_PB3,//set quad mode otherwise set none,only PB3 support
	.spi_hold_io3_pin  = GPIO_PD4,//set quad mode otherwise set none, only PD4 support

};

spi_config_t spi_oled_protocol_config = {
	.spi_io_mode		= SPI_SINGLE,
	.spi_dummy_cnt 	= 0,
	.spi_cmd_en		= 1,
	.spi_addr_en		= 1,//must
	.spi_addr_len		= 3,//when spi_addr_en = false, invalid set.
	.spi_cmd_fmt_en	= 0,//when spi_cmd_en = false, invalid set.
	.spi_addr_fmt_en	= 1,//when spi_addr_en = false, invalid set.
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
 * @brief    This function serves to reset oled.
 * @return   none
 */
void oled_reset()
{
	gpio_write(OLED_RESET_PIN,0);
	sleep_ms(120);
	gpio_write(OLED_RESET_PIN,1);
	sleep_ms(120);
}

/**
 * @brief    This function serves to write "cmd(0x02) + address + data" to oled.
 * @param[in] 	addr 	- the operating address of oled.
 * @param[in] 	data 		-  pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @return   none
 */
void oled_spi_master_write_data(unsigned int addr, unsigned char *data, unsigned int data_len)
{
	 spi_master_write_plus(OLED_SINGLE_WRITE_CMD,addr,data,data_len,SPI_MODE_WR_WRITE_ONLY);
}

/**
 * @brief    This function serves to write  "cmd(0x02) + address" to oled.
 * @param[in] 	addr 	- the operating address of oled.
 * @return   none
 */
void oled_spi_master_write_cmd(unsigned int addr)
{
	 spi_master_write_plus(OLED_SINGLE_WRITE_CMD,addr,0,0,SPI_MODE_NONE_DATA);
}

/**
 * @brief     	This function serves to single/dual/quad write data to the SPI slave repeatedly.
 * @param[in] 	cmd 		- cmd one byte will first write.
 * @param[in] 	addr 		- the address of slave.
 * @param[in] 	data 		- pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @param[in] 	wr_mode 	- write mode.dummy or not dummy.
 * @param[in] 	repeat_time - number of times to write data repeatedly.
 * @return  	none
 * @attention 	Only data would be written repeatedly. the typical sending order is cmd + address + data * repeat_time,
 * 				cmd and address would not be repeated.
 */
void spi_master_write_repeat_plus( unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int len, unsigned int repeat_time, spi_wr_tans_mode_e wr_mode)
{
	unsigned int i = 0, j = 0, k = 0;
	spi_tx_dma_dis();
	spi_tx_fifo_clr();
	if (addr)
	{
		spi_set_address(addr);
	}
	spi_tx_cnt(len*repeat_time);
	spi_set_transmode( wr_mode);
	spi_master_set_cmd(cmd);
	for (i = 0; i < repeat_time; i++)
	{
		for (j = 0; j < len; j++,k++)
			{
				while (reg_spi_fifo_state & FLD_SPI_TXF_FULL);
				reg_spi_wr_rd_data(k % 4) = data[j];
			}
	}
	while (spi_is_busy());
}

/**
 * @brief    This function serves to write operating memory address and space size to oled.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @return   none
 * @attention 	For rm69330 oled param x, y, x_delta, y_delta must be multiple of 2.
 */
void oled_set_address(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta)
{
	unsigned char data[4] = {0};
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
	spi_set_normal_mode();

	data[0] = x_h;
	data[1] = x_l;
	data[2] = xe_h;
	data[3] = xe_l;
	oled_spi_master_write_data(0x002A00, data, 4);

	data[0] = y_h;
	data[1] = y_l;
	data[2] = ye_h;
	data[3] = ye_l;
	oled_spi_master_write_data(0x002B00, data, 4);
}

/**
 * @brief    This function serves to fill a rectangular space of oled with single color.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @param[in] color - the single color.
 * @return   none
 * @attention 	For rm69330 oled param x, y, x_delta, y_delta must be multiple of 2.
 */
void oled_fill_rectangle(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,unsigned int color)
{
	unsigned int display_size;
	oled_set_address(x, x_delta, y, y_delta);
	display_size = x_delta * y_delta;

#if OLED_DATA_LANE_MODE == OLED_4DATA_LANE
	spi_set_quad_mode();
#if RGB_SELECT==RGB565
	spi_master_write_repeat_plus(OLED_SINGLE_CMD_QUAD_ADDR_WRITE_CMD, 0x002C00,(unsigned char*)&color, 2, display_size,SPI_MODE_WR_WRITE_ONLY);
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	spi_master_write_repeat_plus(OLED_SINGLE_CMD_QUAD_ADDR_WRITE_CMD, 0x002C00,(unsigned char*)&color, 3, display_size,SPI_MODE_WR_WRITE_ONLY);
#endif
#else
#if RGB_SELECT==RGB565
	spi_master_write_repeat_plus(OLED_SINGLE_WRITE_CMD, 0x002C00,(unsigned char*)&color, 2, display_size,SPI_MODE_WR_WRITE_ONLY);
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	spi_master_write_repeat_plus(OLED_SINGLE_WRITE_CMD, 0x002C00,(unsigned char*)&color, 3, display_size,SPI_MODE_WR_WRITE_ONLY);
#endif
#endif
}

/**
 * @brief    This function serves to draw a 2*2 point in oled with single color.
 * @param[in] x - abscissa of address.
 * @param[in] y - ordinate of address.
 * @param[in] color - the single color.
 * @return   none
 * @attention 	For rm69330 oled param x, y, must be multiple of 2.
 */
void oled_draw_point(unsigned short x,unsigned short y,unsigned int color)
{
	oled_fill_rectangle(x, 2, y, 2,color);
}

/**
 * @brief     	This function servers to config oled.
 * @return  	none
 * @attention 	The configuration process comes from the screen manufacturer,
 * 				 and the specific meaning can be compared with the screen manual.
 */
void oled_config(void)
{
	oled_reset();
	oled_spi_master_write_cmd(0x001100);//Sleep Out
	sleep_ms(120);
	for(int i = 0; i < 284; i++)
	{
		oled_spi_master_write_data((unsigned int)(oled_init[i].oled_addr), (unsigned char*)(&(oled_init[i].oled_data)), 1);
	}

#if  RGB_SELECT==RGB565
	oled_spi_master_write_data((unsigned int)(oled_rgb565.oled_addr), (unsigned char*)(&(oled_rgb565.oled_data)), 1);
#elif RGB_SELECT==RGB666
	oled_spi_master_write_data((unsigned int)(oled_rgb666.oled_addr), (unsigned char*)(&(oled_rgb666.oled_data)), 1);
#elif RGB_SELECT==RGB888
	oled_spi_master_write_data((unsigned int)(oled_rgb888.oled_addr), (unsigned char*)(&(oled_rgb888.oled_data)), 1);
#endif
	oled_spi_master_write_cmd(0x002900);//Display On
}

/**
 * @brief    This function serves to show a rectangular picture in oled.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of picture.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of picture.
 * @param[in] pic_addr - address of picture buffer.
 * @return   none
 * @attention 	For rm69330 oled param x, y, x_delta, y_delta must be multiple of 2.
 */
void oled_show_picture(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,  unsigned char *pic_addr)
{
	unsigned int display_size;
	oled_set_address(x, x_delta, y, y_delta);

#if RGB_SELECT==RGB565
	display_size=2*x_delta * y_delta;
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	display_size=3*x_delta * y_delta;
#endif

#if  OLED_DATA_LANE_MODE == OLED_4DATA_LANE
	spi_set_quad_mode();
	spi_master_write_plus_fetch_flash(OLED_SINGLE_CMD_QUAD_ADDR_WRITE_CMD,0x002C00,pic_addr,display_size,SPI_MODE_WR_WRITE_ONLY);
#else
	spi_master_write_plus_fetch_flash(OLED_SINGLE_WRITE_CMD,0x002C00,pic_addr,display_size,SPI_MODE_WR_WRITE_ONLY);
#endif
}
void user_init()
{
	gpio_set_output_en(OLED_RESET_PIN,1);
	gpio_set_input_en(OLED_RESET_PIN,0);
	gpio_write(OLED_RESET_PIN,1);
	gpio_set_func(OLED_RESET_PIN,AS_GPIO);
	spi_master_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*SPI_CLK)-1),SPI_MODE0);
	spi_set_pin(&spi_pin_config);
	spi_master_config_plus(&spi_oled_protocol_config);
	oled_config();

	//show a picture in oled
#if RGB_SELECT==RGB565
	oled_show_picture(70,320,40,380,(unsigned char *)telink_log_565);
#elif RGB_SELECT==RGB666
	oled_show_picture(70,320,40,380,(unsigned char *)telink_log_666);
#elif RGB_SELECT==RGB888
	oled_show_picture(70,320,40,380,(unsigned char *)telink_log_888);
#endif
	//fill a rectangle with red color in oled
	oled_fill_rectangle(60,50,150,200,RED);
}
volatile unsigned short point_x=120;
volatile unsigned short point_y=200;
void main_loop (void)
{

	if(point_y != 300)
	{
		//draw a line with blue color in oled
		oled_draw_point(point_x,point_y,RED);
		point_y +=2;
	}

}
#endif

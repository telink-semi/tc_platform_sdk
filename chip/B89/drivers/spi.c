/********************************************************************************************************
 * @file	spi.c
 *
 * @brief	This is the source file for B89
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "spi.h"
#include "register.h"
#include "gpio.h"

/**
 * @brief     This function selects a pin port for the SPI interface (master mode)
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */


void spi_master_gpio_set(SPI_GPIO_SclkTypeDef sclk_pin,SPI_GPIO_CsTypeDef cs_pin,SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin)
{
	if(sclk_pin == SPI_GPIO_SCL_A5 ||sclk_pin == SPI_GPIO_SCL_A4 ||sclk_pin == SPI_GPIO_SCL_D0)
	{
		switch(sclk_pin)
		{
			case SPI_GPIO_SCL_A5 :  gpio_set_mux(SPI_GPIO_SCL_A5 ,PA5_SPI_CK); break;
			case SPI_GPIO_SCL_A4 :  gpio_set_mux(SPI_GPIO_SCL_A4 ,PA4_SPI_CK); break;
			case SPI_GPIO_SCL_D0 :  gpio_set_mux(SPI_GPIO_SCL_D0 ,PD0_SPI_CK); break;
		}
	}
	if(sdo_pin == SPI_GPIO_SDO_A2 ||sdo_pin == SPI_GPIO_SDO_B5 ||sdo_pin == SPI_GPIO_SDO_B0 ||sdo_pin == SPI_GPIO_SDO_B1 ||sdo_pin == SPI_GPIO_SDO_D3)
	{
		switch(sdo_pin)
		{
			case SPI_GPIO_SDO_A2 :  gpio_set_mux(SPI_GPIO_SDO_A2 ,PA2_SPI_DO); break;
			case SPI_GPIO_SDO_B5 :  gpio_set_mux(SPI_GPIO_SDO_B5 ,PB5_SPI_DO); break;
			case SPI_GPIO_SDO_B0 :  gpio_set_mux(SPI_GPIO_SDO_B0 ,PB0_SPI_DO); break;
			case SPI_GPIO_SDO_B1 :  gpio_set_mux(SPI_GPIO_SDO_B1 ,PB1_SPI_DO); break;
			case SPI_GPIO_SDO_D3 :  gpio_set_mux(SPI_GPIO_SDO_D3 ,PD3_SPI_DO); break;
		}
	}
	if(sdi_pin == SPI_GPIO_SDI_A0 ||sdi_pin == SPI_GPIO_SDI_A6 ||sdi_pin == SPI_GPIO_SDI_B6 ||sdi_pin == SPI_GPIO_SDI_D4)
	{
		switch(sdi_pin)
		{
			case SPI_GPIO_SDI_A0 :  gpio_set_mux(SPI_GPIO_SDI_A0 ,PA0_SPI_DI); break;
			case SPI_GPIO_SDI_A6 :  gpio_set_mux(SPI_GPIO_SDI_A6 ,PA6_SPI_DI); break;
			case SPI_GPIO_SDI_B6 :  gpio_set_mux(SPI_GPIO_SDI_B6 ,PB6_SPI_DI); break;
			case SPI_GPIO_SDI_D4 :  gpio_set_mux(SPI_GPIO_SDI_D4 ,PD4_SPI_DI); break;
		}
	}
	BM_CLR(reg_gpio_func(sclk_pin), sclk_pin & 0xff);//disable GPIO function
	BM_CLR(reg_gpio_func(sdo_pin), sdo_pin & 0xff);
	BM_CLR(reg_gpio_func(sdi_pin), sdi_pin & 0xff);
	gpio_set_input_en(sdi_pin,1); //enable input function
    spi_masterCSpin_select(cs_pin);
}
/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */


void spi_masterCSpin_select(GPIO_PinTypeDef CSPin)
{
	gpio_set_func(CSPin,AS_GPIO);//enable GPIO function
	gpio_set_input_en(CSPin,0); //disable input function
	gpio_set_output_en(CSPin,1);//enable out put
	gpio_write(CSPin,1);//output high level in idle state
}

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module
 *            Telink spi supports four standard working modes
 *            register  0x0b set working mode
 *            bit0:CPOL-Clock Polarity  ; bit1:CPHA-Clock Phase
 *            MODE0: CPOL = 0 , CPHA =0;
 *            MODE1: CPOL = 0 , CPHA =1;
 *            MODE2: CPOL = 1 , CPHA =0;
 *            MODE3: CPOL = 1 , CPHA =1;
 * @return    none
 */
void spi_master_init(unsigned char DivClock, SPI_ModeTypeDef Mode)
{
	reg_clk_en0 |= FLD_CLK0_SPI_EN;//enable spi clock
	reg_spi_sp = 0;            //clear register(0x0a) value
	reg_spi_sp|= DivClock;     //0x0a: bit0~bit6 set spi clock ; spi clock=system clock/((DivClock+1)*2)
	reg_spi_sp|=FLD_SPI_ENABLE;//0x0a: bit7 enables spi function mode
	reg_spi_ctrl|= FLD_SPI_MASTER_MODE_EN; //0x09: bit1 enables master mode
	reg_spi_inv_clk = ((reg_spi_inv_clk & (~FLD_SPI_MODE_WORK_MODE)) | Mode);// select SPI mode,surpport four modes
}


/**
 * @brief      This function serves to write a bulk of data to the SPI slave
 *             device specified by the CS pin
 * @param[in]  Cmd - pointer to the command bytes needed written into the
 *             slave device first before the writing operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes
 * @param[in]  Data - pointer to the data need to write
 * @param[in]  DataLen - length in byte of the data need to write
 * @param[in]  CSPin - the CS pin specifying the slave device
 * @return     none
 */
void spi_write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin)
{
    int i = 0;
    gpio_write(CSPin,0);//CS level is low

    reg_spi_ctrl &= ~FLD_SPI_DATA_OUT_DIS; //0x09- bit2 enables spi data output
    reg_spi_ctrl &= ~FLD_SPI_RD; //enable write,0x09-bit3 : 0 for read ,1 for write

    /***write Cmd***/
    for (i = 0; i < CmdLen; i++) {

    	reg_spi_data = Cmd[i];
        while(reg_spi_ctrl& FLD_SPI_BUSY); //wait writing finished
    }

    /***write Data***/
    for (i = 0; i < DataLen; i++) {
    	reg_spi_data = Data[i];
        while(reg_spi_ctrl & FLD_SPI_BUSY); //wait writing finished
    }
    /***pull up CS***/
    gpio_write(CSPin,1);//CS level is high
}

/**
 * @brief      This function serves to read a bulk of data from the SPI slave
 *             device specified by the CS pin
 * @param[in]  Cmd - pointer to the command bytes needed written into the
 *             slave device first before the reading operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes
 * @param[out] Data - pointer to the buffer that will cache the reading out data
 * @param[in]  DataLen - length in byte of the data need to read
 * @param[in]  CSPin - the CS pin specifying the slave device
 * @return     none
 */
void spi_read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin)
{
    int i = 0;
    unsigned char temp = 0;
    gpio_write(CSPin,0); //CS level is low
    reg_spi_ctrl &= ~FLD_SPI_DATA_OUT_DIS; ////0x09- bit2 enables spi data output

    /***write cmd***/
    for (i = 0; i < CmdLen; i++) {
    	reg_spi_data = Cmd[i];
        while(reg_spi_ctrl& FLD_SPI_BUSY );//wait writing finished
    }

    reg_spi_ctrl |=FLD_SPI_DATA_OUT_DIS;

    /***when the read_bit was set 1,you can read 0x800008 to take eight clock cycle***/
    reg_spi_ctrl |= FLD_SPI_RD; //enable read,0x09-bit3 : 0 for read ,1 for write
    temp = reg_spi_data; //first byte isn't useful data,only take 8 clock cycle
    while(reg_spi_ctrl &FLD_SPI_BUSY ); //wait reading finished

    /***read data***/
    for (i = 0; i < DataLen; i++) {
    	if(i==(DataLen-1))
    	{
    	   reg_spi_ctrl &= ~FLD_SPI_RD;//when last byte,disable RD function.
    	}
        Data[i] = reg_spi_data; //take 8 clock cycles
        while(reg_spi_ctrl & FLD_SPI_BUSY ); //wait reading finished
    }
    //pull up CS
    gpio_write(CSPin,1);//CS level is high
}

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module
 *            Telink spi supports four standard working modes
 *            register  0x0b set working mode
 *            bit0:CPOL-Clock Polarity  ; bit1:CPHA-Clock Phase
 *            MODE0: CPOL = 0 , CPHA =0;
 *            MODE1: CPOL = 0 , CPHA =1;
 *            MODE2: CPOL = 1 , CPHA =0;
 *            MODE3: CPOL = 1 , CPHA =1;
 * @return    none
 */
void spi_slave_init(unsigned char DivClock, SPI_ModeTypeDef Mode)
{
	reg_clk_en0 |= FLD_CLK0_SPI_EN;//enable spi clock
	reg_spi_sp = 0;            //clear register(0x0a) value
	reg_spi_sp|= DivClock;     //0x0a: bit0~bit6 set spi clock ; spi clock=system clock/((DivClock+1)*2)
	reg_spi_sp|=FLD_SPI_ENABLE;//0x0a: bit7 enables spi function mode
	reg_spi_ctrl&= (~FLD_SPI_MASTER_MODE_EN ); //disable master mode
	reg_spi_inv_clk = ((reg_spi_inv_clk & (~FLD_SPI_MODE_WORK_MODE)) | Mode);// select SPI mode,surpport four modes
}
/**
 * @brief     This function selects a pin port for the SPI interface (slave mode)
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */
void spi_slave_gpio_set(SPI_GPIO_SclkTypeDef sclk_pin,SPI_GPIO_CsTypeDef cs_pin,SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin)
{
	if(sclk_pin == SPI_GPIO_SCL_A5 ||sclk_pin == SPI_GPIO_SCL_A4 ||sclk_pin == SPI_GPIO_SCL_D0)
	{
		switch(sclk_pin)
		{
			case SPI_GPIO_SCL_A5 :  gpio_set_mux(SPI_GPIO_SCL_A5 ,PA5_SPI_CK); break;
			case SPI_GPIO_SCL_A4 :  gpio_set_mux(SPI_GPIO_SCL_A4 ,PA4_SPI_CK); break;
			case SPI_GPIO_SCL_D0 :  gpio_set_mux(SPI_GPIO_SCL_D0 ,PD0_SPI_CK); break;
		}
	}
	if(cs_pin == SPI_GPIO_CS_A1 ||cs_pin == SPI_GPIO_CS_B7 ||cs_pin == SPI_GPIO_CS_D1)
	{
		switch(cs_pin)
		{
			case SPI_GPIO_CS_A1 :  gpio_set_mux(SPI_GPIO_CS_A1 ,PA1_SPI_CN); break;
			case SPI_GPIO_CS_B7 :  gpio_set_mux(SPI_GPIO_CS_B7 ,PB7_SPI_CN); break;
			case SPI_GPIO_CS_D1 :  gpio_set_mux(SPI_GPIO_CS_D1 ,PD1_SPI_CN); break;
		}
	}
	if(sdo_pin == SPI_GPIO_SDO_A2 ||sdo_pin == SPI_GPIO_SDO_B5 ||sdo_pin == SPI_GPIO_SDO_B0 ||sdo_pin == SPI_GPIO_SDO_B1 ||sdo_pin == SPI_GPIO_SDO_D3)
	{
		switch(sdo_pin)
		{
			case SPI_GPIO_SDO_A2 :  gpio_set_mux(SPI_GPIO_SDO_A2 ,PA2_SPI_DO); break;
			case SPI_GPIO_SDO_B5 :  gpio_set_mux(SPI_GPIO_SDO_B5 ,PB5_SPI_DO); break;
			case SPI_GPIO_SDO_B0 :  gpio_set_mux(SPI_GPIO_SDO_B0 ,PB0_SPI_DO); break;
			case SPI_GPIO_SDO_B1 :  gpio_set_mux(SPI_GPIO_SDO_B1 ,PB1_SPI_DO); break;
			case SPI_GPIO_SDO_D3 :  gpio_set_mux(SPI_GPIO_SDO_D3 ,PD3_SPI_DO); break;
		}
	}
	if(sdi_pin == SPI_GPIO_SDI_A0 ||sdi_pin == SPI_GPIO_SDI_A6 ||sdi_pin == SPI_GPIO_SDI_B6 ||sdi_pin == SPI_GPIO_SDI_D4)
	{
		switch(sdi_pin)
		{
			case SPI_GPIO_SDI_A0 :  gpio_set_mux(SPI_GPIO_SDI_A0 ,PA0_SPI_DI); break;
			case SPI_GPIO_SDI_A6 :  gpio_set_mux(SPI_GPIO_SDI_A6 ,PA6_SPI_DI); break;
			case SPI_GPIO_SDI_B6 :  gpio_set_mux(SPI_GPIO_SDI_B6 ,PB6_SPI_DI); break;
			case SPI_GPIO_SDI_D4 :  gpio_set_mux(SPI_GPIO_SDI_D4 ,PD4_SPI_DI); break;
		}
	}
	BM_CLR(reg_gpio_func(sclk_pin), sclk_pin & 0xff);//disable gpio function
	BM_CLR(reg_gpio_func(cs_pin), cs_pin & 0xff);
	BM_CLR(reg_gpio_func(sdo_pin), sdo_pin & 0xff);
	BM_CLR(reg_gpio_func(sdi_pin), sdi_pin & 0xff);
	gpio_set_input_en(sclk_pin,1);//enable input function
	gpio_set_input_en(cs_pin,1);
	gpio_set_input_en(sdi_pin,1);
}

/**
 * @brief 		This function use to set the share mode of SPI.
 * @param[in] 	none
 * @return 		none
 */
void spi_set_share_mode_en(void)
{
	reg_spi_ctrl |= FLD_SPI_SHARE_MODE; //0x09:bit[5] enable share mode
}

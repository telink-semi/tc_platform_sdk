/********************************************************************************************************
 * @file	spi.h
 *
 * @brief	This is the header file for B89
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
#pragma once
#ifndef SPI_H
#define SPI_H

#include "bsp.h"
#include "gpio.h"
#include "compiler.h"
/**
 *  @brief  Define pin for SPI interface
 *  		SDO SDI SCK CN
 *  		A2  A0  A5  A1
 *  		B5  A6  A4  B7
 *  		B0	B6	D0	D1
 *  		B1	D4
 *  		D3
 */

typedef enum {
	SPI_GPIO_SCL_A5=GPIO_PA5,
	SPI_GPIO_SCL_A4=GPIO_PA4,
	SPI_GPIO_SCL_D0=GPIO_PD0,
}SPI_GPIO_SclkTypeDef;

typedef enum {
	SPI_GPIO_SDO_A2=GPIO_PA2,
	SPI_GPIO_SDO_B5=GPIO_PB5,
	SPI_GPIO_SDO_B0=GPIO_PB0,
	SPI_GPIO_SDO_B1=GPIO_PB1,
	SPI_GPIO_SDO_D3=GPIO_PD3,
}SPI_GPIO_SdoTypeDef;

typedef enum {
	SPI_GPIO_SDI_A0=GPIO_PA0,
	SPI_GPIO_SDI_A6=GPIO_PA6,
	SPI_GPIO_SDI_B6=GPIO_PB6,
	SPI_GPIO_SDI_D4=GPIO_PD4,
}SPI_GPIO_SdiTypeDef;

typedef enum {
	SPI_GPIO_CS_A1=GPIO_PA1,
	SPI_GPIO_CS_B7=GPIO_PB7,
	SPI_GPIO_CS_D1=GPIO_PD1,
}SPI_GPIO_CsTypeDef;


/**
 *  @brief  Define the mode for SPI interface
 */
typedef enum {
    SPI_MODE0 = 0,
    SPI_MODE2,
    SPI_MODE1,
    SPI_MODE3,
} SPI_ModeTypeDef;

/**
 *  @brief  Define the clk for SPI interface(system clock = 24M)
 */
typedef enum {
    SPI_CLK_200K = 0x3c,
    SPI_CLK_250K = 0x2e,
    SPI_CLK_500K = 0x17,
    SPI_CLK_1M   = 0x0b,
    SPI_CLK_2M   = 0x05,
    SPI_CLK_4M   = 0x02,
} SPI_24M_ClkTypeDef;


/**
 * @brief This function reset SPI module.
 * @param[in] none
 * @return none
 */


static inline void reset_spi_module(void)
{
	reg_rst0 |= FLD_RST0_SPI;
	reg_rst0 &= (~FLD_RST0_SPI);
}
/**
 * @brief     This function selects a pin port for the SPI interface (master mode)
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */


extern void spi_master_gpio_set(SPI_GPIO_SclkTypeDef sclk_pin,SPI_GPIO_CsTypeDef cs_pin,SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin);

/**
 * @brief     This function selects a pin port for the SPI interface (slave mode)
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */



extern void spi_slave_gpio_set(SPI_GPIO_SclkTypeDef sclk_pin,SPI_GPIO_CsTypeDef cs_pin,SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin);


/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */



extern void spi_masterCSpin_select(GPIO_PinTypeDef CSPin);

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
extern void spi_master_init(unsigned char DivClock, SPI_ModeTypeDef Mode);
/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module (slave mode)
 * @return    none
 */
extern void spi_slave_init(unsigned char DivClock, SPI_ModeTypeDef Mode);
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


extern void spi_write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin);

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


extern void spi_read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin);

/**
 * @brief 		This function use to set the share mode of SPI.
 * @param[in] 	none
 * @return 		none
 */
extern void spi_set_share_mode_en(void);

#endif


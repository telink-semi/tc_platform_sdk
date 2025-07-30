/********************************************************************************************************
 * @file    spi.h
 *
 * @brief   This is the header file for TC321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
    SPI_CLK_200K = 0x3b,
    SPI_CLK_250K = 0x2f,
    SPI_CLK_500K = 0x17,
    SPI_CLK_1M   = 0x0b,
    SPI_CLK_2M   = 0x05,
    SPI_CLK_4M   = 0x02,
} SPI_24M_ClkTypeDef;

/**
 * @brief  Define the SPI pin definition structure.
 */
typedef struct spi_pin_config_t
{
    GPIO_PinTypeDef      spi_clk_pin;
    GPIO_PinTypeDef      spi_csn_pin;
    GPIO_PinTypeDef      spi_mosi_io0_pin;
    GPIO_PinTypeDef      spi_miso_io1_pin;
}spi_pin_config_t;

/**
 * @brief This function reset SPI module.
 * @param[in] none
 * @return none
 */
static inline void reset_spi_module(void)
{
    reg_rst0 &= (~FLD_RST0_SPI);
    reg_rst0 |= FLD_RST0_SPI;
}

/**
 * @brief      This function selects  pin for spi master or slave mode.
 * @param[in]  pin  - the selected pin.
 * @return     none
 */
void spi_set_pin_mux(GPIO_PinTypeDef pin,gpio_func_e function);

/**
 * @brief       This function servers configures spi pin.
 * @param[in]   config - the pointer of pin config struct.
 * @return      none
 * @attention   The connections for pin are as follows
 *               master       slave
 *               csn     <->   csn
 *               clk     <->   clk
 *               mosi    <->   miso
 *               miso    <->   mosi
 */
void spi_set_pin(spi_pin_config_t *spi_pin_config);

/**
 * @brief     This function selects a pin port for the SPI interface (master mode)
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */
void spi_master_gpio_set(GPIO_PinTypeDef sclk_pin,GPIO_PinTypeDef cs_pin,GPIO_PinTypeDef sdo_pin, GPIO_PinTypeDef sdi_pin );

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
void spi_masterCSpin_select(GPIO_PinTypeDef CSPin);

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
void spi_master_init(unsigned char DivClock, SPI_ModeTypeDef Mode);

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module (slave mode)
 * @return    none
 */
void spi_slave_init(unsigned char DivClock, SPI_ModeTypeDef Mode);

/**
 * @brief      This function is used to write large amounts of data to the SPI slave device on the hardware CS pin.
 * @param[in]  Cmd - pointer to the command bytes needed written into the
 *             slave device first before the writing operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes
 * @param[in]  Data - pointer to the data need to write
 * @param[in]  DataLen - length in byte of the data need to write
 * @return     none
 */
void spi_write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen);

/**
 * @brief      This function is used for SPI on the hardware CS pin to read large amounts of data from the device.
 * @param[in]  Cmd - pointer to the command bytes needed written into the
 *             slave device first before the reading operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes
 * @param[out] Data - pointer to the buffer that will cache the reading out data
 * @param[in]  DataLen - length in byte of the data need to read
 * @return     none
 */
void spi_read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen);

/**
 * @brief       This function use to set the share mode of SPI.
 * @param[in]   none
 * @return      none
 */
void spi_set_share_mode_en(void);

#endif

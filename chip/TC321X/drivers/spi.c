/********************************************************************************************************
 * @file    spi.c
 *
 * @brief   This is the source file for TC321X
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
#include "spi.h"
#include "register.h"
#include "gpio.h"

/**
 * @brief      This function selects  pin for spi master or slave mode.
 * @param[in]  pin  - the selected pin.
 * @return     none
 */
void spi_set_pin_mux(GPIO_PinTypeDef pin,gpio_func_e function)
{
    if (pin != GPIO_NONE_PIN)
    {
        gpio_set_input_en(pin,1);
        gpio_set_func( pin, function);
    }
}

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
void spi_set_pin(spi_pin_config_t *spi_pin_config)
{
    spi_set_pin_mux(spi_pin_config->spi_clk_pin,spi_pin_config->spi_clk_pin==GPIO_PB1 ? SPI_PB1_CK_IO: SPI_CK_IO);
    spi_set_pin_mux(spi_pin_config->spi_csn_pin,spi_pin_config->spi_csn_pin==GPIO_PB0 ? SPI_PB0_CN_IO: SPI_CN_IO);
    spi_set_pin_mux(spi_pin_config->spi_mosi_io0_pin,spi_pin_config->spi_mosi_io0_pin==GPIO_PE0 ? SPI_PE0_MOSI_IO: SPI_MOSI_IO);
    spi_set_pin_mux(spi_pin_config->spi_miso_io1_pin,spi_pin_config->spi_miso_io1_pin==GPIO_PE1 ? SPI_PE1_MIS0_IO:SPI_MISO_IO);
}

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
void spi_masterCSpin_select(GPIO_PinTypeDef CSPin)
{
    if(CSPin != GPIO_NONE_PIN)
    {
        gpio_set_func(CSPin, AS_GPIO); // enable GPIO function
        gpio_set_input_en(CSPin, 0);   // disable input function
        gpio_set_output_en(CSPin, 1);  // enable out put
        gpio_write(CSPin, 1);          // output high level in idle state
    }

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
    reg_spi_sp = 0;                         // clear register(0x0a) value
    reg_spi_sp |= DivClock;                 // 0x0a: bit0~bit6 set spi clock; spi clock=system clock/((DivClock+1)*2)
    reg_spi_sp |= FLD_SPI_ENABLE;           // 0x0a: bit7 enables spi function mode
    reg_spi_ctrl |= FLD_SPI_MASTER_MODE_EN; // 0x09: bit1 enables master mode
    reg_spi_inv_clk = ((reg_spi_inv_clk & (~FLD_SPI_MODE_WORK_MODE)) | Mode); // select SPI mode, support four modes
}

/**
 * @brief      This function is used to write large amounts of data to the SPI slave device on the hardware CS pin.
 * @param[in]  Cmd - pointer to the command bytes needed written into the
 *             slave device first before the writing operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes
 * @param[in]  Data - pointer to the data need to write
 * @param[in]  DataLen - length in byte of the data need to write
 * @return     none
 */
void spi_write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen)
{
    int i = 0;

    reg_spi_ctrl &=~(FLD_SPI_CS);           // Hardware CS level is low
    reg_spi_ctrl &= ~FLD_SPI_DATA_OUT_DIS;  // 0x09-bit2 enables spi data output
    reg_spi_ctrl &= ~FLD_SPI_RD;            // enable write, 0x09-bit3 : 0 for read, 1 for write

    /* write Cmd */
    for (i = 0; i < CmdLen; i++) {
        reg_spi_data = Cmd[i];
        while(reg_spi_ctrl & FLD_SPI_BUSY); // wait writing finished
    }
    /* write Data */
    for (i = 0; i < DataLen; i++) {
        reg_spi_data = Data[i];
        while(reg_spi_ctrl & FLD_SPI_BUSY); // wait writing finished
    }
    reg_spi_ctrl |=FLD_SPI_CS;              // Hardware CS level is high

}

/**
 * @brief      This function is used for SPI on the hardware CS pin to read large amounts of data from the device.
 * @param[in]  Cmd - pointer to the command bytes needed written into the
 *             slave device first before the reading operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes
 * @param[out] Data - pointer to the buffer that will cache the reading out data
 * @param[in]  DataLen - length in byte of the data need to read
 * @return     none
 */
void spi_read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen)
{
    int i = 0;
    unsigned char temp = 0;

    reg_spi_ctrl &=~(FLD_SPI_CS);           // Hardware CS level is low
    reg_spi_ctrl &= ~FLD_SPI_DATA_OUT_DIS;  // 0x09-bit2 enables spi data output

    /* write cmd */
    for (i = 0; i < CmdLen; i++) {
        reg_spi_data = Cmd[i];
        while(reg_spi_ctrl & FLD_SPI_BUSY); // wait writing finished
    }

    reg_spi_ctrl |= FLD_SPI_DATA_OUT_DIS;

    /* when the read_bit was set 1,you can read 0x800008 to take eight clock cycle */
    reg_spi_ctrl |= FLD_SPI_RD;             // enable read, 0x09-bit3 : 0 for read,1 for write
    temp = reg_spi_data;                    // first byte isn't useful data, only take 8 clock cycle
    while(reg_spi_ctrl & FLD_SPI_BUSY);     // wait reading finished

    /* read data */
    for (i = 0; i < DataLen; i++) {
        if(i == (DataLen-1))
        {
           reg_spi_ctrl &= ~FLD_SPI_RD;     // when last byte,disable RD function.
        }
        Data[i] = reg_spi_data;             // take 8 clock cycles
        while(reg_spi_ctrl & FLD_SPI_BUSY); // wait reading finished
    }
    reg_spi_ctrl |=FLD_SPI_CS;          // Hardware CS level is high
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
    reg_spi_sp = 0;                            // clear register(0x0a) value
    reg_spi_sp |= DivClock;                    // 0x22:bit[0]~bit[6] set spi clock; spi clock=system clock/((DivClock+1)*2)
    reg_spi_sp |= FLD_SPI_ENABLE;              // 0x22:bit[7] enables spi function mode
    reg_spi_ctrl &= (~FLD_SPI_MASTER_MODE_EN); // disable master mode
    reg_spi_inv_clk = ((reg_spi_inv_clk & (~FLD_SPI_MODE_WORK_MODE)) | Mode); // select SPI mode, support four modes
}

/**
 * @brief       This function use to set the share mode of SPI.
 * @param[in]   none
 * @return      none
 */
void spi_set_share_mode_en(void)
{
    reg_spi_ctrl |= FLD_SPI_SHARE_MODE; // 0x21:bit[5] enable share mode
}

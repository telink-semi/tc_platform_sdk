/********************************************************************************************************
 * @file    zb_flash_ctrl.c
 *
 * @brief   This is the source file for B87
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
#include "../flash_internal_private_func/zb_flash_ctrl.h"

static unsigned char cmd_list[5] = {0x00,0x00,0x00,0x00,0x00};
static unsigned int zb_cam_modify_support_type[] = {0x11325E, 0x12325E, 0x13325E, 0x14325E};
static const unsigned int zb_cam_modify_support_cnt = sizeof(zb_cam_modify_support_type)/sizeof(*zb_cam_modify_support_type);

/**
 * @brief		This function serves to send zb flash cam command.
 * @param[in]	pwdata	- the command.
 * @return		wsize   - the command byte size.
 */
_attribute_ram_code_sec_ static void zb_flash_send_cmd(unsigned char *pwdata, unsigned char wsize)
{
	unsigned char i = 0;
	for(i = 0;i<wsize;i++)
	{
		mspi_write(pwdata[i]);
		mspi_wait();
	}
}

/**
 * @brief		This function serves to set zb flash enter testmode.
 * @param[in]	none.
 */
_attribute_ram_code_sec_ static void zb_flash_enter_testmode(void)
{
	cmd_list[0] = 0x36;
	cmd_list[1] = 0xd7;
	cmd_list[2] = 0xa0;

	mspi_high();
	sleep_us(10);
	mspi_low();

	zb_flash_send_cmd(cmd_list, 1);
	mspi_high();

	sleep_us(2);
	mspi_low();
	zb_flash_send_cmd(&cmd_list[1], 1);
	mspi_high();

	sleep_us(2);
	mspi_low();
	zb_flash_send_cmd(&cmd_list[2], 1);
	mspi_high();
}

/**
 * @brief		This function serves to read zb flash cam value.
 * @param[in]	addr- the cam value address.
 * @rutern		the cam value,it is range from 0 to 9.
 */
_attribute_ram_code_sec_noinline_ static unsigned char zb_flash_read_cam(unsigned char addr)
{
	unsigned char temp = 0;
	cmd_list[0] = 0x0f;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x03;
	cmd_list[4] = 0x40;

	mspi_high();
	sleep_us(2);
	mspi_low();
	zb_flash_send_cmd(cmd_list, 5);
	mspi_high();

	sleep_us(2);
	cmd_list[0] = 0x0f;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x01;
	cmd_list[4] = 0x40;
	mspi_low();
	zb_flash_send_cmd(cmd_list, 5);
	mspi_high();

	sleep_us(2);
	cmd_list[0] = 0x0b;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = addr;
	mspi_low();
	zb_flash_send_cmd(cmd_list, 4);

	mspi_read();
	temp = mspi_read();
	mspi_high();

	return temp;
}

/**
 * @brief		This function serves to wait zb flash cam value write/erase status ready.
 * @param[in]	none.
 */
_attribute_ram_code_sec_ static void zb_flash_polling_to_ready(void)
{
	unsigned char status = 0x03;
	mspi_high();
	sleep_us(2);
	while(status & 0x03)
	{
		cmd_list[0] = 0x05;
		mspi_low();
		zb_flash_send_cmd(cmd_list, 1);

		status = mspi_read();
		mspi_high();
		sleep_us(10);
	}
}

/**
 * @brief		This function serves to write zb flash cam value.
 * @param[in]	pwdata	- the cam data which want to write.
 * @return		wsize   - the cam data byte size,it is range from 0 to 9.
 */
_attribute_ram_code_sec_noinline_ static void zb_flash_write_cam(unsigned char *pwdata, unsigned char wsize)
{
	unsigned char i;

	cmd_list[0] = 0x0f;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x03;
	cmd_list[4] = 0x40;

	mspi_high();
	sleep_us(2);
	mspi_low();
	zb_flash_send_cmd(cmd_list, 5);
	mspi_high();
	sleep_us(2);

	for(i=0;i<wsize;i++)
	{
		cmd_list[0] = 0x06;
		mspi_low();
		zb_flash_send_cmd(cmd_list, 1);
		mspi_high();
		sleep_us(2);

		cmd_list[0] = 0x02;
		cmd_list[1] = 0x00;
		cmd_list[2] = 0x00;
		cmd_list[3] = i;
		cmd_list[4] = pwdata[i];
		mspi_low();
		zb_flash_send_cmd(cmd_list, 5);
		mspi_high();

		zb_flash_polling_to_ready();
	}
}

/**
 * @brief		This function serves to exit zb flash tstmode.
 * @param[in]	none.
 */
_attribute_ram_code_sec_ static void zb_flash_exit_testmode(void)
{
	cmd_list[0] = 0x0f;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x03;
	cmd_list[4] = 0x00;

	mspi_high();
	sleep_us(2);
	mspi_low();
	zb_flash_send_cmd(cmd_list, 5);
	mspi_high();

	sleep_us(2);
	cmd_list[0] = 0x0f;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x01;
	cmd_list[4] = 0x00;
	mspi_low();
	zb_flash_send_cmd(cmd_list, 5);
	mspi_high();

	sleep_us(2);
	cmd_list[0] = 0x18;
	mspi_low();
	zb_flash_send_cmd(cmd_list, 1);
	mspi_high();
}

/**
 * @brief		This function serves to erase zb flash cam value.
 * @param[in]	none.
 */
_attribute_ram_code_sec_noinline_ static void zb_flash_erase_cam(void)
{
	cmd_list[0] = 0x0f;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x03;
	cmd_list[4] = 0x40;

	mspi_high();
	sleep_us(2);
	mspi_low();
	zb_flash_send_cmd(cmd_list, 5);
	mspi_high();
	sleep_us(2);
	cmd_list[0] = 0x06;
	mspi_low();
	zb_flash_send_cmd(cmd_list, 1);
	mspi_high();
	sleep_us(2);

	cmd_list[0] = 0x52;
	cmd_list[1] = 0x00;
	cmd_list[2] = 0x00;
	cmd_list[3] = 0x00;
	mspi_low();
	zb_flash_send_cmd(cmd_list, 4);
	mspi_high();

	zb_flash_polling_to_ready();
}

/**
 * @brief		This function serves to modify zb flash reset voltage(cam[6]).cam[6] default value is 0x10,when flash voltage is less than 0.5V,
 * 				the flash will reset.but in some application solution,the voltage is not decrease less than 0.5 and increase soon,in this case the
 * 				flash will power on fail,need to change the reset voltage to 1.0V(change the cam[6] to 0x00),but the flash standby current will increase
 * 				about 1uA.
 * @param[in]	cam_6  - the cam[6] value which you want to modify.
 * @return		0 - it is zb flash and modify success;
 * 				1 - it is zb flash and modify fail;
 * 				2 - it is not zb flash or it is zb flash but can not use this api to modify cam value;
 * 				0xff - flash is error, the cam value is change to 0x00.
 * @note		The irq will disable to avoid interrupt in the process of this api.
 * 				This process need keep the chip power on, if power down in this process will cause flash error
 * 				and can not use it in low voltage status.
 * 				The best usage is:call this api to change cam value after power on and system init,if change success,write
 * 				a flag to flash to ensure the api only call once.
 *@attention	The return value equal 0 just represent that this configuration is modify success,you need to power down the flash power and power on again
 *				to make it take effect.
 */
_attribute_ram_code_sec_noinline_ unsigned char zb_cam_modify(zb_flash_cam6_e cam_6)
{
	unsigned char rx_cam[10] = {0};

	unsigned int flash_mid = flash_read_mid();

	unsigned char cnt = 0;
	for(cnt = 0; cnt < zb_cam_modify_support_cnt; cnt++)
	{
		if(zb_cam_modify_support_type[cnt] == flash_mid)
		{
			break;
		}
	}
	if(zb_cam_modify_support_cnt == cnt)
	{
		return 0x02;
	}

	unsigned char r = irq_disable();
	/* Step 1: Enter test mode */
	zb_flash_enter_testmode();

	for(unsigned char loop=0; loop<4; loop++)
	{
		/* Step 2: Read CAM settings, 10 bytes */
		for(cnt=0; cnt<10;cnt++)
		{
			rx_cam[cnt] = zb_flash_read_cam(cnt);
		}

		/* check weather the flash is error */
		for(cnt=0; cnt<10;cnt++)
		{
			if(0 != rx_cam[cnt])
			{
				break;
			}
		}
		if(10 == cnt)
		{
			// the cam value is all 0x00, the flash is error.
			/* Step 6: Exit test mode*/
			zb_flash_exit_testmode();
			return 0xff;
		}

		if(cam_6 != rx_cam[6])
		{
			/* the cam value is not all 0x0, and the cam[6] is not the value which you want to change. */
			/* Step 3: Erase CAM settings*/
			zb_flash_erase_cam();
			/* Step 4: Write CAM settings, 10 bytes*/
			rx_cam[6] = cam_6;
			zb_flash_write_cam(rx_cam, 10);
		}
		else{
			/* the cam value is not all 0x0, and the cam[6] is already the value which you want to change. */
			break;
		}
	}

	/* Step 5: Verify CAM settings after modifying, 10 bytes*/
	for(cnt=0; cnt<10;cnt++)
	{
		rx_cam[cnt] = zb_flash_read_cam(cnt);
	}

	/* Step 6: Exit test mode*/
	zb_flash_exit_testmode();

	irq_restore(r);

	/* check weather the flash is error */
	for(cnt=0; cnt<10;cnt++)
	{
		if(0 != rx_cam[cnt])
		{
			break;
		}
	}
	if(10 == cnt)
	{
		// the cam value is all 0x00, the flash is error.
		/* Step 6: Exit test mode*/
		return 0xff;
	}

	if(cam_6 == rx_cam[6])
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief		This function serves to check the zb flash cam value.
 * @param[in]	cam6_check_en - whether to check the cam6, 1 - enable, 0 - disable.
 * @param[in]	cam_6 - the cam6 value, if you enable cam6_check, this value will compare with the cam6 that read from flash.
 * @return		0 - it is zb flash and cam value is the setting value;
 * 				1 - it is zb flash and cam value is error;
 * 				2 - it is not zb flash or it is zb flash but can not use this api to modify cam value;
 */
_attribute_ram_code_sec_noinline_ unsigned char zb_cam_check(unsigned char cam6_check_en, zb_flash_cam6_e cam_6)
{

	unsigned char rx_cam[10] = {0};
	unsigned char rx_cam_check[2][10] = {{0x10, 0xf0, 0x90, 0x30, 0xb0, 0x00, 0x00, 0xb0, 0x00, 0x00},
										{0x10, 0xf0, 0xB0, 0x30, 0xb0, 0x30, 0x00, 0xb0, 0x00, 0x00}};

	rx_cam_check[0][6] = cam_6;
	rx_cam_check[1][6] = cam_6;

	unsigned int flash_mid = flash_read_mid();

	unsigned char cnt = 0;
	for(cnt = 0; cnt < zb_cam_modify_support_cnt; cnt++)
	{
		if(zb_cam_modify_support_type[cnt] == flash_mid)
		{
			break;
		}
	}
	if(zb_cam_modify_support_cnt == cnt)
	{
		return 0x02;
	}

	unsigned char r = irq_disable();
	/* Step 1: Enter test mode */
	zb_flash_enter_testmode();
	
	/* Step 2: Read CAM settings, 10 bytes */
	for(cnt=0; cnt<10;cnt++)
	{
		rx_cam[cnt] = zb_flash_read_cam(cnt);
	}
	
	/* Step 3: Exit test mode*/
	zb_flash_exit_testmode();
				
	/* check weather the cam is error */
	for(cnt=0; cnt<sizeof(rx_cam_check)/sizeof(rx_cam_check[0]);cnt++)
	{
		for(unsigned char index=0; index<10;index++)
		{
			//cam[7] is not a fixed value, so we can not to check it
			//cam[6] is vccok2 setting, you can choices whether to check it
			if((7 != index) && (cam6_check_en?(1):(6 != index)))
			{
				if(rx_cam_check[cnt][index] != rx_cam[index])
				{
					if(sizeof(rx_cam_check)/sizeof(rx_cam_check[0])-1 > cnt)
					{
						break;
					}
					else
					{
						return 0x01;
					}
				}
				else if(9 == index)
				{
					return 0;
				}
			}
		}
	}

	irq_restore(r);
	return 0;
}

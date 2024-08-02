/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for B85m
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

#define DUTCMD_FIRMWARE_ENCRYPT    0x68
#define    TL_TEST_RESP_OK         1
#define    TL_TEST_RESP_ERR        0

typedef struct{
	unsigned char cmd:7;  //bit0~bit6
	unsigned char state:1;//bit7
}cmd_state_t;

typedef struct{
	unsigned char resp[3];
	struct{
	unsigned char cmd:7;  //bit0~bit6
	unsigned char state:1;//bit7
	}cmd_state_t;
	unsigned long param[2];
}__attribute__((packed))tl_test_cmd_pkt_t_another;

struct DUT_CMD_FUNC {
	int	cmd_name;//char
	int	(*func)(void);
};
/* The following demo is about firmware encryption program.If you need to implement other functions, you can refer to this program structure.
   script instruction: dutcmd(cmd,para0,para1,time_out)
   An example of firmware encryption test is as follows.The meaning of the test script is to write the encrypted data to the flash 0 address.
   fast_load(DUT_Demo.bin,1,24)
   dutcmd(0x68,0,0,1024)##test_name:FWEncrypt
   Note:
   Users can get para0 and para1 by "g_test_cmd_ptr->param[0]" and "g_test_cmd_ptr->param[1]" int the program.
   If the test item fails,  the program returns "TL_TEST_RESP_ERR".
   If the test item passes, the program returns "TL_TEST_RESP_OK".
*/
int flash_firmware_protect();
volatile tl_test_cmd_pkt_t_another *g_test_cmd_ptr = (tl_test_cmd_pkt_t_another *)0x840004;

struct DUT_CMD_FUNC dut_cmd_func[] = {
	   {DUTCMD_FIRMWARE_ENCRYPT,	flash_firmware_protect},
};
void firmware_encrypt_based_on_uid(unsigned char* uid,unsigned char* ciphertext)
{
	//Users can change the secret key according to their own needs.
	unsigned char key[16] ={0x00,0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,0x99, 0xaa, 0xbb, 0xcc,0xdd, 0xee, 0xff};
	aes_encrypt(key,uid,ciphertext);
}
int flash_firmware_protect(void)
{
    /********************************************************************************************************
		                        parameter optional base on dutcmd
	*******************************************************************************************************/
	unsigned int address = g_test_cmd_ptr->param[0];
	unsigned int flash_mid,i;
	unsigned char flash_uid[16],rbuf[16];
	int flag = flash_read_mid_uid_with_check(&flash_mid,flash_uid);
	g_test_cmd_ptr->param[0] = flash_mid;
	if(flag==TL_TEST_RESP_ERR){
		return TL_TEST_RESP_ERR;
	}
	/********************************************************************************************************
		     Use UID and Key as input for AES encryption, customers can modify the algorithm by themselves
		     Algorithms can be more complex, such as multiple AES encryption, or adding some XOR operations, etc.
	*******************************************************************************************************/
	unsigned char ciphertext[16] = {0};
	firmware_encrypt_based_on_uid(flash_uid,ciphertext);

	/********************************************************************************************************
		          	  	  Burn the ciphertext and judge whether the burning is successful
	*******************************************************************************************************/
	flash_write_page(address,16,ciphertext);
	flash_read_page(address,16,rbuf);
	for(i = 0;i < 16;i++){
		if(rbuf[i] != ciphertext[i]){
			return TL_TEST_RESP_ERR;
		}
	}
	return  TL_TEST_RESP_OK;

}

void user_init(void)
{

}


void main_loop (void)
{
	unsigned int i;
	unsigned char rr;
	while(1)
	{
		if(g_test_cmd_ptr->cmd_state_t.state == 0x01)
		{
			for (i=0; i<sizeof (dut_cmd_func)/sizeof (struct DUT_CMD_FUNC); i++)
			{
				if (dut_cmd_func[i].cmd_name == (unsigned char)(g_test_cmd_ptr->cmd_state_t.cmd))
				{
					g_test_cmd_ptr->resp[0] = 0;
					g_test_cmd_ptr->resp[1] = 0;
					g_test_cmd_ptr->resp[2] = 0;
					rr = dut_cmd_func[i].func();
					if(rr == TL_TEST_RESP_OK)
						g_test_cmd_ptr->resp[2] = (unsigned char)g_test_cmd_ptr->cmd_state_t.cmd;
					else
						g_test_cmd_ptr->resp[2] = ~((unsigned char)g_test_cmd_ptr->cmd_state_t.cmd);
					break;
				}
			}
			g_test_cmd_ptr->cmd_state_t.state = 0;
		}
	}

}




/********************************************************************************************************
 * @file    printf.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver Group
 * @date    2018
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
#include "register.h"
#include "printf.h"
#include "usbhw.h"
#include "timer.h"



typedef char* VA_LIST;
#define VA_START(list, param) (list = (VA_LIST)((int)&param + sizeof(param)))
#define VA_ARG(list, type) ((type *)(list += sizeof(type)))[-1]
#define VA_END(list) (list = (VA_LIST)0)

#ifndef   NULL
#define   NULL				0
#endif

#if(DEBUG_MODE==1)
#if (DEBUG_BUS==DEBUG_IO)

#ifndef		BIT_INTERVAL
#define		BIT_INTERVAL	(sys_tick_per_us*1000*1000/PRINT_BAUD_RATE)
#endif

/**
 * @brief      This function serves to format string.
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */
_attribute_ram_code_sec_noinline_  void io_putchar(unsigned char byte){
	unsigned char j = 0;
	unsigned int t1 = 0,t2 = 0;
	static unsigned char init_flag = 1;
	if(init_flag==1)
	{
		TX_PIN_GPIO_EN();
		TX_PIN_PULLUP_1M();
		TX_PIN_OUTPUT_EN();//Enable output
		gpio_write(DEBUG_INFO_TX_PIN ,1);// Add this code to fix the problem that the first byte will be error.
		init_flag = 0;
	}

	unsigned char tmp_bit0 = read_reg8(TX_PIN_OUTPUT_REG) & (~(DEBUG_INFO_TX_PIN & 0xff));
	unsigned char tmp_bit1 = read_reg8(TX_PIN_OUTPUT_REG) | (DEBUG_INFO_TX_PIN & 0xff);
	unsigned char bit[10] = {0};

	bit[0] = tmp_bit0;
	bit[1] = (byte & 0x01)? tmp_bit1 : tmp_bit0;
	bit[2] = ((byte>>1) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[3] = ((byte>>2) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[4] = ((byte>>3) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[5] = ((byte>>4) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[6] = ((byte>>5) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[7] = ((byte>>6) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[8] = ((byte>>7) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[9] = tmp_bit1;

	t1 = read_reg32(0x740);
	for(j = 0;j<10;j++)
	{
		t2 = t1;
		while(t1 - t2 < BIT_INTERVAL){
			t1  = read_reg32(0x740);
		}
		write_reg8(TX_PIN_OUTPUT_REG,bit[j]);        //send bit0
	}
}

#elif(DEBUG_BUS==DEBUG_USB)
#define   FIFOTHRESHOLD  	4
#define   BLOCK_MODE   		1
/**
 * @brief      This function serves to format string.
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */
void usb_putchar (char c)
{
#if(BLOCK_MODE)
	while (reg_usb_ep8_fifo_mode & BIT(1));
#endif
  reg_usb_ep8_dat = c;
}

#endif
#endif

/**
 * @brief      This function serves to format string.
 * @param[in]  *out -  buffer to output
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */
void tl_putchar(char **out, char c)
{
	if(out)
	{
		**out = c;
		(*out)++;
	}
	else
	{
#if(DEBUG_MODE==1)
#if(DEBUG_BUS == DEBUG_USB)
		usb_putchar(c);
#elif(DEBUG_BUS == DEBUG_IO)
		io_putchar(c);
#endif
#endif
	}
}

/**
 * @brief      This function serves to format string.
 * @param[in]  *out -  buffer to output
 * @param[in]  c  -  a number need to print 
 * @return     none.
 */
void tl_putnum(char **out, unsigned char c) {
	unsigned char nib = c >> 4;
	if (nib > 9)	nib = nib + 87;
	else		nib = nib + 48;
	tl_putchar(out, nib);

	nib = c & 15;
	if (nib > 9)	nib = nib + 87;
	else		nib = nib + 48;
	tl_putchar(out, nib);
}


/**
 * @brief      This function serves to format string.
 * @param[in]  *out -  buffer to output
 * @param[in]  w  -  a integer need to print 
 * @return     none.
 */
void tl_putnumber(char **out, unsigned int w,int len) {
	int i;
	int c = w;

	for(i=len-1;i>=0;i--)
	{
		c = w >>(i*8);
		tl_putnum(out, c);
	}
}

/**
 * @brief      This function serves to format string.
 * @param[in]  *out -  buffer to output
 * @param[in]  w  -  a integer need to print 
 * @return     none.
 */
void tl_putint(char **out, int w)
{
	unsigned char buf[12],tmp,*p;
	int u;
	p = buf + 11;
	*p = '\0';
	if(w < 0)
	{
		u = -w;
	}
	else
	{
		u = w;
	}
	do{ // at least one time..
        tmp = u % 10;
		*--p = tmp + '0';
		u /= 10;
	}while(u);
	if (w < 0)
	{
		*--p = '-';
	}
	while(*p){
        tl_putchar(out, *p);
        p++;
    }

}

/**
 * @brief      This function serves to format string.
 * @param[in]  *out -  buffer to output
 * @param[in]  *str  -  string need to print 
 * @return     none.
 */
void  tl_putstring(char **out, char * str)
{
    char *s;
	s = str;
	if(s == NULL){
        s = "(null)";
    }
	while(*s){
           tl_putchar(out, *s);
           s++;
	}
}

/**
 * @brief      This function serves to get the actual number field width of bytes. In particular, an output with a variable value of 0 is 1.
 * @param[in]  num -  Input variable
 * @return     the actual number field width of bytes, the returned values are 1 to 4.
 */
unsigned char get_field_width(unsigned int num)
{	
	unsigned char ret;
	for(ret = 1; ret <= 32; ret++)
	{
		num >>= 1;
		if(!num) break;
	}
	return (ret+7)>>3;
}

/**
 * @brief      This function serves to format string.
 * @param[in]  *out -  buffer to output
 * @param[in]  *f -  string need to format
 * @param[in]  a  -  string need to print
 * @return     none.
 */
const char *tl_format_msg(char **out, const char *f, int a)
{
	char c;
	int fieldwidth = 0;
	int flag = 0;
	//when the character is not NUL(ASCAII value =0x00)
	while ((c = *f++) != 0)
	{
		if (c >= '0' && c <= '9')
		{
			//number in character is changed to number in real
			fieldwidth = (fieldwidth * 10) + (c - '0');
		}
		else
		{
			switch (c)
			{
				case 'x':
					flag = 16;
					break;
				case 'd':
					flag = 10;
					break;
				case 's':
					flag = 99;
					break;
				default:
					tl_putchar(out, '*');
					flag = -1;
					break;
			}
		}
		if(flag!=0)
		{
			if(fieldwidth==0)  fieldwidth=get_field_width(a);//Obtain get the actual number field width of variable a of type int adaptively.
			break;
		}
	}
	if(flag == 16)
	tl_putnumber(out, a,fieldwidth);
	else if(flag == 10)
	tl_putint(out, a);
	else if(flag==99)
	tl_putstring(out, (char *)a);

	return f;

}

/**
 * @brief      This function serves to print string
 * @param[in]  *out     -  buffer to output
 * @param[in]  *format  -  format string need to print
 * @param[in]  list   	-  variable list
 * @return     none.
 */
void tl_print(char** out, const char *format, VA_LIST list)
{
	const char *pcStr = format;

	while (*pcStr)
	{                       /* this works because args are all ints */
    	if (*pcStr == '%')
        	pcStr = tl_format_msg(out, pcStr + 1, VA_ARG(list, int));
    	else
        	tl_putchar(out, *pcStr++);
	}

	if(out) **out = '\0';
}

#if(DEBUG_MODE==1)

/**
 * @brief      This function serves to print string
 * @param[in]  *format  -  format string need to print
 * @param[in]  ...   	-  variable number of data
 * @return     none.
 */
void tl_printf(const char *format, ...)
{
	VA_LIST list;
	VA_START(list, format);

#if (DEBUG_BUS==DEBUG_USB)
	static int  first_time = 1;
	if(first_time==1)
	{
		reg_usb_ep8_send_thres = FIFOTHRESHOLD;
		first_time = 0;
	}
#elif(DEBUG_BUS==DEBUG_IO)
	static unsigned char re_enter_flag = 0;
	if(re_enter_flag) return;
	re_enter_flag = 1;
#endif

	tl_print(0, format, list);
	VA_END(list);

#if(DEBUG_BUS==DEBUG_IO)
	re_enter_flag = 0;
#endif
}
#endif


/**
 * @brief      This function serves to print string to buffer
 * @param[in]  *buff    -  buffer to print
 * @param[in]  *format  -  format string need to print
 * @param[in]  ...   	-  variable number of data
 * @return     none.
 */
void tl_sprintf(char* buff, const char *format, ...)
{
	VA_LIST list;

	VA_START(list, format);
	tl_print(&buff, format, list);
	VA_END(list);
}




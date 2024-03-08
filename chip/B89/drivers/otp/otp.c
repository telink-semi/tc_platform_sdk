/********************************************************************************************************
 * @file	otp.c
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
#include "otp/otp.h"
#include "timer.h"
#include "irq.h"


extern unsigned char	otp_program_flag;
unsigned char g_pce_timeout=16;
unsigned char g_irq_en=0;



/**
 * @brief     This function servers to waiting for pce timeout.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_pce_timeout_exceed(){
	 g_irq_en = irq_disable();
#if OTP_PCE_AUTO_MODE_EN
#if !ONLY_SUPPORT_OTP_PROGRAM
	 if(otp_program_flag==1)
#endif
	{
	 unsigned long t = clock_time();
	 /*
	 * 1.If the pce auto mode is enabled, there will be a timeout mechanism. when the software manually operates the pce within the timeout time,
	 *   if the pce is manually lowered, timeout will not be invalid, however, if the timeout is not over, pull the pce up manually. when the timeout is over,
	 *   pull the pce down.therefore, in order to prevent the interaction between software and hardware and disrupt the timing sequence,
	 *   it is necessary to wait for the pce hardware to pull down automatically before performing software processing;
	 * 2.when pce_auto_mode is enabled, the pce hardware will automatically pull down, which will be triggered only after mcu fetches the instruction from otp.
	 *   If the previous function (to pull up the pce) moves to the next function (to wait for the pce hardware to pull down automatically),
	 *   the prefetch instruction (cache:4 word) causes no fetch from otp,then it gets stuck, so add timeout_exceed to handle this exception,
	 */
	 while((reg_otp_ctrl0&FLD_OTP_PCE)&&(!clock_time_exceed(t, g_pce_timeout)));
	}
#endif
}


#if OTP_ALL_FUNCTION_EN
/**
 * @brief    1. This function servers to set otp pce timeout time, the optimal time cannot be set according to the system clock,
 *              because the time between two otp fetch commands cannot be determined,otp fetch commands will only be sent when the cache miss occurs,
 *              but when the miss is not known,so this time is self-determining based on power consumption and efficiency.
 *           2. due to otp setup logic problems, tcs and timeout have a certain relationship: tcs>timeout,the minimum value of tcs is 10us,
 *              which has been set as the optimal under different system clocks,so the relationship between tcs and timeout is shown as follows:
 *               * | :-------------- | :--------------- | :------------------  |
 *                 |    SYS_CLK      |    tcs_config    |   timeout_config     |
 *                 |   4M_Crystal    |     0x01         |     = 0x00 (15.75us) |
 *                 |   6M_Crystal    |     0x01         |     = 0x00 (10.5us)  |
 *                 |   8M_Crystal    |     0x01         |     = 0x00 (7.88us)  |
 *                 |   12M_Crystal   |     0x02         |     < 0x02 (10.5us)  |
 *                 |   16M_Crystal   |     0x02         |     < 0x02 (7.88us)  |
 *                 |   24M_Crystal   |     0x04         |     < 0x04 (10.5us)  |
 * @param[in] timeout_config -range:0-3,timeout_time = (timeout_config(bit[2:3])+1)*0x3f(system clock cycle),the default value is 0;
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_timeout(unsigned char timeout_config){
	/*
	 * tcs>timeout explains as follows:
	 * 1.when tcs=timeout:When the time of timeout and tcs is reached at the same time after the command is fetched,
	 *   pce is pulled down and ready signal is pulled up. when the next command is fetched, pce is pulled up, but the ready signal is already high,
	 *   which does not meet the tcs time, ack is returned, resulting in incorrect command fetching.
	 * 2.when tcs<timeout:When the tcs time is reached, the count cnt will return to 0,
	 *   the timeout time will not be reached, and the pce will not be pulled down. this situation does not affect the work,
	 *   but the timeout mechanism is almost gone.
	 * 3.when tcs>timeout:When timeout is up, count cnt returns to 0 and pce is pulled down, tcs time will not be reached,
	 *   when pce is pulled down, ready signal will not rise, so there will be no problem.
	 */
	reg_otp_offsetl =(reg_otp_offsetl &(~FLD_OTP_PCE_TO_SEL))|(timeout_config<<2);
	/*
	 * g_pce_timeout = (timeout_config+1)*(0x3f/system_clock+1) in here, use the minimum system clock(4M) defined timeout time
     *               = (timeout_config+1)*(0x3f/4+1)
     *               = (timeout_config+1)*16
     * because there is a certain relationship between tcs and timeout time, when system_clock is 4M,
     * timeout_config can only be 0. g_pce_timeout can be used in all system_clock cases.
	 */
	g_pce_timeout = 16;

}
#endif



/**
 * @brief      This function serves to enable the otp test area,only for internal testing,there are two operating units, each of which is 16 words,one start at address 0x00,
 *             the other the otp start at address 0x4000, test area address is independent of the real otp address,if use the otp test area,need to use this function.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_en(void){
	 reg_otp_ctrl0 |= FLD_OTP_PTR;
}
/**
 * @brief      This function serves to disable the otp test area,only for internal testing,if do not use the otp test area,use this function,
 *             will transition from test area to operating on real the otp.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_dis(void){
	reg_otp_ctrl0 &= ~(FLD_OTP_PTR);
}

/**
 * @brief      This function serves to wait until the operation of OTP is done.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_ static inline void otp_wait_done(void)
{
	while(reg_otp_dat & FLD_OTP_BUSY);
}

/*
 * @brief     This function is a common sequence used by these interfaces:otp_write32/otp_read_cycle/otp_set_active_mode.
 * @param[in] ptm_mode - ptm type.
 * @return    none
 */
_attribute_ram_code_sec_ static  void  otp_start(OTP_PtmTypeDef ptm_mode)
{
	//the pce pulls down, and the ready signal pulls it down,instead of manually closing clk_en;
	if(otp_program_flag==0||(!OTP_PCE_AUTO_MODE_EN))
	{
	   reg_otp_ctrl0 &= ~(FLD_OTP_PCE);
	}
	//ptm
	reg_otp_ctrl1 &= (~FLD_OTP_PTM);
	reg_otp_ctrl1 |= ptm_mode;
	//Tms >= 1(ns)
	//pce
	reg_otp_ctrl0 |= (FLD_OTP_PCE);
	//clk_en needs to be opened manually if the program is in flash;
#if !ONLY_SUPPORT_OTP_PROGRAM
	if(otp_program_flag==0){
		reg_otp_ctrl0 |= (FLD_OTP_PCLK_EN);
	}
#endif
}


/**
 * @brief      This function serves to write data to OTP memory,4 bytes one time.
 * @param[in]  ptm_mode - write mode.
 * @param[in]  addr  - the address of the data,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  data  - the data need to be write,4 bytes.
 * @return     none
 */
_attribute_ram_code_sec_ static void otp_write32(OTP_PtmTypeDef ptm_mode,unsigned int addr, unsigned int data)
{
	//if code run in otp,when switching to ram_code,prog/pwe must be pulled down,pce/pclk may be pulled up.
	//when write to otp, ptm needs to be configured,prog/pwe/pce need to pull down.
	otp_start(ptm_mode);
	//tcsp
	sleep_us(Tcsp);
	//prog pas addr data
	reg_otp_ctrl0 |= FLD_OTP_PPROG ;
	reg_otp_ctrl0 |=FLD_OTP_PAS;
	reg_otp_pa = addr;
	reg_otp_wr_dat = data;
	reg_otp_paio = 0;
	//Tpps
	sleep_us(Tpps);
	//redundancy programming  38*2
	for(unsigned char i = 1; i <= 76; i++){
		reg_otp_ctrl0 |= FLD_OTP_PWE;
		sleep_us(Tpw);
		reg_otp_ctrl0 &= ~(FLD_OTP_PWE);
		if(i<38){
			 reg_otp_paio = i;
		}
		else if(i==38){
			reg_otp_ctrl0&=(~FLD_OTP_PAS);
			reg_otp_pa = addr;
			reg_otp_wr_dat = data;
			reg_otp_paio = 0;
		}
		else if((i>38)&&(i<76))
		{
			reg_otp_paio = i-38;
		}
		else if(i==76)
		{
			break;
		}
		//because the for loop and the if judge the time,choose to use Tpwi/2.
		 sleep_us(Tpwi/2);
	}
	sleep_us(Tpph);
	//prog =0
	reg_otp_ctrl0 &= ~(FLD_OTP_PPROG);
	sleep_us(5);
	//pce=0
	reg_otp_ctrl0 &= ~(FLD_OTP_PCE);
	//Tms >= 1(ns)
	reg_otp_ctrl1 = ~(FLD_OTP_PTM);
}


/**
 * @brief      This function serves to read data from OTP memory.
 * @param[in]  ptm_mode - read mode.
 * @param[in]  addr - the address of the data,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_ static void otp_read_cycle(OTP_PtmTypeDef ptm_mode,unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	//if code run in otp,when switching to ram_code,pce/pclk may be pulled up,when read to otp,ptm needs to be configured,pclk/pce need to pull down.
	otp_start(ptm_mode);
	//after pulling the pce up, wait for the ready signal (clk working), no need to wait for the tcs manually.
    while(!(reg_otp_dat|FLD_OTP_READY));
    /*
	 * change the address increment mode to manual mode.In the address increment mode, due to the address increment function,
	 * when the for loop ends, an extra word will be added to the address, which may be increased to the address 0x7ffc that is not wanted to be read,
	 * triggering the encryption and decryption module by mistake.
	 */
    for(unsigned int i = 0; i < word_len; i++){
		reg_otp_pa = addr+i*4;
		reg_otp_rd_dat;
     	otp_wait_done();
		buff[i] = reg_otp_rd_dat;
	/*
	 * after executing reg_otp_rd_dat, two actions are triggered, one is to read the data, and the other is  trig hardware to read the current address,
	 * add a wait to prevent another read from 0x3ffc and an exception may occur during otp operations,so there is no limit to the address of the OTP.
	 */
		otp_wait_done();
	}

      reg_otp_ctrl0 &= ~ FLD_OTP_PCE;
}




/**
 * @brief      This function serves to preparations after otp software operation.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_ static void otp_auto_pce_restore(){

#if !ONLY_SUPPORT_OTP_PROGRAM
	if(otp_program_flag==1)
#endif
	{
	reg_otp_ctrl1 &= (~FLD_OTP_PTM);
    reg_otp_ctrl1 |= OTP_PTM_READ;
    /*
	 * if enable the pce_auto_mode function, after coming out of the software interface,
	 * the hardware will automatically give the correct timing when taking instructions, without manual configuration;
	 * if no enable the pce_auto_mode function,after coming out of the software interface,
	 * when the program is running in otp, the otp read sequence needs to be restored;
	 */
    #if !OTP_PCE_AUTO_MODE_EN
    	reg_otp_ctrl0 |= FLD_OTP_PCE;
        while(!(reg_otp_dat|FLD_OTP_READY));
    #endif
	}

	irq_restore(g_irq_en);
}
/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 *             voltage: otp write: 2.25V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 * @param[in]  addr - the address of the data,it has to be a multiple of 4,the OTP memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     0 :it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             1 :it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_write(unsigned int addr,unsigned int word_len,unsigned int *buff)
{
    otp_pce_timeout_exceed();
	for(unsigned int i=0;i<word_len;i++)
	{
		unsigned int temp = 0;
		otp_read_cycle(OTP_PTM_READ,addr + i*4, 1, (unsigned int *)&temp);
		if(temp == 0xffffffff){
			otp_write32(OTP_PTM_PROG,addr + i*4,buff[i]);
	    }else if(temp != buff[i]){
	    	otp_auto_pce_restore();
		    return 1;
	    }

	}

	otp_auto_pce_restore();

	return 0;
}
/**
 * @brief      This function serves to read data from OTP memory,belong to otp normal read.
 *             otp has three kinds of read mode,in general,just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,need to use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_read(unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	otp_pce_timeout_exceed();
	otp_read_cycle(OTP_PTM_READ,addr,word_len,buff);
	otp_auto_pce_restore();
}

#if OTP_ALL_FUNCTION_EN
/**
 * @brief      This function serves to read data from OTP memory,belong to otp pgm marginread.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,need to use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_pgm_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	otp_pce_timeout_exceed();
	otp_read_cycle(OTP_PTM_INIT_MARGIN_READ,addr,word_len,buff);
	otp_auto_pce_restore();

}
/**
 * @brief      This function serves to read data from OTP memory,belong to otp initial margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,pm_set_dcdc_output(DCDC_2P5V) can be to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,need to use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_initial_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff)
{
	otp_pce_timeout_exceed();
	otp_read_cycle(OTP_PTM_PGM_MARGIN_READ,addr,word_len,buff);
	otp_auto_pce_restore();
}
#endif





/********************************************************************************************************
 * @file    cstartup_flash.S
 *
 * @brief   This is the boot file for TC321X
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
#ifdef MCU_STARTUP_FLASH

#include "compiler.h"

//The default driver configuration is 32K to ensure that the default driver configuration can work properly on all models.
//In actual use, please configure the sram size according to your own model, otherwise the efficiency will be affected.
#define SRAM_32K      (0x848000)	//32KSRAM
#define SRAM_64K      (0x850000)	//64KSRAM

#define	SRAM_SIZE     SRAM_32K


/**
 * @brief	1. If the size of the chip's sram is 32K (SRAM_SIZE needs to be set to SRAM_32K), the bin compiled by ALL_SRAM_CODE=1 can be burned to flash or loaded to sram directly with the tool;
            2. If the chip's sram size is 64K:
               - SRAM_SIZE is set to SRAM_32K, ALL_SRAM_CODE=1 compiled bin, burned to flash or directly loaded to sram with tools, both can be executed;
               - SRAM_SIZE is set to SRAM_64K, ALL_SRAM_CODE=1, if burned into flash, ALL_SRAM_START_MODE = ALL_SRAM_WITH_FLASH_START;
                 if burned into sram, ALL_SRAM_START_MODE = ALL_SRAM_START_MODE. ALL_SRAM_START_MODE = ALL_SRAM_WITH_RAM_START;
 */
#ifndef	ALL_SRAM_CODE
#define	 ALL_SRAM_CODE					0
#endif

#if(SRAM_SIZE   ==  SRAM_64K && ALL_SRAM_CODE)
#define ALL_SRAM_WITH_FLASH_START      0
#define ALL_SRAM_WITH_RAM_START        1
#define ALL_SRAM_START_MODE            ALL_SRAM_WITH_RAM_START
#endif

#if	ALL_SRAM_CODE

#define  FLL_STK_EN           			0
#define  ZERO_IC_TAG_EN		  			0
#define  IC_TAG_CACHE_ADDR_EQU_EN    	1
#define  COPY_DATA_EN					0

#else

#define  FLL_STK_EN           			1
#define  ZERO_IC_TAG_EN		  			1
#define  IC_TAG_CACHE_ADDR_EQU_EN    	0
#define  COPY_DATA_EN					1

#endif

#define WD_SET_AND_START_EN             0 /* default 0; 1: Set and Start the watchdog in advance. */

#ifndef __LOAD_RAM_SIZE__
#define	__LOAD_RAM_SIZE__		0xc
#endif

#ifndef __IRQ_STK_SIZE__
#define	__IRQ_STK_SIZE__		0x180
#endif
	.code	16
@********************************************************************************************************
@                                           MACROS AND DEFINIITIONS
@********************************************************************************************************

					@ Mode, correspords to bits 0-5 in CPSR
	.equ MODE_BITS,		0x1F	@ Bit mask for mode bits in CPSR
	.equ IRQ_MODE, 		0x12	@ Interrupt Request mode
	.equ SVC_MODE, 		0x13	@ Supervisor mode 
	.equ IRQ_STK_SIZE,	__IRQ_STK_SIZE__
#ifdef DUT_TEST
	.equ __LOAD_FLASH, 	0
	.equ __LOAD_DUT, 	12
	.equ __LOAD_RAM, 	0
#else
#if	 ALL_SRAM_CODE
	.equ __LOAD_FLASH, 	0
	.equ __LOAD_DUT, 	0
	.equ __LOAD_RAM, 	1
#else
	.equ __LOAD_FLASH, 	1
	.equ __LOAD_DUT, 	0
	.equ __LOAD_RAM, 	0
#endif
#endif
@********************************************************************************************************
@                                            TC32 EXCEPTION VECTORS
@********************************************************************************************************

	.section	.vectors,"ax"
	.global		__reset
	.global	 	__irq
	.global 	__start
	.global		__LOAD_RAM
	.global		__LOAD_DUT
	.global		__LOAD_FLASH

__start:					@ MUST,  referenced by boot.link

	.extern irq_handler

	.extern firmwareVersion

    .extern  _cstartup_ram_funcs_size_div_16_
	.extern  _ramcode_size_div_16_
	.extern  _ramcode_size_div_256_
	.extern  _ramcode_size_div_16_align_256_
	.extern  _ramcode_size_align_256_
	.extern  _ictag_start_
	.extern  _ictag_end_
	.extern  g_pm_multi_addr
	.org 0x0
	tj	__reset
#ifdef DUT_TEST
	.word	(0x0004)
#endif
@	.word	(firmwareVersion)
	.org 0x8
	.word	(0x544c4e4b)
	@Boot will write the value here to the digital register 0x602, mainly the difference of 0x602[4],
	@write 0 to fetch instructions from flash;

#if(SRAM_SIZE   ==  SRAM_32K)
#if	ALL_SRAM_CODE
	.word	(0x00880000 + _bin_size_div_16)
#else
	.word	(0x00880000 + _ramcode_size_div_16_align_256_)
#endif
#elif(SRAM_SIZE   ==  SRAM_64K )
    .word	(0x00880000 + _cstartup_ram_funcs_size_div_16_)
	.extern copy_ramcode
	.extern copy_bincode
#endif

	.org 0x10
	tj		__irq
	.org 0x14
#ifdef DUT_TEST
	.word	(0x01008278+__LOAD_DUT<<16)
#else
	.word	(0x00008278)
#endif
	.org 0x18
	.word	(_bin_size_)
	.extern  main

	.org 0x20

	.global	start_suspend
	.thumb_func
	.type	start_suspend, %function

start_suspend:						@ instruction cache address + 0x58: 0x3c16 * 4 = 0xf058
	tpush   {r2-r3}

    tmovs	r2, #129				@0x81
    tloadr	r3, __suspend_data      @0x80006f
    tstorerb	r2, [r3, #0]		@*(volatile unsigned char *)0x80006f = 0x81

    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8
    tmov	r8, r8

    tpop	{r2-r3}
    tjex	lr
	.align 4
__suspend_data:
	.word   (0x80006f)

__reset:
#if WD_SET_AND_START_EN
    @ reference wd_set_interval_ms
    tloadr      r1, WD_REG_BASE    @0x800620
    tloadr      r0, WD_REG_BASE+8  @  default 0x5b
    tshftl      r0, r0,#1          @  (WD_REG_BASE+8)<<1
    tstorerb    r0, [r1, #1]       @ *(unsigned char*)(0x800621)= (WD_REG_BASE+8)<<1
    tshftr      r0, r0,#8          @  (WD_REG_BASE+8)>>8
    tstorerb    r0, [r1, #2]       @ *(unsigned char*)(0x800622)=(WD_REG_BASE+8)>>8

    @ reference wd_start
    tloadr      r1, WD_REG_BASE    @0x800620
    tmov        r0, #0x40          @ 01000000
    tstorerb    r0, [r1, #0]       @ *(unsigned char*)(0x800620)=0x60,reg_tmr_ctrl8[6]=1

    tloadr      r1, WD_REG_BASE+4  @0x800622
    tloadrb     r2, [r1]
    tmov        r0, #0x80          @ 10000000
    tor         r0, r2
    tstorerb    r0, [r1, #0]       @ reg_tmr_ctrl8 |=reg_tmr_ctrl8|0x80
#endif

#if 0
	@ add debug, PB4 output 1
	tloadr     	r1, DEBUG_GPIO_OEN    @0x80050a  PB oen
	tmov		r0, #16		@0b 00010000
	tstorerb	r0, [r1, #0]

	tloadr     	r1, DEBUG_GPIO_OUTPUT
	tmov		r0, #16			@0b 00010000
	tstorerb	r0, [r1, #0]	@0x8005c4  PB output set
#endif


#if BLC_PM_DEEP_RETENTION_MODE_EN
FLASH_WAKEUP_BEGIN:
	tloadr      r0,DATA_I+44
	tmov		r1,#0
	tstorerb    r1,[r0,#1]
	tmov        r1,#171						@Flash deep cmd: 0xAB
	tstorerb    r1,[r0,#0]
	tmov		r2,#0
	tmov        r3,#6
TNOP:
	tadd        r2,#1
	tcmp        r2,r3
	tjle        TNOP
	tmov		r1,#1
	tstorerb    r1,[r0,#1]
FLASH_WAKEUP_END:
#endif


@********************************************************************************************************
@                              		 FILL .DATA AND .BSS WITH 0xFF
@********************************************************************************************************
#if FLL_STK_EN

	tloadr	r0, FLL_D						@r0 = 0xffffffff
	tloadr	r1, FLL_D+4						@r1 = _start_data_
	tloadr	r2, FLL_D+8						@r2 = 0x850000

FLL_STK:
	tcmp	r1, r2							@
	tjge	FLL_STK_END						@r1>=r2 jump to FLL_STK_END
	tstorer r0, [r1, #0]					@*(unsigned int*)(_start_data_)=0xffffffff
	tadd    r1, #4							@r1 = r1+4
	tj		FLL_STK							@jump to FLL_STK
FLL_STK_END:

#endif
@********************************************************************************************************
@                              		 UPDATE SP UNDER IRQ/SVC MODE
@********************************************************************************************************
	tloadr	r0, DAT0						@r0 = 0x12 IRQ
	tmcsr	r0								@CPSR=r0
	tloadr	r0, DAT0 + 8					@r0 = irq_stk + IRQ_STK_SIZE
	tmov	r13, r0  						@r13/SP= r0    	update SP under IRQ mode

	tloadr	r0, DAT0 + 4					@r0 = 0x13 SVC
	tmcsr	r0								@CPSR=r0
	tloadr	r0, DAT0 + 12					@r0 = 0x850000
	tmov	r13, r0  						@r13= r0		update SP under SVC mode
@********************************************************************************************************
@                                    .BSS INITIALIZATION FOR 0
@********************************************************************************************************
	tmov	r0, #0							@r0 = 0
	tloadr	r1, DAT0 + 16					@r1 = _start_bss_
	tloadr	r2, DAT0 + 20					@r2 = _end_bss_

ZERO_BSS_BEGIN:
	tcmp	r1, r2
	tjge	ZERO_BSS_END					@r1>=r2 jump to ZERO_BSS_END
	tstorer	r0, [r1, #0]					@*(unsigned int*)(_start_bss_)=r0=0
	tadd    r1, #4							@r1 = r1 + 4
	tj		ZERO_BSS_BEGIN					@jump to ZERO_BSS_BEGIN
ZERO_BSS_END:
@********************************************************************************************************
@                                    IC TAG INITIALIZATION
@********************************************************************************************************
#if ZERO_IC_TAG_EN
ZERO_TAG:
	tmov    r0, #0
	tloadr	r1, DAT0 + 28					@r1 = _ictag_start_
	tloadr	r2, DAT0 + 32					@r2 = _ictag_end_
ZERO_TAG_BEGIN:
	tcmp	r1, r2
	tjge	ZERO_TAG_END					@r1>=r2 jump to ZERO_TAG_END
	tstorer	r0, [r1, #0]					@*(unsigned int*)(_ictag_start_)=r0=0
	tadd    r1, #4							@r1 = r1 + 4
	tj		ZERO_TAG_BEGIN					@jump to ZERO_TAG_BEGIN
ZERO_TAG_END:
#endif
@********************************************************************************************************
@                                    IC CACHE INITIALIZATION
@********************************************************************************************************

SET_IC:
	tloadr     	r1, DAT0 + 24				@ r1 = 0x80060c
	tloadr      r0, DAT0 + 36				@ r0 = _ramcode_size_align_256_
	tshftr      r0,r0,#8                    @ r0 = _ramcode_size_align_256_/256
	tstorerb	r0, [r1, #0]				@ *(unsigned int*)(0x80060c) = r0
#if IC_TAG_CACHE_ADDR_EQU_EN

#else
	tadd    	r0, #1						@ r0 = r0 + 1
#endif
	tstorerb	r0, [r1, #1]				@ *(unsigned int*)(0x80060d) = r0
SET_IC_END:
@********************************************************************************************************
@                                    DCDC INITIALIZATION FOR FLASH
@********************************************************************************************************
 #if  BLC_PM_DEEP_RETENTION_MODE_EN
	/*system on*/
	tloadr      r0,DATA_I+32                @0x00800060
	tloadr      r1,DATA_I+36                @0xff000000
	tstorer     r1,[r0,#0]                  @*(unsigned int*)0x800060=0x00ffffff
	tloadr      r0,DATA_I+48                @0x0080005e
	tmov        r1, #1
	tstorerb    r1,[r0,#0]					@*(unsigned int*)0x80005e=0x01



    /*check ana_reg_0x7e*/
	tloadr		r0,DATA_I+24				@0x7e

RANA_REG_BEGIN:
	tloadr		r1,DATA_I+28			    @0x008000b8
	tmov		r2,r0						@ana_reg_adr:0x7e
	tstorerb	r2,[r1,#0]					@*(unsigned int*)0x8000b8=0x7e;
	tmov   	    r2,#64						@ana_reg_dat:0x40=64
	tstorerb	r2,[r1,#2]					@*(unsigned int*)0x8000ba=0x40;

RWAIT_REG_FINISH:
	tloadrb	    r2,[r1,#2]
	tshftl		r2,r2,#31
	tshftr		r2,r2,#31
	tcmp		r2,#1
	tjeq		RWAIT_REG_FINISH
	tloadrb	    r2,[r1,#1]
RANA_REG_END:

	tshftl      r2,r2,#31
	tshftr		r2,r2,#31
	tcmp        r2,#0
	/*if ana_reg_0x7e==0x00 retention data initiate after waking up from deep*/
	tjeq		POWER_ON_OR_DEEP_BRANCH
DEEP_RETENTION_BRANCH:
	tloadr      r0, MULTI_ADDRESS_DATA
	tloadr		r1, MULTI_ADDRESS_DATA+4
	tloadrh     r2, [r1,#0]
	tstorerh    r2, [r0,#0]
#if COPY_DATA_EN
	tj			COPY_DATA                   @deep wakeup with retention
#else
	tj          COPY_DATA_END
#endif
POWER_ON_OR_DEEP_BRANCH:
#endif

#if(SRAM_SIZE == SRAM_64K)
#if (ALL_SRAM_CODE&&(ALL_SRAM_START_MODE == ALL_SRAM_WITH_FLASH_START))
    tjl copy_bincode
#elif(ALL_SRAM_CODE ==0)
    tjl copy_ramcode
#endif
#endif


@********************************************************************************************************
@                                    RETENTION DATA INITIALIZATION
@********************************************************************************************************
COPY_RET_DATA:
	tloadr		r1, DATA_I					@ r1 = _rstored_
	tloadr		r2, DATA_I+4				@ r2 = _retention_data_start_
	tloadr		r3, DATA_I+8				@ r3 = _retention_data_end_
COPY_RET_DATA_BEGIN:
	tcmp		r2, r3						@
	tjge		COPY_RET_DATA_END			@ r2>=r3 jump to COPY_RET_DATA_END
	tloadr		r0, [r1, #0]				@ r0 = *(unsigned int*)(_rstored_)
	tstorer 	r0, [r2, #0]				@ *(unsigned int*)(_retention_data_start_) = r0
	tadd    	r1, #4						@ r1 = r1 + 4
	tadd		r2, #4						@ r2 = r2 + 4
	tj			COPY_RET_DATA_BEGIN			@ jump to COPY_RET_DATA_BEGIN
COPY_RET_DATA_END:

@********************************************************************************************************
@                                    DATA INITIALIZATION
@********************************************************************************************************
COPY_DATA:
	tloadr		r1, DATA_I+12				@ r1 = _dstored_
	tloadr		r2, DATA_I+16				@ r2 = _start_data_
	tloadr		r3, DATA_I+20				@ r3 = _end_data_
COPY_DATA_BEGIN:
	tcmp		r2, r3						@
	tjge		COPY_DATA_END				@ r2>=r3 jump to COPY_DATA_END
	tloadr		r0, [r1, #0]				@ r0 = *(unsigned int*)(_dstored_)
	tstorer 	r0, [r2, #0]				@ *(unsigned int*)(_start_data_) = r0
	tadd    	r1, #4						@ r1 = r1 + 4
	tadd		r2, #4						@ r2 = r2 + 4
	tj			COPY_DATA_BEGIN				@ jump to COPY_DATA_BEGIN
COPY_DATA_END:



#if 0
SETSPISPEED:
	tloadr     	r1, DAT0 + 36
	tmov		r0, #0xbb		@0x0b for fast read; 0xbb for dual dat/adr
	tstorerb	r0, [r1, #0]
	tmov		r0, #3			@3 for dual dat/adr
	tstorerb	r0, [r1, #1]
#endif

ENTER_MAIN:
	tjl	main
END:	tj	END

	.balign	4
DAT0:
	.word	0x12			    		@0		IRQ
	.word	0x13			    		@4		SVC
	.word	(irq_stk + IRQ_STK_SIZE)	@8


    .word	(SRAM_SIZE)		    		@12  	stack end


	.word	(_start_bss_)               @16
	.word	(_end_bss_)                 @20
	.word	(0x80060c)                  @24
	.word	_ictag_start_               @28		IC tag start
	.word	_ictag_end_	            	@32		IC tag end
#if IC_TAG_CACHE_ADDR_EQU_EN
	.word	(0x00f000)    				@36
#endif
	.word	_ramcode_size_align_256_    @40

DATA_I:	
	.word   (_rstored_)                 @0
	.word   (_retention_data_start_)    @4
    .word   (_retention_data_end_)      @8
	.word	_dstored_					@12
	.word	_start_data_				@16
	.word	_end_data_					@20
    .word	(0x0000007e)                @24
	.word	(0x008000b8)                @28
	.word	(0x00800060)                @32
    .word	(0x00ffffff)                @36
    .word	(0x000000f7)                @40
	.word	(0x0080000c)                @44
	.word	(0x0080005e)                @48

DEBUG_GPIO_OEN:
	.word	(0x80050a)                  @  PBx oen

DEBUG_GPIO_OUTPUT:
	.word	(0x8005c4)                  @  PBx output set

MC_ICCT1:
	.word	(0x800603)                  @  mc icct1

#if WD_SET_AND_START_EN
WD_REG_BASE:
    .word   (0x800620)                  @0  reg_tmr_ctrl
    .word   (0x800622)                  @4  reg_wd_ctrl1
@ capt tick=500ms=(12000000>>18)=0x2d, capt tick = 1s =(24000000>>18) =0x5b,
@ capt tick=  2s =(48000000>>18)=0xb7, capt tick = 3s =(72000000>>18) =0x112,
@ capt tick=  4s =(96000000>>18)=0x16e,capt tick = 5s =(120000000>>18)=0x1c9,
    .word   (0x5b)                      @8  capt tick
#endif

FLL_D:
	.word	0xffffffff                	@0
	.word	(_start_data_)    			@4


    .word	(SRAM_SIZE)		    		@12  	stack end


	.word   _ramcode_size_div_256_      @12

MULTI_ADDRESS_DATA:
	.word	(0x0080063e)				@0
	.word   g_pm_multi_addr				@4

	.align 4
__irq:
	tpush    	{r14}					@push R14/LR register
	tpush    	{r0-r7}					@push r0~r7 into stack
	tmrss    	r0						@save cpsr to r0
	
	tmov		r1, r8					@r8~r12 cannot use the way like {r0~r7} to push into stack
	tmov		r2, r9					@must move r8~r12 into r1~r5 to push
	tmov		r3, r10
	tmov		r4, r11
	tmov		r5, r12
	tpush		{r0-r5}
	
	tjl      	irq_handler				@jump to irq_handler

	tpop		{r0-r5}					@pop r8~r12
	tmov		r8, r1
	tmov		r9, r2
	tmov		r10,r3
	tmov		r11,r4
	tmov		r12,r5

	tmssr    	r0						@return r0 to cpsr
	tpop		{r0-r7}					@pop r0~r7
	treti    	{r15}					@return to pc



ASMEND:

	.section .bss
	.align 4
	.lcomm irq_stk, IRQ_STK_SIZE
	.end
#endif

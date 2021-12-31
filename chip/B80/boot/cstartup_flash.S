/********************************************************************************************************
 * @file	cstartup_flash.S
 *
 * @brief	This is the boot file for b80
 *
 * @author	Driver Group
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifdef MCU_STARTUP_FLASH
#include "driver_func_cfg.h"

#define SRAM_SIZE_16K_EN		    	    1/*defaut sram size=16k*/
#define FLASH_CODE_EN						1/*default flash code, run otp code need cahnge to 0.*/

#define  FLL_STK_EN           			1
#define  ZERO_IC_TAG_EN		  			1
#define  IC_TAG_CACEH_ADDR_EQU_EN    	0
#define  FLASH_WAKEUP_EN                1
#define  COPY_DATA_EN					1
#define  MULTI_ADDRESS_START			1

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
	.equ __LOAD_FLASH, 	1
	.equ __LOAD_DUT, 	0
	.equ __LOAD_RAM, 	0
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

	.extern  _ramcode_size_div_16_
	.extern  _ramcode_size_div_256_
	.extern  _ramcode_size_div_16_align_256_
	.extern  _ramcode_size_align_256_
	.extern  _ictag_start_
	.extern  _ictag_end_
	.extern  tl_multi_addr
	.org 0x0
	tj	__reset
@	.word	(firmwareVersion)
	.org 0x8
	.word	(0x544c4e4b)
	@Boot will write the value here to the digital register 0x602, mainly the difference of 0x602[4],
	@write 1 to fetch instructions from otp, and write 0 to fetch instructions from flash;
#if FLASH_CODE_EN
	.word	(0x00880000 + _ramcode_size_div_16_align_256_)
#else
	.word	(0x00980000 + _ramcode_size_div_16_align_256_)
#endif

	.org 0x10
	tj		__irq
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

#if 0
	@ add debug, PB4 output 1
	tloadr     	r1, DEBUG_GPIO    @0x80058a  PB oen
	tmov		r0, #139      @0b 11101111
	tstorerb	r0, [r1, #0]

	tmov		r0, #16			@0b 00010000
	tstorerb	r0, [r1, #1]	@0x800583  PB output
#endif

#if FLASH_CODE_EN
	@when running the flash program, let the otp enter the deep standby mode to reduce the power consumption of the chip.
	tloadr		r3, OTP_CTRL	  @0x800010	otp ctrl
	tloadrb		r1, [r3, #0]
	tmovs		r0, #253		  @0b 11111101
	tand		r1, r0
 	tstorerb	r1, [r3, #0]	  @pce = 0

	tloadrb		r1, [r3, #1]
	tmovs		r0, #207		  @0b 11001111 mc icct1
	tand		r1, r0
 	tstorerb	r1, [r3, #1]	  @pdstb = 0  pldo = 0
#else
	/*
	 * When running the otp program, after the retention sleep wakes up, the otp ack enablement will be lost, causing the program to freeze,
	 * and you need to re-enable otp ack, add by junhui.hu, confirmed by jianzhi 20211021.
	 */
	tloadr		r1, MC_ICCT1
	tmovs		r0, #1		  	  @0b 00000001
	tstorerb	r0, [r1, #0]	  @otp ack = 1
#endif

#if FLASH_WAKEUP_EN

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

@ efuse load need delay about 18us
	tmov 	r2,#0
	tmov 	r3,#110
EFUSE_DELAY:
	tadd 	r2,#1
	tcmp 	r2,r3
	tjle 	EFUSE_DELAY
EFUSE_DELAY_END:

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
#if PM_RETENTION_FUNCTION_EN
	/*system on*/
	tloadr      r0,DATA_I+32                @0x00800060
	tloadr      r1,DATA_I+36                @0xff000000
	tstorer     r1,[r0,#0]                  @*(unsigned int*)0x800060=0xff000000
	tshftr      r1,r1,#24					@0x000000ff
	tstorerb    r1,[r0,#4]					@dig_0x64=0xff
	tloadr      r1,DATA_I+40				@dig_0x65=0xf7
	tstorerb    r1,[r0,#5]


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
#if MULTI_ADDRESS_START
	tjeq		MULTI_ADDRESS_END
MULTI_ADDRESS_BEGIN:
	tloadr      r0, MULTI_ADDRESS_DATA
	tloadr		r1, MULTI_ADDRESS_DATA+4
	tloadr      r2, [r1,#0]
	tstorerh    r2, [r0,#0]
#if COPY_DATA_EN
	tj			COPY_DATA                   @deep wakeup with retention
#else
	tj          COPY_DATA_END
#endif
MULTI_ADDRESS_END:
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
#endif

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
#if  SRAM_SIZE_16K_EN
	.word	(0x844000)		    		@12  	stack end
#else
	.word	(0x850000)		    		@12  	stack end
#endif
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
    .word	(0xff080000)                @36
    .word	(0x000000f7)                @40
	.word	(0x0080000c)                @44

DEBUG_GPIO:
	.word	(0x80050a)                  @  PBx oen

OTP_CTRL:
	.word	(0x800010)                  @  otp control

MC_ICCT1:
	.word	(0x800603)                  @  mc icct1

FLL_D:
	.word	0xffffffff                	@0
	.word	(_start_data_)    			@4
#if  SRAM_SIZE_16K_EN
	.word	(0x844000)		    		@12  	stack end
#else
	.word	(0x850000)		    		@12  	stack end
#endif

	.word   _ramcode_size_div_256_      @12

MULTI_ADDRESS_DATA:
	.word	(0x0080063e)				@0
	.word   tl_multi_addr				@4

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

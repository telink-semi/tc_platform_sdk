/********************************************************************************************************
 * @file    driver_cstartup.S
 *
 * @brief   This is the boot file for Telink MCU
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifdef MCU_STARTUP_MULTI_BIN_DRIVER
#include "compiler.h"
#include "../../common/memory_config.h"
    .equ __DRIVER_ROM_START_ADDR,     DRIVER_ROM_START_ADDR
    .equ __DRIVER_SRAM_START_ADDR,    DRIVER_SRAM_START_ADDR
    .equ __DRIVER_RAM_MAX_SIZE,       DRIVER_RAM_MAX_SIZE
    .equ __DRIVER_ROM_MAX_SIZE,       DRIVER_ROM_MAX_SIZE



    .global     __DRIVER_RAM_MAX_SIZE
    .global     __DRIVER_ROM_MAX_SIZE
    .global     __DRIVER_ROM_START_ADDR
    .global     __DRIVER_SRAM_START_ADDR


	.section	.vectors,"ax"
	.global		__reset
	.global 	__start



__start:					@ MUST,  referenced by boot.link

	.extern  tl_multi_addr
	.org 0x0
	tj	__reset

	.org 0x18
	.word	(_SRAM_USAGE_SIZE)
	.extern  main

    .org 0x28
	.word     g_driver_func_list_addr        //@0x28
	.word     _RETENTION_DATA_LMA_START      //@0x2c
    .word     _RETENTION_DATA_VMA_START      //@0x30
    .word     _RETENTION_DATA_VMA_END        //@0x34
    .word     _DATA_LMA_START                //@0x38
    .word     _DATA_VMA_START                //@0x3c
    .word     _DATA_VMA_END                  //@0x40
    .word     _BSS_VMA_START                 //@0x44
    .word     _BSS_VMA_END                   //@0x48
    .word     _SRAM_USAGE_SIZE               //@0x4c





    .balign	4
__reset:
	tjl	main
END:
    tj	END


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



#endif

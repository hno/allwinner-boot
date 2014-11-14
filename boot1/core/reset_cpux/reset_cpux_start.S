/*
;***********************************************************************************************************************
;                                                         eGON
;                                         the Embedded GO-ON Bootloader System
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;                                                  All Rights Reserved
;
; File Name : Boot1.s
;
; Author : Gary.Wang
;
; Version : 1.1.0
;
; Date : 2007.10.15
;
; Description :
;
; Functions list : none.
;
; Others : None at present.
;
;
; History :
;
;  <Author>        <time>       <version>      <description>
;
; Gary.Wang       2007.10.15      1.1.0        build the file
;
;***********************************************************************************************************************
*/
	#include "arm_a8.h"

/*********************************BOOT 系统初始化代码********************************/
	.globl cpux_s_start
cpux_s_start:
@;//disable all interrupts
@;//set processor mode as SVC
	mrs r0, cpsr
	bic r0, r0, #ARMV7_MODE_MASK
	orr r0, r0, #ARMV7_SVC_MODE
	orr r0, r0, #( ARMV7_FIQ_MASK | ARMV7_IRQ_MASK )      @;// After reset, ARM automaticly disables IRQ and FIQ, and runs in SVC mode.
	bic r0, r0, #ARMV7_CC_E_BIT                           @;// set little-endian
	msr cpsr_c, r0


@;// configure memory system : disable MMU,cache and write buffer; set little_endian; set high interrupt vector table
	mrc p15, 0, r0, c1, c0
	bic r0, r0, #( ARMV7_C1_M_BIT | ARMV7_C1_C_BIT )  @;// disable MMU, data cache
	bic r0, r0, #( ARMV7_C1_I_BIT | ARMV7_C1_Z_BIT )  @;// disable instruction cache, disable flow prediction
	bic r0, r0, #( ARMV7_C1_A_BIT)                    @;// disable align
	mcr p15, 0, r0, c1, c0

	mrs r0, cpsr
    bic r0, r0, #ARMV7_MODE_MASK
    orr r0, r0, #ARMV7_SVC_MODE
    msr cpsr_c, r0

@;/**********************************the end of initializing system*********************************/

@;// set SP for SVC mode	
	mrc p15, 0, r0, c0, c0, 5
	lsl	r0, r0, #10
	add	sp, r0, #0x41000000
 
	b  cpux_c_start

 	b .                                 @;// infinite loop


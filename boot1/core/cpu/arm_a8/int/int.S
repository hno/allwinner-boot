/*
;*****************************************************************************************************************
;                                                         eGON
;                                         the Embedded GO-ON Bootloader System
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;											       All Rights Reserved
;
; File Name : asm_misc.s
;
; Author : Gary.Wang
;
; Version : 1.1.0
;
; Date : 2009.09.14
;
; Description :
;
; Others : None at present.
;
;
; History :
;
;  <Author>        <time>       <version>      <description>
;
; Gary.Wang       2009.09.14      1.1.0        build the file
;
;******************************************************************************************************************
*/


#include "arm_a8.h"



/*
;*******************************************************************************
;函数名称: open_sys_int
;函数原型: void open_sys_int( void )
;函数功能: open system irq interrupt
;入口参数: void
;返 回 值: void
;备    注:
;*******************************************************************************
*/
	.globl open_sys_int
open_sys_int:
	mrs r0, cpsr
	bic r0, r0, #( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )
	msr cpsr_c, r0
	mov pc, lr


/*
;*******************************************************************************
;函数名称: close_sys_int
;函数原型: void close_sys_int( void )
;函数功能: close system irq interrupt
;入口参数: void
;返 回 值: void
;备    注:
;*******************************************************************************
*/
	.globl close_sys_int
close_sys_int:
	mrs r0, cpsr
	orr r0, r0, #( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )
	msr cpsr_c, r0
	mov pc, lr



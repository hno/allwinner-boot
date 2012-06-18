;/*
;**********************************************************************************************************************
;*											        eGon
;*						           the Embedded GO-ON Bootloader System
;*									       eGON arm boot sub-system
;*
;*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
;*                                           All Rights Reserved
;*
;* File    : arm_swi
;*
;* By      : Jerry
;*
;* Version : V2.00
;*
;* Date	  :
;*
;* Descript:
;**********************************************************************************************************************
;*/
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
	.globl _sys_int_enable
_sys_int_enable:
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
	.globl _sys_int_disable
_sys_int_disable:
	mrs r0, cpsr
	orr r0, r0, #( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )
	msr cpsr_c, r0
	mov pc, lr


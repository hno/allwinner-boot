/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : arm_start
*
* By      : Jerry
*
* Version : V2.00
*
* Date	   :  2020-9-19
*
* Descript:
*
**********************************************************************************************************************
*/

	#include "arm_a8.h"
/*********************************BOOT 系统初始化代码********************************/
/*
@start
@; ARM 向量表 ..........
@; 00 - Reset
@; 04 - Undefined instructions
@; 08 - SWI instructions
@; 0C - prefetch abort
@; 10 - Data abort
@; 14 - Reserved
@; 18 - IRQ interrupts
@; 1C - FIQ interrupts
@;支持长跳转
*/
     ldr     pc,_undefined_instruction
     ldr     pc,_undefined_instruction
     ldr     pc,_software_interrupt
     ldr     pc,_undefined_instruction
     ldr     pc,_undefined_instruction
     ldr     pc,_undefined_instruction
     ldr     pc,_irq
     ldr     pc,_undefined_instruction

_undefined_instruction:	.word undefined_instruction
_software_interrupt:    .word software_interrupt
_irq:					.word irq_interrupt

#define     INTC_REG_VCTR    0x1c20400
/*---------------------------------------------------------------------------------------*/

irq_interrupt:
	sub     lr, lr, #4                                      @; 保存返回地址
	stmfd   sp!, {r0-r12, lr}@; save context       	        @; 寄存器压栈
	mrs     r3, spsr                                        @; 读取SPSR
	stmfd   sp!, {r3}	                                    @; 压栈

    msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_SYSTEM_MODE)     @; 切换到SYSTEM模式
    stmfd   sp!, {r0-r12, lr}                                @; 保存lr_usr和其它用到的寄存器

	bl      gic_irq_handler

    ldmfd   sp!, {r0-r12, lr}                           @; 恢复SYSTEM模式寄存器
    msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_IRQ_MODE)     @; 切换到IRQ模式
    ldmfd   sp!, {r3} 	                                    @; 数据出栈
    msr     spsr_cxsf, r3                                   @; 还原spsr

	ldmfd   sp!, {r0-r12, pc}^    @;从异常模式返回; unknown event ignore


undefined_instruction:
   b  undefined_instruction

software_interrupt:
   b  eGon2_swi_handler
/*------------------------------------------end------------------------------------------*/






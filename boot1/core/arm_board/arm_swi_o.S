/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : arm_swi
*
* By      : Jerry
*
* Version : V2.00
*
* Date	   :
*
* Descript:
*
**********************************************************************************************************************
*/

	#include "arm_a8.h"
/*********************************ARM SWI HANDLE********************************/
	.globl eGon2_swi_handler
eGon2_swi_handler:
	stmfd   sp!, {r0-r4}               @; 保存regs
	ldr     r3,  [lr,#-4]              @; swi号
	bic     r3,  r3, #0xff000000       @; Mask off top 8 bits

	mrs     r0, spsr                   @; move the spsr into gp register
	mov     r1, sp                     @; 保存svc模式下的sp指针到r1
	mov     r4, lr					   @; 保存svc模式下的lr到r4寄存器中
@;	add     sp, sp, #20				   @; 修正svc的sp指针(后面即将弹出r0-r4，由于已经切换到system模式，无法修改svc的sp，因此提前修改)
	msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_SYSTEM_MODE)   @;切换到SYSTEM模式,关闭IRQ,FIQ
    stmfd   sp!, {r4}				   @; 把r4(即svc模式下的lr)压到system的栈中
    stmfd   sp!, {r0, r5-r12}		   @; 保存r0(svc模式下的spsr)，r5-r7(后面将使用到)

    bic     r5, r0, #ARMV7_MODE_MASK    @; 切换到SYSTEM模式
    orr     r5, r5, #ARMV7_SYSTEM_MODE  @;
@;    mov     r6, r1					   @; 原来r1保存svc的栈指针，现在用r6保存，因为r1由于弹出数据将被修改

	msr     cpsr_c, r5				   @; 还原到进入svc之前的状态
@;	stmfd   sp!, {r6}

	ldr     r2, =eGon2_swi_handler_entry
	mov     r0,  r3
	mov     lr,  pc					   @; 保存system模式下的lr
	add     pc,  r2, #0				   @; 执行函数
@;	ldmfd   sp!, {r6}

	msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_SVC_MODE)
	ldmfd   sp!, {r0-r4}

	msr     cpsr_c, #(ARMV7_FIQ_MASK | ARMV7_IRQ_MASK | ARMV7_SYSTEM_MODE)

	ldr     r5, [sp], #4			   @; 从system的栈中取出数据送给r5
    msr     cpsr_cxsf, r5			   @; 还原到进入svc之前的状态

@;    ldmfd   r6!, {r0-r4}

    ldmfd   sp!, {r5-r12, pc}          @; pop new task's context





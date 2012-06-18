;/*
;**********************************************************************************************************************
;*											        eGon
;*						           the Embedded GO-ON Bootloader System
;*									       eGON arm boot sub-system
;*
;*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
;*                                           All Rights Reserved
;*
;* File    : cleanflushcache.s
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
/*
;==================================================================
; Define macro to get dcache line size
;==================================================================
*/
	.MACRO DCACHE_LINE_SIZE  REG, TMP
/*
;MRC     p15, 1, $TMP, c0, c0, 0     @read Cache Size ID Register
;AND     $TMP, $TMP, #7              @cache line size encoding
;MOV     $REG, #16                   @size offset
;MOV     $REG, $REG, lsl $TMP        @actual cache line size
*/
	MOV		\REG, #0x40
	.ENDM

	.globl ARMV7_FLUSH_ICACHE_REGION

ARMV7_FLUSH_ICACHE_REGION:

    STMFD   sp!, {r0-r12, lr}
    DCACHE_LINE_SIZE r2, r3
    SUB     r3, r2, #1
    BIC     r0, r0, r3
1:
    MCR     p15, 0, r0, c7, c11, 1      @clean D line to the point of unification
    DSB
    MCR     p15, 0, r0, c7, c5, 1       @invalidate I line
    ADD     r0, r0, r2
    CMP     r0, r1
    BLO     1b
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 6       @invalidate BTB
    DSB
    ISB
    LDMFD   sp!, {r0-r12, lr}
    MOV     pc, lr



	.globl  _FlushDCacheRegion

_FlushDCacheRegion:

    DCACHE_LINE_SIZE r2, r3
    sub     r3, r2, #1
    bic     r0, r0, r3
1:
    MCR     p15, 0, r0, c7, c14, 1      @clean & invalidate D / U line
    ADD     r0, r0, r2
    CMP     r0, r1
    BLO     1b
    DSB
    MOV     pc, lr

/*
;==================================================================
;------------------------------------------------------------------
; Clean DCache Region from start to end
; void ARMV7_CLEAN_DCACHE_REGION(void *start, void *end);
;------------------------------------------------------------------
;==================================================================
*/
	.globl	_CleanDCacheRegion

_CleanDCacheRegion:

    DCACHE_LINE_SIZE r2, r3
    SUB     r3, r2, #1
    BIC     r0, r0, r3
1:
    MCR     p15, 0, r0, c7, c10, 1      @clean D / U line
    ADD     r0, r0, r2
    CMP     r0, r1
    BLO     1b
    DSB
    MOV     pc, lr



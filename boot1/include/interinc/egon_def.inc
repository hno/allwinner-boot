;
;**********************************************************************************************************************
;*											        eGon
;*						           the Embedded GO-ON Bootloader System
;*									       eGON arm boot sub-system
;*
;*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
;*                                           All Rights Reserved
;*
;* File    : egon_def.inc
;*
;* By      : Jerry
;*
;* Version : V2.00
;*
;* Date	  :
;*
;* Descript:
;**********************************************************************************************************************
;
    IF :LNOT::DEF:__egon_def_inc__
	GBLA  __egon_def_inc__

    INCLUDE  egon_mem_distribute.inc


EGON2_DRAM_BASE       equ          0x40000000
EGON2_DRAM_SIZE       equ          0x02000000
EGON2_SRAM_BASE       equ          0x0
EGON2_SRAM_SIZE       equ          ( 32 * 1024 )
EGON2_RESET_BASE      equ          0xFFFF0000



BOOT0_BASE            equ          ( EGON2_SRAM_BASE                       )
BOOT1_BASE            equ          ( EGON2_DRAM_BASE  + EGON2_BOOT1_OFFSET )
FEL_BASE              equ          ( EGON2_RESET_BASE + 0x20               )






	ENDIF     ;;; IF :LNOT::DEF:__egon_def_inc__

	END       ;;; end of egon_def.inc

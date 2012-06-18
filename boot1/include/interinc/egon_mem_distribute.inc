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
    IF :LNOT::DEF:__egon_mem_distribute_inc__
	GBLA  __egon_mem_distribute_inc__


EGON2_BOOT1_OFFSET         equ         0x02400000


	ENDIF     ;;; IF :LNOT::DEF:__egon_mem_distribute_inc__

	END       ;;; end of egon_mem_distribute.inc

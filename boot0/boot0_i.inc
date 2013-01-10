;***********************************************************************************************************************
;                                                         eGON
;                                         the embedded GO-ON bootloader system
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;											       All Rights Reserved
;
; File Name : boot0_i.inc
;
; Author : Gary.Wang
;
; Version : 1.0
;
; Date : 2007.12.18
;
; Description : This file provides some arguments to configure Boot(Boot0 and Boot1) for assembly
;             language file. This file is very similar to file "boot0_i.h"; the two files
;             should be modified at the same time to keep coherence of configuring information.
;
; Others : None at present.
;
;
; History :
;
;  <Author>        <time>       <version>      <description>
;
; Gary.Wang       2007.12.18      1.1.0         build the file
;
;************************************************************************************************************************
    IF :LNOT::DEF:__boot0_i_inc
	GBLA  __boot0_i_inc


	GBLA    USE_PHYSICAL_ADDRESS


    INCLUDE    ./../boot1/include/egon2.inc


EGON2_SRAM_BASE    EQU    0
EGON2_SRAM_SIZE    EQU    ( 32 * 1024 )

BOOT0_STACK_BOTTOM     EQU     ( EGON2_SRAM_BASE + EGON2_SRAM_SIZE )



	ENDIF     ;;; IF :LNOT::DEF:__boot0_i_inc

	END       ;;; end of boot0_i.inc

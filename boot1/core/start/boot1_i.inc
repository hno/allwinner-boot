;***********************************************************************************************************************
;                                                         eGON
;                                         the embedded GO-ON bootloader system
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;											       All Rights Reserved
;
; File Name : boot1_i.inc
;
; Author : Gary.Wang
;
; Version : 1.0
;
; Date : 2007.12.18
;
; Description : This file provides some arguments to configure Boot(boot1 and Boot1) for assembly
;             language file. This file is very similar to file "boot1_i.h"; the two files
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
    IF :LNOT::DEF:__boot1_i_inc
	GBLA  __boot1_i_inc


	GBLA    USE_PHYSICAL_ADDRESS

	INCLUDE    egon2.inc
	INCLUDE    egon_def.inc


BOOT_SYSTEM_STACK_BOTTOM  EQU     ( BOOT1_BASE               + 0x00300000 )
BOOT_IRQ_STACK_BOTTOM     EQU     ( BOOT_SYSTEM_STACK_BOTTOM - 0x00080000 )
BOOT_SVC_STACK_BOTTOM     EQU     ( BOOT_IRQ_STACK_BOTTOM    - 0x00080000 )




	ENDIF     ;;; IF :LNOT::DEF:__boot1_i_inc

	END       ;;; end of boot1_i.inc

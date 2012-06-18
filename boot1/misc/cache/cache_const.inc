;/*
;**********************************************************************************************************************
;*											        eGon
;*						           the Embedded GO-ON Bootloader System
;*									       eGON cache sub-system
;*
;*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
;*                                           All Rights Reserved
;*
;* File    : chahe.inc
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
        if :LNOT::DEF:_CACHE_CONST_INC_
	    GBLA          _CACHE_CONST_INC_


EBSP_CPU_ARM920T            equ 100
EBSP_CPU_ARM922T            equ 101
EBSP_CPU_ARM926EJ_S         equ 102
EBSP_CPU_ARM940T            equ 103
EBSP_CPU_ARM946E_S          equ 104
EBSP_CPU_ARM1022E           equ 105
EBSP_CPU_ARM1026EJ_S        equ 106
EBSP_CPU_SA_110             equ 107
EBSP_CPU_XSCALE             equ 108
EBSP_CPU_ARM720T            equ 109
EBSP_CPU_AC320              equ 110

EBSP_CPU                    equ EBSP_CPU_ARM926EJ_S

 	    endif     ;//  IF :LNOT::DEF:_CACHE_CONST_INC_
 	    end


;/*
;*********************************************************************************************************
;*											        ePOS
;*						           the Easy Portable/Player Operation System
;*									           epos headfile
;*
;*						        (c) Copyright 2006-2007, Steven.ZGJ China
;*											All	Rights Reserved
;*
;* File    : ebios.inc
;* By      : Steven.ZGJ
;* Version : V0.1
;*
;* note：所有的模块内部定义的全局性的常量，都需要添加"模块名_xxx", 比如"MEMS_xxx", "EXEC_xxx"
;************************************************************************************************************************
;*/

    if :LNOT::DEF:_EBIOS_INC_
	GBLA  _EBIOS_INC_


    include  arch\\hd_scrpt.inc
    include  arch\\ccmu.inc
    include  arch\\pio.inc
    include  arch\\intc.inc


    ENDIF

    end         ;//_EBIOS_INC_


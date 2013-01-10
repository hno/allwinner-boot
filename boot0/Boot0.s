;***********************************************************************************************************************
;                                                         eGON
;                                         the Embedded GO-ON Bootloader System
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;                                                  All Rights Reserved
;
; File Name : Boot0.s
;
; Author : Gary.Wang
;
; Version : 1.1.0
;
; Date : 2007.10.11
;
; Description :
;
; Functions list : none.
;
; Others : None at present.
;
;
; History :
;
;  <Author>        <time>       <version>      <description>
;
; Gary.Wang       2007.10.07      1.1.0        build the file
;
; Gary.Wang       2007.10.10      1.1.0        delete "interrupt vector table"
;
; Gary.Wang       2007.10.11      1.1.0        add "boot_file_head_t"
;
;***********************************************************************************************************************

        INCLUDE    boot0_i.inc

    	IMPORT   Boot0_C_part
    	IMPORT   set_pll


		PRESERVE8


        AREA  init, CODE, READONLY
        CODE32
		ENTRY



;/*********************************the begin of initializing system********************************/



initialize


	mrs r0, cpsr
	bic r0, r0, #ARMV7_MODE_MASK
	orr r0, r0, #ARMV7_SVC_MODE
	orr r0, r0, #( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )    ;// After reset, ARM automaticly disables IRQ and FIQ, and runs in SVC mode.
	bic r0, r0, #ARMV7_CC_E_BIT                         ;// set little-endian
	msr cpsr_c, r0


;// configure memory system : disable MMU,cache and write buffer; set little_endian;
	mrc p15, 0, r0, c1, c0
	bic r0, r0, #( ARMV7_C1_M_BIT | ARMV7_C1_C_BIT )  ;// disable MMU, data cache
	bic r0, r0, #( ARMV7_C1_I_BIT | ARMV7_C1_Z_BIT )  ;// disable instruction cache, disable flow prediction
	bic r0, r0, #( ARMV7_C1_A_BIT)                    ;// disable align
	mcr p15, 0, r0, c1, c0

;// set SP for C language
	ldr sp, =BOOT0_STACK_BOTTOM

	bl  set_pll
;/**********************************the end of initializing system*********************************/
	bl  Boot0_C_part

 	b .                                 ;// infinite loop



	END

;///**********************************************************************************************************************
;//*                                                     sun define
;//*                            SUN CPU hardware registers, memory, interrupts, ... define
;//*
;//*                             Copyright(C), 2009-2010, ulive
;//*											       All Rights Reserved
;//*
;//* File Name : sunii.inc
;//*
;//* Author : kevin.z
;//*
;//* Version : 1.1.0
;//*
;//* Date : 2009-9-7 11:08
;//*
;//* Description : This file provides some defination of suni hardware registers, memory, interrupt
;//*             and so on. This file is very similar to file "sunii.h"; the two files should be
;//*             modified at the same time to keep coherence of information.
;//*
;//* Others : None at present.
;//*
;//*
;//* History :
;//*
;//*  <Author>        <time>       <version>      <description>
;//*
;//* kevin.z       2009-9-7 11:09    1.1.0        build the file
;//**********************************************************************************************************************/
	IF :LNOT::DEF:_SUNII_INC_
	GBLA  _SUNII_INC_

;///*
;//*********************************************************************************************************
;//*   hardware registers base define
;//*********************************************************************************************************
;//*/


REGS_pBASE		EQU    0x01C00000	   ;//寄存器物理地址
REGS_pSIZE      EQU    0x00300000      ;//寄存器物理空间大小
DRAM_pBASE      EQU    0x80000000
SRAM_pBASE      EQU    0x00000000
SRAM_SIZE       EQU    (32 * 1024)


;// 物理地址
SRAM_REGS_BASE         EQU     ( REGS_pBASE + 0x00000 )    ;//SRAM controller
DRAM_REGS_BASE         EQU     ( REGS_pBASE + 0x01000 )    ;//SDRAM/DDR controller
DMAC_REGS_BASE         EQU     ( REGS_pBASE + 0x02000 )    ;//DMA controller
NAFC_REGS_BASE         EQU     ( REGS_pBASE + 0x03000 )    ;//nand flash controller
TSC_REGS_BASE          EQU     ( REGS_pBASE + 0x04000 )    ;//transport stream interface
SPIC0_REGS_BASE        EQU     ( REGS_pBASE + 0x05000 )    ;//spi
SPIC1_REGS_BASE        EQU     ( REGS_pBASE + 0x06000 )    ;//spi
MSHC_REGS_BASE         EQU     ( REGS_pBASE + 0x07000 )    ;//ms
CSIC_REGS_BASE         EQU     ( REGS_pBASE + 0x09000 )    ;//csi controller
TVEC_REGS_BASE         EQU     ( REGS_pBASE + 0x0a000 )    ;//tv
EMAC_REGS_BASE         EQU     ( REGS_pBASE + 0x0b000 )    ;//emac
LCDC_REGS_BASE         EQU     ( REGS_pBASE + 0x0c000 )    ;//lcd
MACC_REGS_BASE         EQU     ( REGS_pBASE + 0x0e000 )    ;//media accelerate
SDMC0_REGS_BASE        EQU     ( REGS_pBASE + 0x0f000 )    ;//sdmmc0 controller
SDMC1_REGS_BASE        EQU     ( REGS_pBASE + 0x10000 )    ;//sdmmc1 controller
SDMC2_REGS_BASE        EQU     ( REGS_pBASE + 0x11000 )    ;//sdmmc2 controller
SDMC3_REGS_BASE        EQU     ( REGS_pBASE + 0x12000 )    ;//sdmmc3 controller
USBC0_REGS_BASE        EQU     ( REGS_pBASE + 0x13000 )    ;//usb/otg 0 controller
USBC1_REGS_BASE        EQU     ( REGS_pBASE + 0x14000 )    ;//usb/otg 1 controller
CCMU_REGS_BASE         EQU     ( REGS_pBASE + 0x20000 )    ;//clock manager unit
INTC_REGS_BASE         EQU     ( REGS_pBASE + 0x20400 )    ;//arm interrupt controller
PIOC_REGS_BASE         EQU     ( REGS_pBASE + 0x20800 )    ;//general perpose I/O
TMRC_REGS_BASE         EQU     ( REGS_pBASE + 0x20c00 )    ;//timer
UART0_REGS_BASE        EQU     ( REGS_pBASE + 0x21000 )    ;//uart0
UART1_REGS_BASE        EQU     ( REGS_pBASE + 0x21400 )    ;//uart1
UART2_REGS_BASE        EQU     ( REGS_pBASE + 0x21800 )    ;//uart2
UART3_REGS_BASE        EQU     ( REGS_pBASE + 0x21c00 )    ;//uart3
SPDIF_REGS_BASE        EQU     ( REGS_pBASE + 0x22000 )    ;//SPDIF interface
PS2_REGS_BASE          EQU     ( REGS_pBASE + 0x22400 )    ;//ps/2 interface
AC97_REGS_BASE         EQU     ( REGS_pBASE + 0x22800 )    ;//AC97 interface
IRCC_REGS_BASE         EQU     ( REGS_pBASE + 0x22c00 )    ;//fir
I2SC_REGS_BASE         EQU     ( REGS_pBASE + 0x23000 )    ;//i2s
LRAC_REGS_BASE         EQU     ( REGS_pBASE + 0x23400 )    ;//lradc
ADDA_REGS_BASE         EQU     ( REGS_pBASE + 0x23c00 )    ;//AD/DA
TWIC0_REGS_BASE        EQU     ( REGS_pBASE + 0x24000 )    ;//twi0
TWIC1_REGS_BASE        EQU     ( REGS_pBASE + 0x24400 )    ;//twi1
TPC_REGS_BASE          EQU     ( REGS_pBASE + 0x24800 )    ;//touch panel controller
DISE_REGS_BASE         EQU     ( REGS_pBASE + 0x200000)    ;//display engine



	ENDIF
	END
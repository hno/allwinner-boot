;/**********************************************************************************************************************
;*                                                  PIOC BSP for sun
;*                                 PIOC hardware registers definition and BSP interfaces
;*
;*                             Copyright(C), 2006-2008, ULIVE
;*											       All Rights Reserved
;*
;* File Name : pioc.inc
;*
;* Author : Gary.Wang
;*
;* Version : 1.1.0
;*
;* Date : 2008.05.23
;*
;* Description : This file provides some definition of PIOC's hardware registers and BSP interfaces.
;*             This file is very similar to file "pioc.h"; the two files should be modified at the
;*             same time to keep coherence of information.
;*
;* Others : None at present.
;*
;*
;* History :
;*
;*  <Author>        <time>       <version>      <description>
;*
;* Gary.Wang      2008.05.23       1.1.0        build the file
;*
;**********************************************************************************************************************/
	IF :LNOT::DEF:_PIOC_INC_
	GBLA  _PIOC_INC_


	INCLUDE    hd_scrpt.inc

;/*
;*********************************************************************************************************
;*   PIO Controller define          < maintained by Gary >
;*********************************************************************************************************
;*/

  ;/* offset */
PIOC_REG_o_A_EN             EQU    0x00
PIOC_REG_o_A_OUTPUT_CTRL    EQU    0x04
PIOC_REG_o_A_INPUT_CRTL     EQU    0x08
PIOC_REG_o_A_DATA           EQU    0x0C
PIOC_REG_o_A_MULDRV         EQU    0x10
PIOC_REG_o_A_PULL           EQU    0x14
PIOC_REG_o_A_MUP            EQU    0x18
PIOC_REG_o_B_EN             EQU    0x1C
PIOC_REG_o_B_OUTPUT_CTRL    EQU    0x20
PIOC_REG_o_B_INPUT_CRTL     EQU    0x24
PIOC_REG_o_B_DATA           EQU    0x28
PIOC_REG_o_B_MULDRV         EQU    0x2C
PIOC_REG_o_B_PULL           EQU    0x30
PIOC_REG_o_B_MUP            EQU    0x34
PIOC_REG_o_C_EN             EQU    0x38
PIOC_REG_o_C_OUTPUT_CTRL    EQU    0x3C
PIOC_REG_o_C_INPUT_CRTL     EQU    0x40
PIOC_REG_o_C_DATA           EQU    0x44
PIOC_REG_o_IO_MUP           EQU    0x48
PIOC_REG_o_IO_MULDRV        EQU    0x4C
PIOC_REG_o_IO_PULL          EQU    0x50
  ;/* registers */
PIOC_REG_A_EN               EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_EN          )
PIOC_REG_A_OUTPUT_CTRL      EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_OUTPUT_CTRL )
PIOC_REG_A_INPUT_CRTL       EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_INPUT_CRTL  )
PIOC_REG_A_DATA             EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_DATA        )
PIOC_REG_A_MULDRV           EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_MULDRV      )
PIOC_REG_A_PULL             EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_PULL        )
PIOC_REG_A_MUP              EQU    ( PIOC_REGS_BASE + PIOC_REG_o_A_MUP         )
PIOC_REG_B_EN               EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_EN          )
PIOC_REG_B_OUTPUT_CTRL      EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_OUTPUT_CTRL )
PIOC_REG_B_INPUT_CRTL       EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_INPUT_CRTL  )
PIOC_REG_B_DATA             EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_DATA        )
PIOC_REG_B_MULDRV           EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_MULDRV      )
PIOC_REG_B_PULL             EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_PULL        )
PIOC_REG_B_MUP              EQU    ( PIOC_REGS_BASE + PIOC_REG_o_B_MUP         )
PIOC_REG_C_EN               EQU    ( PIOC_REGS_BASE + PIOC_REG_o_C_EN          )
PIOC_REG_C_OUTPUT_CTRL      EQU    ( PIOC_REGS_BASE + PIOC_REG_o_C_OUTPUT_CTRL )
PIOC_REG_C_INPUT_CRTL       EQU    ( PIOC_REGS_BASE + PIOC_REG_o_C_INPUT_CRTL  )
PIOC_REG_C_DATA             EQU    ( PIOC_REGS_BASE + PIOC_REG_o_C_DATA        )
PIOC_REG_IO_MUP             EQU    ( PIOC_REGS_BASE + PIOC_REG_o_IO_MUP        )
PIOC_REG_IO_MULDRV          EQU    ( PIOC_REGS_BASE + PIOC_REG_o_IO_MULDRV     )
PIOC_REG_IO_PULL            EQU    ( PIOC_REGS_BASE + PIOC_REG_o_IO_PULL       )
  ;/* bit position */
    ;/* PIOC_REG_A_EN bit position */
PIOC_BP_SPI0_CS0_EN         EQU    27
PIOC_BP_SPI0_CK_EN          EQU     2
PIOC_BP_SPI0_MOSI_EN        EQU     4
PIOC_BP_SPI0_MISO_EN        EQU     3
   ;/* PIOC_REG_B_EN bit position */
PIOC_BP_TWI0_SCK_EN         EQU     6
PIOC_BP_TWI0_SDA_EN         EQU     7
  ;/* bit field mask */
PIOC_BITS_1                 EQU    0x01
PIOC_BITS_2                 EQU    0x03
PIOC_BITS_3                 EQU    0x07
PIOC_BITS_4                 EQU    0x0F
	;/* PIOC_REG_A_EN bit field mask */
PIOC_MASK_SPI0_CS0_EN       EQU    ( PIOC_BITS_1 << PIOC_BP_SPI0_CS0_EN  )
PIOC_MASK_SPI0_CK_EN        EQU    ( PIOC_BITS_1 << PIOC_BP_SPI0_CK_EN   )
PIOC_MASK_SPI0_MOSI_EN      EQU    ( PIOC_BITS_1 << PIOC_BP_SPI0_MOSI_EN )
PIOC_MASK_SPI0_MISO_EN      EQU    ( PIOC_BITS_1 << PIOC_BP_SPI0_MISO_EN )
	;/* PIOC_REG_B_EN bit field mask */
PIOC_MASK_TWI0_SCK_EN       EQU    ( PIOC_BITS_1 << PIOC_BP_TWI0_SCK_EN  )
PIOC_MASK_TWI0_SDA_EN       EQU    ( PIOC_BITS_1 << PIOC_BP_TWI0_SDA_EN  )


	ENDIF     ;;; IF :LNOT::DEF:__PIOC_INC
	END       ;;; end of pioc.inc
;*********************************************************************************************************************
;                                                    CCMU  FOR  1620
;                                             CCMU hardware registers definition
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;											       All Rights Reserved
;
; File Name   : ccmu.inc
;
; Author      : Jerry.Wang
;
; Version     : 1.1.0
;
; Date        : 2011-1-22 14:13:17
;
;* Description : This file provides some definition of CCMU's hardware registers and BSP interfaces.
;*             This file is very similar to file "ccmu.h"; the two files should be modified at the
;*             same time to keep coherence of information.
;
; Others      : None at present.
;
;
; History     :
;
;  <Author>        <time>       <version>      <description>
;
; Jerry.Wang      2011-1-22       1.1.0        build the file
;
;***********************************************************************************************************************
	IF :LNOT::DEF:_CCMU_INC_
	GBLA  _CCMU_INC_



;/*
;*********************************************************************************************************
;*   Clock Control Manage Uint define
;*********************************************************************************************************
;*/


;/* Offset */
CCMU_REG_o_PLL1_CTRL            EQU          0x00
CCMU_REG_o_PLL1_TUNING          EQU          0x04
CCMU_REG_o_PLL2_CTRL            EQU          0x08
CCMU_REG_o_PLL2_TUNING          EQU          0x0C
CCMU_REG_o_PLL3_CTRL            EQU          0x10
CCMU_REG_o_PLL3_TUNING          EQU          0x14
CCMU_REG_o_PLL4_CTRL            EQU          0x18
CCMU_REG_o_PLL4_TUNING          EQU          0x1C
CCMU_REG_o_PLL5_CTRL            EQU          0x20
CCMU_REG_o_PLL5_TUNING          EQU          0x24
CCMU_REG_o_PLL6_CTRL            EQU          0x28
CCMU_REG_o_PLL6_TUNING          EQU          0x2C
CCMU_REG_o_PLL7_CTRL            EQU          0x30
CCMU_REG_o_PLL7_TUNING          EQU          0x34
CCMU_REG_o_PLL1_TUNING2         EQU          0x38
CCMU_REG_o_PLL5_TUNING2         EQU          0x3C

CCMU_REG_o_PLTD                 EQU          0x4C
CCMU_REG_o_OSC24M				EQU          0x50
CCMU_REG_o_AHB_APB              EQU          0x54
CCMU_REG_o_APB1					EQU          0x58
CCMU_REG_o_AXI_MOD				EQU          0x5C
CCMU_REG_o_AHB_MOD0				EQU          0x60
CCMU_REG_o_AHB_MOD1				EQU          0x64
CCMU_REG_o_APB_MOD0				EQU          0x68
CCMU_REG_o_APB_MOD1				EQU          0x6C

CCMU_REG_o_NAND                 EQU          0x80
CCMU_REG_o_SD_MMC0              EQU          0x88
CCMU_REG_o_SD_MMC2              EQU          0x90
CCMU_REG_o_SPI0                 EQU          0xA0
CCMU_REG_o_SPI1                 EQU          0xA4


;/* registers */
CCMU_REG_PLL1_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL1_CTRL   )
CCMU_REG_PLL1_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL1_TUNING )
CCMU_REG_PLL2_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL2_CTRL   )
CCMU_REG_PLL2_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL2_TUNING )
CCMU_REG_PLL3_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL3_CTRL   )
CCMU_REG_PLL3_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL3_TUNING )
CCMU_REG_PLL4_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL4_CTRL   )
CCMU_REG_PLL4_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL4_TUNING )
CCMU_REG_PLL5_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL5_CTRL   )
CCMU_REG_PLL5_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL5_TUNING )
CCMU_REG_PLL6_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL6_CTRL   )
CCMU_REG_PLL6_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL6_TUNING )
CCMU_REG_PLL7_CTRL              EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL7_CTRL   )
CCMU_REG_PLL7_TUNING            EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL7_TUNING )
CCMU_REG_PLL1_TUNING2           EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL1_TUNING2)
CCMU_REG_PLL5_TUNING2           EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLL5_TUNING2)

CCMU_REG_PLTD                   EQU          ( CCMU_REGS_BASE + CCMU_REG_o_PLTD        )
CCMU_REG_OSC24M				    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_OSC24M	   )
CCMU_REG_AHB_APB                EQU          ( CCMU_REGS_BASE + CCMU_REG_o_AHB_APB     )
CCMU_REG_APB1				    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_APB1		   )
CCMU_REG_AXI_MOD			    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_AXI_MOD	   )
CCMU_REG_AHB_MOD0			    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_AHB_MOD0	   )
CCMU_REG_AHB_MOD1			    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_AHB_MOD1	   )
CCMU_REG_APB_MOD0			    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_APB_MOD0	   )
CCMU_REG_APB_MOD1			    EQU          ( CCMU_REGS_BASE + CCMU_REG_o_APB_MOD1	   )

CCMU_REG_NAND                   EQU          ( CCMU_REGS_BASE + CCMU_REG_o_NAND        )
CCMU_REG_SD_MMC0                EQU          ( CCMU_REGS_BASE + CCMU_REG_o_SD_MMC0     )
CCMU_REG_SD_MMC2                EQU          ( CCMU_REGS_BASE + CCMU_REG_o_SD_MMC2     )
CCMU_REG_SPI0                   EQU          ( CCMU_REGS_BASE + CCMU_REG_o_SPI0        )
CCMU_REG_SPI1                   EQU          ( CCMU_REGS_BASE + CCMU_REG_o_SPI1        )


;/* PLL1 COREPLL bit position */
CCMU_BP_PLL1_EN                 EQU            31
CCMU_BP_PLL1_CHG_PLL4           EQU            25
CCMU_BP_PLL1_DIVDER_P           EQU            16
CCMU_BP_PLL1_FACTOR_N           EQU             8
CCMU_BP_PLL1_FACTOR_K           EQU             4
CCMU_BP_PLL1_FACTOR_M           EQU             0
;/* CCMU_REG_OSC24M bit position */
CCMU_BP_LDO_KEY_FIELD           EQU            24
CCMU_BP_LDO_EN                  EQU            16
CCMU_BP_OSC2M_GSM               EQU             1
CCMU_BP_OSC2M_EN                EQU             0

    ;/* CCMU_REG_AHB_APB  bit position */
CCMU_BP_AC328_CLK_SRC           EQU            16
CCMU_BP_APB0_CLK_DIV            EQU             8
CCMU_BP_AHB_CLK_DIV             EQU             4
CCMU_BP_AXI_CLK_DIV             EQU             0

  	;/* CCMU_REG_AHB_MODULE0 bit position */
CCMU_BP_SPI1_AHB_CLK_GATE       EQU            21
CCMU_BP_SPI0_AHB_CLK_GATE       EQU            20
CCMU_BP_NAND_AHB_CLK_GATE       EQU            13
CCMU_BP_SDMMC2_AHB_CLK_GATE     EQU            10
CCMU_BP_SDMMC0_AHB_CLK_GATE     EQU             8
CCMU_BP_DMA_AHB_CLK_GATE        EQU             6
CCMU_BP_CSI_AHB_CLK_GATE        EQU             5

  	;/* CCMU_REG_APB_MODULE0 bit position */
CCMU_BP_PIO_APB_CLK_GATE        EQU             5



  ;/* bit field mask */
CCMU_BITS_0					  EQU    0x00
CCMU_BITS_1                   EQU    0x01
CCMU_BITS_2                   EQU    0x03
CCMU_BITS_3                   EQU    0x07
CCMU_BITS_4                   EQU    0x0F
CCMU_BITS_5                   EQU    0x1F
CCMU_BITS_6                   EQU    0x3F
CCMU_BITS_7                   EQU    0x7F
CCMU_BITS_8                   EQU    0xFF


   ;/* PLL1 COREPLL bit field mask */
CCMU_MASK_PLL1_EN             EQU   ( CCMU_BITS_1 << CCMU_BP_PLL1_EN        )
CCMU_MASK_PLL1_CHG_PLL4       EQU   ( CCMU_BITS_1 << CCMU_BP_PLL1_CHG_PLL4  )
CCMU_MASK_PLL1_DIVDER_P       EQU   ( CCMU_BITS_2 << CCMU_BP_PLL1_DIVDER_P  )
CCMU_MASK_PLL1_FACTOR_N       EQU   ( CCMU_BITS_5 << CCMU_BP_PLL1_FACTOR_N  )
CCMU_MASK_PLL1_FACTOR_K       EQU   ( CCMU_BITS_2 << CCMU_BP_PLL1_FACTOR_K  )
CCMU_MASK_PLL1_FACTOR_M       EQU   ( CCMU_BITS_2 << CCMU_BP_PLL1_FACTOR_M  )
   ;/* CCMU_REG_AC320_PLL bit field mask */
CCMU_MASK_LDO_KEY_FIELD       EQU   ( CCMU_BITS_8 << CCMU_BP_LDO_KEY_FIELD  )
CCMU_MASK_LDO_EN              EQU   ( CCMU_BITS_1 << CCMU_BP_LDO_EN         )
CCMU_MASK_OSC2M_GSM           EQU   ( CCMU_BITS_1 << CCMU_BP_OSC2M_GSM      )
CCMU_MASK_OSC2M_EN            EQU   ( CCMU_BITS_1 << CCMU_BP_OSC2M_EN       )

  	;/* CCMU_REG_AHB_APB bit field mask */
CCMU_MASK_AC328_CLK_SRC       EQU   ( CCMU_BITS_2 << CCMU_BP_AC328_CLK_SRC  )
CCMU_MASK_APB0_CLK_DIV        EQU   ( CCMU_BITS_2 << CCMU_BP_APB0_CLK_DIV   )
CCMU_MASK_AHB_CLK_DIV         EQU   ( CCMU_BITS_2 << CCMU_BP_AHB_CLK_DIV    )
CCMU_MASK_AXI_CLK_DIV         EQU   ( CCMU_BITS_2 << CCMU_BP_AXI_CLK_DIV    )

  	;/* CCMU_REG_AHB_MODULE bit field mask */
CCMU_MASK_SPI1_AHB_CLK_GATE    EQU  ( CCMU_BITS_1 << CCMU_BP_SPI1_AHB_CLK_GATE   )
CCMU_MASK_SPI0_AHB_CLK_GATE    EQU  ( CCMU_BITS_1 << CCMU_BP_SPI0_AHB_CLK_GATE   )
CCMU_MASK_NAND_AHB_CLK_GATE    EQU  ( CCMU_BITS_1 << CCMU_BP_NAND_AHB_CLK_GATE   )
CCMU_MASK_SDMMC2_AHB_CLK_GATE  EQU  ( CCMU_BITS_1 << CCMU_BP_SDMMC2_AHB_CLK_GATE )
CCMU_MASK_SDMMC0_AHB_CLK_GATE  EQU  ( CCMU_BITS_1 << CCMU_BP_SDMMC0_AHB_CLK_GATE )
CCMU_MASK_DMA_AHB_CLK_GATE     EQU  ( CCMU_BITS_1 << CCMU_BP_DMA_AHB_CLK_GATE    )

  	;/* CCMU_REG_APB_MODULE bit field mask */
CCMU_MASK_PIO_APB_CLK_GATE     EQU  ( CCMU_BITS_1 << CCMU_BP_PIO_APB_CLK_GATE   )




	ENDIF     ;;; IF :LNOT::DEF:__CCMU_INC
	END       ;;; end of ccmu.inc

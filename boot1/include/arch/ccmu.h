/*
**********************************************************************************************************************
*                                                    CCMU  FOR  1620
*                                             CCMU hardware registers definition
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : ccmu.h
*
* Author      : Jerry.Wang
*
* Version     : 1.1.0
*
* Date        : 2011-1-22 14:13:05
*
* Description : This file provides some definition of CCMU's hardware registers and BSP interfaces.
*             This file is very similar to file "ccmu.inc"; the two files should be modified at the
*             same time to keep coherence of information.
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Jerry.Wang      2011-1-22       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef _CCMU_H_
#define _CCMU_H_

#include  "hd_scrpt.h"

/* Offset */
#define CCMU_REG_o_PLL1_CTRL            0x00
#define CCMU_REG_o_PLL2_CTRL            0x08
#define CCMU_REG_o_PLL3_CTRL            0x10
#define CCMU_REG_o_PLL4_CTRL            0x18
#define CCMU_REG_o_PLL5_CTRL            0x20
#define CCMU_REG_o_PLL6_CTRL            0x28
#define CCMU_REG_o_PLL7_CTRL            0x30
#define CCMU_REG_o_PLL8_CTRL            0x38
#define CCMU_REG_o_MIPI_PLL_CTRL        0x40
#define CCMU_REG_o_PLL9_CTRL            0x44
#define CCMU_REG_o_PLL10_CTRL           0x48

#define CCMU_REG_o_AXI_MOD				0x50
#define CCMU_REG_o_AHB1_APB1			0x54
#define CCMU_REG_o_APB2_DIV				0x58
#define CCMU_REG_o_AXI_GATING			0x5C
#define CCMU_REG_o_AHB1_GATING0			0x60
#define CCMU_REG_o_AHB1_GATING1			0x64
#define CCMU_REG_o_APB1_GATING			0x68
#define CCMU_REG_o_APB2_GATING			0x6C

#define CCMU_REG_o_NAND0                0x80
#define CCMU_REG_o_NAND1                0x84
#define CCMU_REG_o_SD_MMC0              0x88
#define CCMU_REG_o_SD_MMC2              0x90

#define CCMU_REG_o_MBUS0                0x15c
#define CCMU_REG_o_MBUS1                0x160



/* registers */
#define CCMU_REG_PLL1_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL1_CTRL   )
#define CCMU_REG_PLL2_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL2_CTRL   )
#define CCMU_REG_PLL3_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL3_CTRL   )
#define CCMU_REG_PLL4_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL4_CTRL   )
#define CCMU_REG_PLL5_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL5_CTRL   )
#define CCMU_REG_PLL6_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL6_CTRL   )
#define CCMU_REG_PLL7_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL7_CTRL   )
#define CCMU_REG_PLL8_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL8_CTRL   )
#define CCMU_REG_PLL9_CTRL              __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL9_CTRL   )
#define CCMU_REG_PLL10_CTRL             __REG( CCMU_REGS_BASE + CCMU_REG_o_PLL10_CTRL  )
#define CCMU_REG_MIPI_PLL_CTRL          __REG( CCMU_REGS_BASE + CCMU_REG_o_MIPI_PLL_CTRL  )


#define CCMU_REG_AXI_MOD                __REG( CCMU_REGS_BASE + CCMU_REG_o_AXI_MOD     )
#define CCMU_REG_AHB1_APB1				__REG( CCMU_REGS_BASE + CCMU_REG_o_AHB1_APB1   )
#define CCMU_REG_APB2_DIV               __REG( CCMU_REGS_BASE + CCMU_REG_o_APB2_DIV    )
#define CCMU_REG_AXI_GATING				__REG( CCMU_REGS_BASE + CCMU_REG_o_AXI_GATING  )
#define CCMU_REG_AHB1_GATING0			__REG( CCMU_REGS_BASE + CCMU_REG_o_AHB1_GATING0)
#define CCMU_REG_AHB1_GATING1			__REG( CCMU_REGS_BASE + CCMU_REG_o_AHB1_GATING1)
#define CCMU_REG_APB1_GATING			__REG( CCMU_REGS_BASE + CCMU_REG_o_APB1_GATING )
#define CCMU_REG_APB2_GATING			__REG( CCMU_REGS_BASE + CCMU_REG_o_APB2_GATING )


#define CCMU_REG_NAND                   __REG( CCMU_REGS_BASE + CCMU_REG_o_NAND        )
#define CCMU_REG_SD_MMC0                __REG( CCMU_REGS_BASE + CCMU_REG_o_SD_MMC0     )
#define CCMU_REG_SD_MMC2                __REG( CCMU_REGS_BASE + CCMU_REG_o_SD_MMC2     )
#define CCMU_REG_SPI0                   __REG( CCMU_REGS_BASE + CCMU_REG_o_SPI0        )
#define CCMU_REG_SPI1                   __REG( CCMU_REGS_BASE + CCMU_REG_o_SPI1        )

#define CCMU_REG_MBUS0                  __REG( CCMU_REGS_BASE + CCMU_REG_o_MBUS0       )
#define CCMU_REG_MBUS1                  __REG( CCMU_REGS_BASE + CCMU_REG_o_MBUS1       )

#endif    // #ifndef _CCMU_H_

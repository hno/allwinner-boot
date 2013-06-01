/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/
/*
************************************************************************************************************************
*                                                  SRAM BSP for sun
*                                 SRAM hardware registers definition and BSP interfaces
*
*                             Copyright(C), 2006-2008, uLIVE
*											       All Rights Reserved
*
* File Name : sram.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2008.06.02
*
* Description : This file provides some definition of SRAM's hardware registers and BSP interfaces.
*             This file is very similar to file "sram.inc"; the two files should be modified at the
*             same time to keep coherence of information.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.06.02       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef _SRAM_H_
#define _SRAM_H_


#include "hd_scrpt.h"



/*
*********************************************************************************************************
*   SRAM Controller define          < maintained by Gary >
*********************************************************************************************************
*/
  /* offset */
#define SRAM_REG_o_CFG              0x00
#define SRAM_REG_o_TIMING_CTRL      0x04
#define SRAM_REG_o_GP_DATA          0x08
#define SRAM_REG_o_AC320_BIST0      0x0C
#define SRAM_REG_o_AC320_BIST1      0x10
#define SRAM_REG_o_BIST_CTRL        0x14
#define SRAM_REG_o_BIST_START_ADDR  0x18
#define SRAM_REG_o_BIST_END_ADDR    0x1C
#define SRAM_REG_o_CHIP_VERSION     0x20
#define SRAM_REG_o_AC320_P_EN       0x30
#define SRAM_REG_o_AC320_P_C_CNT    0x34
#define SRAM_REG_o_AC320_P_I_CNT    0x38
#define SRAM_REG_o_AC320_IC_CNT     0x3C
#define SRAM_REG_o_AC320_DC_CNT     0x40
#define SRAM_REG_o_AC320_ICH_CNT    0x44
#define SRAM_REG_o_AC320_DCH_CNT    0x48
#define SRAM_REG_o_AC320_C7         0x4C
#define SRAM_REG_o_TEST_DBG         0x60


/* address */
#define SRAM_REG_ADDR_CFG                ( SRAM_REGS_BASE + SRAM_REG_o_CFG             )
#define SRAM_REG_ADDR_TIMING_CTRL        ( SRAM_REGS_BASE + SRAM_REG_o_TIMING_CTRL     )
#define SRAM_REG_ADDR_GP_DATA            ( SRAM_REGS_BASE + SRAM_REG_o_GP_DATA         )
#define SRAM_REG_ADDR_AC320_BIST0        ( SRAM_REGS_BASE + SRAM_REG_o_AC320_BIST0     )
#define SRAM_REG_ADDR_AC320_BIST1        ( SRAM_REGS_BASE + SRAM_REG_o_AC320_BIST1     )
#define SRAM_REG_ADDR_BIST_CTRL          ( SRAM_REGS_BASE + SRAM_REG_o_BIST_CTRL       )
#define SRAM_REG_ADDR_BIST_START_ADDR    ( SRAM_REGS_BASE + SRAM_REG_o_BIST_START_ADDR )
#define SRAM_REG_ADDR_BIST_END_ADDR      ( SRAM_REGS_BASE + SRAM_REG_o_BIST_END_ADDR   )
#define SRAM_REG_ADDR_CHIP_VERSION       ( SRAM_REGS_BASE + SRAM_REG_o_CHIP_VERSION    )
#define SRAM_REG_ADDR_AC320_P_EN         ( SRAM_REGS_BASE + SRAM_REG_o_AC320_P_EN      )
#define SRAM_REG_ADDR_AC320_P_C_CNT      ( SRAM_REGS_BASE + SRAM_REG_o_AC320_P_C_CNT   )
#define SRAM_REG_ADDR_AC320_P_I_CNT      ( SRAM_REGS_BASE + SRAM_REG_o_AC320_P_I_CNT   )
#define SRAM_REG_ADDR_AC320_IC_CNT       ( SRAM_REGS_BASE + SRAM_REG_o_AC320_IC_CNT    )
#define SRAM_REG_ADDR_AC320_DC_CNT       ( SRAM_REGS_BASE + SRAM_REG_o_AC320_DC_CNT    )
#define SRAM_REG_ADDR_AC320_ICH_CNT      ( SRAM_REGS_BASE + SRAM_REG_o_AC320_ICH_CNT   )
#define SRAM_REG_ADDR_AC320_DCH_CNT      ( SRAM_REGS_BASE + SRAM_REG_o_AC320_DCH_CNT   )
#define SRAM_REG_ADDR_AC320_C7           ( SRAM_REGS_BASE + SRAM_REG_o_AC320_C7        )
#define SRAM_REG_ADDR_TEST_DBG           ( SRAM_REGS_BASE + SRAM_REG_o_TEST_DBG        )



/* registers */
#define SRAM_REG_CFG                __REG( SRAM_REG_ADDR_CFG             )
#define SRAM_REG_TIMING_CTRL        __REG( SRAM_REG_ADDR_TIMING_CTRL     )
#define SRAM_REG_GP_DATA            __REG( SRAM_REG_ADDR_GP_DATA         )
#define SRAM_REG_AC320_BIST0        __REG( SRAM_REG_ADDR_AC320_BIST0     )
#define SRAM_REG_AC320_BIST1        __REG( SRAM_REG_ADDR_AC320_BIST1     )
#define SRAM_REG_BIST_CTRL          __REG( SRAM_REG_ADDR_BIST_CTRL       )
#define SRAM_REG_BIST_START_ADDR    __REG( SRAM_REG_ADDR_BIST_START_ADDR )
#define SRAM_REG_BIST_END_ADDR      __REG( SRAM_REG_ADDR_BIST_END_ADDR   )
#define SRAM_REG_CHIP_VERSION       __REG( SRAM_REG_ADDR_CHIP_VERSION    )
#define SRAM_REG_AC320_P_EN         __REG( SRAM_REG_ADDR_AC320_P_EN      )
#define SRAM_REG_AC320_P_C_CNT      __REG( SRAM_REG_ADDR_AC320_P_C_CNT   )
#define SRAM_REG_AC320_P_I_CNT      __REG( SRAM_REG_ADDR_AC320_P_I_CNT   )
#define SRAM_REG_AC320_IC_CNT       __REG( SRAM_REG_ADDR_AC320_IC_CNT    )
#define SRAM_REG_AC320_DC_CNT       __REG( SRAM_REG_ADDR_AC320_DC_CNT    )
#define SRAM_REG_AC320_ICH_CNT      __REG( SRAM_REG_ADDR_AC320_ICH_CNT   )
#define SRAM_REG_AC320_DCH_CNT      __REG( SRAM_REG_ADDR_AC320_DCH_CNT   )
#define SRAM_REG_AC320_C7           __REG( SRAM_REG_ADDR_AC320_C7        )
#define SRAM_REG_TEST_DBG           __REG( SRAM_REG_ADDR_TEST_DBG        )

  /* bit field */
#define BF_CE0S_FORCED_EN           12
#define BF_CE0S_FORCED_HI_LOW       11

#define SRAM_MAT_CTL_PRIO           (0x01<<28)
#define SRAM_D_MAP_CTL_MASK         (0x03<<26)
#define SRAM_C_16K_MAP_CTL          (0x03<<24)
#define SRAM_C_8KH_MAP_CTL          (0x03<<22)
#define SRAM_C_8KL_MAP_CTL          (0x03<<20)
#define SRAM_B_256_4_MAP_CTL        (0x03<<18)
#define SRAM_B_256_3_MAP_CTL        (0x03<<16)
#define SRAM_B_256_2_MAP_CTL        (0x03<<14)
#define SRAM_B_256_1_MAP_CTL        (0x03<<12)
#define SRAM_B_1K_2_MAP_CTL         (0x03<<10)
#define SRAM_B_1K_1_MAP_CTL         (0x03<<08)
#define SRAM_B_2K_MAP_CTL           (0x03<<06)
#define SRAM_B_3KW_MAP_CTL          (0x03<<04)
#define SRAM_B_16KW_MAP_CTL         (0x03<<02)
#define SRAM_B_8KW_MAP_CTL          (0x03<<00)


#endif    // #ifndef _SRAM_H_

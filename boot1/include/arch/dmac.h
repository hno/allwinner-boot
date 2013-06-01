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
*                                                  DMAC BSP for sun
*                                 DMAC hardware registers definition and BSP interfaces
*
*                             Copyright(C), 2006-2008, uLIVE Co., Ltd.
*											       All Rights Reserved
*
* File Name : dmac.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2008.05.25
*
* Description : This file provides some definition of DMAC's hardware registers and BSP interfaces.
*             This file is very similar to file "dmac.inc"; the two files should be modified at the
*             same time to keep coherence of information.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.05.25       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef _DMAC_H_
#define _DMAC_H_

/*
*********************************************************************************************************
*   DMA Controller define          < maintained by Steve >
*********************************************************************************************************
*/

#define DMAC_DMATYPE_NORMAL         0
#define DMAC_DMATYPE_DEDICATED      1

/* port */
#define DMA_N0                      0
#define DMA_N1                      1
#define DMA_N2                      2
#define DMA_N3                      3
#define DMA_D0                      0
#define DMA_D1                      1
#define DMA_D2                      2
#define DMA_D3                      3
#define DMA_OFFSET                  0x20
  /* offset */
#define DMAC_REG_o_IRQ_EN           0x00
#define DMAC_REG_o_IRQ_PENDING      0x04
#define DMAC_REG_o_SYS_PRI          0x08
#define DMAC_REG_o_CFG              0x00
#define DMAC_REG_o_SRC_ADDR         0x04
#define DMAC_REG_o_DST_ADDR         0x08
#define DMAC_REG_o_BYTE_CNT         0x0C
#define DMAC_REG_o_PAGE_SIZE        0x10
#define DMAC_REG_o_PAGE_STEP        0x14
#define DMAC_REG_o_CMT_BLK          0x18
  /* registers */
#define DMAC_REG_IRQ_EN             __REG( DMAC_REGS_BASE + DMAC_REG_o_IRQ_EN                     )
#define DMAC_REG_IRQ_PENDING        __REG( DMAC_REGS_BASE + DMAC_REG_o_IRQ_PENDING                )
#define DMAC_REG_SYS_PRI            __REG( DMAC_REGS_BASE + DMAC_REG_o_SYS_PRI                    )
#define DMAC_REG_N0_CFG             __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N0_SRC_ADDR        __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N0_DST_ADDR        __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N0_BYTE_CNT        __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_N1_CFG             __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N1_SRC_ADDR        __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N1_DST_ADDR        __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N1_BYTE_CNT        __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_N2_CFG             __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N2_SRC_ADDR        __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N2_DST_ADDR        __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N2_BYTE_CNT        __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_N3_CFG             __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N3_SRC_ADDR        __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N3_DST_ADDR        __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N3_BYTE_CNT        __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )

#define DMAC_REG_D0_CFG             __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D0_SRC_ADDR        __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D0_DST_ADDR        __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D0_BYTE_CNT        __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D0_PAGE_SIZE       __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D0_PAGE_STEP       __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D0_CMT_BLK         __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D1_CFG             __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D1_SRC_ADDR        __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D1_DST_ADDR        __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D1_BYTE_CNT        __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D1_PAGE_SIZE       __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D1_PAGE_STEP       __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D1_CMT_BLK         __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D2_CFG             __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D2_SRC_ADDR        __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D2_DST_ADDR        __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D2_BYTE_CNT        __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D2_PAGE_SIZE       __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D2_PAGE_STEP       __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D2_CMT_BLK         __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D3_CFG             __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D3_SRC_ADDR        __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D3_DST_ADDR        __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D3_BYTE_CNT        __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D3_PAGE_SIZE       __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D3_PAGE_STEP       __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D3_CMT_BLK         __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )

#define DMAC_REG_D4_CFG             __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D4_SRC_ADDR        __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D4_DST_ADDR        __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D4_BYTE_CNT        __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D4_PAGE_SIZE       __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D4_PAGE_STEP       __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D4_CMT_BLK         __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D5_CFG             __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D5_SRC_ADDR        __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D5_DST_ADDR        __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D5_BYTE_CNT        __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D5_PAGE_SIZE       __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D5_PAGE_STEP       __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D5_CMT_BLK         __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D6_CFG             __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D6_SRC_ADDR        __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D6_DST_ADDR        __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D6_BYTE_CNT        __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D6_PAGE_SIZE       __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D6_PAGE_STEP       __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D6_CMT_BLK         __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D7_CFG             __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D7_SRC_ADDR        __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D7_DST_ADDR        __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D7_BYTE_CNT        __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D7_PAGE_SIZE       __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D7_PAGE_STEP       __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D7_CMT_BLK         __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
  /* registers' another defination */
#define DMAC_REG_N_CFG(port)        __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N_SRC_ADDR(port)   __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N_DST_ADDR(port)   __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N_BYTE_CNT(port)   __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D_CFG(port)        __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D_SRC_ADDR(port)   __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D_DST_ADDR(port)   __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D_BYTE_CNT(port)   __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D_PAGE_SIZE(port)  __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D_PAGE_STEP(port)  __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D_CMT_BLK(port)    __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )


#endif    // #ifndef _DMAC_H_

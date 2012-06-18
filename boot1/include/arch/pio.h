/*
************************************************************************************************************************
*                                                  PIOC BSP for sun
*                                 PIOC hardware registers definition and BSP interfaces
*
*                             Copyright(C), 2006-2008, uLIVE
*											       All Rights Reserved
*
* File Name : pio.h
*
* Author : Jerry
*
* Version : 1.1.0
*
* Date : 2008.05.23
*
* Description : This file provides some definition of PIOC's hardware registers and BSP interfaces.
*             This file is very similar to file "pioc.inc"; the two files should be modified at the
*             same time to keep coherence of information.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
*   Jerry         2008.05.23       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef _PIO_H_
#define _PIO_H_


#include "hd_scrpt.h"


/*
*********************************************************************************************************
*   PIO Controller define          < maintained by Jerry >
*********************************************************************************************************
*/
  /* offset */
#define PIOC_REG_o_OFFSET               0x04

#define PIOC_REG_o_CFG0                 0x00
#define PIOC_REG_o_CFG1                 0x04
#define PIOC_REG_o_CFG2                 0x08
#define PIOC_REG_o_CFG3                 0x0C
#define PIOC_REG_o_DATA                 0x10
#define PIOC_REG_o_DRV0                 0x14
#define PIOC_REG_o_DRV1                 0x18
#define PIOC_REG_o_PUL0                 0x1C
#define PIOC_REG_o_PUL1                 0x20
#define PIOC_REG_o_INT_CFG0             0x200
#define PIOC_REG_o_INT_CFG1             0x204
#define PIOC_REG_o_INT_CFG2             0x208
#define PIOC_REG_o_INT_CFG3             0x20C
#define PIOC_REG_o_INT_CTL              0x210
#define PIOC_REG_o_INT_STA              0x214
#define PIOC_REG_o_INT_DEB              0x218
#define PIOC_REG_o_PAD_DRV              0x220
#define PIOC_REG_o_PAD_PUL              0x224

#define GPIO_GROUP_A                    (0)
#define GPIO_GROUP_B                    (1)
#define GPIO_GROUP_C                    (2)
#define GPIO_GROUP_D                    (3)
#define GPIO_GROUP_E                    (4)
#define GPIO_GROUP_F                    (5)
#define GPIO_GROUP_G                    (6)
#define GPIO_GROUP_H                    (7)
#define GPIO_GROUP_I                    (8)

  /* registers */
#define PIOC_REG_CFG0(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_CFG0 )
#define PIOC_REG_CFG1(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_CFG1 )
#define PIOC_REG_CFG2(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_CFG2 )
#define PIOC_REG_CFG3(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_CFG3 )
#define PIOC_REG_DATA(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_DATA )
#define PIOC_REG_DRV0(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_DRV0 )
#define PIOC_REG_DRV1(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_DRV1 )
#define PIOC_REG_PUL0(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_PUL0 )
#define PIOC_REG_PUL1(port)             __REG( PIOC_REGS_BASE + 0x24 * port + PIOC_REG_o_PUL1 )
#define PIOC_REG_INT_CFG0               __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_CFG0           )
#define PIOC_REG_INT_CFG1               __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_CFG1           )
#define PIOC_REG_INT_CFG2               __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_CFG2           )
#define PIOC_REG_INT_CFG3               __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_CFG3           )
#define PIOC_REG_INT_CTL                __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_CTL            )
#define PIOC_REG_INT_STA                __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_STA            )
#define PIOC_REG_INT_DEB                __REG( PIOC_REGS_BASE + PIOC_REG_o_INT_DEB            )
#define PIOC_REG_PAD_DRV                __REG( PIOC_REGS_BASE + PIOC_REG_o_PAD_DRV            )
#define PIOC_REG_PAD_PUL                __REG( PIOC_REGS_BASE + PIOC_REG_o_PAD_PUL            )


#define PIO_REG_CFG(n, i)               ((unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24 + 0x10))

#define PIO_REG_CFG_VALUE(n, i)          __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00)
#define PIO_REG_DLEVEL_VALUE(n, i)       __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14)
#define PIO_REG_PULL_VALUE(n, i)         __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C)
#define PIO_REG_DATA_VALUE(n) 	         __REG( PIOC_REGS_BASE + ((n)-1)*0x24 + 0x10)

#define PIO_REG_BASE(n)                 ((unsigned int *)( PIOC_REGS_BASE + ((n)-1)*0x24))

#endif    // #ifndef _PIO_H_

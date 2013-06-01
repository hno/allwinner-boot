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
*                                                  TWIC BSP for SUN
*                                 TWIC hardware registers definition and BSP interfaces
*
*                             Copyright(C), 2006-2008, uLIVE
*											       All Rights Reserved
*
* File Name : twi.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2008.05.25
*
* Description : This file provides some definition of TWIC's hardware registers and BSP interfaces.
*             This file is very similar to file "twic.inc"; the two files should be modified at the
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
#ifndef _TWI_H_
#define _TWI_H_


#include "hd_scrpt.h"


/*
*********************************************************************************************************
*   TWI Controller define          < maintained by Gary >
*********************************************************************************************************
*/
  /* port */

/* Offset */
#define TWIC_REG_o_ADDR             0x00
#define TWIC_REG_o_XADDR            0x04
#define TWIC_REG_o_DATA             0x08
#define TWIC_REG_o_CTL              0x0C
#define TWIC_REG_o_STATUS           0x10
#define TWIC_REG_o_CLKR             0x14
#define TWIC_REG_o_SRST             0x18
#define TWIC_REG_o_EFR              0x1c

#define TWIC_REGS_BASE              TWIC0_REGS_BASE
/* registers */
#define TWIC_REG_ADDR               __REG(TWIC_REGS_BASE + TWIC_REG_o_ADDR  )
#define TWIC_REG_XADDR              __REG(TWIC_REGS_BASE + TWIC_REG_o_XADDR )
#define TWIC_REG_DATA               __REG(TWIC_REGS_BASE + TWIC_REG_o_DATA  )
#define TWIC_REG_CTL                __REG(TWIC_REGS_BASE + TWIC_REG_o_CTL   )
#define TWIC_REG_STATUS             __REG(TWIC_REGS_BASE + TWIC_REG_o_STATUS)
#define TWIC_REG_CLKR               __REG(TWIC_REGS_BASE + TWIC_REG_o_CLKR  )
#define TWIC_REG_SRST               __REG(TWIC_REGS_BASE + TWIC_REG_o_SRST  )
#define TWIC_REG_EFR                __REG(TWIC_REGS_BASE + TWIC_REG_o_EFR   )


/* status or interrupt source */
/*------------------------------------------------------------------------------
* Code   Status
* 00h    Bus error
* 08h    START condition transmitted
* 10h    Repeated START condition transmitted
* 18h    Address + Write bit transmitted, ACK received
* 20h    Address + Write bit transmitted, ACK not received
* 28h    Data byte transmitted in master mode, ACK received
* 30h    Data byte transmitted in master mode, ACK not received
* 38h    Arbitration lost in address or data byte
* 40h    Address + Read bit transmitted, ACK received
* 48h    Address + Read bit transmitted, ACK not received
* 50h    Data byte received in master mode, ACK transmitted
* 58h    Data byte received in master mode, not ACK transmitted
* 60h    Slave address + Write bit received, ACK transmitted
* 68h    Arbitration lost in address as master, slave address + Write bit received, ACK transmitted
* 70h    General Call address received, ACK transmitted
* 78h    Arbitration lost in address as master, General Call address received, ACK transmitted
* 80h    Data byte received after slave address received, ACK transmitted
* 88h    Data byte received after slave address received, not ACK transmitted
* 90h    Data byte received after General Call received, ACK transmitted
* 98h    Data byte received after General Call received, not ACK transmitted
* A0h    STOP or repeated START condition received in slave mode
* A8h    Slave address + Read bit received, ACK transmitted
* B0h    Arbitration lost in address as master, slave address + Read bit received, ACK transmitted
* B8h    Data byte transmitted in slave mode, ACK received
* C0h    Data byte transmitted in slave mode, ACK not received
* C8h    Last byte transmitted in slave mode, ACK received
* D0h    Second Address byte + Write bit transmitted, ACK received
* D8h    Second Address byte + Write bit transmitted, ACK not received
* F8h    No relevant status information or no interrupt
*-----------------------------------------------------------------------------*/

#endif    // #ifndef _TWI_H_

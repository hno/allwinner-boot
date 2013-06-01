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
#ifndef _LRADCK_C_H_
#define _LRADCK_C_H_

#include "hd_scrpt.h"


#define   LRADCK_READ_REG(reg)            (reg)
#define   LRADCK_WRITE_REG(reg, val)      ((reg) = (val))
/*offset*/

#define  KEYC_o_CTRL               0x00
#define  KEYC_o_INT                0x04
#define  KEYC_o_INT_STATUS         0x08
#define  KEYC_o_DATA0              0x0c
#define  KEYC_o_DATA1              0x10

#define  KEYC_REG_CTRL            __REG(LRAC_REGS_BASE + KEYC_o_CTRL)
#define  KEYC_REG_INT             __REG(LRAC_REGS_BASE + KEYC_o_INT )
#define  KEYC_REG_INT_STATUS      __REG(LRAC_REGS_BASE + KEYC_o_INT_STATUS )
#define  KEYC_REG_DATA0           __REG(LRAC_REGS_BASE + KEYC_o_DATA0)
#define  KEYC_REG_DATA1           __REG(LRAC_REGS_BASE + KEYC_o_DATA1)


#endif  /* _LRADCK_C_H_ */

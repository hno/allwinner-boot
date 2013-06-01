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
#ifndef _TIMER_H_
#define _TIMER_H_

  /* base */
#define bTMRC_CTL(Nb)	__REG(TMRC_REGS_BASE + (Nb))

/* Registers */
#define TMRC_REG_CTRL           bTMRC_CTL(0x00)
#define TMRC_REG_STATUS         bTMRC_CTL(0x04)
#define TMRC_REG_TM0CTRL        bTMRC_CTL(0x10)
#define TMRC_REG_TM0IVALUE      bTMRC_CTL(0x14)
#define TMRC_REG_TM0CVALUE      bTMRC_CTL(0x18)

#define RTC_YMD_REG             bTMRC_CTL(0xC4)
#define RTC_HMS_REG             bTMRC_CTL(0xC8)

#define WATCHDOG_REG_CTRL       bTMRC_CTL(0x90)
#define WATCHDOG_REG_MODE       bTMRC_CTL(0x94)




#endif  /* _TMRC_H_ */


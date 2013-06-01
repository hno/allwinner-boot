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


#ifndef _EGON2_H_
#define _EGON2_H_

//#define  CONFIG_AW_FPGA_PLATFORM
//#define   BOOT1_DEBUG
#define   SCRIPT_INSTALL_EARLY
#define   SPEED_UP_BOOT
#define   CONFIG_LOAD_BOOTIMG

//#define  FORCE_BOOT_STANDBY
//#define  CONFIG_AW_HOMELET_PRODUCT


#include "types.h"
#include "arch.h"
#include "interinc/boot1_v2.h"
#include "eGon2_mod.h"
#include "eGon2_driver.h"
#include "eGon2_syscall.h"
#include "libc/eGon2_libc.h"

#define BOOT_LOGO_MEM_BASE            (0x46400000)
#define BOOT_LOGO_MEM_SIZE            (SZ_16M)

#endif	//_EGON2_H_




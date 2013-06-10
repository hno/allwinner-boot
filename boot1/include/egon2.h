/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									         eGON boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : eGon2.h
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
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




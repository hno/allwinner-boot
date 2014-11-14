/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : include.h
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
#ifndef  __INCLUDE_H__
#define  __INCLUDE_H__

#include "egon2.h"
#include "string.h"
#include "BootMain_Debug.h"
#include "common.h"
#include "Board.h"
#include "part_info//part_info.h"
#include "usb_device//usb_storage_init.h"

//#define BOOT_LOGO_MEM_BASE            (EGON2_DRAM_BASE + SZ_512M - SZ_64M - SZ_32M)
#define BOOT_LOGO_MEM_BASE            (0x46400000)
#define BOOT_LOGO_MEM_SIZE            (SZ_16M)

#define BOOT_USB_DETECT_DELAY_TIME    (1000)

#endif   //__INCLUDE_H__


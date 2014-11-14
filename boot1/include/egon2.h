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

//#define  CONFIG_SUN6I_FPGA
#undef   CONFIG_SUN6I_FPGA
#define  AW_FPGA_V4_PLATFORM

#include "types.h"
#include "arch.h"
#include "interinc/boot1_v2.h"
#include "eGon2_mod.h"
#include "eGon2_driver.h"
#include "eGon2_syscall.h"
#include "libc/eGon2_libc.h"


#endif	//_EGON2_H_




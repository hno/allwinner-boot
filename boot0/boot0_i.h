/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : boot0_i.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.13
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.09.13       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __boot0_i_h
#define  __boot0_i_h

#define  UBOOT_BASE  0x4a000000
//#define  CONFIG_AW_FPGA_PLATFORM
//#define  CONFIG_HOLD_SUPERSTANDBY_DATA_BY_PMU

#include "types.h"
#include "storage_media_cfg.h"
#include "boot0_v2.h"
#include "boot1_v2.h"
#include "boot0_misc.h"
#include "arch.h"
#include "drv/boot0_drv.h"
#include "sys_op/sys_op.h"
#include "lib/lib.h"
#include "nand_for_boot.h"
#include "load_Boot1_from_nand/load_Boot1_from_nand.h"
#include "load_boot1_from_sdmmc/load_boot1_from_sdmmc.h"



#endif     //  ifndef __boot0_i_h

/* end of boot0_i.h */

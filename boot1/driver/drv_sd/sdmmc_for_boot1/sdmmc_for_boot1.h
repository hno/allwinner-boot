/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod demo sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : sdmmc_for_boot0.c
*
* By      : Neil
*
* Version : V1.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#ifndef  _SDMMC_FOR_BOOT1_H_
#define  _SDMMC_FOR_BOOT1_H_

#include  "egon2.h"
#include "types.h"
//物理操作
extern __s32 SDMMC_PhyInit(__u32 card_no, __u32 speed_mode, __u32 line_sel);
extern __s32 SDMMC_PhyExit(__u32 card_no);

extern __s32 SDMMC_PhyRead     (__u32 start_sector, __u32 nsector, void *buf, __u32 card_no);
extern __s32 SDMMC_PhyWrite    (__u32 start_sector, __u32 nsector, void *buf, __u32 card_no);

//逻辑操作
extern __s32 SDMMC_LogicalInit(__u32 card_no, __s32 card_oofset, __u32 speed_mode, __u32 line_sel);
extern __s32 SDMMC_LogicalExit(__u32 card_no);

extern __s32 SDMMC_LogicalRead (__u32 start_sector, __u32 nsector, void *buf, __u32 card_no);
extern __s32 SDMMC_LogicalWrite(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no);

#endif   //_SDMMC_FOR_BOOT1_H_


/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, Allwinner Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : sdcard_op.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.12.13
*
* Description : This file provides some basic functions to operating Nand Flash Controller rather
*             than Nand Flash chips.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.12.13      1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  _SDMMC_FOR_BOOT_H_
#define  _SDMMC_FOR_BOOT_H_

/*
************************************************************************************************************************
*                       SPMMC_PhyInit
*
*Description: SDMMC的物理操作初始化
*
*Arguments  :   none
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_PhyInit(__u32 card_no, __u32 speed_mode, __u32 line_sel);
/*
************************************************************************************************************************
*                       SDMMC_PhyExit
*
*Description: SDMMC的物理操作退出
*
*Arguments  :   none
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_PhyExit (__u32 card_no);
/*
************************************************************************************************************************
*                       SDMMC_PhyRead
*
*Description: SDMMC的物理读数据
*
*Arguments  :   标准块设备操作
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_PhyRead (__u32 block, __u32 nblock, void *buf, __u32 card_no);
/*
************************************************************************************************************************
*                       SDMMC_PhyWrite
*
*Description: SDMMC的物理写数据
*
*Arguments  :   标准块设备操作
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_PhyWrite(__u32 block, __u32 nblock, void *buf, __u32 card_no);
/*
************************************************************************************************************************
*                       SDMMC_PhyInit
*
*Description: SDMMC的逻辑操作初始化
*
*Arguments  :   none
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_LogicalInit(__u32 card_no, __s32 card_offset, __u32 speed_mode, __u32 line_sel);
/*
************************************************************************************************************************
*                       SDMMC_LogicalExit
*
*Description: SDMMC的逻辑操作初始化
*
*Arguments  :   none
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_LogicalExit(__u32 card_no);
/*
************************************************************************************************************************
*                       SDMMC_LogicalRead
*
*Description: SDMMC的逻辑读数据
*
*Arguments  :   标准块设备操作
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_LogicalRead (__u32 block, __u32 nblock, void *buf, __u32 card_no);
/*
************************************************************************************************************************
*                       SDMMC_LogicalWrite
*
*Description: SDMMC的逻辑写数据
*
*Arguments  :   标准块设备操作
*
*Return     :   = 0      ok;
*               = -1     fail.
************************************************************************************************************************
*/
extern  __s32   SDMMC_LogicalWrite(__u32 block, __u32 nblock, void *buf, __u32 card_no);

extern  __s32 	SDMMC_PhyDiskSize(void);
extern  __s32 	SDMMC_LogicalDiskSize(void);

#endif /*_SDMMC_FOR_BOOT_H_*/

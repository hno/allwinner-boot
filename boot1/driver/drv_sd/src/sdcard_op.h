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
#ifndef  _SDCARD_OP_H_
#define  _SDCARD_OP_H_

#include  "egon2.h"
#include  "sdcard_bsp.h"

enum{ UNKNOWN = 0x0, SDMEM, MMC, SDIO, CEATA };

typedef struct{
	__u32 bus_width;	// bus width,1/4/8
	__u32 spec_ver;
	__u32 tran_speed;	// high speed (26MB/sec) or low speed
	__u32 type;			// card type, bit 0-3 indicate SDMEM/MMC/SDIO,bit 4 indicates if high speed mode,
						// bit 5 indicates if high capacity,bit6-bit31 reserved
	__u32 rca;			// card relative address
	__u32 cid[4];		// register CID value for sd mem or mmc
	__u32 csd[4];		// register CSD calue for sd mem or mmc
	__u32 scr[2];       // register SCR calue for sd mem or mmc
	__u32 volume; 		// card capacity,unit by sector(512 bytes)
}CARD_INFO_T;


extern  __s32 SDMMC_Init(__s32 card_no, __u32 sysclk, __u32 speed_mode, __u32 line_mode);
extern  __s32 SDMMC_Exit (__s32 gpio_no);
extern  __s32 SDMMC_Read (__u32 block, __u32 nblock, void *buf, __u32 card_no);
extern  __s32 SDMMC_Write(__u32 block, __u32 nblock, void *buf, __u32 card_no);

extern  __s32 SDMMC_GetDiskSize(void);

#endif /*_SDCARD_OP_H_*/

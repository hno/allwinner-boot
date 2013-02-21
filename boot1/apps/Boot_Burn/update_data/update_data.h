/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#ifndef __UPDATA_DATA_H__
#define __UPDATA_DATA_H__

#include  "include.h"

extern  __s32  update_flash_init(void);
extern  __s32  update_flash_exit(void);

extern  __s32  update_flash_read (__u32 nSectNum, __u32 nSectorCnt, void * pBuf);
extern  __s32  update_flash_write(__u32 nSectNum, __u32 nSectorCnt, void * pBuf);


#endif	/* __UPDATA_DATA_H__ */


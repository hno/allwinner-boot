/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : card_sprite_flash.h
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
#ifndef   __CARD_SPRITE_FLASH__
#define   __CARD_SPRITE_FLASH__


extern    int   update_boot0_info (void *buf0, char *buf);
extern    int   update_boot1_info (void *buf0, char *buf);

extern    int   sprite_flash_init (int *type);
extern    int   sprite_flash_exit (int type);

extern    int   sprite_flash_open (void);
extern    int   sprite_flash_close(void);
extern    int   sprite_flash_read (__u32 nSectNum,  __u32 nSectorCnt, void * pBuf);
extern    int   sprite_flash_write(__u32 nSectNum,  __u32 nSectorCnt, void * pBuf);


extern    int 	sprite_flash_hardware_scan(void *mbr_i,void *flash_info, int erase_flash);

extern    int   create_stdmbr(void *mbr_i);

#endif




/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON axf sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : mod_private
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
#ifndef	_MOD_PRIVATE_H_
#define	_MOD_PRIVATE_H_

#include  "include.h"

typedef int          ( * eGon2_dinit )  (void                                                                         );
typedef int          ( * eGon2_dexit )  (void                                                                         );
typedef unsigned int ( * eGon2_dopen )  (unsigned int  mid, void   * open_arg                                         );
typedef int          ( * eGon2_dclose)  (unsigned int  mid                                                            );
typedef unsigned int ( * eGon2_dread )  (void       *pdata, unsigned int size, unsigned int n  , unsigned int  mid    );
typedef unsigned int ( * eGon2_dwrite)  (const void *pdata, unsigned int size, unsigned int n  , unsigned int  mid    );
typedef int          ( * eGon2_dioctl)  (unsigned int  mid, unsigned int cmd ,   signed int aux, void         *pbuffer);
typedef int          ( * eGon2_dstandby)(unsigned int cmd , void         *pbuffer									  );


typedef struct eGon2_drv_head
{
	unsigned int    mid;
	eGon2_dinit    	init;
	eGon2_dexit  	exit;
	eGon2_dopen   	open;
	eGon2_dclose  	close;
	eGon2_dread   	read;
	eGon2_dwrite  	write;
	eGon2_dioctl  	ioctl;
	eGon2_dstandby  standby;
	int     	    reserved[4];
}eGon2_drv_head_t;

typedef struct eGon2_drv_manage
{
    long long           mid_bitmap;
    eGon2_drv_head_t    drv_manage[EMOD_COUNT_MAX];
}
eGon2_drv_manage_t;

typedef struct  eGon2_mod_hd
{
    unsigned int    mod_id;
    unsigned int    usr_open;
    unsigned int    usr_hd;
    unsigned int    reserved;
}
eGon2_mod_hd_t;

#endif  /* _MOD_PRIVATE_H_ */



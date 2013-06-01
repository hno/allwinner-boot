/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
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



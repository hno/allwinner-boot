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
#ifndef __UPDATA_DATA_H__
#define __UPDATA_DATA_H__

#include  "../card_sprite_i.h"


#ifndef SZ_FLAG
#define SZ_FLAG
#define SZ_512       0x00000200U
#define SZ_1K        0x00000400U
#define SZ_2K        0x00000800U
#define SZ_4K        0x00001000U
#define SZ_8K        0x00002000U
#define SZ_16K       0x00004000U
#define SZ_32K       0x00008000U
#define SZ_64K       0x00010000U
#define SZ_128K      0x00020000U
#define SZ_256K      0x00040000U
#define SZ_512K      0x00080000U
#define SZ_1M        0x00100000U
#define SZ_2M        0x00200000U
#define SZ_4M        0x00400000U
#define SZ_8M        0x00800000U
#define SZ_16M       0x01000000U
#define SZ_32M       0x02000000U
#define SZ_64M       0x04000000U
#define SZ_128M      0x08000000U
#define SZ_256M      0x10000000U
#define SZ_512M      0x20000000U
#define SZ_1G        0x40000000U
#define SZ_2G        0x80000000U
#define SZ_4G        0x0100000000ULL
#define SZ_8G        0x0200000000ULL
#define SZ_16G       0x0400000000ULL
#define SZ_32G       0x0800000000ULL
#define SZ_64G       0x1000000000ULL
#endif

extern  __s32  update_boot0(void *buf0, char *buf, int sprite_type);
extern  __s32  update_boot1(void *buf1, char *buf, int sprite_type);

extern  __s32  update_flash_init(int* type);
extern  __s32  update_flash_exit(int type);

extern  __s32  update_flash_open (__u32 sector_lo, __u32 sector_hi);
extern  __s32  update_flash_write(void *pbuf, __u32 len);
extern  __s32  update_flash_close(void);

extern  __s32  update_flash_read_ext(void *pbuf, __u32 len);
extern  __s32  update_flash_write_ext(void *pbuf, __u32 len);
extern  __s32  update_force_to_flash(void);

extern  __s32  erase_all_flash( void );


extern    int  update_flash_hardware_scan(void *mbr_i, void *flash_info, int erase_flash);


#endif	/* __UPDATA_DATA_H__ */


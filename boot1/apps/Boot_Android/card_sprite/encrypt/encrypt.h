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
#ifndef    __ENCRYPT_H__
#define    __ENCRYPT_H__

#include  "../card_sprite_i.h"

__s32  init_code(int encrypt_flag);
__s32  encode(void * ibuf, void * obuf, __u32 len);
__s32  decode(__u32 src_buf, __u32 dest_buf, __u32 len, __u32 *buf_addr);
__s32  exit_code(void);

#endif    //__ENCRYPT_H__

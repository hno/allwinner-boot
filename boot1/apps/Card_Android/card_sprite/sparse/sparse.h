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

#ifndef __SPARSE_H
#define __SPARSE_H

#include "sparse_format.h"

#define   ANDROID_FORMAT_UNKNOW    (0)
#define   ANDROID_FORMAT_BAD       (-1)
#define   ANDROID_FORMAT_DETECT    (1)


int unsparse_probe(char *source, __u32 length, __u32 flash_start);
int unsparse_dram_write(void *pbuf, int length);
int unsparse_deal(void);
unsigned int unsparse_checksum(void);

#endif /* __SPARSE_H */

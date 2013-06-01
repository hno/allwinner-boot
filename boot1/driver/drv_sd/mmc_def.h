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

#ifndef _MMC_DEF_H_
#define _MMC_DEF_H_

#include "egon2.h"
#include <string.h>

#define MMC_SUN6I
#define MAX_MMC_NUM			4
#define MMC_TRANS_BY_DMA
//#define MMC_DEBUG
#undef  MMC_DEBUG

#define MMC_REG_BASE		0x01c0f000
#define MMC_REG_FIFO_OS		(0x200)
#define CCMU_HCLKGATE0_BASE 	0x01c20060
#define CCMU_HCLKRST0_BASE 	0x01c202c0
#define CCMU_MMC0_CLK_BASE 	0x01c20088
#define CCMU_MMC2_CLK_BASE 	0x01c20090
#define CCMU_PLL5_CLK_BASE 	0x01c20090
#define CCMU_PLL6_CLK_BASE 	0x01c20028
#define __be32_to_cpu(x)	((0x000000ff&((x)>>24)) | (0x0000ff00&((x)>>8)) | 			\
							 (0x00ff0000&((x)<< 8)) | (0xff000000&((x)<<24)))

#ifndef NULL
#define NULL (void*)0
#endif

#ifdef MMC_DEBUG
#define mmcinfo(fmt...)	wlibc_uprintf("[mmc]: "fmt)
#define mmcdbg(fmt...)	wlibc_uprintf("[mmc]: "fmt)
#define mmcmsg(fmt...)	wlibc_uprintf(fmt)
#else
#define mmcinfo(fmt...)
#define mmcdbg(fmt...)
#define mmcmsg(fmt...)
#endif

#define readb(addr)		(*((volatile unsigned char  *)(addr)))
#define readw(addr)		(*((volatile unsigned short *)(addr)))
#define readl(addr)		(*((volatile unsigned long  *)(addr)))
#define writeb(v, addr)	(*((volatile unsigned char  *)(addr)) = (unsigned char)(v))
#define writew(v, addr)	(*((volatile unsigned short *)(addr)) = (unsigned short)(v))
#define writel(v, addr)	(*((volatile unsigned long  *)(addr)) = (unsigned long)(v))

extern void OSAL_CacheRangeFlush(void*Address, __u32 Length, __u32 Flags);

#endif /* _MMC_H_ */

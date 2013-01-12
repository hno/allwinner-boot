/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for general mmc operations
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */

#ifndef _MMC_DEF_H_
#define _MMC_DEF_H_

#include "boot0_i.h"
#include <string.h>

#define MMC_SUN7I
#define MAX_MMC_NUM			4
#define MMC_TRANS_BY_DMA
//#define MMC_DEBUG

#define MMC_REG_BASE		0x01c0f000
#define MMC_REG_FIFO_OS		(0x200)
#define CCMU_HCLKGATE0_BASE 	0x01c20060
#define CCMU_HCLKRST0_BASE 	0x01c202c0
#define CCMU_MMC0_CLK_BASE 	0x01c20088
#define CCMU_MMC2_CLK_BASE 	0x01c20090
#define CCMU_PLL5_CLK_BASE 	0x01c20020
#define __be32_to_cpu(x)	((0x000000ff&((x)>>24)) | (0x0000ff00&((x)>>8)) | 			\
							 (0x00ff0000&((x)<< 8)) | (0xff000000&((x)<<24)))

#ifndef NULL
#define NULL (void*)0
#endif

#ifdef MMC_DEBUG
#define mmcinfo(fmt...)	OSAL_printf("[mmc]: "fmt)
#define mmcdbg(fmt...)	OSAL_printf("[mmc]: "fmt)
#define mmcmsg(fmt...)	OSAL_printf(fmt)
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

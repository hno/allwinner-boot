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
#include "types.h"
#include "cache.h"


extern void _FlushICacheRegion     (void *adr, void *end);
extern void _FlushDCacheRegion     (void *adr, void *end);
extern void _CleanDCacheRegion     (void *adr, void *end);
extern void _CleanFlushDCacheRegion(void *adr, void *end);
extern void _CleanFlushCacheRegion (void *adr, void *end);

/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_DCacheEnable(void)
{
	__u32 reg_val;

	asm("mrc p15,0, %0, c1,c0,0" : "=r"(reg_val));
	reg_val |= 0x0004;
	asm("mcr p15,0, %0, c1,c0,0" : : "r"(reg_val));
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_DCacheDisable(void)
{
	__u32 reg_val;

	asm("mrc p15,0, %0, c1,c0,0" : "=r"(reg_val));
	reg_val &= ~0x0004;
	asm("mcr p15,0, %0, c1,c0,0" : : "r"(reg_val));
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_ICacheEnable(void)
{
	__u32 reg_val;

	asm("mrc p15,0, %0, c1,c0,0" : "=r"(reg_val));
	reg_val |= 0x1000;
	asm("mcr p15,0, %0, c1,c0,0" : : "r"(reg_val));
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_ICacheDisable(void)
{
	__u32 reg_val;

	asm("mrc p15,0, %0, c1,c0,0" : "=r"(reg_val));
	reg_val &= ~0x1000;
	asm("mcr p15,0, %0, c1,c0,0" : : "r"(reg_val));
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_CleanDCacheRegion(void *adr, __u32 bytes)
{
    _CleanDCacheRegion(adr, (void *)((unsigned int)adr + bytes));
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_CleanFlushDCacheRegion(void *adr, __u32 bytes)
{
    //_CleanDCacheRegion(adr, (void *)((unsigned int)adr + bytes));
	_FlushDCacheRegion(adr, (void *)((unsigned int)adr + bytes));
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void wlibc_FlushDCacheRegion(void *adr, __u32 bytes)
{
    _FlushDCacheRegion(adr, (void *)((unsigned int)adr + bytes));
}


void wlibc_CleanFlushCacheRegion(void *adr, __u32 bytes)
{
}

void wlibc_FlushCacheRegion(void *adr, __u32 bytes)
{
}

void wlibc_FlushICacheRegion(void *adr, __u32 bytes)
{
}



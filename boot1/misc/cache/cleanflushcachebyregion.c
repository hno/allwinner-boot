/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON cache sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : cleanflushcache.c
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



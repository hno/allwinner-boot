/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
/*
*******************************************************************************
*                     xDelay_Unit
*
* Description:
*    void
*
* Parameters:
*    CoreClk  :  input. 系统时钟频率. 单位 : Mhz
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
__u32 xDelay_Unit(__u32 CoreClk)
{
    return (CoreClk * 1000 * 1000) / 1000;
}
/*
*******************************************************************************
*                     xDelayMS
*
* Description:
*    void
*
* Parameters:
*    Milliseconds  :  input. 单位：毫秒
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
/*
void xDelayMS(__u32 Milliseconds)
{
    __u32 cnt = 1;

    cnt = Milliseconds * xDelay_Unit(240);
    while(cnt--);
}
*/

void  __msdelay( __u32 ms )
{
	wBoot_timer_delay(ms);
}


#define cache_line_size 			32
#define DEFAULT_CLOCK               25

void  CleanFlushDCacheRegion(__u32 addr, __u32 cnt)
{
	wlibc_CleanFlushDCacheRegion((void *)addr, cnt);
}


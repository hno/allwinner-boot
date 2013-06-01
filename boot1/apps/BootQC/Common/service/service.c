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


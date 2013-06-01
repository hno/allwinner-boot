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
#include "include.h"

static  volatile  unsigned  timerl = 0;

//使用TIMER5记录时间，对相关硬件进行初始化操作

//启动一个定时器计时
unsigned    timer_start(void)
{
    __u32 reg_val;
    __u32 i;

	*(unsigned int *)(0x01c20C00 + 0x6C) = 0;
	*(unsigned int *)(0x01c20C00 + 0x70) = 0;
	do
	{
		reg_val = ((*(unsigned int *)(0x01c20C00 + 0x60)) >> 4) & 0x01;
	}
	while(reg_val);
	for(i=0;i<50;i++);

	do
	{
		reg_val = ((*(unsigned int *)(0x01c20C00 + 0x60)) >> 5) & 0x01;
	}
	while(reg_val);

	timerl = *(unsigned int *)(0x01c20C00 + 0x6C);

    return 0;
}

//使用AVS记录时间，并获取从start开始到stop之间的时间，单位ms
unsigned   timer_stop(void)
{
    __u32 reg_val;

	do
	{
		reg_val = ((*(unsigned int *)(0x01c20C00 + 0x60)) >> 5) & 0x01;
	}
	while(reg_val);

	timerl = (*(unsigned int *)(0x01c20C00 + 0x6C)) - timerl;

	timerl/= 24000;

    return timerl;
}



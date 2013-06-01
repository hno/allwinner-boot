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
#include  "egon2.h"
#include  "BootMain.h"

const eGon2_mod_t modinfo =
{
    {'e','G','o','n','2','a','p','p'},		//MAGIC字符，用于标识是eGON2的驱动/应用代码
    0x01000000,                			//版本
    0x0,                                    	//模块的ID，每个elf都应该有一个模块ID，不论驱动还是应用，且各不相同
    BootMain,                          		//pcb里第一个任务的首地址
    0						//驱动函数列表，列出了驱动所必要的7个函数
};

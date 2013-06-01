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
#ifndef  __BOOTMAIN_DEBUG_H__
#define  __BOOTMAIN_DEBUG_H__

#define  BOOT_CONFIG_DEBUG_LEVEL   3

#if(BOOT_CONFIG_DEBUG_LEVEL == 0)
#define  DMSG_TEST(...)
#define  DMSG_INFO(...)
#define  DMSG_PANIC(...)
#elif(BOOT_CONFIG_DEBUG_LEVEL == 1)
#define  DMSG_TEST(...)
#define  DMSG_INFO(...)
#define  DMSG_PANIC			__wrn
#elif(BOOT_CONFIG_DEBUG_LEVEL == 2)
#define  DMSG_TEST(...)
#define  DMSG_INFO			__inf
#define  DMSG_PANIC			__wrn
#elif(BOOT_CONFIG_DEBUG_LEVEL == 3)
#define  DMSG_TEST			__inf
#define  DMSG_INFO			__inf
#define  DMSG_PANIC			__wrn
#endif

#define  sprite_malloc     wBoot_malloc
#define  sprite_free       wBoot_free

extern   void sprite_wrn(const char *str, ...);

#endif   //__BOOTMAIN_DEBUG_H__



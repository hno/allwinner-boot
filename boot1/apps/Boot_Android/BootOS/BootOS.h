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
#ifndef  __BOOTOS_H__
#define  __BOOTOS_H__

extern  void BootOS(unsigned int para_addr, unsigned int kernal_addr);
extern  int  BootOS_detect_os_type(unsigned *para_addr, unsigned *kernal_addr, void *os_info, int *logo_status);

extern  int  do_boot_linux (char *para_addr, char *cmdline, unsigned cmdline_length);

#endif   //__BOOTOS_H__


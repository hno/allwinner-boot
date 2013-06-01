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
#include "cpu_private.h"


void  set_vect_low_addr( void )
{
	__u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	//__asm{MRC p15,0,c1format,c1,c0,0}
	c1format &= ~ 0x00002000;
	c1format |= 0;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	//__asm{MCR p15,0,c1format,c1,c0,0}
}


void  set_vect_high_addr( void )
{
	__u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	//__asm{MRC p15,0,c1format,c1,c0,0}
	c1format &= ~ 0x00002000;
	c1format |= 0x00002000;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	//__asm{MCR p15,0,c1format,c1,c0,0}
}


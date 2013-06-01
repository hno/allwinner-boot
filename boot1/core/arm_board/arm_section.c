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
/*
**********************************************************************************************************************
*                                               move_RW
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void move_RW( void )
{
	;
}
/*
**********************************************************************************************************************
*                                               clear_ZI
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
extern int _bss_start, _bss_end;
extern int _standby_start, _standby_end;
extern int _arm_start, _arm_end;
extern int _standby_start_lma;

void clear_ZI( void )
{
	int *dst;

	for (dst = &_bss_start; dst< &_bss_end; dst++)
	{
		*dst = 0;
	}
}
/*
**********************************************************************************************************************
*                                               reposition_int_vect
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void  reposition_arm_start( void )
{
	int *dst = 0;
	int *src = &_arm_start;

	while (src < &_arm_end)
	{
		*dst++ = *src++;
	}
}
/*
**********************************************************************************************************************
*                                               reposition_boot_standby
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void  reposition_boot_standby( void )
{
	int *dst = &_standby_start;
	int *src = &_standby_start_lma;

	while (dst < &_standby_end)
	{
		*dst++ = *src++;
	}
}



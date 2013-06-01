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
__s32 eGon2_dynamic(void)
{
	char *base = (char *)BT1_head.prvt_head.script_buf;
	int   i;
	dynamic_data_form  *dform;
	dynamic_boot_head  *dhead;

	dhead = (dynamic_boot_head *)base;
	if(strncmp(dhead->magic, "dynamic", sizeof("dynamic")))
	{
		eGon2_printf("dynamic data magic error\n");

		return -1;
	}
	base += sizeof(dynamic_boot_head);
	for(i=0;i<dhead->count;i++)
	{
		dform = (dynamic_data_form *)base;
		eGon2_script_parser_patch_str("dynamic", dform->name, base + sizeof(dynamic_data_form), dform->usedsize>>2);
		base += dform->usedsize;
	}

	return 0;
}



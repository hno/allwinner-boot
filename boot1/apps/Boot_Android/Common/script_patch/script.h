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
#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#define   MAX_PATH                   256
#define   ITEM_MAIN_NAME_MAX          32

#define   DATA_COUNT_MAX         (4)
#define   DATA_TYPE_SINGLE_WORD  (1)
#define   DATA_TYPE_STRING       (2)
#define   DATA_TYPE_MULTI_WORD   (3)
#define   DATA_TYPE_GPIO         (4)

typedef struct _script_item
{
	char  item_name[ITEM_MAIN_NAME_MAX];           //主键名称
	int   item_length;                             //主键成员的个数
	int   item_offset;							   //主键成员开始的偏移量
}
script_item_t;

typedef struct _script_head
{
	int   item_num;
	int   version[3];
}
script_head_t;

extern int script_patch(char *script_name, void *script_buf, int boot_script_type);

#endif   //_SCRIPT_H_


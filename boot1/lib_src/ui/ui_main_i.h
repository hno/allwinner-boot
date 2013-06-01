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
#ifndef  _ui_main_I_H_
#define  _ui_main_I_H_

#include "egon2.h"
#include "string.h"

typedef struct _ui_main_source_t
{
	__u32   disp_hd;
	__u32   layer_hdl;
	int 	screen_width;
	int 	screen_height;
	int 	screen_size;
	__u32   color;
	int     this_x;
	int     this_y;
	char 	*screen_buf;
	__disp_layer_info_t  *layer_para;
}
ui_main_source_t;


extern  ui_main_source_t  ui_source;


#endif  /* _ui_main_I_H_ */

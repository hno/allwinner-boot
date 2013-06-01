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
#ifndef  __PARSE_PICTURE_H__
#define  __PARSE_PICTURE_H__

#include "types.h"

typedef struct tag_Picture
{
    void *Buffer;   		/* 存放图片数据     */
    __u32 BufferSize;   	/* buffer长度       */

    __u32 BitCount; 		/* 一个像素的bit数  */
    __u32 Width;    		/* 图片宽度         */
    __u32 Height;   		/* 图片高度         */
    __u32 RowSize;  		/* 图片一行的大小   */
}Picture_t;


int Parse_Pic_BMP_ByPath(char *Path, Picture_t *PictureInfo, unsigned address);

#endif   //__PARSE_PICTURE_H__


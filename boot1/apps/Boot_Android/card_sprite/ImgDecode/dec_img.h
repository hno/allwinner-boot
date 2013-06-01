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
#ifndef  __DEC_IMG_H__
#define  __DEC_IMG_H__

#include  "../card_sprite_i.h"

//------------------------------------------------------------------------------------------------------------
// image ½âÎö½Ó¿Ú
//------------------------------------------------------------------------------------------------------------
typedef void * 		HIMAGE;

typedef void * 		HIMAGEITEM;

extern HIMAGE 		Img_Open		(__u32 start_sector, int size);

extern HIMAGEITEM 	Img_OpenItem	(HIMAGE hImage, char * MainType, char * subType);

extern __u64		Img_GetItemSize	(HIMAGE hImage, HIMAGEITEM hItem);

extern __u32 		Img_ReadItemData(HIMAGE hImage, HIMAGEITEM hItem, void * buffer, __u64 Length);

extern __u32 		Img_CloseItem	(HIMAGE hImage, HIMAGEITEM hItem);

extern void  		Img_Close		(HIMAGE hImage);


#endif   //__DEC_IMG_H__


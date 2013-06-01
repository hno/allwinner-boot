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
//------------------------------------------------------------------------------------------------------------
//
//2009-04-18 16:02:07
//
//scott
//
//encode.h
//
// 基于rc6的加密算法模块
//
//------------------------------------------------------------------------------------------------------------

#ifndef __ENCODE___H__
#define __ENCODE___H__


#include  "../card_sprite_i.h"
//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

#define OK				0
#define NULL			0

#define ENCODE_LEN		16				//一次加密16个字节

typedef void * HENCODE;

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

HENCODE Initial(void * param0, void * param1, __u32 index);

#ifdef __RC_ENCODE__
__u32     Encode(HENCODE hEncode, void * ibuf, void * obuf);
#endif //__RC_ENCODE__

#if 1
__u32		Decode(HENCODE hEncode, void * ibuf, void * obuf);
#endif //__RC_DECODE__

__u32		UnInitial(HENCODE hEncode);

typedef HENCODE (*pInitial)(void * param0, void * param1, __u32 index);
typedef __u32	(*pEnDecode)(HENCODE hEncode, void * ibuf, void * obuf);
typedef __u32	(*pUnInitial)(HENCODE hEncode);
//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
typedef struct tag_RC_ENDECODE_IF
{
	HENCODE 	handle;
	pInitial	Initial;
	pEnDecode 	EnDecode;
	pUnInitial	UnInitial;
}RC_ENDECODE_IF_t;


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#endif //__ENCODE___H__



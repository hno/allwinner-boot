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
// twofish.cpp
//
// scott
//
// 2009-04-27 17:49:28
//
//------------------------------------------------------------------------------------------------------------

#ifndef ___TWO_FISH____CPP_____
#define ___TWO_FISH____CPP_____

#include "twofish.h"
#include "twofish_new.h"


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

//typedef void * HTF;

#define HTF_MAGIC	"TFMG"


TWI	twi_org;

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
HTF TFInitial(__u32 * key, __u32 keylen)
{
	TWI	*twi = &twi_org;

	if (NULL == key)
		return NULL;

	if (0 != (keylen %4))
		return NULL;

	switch (keylen)
	{
	case KEY_LEN_128_BIT://		(128 / 8)	//128 bit
		break;
	case KEY_LEN_192_BIT://		(192 / 8)	//192 bit
		break;
	case KEY_LEN_256_BIT://		(256 / 8)	//256 bit
		break;
	default:
		return NULL;
	}

	memset((void *)twi, 0, sizeof(TWI));

	twofish_new_set_key(twi, (__u32 *)key, keylen);

	return twi;
}

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

__u32 TFDecode(HTF hTF, void * ibuf, __u32 len, void * obuf)
{
	TWI	*twi = (TWI	*)hTF;
    //decode data
	__u32 left_len = len;
	__u32 this_len = 0;
	__u32 offset_len = 0;
	__u8  *iaddr, *oaddr;

    iaddr = (__u8 *)ibuf;
    oaddr = (__u8 *)obuf;
	//逐个分组进行处理
	while(left_len > 0)
	{
		this_len = (left_len > TF_SIZE)? (TF_SIZE) : (left_len);
		twofish_new_decrypt(twi, (__u32 *)(iaddr +  offset_len), (__u32 *)(oaddr + offset_len));
        offset_len += this_len;
		left_len -= this_len;
	}

	return 0;
}


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
__u32 TFUnInitial(HTF hTF)
{
	__u32 ret = 0;
	TWI	*twi = (TWI	*)hTF;

	if (NULL == twi)
		return __LINE__;

	memset(twi, 0, sizeof(TWI));
	twi = NULL;

	return ret;
}


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

#endif //___TWO_FISH____CPP_____



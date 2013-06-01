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
//2008-10-07 10:22:17
//
//scott
//
//snfile.h
//
//------------------------------------------------------------------------------------------------------------

#ifndef __SN_FILE____H____
#define __SN_FILE____H____

#include "../include/basetypes.h"

#define FLOAT_SIZE			sizeof(float)	//

//------------------------------------------------------------------------------------------------------------
// sn file name
//------------------------------------------------------------------------------------------------------------
//#define SN_FILE_NAME_LEN	128			//sn文件名长度


// sn file head is same with key file head
#define SN_FILE_HEAD_LEN	1024		//sn文件头区的最大长度


//------------------------------------------------------------------------------------------------------------
// sn head 0
//------------------------------------------------------------------------------------------------------------
#define SN_FILE_CRC_DEF		0x746f6373	//默认的crc值
#define SN_FILE_CRC_OFF		0			//crc数据偏移量
#define SN_FILE_CRC_LEN		4			//crc数据长度


#define SN_MAGIC			0x2e4e532e  //".SN."
#define SN_MAGIC_OFF		4			//magic数据偏移量
#define SN_MAGIC_LEN		4			//magic数据长度

//#define SN_HEADLEN		...
#define SN_HEADLEN_OFF		12			//头长度的偏移量
#define SN_HEADLEN_LEN		4			//头长度的数据长度

#define SN_HEADOFF_OFF		20			//头偏移的偏移量
#define SN_HEADOFF_LEN		4			//头偏移的数据长度

#define SN_HEAD_START		0x20


//------------------------------------------------------------------------------------------------------------
//sn head
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_SN_HEAD{
	__u16 version;			//版本
	__u16 size;				//长度
	__u32 res0;				//保留
	__u32 attr;				//属性
	__u32 dataoff;			//数据在数据区的偏移量
	__u32 datalen;			//数据的长度
	__u32 SNCount;			//本文件序列号的个数
	__s32 Year;				//年
	__s32 Month;				//月
	__s32 Day;				//日
	__s32 Hour;				//时
	__s32 Minute;				//分
	__s32 Second;				//秒
	__u32 FreeSNIndex;		//当前未使用的SN的索引号，取得SN后需要累加.(0 - SNCount-1)
	__u32 res1;
}SN_HEAD_t;
#pragma pack(pop)


#define SN_HEAD_SIZE			sizeof(SN_HEAD_t)	//
#define SN_HEADLEN				SN_HEAD_SIZE		//
//------------------------------------------------------------------------------------------------------------
//sn data
//------------------------------------------------------------------------------------------------------------

//sn item

#define SN_PID_LEN				8
#define SN_SID_LEN				8
#define SN_SNVER_LEN			4
#define SN_GUID_LEN				36
#define SN_ITEM_LEN				128
#define SN_RES_LEN	    		(SN_ITEM_LEN - sizeof(u32) - SN_PID_LEN - SN_SID_LEN - SN_SNVER_LEN - SN_GUID_LEN)

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_SN_ITEM{
	__u32 ProductNum;           //产品流水号
	__s8  pid[SN_PID_LEN];		//代理商
	__s8  sid[SN_SID_LEN];		//方案
	__s8  snver[SN_SNVER_LEN];	//snver 序列号应用版本
	__s8  GUID[SN_GUID_LEN];	//GUID
	__u8  res[SN_RES_LEN];		//保留
}SN_ITEM_t;
#pragma pack(pop)


#define SN_ITEM_SIZE sizeof(SN_ITEM_t)

//------------------------------------------------------------------------------------------------------------
//sn mark
//------------------------------------------------------------------------------------------------------------

#define SN_MARK_LEN				32
#define MARK_INFO_LEN			(SN_MARK_LEN - sizeof(u32) - sizeof(u32))

//attr
#define ITEM_ATTR_UNUSED		0	//序列号未使用
#define ITEM_ATTR_USED			1	//序列号使用了
#define ITEM_ATTR_INUSE			2	//序列号正在使用
#define ITEM_ATTR_ERROR			3	//序列号出错
#define ITEM_ATTR_INVALID		4	//序列号非法

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct tag_SN_ITEM_MARK{
	__u32 attr;					//序列号属性 unused, used, inuse, invalid, error
	__u32 value;					//属性值
	__s8  info[MARK_INFO_LEN];	//信息
}SN_ITEM_MARK_t;
#pragma pack(pop)

#define SN_ITEM_MARK_SIZE	sizeof(SN_ITEM_MARK_t)

// sn data length : item = mark
#define SN_LEN			(SN_ITEM_SIZE + SN_ITEM_MARK_SIZE)


#pragma pack(push, 1)
typedef struct tag_SN{
	SN_ITEM_t		item;		//序列号
	SN_ITEM_MARK_t	mark;		//序列号标记
}SN_t;
#pragma pack(pop)


//------------------------------------------------------------------------------------------------------------
// encode sn head
//------------------------------------------------------------------------------------------------------------
#define GUID_LEN					36
#define ENCODE_SN_HEAD_LEN			128


#pragma pack(push, 1)
typedef struct tag_ENCODE_SN_HEAD{
	__s8 	GUID[GUID_LEN];				//GUID
	__u32	encodelen;					//加密后的长度
	__u32 origionlen;					//加密前的长度
	__u32 method;						//加密算法
	__u8  res[ENCODE_SN_HEAD_LEN - GUID_LEN - 12];//保留
}ENCODE_SN_HEAD_t;
#pragma pack(pop)

#define ENCODE_SN_HEAD_SIZE sizeof(ENCODE_SN_HEAD_t)



#endif //__SN_FILE____H____



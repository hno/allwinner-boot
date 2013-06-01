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
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//    I MMMMMMMMMMI                                                                                         //
//      I FF      BBI                                                                                       //
//      I FF      I EE  FFMMFFI MMMMLL  I MMMMMMEE          EEMMBBLLBBFF    FFMMMMBBI   I MMBBBBMMMMI       //
//      I FF        EEI   I BBBBI   I     I     LLFF      EELL  I BBFF    FFFF    I BBI   I BBI   I EE      //
//      I FF        EEI   I FF          I BBMMMMMMFF      FF      I FF  I EE        I LL  I FF      EEI     //
//      I FF      I EE    I FF          EEI     I FF      FF      I FF  I EE        I LL  I FF      EEI     //
//      I FF      FFI     I FF          EEI     BBFF      FFI     EEFF    FFI       FFI   I FF      EEI     //
//    I MMMMMMMMMMFF    BBMMMMMMMMFF    LLMMMMMMFFMMFF    I BBMMMMFFFF    I BBMMMMMMLL  I MMMMFF  BBMMBBI   //
//                                                                I FF                                      //
//                                                                FFLL                                      //
//                                                          BBMMMMEE                                        //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                              Dragon System                                               //
//                                                                                                          //
//                               (c) Copyright 2006-2007, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : ImgDecode.cpp                                                                                  //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2008-11-03 9:57:52                                                                             //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2008-11-03 9:57:55                                                                                     //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

#include  "../card_sprite_i.h"

//#include "imgdecode.h"
#include "imagefile_new.h"

#define HEAD_ID				0		//头加密接口索引
#define TABLE_ID			1		//表加密接口索引
#define DATA_ID				2		//数据加密接口索引
#define IF_CNT				3		//加密接口个数	现在只有头加密，表加密，数据加密3种
#define	MAX_KEY_SIZE 		32		//密码长度

#pragma pack(push, 1)
typedef struct tag_IMAGE_HANDLE
{

//	HANDLE  fp;			//

	ImageHead_t  ImageHead;		//img头信息

	ImageItem_t *ItemTable;		//item信息表

//	RC_ENDECODE_IF_t rc_if_decode[IF_CNT];//解密接口

//	BOOL			bWithEncpy; // 是否加密
}IMAGE_HANDLE;

#define INVALID_INDEX		0xFFFFFFFF


typedef struct tag_ITEM_HANDLE{
	uint	index;					//在ItemTable中的索引
	uint    reserved[3];
//	long long pos;
}ITEM_HANDLE;

#define ITEM_PHOENIX_TOOLS 	  "PXTOOLS "
#define   TEST_BLK_BYTES      (512 * 1024)

char *img_file_buffer;
uint img_file_start;			//固件的起始位置
//------------------------------------------------------------------------------------------------------------
//image解析插件的接口
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
HIMAGE 	Img_Open	(__u32 start)
{
	IMAGE_HANDLE * pImage = NULL;
	uint ItemTableSize;					//固件索引表的大小

	img_file_start = start;
	__inf("img start = %x\n", img_file_start);
	pImage = (IMAGE_HANDLE *)wBoot_malloc(sizeof(IMAGE_HANDLE));
	if (NULL == pImage)
	{
		__inf("sunxi sprite error: fail to wBoot_malloc memory for img head\n");

		return NULL;
	}
	memset(pImage, 0, sizeof(IMAGE_HANDLE));
	//------------------------------------------------
	//读img头
	//------------------------------------------------
	//__inf("try to read mmc start %d\n", img_file_start);
	if(wBoot_block_read(img_file_start, IMAGE_HEAD_SIZE/512, &pImage->ImageHead))
	{
		__inf("sunxi sprite error: read image head fail\n");

		goto _img_open_fail_;
	}
	//------------------------------------------------
	//比较magic
	//------------------------------------------------
	if (memcmp(pImage->ImageHead.magic, IMAGE_MAGIC, 8) != 0)
	{
		__inf("sunxi sprite error: image magic is bad\n");

		goto _img_open_fail_;
	}
	//------------------------------------------------
	//为索引表开辟空间
	//------------------------------------------------
	ItemTableSize = pImage->ImageHead.itemcount * sizeof(ImageItem_t);
	pImage->ItemTable = (ImageItem_t*)wBoot_malloc(ItemTableSize);
	if (NULL == pImage->ItemTable)
	{
		__inf("sunxi sprite error: fail to wBoot_malloc memory for item table\n");

		goto _img_open_fail_;
	}
	//------------------------------------------------
	//读出索引表
	//------------------------------------------------
	if(wBoot_block_read(img_file_start + (IMAGE_HEAD_SIZE/512), ItemTableSize/512, pImage->ItemTable))
	{
		__inf("sunxi sprite error: read iamge item table fail\n");

		goto _img_open_fail_;
	}
    img_file_buffer = (char *)wBoot_malloc(TEST_BLK_BYTES);
    if(!img_file_buffer)
    {
        __inf("sunxi sprite error: fail to wBoot_malloc memory for item buffer");
        goto _img_open_fail_;

    }
	return pImage;

_img_open_fail_:
	if(pImage->ItemTable)
	{
		wBoot_free(pImage->ItemTable);
	}
	if(pImage)
	{
		wBoot_free(pImage);
	}

	return NULL;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
HIMAGEITEM 	Img_OpenItem	(HIMAGE hImage, char * MainType, char * subType)
{
	IMAGE_HANDLE* pImage = (IMAGE_HANDLE *)hImage;
	ITEM_HANDLE * pItem  = NULL;
	uint          i;

	if (NULL == pImage || NULL == MainType || NULL == subType)
	{
		return NULL;
	}

	pItem = (ITEM_HANDLE *) wBoot_malloc(sizeof(ITEM_HANDLE));
	if (NULL == pItem)
	{
		__inf("sunxi sprite error : cannot wBoot_malloc memory for item\n");

		return NULL;
	}
	pItem->index = INVALID_INDEX;

	for (i = 0; i < pImage->ImageHead.itemcount ; i++)
	{
//		int nCmp = memcmp(ITEM_PHOENIX_TOOLS, MainType, MAINTYPE_LEN);
//
//		if (nCmp == 0)//
//		{
//			if (memcmp(MainType, pImage->ItemTable[i].mainType, MAINTYPE_LEN) == 0 )
//			{
//				pItem->index = i;
//
//				return pItem;
//			}
//		}
//		else
//		{
//			nCmp = memcmp(MainType, pImage->ItemTable[i].mainType, MAINTYPE_LEN);
//			if(nCmp == 0)
//			{
//				nCmp = memcmp(subType,  pImage->ItemTable[i].subType,  SUBTYPE_LEN);
//				if( nCmp == 0)
//				{
//					pItem->index = i;
//
//					return pItem;
//				}
//			}
//		}
		if(!memcmp(subType,  pImage->ItemTable[i].subType,  SUBTYPE_LEN))
		{
			pItem->index = i;

			return pItem;
		}
	}

	__inf("sunxi sprite error : cannot find item %s %s\n", MainType, subType);

	wBoot_free(pItem);
	pItem = NULL;

	return NULL;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
__u64 Img_GetItemSize	(HIMAGE hImage, HIMAGEITEM hItem)
{
	IMAGE_HANDLE* pImage = (IMAGE_HANDLE *)hImage;
	ITEM_HANDLE * pItem  = (ITEM_HANDLE  *)hItem;
	long long       size;

	if (NULL == pItem)
	{
		__inf("sunxi sprite error : item is NULL\n");

		return 0;
	}

	size = pImage->ItemTable[pItem->index].filelenHi;
	size <<= 32;
	size |= pImage->ItemTable[pItem->index].filelenLo;

	return size;
}

//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
uint Img_GetItemStart	(HIMAGE hImage, HIMAGEITEM hItem)
{
	IMAGE_HANDLE* pImage = (IMAGE_HANDLE *)hImage;
	ITEM_HANDLE * pItem  = (ITEM_HANDLE  *)hItem;
	long long       start;
	long long		offset;

	if (NULL == pItem)
	{
		__inf("sunxi sprite error : item is NULL\n");

		return 0;
	}
	offset = pImage->ItemTable[pItem->index].offsetHi;
	offset <<= 32;
	offset |= pImage->ItemTable[pItem->index].offsetLo;
	start = offset/512;

	return ((uint)start + img_file_start);
}
//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//     返回实际读取数据的长度
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
uint Img_ReadItem(HIMAGE hImage, HIMAGEITEM hItem, void *buffer, uint buffer_size)
{
	IMAGE_HANDLE* pImage = (IMAGE_HANDLE *)hImage;
	ITEM_HANDLE * pItem  = (ITEM_HANDLE  *)hItem;
	long long     start;
	long long	  offset;
	uint	      file_size;

	if (NULL == pItem)
	{
		__inf("sunxi sprite error : item is NULL\n");

		return 0;
	}
	if(pImage->ItemTable[pItem->index].filelenHi)
	{
		__inf("sunxi sprite error : the item too big\n");

		return 0;
	}

	file_size = pImage->ItemTable[pItem->index].filelenLo;
	// __inf("%s:buffer_size=%d,file_size=%d\n",__FUNCTION__,buffer_size,file_size);
	file_size = (file_size + 1023) & (~(1024 - 1)); 
//      __inf("%s:after modefiy, file_size=%d\n",__FUNCTION__,file_size);
//	if(file_size > buffer_size)
//	{
//		__inf("sunxi sprite error : buffer is smaller than data size\n");

//		return 0;
//	}

	offset = pImage->ItemTable[pItem->index].offsetHi;
	offset <<= 32;
	offset |= pImage->ItemTable[pItem->index].offsetLo;
	start = offset/512;

	if(wBoot_block_read((uint)start + img_file_start, file_size/512, img_file_buffer))
	{
		__inf("sunxi sprite error : read item data failed\n");

		return 0;
	}
    memcpy(buffer,img_file_buffer,buffer_size);
    
	return file_size;
}
//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
int Img_CloseItem	(HIMAGE hImage, HIMAGEITEM hItem)
{
	ITEM_HANDLE * pItem = (ITEM_HANDLE *)hItem;

	if (NULL == pItem)
	{
		__inf("sunxi sprite error : item is null when close it\n");

		return -1;
	}
	wBoot_free(pItem);
	pItem = NULL;

	return 0;
}



//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
void  Img_Close	(HIMAGE hImage)
{
	IMAGE_HANDLE * pImage = (IMAGE_HANDLE *)hImage;

	if (NULL == pImage)
	{
		__inf("sunxi sprite error : imghead is null when close it\n");

		return ;
	}

	if (NULL != pImage->ItemTable)
	{
		wBoot_free(pImage->ItemTable);
		pImage->ItemTable = NULL;
	}

	memset(pImage, 0, sizeof(IMAGE_HANDLE));
	wBoot_free(pImage);
	pImage = NULL;

	return ;
}



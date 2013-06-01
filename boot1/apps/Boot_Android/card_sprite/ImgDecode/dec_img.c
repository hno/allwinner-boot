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
#include "dec_img.h"


#include "../imgencodedecode/encode.h"
#include "../fileformat/imagefile.h"				//private head file

#define ITEM_PHOENIX_TOOLS 	  "PXTOOLS "


#define min( x, y )          ( (x) < (y) ? (x) : (y) )
#define max( x, y )          ( (x) > (y) ? (x) : (y) )

#define HEAD_ID				0		//头加密接口索引
#define TABLE_ID			1		//表加密接口索引
#define DATA_ID				2		//数据加密接口索引
#define IF_CNT				3		//加密接口个数	现在只有头加密，表加密，数据加密3种
#define	MAX_KEY_SIZE 		32		//密码长度

#pragma pack(push, 1)
typedef struct tag_IMAGE_HANDLE
{

	CSzFile      * fp;			//文件句柄

	ImageHead_t  ImageHead;		//img头信息

	ImageItem_t *ItemTable;		//item信息表

	RC_ENDECODE_IF_t rc_if_decode[IF_CNT];//解密接口
	
	__bool			bWithEncpy; // 是否加密
	
}IMAGE_HANDLE;

#define INVALID_INDEX		0xFFFFFFFF

#pragma pack(push, 1)
typedef struct tag_ITEM_HANDLE{
	__u32	index;					//在ItemTable中的索引
	__u64 pos;
}ITEM_HANDLE;
#pragma pack(pop)

static  __u8 *buffer_encode;
static  int   buffer_encode_size;

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
HIMAGE 	Img_Open	(__u32 start_sector, int size)
{
	IMAGE_HANDLE  *pImage;
	ImageHead_t ImageHead;
	char	seed[]	= "img";
	char key_buff[MAX_KEY_SIZE];
	__u32 key_len = MAX_KEY_SIZE;
	__u32 ItemTableSize;
	__u8 * ItemTableBuf, * pItemTableDecode;
	__u32 i;

    pImage = (IMAGE_HANDLE *)wBoot_malloc(sizeof(IMAGE_HANDLE));
    if(!pImage)
    {
        __wrn("DEC_IMG: fail to get memory for pImage\n");

        return NULL;
    }
	memset(pImage, 0, sizeof(IMAGE_HANDLE));
	buffer_encode_size = size;
	//------------------------------------------------
	//初始化解密接口
	//------------------------------------------------
	for (i = 0; i< IF_CNT; i++)
	{
		pImage->rc_if_decode[i].handle 		= NULL;
		pImage->rc_if_decode[i].Initial		= Initial;
		pImage->rc_if_decode[i].EnDecode	= Decode;
		pImage->rc_if_decode[i].UnInitial	= UnInitial;
		memset(key_buff, i, key_len);		//前面的数据初始化为0 note
		key_buff[key_len - 1] = seed[i];	//最后一个字节修改    note

		pImage->rc_if_decode[i].handle = pImage->rc_if_decode[i].Initial(key_buff, &key_len, i);
		if (NULL == pImage->rc_if_decode[i].handle)
		{
		    wBoot_free(pImage);
		    __wrn("DEC_IMG: pImage->rc_if_decode[i].handle is NULL\n");

			return NULL;
		}
	}
	//------------------------------------------------
	//打开img文件
	//------------------------------------------------
	pImage->fp = File_Open(start_sector, 0, 4, 0);
	if (NULL == pImage->fp)
	{
	    wBoot_free(pImage);
		__wrn("DEC_IMG: open img file failed\n");

		return NULL;
	}

	//------------------------------------------------
	//读img头
	//------------------------------------------------
	if(File_Read(&ImageHead, sizeof(ImageHead_t), 1, pImage->fp))
	{
        
	    wBoot_free(pImage);
		__wrn("DEC_IMG: read imagehead fail!\n");

        return  NULL;

    }
     
	if(memcmp(ImageHead.magic, IMAGE_MAGIC, 8) == 0)
	{
		pImage->bWithEncpy = 0;
       // __wrn("DEC_IMG: no encpy used!\n");
	}
	else
	{
		pImage->bWithEncpy = 1;
       // __wrn("DEC_IMG:  encpy used!\n");
	}
	
	if(pImage->bWithEncpy)
	{
    	__u8 * pHead = (__u8 *) &ImageHead;
    	__u8 * pHeadDecode = (__u8 *)&pImage->ImageHead;
    	__u8 * pin, *pout;

    	for (i = 0; i < sizeof(ImageHead_t); i+= ENCODE_LEN)
    	{
    		pin = pHead + i;
    		pout= pHeadDecode + i;
    		if (OK  != pImage->rc_if_decode[HEAD_ID].EnDecode(pImage->rc_if_decode[HEAD_ID].handle, pin , pout))
    		{
    		    wBoot_free(pImage);
    			File_Close(pImage->fp);
    			__wrn("DEC_IMG: decode img file head failed\n");

    			return NULL;
    		}
    	}
    }
    else
    {
        memcpy((void *)(&(pImage->ImageHead)), (void*)&ImageHead, sizeof(ImageHead_t));
    }
	//------------------------------------------------
	//比较magic
	//------------------------------------------------
	//__wrn("DEC_IMG: img header magic:%s\n",pImage->ImageHead.magic);
	if (memcmp(pImage->ImageHead.magic, IMAGE_MAGIC, 8))
	{
	    wBoot_free(pImage);
		File_Close(pImage->fp);
		__wrn("DEC_IMG: img header magic error\n");

		return NULL;
	}
	ItemTableSize = pImage->ImageHead.itemcount * sizeof(ImageItem_t);
	pImage->ItemTable = (ImageItem_t*)wBoot_malloc(ItemTableSize);
	if(!pImage->ItemTable)
	{
	    wBoot_free(pImage);
		File_Close(pImage->fp);
		__wrn("DEC_IMG: fail to get memory for ItemTable\n");

		return NULL;
	}

	//------------------------------------------------
	//read ItemTable
	//------------------------------------------------
	ItemTableBuf = (__u8 *)wBoot_malloc(ItemTableSize);
	if(!ItemTableBuf)
	{
	    wBoot_free(pImage->ItemTable);
	    wBoot_free(pImage);
		File_Close(pImage->fp);
		__wrn("DEC_IMG: fail to get memory for ItemTableBuf\n");

		return NULL;
	}
  //  __wrn("DEC_IMG: pImage->ImageHead.itemoffset=0x%x\n",pImage->ImageHead.itemoffset);
  // __wrn("DEC_IMG: pImage->ImageHead.itemSize=0x%x\n",pImage->ImageHead.itemsize);
  //  __wrn("DEC_IMG: pImage->ImageHead.itemcount=%d\n",pImage->ImageHead.itemcount);
    
	File_Seek(pImage->fp, pImage->ImageHead.itemoffset, SZ_SEEK_SET);
	if(File_Read(ItemTableBuf, ItemTableSize, 1, pImage->fp))
	{
		__wrn("DEC_IMG: read ItemTable failed\n");
	    return 0;
	}
	File_Seek(pImage->fp, 0, SZ_SEEK_CUR);
	//------------------------------------------------
	// decode ItemTable
	//------------------------------------------------
	pItemTableDecode = (__u8 *)pImage->ItemTable;
    if(pImage->bWithEncpy)
	{
	    __u8 *pin, *pout;

    	for (i = 0; i < ItemTableSize; i+= ENCODE_LEN)
    	{
    		pin = ItemTableBuf + i;
    		pout= pItemTableDecode + i;

    		if (OK  != pImage->rc_if_decode[TABLE_ID].EnDecode(pImage->rc_if_decode[TABLE_ID].handle, pin , pout))
    		{
    		    wBoot_free(pImage->ItemTable);
    		    wBoot_free(ItemTableBuf);
    			File_Close(pImage->fp);
    			wBoot_free(pImage);
    			__wrn("DEC_IMG: decode img file ItemTable failed\n");

    			return NULL;
    		}
    	}
    }else
	{
	
		memcpy(pItemTableDecode, ItemTableBuf, ItemTableSize);
	
	}
	
    wBoot_free(ItemTableBuf);

    buffer_encode = (__u8 *)wBoot_malloc(buffer_encode_size);
    if(!buffer_encode)
    {
        wBoot_free(pImage->ItemTable);
	    wBoot_free(ItemTableBuf);
		File_Close(pImage->fp);
		wBoot_free(pImage);
		__wrn("DEC_IMG: fail to get memory for buffer_encode\n");

		return NULL;
    }

	return pImage;
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
	__u32 i;

	pItem = (ITEM_HANDLE *)wBoot_malloc(sizeof(ITEM_HANDLE));
	if(!pItem)
	{
		__wrn("DEC_IMG: fail to get memory for pItem\n");
	    return NULL;
	}

	pItem->index = INVALID_INDEX;
	pItem->pos = 0;

	for (i = 0; i < pImage->ImageHead.itemcount ; i++)
	{
    //  __wrn("pImage->ItemTable[%d].mainType=%s,subType=%s\n",i,pImage->ItemTable[i].mainType,\
    //    pImage->ItemTable[i].subType);
        if (memcmp(ITEM_PHOENIX_TOOLS, MainType, MAINTYPE_LEN) == 0)//
		{
			if (memcmp(MainType, pImage->ItemTable[i].mainType, MAINTYPE_LEN) == 0 )
			{
				pItem->index = i;
				return pItem;
			}
		}
		else if (memcmp(MainType, pImage->ItemTable[i].mainType, MAINTYPE_LEN) == 0 &&
		         memcmp(subType,  pImage->ItemTable[i].subType,  SUBTYPE_LEN)  == 0)
		{
			pItem->index = i;
			return pItem;
		}
	}

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
    IMAGE_HANDLE *pImage = (IMAGE_HANDLE *)hImage;
    ITEM_HANDLE  *pItem  = (ITEM_HANDLE  *)hItem;
    __u64      nItemSize;

 	nItemSize  = pImage->ItemTable[pItem->index].filelenLo;
    nItemSize |= (__u64)pImage->ItemTable[pItem->index].filelenHi << 32;
//	return pImage->ItemTable[pItem->index].filelen;
  	return nItemSize;
    //由于有第一层加密，因此都会按照分组进行
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

static __u32 __Img_ReadItemData(HIMAGE hImage, HIMAGEITEM hItem, void * buffer, __u64 Length);

// 根据分组进行加速处理的版本 scott 2009-06-22 10:37:17
__u32 Img_ReadItemData(HIMAGE hImage, HIMAGEITEM hItem, void * buffer, __u64 Length)
{
	__u32 readlen = 0;
	IMAGE_HANDLE* pImage = (IMAGE_HANDLE *)hImage;
	ITEM_HANDLE * pItem  = (ITEM_HANDLE  *)hItem;
    __u64      filelen;
    __u64      datalen;
    __u64      offset;
    __u64      pos=0;
	__u32 this_read;
	__u32 i;

    pEnDecode pfDecode = pImage->rc_if_decode[DATA_ID].EnDecode;
	u8* pInPutBuffer = (u8*)buffer;
	u8* pBufferRead = pInPutBuffer ;
    filelen  = pImage->ItemTable[pItem->index].filelenLo;
    filelen |= (__u64)pImage->ItemTable[pItem->index].filelenHi << 32;
	if (pItem->pos >=filelen) //filelen <= datalen
	{
		__wrn("DEC_IMG: file pointer position is larger than file length\n");
		return 0;
	}
	//------------------------------------------------
	//约束数据不会超出加密数据的范围
	//------------------------------------------------
	datalen  = pImage->ItemTable[pItem->index].datalenLo;
    datalen |= (__u64)pImage->ItemTable[pItem->index].datalenHi << 32;
	Length = min((__u64)Length, datalen - pItem->pos);
	//------------------------------------------------
	//加密后的数据以16byte进行分组，需要处理跨边界的情况
	//------------------------------------------------
	if ((pItem->pos % ENCODE_LEN) == 0)	//pos正好是分组的边界的情况
	{
	    __u8 *pin, *pout;
	    __u32 n;

	    offset  = pImage->ItemTable[pItem->index].offsetLo;
        offset |= (__u64)pImage->ItemTable[pItem->index].offsetHi << 32;
       // __inf("item offset low=%d\n",pImage->ItemTable[pItem->index].offsetLo);
       // __inf("item offset high=%d\n",pImage->ItemTable[pItem->index].offsetHi);
        pos = offset + pItem->pos;
		File_Seek(pImage->fp, pos, SZ_SEEK_SET);
		readlen = 0;
		while(readlen < Length)
		{
			//------------------------------------------------
			//每次读取n个分组
			//------------------------------------------------
			this_read = min(buffer_encode_size, (Length - readlen));
			n = (this_read + ENCODE_LEN - 1) / ENCODE_LEN;	//
			//memset(buffer_encode, 0, n * ENCODE_LEN);
			//一次读n个分组,速度更快 note has bug
			if(pImage->bWithEncpy == 0)
			{
				pBufferRead = pInPutBuffer + readlen;
				if(File_Read(pBufferRead, n * ENCODE_LEN, 1, pImage->fp))	//OK 测试通过，必须读取整个的分组
           		 {
                	//如果读不出数据
                	__wrn("DEC_IMG: read ItemData failed\n");
                	return 0;
            	 }
			
			}else
			{
			
			
				if(File_Read(buffer_encode, n * ENCODE_LEN, 1, pImage->fp))	//OK 测试通过，必须读取整个的分组
           		 {
            	    //如果读不出数据
               		 __wrn("DEC_IMG: read ItemData failed\n");
               		 return 0;
            	 }
				File_Seek(pImage->fp, 0, SZ_SEEK_CUR);
				//------------------------------------------------
				//分组数据解密
				//------------------------------------------------
				pin  = buffer_encode;
				pout = (__u8 *)buffer;
				pout = pout + readlen;	//实际输出数据的偏移量

				for (i = 0; i < n; i++)	//逐个分组进行解密
				{
					//------------------------------------------------
					//每次解密一个分组
					//------------------------------------------------
					if (OK !=  pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
					{
						__wrn("DEC_IMG: decode ItemData failed\n");
						return 0;
					}
					pin += ENCODE_LEN;
					pout+= ENCODE_LEN;
				}
			}
			//------------------------------------------------
			//计算实际有效数据长度
			//------------------------------------------------
			readlen += this_read;
		}
		pItem->pos += readlen;
		return readlen;
	}
	else
	{
		//------------------------------------------------
		//这里强制只处理分组对齐的情况，对于以前的一些固件包可能会引起不兼容的问题，
		//那种情况下只好启用原始版本来处理了
		//------------------------------------------------
		__wrn("__Img_ReadItemData\n");
		return  __Img_ReadItemData(hImage, hItem,  buffer, Length);
	}

	//return 0;
}


//原始的版本，可以运行，不过每次读img文件是16byte，速度不高，需要进行提速
__u32 __Img_ReadItemData(HIMAGE hImage, HIMAGEITEM hItem, void * buffer, __u64 Length)
{

	IMAGE_HANDLE* pImage = (IMAGE_HANDLE *)hImage;
	ITEM_HANDLE * pItem  = (ITEM_HANDLE  *)hItem;
    __u64      filelen;
    __u64      datalen;
    __u64      offset;
    __u64      pos=0;
    __u64      readlen=0;
	__u8 buffer_encode[ENCODE_LEN];


	pEnDecode pfDecode = pImage->rc_if_decode[DATA_ID].EnDecode;
	if (NULL == pImage || NULL == pItem || NULL == buffer || 0 == Length)
	{
		return 0;
	}

   	filelen  = pImage->ItemTable[pItem->index].filelenLo;
    filelen |= (__u64)pImage->ItemTable[pItem->index].filelenHi << 32;
	if (pItem->pos >=filelen) //filelen <= datalen
	{
		return 0;
	}
	//------------------------------------------------
	//约束数据不会超出加密数据的范围
	//------------------------------------------------
	datalen = pImage->ItemTable[pItem->index].datalenLo;
    datalen |= (__u64)pImage->ItemTable[pItem->index].datalenHi << 32;
	Length = min((__u64)Length, datalen - pItem->pos);
	if(pImage->bWithEncpy == 0)
	{
   		offset = pImage->ItemTable[pItem->index].offsetLo;
        offset |= (__u64)pImage->ItemTable[pItem->index].offsetHi << 32;
        pos = offset + pItem->pos;
		File_Seek(pImage->fp, pos, SZ_SEEK_SET);
        
		if(File_Read(buffer, Length, 1, pImage->fp))
		{
			return 0;
		}
		pItem->pos += Length;
		return Length;
	}

	//------------------------------------------------
	//加密后的数据以16byte进行分组，需要处理跨边界的情况
	//------------------------------------------------
	if ((pItem->pos % ENCODE_LEN) == 0)	//pos正好是分组的边界的情况
	{
	    __u8 *pin, *pout;

   		 offset = pImage->ItemTable[pItem->index].offsetLo;
        offset |= (__u64)pImage->ItemTable[pItem->index].offsetHi << 32;
        pos = offset + pItem->pos;

		File_Seek(pImage->fp, pos, SZ_SEEK_SET);

		while(readlen < Length)
		{
			//每次读取一个分组
			//memset(buffer_encode, 0, ENCODE_LEN);
			if(File_Read(buffer_encode, ENCODE_LEN, 1, pImage->fp))
			{
			    return 0;
			}
			File_Seek(pImage->fp, 0, SZ_SEEK_CUR);
			//分组数据解密
			pin = buffer_encode;
			pout= (__u8 *)buffer;
			pout= pout + readlen;
			if (OK != pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
				return 0;
			//计算实际有效数据长度
			readlen += min(Length- readlen, ENCODE_LEN);
		}
		pItem->pos += readlen;
		return readlen;
	}
	else //pos不在边界
	{
		//pos不在边界，向头方向seek

        offset = pImage->ItemTable[pItem->index].offsetLo;
        offset |= (__u64)pImage->ItemTable[pItem->index].offsetHi << 32;		
		pos = offset +pItem->pos - (pItem->pos % ENCODE_LEN);
		File_Seek(pImage->fp, pos, SZ_SEEK_SET);

		//-----------------------------------
		//**********************OOOOOOOOOOOOO     *表示已经读取得数据 O表示未读取得数据
		//-----------------------------------
		if ((0 < Length) && (Length < ENCODE_LEN)) //读取的数据不足一个分组长度
		{
		    __u8 *pin, *pout;
			__u32 read = ENCODE_LEN - (pItem->pos % ENCODE_LEN); //分组中未读取的数据长度
			if (Length <= read)	//需要读取得数据小于等于分组中未读取的数据长度 只用读一个分组即可
			{
				//memset(buffer_encode, 0, ENCODE_LEN);
				if(File_Read(buffer_encode, ENCODE_LEN, 1, pImage->fp))
				{
				    return 0;
				}
				//分组数据解密
				pin = buffer_encode;
				pout= (__u8 *)buffer;
				pout     = pout + readlen;
				if (OK != pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
					return 0;

				readlen = Length;
				pItem->pos += readlen;
				return readlen;
			}
			else //需要读两个分组的数据
			{
				//第一个分组
				__u8 *pin, *pout;
				__u32 Left_Length;
				__u32 read = ENCODE_LEN - pItem->pos % ENCODE_LEN;
				//memset(buffer_encode, 0, ENCODE_LEN);
				if(File_Read(buffer_encode, ENCODE_LEN, 1, pImage->fp))
				{
				    return 0;
				}
				//分组数据解密
				pin  = buffer_encode;
				pout = (__u8 *)buffer;
				pout = pout + readlen;
				if (OK != pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
					return 0;

				readlen += read;

				//第二个分组
				Left_Length = Length - read;			//剩余的数据
				//memset(buffer_encode, 0, ENCODE_LEN);
				if(File_Read(buffer_encode, ENCODE_LEN, 1, pImage->fp))
				{
				    return 0;
				}
				File_Seek(pImage->fp, 0, SZ_SEEK_CUR);
				//分组数据解密
				pin = buffer_encode;
				pout= (__u8 *)buffer;
				pout     = pout + readlen;
				if (OK != pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
					return 0;
				readlen += Left_Length;

				pItem->pos += readlen;
				return readlen;
			}
		}
		else if (Length >= ENCODE_LEN) //读取的数据不少于一个分组长度
		{
			__u8 *pin, *pout;
			__u32 Left_Length, Left_readlen;
			__u32 read = ENCODE_LEN - pItem->pos % ENCODE_LEN;
			//memset(buffer_encode, 0, ENCODE_LEN);
			if(File_Read(buffer_encode, ENCODE_LEN, 1, pImage->fp))
			{
			    return 0;
			}
			//分组数据解密
			pin = buffer_encode;
			pout= (__u8 *)buffer;
			pout     = pout + readlen;
			if (OK != pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
				return 0;

			readlen += read;

			//------------------------------------------------
			//剩余的数据按照分组进行处理
			//------------------------------------------------
			Left_Length = Length - read;
			Left_readlen= 0;
			while(Left_readlen < Left_Length)
			{
				//每次读取一个分组
				//memset(buffer_encode, 0, ENCODE_LEN);
				if(File_Read(buffer_encode, ENCODE_LEN, 1, pImage->fp))
				{
				    return 0;
				}
				File_Seek(pImage->fp, 0, SZ_SEEK_CUR);
				//分组数据解密
				pin = buffer_encode;
				pout= (__u8 *)buffer;
				pout     = pout + readlen;
				if (OK != pfDecode(pImage->rc_if_decode[DATA_ID].handle, pin , pout))
					return 0;
				//计算实际有效数据长度
				Left_readlen += min(Left_Length - Left_readlen, ENCODE_LEN);
			}

			readlen += Left_readlen;
		}

		pItem->pos += readlen;
		return readlen;
	}

//	return 0;
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
__u32 Img_CloseItem	(HIMAGE hImage, HIMAGEITEM hItem)
{
	ITEM_HANDLE * pItem = (ITEM_HANDLE *)hItem;
	if (NULL == pItem)
		return __LINE__;

	pItem = NULL;

	return OK;
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

	if (NULL != pImage->fp)
	{
		File_Close(pImage->fp);
		pImage->fp = NULL;
	}

	if (NULL != pImage->ItemTable)
	{
		pImage->ItemTable = NULL;
	}

	memset(pImage, 0, sizeof(IMAGE_HANDLE));
	pImage = NULL;

	if(buffer_encode)
	{
	    wBoot_free(buffer_encode);
    }
	return ;
}


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------




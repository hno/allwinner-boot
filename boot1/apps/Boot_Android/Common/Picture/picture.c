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
#include "picture.h"

#define   BMP_MAGIC      (0x4D42)


/*bmp header struct*/
typedef struct
{
    __u16 bfType;                                   //bit map type
    __u32 bfSize;                                   //total file size
    __u16 bfReserved1;                              //reserved , must be 0
    __u16 bfReserved2;                              //reserved, must be 0
    __u32 bfOffBits;                                //full img offet
}
__attribute__ ((packed)) __bmp_file_head_t;

/*bmp info struct,windows*/
typedef struct
{
    __u32 biSize;                                   //bit map info size
    __u32 biWidth;                                  //img width with pixel unit
    __s32 biHeight;                                 //img height with pixel unit
    __u16 biPlanes;                                 //bit map planes,always 1
    __u16 biBitCount;                               //bit planes per pixel
    __u32 biCompression;                            //compress method
    __u32 biSizeImage;                              //img size with byte unit
    __u32 biXPelsPerMeter;                          //the horizontal resolution with pixel/metre unit
    __u32 biYPelsPerMeter;                          //the vertical resolution with pixel/metre unit
    __u32 biClrUsed;                                //the color numbers used by bmp
    __u32 biClrlmportant;                           //important color numbers
}
__attribute__ ((packed)) __bmp_info_header_t;
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
__s32 Parse_Pic_BMP_ByPath(char *Path, Picture_t *PictureInfo, __u32 address)
{
    __s32                   ret = -1;
    int						length;
    char                    tmp_data[512];
    char					*buffer = NULL, *tmp_buffer;
	__bmp_file_head_t 		*bmp_file_header;
	__bmp_info_header_t 	*bmp_info_header;
	H_FILE                  fd = NULL;                 //file handle

	fd = wBoot_fopen(Path, "rb");
	if(!fd)
	{
		return -1;
	}

	wBoot_fread(tmp_data, 1, 512, fd);
    bmp_file_header = (__bmp_file_head_t *)tmp_data;
    if(bmp_file_header->bfType != BMP_MAGIC)
    {
        goto _get_bmp_by_name_error;
    }
    bmp_info_header = (__bmp_info_header_t *)(tmp_data + sizeof(__bmp_file_head_t));
    PictureInfo->Height = (bmp_info_header->biHeight > 0) ?
    			           bmp_info_header->biHeight : (-bmp_info_header->biHeight);
    PictureInfo->Width = bmp_info_header->biWidth;
	//PictureInfo->BufferSize = bmp_file_header->bfSize - bmp_file_header->bfOffBits;
	PictureInfo->BufferSize = PictureInfo->Height * PictureInfo->Width * 4;
	PictureInfo->BitCount = 32;
	PictureInfo->RowSize  = 4 * PictureInfo->Width;
	wBoot_fseek(fd, 0, 0);
	length = wBoot_flen(fd);
	buffer = (char *)wBoot_malloc(length);
	if(!buffer)
	{
		goto _get_bmp_by_name_error;
	}
	wBoot_fread(buffer, 1, length, fd);
    if(!address)
    {
        PictureInfo->Buffer = (char *)wBoot_malloc(PictureInfo->BufferSize);
    }
    else
    {
        PictureInfo->Buffer = (char *)address;
    }
    tmp_buffer = buffer + bmp_file_header->bfOffBits;
    //获取数据
    if(bmp_info_header->biHeight > 0)
    {
        __u32 i, j;
        __s32 *src, *dest;

        dest = (int *)PictureInfo->Buffer;
        for(i=0; i<(bmp_info_header->biHeight); i++)
        {
            src  = (int *)(tmp_buffer + (bmp_info_header->biHeight - i - 1) * (bmp_info_header->biWidth << 2));
            for(j=0;j<bmp_info_header->biWidth; j++)
            {
                *dest ++ = *src ++;
            }
        }
    }
    else
    {
    	memcpy(PictureInfo->Buffer, tmp_buffer, PictureInfo->BufferSize);
    }

    ret = 0;
_get_bmp_by_name_error:
    if(fd)
    {
        wBoot_fclose(fd);
    }
    if(buffer)
    {
    	wBoot_free(buffer);
    }

    return ret;
}


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

part_info_t           part_info;

typedef struct tag_CRC32_DATA
{
	unsigned int CRC;				//int的大小是32位
	unsigned int CRC_32_Tbl[256];	//用来保存码表
}CRC32_DATA_t;
/*
************************************************************************************************************
*
*                                             _calc_crc32
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
static __u32 calc_crc32(void * buffer, __u32 length)
{
	__u32 i, j;
	CRC32_DATA_t crc32;		//
	__u32 CRC32 = 0xffffffff; //设置初始值

	crc32.CRC = 0;

	for( i = 0; i < 256; ++i)//用++i以提高效率
	{
		crc32.CRC = i;
		for( j = 0; j < 8 ; ++j)
		{
			//这个循环实际上就是用"计算法"来求取CRC的校验码
			if(crc32.CRC & 1)
				crc32.CRC = (crc32.CRC >> 1) ^ 0xEDB88320;
			else //0xEDB88320就是CRC-32多项表达式的值
				crc32.CRC >>= 1;
		}
		crc32.CRC_32_Tbl[i] = crc32.CRC;
	}

	CRC32 = 0xffffffff; //设置初始值
    for( i = 0; i < length; ++i)
    {
        CRC32 = crc32.CRC_32_Tbl[(CRC32^((unsigned char*)buffer)[i]) & 0xff] ^ (CRC32>>8);
    }

    //return CRC32;
	return CRC32^0xffffffff;
}
/*
************************************************************************************************************
*
*                                             _calc_crc32
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
int check_private_part(int keyvalue)
{
	int   i;
	int   ret;
	__u32 crc;
	char  buffer[MBR_SIZE];
	MBR  *mbr_info;

	ret = update_flash_init();
	if(ret)
	{
		__inf("nand init failed\n");

		return -1;
	}
	memset(buffer, 0, MBR_SIZE);
	ret = update_flash_read(0, MBR_SIZE/512, buffer);
	if(ret)
	{
		__inf("nand read mbr failed\n");

		return -1;
	}
	mbr_info = (MBR *)buffer;
	crc = calc_crc32((void *)&mbr_info->version, sizeof(MBR) - 4);
	if(crc != mbr_info->crc32)
	{
		__inf("nand mbr is bad\n");

		return -1;
	}
	for(i=0;i<mbr_info->PartCount;i++)
	{
		if(strcmp(mbr_info->array[i].name, "private"))
		{
			continue;
		}
		else
		{
//			char buffer[32 * 1024];
//			int  len;
//			int  start;
//
//			__inf("ready to erase private part\n");
			part_info.addr = mbr_info->array[i].addrlo;
			part_info.len  = mbr_info->array[i].lenlo;
//			memset(buffer, 0xff, 32 * 1024);
//
//			len = part_info.len;
//			start = part_info.addr;
//			__inf("part size=%d\n", len);
//			__inf("part start=%x\n", start);
//			while(len >= 32 * 1024/512)
//			{
//				update_flash_write(start, 32 * 1024/512, buffer);
//				start += 32 * 1024/512;
//				len   -= 32 * 1024/512;
//			}
			__inf("ready to download private part\n");

			usb_start(1);
    		usb_run();

			break;
		}
	}
	if(i == mbr_info->PartCount)
	{
		__inf("there is no part called private\n");

		return -1;
	}

	update_flash_exit();

	return 0;
}



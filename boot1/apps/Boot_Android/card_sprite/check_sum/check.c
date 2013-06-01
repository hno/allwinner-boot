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

#include  "check_sum.h"

typedef struct tag_CRC32_DATA
{
	unsigned int CRC;				//int的大小是32位
	unsigned int CRC_32_Tbl[256];	//用来保存码表
}CRC32_DATA_t;
//#pragma arm section  code="check_magic"
/********************************************************************************
*函数名称: check_magic
*函数原型: __s32 check_magic( __u32 *mem_base, const char *magic )
*函数功能: 使用“算术和”来校验内存中的一段数据
*入口参数: mem_base       Boot文件在内存中的起始地址
*          magic          Boot的magic
*返 回 值: CHECK_IS_CORRECT      校验正确
*          CHECK_IS_WRONG        校验错误
*备    注:
********************************************************************************/
__s32 check_magic( __u32 *mem_base, const char *magic )
{
	__u32 i;
	boot_file_head_t *bfh;
	__u32 sz;
	unsigned char *p;


	bfh = (boot_file_head_t *)mem_base;
	p = bfh->magic;
	for( i = 0, sz = sizeof( bfh->magic );  i < sz;  i++ )
	{
		if( *p++ != *magic++ )
			return CHECK_IS_WRONG;
	}


	return CHECK_IS_CORRECT;
}

//#pragma arm section




//#pragma arm section  code="check_sum"
/********************************************************************************
*函数名称: check_sum
*函数原型: __s32 check_sum( __u32 *mem_base, __u32 size, const char *magic )
*函数功能: 使用“算术和”来校验内存中的一段数据
*入口参数: mem_base           待校验的数据在内存中的起始地址（必须是4字节对齐的）
*          size               待校验的数据的个数（以字节为单位，必须是4字节对齐的）
*返 回 值: CHECK_IS_CORRECT   校验正确
*          CHECK_IS_WRONG     校验错误
*备    注:
********************************************************************************/
__s32 check_sum( __u32 *mem_base, __u32 size )
{
	__u32 *buf;
	__u32 count;
	__u32 src_sum;
	__u32 sum;
	boot_file_head_t  *bfh;


	bfh = (boot_file_head_t *)mem_base;

	/* 生成校验和 */
	src_sum = bfh->check_sum;                  // 从Boot_file_head中的“check_sum”字段取出校验和
	bfh->check_sum = STAMP_VALUE;              // 将STAMP_VALUE写入Boot_file_head中的“check_sum”字段

	count = size >> 2;                         // 以 字（4bytes）为单位计数
	sum = 0;
	buf = (__u32 *)mem_base;
	do
	{
		sum += *buf++;                         // 依次累加，求得校验和
		sum += *buf++;                         // 依次累加，求得校验和
		sum += *buf++;                         // 依次累加，求得校验和
		sum += *buf++;                         // 依次累加，求得校验和
	}while( ( count -= 4 ) > (4-1) );

	while( count-- > 0 )
		sum += *buf++;

	bfh->check_sum = src_sum;                  // 恢复Boot_file_head中的“check_sum”字段的值

	if( sum == src_sum )
		return CHECK_IS_CORRECT;               // 校验成功
	else
		return CHECK_IS_WRONG;                 // 校验失败
}

//#pragma arm section

#define  ADD_SUM_MAGIC	0x62332672

__s32 verify_sum( void *mem_base, __u32 size )
{
	__u32 *buf;
	__u32 count;
	__u32 sum;

	count = size >> 2;                         // 以 字（4bytes）为单位计数
	sum = 0;
	buf = (__u32 *)mem_base;
	while(count--)
	{
		sum += *buf++;                         // 依次累加，求得校验和
	};

	switch(size & 0x03)
	{
		case 0:
			return sum;
		case 1:
			sum += (*buf & 0x000000ff);
			break;
		case 2:
			sum += (*buf & 0x0000ffff);
			break;
		case 3:
			sum += (*buf & 0x00ffffff);
			break;
	}

	return sum;
}
//#pragma arm section  code="check_file"
/********************************************************************************
*函数名称: check_file
*函数原型: __s32 check_file( __u32 *mem_base, __u32 size, const char *magic )
*函数功能: 使用“算术和”来校验内存中的一段数据
*入口参数: mem_base       待校验的数据在内存中的起始地址（必须是4字节对齐的）
*          size           待校验的数据的个数（以字节为单位，必须是4字节对齐的）
*          magic          magic number, 待校验文件的标识码
*返 回 值: CHECK_IS_CORRECT       校验正确
*          CHECK_IS_WRONG         校验错误
*备    注:
********************************************************************************/
__s32 check_file( __u32 *mem_base, __u32 size, const char *magic )
{
	if( check_magic( mem_base, magic ) == CHECK_IS_CORRECT
        &&check_sum( mem_base, size  ) == CHECK_IS_CORRECT )
        return CHECK_IS_CORRECT;
    else
    	return CHECK_IS_WRONG;
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
__u32 calc_crc32(void * buffer, __u32 length)
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

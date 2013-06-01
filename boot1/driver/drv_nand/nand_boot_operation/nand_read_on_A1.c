/*
* (C) Copyright 2007-2012
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Neil Peng<penggang@allwinnertech.com>
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
#ifndef __nand_read_on_A1_c
#define __nand_read_on_A1_c


#include "nand_flash.h"



__u32 NF_BLOCK_SIZE = 0;
__u32 NF_BLK_SZ_WIDTH = 0;
__u32 NF_PAGE_SIZE = 0;
__u32 NF_PG_SZ_WIDTH = 0;
__u32 BOOT1_LAST_BLK_NUM = 0;


__u32 page_with_bad_block = 0;


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
__s32 NF_open( void )
{
	__u32  blk_for_boot1;
	struct boot_flash_info info;

	if( NAND_PhyInit( ) == FAIL )
		return NF_ERROR;

	if( NAND_GetFlashInfo( &info ) == FAIL )
	{
		goto error;
	}

	page_with_bad_block = info.pagewithbadflag;
	NF_BLOCK_SIZE = info.blocksize * NF_SECTOR_SIZE;
	switch( NF_BLOCK_SIZE )
	{
		case SZ_16K :
			NF_BLK_SZ_WIDTH = 14;
			blk_for_boot1   = BLKS_FOR_BOOT1_IN_16K_BLK_NF;
			break;
		case SZ_32K :
			NF_BLK_SZ_WIDTH = 15;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_32K_BLK_NF;
			break;
		case SZ_128K :
			NF_BLK_SZ_WIDTH = 17;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_128K_BLK_NF;
			break;
		case SZ_256K :
			NF_BLK_SZ_WIDTH = 18;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_256K_BLK_NF;
			break;
		case SZ_512K :
			NF_BLK_SZ_WIDTH = 19;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_512K_BLK_NF;
			break;
		case SZ_1M :
			NF_BLK_SZ_WIDTH = 20;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_1M_BLK_NF;
			break;
		case SZ_2M :
			NF_BLK_SZ_WIDTH = 21;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_2M_BLK_NF;
			break;
		case SZ_4M :
			NF_BLK_SZ_WIDTH = 22;
			blk_for_boot1   =  BLKS_FOR_BOOT1_IN_4M_BLK_NF;
			break;
		default :
			goto error;
	}
	BOOT1_LAST_BLK_NUM = BOOT1_START_BLK_NUM + blk_for_boot1 - 1;

	NF_PAGE_SIZE = info.pagesize * NF_SECTOR_SIZE;
	switch( NF_PAGE_SIZE )
	{
		case SZ_512 :
			NF_PG_SZ_WIDTH =  9;
			break;
		case SZ_2K :
			NF_PG_SZ_WIDTH = 11;
			break;
		case SZ_4K :
			NF_PG_SZ_WIDTH = 12;
			break;
		case SZ_8K :
			NF_PG_SZ_WIDTH = 13;
			break;
		case SZ_16K :
			NF_PG_SZ_WIDTH = 14;
			break;
		default :
			goto error;
	}

	return NF_OK;

error:
	NAND_PhyExit( );

	return NF_ERROR;
}





__s32 NF_close( void )
{
	if( NAND_PhyExit( ) == FAIL )
		return NF_ERROR;
	else
		return NF_OK;
}





/*******************************************************************************
*函数名称: NF_read
*函数原型：int32 NF_read( __u32 sector_num, void *buffer, __u32 N )
*函数功能: 读取nand flash中的数据放入内存中。
*入口参数: sector_num         flash中起始sector number
*          buffer             内存中的起始地址
*          N                  读取的sector个数（以sector为单位）
*返 回 值: NF_OK              正确读取
*          NF_OVERTIME_ERR    操作超时
*          NF_ERROR           读取有误
*备    注: sector_num必须是某个page内的第一个sector. 不能跨块操作。
*******************************************************************************/
__s32 NF_read( __u32 sector_num, void *buffer, __u32 N )
{
	struct boot_physical_param  para;
	__u8  oob_buf[MAX_PAGE_SIZE/NF_SECTOR_SIZE * OOB_BUF_SIZE_PER_SECTOR];
	__u32 page_nr;
	__u32 scts_per_page = NF_PAGE_SIZE >> NF_SCT_SZ_WIDTH;
	__u32 residue;
	__u32 start_page;
	__u32 start_sct;
	__u32 i;
	__u32 blk_num;

	para.chip = 0;
	start_sct = nand_g_mod( sector_num, NF_BLOCK_SIZE >> NF_SCT_SZ_WIDTH, &blk_num );
	para.block = blk_num;
	if( nand_g_mod( start_sct, NF_PAGE_SIZE >> NF_SCT_SZ_WIDTH, &start_page ) != 0 )
		return NF_ERROR;
	para.oobbuf = oob_buf;
	residue = nand_g_mod( N, scts_per_page, &page_nr );
	for( i = 0; i < page_nr; i++ )
	{
		para.mainbuf = (__u8 *)buffer + NF_PAGE_SIZE * i;
		para.page = start_page + i;
		if( NAND_PhyRead( &para ) == FAIL )
			return NF_ERROR;
	}
	if( residue != 0 )
	{
		__u8  page_buf[MAX_PAGE_SIZE];
		__u32 j;
		__u32 word_nr;
		__u32 *p;
		__u32 *q;

		para.mainbuf = page_buf;
		para.page = page_nr;
		if( NAND_PhyRead( &para ) == FAIL )
			return NF_ERROR;
		for( j = 0, p = (__u32 *)( (__u32)buffer + NF_PAGE_SIZE * page_nr ), word_nr = residue * NF_SECTOR_SIZE >> 2, q = (__u32 *)page_buf;
			 j < word_nr;
			 j++ )
			*p++ = *q++;
	}
	return NF_OK;
}






/*******************************************************************************
*函数名称: NF_read_status
*函数原型：int32 NF_read_status( __u32 blk_num )
*函数功能: 判断 blk_num 制定的块是否是坏块。
*入口参数: blk_num         块号
*返 回 值: NF_GOOD_BLOCK     此块不是坏块
*          NF_BAD_BLOCK      此块是坏块
*          NF_OVERTIME_ERR   超时错误
*          NF_ERROR          操作失败
*备    注:
*******************************************************************************/
static __u8  page_buf[MAX_PAGE_SIZE];
__s32 NF_read_status( __u32 blk_num )
{
	struct boot_physical_param  para;
	__u8  oob_buf[MAX_PAGE_SIZE/NF_SECTOR_SIZE * OOB_BUF_SIZE_PER_SECTOR];
	
	para.chip = 0;
	para.block = blk_num;
	para.page = page_with_bad_block;
	para.mainbuf = page_buf;
	para.oobbuf = oob_buf;
   
	if( NAND_PhyRead( &para ) == FAIL )
		return NF_ERROR;
	if( oob_buf[0] != (__u8)0xFF )
	{
        
		return NF_BAD_BLOCK;
         
	}
	else
     {
       
		return NF_GOOD_BLOCK;
	}
}


#endif     //  ifndef __nand_read_on_A1_c

/* end of nand_read_on_A1.c  */

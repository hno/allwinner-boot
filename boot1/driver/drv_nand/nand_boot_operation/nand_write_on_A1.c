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
#ifndef __nand_write_on_A1_c
#define __nand_write_on_A1_c


#include "nand_flash.h"


/*******************************************************************************
*函数名称: NF_erase
*函数原型：int32 NF_erase( __u32 blk_num )
*函数功能: 擦除 blk_num 指定的块。
*入口参数: blk_num         块号
*返 回 值: NF_OK               擦除操作成功
*          NF_ERASE_ERR        擦除操作失败
*          NF_OVERTIME_ERR     超时错误
*备    注:
*******************************************************************************/
__s32 NF_erase( __u32 blk_num )
{
	struct boot_physical_param  para;

	para.chip = 0;
	para.block = blk_num;
	if( NAND_PhyErase( &para ) == SUCCESS )
    {
        return NF_OK;
    }
    else
    {
        return NF_ERASE_ERR;
    }
}






/*******************************************************************************
*函数名称: NF_write
*函数原型：int32 NF_write( __u32 sector_num, void *buffer, __u32 N )
*函数功能: 将内存中从地址buffer开始的size个字节数据写入到 nand flash 中指定的地址
*入口参数: sector_num         flash中起始sector number
*          buffer             内存中的起始地址
*          N                  sector个数（以sector为单位）
*返 回 值: NF_OK              编程操作成功
*          NF_PROG_ERR        编程操作失败
*          NF_OVERTIME_ERR    超时错误
*备    注: sector_num必须是某个page内的第一个sector. N必须覆盖完整的page。
*          不能跨块操作。
*******************************************************************************/
__s32 NF_write( __u32 sector_num, void *buffer, __u32 N )
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
	__u8  nand_version[4];

	NAND_VersionGet(nand_version);
	para.chip = 0;
	start_sct = nand_g_mod( sector_num, NF_BLOCK_SIZE >> NF_SCT_SZ_WIDTH, &blk_num );
	para.block = blk_num;
	if( nand_g_mod( start_sct, NF_PAGE_SIZE >> NF_SCT_SZ_WIDTH, &start_page ) != 0 )
		return NF_PROG_ERR;

	memset(oob_buf, 0xff, MAX_PAGE_SIZE/NF_SECTOR_SIZE * OOB_BUF_SIZE_PER_SECTOR);
	*(__s32 *)oob_buf = *(__s32 *)nand_version;

	para.oobbuf = oob_buf;
	residue = nand_g_mod( N, scts_per_page, &page_nr );
	if( residue != 0 )
		return NF_PROG_ERR;
	for( i = 0; i < page_nr; i++ )
	{
		para.mainbuf = (__u8 *)buffer + NF_PAGE_SIZE * i;
		para.page = start_page + i;
		if( NAND_PhyWrite( &para ) != SUCCESS )
			return NF_PROG_ERR;
	}

	return NF_OK;
}

/*******************************************************************************
*函数名称: NF_write
*函数原型：int32 NF_write( __u32 sector_num, void *buffer, __u32 N )
*函数功能: 将内存中从地址buffer开始的size个字节数据写入到 nand flash 中指定的地址
*入口参数: sector_num         flash中起始sector number
*          buffer             内存中的起始地址
*          N                  sector个数（以sector为单位）
*返 回 值: NF_OK              编程操作成功
*          NF_PROG_ERR        编程操作失败
*          NF_OVERTIME_ERR    超时错误
*备    注: sector_num必须是某个page内的第一个sector. N必须覆盖完整的page。
*          不能跨块操作。
*******************************************************************************/
__s32 NF_write_Seq( __u32 sector_num, void *buffer, __u32 N )
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
	__u8  nand_version[4];

	NAND_VersionGet(nand_version);
	para.chip = 0;
	start_sct = nand_g_mod( sector_num, NF_BLOCK_SIZE >> NF_SCT_SZ_WIDTH, &blk_num );
	para.block = blk_num;
	if( nand_g_mod( start_sct, NF_PAGE_SIZE >> NF_SCT_SZ_WIDTH, &start_page ) != 0 )
		return NF_PROG_ERR;
	memset(oob_buf, 0xff, MAX_PAGE_SIZE/NF_SECTOR_SIZE * OOB_BUF_SIZE_PER_SECTOR);
	*(__s32 *)oob_buf = *(__s32 *)nand_version;

	para.oobbuf = oob_buf;
	residue = nand_g_mod( N, scts_per_page, &page_nr );
	if( residue != 0 )
		return NF_PROG_ERR;
	for( i = 0; i < page_nr; i++ )
	{
		para.mainbuf = (__u8 *)buffer + NF_PAGE_SIZE * i;
		para.page = start_page + i;
		if( NAND_PhyWrite_Seq( &para ) != SUCCESS )
			return NF_PROG_ERR;
	}
//	msg("quit write.\n");
	return NF_OK;
}




/*******************************************************************************
*函数名称: NF_mark_bad_block
*函数原型：NF_mark_bad_block( __u32 blk_num )
*函数功能: 标记blk_num指定的块为坏块。
*入口参数: blk_num            块号
*返 回 值: NF_OK              编程操作成功
*          NF_PROG_ERR        编程操作失败
*          NF_OVERTIME_ERR    超时错误
*备    注:
*******************************************************************************/
__s32 NF_mark_bad_block( __u32 blk_num )
{
	struct boot_physical_param  para;
	__u8  oob_buf[MAX_PAGE_SIZE/NF_SECTOR_SIZE * OOB_BUF_SIZE_PER_SECTOR];
	__u8  page_buf[MAX_PAGE_SIZE];
	__u32 *p;
	__u32 *end_p;

	for( p = (__u32 *)page_buf, end_p = (__u32 *)( page_buf + NF_PAGE_SIZE ) ; p < end_p;  )
		*p++ = 0xFFFFFFFF;
	oob_buf[0] = BAD_BLK_FLAG;
	para.chip = 0;
	para.block = blk_num;
	para.page = page_with_bad_block;
	para.mainbuf = page_buf;
	para.oobbuf = oob_buf;
	if( NAND_PhyWrite( &para ) != SUCCESS )
		return NF_PROG_ERR;
	return NF_OK;
}






__s32 NF_verify_block( __u32 blk_num )
{
	__u32 i;
	__u32 j;
	__u32 k;
	__u32 blk_base;
	__u8  page_buf[MAX_PAGE_SIZE];
	__u8  stamp;
	__u32 scts_per_page = NF_PAGE_SIZE >> NF_SCT_SZ_WIDTH;
	__u32 scts_per_blk  = NF_BLOCK_SIZE >> NF_SCT_SZ_WIDTH;
	__u32 pages_per_blk;

	nand_g_mod( scts_per_blk, scts_per_page, &pages_per_blk );
	for( k = 0, stamp = 0xFF;  k < 2;  k++, stamp = ~stamp )
	{
		if( NF_erase( blk_num ) != NF_OK )
		{
    		goto bad_block;
		}

		for( i = 0;  i < NF_PAGE_SIZE;  i++ )
			page_buf[i] = stamp;

		for( blk_base = blk_num * scts_per_blk, i = 0;
			 i < pages_per_blk ;
			 i++ )
		{
			if( NF_write( ( blk_base + i * scts_per_page ) , page_buf, scts_per_page ) != NF_OK )
			{
    			goto bad_block;
			}

			if( NF_read ( ( blk_base + i * scts_per_page ) , page_buf, scts_per_page ) != NF_OK )
			{
    			goto bad_block;
			}

			for( j = 0;  j < NF_PAGE_SIZE;  j++ )
			{
				if( page_buf[j] != stamp )
				{
    				goto bad_block;
				}
			}
		}
	}

    return NF_GOOD_BLOCK;


bad_block:
    return NF_BAD_BLOCK;
}




#endif     //  ifndef __nand_write_on_A1_c

/* end of nand_write_on_A1.c  */

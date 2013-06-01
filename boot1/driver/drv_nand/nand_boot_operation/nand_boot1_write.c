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
#include "nand_flash.h"

#define TOLERANCE     3         // tolerance for the loop "erase--write--write error--good block by checking"

//#pragma arm section  code="write_in_one_blk"
/*******************************************************************************
*函数名称: write_in_one_blk
*函数原型：int32 write_in_one_blk( __u32 blk_num, void *buf,
*                                  __u32 size, __u32 blk_size )
*函数功能: 将内存中从buf开始的size大小的内容填充满blk_num指定的块。
*入口参数: blk_num           待访问的nand flash块的块号
*          buf               缓冲区的起始地址
*          size              待写入数据的大小
*          blk_size          待访问的nand flash块的尺寸
*返 回 值: ADV_NF_OK             写入成功
*          ADV_NF_NEW_BAD_BLOCK  当前块变成了坏块
*          ADV_NF_OVERTIME_ERR   操作超时
*备    注: 1. 本程序不检验当前块的好坏；块的好坏必须在调用本程序前检验
*          2. 各个备份应该从当前块的起始地址处依次、紧密排放。
*******************************************************************************/
__s32 write_in_one_blk( __u32 blk_num, void *buf, __u32 size, __u32 blk_size )
{
    __u32 j;
    __s32  status;
    __u32 copy_base;
    __u32 copy_end;
    __u32 blk_end;
    __u32 scts_per_copy;

    for( j = 0, scts_per_copy = size >> NF_SCT_SZ_WIDTH; j < TOLERANCE; j++ )
    {
	    status = NF_erase( blk_num );
	    if( status == NF_OVERTIME_ERR )
	    	return ADV_NF_OVERTIME_ERR;
        else if( status == NF_ERASE_ERR )
        {
        	__wrn("fail in erasing block %u.\n", blk_num);
        	NF_mark_bad_block( blk_num );         // 在标记坏块前，先擦除
        	return ADV_NF_NEW_BAD_BLOCK;
        }
		__msg("Succeed in erasing block %u.\n", blk_num);

        for( copy_base = blk_num * blk_size, copy_end = copy_base + size, blk_end = copy_base + blk_size;
        	 copy_end <= blk_end;
        	 copy_base += size, copy_end = copy_base + size )
        {
    	    status = NF_write( copy_base >> NF_SCT_SZ_WIDTH, buf, scts_per_copy );
    		__msg("finish in progmming address %x on block %u.\n", copy_base, blk_num );
    	    if( status == NF_OVERTIME_ERR )
    	    	return ADV_NF_OVERTIME_ERR;
    	    else if( status == NF_PROG_ERR )
    	    {
	            if( NF_verify_block( blk_num ) == NF_GOOD_BLOCK )
	            	goto try_again;               // 给予“擦--写--写出错--校验是好块”这种情况有限次数（TOLERANCE）的机会
	            else
					goto failure;		          // 经检验确实是坏块，先擦除后标记
	        }
	    }
	    break;	                                  // 成功填充满了当前块，跳出当前循环

try_again:
		continue;
	}


	if( j < TOLERANCE )                           // 成功
	{
//	   	__msg("succeed in programming block %u.\n", blk_num);
		return ADV_NF_OK;
	}


failure:
		__msg("fail in programming block %u, it is bad block.\n", blk_num);
	/* 先擦除，后标记 */
	NF_erase( blk_num );
	NF_mark_bad_block( blk_num );
	return ADV_NF_NEW_BAD_BLOCK;
}
//#pragma arm section  code="write_in_many_blks"
/*******************************************************************************
*函数名称: write_in_many_blks
*函数原型：int32 write_in_many_blks( __u32 start_blk, __u32 last_blk_num, void *buf,
*				             	     __u32 size, __u32 blk_size, __u32 * blks )
*函数功能: 将内存中的数据顺序写入到多个块中
*入口参数: start_blk         待访问的nand flash起始块号
*          last_blk_num      最后一个块的块号，用来限制访问范围
*          buf               缓冲区的起始地址
*          size              待写入数据的大小
*          blk_size          待访问的nand flash的块大小
*          blks              所占据的块数，包括坏块
*返 回 值: NO_NEW_BAD_BLOCK  写入成功，没有增加新坏块
*          NF_LACK_BLKS      块数不足
*          NF_NEW_BAD_BLOCK  写入成功，但增加了新坏块
*          OVERTIME_ERR   操作超时
*备    注: 数据从起始块开始依次、紧密排放。
*******************************************************************************/
__s32 write_in_many_blks( __u32 start_blk, __u32 last_blk_num, void *buf,
					      __u32 size, __u32 blk_size, __u32 * blks )
{
    __u32 buf_base;
	__u32 buf_off;
    __u32 size_writed;
    __u32 rest_size;
    __u32 blk_num;
    __u32 blk_status;
    __s32 status;


	for( blk_num = start_blk, buf_base = (__u32)buf, buf_off = 0, blk_status = ADV_NF_NO_NEW_BAD_BLOCK;
         blk_num <= last_blk_num && buf_off < size;
         blk_num++ )
    {
    	status = NF_read_status( blk_num );
    	if( status == NF_OVERTIME_ERR )
    		return ADV_NF_OVERTIME_ERR;
    	else if( status == NF_BAD_BLOCK )		            // 如果当前块是坏块，则进入下一块
    		continue;

    	rest_size = size - buf_off ;                        // 未载入部分的尺寸
    	size_writed = ( rest_size < blk_size ) ?  rest_size : blk_size ;  // 确定此次待载入的尺寸

    	status = write_in_one_blk( blk_num, (void *)buf_base, size_writed, blk_size );
    	if( status == NF_OVERTIME_ERR )
    		return ADV_NF_OVERTIME_ERR;
    	else if( status == NF_ERASE_ERR )
    	{
    		blk_status = ADV_NF_NEW_BAD_BLOCK;
       		continue;
       	}

    	buf_base += size_writed;
    	buf_off  += size_writed;
    }

    *blks = blk_num - start_blk;                            // 返回占用的块数（包括坏块）
    if( buf_off == size )
		return blk_status;                                  // 成功，返回坏块的增添情况
	else
		return ADV_NF_LACK_BLKS;                                // 失败，因为块不足
}
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
__s32  Nand_Burn_Boot1(__u32 Boot1_buf, __u32 length )
{
    __u32 	            i;
    __u32               write_blks;
	__s32               status;
	__s32				ret = -1;

    if(NF_ERROR==NF_open( ))                                             // 打开nand flash
    {
        __inf("%s: NF_open fail !\n",__FUNCTION__);
		return -1;
    }
    if( length <= NF_BLOCK_SIZE )
    {
        for( i = BOOT1_START_BLK_NUM;  i <= BOOT1_LAST_BLK_NUM;  i++ )
        {
            //if( NF_read_status( i ) == NF_BAD_BLOCK )		// 如果当前块是坏块，则进入下一块
            //    continue;

            /* 在当前块中填充满Boot1的备份 */
            status = write_in_one_blk( i, (void *)Boot1_buf, length, NF_BLOCK_SIZE );
            if( status == ADV_NF_OVERTIME_ERR )
                goto over_time;
        }
    }
    else
    {
    	for( i = BOOT1_START_BLK_NUM;  i <= BOOT1_LAST_BLK_NUM;  i++ )
        {
            //if( NF_read_status( i ) == NF_BAD_BLOCK )		// 如果当前块是坏块，则进入下一块
            //    continue;

        	status = write_in_many_blks( i, BOOT1_LAST_BLK_NUM, (void*)Boot1_buf,
            	                        length, NF_BLOCK_SIZE, &write_blks );
            if( status == ADV_NF_OVERTIME_ERR )
                goto over_time;

        	i = i + write_blks -1;
        }
	}

	ret = 0;
over_time:

    NF_close( );

    return ret;
}









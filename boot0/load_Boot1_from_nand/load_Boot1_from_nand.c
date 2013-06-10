/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : load_Boot1_from_nand.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.10.14
*
* Description : This file provides a function "load_Boot1_from_nand" to load a good copy of Boot1
*             from outside nand flash chips to SRAM.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.10.14      1.1.0        build the file
*
************************************************************************************************************************
*/
#include "load_Boot1_from_nand_i.h"




/*******************************************************************************
*函数名称: load_Boot1_from_nand
*函数原型：int32 load_Boot1_from_nand( void )
*函数功能: 将一份好的Boot1从nand flash中载入到SRAM中。
*入口参数: void
*返 回 值: OK                         载入并校验成功
*          ERROR                      载入并校验失败
*备    注:
*******************************************************************************/
__s32 load_Boot1_from_nand( void )
{
    __u32 i;
    __s32  status;
    __u32 length;
    __u32 read_blks;
	boot_file_head_t  *bfh;


	if(NF_ERROR==NF_open( ))                         // 打开nand flash
	{
		msg("can't open nand flash.\n");
		return ERROR;
	}
	msg("Succeed in opening nand flash.\n");
	msg("block from %d to %d\n", BOOT1_START_BLK_NUM, BOOT1_LAST_BLK_NUM);
    for( i = BOOT1_START_BLK_NUM;  i <= BOOT1_LAST_BLK_NUM;  i++ )
    {
    	msg("deal block %d\n", i);
    	if( NF_read_status( i ) == NF_BAD_BLOCK )		// 如果当前块是坏块，则进入下一块
    	{
    		msg("nand block %d is bad\n", i);
            continue;
		}
        /* 载入当前块最前面512字节的数据到SRAM中，目的是获取文件头 */
        if( NF_read( i << ( NF_BLK_SZ_WIDTH - NF_SCT_SZ_WIDTH ), (void *)BOOT1_BASE, 1 )  == NF_OVERTIME_ERR )
        {
		    msg("the first data is error\n");
			continue;
		}
		msg("Succeed in reading Boot1 file head.\n");

		/* 察看是否是文件头 */
		if( check_magic( (__u32 *)BOOT1_BASE, BOOT1_MAGIC ) != CHECK_IS_CORRECT )
		{
			msg("ERROR! block %u doesn't store head of Boot1 copy.\n", i );
			continue;
		}


		bfh = (boot_file_head_t *) BOOT1_BASE;
        length =  bfh->length;
      //  msg("The size of Boot1 is %x.\n", length );
      //  msg("The align size of Boot1 is %x.\n", bfh->align_size);
        if( ( length & ( NF_SECTOR_SIZE - 1 ) ) != 0 )     // length必须是NF_SECTOR_SIZE对齐的
        {
            msg("the boot1 is not aligned by %x\n", NF_SECTOR_SIZE);
        	continue;
		}
		msg("The size of Boot1 is %x.\n", length );

        if( length <=  NF_BLOCK_SIZE )
        {
        	/* 从一个块中载入Boot1的备份 */
        	status = load_and_check_in_one_blk( i, (void *)BOOT1_BASE, length, NF_BLOCK_SIZE, BOOT1_MAGIC );
        	if( status == ADV_NF_OVERTIME_ERR )            // 块数不足
        	{
        		msg("one block ADV_NF_OVERTIME_ERR\n");
        		continue;
        	}
        	else if( status == ADV_NF_OK )
        	{
                msg("Check is correct.\n");
                bfh->eGON_vsn[2] = 0;
                NF_close( );                        // 关闭nand flash
                return OK;
            }
        }
        else
        {
        	/* 从多个块中载入一份Boot1的备份 */
        	status = load_in_many_blks( i, BOOT1_LAST_BLK_NUM, (void*)BOOT1_BASE,
        								length, NF_BLOCK_SIZE, &read_blks );
        	if( status == ADV_NF_LACK_BLKS )        // 块数不足
        	{
        		msg("ADV_NF_LACK_BLKS\n");
        		NF_close( );                        // 关闭nand flash
        		return ERROR;
        	}
        	else if( status == ADV_NF_OVERTIME_ERR )
        	{
        		msg("mult block ADV_NF_OVERTIME_ERR\n");
        		continue;
			}
            if( check_sum( (__u32 *)BOOT1_BASE, length ) == CHECK_IS_CORRECT )
            {
                msg("The file stored in start block %u is perfect.\n", i );
                bfh->eGON_vsn[2] = 0;
                NF_close( );                        // 关闭nand flash
                return OK;
            }
        }
    }


	msg("Can't find a good Boot1 copy in nand.\n");
    NF_close( );                        // 关闭nand flash
    msg("Ready to quit \"load_Boot1_from_nand\".\n");
    return ERROR;
}


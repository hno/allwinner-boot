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

static __s32  burn_boot0_1k_per_page( __u32 Boot0_buf );
static __s32  burn_boot0_lsb_mode( __u32 read_retry_type, __u32 Boot0_buf );
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
__s32  Nand_Burn_Boot0(__u32 Boot0_buf, __u32 length )
{
	boot_nand_para_t    nand_param;
    __s32				ret = -1;
    __u32 read_retry_type, read_retry_mode;

	if( NAND_Init( ) == FAIL )
		return -1;

	NAND_GetParam(&nand_param);

	NAND_Exit();

	read_retry_type = (nand_param.ReadRetryType);
	read_retry_mode = (read_retry_type>>16)&0xff;

	if( (read_retry_type>0)&&(read_retry_mode < 0x10))
	{
	    if( burn_boot0_lsb_mode(read_retry_type, Boot0_buf) )
	        goto error;
	}
	else
	{
	    if( burn_boot0_1k_per_page(Boot0_buf) )
	        goto error;
	}

	ret = 0;

error:

    return ret;
}

__s32 NAND_LSBEnable(__u32 chip, __u32 read_retry_type)
{
	return (NFC_LSBEnable(chip, read_retry_type));
}

__s32 NAND_LSBDisable(__u32 chip, __u32 read_retry_type)
{
	return (NFC_LSBDisable(chip, read_retry_type));
}

__s32 NAND_LSBInit(__u32 read_retry_type)
{
	return (NFC_LSBInit(read_retry_type));
}

__s32 NAND_LSBExit(__u32 read_retry_type)
{
	return (NFC_LSBExit(read_retry_type));
}


static __s32  burn_boot0_1k_per_page( __u32 Boot0_buf )
{
    __u32 	            i;
    __u32               length;
	struct boot_flash_info info;
	__u32               pages_per_block;
	__u32               copies_per_block;
	__u32 				nand_version;
	__u32			 	nand_version0, nand_version1;
    __u8  				oob_buf[32];

    __inf("burn boot0 normal mode!\n");

	memset(oob_buf, 0xff, 32);
    nand_version = NAND_GetNandVersion();
    nand_version0 = (nand_version>>16)&0xff;
    nand_version1 = (nand_version>>24)&0xff;
    oob_buf[0] = 0xff;
    oob_buf[1] = 0x00;
    oob_buf[2] = nand_version0;
    oob_buf[3] = nand_version1;

	if( NAND_PhyInit( ) == FAIL )
		return -1;
	__inf("open nand.\n");
	if( NAND_GetFlashInfo( &info ) == FAIL )
	{
		__inf("get flash info failed.\n");
		goto error;
	}

	/* 检查 page count */
	pages_per_block = info.blocksize / info.pagesize;
	if( pages_per_block%BOOT0_PAGE_ADVANCE != 0 )
	{
		__inf("Error! The page count of one block is %d, not boot0-page-advance %d aligned.\n",
		    pages_per_block, BOOT0_PAGE_ADVANCE);
		goto error;
	}

	copies_per_block = pages_per_block / BOOT0_PAGE_ADVANCE;
    for( i = BOOT0_START_BLK_NUM;  i <= BOOT0_LAST_BLK_NUM;  i++ )
    {
		struct boot_physical_param  para;
		__u32  j;
		__u32  k;

        __inf("boot0 %x \n", i);

		/* 擦除块 */
		para.chip  = 0;
		para.block = i;
		if( NAND_PhyErase( &para ) != SUCCESS )
		{
		    __inf("Fail in erasing block %d.\n", i );
    		continue;
    	}

        /* 在块中烧写boot0备份 */
        for( j = 0;  j < copies_per_block;  j++ )
       	{

			for( k = 0;  k < BOOT0_PAGE_ADVANCE;  k++ )
			{
				para.chip  = 0;
				para.block = i;
				para.page = j * BOOT0_PAGE_ADVANCE + k;
				para.mainbuf = (void *) (Boot0_buf + k * SZ_1K);
				para.oobbuf = oob_buf;
				if( NAND_PhyWrite_1K( &para ) != SUCCESS )
				{
					__inf("Warning. Fail in writing page %d in block %d.\n", j * BOOT0_PAGE_ADVANCE + k, i );
       			}
       		}
       	}
    }

    NAND_PhyExit( );
	return 0;

error:
    NAND_PhyExit( );
    return -1;
}




static __s32  burn_boot0_lsb_mode( __u32 read_retry_type, __u32 Boot0_buf )
{
    __u32 	            i;
    __u32               length;
	struct boot_flash_info info;
	__u32 nand_version, nand_version0, nand_version1;
    __u8  oob_buf[32];
    __u32 page_size;

     __inf("burn boot0 lsb mode!\n");

    memset(oob_buf, 0xff, 32);
    nand_version = NAND_GetNandVersion();
    nand_version0 = (nand_version>>16)&0xff;
    nand_version1 = (nand_version>>24)&0xff;
    oob_buf[0] = 0xff;
    oob_buf[1] = 0x00;
    oob_buf[2] = nand_version0;
    oob_buf[3] = nand_version1;

	if( NAND_PhyInit( ) == FAIL )
		return -1;
	__inf("open nand.\n");
	__inf("read retry mode: 0x%x\n", read_retry_type);
	if( NAND_LSBInit(read_retry_type) )
	{
	    __inf("lsb init failed.\n");
		goto error;
	}
	NAND_LSBEnable(0, read_retry_type);
	__inf("lsb enalbe \n");
	if( NAND_GetFlashInfo( &info ) == FAIL )
	{
		__inf("get flash info failed.\n");
		goto error;
	}

	/* 检查 page count */
	page_size = info.pagesize*512;
	if(page_size == 8192*2) //change for h27ucg8t2btr 16k pagesize
		page_size = 8192;
    for( i = BOOT0_START_BLK_NUM;  i <= BOOT0_LAST_BLK_NUM;  i++ )
    {
		struct boot_physical_param  para;
		__u32  k;

        __inf("boot0 %x \n", i);

		/* 擦除块 */
		para.chip  = 0;
		para.block = i;
		if( NAND_PhyErase( &para ) != SUCCESS )
		{
		    __inf("Fail in erasing block %d.\n", i );
    		continue;
    	}

        /* 在块中烧写boot0备份, lsb mode下，每个块只能写前4个page */
		for( k = 0;  k < 4;  k++ )
		{
			para.chip  = 0;
			para.block = i;
			para.page  = k;
			para.mainbuf = (void *) (Boot0_buf + k * page_size);
			para.oobbuf = oob_buf;
			if( NAND_PhyWrite_Seq( &para ) != SUCCESS )
			{
				__inf("Warning. Fail in writing page %d in block %d.\n", k, i );
   			}
   		}

    }

    //check boot0
    for( i = BOOT0_START_BLK_NUM;  i <= BOOT0_LAST_BLK_NUM;  i++ )
    {
		struct boot_physical_param  para;
		__u32  k;

        __inf("boot0 %x \n", i);

        /* 在块中烧写boot0备份, lsb mode下，每个块只能写前4个page */
		for( k = 0;  k < 4;  k++ )
		{
			para.chip  = 0;
			para.block = i;
			para.page  = k;
			para.mainbuf = (void *) (Boot0_buf + k * page_size);
			para.oobbuf = oob_buf;
			if( NAND_PhyRead_Seq( &para ) != SUCCESS )
			{
				__inf("Warning. Fail in reading page %d in block %d.\n",  k, i );
   			}
   		}

    }

    NAND_LSBDisable(0, read_retry_type);
    NAND_LSBExit(read_retry_type);
	__inf("lsb disalbe \n");
    NAND_PhyExit( );
	return 0;

error:
    NAND_PhyExit( );
    return -1;
}






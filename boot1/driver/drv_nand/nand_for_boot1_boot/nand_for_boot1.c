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

#include "egon2.h"
#include "boot0_v2.h"
#include "bsp_nand.h"
#include "string.h"


#define  OOB_BUF_SIZE                   32

static __u32 nand_good_block_ratio_flag = 0;
static __u32 nand_good_blk_ratio = 0;


extern __s32 NAND_Print(const char * str, ...);
/*
************************************************************************************************************************
*                       READ ONE SINGLE PAGE
*
*Description: read one page data from nand based on single plane;
*
*Arguments  : *readop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  read ok;
*               = FAIL    read fail.
************************************************************************************************************************
*/
__s32 NAND_PhyRead (struct boot_physical_param *readop)
{

	return(PHY_SimpleRead (readop));

}

/*
************************************************************************************************************************
*                       READ ONE SINGLE PAGE
*
*Description: read one page data from nand based on single plane;
*
*Arguments  : *readop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  read ok;
*               = FAIL    read fail.
************************************************************************************************************************
*/
__s32 NAND_PhyRead_Seq (struct boot_physical_param *readop)
{

	return(PHY_SimpleRead_Seq (readop));

}

__s32 NAND_PhyRead_1K (struct boot_physical_param *readop)
{

	return(PHY_SimpleRead_1K (readop));

}

/*
************************************************************************************************************************
*                       WRITE ONE SINGLE PAGE
*
*Description: write one page data to nand based on single plane;
*
*Arguments  : *writeop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  	write ok;
*               = FAIL    	write fail.
*				= BADBLOCK	write fail and bad block made by program
************************************************************************************************************************
*/
__s32 NAND_PhyWrite (struct boot_physical_param *writeop)
{
	return (PHY_SimpleWrite(writeop));
}
__s32 NAND_PhyWrite_1K (struct boot_physical_param *writeop)
{
	return (PHY_SimpleWrite_1K(writeop));
}
__s32 NAND_PhyWrite_Seq (struct boot_physical_param *writeop)
{
	return (PHY_SimpleWrite_Seq(writeop));
}


/*
************************************************************************************************************************
*                       ERASE ONE SINGLE BLOCK
*
*Description: erase one block in nand based on single plane;
*
*Arguments  : *eraseop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  	erase ok;
*               = FAIL    	erase fail.
*				= BADBLOCK	erase fail and bad block made by erase
************************************************************************************************************************
*/
__s32 NAND_PhyErase(struct boot_physical_param *eraseop)
{
	return (PHY_SimpleErase (eraseop ));
}

/*
************************************************************************************************************************
*                       INIT NAND FLASH
*
*Description: initial nand flash,request hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  initial ok;
*               = FAIL    initial fail.
************************************************************************************************************************
*/
__s32 NAND_PhyInit(void)
{
	__s32 ret;
//	__u32 nand_good_blk_ratio;
	boot_nand_para_t param;

//	if(!nand_good_block_ratio_flag)
//	{
//    	if(wBoot_get_para( WBOOT_PARA_NANDFLASH_INFO, (void *)&param))
//    	{
//			NAND_Print("get good block ratio info failed.\n");
//			return -1;
//    	}
//    	nand_good_blk_ratio = param.good_block_ratio;
//    }
//    else
//    {
//    	nand_good_blk_ratio = nand_good_block_ratio_flag;
//    }
//	NAND_Print("get the good blk ratio from hwscan : %d \n", nand_good_blk_ratio);
	NAND_Print("NB1 : enter phy init\n");

    ret = PHY_Init();
	if (ret)
	{
		NAND_Print("NB1 : nand phy init fail\n");
		return ret;
	}

	ret = SCN_AnalyzeNandSystem();
	if (ret)
	{
		NAND_Print("NB1 : nand scan fail\n");
		return ret;
	}

	//modify ValidBlkRatio
	if(wBoot_get_para( WBOOT_PARA_NANDFLASH_INFO, (void *)&param))
  {
		NAND_Print("get good block ratio info failed.\n");
		return -1;
  }
  else
  {
  	nand_good_blk_ratio = param.good_block_ratio;
  	if(nand_good_blk_ratio == 0)
  		 NAND_Print("get the good blk ratio is 0,use preset value\n");
  	else
  	{
  		NAND_SetValidBlkRatio(nand_good_blk_ratio);
  		NAND_Print("get the good blk ratio from hwscan : %d \n", nand_good_blk_ratio);
		}
	}
  
	NAND_Print("NB1 : nand phy init ok\n");
	return(PHY_ChangeMode(1));
}

/*
************************************************************************************************************************
*                       RELEASE NAND FLASH
*
*Description: release  nand flash and free hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  release ok;
*               = FAIL    release fail.
************************************************************************************************************************
*/
__s32 NAND_PhyExit(void)
{
	PHY_Exit();

	return 0;
}
/*
************************************************************************************************************************
*                       WRITE SOME LOGICAl SECTORS
*
*Description: write some logic area;
*
*Arguments  : nSectnum   - start sector number
*			  nSectorCnt - sector count
*			  *pBuf		 - data buffer
*Return     :   = SUCESS  	write ok;
*               = FAIL    	write fail.

************************************************************************************************************************
*/
__s32 NAND_LogicWrite(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
{
    #ifdef BOOT_CACHE_OPEN
	    return (NAND_CacheWrite(nSectNum, nSectorCnt, pBuf));
	#else
	    return (LML_Write(nSectNum, nSectorCnt, pBuf));
	#endif
}
/*
************************************************************************************************************************
*                       READ SOME LOGICAl SECTORS
*
*Description: read some logic area;
*
*Arguments  : nSectnum   - start sector number
*			  nSectorCnt - sector count
*			  *pBuf		 - data buffer
*Return     :   = SUCESS  	read ok;
*               = FAIL    	read fail.

************************************************************************************************************************
*/
__s32 NAND_LogicRead(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
{
    #ifdef BOOT_CACHE_OPEN
	    return NAND_CacheRead(nSectNum,nSectorCnt,pBuf);
      #else
	    return LML_Read(nSectNum,nSectorCnt,pBuf);
	#endif

}
/*
************************************************************************************************************************
*                       INIT NAND DRIEVER
*
*Description: configure nand flash driver;
*
*Arguments  : void
*Return     :   = SUCESS  	init ok;
*               = other    	init fail.

************************************************************************************************************************
*/
__s32 NAND_Init(void)
{
	__s32  result;

	NAND_Print("NB1 : enter NFB_Init\n");

	result = NAND_PhyInit();
	if (result < 0)
	{
		NAND_Print("NB1 : phy init fail\n");
		return -1;
	}

    //Format nand flash for logical access
    result = FMT_Init();
    if(result < 0)
    {
    	NAND_Print("NB1 : format init fail\n");
		return -5;
    }

    result = FMT_FormatNand();
    if(result < 0)
    {
    	NAND_Print("NB1 : format fail\n");
        return -6;
    }

    result = FMT_Exit();
    if(result < 0)
    {
        return -7;
    }

    //Init nand flash logical module
    result = LML_Init();
    if(result < 0)
    {
         return -8;
    }
#ifdef BOOT_CACHE_OPEN
    result = NAND_CacheOpen();
#endif

    NAND_Print("NB1 : init ok\n");
    return result;
}
/*
************************************************************************************************************************
*                       EXIT NAND DRIVER
*
*Description: ;
*
*Arguments  : void
*Return     :   = SUCESS  	exit ok;
*               = FAIL    	exit fail.

************************************************************************************************************************
*/
__s32 NAND_Exit(void)
{
	__s32   result;

#ifdef BOOT_CACHE_OPEN
    NAND_CacheClose();
#endif

    //exit nand flahs logic module
    result = LML_Exit();
    if(result < 0)
    {
        return -1;
    }

    //exit nand flash physical module
    result = NAND_PhyExit();
    if(result < 0)
    {
        return -2;
    }
    return result;
}

/*
************************************************************************************************************************
*                       SCAN NAND HW
*
*Description: initial nand flash,request hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  initial ok;
*               = FAIL    initial fail.
************************************************************************************************************************
*/
__s32 NAND_HWScanStart(boot_nand_para_t *nand_param)
{
	__s32 ret;

	NAND_Print("NHW : start nand scan\n");
	ret = PHY_Init();
	if (ret)
	{
		NAND_Print("NHW : nand phy init fail\n");
		return ret;
	}

	ret = SCN_AnalyzeNandSystem();
	if (ret)
	{
		NAND_Print("NHW : nand scan fail\n");
		return ret;
	}

	NAND_GetParam(nand_param);

	nand_good_block_ratio_flag = nand_param->good_block_ratio;
	NAND_Print("NHW : nand hw scan ok\n");

	return(PHY_ChangeMode(1));
}

/*
************************************************************************************************************************
*                       RELEASE NAND FLASH
*
*Description: release  nand flash and free hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  release ok;
*               = FAIL    release fail.
************************************************************************************************************************
*/
__s32 NAND_HWScanStop(void)
{
	PHY_Exit();

	NAND_Print("NHW : nand hw exit\n");

	return 0;
}

static __u32 max_badblk(__u32 v0, __u32 v1)
{
	__u32 val;

	if(v0 > v1)
		val = v0;
	else
		val = v1;

	return val;
}

static __s32 _cal_real_chip(__u32 global_bank, __u32 chip_connect_info)
{
	__u32 chip;
	__u32 i,cnt;

	cnt = 0;
	chip = global_bank;

	for (i = 0; i < 8; i++ )
	{
		if (chip_connect_info & (1 << i))
            {
    			cnt++;
    			if (cnt == (chip+1))
    			    {
        				chip = i;
        				return chip;
        			}
    		}
	}

	return -1;
}

/*******************************************************************************
*函数名称: mark_bad_block
*函数原型：mark_bad_block( __u32 chip_num, __u32 blk_num )
*函数功能: 标记blk_num指定的块为坏块。
*入口参数: chip_num           chip number
           blk_num            块号
*返 回 值: 0              编程操作成功
*          other          编程操作失败
*备    注:
*******************************************************************************/
static __s32 mark_bad_block( __u32 chip_num, __u32 blk_num)
{
    boot_flash_info_t info;
	struct boot_physical_param  para;
	__u8   oob_buf[OOB_BUF_SIZE];
	__u8*  page_buf;
	__s32  page_index[4];
    __u32  page_with_bad_block, page_per_block;
	__u32  i;
	__s32  mark_err_flag = -1;


	if( NAND_GetFlashInfo( &info ))
	{
		NAND_Print("get flash info failed.\n");
		return -1;
	}

	//cal nand parameters
	//page_buf = (__u8*)(MARK_BAD_BLK_BUF_ADR);
    page_buf = (__u8*)wBoot_malloc(32 * 1024);
    if(!page_buf)
    {
        NAND_Print("malloc memory for page buf fail\n");
        return -1;
    }

	page_with_bad_block = info.pagewithbadflag;
	page_per_block = info.blocksize/info.pagesize;

	//read the first, second, last, last-1 page for check bad blocks
	page_index[0] = 0;
	page_index[1] = 0xEE;
	page_index[2] = 0xEE;
	page_index[3] = 0xEE;

    switch(page_with_bad_block & 0x03)
    {
        case 0x00:
            //the bad block flag is in the first page, same as the logical information, just read 1 page is ok
            break;

        case 0x01:
            //the bad block flag is in the first page or the second page, need read the first page and the second page
            page_index[1] = 1;
            break;

        case 0x02:
            //the bad block flag is in the last page, need read the first page and the last page
            page_index[1] = page_per_block - 1;
            break;

        case 0x03:
            //the bad block flag is in the last 2 page, so, need read the first page, the last page and the last-1 page
            page_index[1] = page_per_block - 1;
            page_index[2] = page_per_block - 2;
            break;
    }

	for(i =0; i<4; i++)
	{
		oob_buf[0] = 0x0;
		oob_buf[1] = 0x1;
		oob_buf[2] = 0x2;
		oob_buf[3] = 0x3;
		oob_buf[4] = 0x89;
		oob_buf[5] = 0xab;
		oob_buf[6] = 0xcd;
		oob_buf[7] = 0xef;

		para.chip = chip_num;
		para.block = blk_num;
		para.page = page_index[i];
		para.mainbuf = page_buf;
		para.oobbuf = oob_buf;

		if(para.page == 0xEE)
		    continue;

        NAND_PhyWrite( &para );
        NAND_PhyRead( &para );

		if(oob_buf[0] !=0xff)
			mark_err_flag = 0;
	}

	wBoot_free(page_buf);

	return mark_err_flag;
}
/*
************************************************************************************************************************
*                       NAND_VersionGet
*
*Description: Get the nand flash driver is current version
*
*Arguments  : pointer.
*
*Return     :   = 0  version get;
*
************************************************************************************************************************
*/
__s32 NAND_VersionGet(__u8 *version)
{
    version[0] = 0xff;     //bad block flag
    version[1] = 0x00;     //reserved, set to 0x00
    version[2] = NAND_VERSION_0;     //nand driver version 0, current vresion is 0x02
    version[3] = NAND_VERSION_1;     //nand driver version 1, current vresion is 0x00

    return 0;
}
/*
************************************************************************************************************************
*                       NAND_VersionCheck
*
*Description: Check the nand flash driver is current version
*
*Arguments  : void.
*
*Return     :   = 0  version match;
*                 1  version not match
*                -1  can't get invalid version info
************************************************************************************************************************
*/
__s32 NAND_VersionCheck(void)
{
	struct boot_physical_param boot0_readop_temp;
	struct boot_physical_param *boot0_readop = NULL;
	__u32 block_index;
	__u32 cnt1;
	__s32 version_match_flag = -1;
	//__u32 chip_type;
	__s32 i;
	__u8  oob_buf[32];
	__u32* main_data;
	__u8  nand_version[4];

    /********************************************************************************
    *   nand_version[2] = 0xFF;          //the sequnece mode version <
    *   nand_version[2] = 0x01;          //the first interleave mode version, care ecc
    *                                      2010-06-05
    *   nand_version[2] = 0x02;          //the current version, don't care ecc
    *                                      2010-07-13
    *   NOTE:  need update the nand version in update_boot0 at the same time
    ********************************************************************************/
    nand_version[0] = 0xff;     //bad block flag
    nand_version[1] = 0x00;     //reserved, set to 0x00
    nand_version[2] = NAND_VERSION_0;     //nand driver version 0, current vresion is 0x02
    nand_version[3] = NAND_VERSION_1;     //nand driver version 1, current vresion is 0x00

    NAND_Print("check nand version start.\n");
	NAND_Print("Current nand driver version is %x %x %x %x \n", nand_version[0], nand_version[1], nand_version[2], nand_version[3]);

    boot0_readop = &boot0_readop_temp;

	//init boot0_readop
	boot0_readop->block = 0x0;
	boot0_readop->chip = 0;
	boot0_readop->mainbuf = (void*)wBoot_malloc(32 * 1024);
    if(!boot0_readop->mainbuf)
    {
        NAND_Print("malloc memory for boot0 read operation fail\n");
        return -1;
    }

	boot0_readop->oobbuf = oob_buf;
	boot0_readop->page = 0;
	boot0_readop->sectorbitmap = 0;

    main_data = (__u32*)wBoot_malloc(32 * 1024);
    if(!main_data)
    {
        NAND_Print("malloc memory for main data fail\n");
        return -1;
    }

    //scan boot0 area blocks
	for(block_index=BOOT0_START_BLK_NUM;block_index<BOOT0_LAST_BLK_NUM + 1;block_index++)
	{

		boot0_readop->block = block_index;
		boot0_readop->page = 0;
		cnt1 = 0;

        NAND_PhyRead_1K(boot0_readop);

        //check the current block is a bad block
		if(oob_buf[0] != 0xFF)
	    {
			NAND_Print("block %u is bad block.\n",block_index);
			continue;
	    }


		//check the current block is a all 0xFF block
		for(i=0; i<256; i++)
		{
			if(*(main_data+i) == 0xffffffff)
				cnt1++;
		}

		if(cnt1 == 256)
	    {
			NAND_Print("block %u is cleared block.\n",block_index);
			continue;
	    }

	    if((oob_buf[1] == 0x00) || (oob_buf[1] == 0xFF))
	    {
	       NAND_Print("Media version is valid in block %u, version info is %x %x %x %x \n", block_index, oob_buf[0], oob_buf[1], oob_buf[2], oob_buf[3]);
	       if(oob_buf[2] == nand_version[2])
	       {
	            NAND_Print("nand driver version match ok in block %u.\n",block_index);
    		    version_match_flag = 0;
    		    break;
	       }
	       else
	       {
	            NAND_Print("nand driver version match fail in block %u.\n",block_index);
    		    version_match_flag = 1;
    		    break;
	       }

	    }
	    else
	    {
	        NAND_Print("Media version is invalid in block %uversion info is %x %x %x %x \n", block_index, oob_buf[0], oob_buf[1], oob_buf[2], oob_buf[3]);
	    }

	}

    if(block_index == (BOOT0_LAST_BLK_NUM + 1))
    {
         NAND_Print("can't find valid version info in boot blocks. \n");
         version_match_flag = -1;
    }

    wBoot_free(main_data);
	return version_match_flag;
}

/*
************************************************************************************************************************
*                       NAND_EraseBootBlocks
*
*Description: Check the nand flash driver is current version
*
*Arguments  : connecnt info.
*
*Return     :   = 0     OK;
*               other  Fail.
************************************************************************************************************************
*/
__s32  NAND_EraseBootBlocks( const boot_nand_para_t *nand_param)
{

    struct boot_physical_param  para;
	__s32  i;
	__s32  ret;

	NAND_Print("Ready to erase boot blocks.\n");

	for( i = 0;  i < 7;  i++ )
	{
		para.chip = 0;
		para.block = i;
		ret = NAND_PhyErase( &para ) ;
		if(ret)
		    NAND_Print("erasing block %u failed.\n", i );
	}

    NAND_Print("has cleared the boot blocks.\n");

    return 0;

}

/*
************************************************************************************************************************
*                       NAND_EraseChip
*
*Description: Erase chip, only erase the all 0x0 blocks
*
*Arguments  : connecnt info.
*
*Return     :   = 0     OK;
*               other  Fail.
************************************************************************************************************************
*/
__s32  NAND_EraseChip( const boot_nand_para_t *nand_param)
{
	boot_flash_info_t info;
	struct boot_physical_param  para_read;

	__s32  i,j,k,m;
	__s32  ret;
	__s32  cnt0, cnt1;
	__s32  mark_err_flag;
	__u32  bad_block_flag;
	__u32  page_with_bad_block, page_size, page_per_block;
	__s32  page_index[4];
	__u32  chip;
	__u8   oob_buf_read[OOB_BUF_SIZE];
	__u8*  page_buf_read;
	__s32  error_flag = 0;
	__s32  block_start;
	__u32  die_skip_flag = (nand_param->OperationOpt)&(0x1<<11);
	__u32  block_cnt_of_die = (nand_param->BlkCntPerDie);



    page_buf_read = (__u8*)wBoot_malloc(32 * 1024);
    if(!page_buf_read)
    {
        NAND_Print("malloc memory for page read fail\n");
        return -1;
    }
	NAND_Print("Ready to erase chip.\n");
	// get nand info to cal
	NAND_Print("nfb phy init ok.\n");
	if( NAND_GetFlashInfo( &info ) != 0 )
	{
		NAND_Print("get flash info failed.\n");
		wBoot_free(page_buf_read);
		return -1;
	}
	NAND_Print("Succeed in getting flash info.\n");

	page_size = 512*info.pagesize;
	page_with_bad_block = info.pagewithbadflag;
	page_per_block = info.blocksize/info.pagesize;
	NAND_Print("page size:%x, block size: %x, bad block position: %x.\n",page_size, page_per_block,page_with_bad_block);

    page_index[0] = 0;
	page_index[1] = 0xEE;
	page_index[2] = 0xEE;
	page_index[3] = 0xEE;

    switch(page_with_bad_block & 0x03)
    {
        case 0x00:
            //the bad block flag is in the first page, same as the logical information, just read 1 page is ok
            break;

        case 0x01:
            //the bad block flag is in the first page or the second page, need read the first page and the second page
            page_index[1] = 1;
            break;

        case 0x02:
            //the bad block flag is in the last page, need read the first page and the last page
            page_index[1] = page_per_block - 1;
            break;

        case 0x03:
            //the bad block flag is in the last 2 page, so, need read the first page, the last page and the last-1 page
            page_index[1] = page_per_block - 1;
            page_index[2] = page_per_block - 2;
            break;
    }

    NAND_Print("chip_cnt = %x, chip_connect = %x, rb_cnt = %x,  rb_connect = %x,  good_block_ratio =%x \n",nand_param->ChipCnt,nand_param->ChipConnectInfo,nand_param->RbCnt,nand_param->RbConnectInfo,nand_param->good_block_ratio);

	for( i = 0;  i < nand_param->ChipCnt;  i++ )
	{
	    //select chip
		chip = _cal_real_chip( i, nand_param->ChipConnectInfo );
        NAND_Print("erase chip %u \n", chip);

        //scan for bad blocks, only erase good block, all 0x00 blocks is defined bad blocks
		if(i == 0)
		{
			block_start = 7;
		}
		else
		{
			block_start = 0;
		}
		for( j = block_start;  j < info.blk_cnt_per_chip;  j++ )
		{
			para_read.chip = chip;
			if(!die_skip_flag)
			    para_read.block = j;
			else
			    para_read.block = j%block_cnt_of_die + 2*block_cnt_of_die*(j/block_cnt_of_die);
			para_read.mainbuf = page_buf_read;
			para_read.oobbuf = oob_buf_read;

			bad_block_flag = 0;

			for(k = 0; k<4; k++)
			{
				cnt0 =0;
				cnt1 =0;
				para_read.page = page_index[k];
				if( para_read.page== 0xEE)
				    break;

				ret = NAND_PhyRead(& para_read );

				//check the current block is a all 0x00 block

				for(m=0; m<8; m++)   //check user data, 8 byte
			    {
			        if(oob_buf_read[m] == ((__u8)0x0) )
			            cnt1++;
			        else
			            break;
			    }
				for(m=0; m<page_size; m++)  //check main data
				{
					if(page_buf_read[m] == ((__u8)0x0) )
						cnt0++;
					else
						break;
			    }

				if((cnt0 == page_size)&&(cnt1 == 8))
				{
					bad_block_flag = 1;
					NAND_Print("find a all 0x00 block %u\n", j);
					break;
				}

			}

			if(bad_block_flag)
				continue;

			ret = NAND_PhyErase( &para_read );
			if( ret != 0 )
	    		{
	    		    NAND_Print("erasing block %u failed.\n", j );
			    	mark_err_flag = mark_bad_block( i, j );
	    		    if( mark_err_flag!= 0 )
	    		        {
        					error_flag++;
        					NAND_Print("error in marking bad block flag in chip %u, block %u, mark error flag %u.\n", i, j, mark_err_flag);
        				}
	    		}

    		}
	}

	NAND_Print("has cleared the chip.\n");
	if(error_flag)
		NAND_Print("the nand is Bad.\n");
	else
		NAND_Print("the nand is OK.\n");

    wBoot_free(page_buf_read);

	return 0;

}

/*******************************************************************************
*函数名称: NAND_BadBlockScan
*函数原型：bad_blcok_scan(const boot_nand_para_t *nand_param)
*函数功能: 标记blk_num指定的块为坏块。
*入口参数: nand_param
*返 回 值: >0              编程操作成功
*          -1              编程操作失败
*备    注:
*******************************************************************************/
__s32 NAND_BadBlockScan(const boot_nand_para_t *nand_param)
{
	boot_flash_info_t info;
	struct boot_physical_param  para;

	__s32  i, j, k;
	__u32  page_with_bad_block, page_per_block;
	__s32  page_index[4];
	__u32  bad_block_cnt[8];
	__u32  bad_block_num = 0;
	__u32  good_block_num = 0;
	__s32  good_block_ratio = -1;
	__u32  chip;
	__u8   oob_buf[OOB_BUF_SIZE];
	__u8*  page_buf;
	__u32  die_skip_flag = (nand_param->OperationOpt)&(0x1<<11);
	__u32  block_cnt_of_die = (nand_param->BlkCntPerDie);

	for(i=0; i<8; i++)
	    bad_block_cnt[i] = 0;

	NAND_Print("Ready to scan bad blocks.\n");

    // get nand info to cal
	NAND_Print("nfb phy init ok.\n");
	if( NAND_GetFlashInfo( &info ) )
	    {
    		NAND_Print("get flash info failed.\n");
    		return -1;
    	}
	NAND_Print("Succeed in getting flash info.\n");

	//cal nand parameters
	//page_buf = (__u8*)(BAD_BLK_SCAN_BUF_ADR);
    page_buf = (__u8*)wBoot_malloc(32 * 1024);
    if(!page_buf)
    {
        NAND_Print("malloc memory for page buf fail\n");
        return -1;
    }

	page_with_bad_block = info.pagewithbadflag;
	page_per_block = info.blocksize/info.pagesize;

	//read the first, second, last, last-1 page for check bad blocks
	page_index[0] = 0;
	page_index[1] = 0xEE;
	page_index[2] = 0xEE;
	page_index[3] = 0xEE;

    switch(page_with_bad_block & 0x03)
    {
        case 0x00:
            //the bad block flag is in the first page, same as the logical information, just read 1 page is ok
            break;

        case 0x01:
            //the bad block flag is in the first page or the second page, need read the first page and the second page
            page_index[1] = 1;
            break;

        case 0x02:
            //the bad block flag is in the last page, need read the first page and the last page
            page_index[1] = page_per_block - 1;
            break;

        case 0x03:
            //the bad block flag is in the last 2 page, so, need read the first page, the last page and the last-1 page
            page_index[1] = page_per_block - 1;
            page_index[2] = page_per_block - 2;
            break;
    }

    //scan bad blocks
	for( i = 0;  i < info.chip_cnt;  i++ ){

		chip = _cal_real_chip( i, nand_param->ChipConnectInfo );
		NAND_Print("scan CE %u\n", chip);
		bad_block_cnt[chip] = 0;

		for( j = 0;  j < info.blk_cnt_per_chip;  j++ )
		{
			para.chip = chip;
			if(!die_skip_flag)
			    para.block = j;
			else
			    para.block = j%block_cnt_of_die + 2*block_cnt_of_die*(j/block_cnt_of_die);
			para.mainbuf = page_buf;
			para.oobbuf = oob_buf;

			for(k = 0; k<4; k++)
			{
				// read pages for check
				para.page = page_index[k];
				if(para.page == 0xEE)
				    continue;
				NAND_PhyRead(&para );

				// find bad blocks
				if(oob_buf[0] != 0xff)
				{
				    NAND_Print("find defined bad block in chip %u, block %u.\n", i, j);
					bad_block_cnt[chip]++;
                    break;
				}
			}
		}
	}

	// cal bad block num
	if(info.chip_cnt == 0x1)        //for one CE
    	{
    	    if(nand_param->ChipConnectInfo == 0x1)
    	        {
    	            bad_block_num = bad_block_cnt[0]<<1;
    	        }
    		else
    		    {
    		        NAND_Print("chip connect parameter %x error \n", nand_param->ChipConnectInfo);
    		        wBoot_free(page_buf);

        			return -1;
    		    }
    	}
	else if(info.chip_cnt == 2)     //for two CE
    	{
    		if(nand_param->ChipConnectInfo == 0x3)
    		    {
    			    bad_block_num = (bad_block_cnt[0] + bad_block_cnt[1])<<1;
    		    }
    		else if(nand_param->ChipConnectInfo == 0x5)
        		{
        			bad_block_num = (bad_block_cnt[0] + bad_block_cnt[2])<<1;
        		}
    		else if(nand_param->ChipConnectInfo == 0x81)
        		{
        			bad_block_num = (bad_block_cnt[0] + bad_block_cnt[7])<<1;
        		}
    		else
        		{
        			NAND_Print("chip connect parameter %x error \n", nand_param->ChipConnectInfo);
        			wBoot_free(page_buf);

        			return -1;
        		}
    	}
	else if(info.chip_cnt == 4)     //for four CE
	    {
    		if(nand_param->ChipConnectInfo == 0xf)
    		    {
    			    bad_block_num = max_badblk((bad_block_cnt[0] + bad_block_cnt[2]),(bad_block_cnt[1] + bad_block_cnt[3]))<<1;
    		    }
    		else if(nand_param->ChipConnectInfo == 0x55)
    		    {
    			    bad_block_num = max_badblk((bad_block_cnt[0] + bad_block_cnt[2]),(bad_block_cnt[4] + bad_block_cnt[6]))<<1;
    		    }
    		else
    		    {
    			    NAND_Print("chip connect parameter %x error \n",nand_param->ChipConnectInfo);
    			    wBoot_free(page_buf);

    			    return -1;
    		    }

	    }
	else if(info.chip_cnt == 8)     //for eight CE
	    {
    		if(nand_param->ChipConnectInfo == 0xff)
    		    {
        			bad_block_num = max_badblk((bad_block_cnt[0] + bad_block_cnt[2]),(bad_block_cnt[1] + bad_block_cnt[3]));
        			bad_block_num = 2*max_badblk(bad_block_num, max_badblk((bad_block_cnt[4] + bad_block_cnt[6]),(bad_block_cnt[5] + bad_block_cnt[7])));

        		}
    		else
    		    {
        			NAND_Print("chip connect parameter %x error \n",nand_param->ChipConnectInfo);
        			wBoot_free(page_buf);

        			return -1;
        		}
	    }
	else
	    {
	        NAND_Print("chip cnt parameter %x error \n",nand_param->ChipCnt);
	        wBoot_free(page_buf);

        	return -1;
	    }


	//cal good block num required per 1024 blocks
	good_block_num = (1024*(info.blk_cnt_per_chip - bad_block_num))/info.blk_cnt_per_chip -50;
    for(i=0; i<info.chip_cnt; i++)
    {
		chip = _cal_real_chip( i, nand_param->ChipConnectInfo );
		NAND_Print(" %d bad blocks in CE %u \n", bad_block_cnt[chip], chip);
	}
	NAND_Print("cal bad block num is %u \n", bad_block_num);
	NAND_Print("cal good block num is %u \n", good_block_num);

    //cal good block ratio
	for(i=0; i<10; i++)
	{
		if(good_block_num >= (nand_param->good_block_ratio - 32*i))
		    {
    			good_block_ratio = (nand_param->good_block_ratio - 32*i);
    			NAND_Print("good block ratio is %u \n",good_block_ratio);
    			break;
    		}
	}
    wBoot_free(page_buf);

 	return good_block_ratio;
}



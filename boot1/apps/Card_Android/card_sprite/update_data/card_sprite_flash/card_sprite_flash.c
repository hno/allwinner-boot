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
#include  "../update_data.h"
#include  "string.h"

static  int  card_sprite_type = 0;

/*
************************************************************************************************************
*
*                                             update_boot0_info
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
__s32 update_boot0_info(void *buf0, char *buf)
{
	boot0_file_head_t    *boot0     = (boot0_file_head_t *)buf0;

	if(!card_sprite_type)
	{
		boot_nand_para_t     *nand_info = (boot_nand_para_t *)buf;
		memcpy(&boot0->prvt_head.storage_data, nand_info, sizeof(boot_nand_para_t));
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             update_boot1_info
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
__s32 update_boot1_info(void *buf1, char *buf)
{
	boot1_file_head_t    *boot1     = (boot1_file_head_t *)buf1;
	boot_nand_para_t     *nand_info = (boot_nand_para_t *)buf;

	if(!card_sprite_type)
	{
		memcpy(&boot1->prvt_head.storage_data, nand_info, sizeof(boot_nand_para_t));
	}

	return 0;
}

/*
************************************************************************************************************
*
*                                             sprite_flash_open
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
__s32 sprite_flash_init(int *type)
{
	int  line_sel;
	int  card_offset;
	int  ret;

//    if(!card_sprite_type)
//	{
//		return NAND_Init();
//	}
    if(0==NAND_Init())
    {
        card_sprite_type=0;
        *type=card_sprite_type;
        return 0;
    }
    else
    {
        card_sprite_type=1;
        *type=card_sprite_type;
    	ret = wBoot_script_parser_fetch("card2_boot_para", "card_line", &line_sel, 1);
    	if(ret)
    	{
    		line_sel = 4;
    	}
    	ret = wBoot_script_parser_fetch("card_boot", "logical_start", &card_offset, 1);
    	if(ret)
    	{
    		card_offset = 40960;
    	}
        
    	return SDMMC_LogicalInit(2, card_offset, line_sel);

    }

}

/*
************************************************************************************************************
*
*                                             sprite_flash_close
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
__s32 sprite_flash_exit(int type)
{
	if(!type)
	{
		return NAND_Exit();
	}
	else if(type == 1)
	{
		return SDMMC_LogicalExit(2);
	}
	else
	{
		return -1;
	}
}


/*
************************************************************************************************************
*
*                                             sprite_flash_read
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
__s32 sprite_flash_read(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
{
	if(!card_sprite_type)
	{
		return NAND_LogicRead(nSectNum, nSectorCnt, pBuf);
	}
	else if(card_sprite_type == 1)
	{
		return SDMMC_LogicalRead(nSectNum, nSectorCnt, pBuf, 2);
	}
	else
	{
		return -1;
	}
}
/*
************************************************************************************************************
*
*                                             sprite_flash_write
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
__s32 sprite_flash_write(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
{
	if(!card_sprite_type)
	{
		return NAND_LogicWrite(nSectNum, nSectorCnt, pBuf);
	}
	else if(card_sprite_type == 1)
	{
		return SDMMC_LogicalWrite(nSectNum, nSectorCnt, pBuf, 2);
	}
	else
	{
		return -1;
	}
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
int sprite_flash_hardware_scan(void *mbr_i,void *flash_info, int erase_flash)
{
	int  ret,i;
	boot_nand_para_t  nand_para;
	int  line_sel;
	int  speed_mode;
	int  type;
    MBR* mbr_info=(MBR*)mbr_i;
	int good_block_ratio = 0;
	//扫描NAND

	type = 0;
//	ret = wBoot_script_parser_fetch("target", "storage_type", &type, 1);
//	__inf("storage type = %d\n", type);
//	if(type == 2)
//	{
//		goto __hardware_scan__;
//	}

    if(0==card_sprite_type)
    {
        if(NAND_HWScanStart(&nand_para))
    	{
    		__inf("sprite update error: hwscan start failed\n");
    		ret = -1;

    		goto __hardware_scan__;
    	}
    	if(erase_flash)
    	{
    		if(NAND_EraseChip((const boot_nand_para_t*)&nand_para))
    		{
    			__inf("sprite update error: erase flash failed\n");
    			ret = -2;

    			goto __hardware_scan__;
    		}
    	}
    	good_block_ratio = NAND_BadBlockScan((const boot_nand_para_t*)&nand_para);
    	if((good_block_ratio == -1)| (good_block_ratio == 0))
    	{
    			__inf("sprite update error: nand bad block scan failed\n");
    			ret = -2;

    			goto __hardware_scan__;
    	}
    	nand_para.good_block_ratio = good_block_ratio;
    	NAND_SetValidBlkRatio(good_block_ratio);
    	__inf("****************************************************************************************** \n");
    	__inf("get the good blk ratio from hwscan : %d \n", good_block_ratio);
    	//扫描结束
    	if(NAND_HWScanStop())
    	{
    		__inf("sprite update error: hwscan stop failed\n");
    		ret = -3;

    		goto __hardware_scan__;
    	}

    	memcpy(flash_info, &nand_para, sizeof(boot_nand_para_t));

    	return 0;
    }
__hardware_scan__:
	//nand操作失败，尝试操作卡
	ret = wBoot_script_parser_fetch("card2_boot_para", "card_line", &line_sel, 1);
	if(ret)
	{
		line_sel = 4;
	}
//	ret = wBoot_script_parser_fetch("card2_boot_para", "card_high_speed", &speed_mode, 1);
//	if(ret)
//	{
//		speed_mode = 1;
//	}
	ret = SDMMC_PhyInit(2,line_sel);
	if(ret == -1)
	{
		__inf("sprite error: try sdcard failed\n");

		return -1;
	}
	card_sprite_type = 1;
    
	if(erase_flash)
	{
      char *data;
      int erase_sector_count=16;//how many sector do you want to erase ?
      data = wBoot_malloc(512*erase_sector_count);
      memset(data,0xff,512*erase_sector_count);
      for(i=0;i<mbr_info->PartCount;i++)
      {
            __inf("erase sdcard part: %s ,start sector=%d,sector count=%d\n",\
                mbr_info->array[i].name,mbr_info->array[i].addrlo,erase_sector_count);
					
            if(SDMMC_LogicalWrite(mbr_info->array[i].addrlo,erase_sector_count,data,2))
            {
                __inf("erase sdcard part %s fail!\n",mbr_info->array[i].name);
                wBoot_free(data);
               return -1;
            }

      }
     wBoot_free(data);
	}

	SDMMC_PhyExit(2);

	return 1;

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
int create_stdmbr(void *mbr_i)
{
	mbr_stand  *mbrst;
	MBR        *mbr = (MBR *)mbr_i;
	char        mbr_bufst[512];
	int        i;
	int        size, usize;
//	char       tmp_buffer[] = {0xEB, 0x3C, 0x90, 								//跳转指令
//							   'M', 'S', 'D', 'O', 'S', '5', '.', '0', 			//厂商标志
//							   0x00, 0x02, 										//每扇区字节数
//							  };

	size = 0;
	for(i=1;i<mbr->PartCount-1;i++)
	{
		memset(mbr_bufst, 0, 512);
		mbrst = (mbr_stand *)mbr_bufst;

		size += mbr->array[i].lenlo;

		mbrst->part_info[0].part_type  	   = 0x83;
		mbrst->part_info[0].start_sectorl  = ((mbr->array[i].addrlo - i + 20 * 1024 * 1024/512 ) & 0x0000ffff) >> 0;
		mbrst->part_info[0].start_sectorh  = ((mbr->array[i].addrlo - i + 20 * 1024 * 1024/512 ) & 0xffff0000) >> 16;
		mbrst->part_info[0].total_sectorsl = ( mbr->array[i].lenlo & 0x0000ffff) >> 0;
		mbrst->part_info[0].total_sectorsh = ( mbr->array[i].lenlo & 0xffff0000) >> 16;

		if(i != mbr->PartCount-2)
		{
			mbrst->part_info[1].part_type  = 0x05;  //扩展分区
			mbrst->part_info[1].start_sectorl  = i;
			mbrst->part_info[1].start_sectorh  = 0;
			mbrst->part_info[1].total_sectorsl = (mbr->array[i].lenlo  & 0x0000ffff) >> 0;
			mbrst->part_info[1].total_sectorsh = (mbr->array[i].lenlo  & 0xffff0000) >> 16;
		}

		mbrst->end_flag = 0xAA55;
		if(SDMMC_PhyWrite(i, 1, mbr_bufst, 2))
		{
			__inf("write standard mbr %d failed\n", i);
			return -1;
		}
	}
	memset(mbr_bufst, 0, 512);
	mbrst = (mbr_stand *)mbr_bufst;

	//memcpy(mbr_bufst, tmp_buffer, 13);
	usize = SDMMC_PhyDiskSize(2) - 20 * 1024 * 1024/512 - size;
    //udisk
	mbrst->part_info[0].indicator = 0x80; //活动分区  
	mbrst->part_info[0].part_type = 0x0B; //fat32分区
	mbrst->part_info[0].start_sectorl  = ((mbr->array[mbr->PartCount-1].addrlo + 20 * 1024 * 1024/512 ) & 0x0000ffff) >> 0;
	mbrst->part_info[0].start_sectorh  = ((mbr->array[mbr->PartCount-1].addrlo + 20 * 1024 * 1024/512 ) & 0xffff0000) >> 16;
	mbrst->part_info[0].total_sectorsl = ( usize & 0x0000ffff) >> 0;
	mbrst->part_info[0].total_sectorsh = ( usize & 0xffff0000) >> 16;
    //bootloader
	mbrst->part_info[1].part_type = 0x06; //fat16分区
	mbrst->part_info[1].start_sectorl  = ((mbr->array[0].addrlo + 20 * 1024 * 1024/512) & 0x0000ffff) >> 0;
	mbrst->part_info[1].start_sectorh  = ((mbr->array[0].addrlo + 20 * 1024 * 1024/512) & 0xffff0000) >> 16;
	mbrst->part_info[1].total_sectorsl = (mbr->array[0].lenlo  & 0x0000ffff) >> 0;
	mbrst->part_info[1].total_sectorsh = (mbr->array[0].lenlo  & 0xffff0000) >> 16;
    //ebr
	mbrst->part_info[2].part_type = 0x05;//扩展分区
	mbrst->part_info[2].start_sectorl  = 1;
	mbrst->part_info[2].start_sectorh  = 0;
	mbrst->part_info[2].total_sectorsl = (size & 0x0000ffff) >> 0;
	mbrst->part_info[2].total_sectorsh = (size & 0xffff0000) >> 16;

	mbrst->end_flag = 0xAA55;
	if(SDMMC_PhyWrite(0, 1, mbr_bufst, 2))
	{
		__inf("write standard mbr 0 failed\n");
		return -1;
	}

	return 0;
}


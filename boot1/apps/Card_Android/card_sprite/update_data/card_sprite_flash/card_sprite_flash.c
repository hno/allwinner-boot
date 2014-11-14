/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : card_sprite_nand.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include  "../update_data.h"
#include  "string.h"
#include  "mctl_hal.h"

static  int  card_sprite_type = 0;
static  int  sdcard_size;
/*
************************************************************************************************************
*
*                                             update_boot0_info
*
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
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
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
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
		__inf("+++++++++++++++++++++++++++++ good block ratio %d \n", nand_info->good_block_ratio);
	}

	return 0;
}

/*
************************************************************************************************************
*
*                                             sprite_flash_open
*
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
*
*
************************************************************************************************************
*/
__s32 sprite_flash_init(void)
{
	int  line_sel;
	int  speed_mode;
	int  card_offset;
	int  ret;

	if(!card_sprite_type)
	{
		__inf("nand init\n");
		return NAND_Init();
	}
	else if(card_sprite_type == 1)
	{
		__inf("card2 init\n");
		ret = wBoot_script_parser_fetch("card2_boot_para", "card_line", &line_sel, 1);
		if(ret)
		{
			line_sel = 4;
		}
		ret = wBoot_script_parser_fetch("card2_boot_para", "card_high_speed", &speed_mode, 1);
		if(ret)
		{
			speed_mode = 1;
		}
		ret = wBoot_script_parser_fetch("card_boot", "logical_start", &card_offset, 1);
		if(ret)
		{
			card_offset = 40960;
		}
		if(SDMMC_LogicalInit(2, card_offset, line_sel) < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

/*
************************************************************************************************************
*
*                                             sprite_flash_close
*
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
*
*
************************************************************************************************************
*/
__s32 sprite_flash_exit(int type)
{
	if(!card_sprite_type)
	{
		return NAND_Exit();
	}
	else if(card_sprite_type == 1)
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
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
*
*
************************************************************************************************************
*/
__s32 sprite_flash_read(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
{
	int ret;

	//__inf("read start %x, count %x\n", nSectNum, nSectorCnt);
	if(!card_sprite_type)
	{
		ret = NAND_LogicRead(nSectNum, nSectorCnt, pBuf);
		//__inf("read nand end\n");

		return ret;
	}
	else if(card_sprite_type == 1)
	{
		ret = SDMMC_LogicalRead(nSectNum, nSectorCnt, pBuf, 2);
		__inf("read card end\n");

		return (ret>0) ? 0 : (-1);
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
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
*
*
************************************************************************************************************
*/
__s32 sprite_flash_write(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
{
	int ret;

	//__inf("write start %x, count %x\n", nSectNum, nSectorCnt);
	if(!card_sprite_type)
	{
		ret = NAND_LogicWrite(nSectNum, nSectorCnt, pBuf);
		//__inf("write nand end\n");

		return ret;
	}
	else if(card_sprite_type == 1)
	{
		ret = SDMMC_LogicalWrite(nSectNum, nSectorCnt, pBuf, 2);
		//__inf("write card end\n");

		return (ret>0) ? 0 : (-1);
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
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
*
*
************************************************************************************************************
*/
int sprite_flash_hardware_scan(void *flash_info, int erase_flash)
{
	int  ret;
	boot_nand_para_t  nand_para;
	int  line_sel;
	int  speed_mode;
	int  type;
	__u32 good_block_ratio;
	//ɨ??NAND

	type = 0;
	ret = wBoot_script_parser_fetch("target", "storage_type", &type, 1);
	__inf("storage type = %d\n", type);
	if(type == 2)
	{
		goto __hardware_scan__;
	}
	if(NAND_HWScanStart(&nand_para))
	{
		__inf("sprite update error: hwscan start failed\n");
		ret = -1;

		goto __hardware_scan__;
	}

	good_block_ratio = NAND_BadBlockScan((const boot_nand_para_t*)&nand_para);
	if((good_block_ratio == -1)| (good_block_ratio == 0))
	{
		__inf("sprite update error: nand bad block scan failed\n");
		ret = -2;

		goto __hardware_scan__;
	}
	nand_para.good_block_ratio = good_block_ratio;
	NAND_SetValidBlkRatio(nand_para.good_block_ratio);
	__inf("****************************************************************************************** \n");
	__inf("get the good blk ratio from hwscan without erase chip: %d \n", good_block_ratio);
//	if(erase_flash)
//	{
//		if(NAND_EraseChip((const boot_nand_para_t*)&nand_para))
//		{
//			__inf("sprite update error: erase flash failed\n");
//			ret = -2;
//
//			goto __hardware_scan__;
//		}
//	}
	//ɨ??????
	if(NAND_HWScanStop())
	{
		__inf("sprite update error: hwscan stop failed\n");
		ret = -3;

		goto __hardware_scan__;
	}

	memcpy(flash_info, &nand_para, sizeof(boot_nand_para_t));

	return 0;
__hardware_scan__:
	//nand????ʧ?ܣ????Բ?????
	ret = wBoot_script_parser_fetch("card2_boot_para", "card_line", &line_sel, 1);
	if(ret)
	{
		line_sel = 4;
	}
	ret = wBoot_script_parser_fetch("card2_boot_para", "card_high_speed", &speed_mode, 1);
	if(ret)
	{
		speed_mode = 1;
	}
	ret = SDMMC_PhyInit(2, line_sel);
	if(ret == -1)
	{
		__inf("sprite error: try sdcard failed\n");

		return -1;
	}
	card_sprite_type = 1;
	if(erase_flash)
	{
		int   rest, start;
		char  *data;

		data = wBoot_malloc(1024 * 1024);
		memset(data, 0xff, 1024 * 1024);
		rest = ret;
		start = 0;

		while(rest > 2048)
		{
			if(!SDMMC_LogicalWrite1(start, 2048, data, 2))
			{
				__inf("erase sdcard from sector %d, total 2048 failed\n", start);

				return -1;
			}
			start += 2048;
			rest -= 2048;
		}
		if(rest)
		{
			if(!SDMMC_LogicalWrite1(start, rest, data, 2))
			{
				__inf("erase sdcard from sector %d, total %d failed\n", start, rest);

				return -1;
			}
		}
		if(data)
		{
			wBoot_free(data);
		}
	}
	SDMMC_PhyExit(2);

	return 1;

}
/*
************************************************************************************************************
*
*                                             function
*
*    ???????ƣ?
*
*    ?????б���
*
*    ????ֵ  ??
*
*    ˵??    ??
*
*
************************************************************************************************************
*/
int sprite_flash_erase(void *flash_info, int erase_flash)
{
	int ret = 0;
	boot_nand_para_t  nand_para;
	__u32  good_block_ratio;

	if(erase_flash)
	{
		if(NAND_HWScanStart(&nand_para))
		{
			__inf("sprite update error: hwscan start failed\n");
			ret = -1;

			goto __flash_erase__;
		}

		if(NAND_EraseChip((const boot_nand_para_t*)&nand_para))
		{
			__inf("sprite update error: erase flash failed\n");
			ret = -2;

			goto __flash_erase__;
		}
		good_block_ratio = NAND_BadBlockScan((const boot_nand_para_t*)&nand_para);
		if((good_block_ratio == -1)| (good_block_ratio == 0))
		{
				__inf("sprite update error: nand bad block scan failed\n");
				ret = -2;

				goto __flash_erase__;
		}
		nand_para.good_block_ratio = good_block_ratio;
		NAND_SetValidBlkRatio(nand_para.good_block_ratio);
		__inf("****************************************************************************************** \n");
		__inf("get the good blk ratio from hwscan : %d \n", good_block_ratio);
		//扫描结束
		if(NAND_HWScanStop())
		{
			__inf("sprite update error: hwscan stop failed\n");
			ret = -3;

			goto __flash_erase__;
		}

		memcpy(flash_info, &nand_para, sizeof(boot_nand_para_t));
	}

__flash_erase__:

	return ret;
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б��
*
*    ����ֵ  ��
*
*    ˵��    ��
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
//	char       tmp_buffer[] = {0xEB, 0x3C, 0x90, 								//??תָ??
//							   'M', 'S', 'D', 'O', 'S', '5', '.', '0', 			//???̱?־
//							   0x00, 0x02, 										//ÿ?????ֽ???
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
			mbrst->part_info[1].part_type  = 0x05;
			mbrst->part_info[1].start_sectorl  = i;
			mbrst->part_info[1].start_sectorh  = 0;
			mbrst->part_info[1].total_sectorsl = (mbr->array[i].lenlo  & 0x0000ffff) >> 0;
			mbrst->part_info[1].total_sectorsh = (mbr->array[i].lenlo  & 0xffff0000) >> 16;
		}

		mbrst->end_flag = 0xAA55;
		if(!SDMMC_LogicalWrite1(i, 1, mbr_bufst, 2))
		{
			__inf("write standard mbr %d failed\n", i);
			return -1;
		}
	}
	memset(mbr_bufst, 0, 512);
	mbrst = (mbr_stand *)mbr_bufst;

	//memcpy(mbr_bufst, tmp_buffer, 13);
	{
	s32 tmp = SDMMC_LogicalDiskSize(2);
	usize = tmp - 20 * 1024 * 1024/512 - size;
	__inf("SDMMC_LogicalDiskSize return=%d,usize%d,size%d\n", tmp,usize,size);
	}
	mbrst->part_info[0].indicator = 0x80;
	mbrst->part_info[0].part_type = 0x0B;
	mbrst->part_info[0].start_sectorl  = ((mbr->array[mbr->PartCount-1].addrlo + 20 * 1024 * 1024/512 ) & 0x0000ffff) >> 0;
	mbrst->part_info[0].start_sectorh  = ((mbr->array[mbr->PartCount-1].addrlo + 20 * 1024 * 1024/512 ) & 0xffff0000) >> 16;
	mbrst->part_info[0].total_sectorsl = ( usize & 0x0000ffff) >> 0;
	mbrst->part_info[0].total_sectorsh = ( usize & 0xffff0000) >> 16;

	mbrst->part_info[1].part_type = 0x06;
	mbrst->part_info[1].start_sectorl  = ((mbr->array[0].addrlo + 20 * 1024 * 1024/512) & 0x0000ffff) >> 0;
	mbrst->part_info[1].start_sectorh  = ((mbr->array[0].addrlo + 20 * 1024 * 1024/512) & 0xffff0000) >> 16;
	mbrst->part_info[1].total_sectorsl = (mbr->array[0].lenlo  & 0x0000ffff) >> 0;
	mbrst->part_info[1].total_sectorsh = (mbr->array[0].lenlo  & 0xffff0000) >> 16;

	mbrst->part_info[2].part_type = 0x05;
	mbrst->part_info[2].start_sectorl  = 1;
	mbrst->part_info[2].start_sectorh  = 0;
	mbrst->part_info[2].total_sectorsl = (size & 0x0000ffff) >> 0;
	mbrst->part_info[2].total_sectorsh = (size & 0xffff0000) >> 16;

	mbrst->end_flag = 0xAA55;
	__inf("SDMMC_LogicalWrite1 mbr_bufst start\n");
	if(!SDMMC_LogicalWrite1(0, 1, mbr_bufst, 2))
	{
		__inf("write standard mbr 0 failed\n");
		return -1;
	}
	__inf("SDMMC_LogicalWrite1 mbr_bufst ok\n");
	return 0;
}


int sprite_detect_flash_ch(void)
{
#if 0
	__u32 dramBondingId;
	__u32 channelCnt;
    __dram_para_t      *dram_p;
	boot1_file_head_t  *boot1_head;


	boot1_head = (boot1_file_head_t *)BOOT1_BASE;
	dram_p     = (__dram_para_t *)boot1_head->prvt_head.script_buf;

	dramBondingId = (dram_p->dram_tpr13>>3) & 0x3;
	channelCnt = NAND_GetChannelCnt();

	__inf("dram ch=%d\n", dramBondingId);
	__inf("nand ch=%d\n", channelCnt);

	if (dramBondingId == 0)
    {
        if (channelCnt != 2)
        {
            __inf("dram ch %d is not match nand ch %d\n", dramBondingId, channelCnt);

            return -1;
        }
    }
#endif
    return 0;
}


/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON start sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : basic_block_device.c
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
#include "include.h"


#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
#include "sdmmc_for_boot.h"

static __u32 sdcard_num;

__s32 eGon2_block_device_init(void)
{
    __s32 gpio_no, ret;
    int   *storage = (int *)&BT1_head.prvt_head.storage_type;

	boot_sdcard_info_t  *sdcard_info = (boot_sdcard_info_t *)BT1_head.prvt_head.storage_data;

	sdcard_num = BT1_head.boot_head.eGON_vsn[0];
	gpio_no    = BT1_head.boot_head.eGON_vsn[1];
	*storage   = ((gpio_no << 16) | sdcard_num);

    //return  SDMMC_LogicalInit(*storage, sdcard_info->boot_offset, sdcard_info->speed_mode[gpio_no], sdcard_info->line_count[gpio_no]);
	ret = SDMMC_LogicalInit(sdcard_num, 20 * 1024 * 1024/512, 4);

	if(ret < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

__s32 eGon2_block_device_exit(void)
{
    return SDMMC_LogicalExit(sdcard_num);
}

__s32 eGon2_block_device_read(__u32 start_block, __u32 nblock, void *pbuffer)
{
	__u32 blk = SDMMC_LogicalRead(start_block, nblock, pbuffer, sdcard_num);

	if(blk == nblock)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

__s32 eGon2_block_device_write(__u32 start_block, __u32 nblock, void *pbuffer)
{
    __u32 blk =  SDMMC_LogicalWrite(start_block, nblock, pbuffer, sdcard_num);
	if(blk == nblock)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

__s32 eGon2_block_size(void)
{
	return SDMMC_LogicalDiskSize();
}

__s32 eGon2_block_phyread(__u32 start_block, __u32 nblock, void *pbuffer)
{
    __u32 blk =  SDMMC_PhyRead(start_block, nblock, pbuffer, sdcard_num);
	if(blk == nblock)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

__s32 eGon2_block_phywrite(__u32 start_block, __u32 nblock, void *pbuffer)
{
    __u32 blk =  SDMMC_PhyWrite(start_block, nblock, pbuffer, sdcard_num);
	if(blk == nblock)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

#elif SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND

#include "nand_for_boot.h"
__s32 eGon2_block_device_init(void)
{
	int   *storage = (int *)&BT1_head.prvt_head.storage_type;

	*storage = 0;

	return NAND_Init();
}

__s32 eGon2_block_device_exit(void)
{
    return NAND_Exit();
}


__s32 eGon2_block_device_read(__u32 start_block, __u32 nblock, void *pbuffer)
{
    return NAND_LogicRead(start_block, nblock, pbuffer);
}

__s32 eGon2_block_device_write(__u32 start_block, __u32 nblock, void *pbuffer)
{
    return NAND_LogicWrite(start_block, nblock, pbuffer);
}

__s32 eGon2_block_phyread(__u32 start_block, __u32 nblock, void *pbuffer)
{
    return NAND_LogicRead(start_block, nblock, pbuffer);
}

__s32 eGon2_block_phywrite(__u32 start_block, __u32 nblock, void *pbuffer)
{
    return NAND_LogicWrite(start_block, nblock, pbuffer);
}

__s32 eGon2_block_size(void)
{
	//return NAND_GetDiskSize();
	return 0;
}

void eGon2_block_ratio(void)
{
	boot_nand_para_t *nand_info = (boot_nand_para_t *)BT1_head.prvt_head.storage_data;
	int  good_block_ratio;

	good_block_ratio = nand_info->good_block_ratio;
	if(!eGon2_script_parser_patch("nand0_para", "good_block_ratio", good_block_ratio))
	{
		eGon2_printf("nand ratio=%d\n", good_block_ratio);
	}
}

#endif  /* end of SYS_STORAGE_MEDIA_TYPE */


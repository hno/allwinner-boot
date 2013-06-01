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
#include "include.h"


#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
#include "sdmmc_for_boot.h"

static __u32 sdcard_num;

__s32 eGon2_block_device_init(void)
{
    __s32 gpio_no,ret;
    int   *storage = (int *)&BT1_head.prvt_head.storage_type;

	boot_sdcard_info_t  *sdcard_info = (boot_sdcard_info_t *)BT1_head.prvt_head.storage_data;

	sdcard_num = BT1_head.boot_head.eGON_vsn[0];
	gpio_no    = BT1_head.boot_head.eGON_vsn[1];
	*storage   = ((gpio_no << 16) | sdcard_num);
    __debug("boot sdcard number=%d\n",sdcard_num);
    __debug("boot sdcard speed_mode=%d\n",sdcard_info->speed_mode[sdcard_num]);
    __debug("boot sdcard line_sel=%d\n",sdcard_info->line_sel[sdcard_num]);

//    return  SDMMC_LogicalInit(*storage, sdcard_info->boot_offset, sdcard_info->speed_mode[gpio_no], sdcard_info->line_count[gpio_no]);
    //return SDMMC_LogicalInit(sdcard_num, 20 * 1024 * 1024/512,sdcard_info->speed_mode[sdcard_num],sdcard_info->line_sel[sdcard_num]);
    return SDMMC_LogicalInit(sdcard_num, 20 * 1024 * 1024/512, 4);
    
}

__s32 eGon2_block_device_exit(void)
{
    return SDMMC_LogicalExit(sdcard_num);
}

__s32 eGon2_block_device_read(__u32 start_block, __u32 nblock, void *pbuffer)
{

    return SDMMC_LogicalRead(start_block, nblock, pbuffer, sdcard_num);

}

__s32 eGon2_block_device_write(__u32 start_block, __u32 nblock, void *pbuffer)
{

    SDMMC_LogicalWrite(start_block, nblock, pbuffer, sdcard_num);

}

__s32 eGon2_block_size(void)
{
	return SDMMC_LogicalDiskSize();
}

__s32 eGon2_block_phyread(__u32 start_block, __u32 nblock, void *pbuffer)
{

    return SDMMC_PhyRead(start_block, nblock, pbuffer, sdcard_num);

}

__s32 eGon2_block_phywrite(__u32 start_block, __u32 nblock, void *pbuffer)
{
    return SDMMC_PhyWrite(start_block, nblock, pbuffer, sdcard_num);

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
	if(!eGon2_script_parser_patch("nand_para", "good_block_ratio", good_block_ratio))
	{
		eGon2_printf("nand good_block_ratio=%d\n", good_block_ratio);
	}
}

#endif  /* end of SYS_STORAGE_MEDIA_TYPE */


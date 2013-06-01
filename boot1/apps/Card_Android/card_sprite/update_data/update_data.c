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
#include  "update_data.h"
#include  "card_sprite_flash/card_sprite_flash.h"


#define   ANDROID_FORMAT_UNKONW    (0)
#define   ANDROID_FORMAT_BAD       (-1)
#define   ANDROID_FORMAT_DETECT    (1)


extern  __u32    nand_hd;
extern  int		 private_type;
extern  int      dynamic_data;
extern  char     mac_addr_store[32];
extern  int      env_exist;

static  int      flash_start_sector = -1;

int		android_format;
char    *flash_memory_base = (char *)0x46000000;
char    *env_memory_base   = (char *)0x44000000;
char    *flash_memory;
int		 flash_memory_size;
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
__s32  update_boot0(void *buf0, char *buf, int sprite_type)
{
    boot0_file_head_t    *boot0  = (boot0_file_head_t *)buf0;
    boot1_file_head_t    *boot1  = (boot1_file_head_t *)BOOT1_BASE;
	__u32                 length;

	//校验数据是否正确
    if(check_file( (__u32 *)buf0, boot0->boot_head.length, BOOT0_MAGIC ) != CHECK_IS_CORRECT)
    {
        sprite_wrn("The input Boot0 in the RAM %X is checked wrong!\n", buf0);

		return -1;
	}
	//读出dram参数
	//填充FLASH信息
	if(!sprite_type)
	{
		update_boot0_info(buf0, buf);
	}
	//memcpy(&boot0->prvt_head.nand_connect_info, &nand_info->nand_para, sizeof(boot_nand_para_t));
	memcpy((void *)&boot0->prvt_head.dram_para, (void *)&boot1->prvt_head.dram_para, sizeof(boot_dram_para_t));
	boot0->boot_head.platform[7] = 1;//0: try dram para.1: read dram para from head.
	/* regenerate check sum */
	gen_check_sum( (void *)boot0 );

    length = boot0->boot_head.length;                         // 获取Boot0文件的尺寸
    /* 校验内存中的Boot0文件 */
    if( check_file( (__u32 *)boot0, length, BOOT0_MAGIC ) != CHECK_IS_CORRECT )
    {
        sprite_wrn("The Boot0 in the RAM %X is checked wrong!\n", boot0);
		return -1;
	}

	if(!sprite_type)
	{
		return Nand_Burn_Boot0((__u32)buf0, length);
	}
	else
	{
		return SDMMC_PhyWrite(BOOT0_SDMMC_START_ADDR, length/512, (void *)boot0, 2);
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
static int dyanmic_data_check(char *buf)
{
	char *base = buf;
	dynamic_boot_head  *dhead;

	dhead = (dynamic_boot_head *)base;
	if(strncmp(dhead->magic, "dynamic", sizeof("dynamic")))
	{
		__inf("dynamic data magic error\n");

		return -1;
	}

	return 0;
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
__s32 update_boot1(void *buf1, char *buf, int sprite_type)
{
    boot1_file_head_t  *boot1 = (boot1_file_head_t *)buf1;
    __u32   length;
//    __s32   ret = 0;
	if(check_file( (__u32 *)buf1, boot1->boot_head.length, BOOT1_MAGIC ) != CHECK_IS_CORRECT)
    {
        sprite_wrn("Error!The input Boot1 in the RAM %X is checked wrong!\n", buf1);
		return -1;
	}
	//填充NAND信息
	boot1->prvt_head.work_mode = 0x20;
	if(!sprite_type)
	{
		update_boot1_info(buf1, buf);
	}
	if(env_exist)
	{
		char 			   *base = (char *)boot1->prvt_head.script_buf;
		dynamic_data_form  *dform;
		dynamic_boot_head  *dhead;

		dhead = (dynamic_boot_head *)base;

		memset(base, 0, 32 * 1024);
		strcpy(dhead->magic, "dynamic");
		dhead->count = 1;

		base += sizeof(dynamic_boot_head);
		dform = (dynamic_data_form *)base;

		strcpy(dform->name, "mac_addr");
		__inf("%s\n", mac_addr_store);
		dform->datasize = strlen(mac_addr_store);
		__inf("size = %d\n", dform->datasize);
		dform->usedsize = ((dform->datasize + 4)>>2)<<2;
		base += sizeof(dynamic_data_form);
		memcpy(base, mac_addr_store, dform->datasize);
	}
//	else	//读出原有的boot1数据，查看是否有动态地址
//	{
//		char *tmp_boot1 = NULL;
//
//		tmp_boot1 = wBoot_malloc(512 * 1024);
//		if(!tmp_boot1)
//		{
//			sprite_wrn("not enough memory to read boot1!\n");
//		}
//		else
//		{
//			if(Nand_Load_Boot1(tmp_boot1, 512 * 1024))
//			{
//				sprite_wrn("the boot1 in flash is bad!\n");
//			}
//			else
//			{
//				boot1_file_head_t  *tmp_boot1_head = (boot1_file_head_t *)tmp_boot1;
//
//				if(!dyanmic_data_check(tmp_boot1_head->prvt_head.script_buf))
//				{
//					memcpy(boot1->prvt_head.script_buf, tmp_boot1_head->prvt_head.script_buf, 32 * 1024);
//					__inf("dynamic data stored\n");
//				}
//			}
//			wBoot_free(tmp_boot1);
//		}
//	}
//	if(ret < 0)
//	{
//		return -1;
//	}
	/* regenerate check sum */
    boot1->boot_head.platform[7]=0xff;                          //boot mode
	gen_check_sum( (void *)boot1 );
	length = boot1->boot_head.length;                         // 获取Boot1文件的尺寸
    /* 校验内存中的Boot1文件 */
    if( check_file( (__u32 *)boot1, length, BOOT1_MAGIC ) != CHECK_IS_CORRECT )
    {
        sprite_wrn("The changed Boot1 in the RAM %X is checked wrong!\n", boot1);

		return -1;
	}
	__inf("burn boot1\n");
	if(!sprite_type)
	{
		return Nand_Burn_Boot1((__u32)boot1, length);
	}
	else if(sprite_type == 1)
	{
		return SDMMC_PhyWrite(BOOT1_SDMMC_START_ADDR, length/512, (void *)boot1, 2);
	}

	return -1;
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
__s32 update_flash_init(int *type)
{
	return sprite_flash_init(type);
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
__s32 update_flash_exit(int type)
{
	return sprite_flash_exit(type);
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
__s32 update_flash_open(__u32 sector_lo, __u32 sector_hi)
{
	android_format      = ANDROID_FORMAT_UNKONW;
	flash_start_sector  = sector_lo;

	if(private_type == 1)
	{
		env_init(flash_start_sector);
	}

    return 0;
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
__s32 update_flash_write(void *pbuf, __u32 len)
{
	__s32	  ret;

	if(android_format == ANDROID_FORMAT_UNKONW)
	{
		ret = unsparse_probe((char *)pbuf, len, flash_start_sector);
		if(ret == -1)
		{
			android_format = ANDROID_FORMAT_BAD;
		}
		else
		{
			android_format = ANDROID_FORMAT_DETECT;
		}
	}
	if(ANDROID_FORMAT_BAD == android_format)
	{
		if(private_type == 1)
		{
			env_dram_write(pbuf, len);
		}
		else
		{
			update_flash_write_ext(pbuf, len);
		}
	}
	else
	{
		//unsparse_dram_write(pbuf, len);
		unsparse_direct_write(pbuf, len);
	}

    return 0;
}

__s32 update_flash_write_ext(void *pbuf, __u32 len)
{
	__u32   sector_count = len>>9;

	if(flash_start_sector < 0)
	{
		return -1;
	}
    if(len & 0x1ff)   //如果不是512字节(扇区)对齐，则增加1扇区
    {
        sector_count ++;
    }
	if(pbuf)
	{
		if(sprite_flash_write(flash_start_sector, sector_count, pbuf))
	    {
	        return -1;
	    }
	}
    flash_start_sector += sector_count;

	return 0;
}

__s32 update_flash_read_ext(void *pbuf, __u32 len)
{
	__u32   sector_count = len>>9;

	if(flash_start_sector < 0)
	{
		return -1;
	}
    if(len & 0x1ff)   //如果不是512字节(扇区)对齐，则增加1扇区
    {
        sector_count ++;
    }
	if(pbuf)
	{
		if(sprite_flash_read(flash_start_sector, sector_count, pbuf))
	    {
	        return -1;
	    }
	}
    flash_start_sector += sector_count;

	return 0;
}

__s32 update_force_to_flash(void)
{
	if(android_format == ANDROID_FORMAT_DETECT)
	{
		return unsparse_deal();
	}
	else if(private_type == 1)
	{
		//set mac to new env
		env_exit();
	}

	return 0;
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
__s32 update_flash_close(void)
{
	flash_start_sector = -1;
	android_format = ANDROID_FORMAT_UNKONW;

	return 0;
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
int update_flash_hardware_scan(void* mbr_i,void *flash_info, int erase_flash)
{
	return sprite_flash_hardware_scan(mbr_i,flash_info, erase_flash);
}


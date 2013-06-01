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


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
#include "card_sprite_i.h"
#include "FormatTool.h"
//------------------------------------------------------------------------------------------------------------
//
// 函数说明
//
//
// 参数说明
//
//
// 返回值
//
//
// 其他
//    无
//
//------------------------------------------------------------------------------------------------------------
//RsvdSecCnt = 0x100; 0x20~0x8000
int FormatDisk(void)
{
	u32 ret = OK;
	u8 buffer[512];
	FormatParameter param;

	__inf("FormatDisk......................\n");
	memset(&param, 0, sizeof(FormatParameter));

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	ret = FATFS_Format((struct FormatParameter *)&param);
	if (OK != ret)
	{
		__inf("FATFS_Format failed %d\n", ret);
		return ret;
	}
	//--------------------------------------------------------------------------
	// clean magic block by scott 2009-11-21
	//--------------------------------------------------------------------------
	memset(buffer, 0, 512);
	//--------------------------------------------------------------------------
	// jerry 2010-5-11 清除boot0的代码
	//--------------------------------------------------------------------------
	if(wBoot_block_dwrite(16, 1, buffer))
//	if(wBoot_block_write(16, 1, buffer))
	{
		__inf("Clean magic block failed ! %d\n", ret);
		return -1;
	}

	return ret;
}



//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------


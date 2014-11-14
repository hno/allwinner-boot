/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#include  "update_data.h"

static int card_sprite_type = 0;
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
__s32 update_flash_init(void)
{
	int  line_sel;
	int  speed_mode;
	int  card_offset;
	int  ret;

	__inf("try nand\n");
	if(!NAND_Init())
	{
		card_sprite_type = 0;
		return 0;
	}
	else
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
			card_sprite_type = 1;
			return 0;
		}
	}
	card_sprite_type = -1;

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
__s32 update_flash_exit(void)
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
__s32 update_flash_read(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
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
		//__inf("read card end\n");

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
__s32 update_flash_write(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
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


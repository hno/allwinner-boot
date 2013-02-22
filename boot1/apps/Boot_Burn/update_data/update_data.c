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


static  int  card_sprite_type = 0;
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
		int  card_offset;
		int  ret;
	  
	  if(0==NAND_Init())
    {

        card_sprite_type=0;
        return 0;
    }
    else
    {
      card_sprite_type=1;
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
__s32 update_flash_write(__u32 nSectNum, __u32 nSectorCnt, void * pBuf)
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


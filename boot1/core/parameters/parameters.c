/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON parameters sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : parameters.c
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
#include "nand_for_boot.h"


/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：eGon2_dispatch_parameters
*
*    参数列表：para_name           需要获取的参数的名称(定义)，所有定义都是用 WBOOT_PARA作为前缀
*
*              para_addr           存放参数的值的地址
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 eGon2_dispatch_parameters(__u32 para_name, void *para_addr)
{
    __s32 ret = 0;

    switch(para_name)
    {
        case WBOOT_PARA_NANDFLASH_INFO:
        {
            boot_nand_para_t *p         = (boot_nand_para_t *)para_addr;
			boot_nand_para_t *nand_info = (boot_nand_para_t *)BT1_head.prvt_head.storage_data;

            memset( p, 0, sizeof(boot_nand_para_t) );
	        p->good_block_ratio = nand_info->good_block_ratio;
        }
        break;

        case WBOOT_PARA_GLOBAL:
        {
            boot1_private_head_t  *boot1_info = (boot1_private_head_t *)para_addr;

            memcpy(boot1_info, &BT1_head.prvt_head, sizeof(boot1_private_head_t));
        }
        break;

        default:
            ret = -1;
            break;
    }

    return ret;
}


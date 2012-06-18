/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : script.c
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
__s32 eGon2_dynamic(void)
{
	char *base = (char *)BT1_head.prvt_head.script_buf;
	int   i;
	dynamic_data_form  *dform;
	dynamic_boot_head  *dhead;

	dhead = (dynamic_boot_head *)base;
	if(strncmp(dhead->magic, "dynamic", sizeof("dynamic")))
	{
		eGon2_printf("dynamic data magic error\n");

		return -1;
	}
	base += sizeof(dynamic_boot_head);
	for(i=0;i<dhead->count;i++)
	{
		dform = (dynamic_data_form *)base;
		eGon2_script_parser_patch_str("dynamic", dform->name, base + sizeof(dynamic_data_form), dform->usedsize>>2);
		base += dform->usedsize;
	}

	return 0;
}



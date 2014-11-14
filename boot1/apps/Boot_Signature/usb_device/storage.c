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
#include "usb_descriptor.h"
#include "usb_params.h"
#include "usb_register.h"
#include "usb_storage.h"

#include "string.h"
/*
************************************************************************************************************
*
*                                             dram_copy
*
*    函数名称：
*
*    参数列表：	src_addr  源地址
*
*				dest_addr 目的地址
*
*				bytes	  传递的字节数
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 dram_copy(__u32 src_addr, __u32 dest_addr, __u32 bytes)
{
	memcpy((void *)src_addr, (void *)dest_addr, bytes);

	return 1;
}



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
#ifndef  __BOOT_HARDWARE_RES_H__
#define  __BOOT_HARDWARE_RES_H__

#include  "types.h"

#define   OS_FOR_USB_UPDATE_FILE_NAME        "c:\\rabbit.bot"

typedef struct
{
    unsigned    layer_hd;                     //用于显示的图层句柄
    unsigned    disp_hd;                      //显示驱动句柄
    unsigned    power_hd;                     //power驱动句柄
    int       display_source;
    char      fel_file_name[32];            //升级文件名称
}
boot_hardware_res;

extern  boot_hardware_res     board_res;

#endif   //__BOOT_HARDWARE_RES_H__


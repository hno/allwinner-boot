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

#ifndef  __BOARD_H__
#define  __BOARD_H__

#include  "fel_detect/fel_detect.h"
#include  "power_ctrl/power_ops_int.h"

extern  int BoardInit(void);
extern  int BoardExit(int logo_status, int boot_status);

extern  int BoardInit_Display(int source, int mode);



#endif   //__BOARD_H__


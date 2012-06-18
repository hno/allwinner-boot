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

#define    CARD_SPRITE_SUCCESSED               1
#define    CARD_SPRITE_FAIL					  -1
#define    CARD_SPRITE_NORMAL				   0

typedef struct
{
    unsigned    layer_hd;                     //用于显示的图层句柄
    unsigned    disp_hd;                      //显示驱动句柄
    unsigned    led_hd[32];                   //用于LED的显示，最多32个
    unsigned    tmr_hd;                       //TIMER句柄
    int			led_count;					  //实际的LED的个数
    int         led_status[32];               //每个LED对应的状态
    int         display_source;
}
boot_hardware_res;

extern  boot_hardware_res     board_res;

#endif   //__BOOT_HARDWARE_RES_H__


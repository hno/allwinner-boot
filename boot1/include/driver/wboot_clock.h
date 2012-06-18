/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON clock sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : wboot_clock.h
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
#ifndef	_WBOOT_CLOCK_H_
#define	_WBOOT_CLOCK_H_

typedef enum  wboot_clock_name_set
{
    WBOOT_CLOCK_CORE_PLL,                           //corepll Ãû³Æ
    WBOOT_CLOCK_AXI_CLOCK,							//coreclock
    WBOOT_CLOCK_AHB_CLOCK,							//ahb
    WBOOT_CLOCK_APB0_CLOCK,							//apb0
    WBOOT_CLOCK_APB1_CLOCK,							//apb1
    WBOOT_CLOCK_
}
wboot_clock_name_set_t;




#endif	//_WBOOT_CLOCK_H_

/* end of _WBOOT_CLOCK_H_ */


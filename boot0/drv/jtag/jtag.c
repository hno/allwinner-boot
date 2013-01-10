/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : jtag.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.13
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.09.13       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __jtag_c
#define  __jtag_c


#include "..\\boot0_drv_i.h"


//*****************************************************************************
//	void jtag_init(uint32 port_id)
//  Description:	Select different JTAG port and enable JTAG
//
//	Arguments:		uint32 port_id   1: GPIOB port, 0: GPIOA port
//
//	Return Value:	None
//*****************************************************************************
void jtag_init(normal_gpio_cfg *jtag_gpio)
{
	boot_set_gpio(jtag_gpio, 6, 1);
}


/*------------------------------------------platform end------------------------------------------*/



#endif     //  ifndef __jtag_c

/* end of jtag.c */

/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : cp15.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.14
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
* Gary.Wang      2009.09.14       1.1.0        build the file
*
************************************************************************************************************************
*/
#include "cpu_private.h"


void  set_vect_low_addr( void )
{
	__u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	//__asm{MRC p15,0,c1format,c1,c0,0}
	c1format &= ~ 0x00002000;
	c1format |= 0;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	//__asm{MCR p15,0,c1format,c1,c0,0}
}


void  set_vect_high_addr( void )
{
	__u32 c1format = 0;

	asm("mrc p15,0,%0,c1,c0,0":"=r"(c1format));
	//__asm{MRC p15,0,c1format,c1,c0,0}
	c1format &= ~ 0x00002000;
	c1format |= 0x00002000;
	asm("mcr p15,0,%0,c1,c0,0"::"r"(c1format));
	//__asm{MCR p15,0,c1format,c1,c0,0}
}


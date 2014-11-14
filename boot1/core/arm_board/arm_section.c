/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : intc.c
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
#include "include.h"
/*
**********************************************************************************************************************
*                                               move_RW
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void move_RW( void )
{
	;
}
/*
**********************************************************************************************************************
*                                               clear_ZI
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
extern int _bss_start, _bss_end;
extern int _standby_start, _standby_end;
extern int _arm_start, _arm_end;
extern int _standby_start_lma;

void clear_ZI( void )
{
	//int *dst, *end;

	memset((void *)&_bss_start, 0, (&_bss_end - &_bss_start) * 4);
	//for (dst = &_bss_start; dst< &_bss_end; dst++)
	//{
	//	*dst = 0;
	//}
}
/*
**********************************************************************************************************************
*                                               reposition_int_vect
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void  reposition_arm_start( void )
{
//	int *dst = 0;
//	int *src = &_arm_start;
//
//	while (src < &_arm_end)
//	{
//		*dst++ = *src++;
//	}
	memcpy((void *)0, (void *)&_arm_start, (&_arm_end - &_arm_start) * 4);
}
/*
**********************************************************************************************************************
*                                               reposition_boot_standby
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void  reposition_boot_standby( void )
{
#if 1
	memcpy((void *)&_standby_start, (void *)&_standby_start_lma, ((__u32)&_standby_end - (__u32)&_standby_start));
#else
	int *dst = &_standby_start;
	int *src = &_standby_start_lma;

	while (dst < &_standby_end)
	{
		*dst++ = *src++;
	}
#endif
}



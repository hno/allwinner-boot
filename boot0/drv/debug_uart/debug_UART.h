/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
*File Name£º debug_UART.h
*
*Author£º Gary.Wang
*
*Version 1.1.0
*
*Date  2008.04.29
*
*Description£º
*
*Others : None at present.
*
*
* History :
*
*  <Author>        <time>      <version>     <description>
*
* Wangyugang      2008.04.29     1.1.0       build the file
*
*******************************************************************************************************************
*/

#ifndef __debug_UART_h
#define __debug_UART_h

#include "uart_hd.h"

extern void   UART_puts( const char * string );
//extern char   UART_getchar_with_display( __u32 timeout );
extern void   UART_printf2( const char * str, ... );
extern void   eFG_printf( const char * str, ... );


#endif     //  ifndef __debug_UART_h

/* end of debug_UART.h  */

/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
*File Name£º uart.h
*
*Author£º Gary.Wang
*
*Version 1.1.0
*
*Date  2009.09.13
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
* Wangyugang      2009.09.13     1.1.0       build the file
*
*******************************************************************************************************************
*/

#ifndef __UART_h
#define __UART_h


extern void   UART_open( __s32 uart_port, void  *uart_ctrl, __u32 apb_freq );
extern void   UART_close( void );
extern void   UART_putchar( char c );
//extern char   UART_getchar( __u32 timeout );


#endif     //  ifndef __UART_h

/* end of UART.h  */

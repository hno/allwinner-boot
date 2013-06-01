/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : misc.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.04.03
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
* Gary.Wang      2009.04.03       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __misc_h
#define  __misc_h


extern __u32 g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p );
extern __s32 str2int( const char *str, const char **err );



#endif     //  ifndef __misc_h

/* end of misc.h */

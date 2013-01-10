/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : boot0_i.h
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
#ifndef  __boot0_misc_h
#define  __boot0_misc_h


#define BOOT0_PRVT_HEAD_VERSION         "1230"    // X.X.XX
#define BOOT0_FILE_HEAD_VERSION         "1230"    // X.X.XX

#define BOOT0_VERSION                   "1230"    // X.X.XX



#define PAGE_BUF_FOR_BOOT0              ( EGON2_DRAM_BASE + SZ_1M )

#ifndef NDEBUG
	#define msg(...)                    UART_printf2(__VA_ARGS__)
#else
	#define msg(...)
#endif

#define HERE                            msg("file:%s, line:%u.\n", __FILE__, __LINE__);


#define OK                    0
#define ERROR                 1



#endif     //  ifndef __boot0_misc_h

/* end of boot0_misc.h */

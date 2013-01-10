/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : load_boot1_from_spinor.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.12.08
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
* Gary.Wang      2009.12.08       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __load_boot1_from_sdmmc_h
#define  __load_boot1_from_sdmmc_h


/*******************************************************************************
*函数名称: load_boot1_from_spinor
*函数原型：int32 load_boot1_from_spinor( void )
*函数功能: 将一份好的Boot1从spi nor flash中载入到SRAM中。
*入口参数: void
*返 回 值: OK                         载入并校验成功
*          ERROR                      载入并校验失败
*备    注:
*******************************************************************************/
extern __s32 load_boot1_from_sdmmc( char *buf);



#endif     //  ifndef __load_boot1_from_spi_nor_h

/* end of load_boot1_from_spinor.h */

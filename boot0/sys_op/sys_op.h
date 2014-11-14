/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : sys_op_i.h
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
#ifndef  __sys_op_h
#define  __sys_op_h


extern   void   mmu_enable( void );
extern   void   mmu_disable( void );
extern   void   mmu_system_init(__u32 dram_base, __u32 dram_size, __u32 mmu_base);



#endif     //  ifndef __sys_op_h

/* end of __sys_op_h.h */

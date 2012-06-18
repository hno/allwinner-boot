/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : intc.h
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
#ifndef  __arm_section_h
#define  __arm_section_h


extern  void  move_RW( void );
extern  void  clear_ZI( void );
extern  void  reposition_arm_start( void );
extern  void  reposition_boot_standby( void );


#endif     //  ifndef __arm_section_h

/* end of __arm_section.h */


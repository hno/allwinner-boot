/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#ifndef  __BOOTOS_H__
#define  __BOOTOS_H__

extern  void BootOS(unsigned int para_addr, unsigned int kernal_addr);
extern  int  BootOS_detect_os_type(unsigned *para_addr, unsigned *kernal_addr, void *os_info, int *logo_status);

extern  int  do_boot_linux (char *para_addr, char *cmdline, unsigned cmdline_length);

#endif   //__BOOTOS_H__


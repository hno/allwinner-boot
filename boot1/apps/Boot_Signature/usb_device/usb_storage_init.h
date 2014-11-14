/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : usb_storage_init.c
*
* By      :
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#ifndef  __USB_STORAGE_INIT_H__
#define  __USB_STORAGE_INIT_H__




extern __u32 usb_start(int status);
extern __u32 usb_run(void);


extern __u32 usb_detect_enter(void);
extern __u32 usb_detect_exit (void);


#endif   //__USB_STORAGE_INIT_H__




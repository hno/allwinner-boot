/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : usb_descriptor.h
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
#ifndef  __USB_DESCRIPTOR_H__
#define  __USB_DESCRIPTOR_H__

#include "usb_descriptor.h"


extern const unsigned char USB_HS_BULK_DevDesc[18];

extern const unsigned char USB_HS_BULK_ConfigDesc[32];

extern const unsigned char USB_FS_BULK_DevDesc[18];

extern const unsigned char USB_FS_BULK_ConfigDesc[32];

extern const unsigned char RequestSense[18];

extern const unsigned char  LangID[4];
extern const unsigned char  iSerialNum0[30];
extern const unsigned char  iSerialNum1[30];
extern const unsigned char  iSerialNum2[30];
extern const unsigned char  iManufacturer[24];
extern const unsigned char  iProduct[22];
extern const unsigned char  iProduct_new[90];


extern const unsigned char TestPkt[54];


extern const unsigned char* USB_StrDec0[4];

extern const unsigned char USB_DevQual[10];

extern const unsigned char USB_OTGDesc[3];

///////////////////////////////////////////////////////////
//For MassStorage Only
///////////////////////////////////////////////////////////
extern const unsigned char  InquiryData[];

extern const unsigned char SenseData[];


#endif  //usb_descriptor.h


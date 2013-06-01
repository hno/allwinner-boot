/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
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


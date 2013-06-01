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
#include "usb_params.h"


const unsigned char USB_HS_BULK_DevDesc[18]    = {0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
	                                        0x1A, 0x1F,   //VID -- Vendor ID
	                                        0x00, 0x07,   //PID -- Product ID
	                                        0x00, 0x01, 0x02, 0x03, 0x01, 0x01};

const unsigned char USB_HS_BULK_ConfigDesc[32] = {//Configuration Descriptor
	                                        0x09, 0x02, 0x20, 0x00, 0x01,
	                                        0x08,  //Value for SetConfig Request
	                                        0x00, 0x80,
	                                        50,  //Max Power = 2*n mA
	                                        //Interface Descriptor
	                                        0x09, 0x04, 0x00, 0x00, 0x02,
	                                        0x08,  //Interface Class    -- 0x08, MassStorage
	                                        0x06,  //Interface SubClass -- 0x06, SPC-2
	                                        0x50,  //Interface Protocol -- Bulk Only
	                                        0x00,
	                                        //Bulk-only IN Endpoint Descriptor
	                                        0x07, 0x05, (0x80|BULK_IN_EP), 0x02,
	                                        0x00, 0x02,
	                                        0x00,
	                                        //Bulk-only OUT Endpoint Descriptor
	                                        0x07, 0x05, BULK_OUT_EP, 0x02,
	                                        0x00, 0x02,
	                                        0x00
	                                        };

const unsigned char USB_FS_BULK_DevDesc[18]    = {0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08,
	                                        0x1A, 0x1F,   //VID -- Vendor ID
	                                        0x00, 0x07,   //PID -- Product ID
	                                        0x00, 0x01, 0x02, 0x03, 0x01, 0x01};

const unsigned char USB_FS_BULK_ConfigDesc[32] = {//Configuration Descriptor
	                                        0x09, 0x02, 0x20, 0x00, 0x01,
	                                        0x08,  //Value for SetConfig Request
	                                        0x00, 0x80,
	                                        50,  //Max Power = 2*n mA
	                                        //Interface Descriptor
	                                        0x09, 0x04, 0x00, 0x00, 0x02,
	                                        0x08,  //Interface Class    -- 0x08, MassStorage
	                                        0x06,  //Interface SubClass -- 0x06, SPC-2
	                                        0x50,  //Interface Protocol -- Bulk Only
	                                        0x00,
	                                        //Bulk-only IN Endpoint Descriptor
	                                        0x07, 0x05, (0x80|BULK_IN_EP), 0x02,
	                                        0x40, 0x00,
	                                        0x00,
	                                        //Bulk-only OUT Endpoint Descriptor
	                                        0x07, 0x05, BULK_OUT_EP, 0x02,
	                                        0x40, 0x00,
	                                        0x00
	                                        };



const unsigned char  LangID[4]        = {0x04, 0x03, 0x09, 0x04};
const unsigned char  iSerialNum0[30]   = {0x1E, 0x03, '2' , 0x00, '0' , 0x00, '1' , 0x00, '0' , 0x00,
	                                       '1' , 0x00, '2' , 0x00, '0' , 0x00, '1' , 0x00, '1' , 0x00,
	                                       '2' , 0x00, '0' , 0x00, '0' , 0x00, '0' , 0x00, '1' , 0x00
	                                       }; //"20101201120001"
const unsigned char  iSerialNum1[30]   = {0x1E, 0x03, '2' , 0x00, '0' , 0x00, '1' , 0x00, '0' , 0x00,
	                                       '1' , 0x00, '2' , 0x00, '0' , 0x00, '1' , 0x00, '1' , 0x00,
	                                       '2' , 0x00, '0' , 0x00, '0' , 0x00, '0' , 0x00, '2' , 0x00
	                                       }; //"20101201120002"
const unsigned char  iSerialNum2[30]   = {0x1E, 0x03, '2' , 0x00, '0' , 0x00, '1' , 0x00, '0' , 0x00,
	                                       '1' , 0x00, '2' , 0x00, '0' , 0x00, '1' , 0x00, '1' , 0x00,
	                                       '2' , 0x00, '0' , 0x00, '0' , 0x00, '0' , 0x00, '3' , 0x00
	                                       }; //"20101201120003"
const unsigned char  iManufacturer[42]=  {0x28,  0x03, 'A' , 0x00, 'l' , 0x00, 'l' , 0x00, 'W' , 0x00,
	                                       'i',  0x00, 'n' , 0x00, 'n' , 0x00, 'e' , 0x00, 'r' , 0x00,
	                                       ' ',  0x00, 'T' , 0x00, 'e' , 0x00, 'c' , 0x00, 'h' , 0x00,
	                                       'n' , 0x00, 'o' , 0x00, 'l' , 0x00, 'o' , 0x00, 'g' , 0x00,
	                                       'y' , 0x00};  //AllWinner Technology
#if 0
const unsigned char  iProduct[22]     = {0x16, 0x03, 'U' , 0x00, 'S' , 0x00, 'B' , 0x00, ' ' , 0x00,
	                                       'T' , 0x00, 'e' , 0x00, 's' , 0x00, 't' , 0x00, 'e' , 0x00,
	                                       'r' , 0x00};      //"USB Tester"
#endif
const unsigned char  iProduct_new[90]     = {0x22, 0x03, 'U', 0x00, 'S', 0x00, 'B', 0x00, ' ', 0x00,
	                                          'S', 0x00, 't', 0x00, 'o', 0x00, 'r', 0x00, 'a', 0x00,
	                                          'g', 0x00, 'e', 0x00, ' ', 0x00, 'T', 0x00, 'o', 0x00,
	                                          'o', 0x00, 'l', 0x00};  //USB Storage Tool



const unsigned char TestPkt[54] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA,
	                                 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xEE, 0xEE, 0xEE,
	                                 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
	                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF,
	                                 0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF, 0xEF, 0xF7,
	                                 0xFB, 0xFD, 0x7E, 0x00};


const unsigned char* USB_StrDec0[4]    = {LangID, iSerialNum0, iManufacturer, iProduct_new};

const unsigned char USB_DevQual[10]   = {0x0A, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00};

const unsigned char USB_OTGDesc[3] = {0x03, 0x09, 0x03};

///////////////////////////////////////////////////////////
//For MassStorage Only
///////////////////////////////////////////////////////////
const unsigned char  InquiryData[]  = {  0x00, 0x80, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 'A', 'W',
	                                     'T',  'e' , 'c' , 'h' , ' ',  ' ',  'U' , 'S',  'B', ' ',
	                                     'S',  't',  'o' , 'r' , 'a' , 'g' , 'e' , ' ',  ' ', ' ',
	                                     ' ',  0x20, 0x30, 0x31, 0x30, 0x30};

const unsigned char SenseData[] = {0x03, 0x00, 0x00, 0x00};

const unsigned char RequestSense[] = {0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x3a,0x00,0x00,0x00,0x00,0x00};



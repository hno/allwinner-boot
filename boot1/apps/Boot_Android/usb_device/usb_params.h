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
#ifndef __USB_PARAMS_H__
#define __USB_PARAMS_H__

#include "include.h"

#define USB0_ROLE  				USB_ROLE_DEV
#define USB0_SPEED 				USB_SPEED_HS  //USB_SPEED_HS, USB_SPEED_FS

#define BULK_IN_EP    			0x1
#define BULK_OUT_EP				0x2

#define USB_DEV_SEC_BITS   		9//
#define USB_DEV_SEC_SIZE   		(0x1<<USB_DEV_SEC_BITS)//


#define USB_EP_FIFO_SIZE		512
#define USB_FIFO_ADDR_BLOCK		64//(USB_EP_FIFO_SIZE<<1)

#define USB_MAX_LUN    			0//

#define MAX_DDMA_SIZE			(16*1024*1024)  //16MB

#define USB_DEV0_TOTAL_CAP  	1//MB
#define USB_DEV0_SEC_BITS   	9//
#define USB_DEV0_SEC_SIZE   	(0x1<<USB_DEV0_SEC_BITS)//
#define USB_DEV0_SEC_CNT    	(((USB_DEV0_TOTAL_CAP-1)<<11)|0xFFF)//

//#define USB_BO_DEV0_MEM_BASE	0x80800000//

//#define USB_BO_DEV0_BUF_BASE	(USB_BO_DEV0_MEM_BASE + (USB_DEV0_TOTAL_CAP<<20))//
#define USB_BO_DEV_BUF_SIZE	    ( 64 * 1024)//

//VBUS Level
#define USB_VBUS_SESSEND		0
#define USB_VUBS_SESSVLD		1
#define USB_VBUS_ABVALID		2
#define USB_VBUS_VBUSVLD		3


////////////////////////////////////////////////////////////////////////////
//enum USB_DEVICE_CLASS {UDC_MassStorage=0x08, UDC_HID=0x03, UDC_HUB=0x09};

#define USB_DEV_DESC_LEN		18
#define USB_CONFIG_DESC_LEN		9
#define USB_INTF_DESC_LEN		9
#define USB_ENDP_DESC_LEN		7

#define USB_CBW_LEN				(31)
#define USB_CBW_SIG				(0x43425355)

#define USB_CSW_LEN				(13)
#define USB_CSW_SIG				(0x53425355)



//////////////////////////////////////////////////////////////////////////
#define   USB2DRAM_PARAMS    	0x0f000f0f
#define   DRAM2USB_PARAMS    	0x0f0f0f00



//////////////////////////////////////////////////////////////////////////
typedef unsigned char   		UBYTE;  /* unsigned byte */
typedef unsigned short  		UHWORD; /* unsigned half word */
typedef unsigned long   		UWORD;  /* unsigned word */


#define get_bvalue(n)    		(*((volatile UBYTE *)(n)))          /* byte input */
#define put_bvalue(n,c)  		(*((volatile UBYTE *)(n)) = (c))    /* byte output */
#define get_hvalue(n)   		(*((volatile UHWORD *)(n)))         /* half word input */
#define put_hvalue(n,c) 		(*((volatile UHWORD *)(n)) = (c))   /* half word output */
#define get_wvalue(n)   		(*((volatile UWORD *)(n)))          /* word input */
#define put_wvalue(n,c) 		(*((volatile UWORD *)(n)) = (c))    /* word output */

//#define set_bit(n,c)    		(*((volatile UBYTE *)(n))|= (c))    /* byte bit set */
#define clr_bit(n,c)    		(*((volatile UBYTE *)(n))&=~(c))    /* byte bit clear */
#define set_bbit(n,c)    		(*((volatile UBYTE *)(n))|= (c))    /* byte bit set */
#define clr_bbit(n,c)    		(*((volatile UBYTE *)(n))&=~(c))    /* byte bit clear */
#define set_hbit(n,c)   		(*((volatile UHWORD *)(n))|= (c))   /* half word bit set */
#define clr_hbit(n,c)   		(*((volatile UHWORD *)(n))&=~(c))   /* half word bit clear */
#define set_wbit(n,c)   		(*((volatile UWORD *)(n))|= (c))    /* word bit set */
#define clr_wbit(n,c)   		(*((volatile UWORD *)(n))&=~(c))    /* word bit clr */

//#define max(x,y) 				(((x)<(y))?(y):(x))
//#define min(x,y) 				(((x)<(y))?(x):(y))


#endif   //__USB_PARAMS_H__




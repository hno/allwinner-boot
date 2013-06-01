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
#ifndef   __USB_STORAGE_H__
#define   __USB_STORAGE_H__

#include "usb_register.h"
#include "usb_params.h"

#define USB_SIE_COUNT		3

#define USB_MAX_EP_NO		5

typedef enum {
	USB_RETVAL_NOTCOMP = 0,
	USB_RETVAL_COMPOK,
	USB_RETVAL_COMPERR
	} USB_RETVAL;

typedef struct
{
    __u8 bmRequest;
    __u8 bRequest;
    __u16 wValue;
    __u16 wIndex;
    __u16 wLength;
} uSetupPKG, *pSetupPKG;

typedef struct {
	__u32 func_addr;

	const __u8* dev_desc;
	const __u8* config_desc; //Support 4 Configurations Most
	const __u8* str_desc[4];
	const __u8* intf_desc;
	const __u8* endp_desc;
	const __u8* dev_qual;
	const __u8* otg_desc;

	//__u32 ConfigDesc_Len;
	__u32 MaxLUN;

	//EP protocol
	#define USB_PRTCL_ILL  	0
	#define USB_PRTCL_ISO  	1
	#define USB_PRTCL_BULK 	2
	#define USB_PRTCL_INT  	3
	__u32 eptx_prtcl[USB_MAX_EP_NO];
	__u32 eprx_prtcl[USB_MAX_EP_NO];
	__u32 eptx_fifo[USB_MAX_EP_NO];  //[31:16]-fifo address; [15]-double buffer; [14:0]-fifo size
	__u32 eprx_fifo[USB_MAX_EP_NO];  //[31:16]-fifo address; [15]-double buffer; [14:0]-fifo size
	__u32 epx_xfer_addr;
	__u32 epx_xfer_residue;
	__u32 epx_xfer_tranferred;
	__u32 epx_buf_tag;

	//Bulk Only Device State Machine
	#define USB_BO_IDLE				0
	#define USB_BO_CBW				1
	#define USB_BO_RXDATA			2
	#define USB_BO_TXDATA			3
	#define USB_BO_CSW				4
	__u32 bo_state;						//Bulk only device state
	__u32 bo_ep_in;
	__u32 bo_ep_out;
	__u32 bo_xfer_addr;				//Bulk only data address
	__u32 bo_xfer_residue;			//Bulk only data residue length
	__u32 bo_xfer_tranferred;  //Bulk only data transferred length
	__u32 bo_memory_base;
	//__u32 bo_capacity;
	//__u32 bo_seccnt;
	__u32 bo_bufbase;

	__u32 csw_fail_flag;
} usb_device, *pusb_device;

typedef struct {
	//USB SIE Hardware Config
	__u32 index;
	__u32 reg_base;
	__u32 irq_no;
	__u32 drq_no;

	volatile __u32 sof_count;

	//USB user initial parameters
	#define USB_ROLE_DEV		2
	__u32 role;
	#define USB_SPEED_UNK		0
	#define USB_SPEED_HS		1
	#define USB_SPEED_FS		2
	#define USB_SPEED_LS		3
	__u32 speed;
	__u32 srp;
	__u32 hnp;

	//usb irq record
	volatile __u32 busirq_status;
	volatile __u32 busirq_flag;

	//Signals for usb state
	volatile __u32 connect;
	volatile __u32 suspend;
	volatile __u32 reset;
	#define USB_OTG_A_DEVICE			0
	#define USB_OTG_B_DEVICE			1
	volatile __u32 otg_dev;

	//Signals for usb debug
	__u32 rst_cnt;
	__u32 cur_fsm;
	__u32 fsm_cnt;
	__u8  fsm_rcd[512];

	//Signals for Data Transfer
	volatile __u32 ep0_flag;
	__u32 ep0_maxpktsz;
	#define USB_EP0_SETUP					0
	#define USB_EP0_DATA					1
	#define USB_EP0_STATUS				2
	__u32 ep0_xfer_state;
	__u32 ep0_xfer_srcaddr;
	__u32 ep0_xfer_residue;
	__u32 ep0_xfer_tranferred;
	//uSetupPKG ep0_setup;

	volatile __u32 eptx_flag[USB_MAX_EP_NO];
	volatile __u32 eprx_flag[USB_MAX_EP_NO];
	#define USB_EPX_SETUP					0
	#define USB_EPX_DATA					1
	#define USB_EPX_END						2
	__u32 eptx_xfer_state[USB_MAX_EP_NO];
	__u32 eprx_xfer_state[USB_MAX_EP_NO];

	//Function Description
	usb_device device;

	//DMA Description
	//DMADESC    dma;
	__u32		 dma;
	__u32		 dma_last_transfer;

	//Timer
	#define USB_IDLE_TIMER								0x0
	#define USB_DEVICE_VBUS_DET_TIMER			0x1
	#define USB_HOST_RESET_TIMER					0x2
	#define USB_HOST_RESUME_TIMER					0x3
	#define USB_HOST_SUSPEND_TIMER				0x4
	#define USB_HOST_DELAY_TIMER					0x5
	__u32 timer;     //timer purpose
	#define USB_DEVICE_VBUS_DET_TIMEOUT		10  //10ms
	__u32 timeout;   //timeout value (in ms)
	__u32 loop;      //Loop counter

	#define USB_BUFFER_SIZE							256
	#define USB_EP0_OUT_BUFFER_START		8
	volatile __u8 buffer[USB_BUFFER_SIZE];

  __u32 power_debouce;
} usb_struct, *pusb_struct;

typedef struct
{
    __u32 dCBWSig;
    __u32 dCBWTag;
    __u32 dCBWDTL;
    __u8  bmCBWFlg;
    __u8  bCBWLUN   : 4;
    __u8  bCBWRes1  : 4;   //Reserved
    __u8  bCBWCBL   : 5;
    __u8  bCBWRes2  : 3;   //Reserved
    __u8  CBWCB[16];
} uCBWPKG, *pCBWPKG;

typedef struct
{
  	__u32 dCSWSig;
  	__u32 dCSWTag;
  	__u32 dCSWDataRes;
  	__u8  bCSWStatus;
} uCSWPKG, *pCSWPKG;




typedef struct
{
  __u8  bLength;
  __u8  bDesType;
  __u8  wTotalLen0;
  __u8  wTotalLen1;
  __u8  bNumIntf;
  __u8  bConfigVal;
  __u8  iConfig;
  __u8  bmAttrib;
  __u8  MaxPower;
} uConfigDes, *pConfigDes;

typedef struct
{
  __u8  bLength;
  __u8  bDesType;
  __u8  bIntfNum;
  __u8  bAltSet;
  __u8  bNumEP;
  __u8  bIntfClass;
  __u8  bIntfSubClass;
  __u8  bIntfProtocol;
  __u8  iInterface;
} uIntfDes, *pIntfDes;


typedef struct
{
  __u8  bLength;
  __u8  bDesType;
  __u8  bEPAddr;
  __u8  bmAttrib;
  __u8  wMaxPktSize0;
  __u8  wMaxPktSize1;
  __u8  bInterval;
} uEPDes, *pEPDes;



#endif   //__USB_STORAGE_H__

















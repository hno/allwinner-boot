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

#include "usb_register.h"
#include "usb_params.h"
#include "usb_storage.h"


//////////////////////////////////////////////////////////////////
 __u32 usb_get_epx_fifo_access(pusb_struct pusb, __u32 ep_no)
{
	return (pusb->reg_base + ((ep_no&0xf)<<2));
}

 void usb_set_dev_addr(pusb_struct pusb, __u32 addr)
{
	put_bvalue(pusb->reg_base + USB_bFADDR_OFF, addr & 0x7f);
}

 void usb_set_ep0_addr(pusb_struct pusb, __u32 addr)
{
	put_bvalue(pusb->reg_base + USB_bTXFADDR_OFF, addr & 0x7f);
}

 void usb_iso_update_enable(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<7);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_iso_update_disable(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<7);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_soft_connect(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<6);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_soft_disconnect(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<6);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_high_speed_enable(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<5);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_high_speed_disable(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<5);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 __u32 usb_is_high_speed(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bPOWER_OFF)>>4)&0x1;
}

 void usb_set_reset(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<3);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_clear_reset(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<3);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_set_resume(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<2);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_clear_resume(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<2);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}


 void usb_set_suspend(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<1);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_clear_suspend(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<1);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}


 __u32 usb_check_suspend(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bPOWER_OFF)>>1)&0x01;
}

 void usb_suspendm_enable(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val |= (0x1<<0);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 void usb_suspendm_disable(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bPOWER_OFF);
	reg_val &= ~(0x1<<0);
	put_bvalue(pusb->reg_base + USB_bPOWER_OFF, reg_val);
}

 __u32 usb_get_ep0_interrupt_status(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hINTRTX_OFF)&0x1;
}

 void usb_clear_ep0_interrupt_status(pusb_struct pusb)
{
	put_hvalue(pusb->reg_base + USB_hINTRTX_OFF, 0x1);
}

 __u32 usb_get_eptx_interrupt_status(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hINTRTX_OFF)&0xffff;
}

 void usb_clear_eptx_interrupt_status(pusb_struct pusb, __u32 bm)
{
	put_hvalue(pusb->reg_base + USB_hINTRTX_OFF, bm&0xffff);
}

 __u32 usb_get_eprx_interrupt_status(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hINTRRX_OFF)&0xffff;
}

 void usb_clear_eprx_interrupt_status(pusb_struct pusb, __u32 bm)
{
	put_hvalue(pusb->reg_base + USB_hINTRRX_OFF, bm&0xffff);
}

 void usb_set_eptx_interrupt_enable(pusb_struct pusb, __u32 bm)
{
	__u16 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hINTRTXE_OFF);
	reg_val |= (bm&0xffff);
	put_hvalue(pusb->reg_base + USB_hINTRTXE_OFF, reg_val);
}

 void usb_clear_eptx_interrupt_enable(pusb_struct pusb, __u32 bm)
{
	__u16 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hINTRTXE_OFF);
	reg_val &= ~(bm&0xffff);
	put_hvalue(pusb->reg_base + USB_hINTRTXE_OFF, reg_val);
}

 void usb_set_eprx_interrupt_enable(pusb_struct pusb, __u32 bm)
{
	__u16 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hINTRRXE_OFF);
	reg_val |= (bm&0xffff);
	put_hvalue(pusb->reg_base + USB_hINTRRXE_OFF, reg_val);
}

 void usb_clear_eprx_interrupt_enable(pusb_struct pusb, __u32 bm)
{
	__u16 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hINTRRXE_OFF);
	reg_val &= ~(bm&0xffff);
	put_hvalue(pusb->reg_base + USB_hINTRRXE_OFF, reg_val);
}

 __u32 usb_get_bus_interrupt_status(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_bINTRUSB_OFF)&0xff;
}

 void usb_clear_bus_interrupt_status(pusb_struct pusb, __u32 bm)
{
	put_bvalue(pusb->reg_base + USB_bINTRUSB_OFF, bm&0xff);
}

 __u32 usb_get_bus_interrupt_enable(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_bINTRUSBE_OFF)&0xff;
}

 void usb_set_bus_interrupt_enable(pusb_struct pusb, __u32 bm)
{
	__u16 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bINTRUSBE_OFF);
	reg_val |= (bm&0xff);
	put_bvalue(pusb->reg_base + USB_bINTRUSBE_OFF, reg_val);
}

 void usb_clear_bus_interrupt_enable(pusb_struct pusb, __u32 bm)
{
	__u16 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bINTRUSBE_OFF);
	reg_val &= ~(bm&0xff);
	put_bvalue(pusb->reg_base + USB_bINTRUSBE_OFF, reg_val);
}

 __u32 usb_get_frame_number(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hFRAME_OFF)&0x7ff;
}

 void usb_select_ep(pusb_struct pusb, __u32 ep_no)
{
	if(ep_no > USB_MAX_EP_NO) return;
	put_bvalue(pusb->reg_base + USB_bINDEX_OFF, ep_no);
}

 __u32 usb_get_active_ep(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_bINDEX_OFF)&0xf;
}

 void usb_set_test_mode(pusb_struct pusb, __u32 bm)
{
	put_bvalue(pusb->reg_base + USB_bTESTMODE_OFF, bm&0xff);
}

 void usb_set_eptx_maxpkt(pusb_struct pusb, __u32 maxpayload, __u32 pktcnt)
{
	__u32 reg_val;

	reg_val = maxpayload & 0x7ff;
	reg_val |= ((pktcnt-1) & 0x1f)<<11;
	put_hvalue(pusb->reg_base + USB_hTXMAXP_OFF, reg_val & 0xffff);
}

 __u32 usb_get_eptx_maxpkt(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hTXMAXP_OFF)&0xffff;
}


 void usb_ep0_disable_ping(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val |= (0x1<<8);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_enable_ping(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val &= ~(0x1<<8);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_flush_fifo(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val |= (0x1<<8);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 __u32 usb_ep0_is_naktimeout(pusb_struct pusb)
{
	return (get_hvalue(pusb->reg_base + USB_hCSR0_OFF)>>7)&0x1;
}

 void usb_ep0_clear_naktimeout(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val &= ~(0x1<<7);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_set_statuspkt(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val |= (0x1<<6);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_clear_statuspkt(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val &= ~(0x1<<6);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_set_reqpkt(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val |= (0x1<<5);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_clear_setupend(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val |= (0x1<<7);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}

 void usb_ep0_clear_rxpktrdy(pusb_struct pusb)
{
	__u32 reg_val;

	reg_val = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	reg_val |= (0x1<<6);
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, reg_val);
}


 __u32 usb_get_ep0_csr(pusb_struct pusb)
{
	__u32 ret;

	ret = get_hvalue(pusb->reg_base + USB_hCSR0_OFF);
	return ret;
}

 void usb_set_ep0_csr(pusb_struct pusb, __u32 csr)
{
	put_hvalue(pusb->reg_base + USB_hCSR0_OFF, csr);
}

 __u32 usb_get_eptx_csr(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hTXCSR_OFF);
}

 void usb_set_eptx_csr(pusb_struct pusb, __u32 csr)
{
	put_hvalue(pusb->reg_base + USB_hTXCSR_OFF, csr);
}

 void usb_eptx_flush_fifo(pusb_struct pusb)
{
	put_hvalue(pusb->reg_base + USB_hTXCSR_OFF, 0x1<<3);
}

 void usb_set_eprx_maxpkt(pusb_struct pusb, __u32 maxpayload, __u32 pktcnt)
{
	__u32 reg_val;

	reg_val = maxpayload & 0x7ff;
	reg_val |= ((pktcnt-1) & 0x1f)<<11;
	put_hvalue(pusb->reg_base + USB_hRXMAXP_OFF, reg_val & 0xffff);
}

 __u32 usb_get_eprx_maxpkt(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hRXMAXP_OFF)&0xffff;
}

 __u32 usb_get_eprx_csr(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hRXCSR_OFF);
}

 void usb_set_eprx_csr(pusb_struct pusb, __u32 csr)
{
	put_hvalue(pusb->reg_base + USB_hRXCSR_OFF, csr);
}

 void usb_set_eprx_csrhi(pusb_struct pusb, __u32 csrhi)
{
	put_bvalue(pusb->reg_base + USB_hRXCSR_OFF + 1, csrhi);
}

 void usb_eprx_flush_fifo(pusb_struct pusb)
{
	put_hvalue(pusb->reg_base + USB_hRXCSR_OFF, 0x1<<4);
}

 __u32 usb_get_ep0_count(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hCOUNT0_OFF)&0x7f;
}

 __u32 usb_get_eprx_count(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hRXCOUNT_OFF)&0x1fff;
}

 void usb_set_ep0_type(pusb_struct pusb, __u32 speed)
{
	put_bvalue(pusb->reg_base + USB_bTXTYPE_OFF, (speed&0x3)<<6);
}

 void usb_set_eptx_type(pusb_struct pusb, __u32 speed, __u32 protocol, __u32 ep_no)
{
	__u32 reg_val;

	reg_val = (speed&0x3)<<6;
	reg_val |= (protocol&0x3)<<4;
	reg_val |= (ep_no&0xf)<<0;
	put_bvalue(pusb->reg_base + USB_bTXTYPE_OFF, reg_val);
}

 void usb_set_ep0_naklimit(pusb_struct pusb, __u32 naklimit)
{
	put_bvalue(pusb->reg_base + USB_bTXINTERVAL_OFF, naklimit&0x1f);
}

 void usb_set_eptx_interval(pusb_struct pusb, __u32 interval)
{
	put_bvalue(pusb->reg_base + USB_bTXINTERVAL_OFF, interval&0xff);
}

 void usb_set_eprx_type(pusb_struct pusb, __u32 speed, __u32 protocol, __u32 ep_no)
{
	__u32 reg_val;

	reg_val = (speed&0x3)<<6;
	reg_val |= (protocol&0x3)<<4;
	reg_val |= (ep_no&0xf)<<0;
	put_bvalue(pusb->reg_base + USB_bRXTYPE_OFF, reg_val);
}

 void usb_set_eprx_interval(pusb_struct pusb, __u32 interval)
{
	put_bvalue(pusb->reg_base + USB_bRXINTERVAL_OFF, interval&0xff);
}

 __u32 usb_get_core_config(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_bCORECONFIG_OFF);
}

 __u32 usb_is_b_device(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>7)&0x1;
}

 __u32 usb_device_connected_is_fs(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>6)&0x1;
}

 __u32 usb_device_connected_is_ls(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>5)&0x1;
}

 __u32 usb_get_vbus_level(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>3)&0x3;
}

 __u32 usb_is_host(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>2)&0x1;
}

 void usb_set_hnp_request(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF);
	reg_val |= 0x1<<1;
	put_bvalue(pusb->reg_base + USB_bDEVCTL_OFF, reg_val);
}

 __u32 usb_hnp_in_porcess(pusb_struct pusb)
{
	return (get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>1)&0x1;
}

 void usb_start_session(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = 0x1;
	put_bvalue(pusb->reg_base + USB_bDEVCTL_OFF, reg_val);
}

 void usb_end_session(pusb_struct pusb)
{
	__u8 reg_val;

	reg_val = 0x0;
	put_bvalue(pusb->reg_base + USB_bDEVCTL_OFF, reg_val);
}

 __u32 usb_check_session(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)&0x1;
}

 __u32 aw_log2(__u32 x)
{
  	__u32 input;
  	__u32 val;

  	input = x;
  	val = 0;
  	while(input>1)
  	{
		input = input>>1;
		val++;
  	}

  	return val;
}

 void usb_set_eptx_fifo_size(pusb_struct pusb, __u32 is_dpb, __u32 size)
{
	__u8 reg_val;

	reg_val = 0;
	if(is_dpb) reg_val |= 0x1<<4;
	reg_val |= aw_log2(size>>3)&0xf;
	put_bvalue(pusb->reg_base + USB_bTxFIFOsz_OFF, reg_val);
}

 void usb_set_eprx_fifo_size(pusb_struct pusb, __u32 is_dpb, __u32 size)
{
	__u8 reg_val;

	reg_val = 0;
	if(is_dpb) reg_val |= 0x1<<4;
	reg_val |= aw_log2(size>>3)&0xf;
	put_bvalue(pusb->reg_base + USB_bRxFIFOsz_OFF, reg_val);
}

 void usb_set_eptx_fifo_addr(pusb_struct pusb, __u32 addr)
{
	put_hvalue(pusb->reg_base + USB_hTxFIFOad_OFF, (addr>>3)&0x1fff);
}

 void usb_set_eprx_fifo_addr(pusb_struct pusb, __u32 addr)
{
	put_hvalue(pusb->reg_base + USB_hRxFIFOad_OFF, (addr>>3)&0x1fff);
}

 void usb_fifo_accessed_by_cpu(pusb_struct pusb)
{
	clr_bbit(pusb->reg_base + USB_bDMACTL_OFF, 0x1<<0);
}

 void usb_fifo_accessed_by_dma(pusb_struct pusb, __u32 ep_no, __u32 is_tx)
{
	__u8 reg_val;

	if(ep_no>USB_MAX_EP_NO) return;
	reg_val = 0x1;
	if(!is_tx) reg_val |= 0x1<<1;
	reg_val |= (ep_no-1)<<2;
	put_bvalue(pusb->reg_base + USB_bDMACTL_OFF, reg_val);
}

 __u8 usb_get_fifo_access_config(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_bDMACTL_OFF)&0xff;
}

 __u32 usb_get_dma_ep_no(pusb_struct pusb)
{
	return ((get_bvalue(pusb->reg_base + USB_bDMACTL_OFF)&0x1f)>>2)+1;
}

 void usb_set_fifo_access_config(pusb_struct pusb, __u8 config)
{
	put_bvalue(pusb->reg_base + USB_bDMACTL_OFF, config);
}

 __u32 usb_get_fsm(pusb_struct pusb)
{
	return get_bvalue(pusb->reg_base + USB_FSM_OFF);
}


 void usb_set_eptx_faddr(pusb_struct pusb, __u32 faddr)
{
	put_bvalue(pusb->reg_base + USB_bTXFADDR_OFF, faddr&0x7f);
}

 void usb_set_eptx_haddr(pusb_struct pusb, __u32 haddr, __u32 is_mtt)
{
	__u8 reg_val;

	reg_val = 0;
	if(is_mtt) reg_val |= 0x1<<7;
	reg_val |= haddr&0x7f;
	put_bvalue(pusb->reg_base + USB_bTXHADDR_OFF, reg_val);
}

 void usb_set_eptx_hport(pusb_struct pusb, __u32 hport)
{
	put_bvalue(pusb->reg_base + USB_bTXHPORT_OFF, hport&0x7f);
}

 void usb_set_eprx_faddr(pusb_struct pusb, __u32 faddr)
{
	put_bvalue(pusb->reg_base + USB_bRXFADDR_OFF, faddr&0x7f);
}

 void usb_set_eprx_haddr(pusb_struct pusb, __u32 haddr, __u32 is_mtt)
{
	__u8 reg_val;

	reg_val = 0;
	if(is_mtt) reg_val |= 0x1<<7;
	reg_val |= haddr&0x7f;
	put_bvalue(pusb->reg_base + USB_bRXHADDR_OFF, reg_val);
}

 void usb_set_eprx_hport(pusb_struct pusb, __u32 hport)
{
	put_bvalue(pusb->reg_base + USB_bRXHPORT_OFF, hport&0x7f);
}


 void usb_set_reqpkt_count(pusb_struct pusb, __u32 count)
{
	put_hvalue(pusb->reg_base + USB_hRXPKTCNT_OFF, count&0xffff);
}

 __u32 usb_get_reqpkt_count(pusb_struct pusb)
{
	return get_hvalue(pusb->reg_base + USB_hRXPKTCNT_OFF)&0xffff;
}

 __u32 usb_get_lsvbusvld(pusb_struct pusb)
{
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>30)&0x1;
}

 __u32 usb_get_extvbusvld(pusb_struct pusb)
{
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>29)&0x1;
}

 __u32 usb_get_extiddig(pusb_struct pusb)
{
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>28)&0x1;
}

 __u32 usb_get_linestate(pusb_struct pusb)
{
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>26)&0x3;
}

 __u32 usb_get_vbusvalid(pusb_struct pusb)
{
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>25)&0x1;
}

 __u32 usb_get_iddig(pusb_struct pusb)
{
	#ifdef CONFIG_AW_FPGA_PLATFORM
	return ((get_bvalue(pusb->reg_base + USB_bDEVCTL_OFF)>>0x7)&0x1);
	#else
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>24)&0x1;
	#endif
}

 __u32 usb_get_fs_linestate(pusb_struct pusb)
{
	return (get_wvalue(pusb->reg_base + USB_wISCR_OFF)>>20)&0x3;
}

 void usb_iddig_pullup_enable(pusb_struct pusb)
{
	set_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<17);
}

 void usb_iddig_pullup_disable(pusb_struct pusb)
{
	clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<17);
}

 void usb_dpdm_pullup_enable(pusb_struct pusb)
{
	set_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<16);
}

 void usb_dpdm_pullup_disable(pusb_struct pusb)
{
	clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<16);
}

 void usb_release_id(pusb_struct pusb)
{
	clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x3<<14);
}

 void usb_force_id(pusb_struct pusb, __u32 id)
{

	if(id) 	set_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<14);
	else   	clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<14);
	set_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<15);

}

 void usb_vbus_src(pusb_struct pusb, __u32 src)
{
	clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x3<<10);
	set_wbit(pusb->reg_base + USB_wISCR_OFF, (src&0x03)<<10);
}

 void usb_release_vbus(pusb_struct pusb)
{
	clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x3<<12);
}

 void usb_force_vbus(pusb_struct pusb, __u32 vbus)
{
	if(vbus) 	set_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<12);
	else   		clr_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<12);
	set_wbit(pusb->reg_base + USB_wISCR_OFF, 0x1<<13);
}

 void usb_drive_vbus(pusb_struct pusb, __u32 vbus, __u32 index)
{
	__u32 temp;
	//Set PB9 Output,USB0-DRV SET 
	if(index == 0)
	{
	  temp = get_wvalue(0x01c20800+0x28);
	  temp &= ~(0x7<<4);
	  temp |= (0x1<<1);
	  put_wvalue(0x01c20800+0x28, temp);
	  if(vbus)
	  {
	  	temp = get_wvalue(0x01c20800+0x34);
	  	temp |= (0x1<<9);
	  	put_wvalue(0x01c20800+0x34,temp);
	  }
	  else
	  {
	    temp = get_wvalue(0x01c20800+0x34);
	  	temp &= ~(0x1<<9);
	  	put_wvalue(0x01c20800+0x34,temp);
	  }
	}

}

 __u32 usb_get_ep_fifo_addr(pusb_struct pusb, __u32 ep_no)
{
	return (pusb->reg_base + USB_bFIFO_OFF(ep_no));
}

// __u32 usb_dma_write_config(pusb_struct pusb)
//{
//	return (((USB_DMA_CONFIG|(0x1f&pusb->drq_no))<<16)|DRAM_DMA_CONFIG);
//}
//
// __u32 usb_dma_read_config(pusb_struct pusb)
//{
//	return ((DRAM_DMA_CONFIG<<16)|(USB_DMA_CONFIG|(0x1f&pusb->drq_no)));
//}


















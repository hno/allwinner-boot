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
#include "usb_descriptor.h"
#include "usb_params.h"
#include "usb_register.h"
#include "usb_storage.h"


typedef struct
{
	char  magic[16];       	//特征字符串，固定是 "usbhandshake"，不足的填空
	int   sizelo;			//盘符的低32位，单位是扇区
	int   sizehi;			//盘符的高32位，单位是扇区
	int   res1;
	int   res2;
}
__usb_handshake_t;



#define USB_NO_DMA		1

__u32 write_len = 0;
__u32 write_offset = 0;
__s32 part_index = 0;
__s32 write_start = 0;

__s32 usb_status = 0;

extern  __u32 usb_work_type;
extern  part_info_t  part_info;

__s32 dram_copy(__u32 src_addr, __u32 dest_addr, __u32 bytes);

__u32 aw_module(__u32 x, __u32 y)
{
	__u32 val;

	if(y==0)
	{
		return 0;
	}
	val = x;
	while(val>=y)
	{
		val -= y;
	}

	return val;
}

void usb_read_ep_fifo(pusb_struct pusb, __u32 ep_no, __u32 dest_addr, __u32 count)
{
	__u8 temp;
	__u8 saved;
	__u32 dest;
	__u32 i;

	if(ep_no>USB_MAX_EP_NO)
	{
		return;
	}
	saved = usb_get_fifo_access_config(pusb);
	usb_fifo_accessed_by_cpu(pusb);

	dest = dest_addr;
	for(i=0; i<count; i++)
	{
		temp = get_bvalue(pusb->reg_base + USB_bFIFO_OFF(ep_no));
		put_bvalue(dest, temp);
		dest += 1;
	}
	if((count!=31)&&(count!=8))
	{
		//USB_HAL_DBG("rxcount=%d, rxdata=0x%x\n", count, *((__u32*)dest_addr));
	}

	usb_set_fifo_access_config(pusb, saved);
}


void usb_write_ep_fifo(pusb_struct pusb, __u32 ep_no, __u32 src_addr, __u32 count)
{
	__u8  temp;
	__u8  saved;
	__u32 src;
	__u32 i;

	if(ep_no>USB_MAX_EP_NO)
	{
		return;
	}
	saved = usb_get_fifo_access_config(pusb);
	usb_fifo_accessed_by_cpu(pusb);

	src = src_addr;
	for(i=0; i<count; i++)
	{
		temp = get_bvalue(src);
		put_bvalue(pusb->reg_base + USB_bFIFO_OFF(ep_no), temp);
		src += 1;
	}

	usb_set_fifo_access_config(pusb, saved);
}

//__inline void usb_set_eptx_fifo_size(pusb_struct pusb, __u32 is_dpb, __u32 size)
//{
//	__u8 reg_val;
//
//	reg_val = 0;
//	if(is_dpb)
//	{
//		reg_val |= 0x1<<4;
//	}
//	reg_val |= aw_log2(size>>3)&0xf;
//	put_bvalue(pusb->reg_base + USB_bTxFIFOsz_OFF, reg_val);
//}
///////////////////////////////////////////////////////////////////
//                 usb bulk transfer
///////////////////////////////////////////////////////////////////
/*
************************************************************************************************************
*
*                                             usb_dev_get_buf_base
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：获取PING_PANG buffer地址
*
*
************************************************************************************************************
*/
#ifndef USB_NO_DMA
static __u32 usb_dev_get_buf_base(pusb_struct pusb, __u32 buf_tag)
{
	return (pusb->device.bo_bufbase + buf_tag*USB_BO_DEV_BUF_SIZE);
}
#endif
/*
************************************************************************************************************
*
*                                             epx_out_handler_dev
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：读取USB FIFO数据函数
*
*
************************************************************************************************************
*/
static USB_RETVAL epx_out_handler_dev(pusb_struct pusb, __u32 ep_no, __u32 dst_addr, __u32 byte_count, __u32 ep_type)
{
	USB_RETVAL ret = USB_RETVAL_NOTCOMP;
	__u32 maxpkt;
	__u32 ep_save = usb_get_active_ep(pusb);
	static __u32 epout_timeout = 0;
#ifndef USB_NO_DMA
	__dma_setting_t  p;
	__u32 dram_addr;
#endif

	usb_select_ep(pusb, ep_no);
	maxpkt = usb_get_eprx_maxpkt(pusb);
	maxpkt = (maxpkt&0x7ff)*(((maxpkt&0xf800)>>11)+1);

	switch(pusb->eprx_xfer_state[ep_no-1])
	{
		case USB_EPX_SETUP:
		{
			pusb->device.epx_buf_tag = 0;
			pusb->device.epx_xfer_addr = dst_addr;
			pusb->device.epx_xfer_residue = byte_count;
			pusb->device.epx_xfer_tranferred = 0;


			if(!maxpkt)
			{
				return USB_RETVAL_COMPOK;
			}
#if 1
			if(byte_count>=maxpkt)
			{
				__u32 xfer_count=0;

#ifndef USB_NO_DMA
				xfer_count = min(pusb->device.epx_xfer_residue, USB_BO_DEV_BUF_SIZE);
				pusb->dma_last_transfer = xfer_count;
				usb_fifo_accessed_by_dma(pusb, ep_no, 0);  //rx

				dram_addr = usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag);

				p.cfg.src_drq_type      = DDMA_SRC_USB0;
				p.cfg.src_addr_type     = 1;   //IO address
				p.cfg.src_burst_length  = 1;   //burst length = 4
				p.cfg.src_data_width    = 2;   //32bit
				p.cfg.dst_drq_type      = DDMA_DST_SDRAM;
				p.cfg.dst_addr_type     = 0;   //linear
				p.cfg.dst_burst_length  = 1;   //burst length = 4
				p.cfg.dst_data_width    = 2;   //32bit
				p.cfg.wait_state        = 0;
				p.cfg.continuous_mode   = 0;

				p.pgsz                  = 0;
				p.pgstp                 = 0;
				p.cmt_blk_cnt           = USB2DRAM_PARAMS;

				wBoot_dma_Setting(pusb->dma, (void *)&p);
				//这是读操作，读的时候刷新目的地址，原则就是刷新DRAM(SRAM)
				wlibc_CleanFlushDCacheRegion((void *)dram_addr, xfer_count);

				wBoot_dma_start(pusb->dma, usb_get_ep_fifo_addr(pusb, ep_no), dram_addr, xfer_count);

				usb_set_eprx_csrhi(pusb, (USB_RXCSR_AUTOCLR|USB_RXCSR_DMAREQEN)>>8);
				pusb->eprx_xfer_state[ep_no-1] = USB_EPX_DATA;
#else
			    xfer_count = min(pusb->device.epx_xfer_residue, maxpkt);
			    if(usb_get_eprx_csr(pusb)&USB_RXCSR_RXPKTRDY)
				{
					usb_fifo_accessed_by_cpu(pusb);
					if(usb_get_fifo_access_config(pusb)&0x1)
					{
						__inf("Error: CPU Access Failed!!\n");
					}
					usb_read_ep_fifo(pusb, ep_no, pusb->device.epx_xfer_addr, xfer_count);
					usb_set_eprx_csr(pusb, usb_get_eprx_csr(pusb)&USB_RXCSR_ISO); //Clear RxPktRdy
					pusb->device.epx_xfer_residue -= xfer_count;
					pusb->device.epx_xfer_addr += xfer_count;
					pusb->device.epx_xfer_tranferred  += xfer_count;
					pusb->eprx_xfer_state[ep_no-1] = USB_EPX_DATA;
					epout_timeout = 0;
				}
				else
				{
					epout_timeout ++;

					if(epout_timeout < 0x1000)
					{
						ret = USB_RETVAL_NOTCOMP;
					}
					else
					{
						ret = USB_RETVAL_COMPERR;
						__inf("Error: RxPktRdy Timeout!!\n");
					}
				}
#endif
			}
			else
#endif
			{
				if(usb_get_eprx_csr(pusb)&USB_RXCSR_RXPKTRDY)
				{
					usb_fifo_accessed_by_cpu(pusb);
					if(usb_get_fifo_access_config(pusb)&0x1)
					{
						__inf("Error: CPU Access Failed!!\n");
					}
					usb_read_ep_fifo(pusb, ep_no, pusb->device.epx_xfer_addr, byte_count);
					usb_set_eprx_csr(pusb, usb_get_eprx_csr(pusb)&USB_RXCSR_ISO); //Clear RxPktRdy
					pusb->device.epx_xfer_residue -= byte_count;
					pusb->device.epx_xfer_tranferred  += byte_count;
					pusb->eprx_xfer_state[ep_no-1] = USB_EPX_SETUP;
					ret = USB_RETVAL_COMPOK;
					epout_timeout = 0;
				}
				else
				{
					epout_timeout ++;

					if(epout_timeout < 0x1000)
					{
						ret = USB_RETVAL_NOTCOMP;
					}
					else
					{
						ret = USB_RETVAL_COMPERR;
						__inf("Error: RxPktRdy Timeout!!\n");
					}
				}
			}
		}
		break;

		case USB_EPX_DATA:
#ifndef USB_NO_DMA
		if(!wBoot_dma_QueryState(pusb->dma))
	 	{
	 		__u32 data_xfered = pusb->dma_last_transfer;

		  	pusb->device.epx_xfer_residue -= data_xfered;
		  	pusb->dma_last_transfer  = 0;
		  	pusb->device.epx_buf_tag = pusb->device.epx_buf_tag ? 0:1;

		  	if(pusb->device.epx_xfer_residue)
		  	{
		  		__u32 xfer_count = min(pusb->device.epx_xfer_residue, USB_BO_DEV_BUF_SIZE);

				pusb->dma_last_transfer = xfer_count;
		  		usb_fifo_accessed_by_dma(pusb, ep_no, 0);

		  		dram_addr = usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag);

			    p.cfg.src_drq_type      = DDMA_SRC_USB0;
			    p.cfg.src_addr_type     = 1;   //IO address
			    p.cfg.src_burst_length  = 1;   //burst length = 4
			    p.cfg.src_data_width    = 2;   //32bit
			    p.cfg.dst_drq_type      = DDMA_DST_SDRAM;
			    p.cfg.dst_addr_type     = 0;   //linear
			    p.cfg.dst_burst_length  = 1;   //burst length = 4
			    p.cfg.dst_data_width    = 2;   //32bit
			    p.cfg.wait_state        = 0;
			    p.cfg.continuous_mode   = 0;

			    p.pgsz                  = 0;
			    p.pgstp                 = 0;
			    p.cmt_blk_cnt           = USB2DRAM_PARAMS;

			    wBoot_dma_Setting(pusb->dma, (void *)&p);
			    //这是读操作，读的时候刷新目的地址，原则就是刷新DRAM(SRAM)
			    wlibc_CleanFlushDCacheRegion((void *)dram_addr, xfer_count);

			    wBoot_dma_start(pusb->dma, usb_get_ep_fifo_addr(pusb, ep_no), dram_addr, xfer_count);
		  	}
		  	else
		  	{
		  		if(byte_count&0x1ff)
			    {
				    usb_set_eprx_csr(pusb, usb_get_eprx_csr(pusb)&USB_RXCSR_ISO);
			    }
			    else
			   	{
				   usb_set_eprx_csr(pusb, usb_get_eprx_csr(pusb)&(USB_RXCSR_ISO|USB_RXCSR_RXPKTRDY));
			   	}
			    pusb->eprx_xfer_state[ep_no-1] = USB_EPX_SETUP;
			    ret = USB_RETVAL_COMPOK;
		  	}

			if(dram_copy(usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag? 0:1), pusb->device.epx_xfer_addr, data_xfered))
		  	{
		  		pusb->device.epx_xfer_addr += data_xfered;
		  		pusb->device.epx_xfer_tranferred += data_xfered;
		  	}
		  	else
		  	{
		  		__inf("Error: buffer to storage copy error!!\n");
			    pusb->eprx_xfer_state[ep_no-1] = USB_EPX_SETUP;
			    ret = USB_RETVAL_COMPERR;
		  	}
	 	}
	 	else
	 	{
	 		__inf("dma busy\n");
	 	}
#else
		if(pusb->device.epx_xfer_residue>0)
		{
			if(usb_get_eprx_csr(pusb)&USB_RXCSR_RXPKTRDY)
			{
				__u32 xfer_count = min(pusb->device.epx_xfer_residue, maxpkt);

				usb_fifo_accessed_by_cpu(pusb);
				if(usb_get_fifo_access_config(pusb)&0x1)
				{
					__inf("Error: CPU Access Failed!!\n");
				}
				usb_read_ep_fifo(pusb, ep_no, pusb->device.epx_xfer_addr, xfer_count);
				usb_set_eprx_csr(pusb, usb_get_eprx_csr(pusb)&USB_RXCSR_ISO); //Clear RxPktRdy
				pusb->device.epx_xfer_residue -= xfer_count;
				pusb->device.epx_xfer_addr += xfer_count;
				pusb->device.epx_xfer_tranferred  += xfer_count;
				pusb->eprx_xfer_state[ep_no-1] = USB_EPX_DATA;
				epout_timeout = 0;
			}
			else
			{
				epout_timeout ++;

				if(epout_timeout < 0x1000)
				{
					ret = USB_RETVAL_NOTCOMP;
				}
				else
				{
					ret = USB_RETVAL_COMPERR;
					__inf("Error: RxPktRdy Timeout!!\n");
				}
			}
		}
		else
		{
			pusb->eprx_xfer_state[ep_no-1] = USB_EPX_SETUP;
			ret = USB_RETVAL_COMPOK;
		}
#endif
		break;

		default:
			__inf("Error: Wrong eprx_xfer_state=%d\n", pusb->eprx_xfer_state[ep_no-1]);
			pusb->eprx_xfer_state[ep_no-1] = USB_EPX_SETUP;
	}

	usb_select_ep(pusb, ep_save);

	return ret;
}
/*
************************************************************************************************************
*
*                                             epx_in_handler_dev
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：写入USB FIFO数据函数
*
*
************************************************************************************************************
*/
static USB_RETVAL epx_in_handler_dev(pusb_struct pusb, __u32 ep_no, __u32 src_addr, __u32 byte_count, __u32 ep_type)
{
  	USB_RETVAL ret = USB_RETVAL_NOTCOMP;
	__u32 maxpkt;
	__u32 ep_save = usb_get_active_ep(pusb);
#ifndef USB_NO_DMA
	__dma_setting_t  p;
	__u32 dram_addr;
	__u32 ping_pang_addr;
#endif
	usb_select_ep(pusb, ep_no);
	maxpkt = usb_get_eptx_maxpkt(pusb);
	maxpkt = (maxpkt&0x7ff)*(((maxpkt&0xf800)>>11)+1);

	switch(pusb->eptx_xfer_state[ep_no-1])
	{
		case USB_EPX_SETUP:
		{
			pusb->device.epx_buf_tag = 0;
			pusb->device.epx_xfer_addr = src_addr;
			pusb->device.epx_xfer_residue = byte_count;
			pusb->device.epx_xfer_tranferred = 0;

			if(!maxpkt)
			{
				return USB_RETVAL_COMPOK;
			}
#if 1
			if(byte_count>=maxpkt)
		 	{
#ifndef USB_NO_DMA
				__u32 xfer_count = 0;

		 		xfer_count = min(pusb->device.epx_xfer_residue, USB_BO_DEV_BUF_SIZE);
		 		ping_pang_addr = usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag);
		 		if(dram_copy(pusb->device.epx_xfer_addr, ping_pang_addr, xfer_count))
	 			{
	 				pusb->device.epx_xfer_addr += xfer_count;
	 				pusb->device.epx_xfer_residue -= xfer_count;
	 			}
		 		else
	 			{
	 				__inf("Error: storage to buffer copy error!!\n");
			    	ret = USB_RETVAL_COMPERR;
			    	break;
	 			}

				usb_fifo_accessed_by_dma(pusb, ep_no, 1);
		 		if(!(usb_get_fifo_access_config(pusb)&0x1))
			    {
					__inf("Error: FIFO Access Config Error!!\n");
			    }

		  		dram_addr = usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag);

			    p.cfg.src_drq_type      = DDMA_DST_SDRAM;
			    p.cfg.src_addr_type     = 0;   //linear
			    p.cfg.src_burst_length  = 1;   //burst length = 4
			    p.cfg.src_data_width    = 2;   //32bit
			    p.cfg.dst_drq_type      = DDMA_SRC_USB0;
			    p.cfg.dst_addr_type     = 1;   //IO address
			    p.cfg.dst_burst_length  = 1;   //burst length = 4
			    p.cfg.dst_data_width    = 2;   //32bit
			    p.cfg.wait_state        = 0;
			    p.cfg.continuous_mode   = 0;

			    p.pgsz                  = 0;
			    p.pgstp                 = 0;
			    p.cmt_blk_cnt           = DRAM2USB_PARAMS;

			    wBoot_dma_Setting(pusb->dma, (void *)&p);
		        //这是写操作，写的时候刷新目的地址，原则就是刷新DRAM(SRAM)
		        wlibc_CleanFlushDCacheRegion((void *)dram_addr, xfer_count);

				wBoot_dma_start(pusb->dma, dram_addr, (__u32)ping_pang_addr, xfer_count);

			    pusb->device.epx_buf_tag = pusb->device.epx_buf_tag ? 0:1;

			    if(pusb->device.epx_xfer_residue)
		    	{
		    		xfer_count = min(pusb->device.epx_xfer_residue, USB_BO_DEV_BUF_SIZE);

		    		if(dram_copy(pusb->device.epx_xfer_addr, usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag), xfer_count))
	    			{
	    				pusb->device.epx_xfer_addr += xfer_count;
	    				pusb->device.epx_xfer_residue -= xfer_count;
	    			}
		    		else
	    			{
	    				__inf("Error: storage to buffer copy error!!\n");
			        	ret = USB_RETVAL_COMPERR;
	    			}
		    		pusb->dma_last_transfer = xfer_count;
		    	}
			    else
		    	{
		    		pusb->dma_last_transfer = 0;
		    	}

		    	usb_set_eptx_csr(pusb, USB_TXCSR_AUTOSET|USB_TXCSR_TXFIFO|USB_TXCSR_DMAREQEN|USB_TXCSR_DMAREQMODE);

				pusb->eptx_xfer_state[ep_no-1] = USB_EPX_DATA;
#else
				usb_fifo_accessed_by_cpu(pusb);
				usb_write_ep_fifo(pusb, ep_no, pusb->device.epx_xfer_addr, maxpkt);
				pusb->device.epx_xfer_residue -= maxpkt;
			  	pusb->device.epx_xfer_tranferred += maxpkt;
			  	pusb->device.epx_xfer_addr += maxpkt;
				usb_set_eptx_csr(pusb, USB_TXCSR_TXFIFO|USB_TXCSR_TXPKTRDY);
			    pusb->eptx_xfer_state[ep_no-1] = USB_EPX_DATA;
#endif
		 	}
			else
#endif
			{
				usb_fifo_accessed_by_cpu(pusb);

				usb_write_ep_fifo(pusb, ep_no, pusb->device.epx_xfer_addr, byte_count);
				if(usb_get_fifo_access_config(pusb)&0x1)
				{
					__inf("Error: FIFO Access Config Error!!\n");
			  	}
				usb_set_eptx_csr(pusb, USB_TXCSR_TXFIFO|USB_TXCSR_TXPKTRDY);
				pusb->eptx_xfer_state[ep_no-1] = USB_EPX_END;
			  	pusb->device.epx_xfer_residue = 0;
			  	pusb->device.epx_xfer_tranferred = byte_count;
			}
		}
		break;

		case USB_EPX_DATA:
		{
#ifndef USB_NO_DMA
			if(!wBoot_dma_QueryState(pusb->dma))
		 	{
		 		if(pusb->dma_last_transfer)
	 			{
			  		dram_addr = usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag);

				    p.cfg.src_drq_type      = DDMA_DST_SDRAM;
				    p.cfg.src_addr_type     = 0;   //linear
				    p.cfg.src_burst_length  = 1;   //burst length = 4
				    p.cfg.src_data_width    = 2;   //32bit
				    p.cfg.dst_drq_type      = DDMA_SRC_USB0;
				    p.cfg.dst_addr_type     = 1;   //IO address
				    p.cfg.dst_burst_length  = 1;   //burst length = 4
				    p.cfg.dst_data_width    = 2;   //32bit
				    p.cfg.wait_state        = 0;
				    p.cfg.continuous_mode   = 0;

				    p.pgsz                  = 0;
				    p.pgstp                 = 0;
				    p.cmt_blk_cnt           = DRAM2USB_PARAMS;

				    wBoot_dma_Setting(pusb->dma, (void *)&p);
			        //这是写操作，写的时候刷新目的地址，原则就是刷新DRAM(SRAM)
			        wlibc_CleanFlushDCacheRegion((void *)dram_addr, pusb->dma_last_transfer);

			        wBoot_dma_start(pusb->dma, dram_addr, usb_get_ep_fifo_addr(pusb, ep_no), pusb->dma_last_transfer);

			    	pusb->device.epx_buf_tag = pusb->device.epx_buf_tag ? 0:1;

			    	if(pusb->device.epx_xfer_residue)//Copy Data from storage to buffer
			    	{
			    		__u32 xfer_count = min(pusb->device.epx_xfer_residue, USB_BO_DEV_BUF_SIZE);

			    		if(dram_copy(pusb->device.epx_xfer_addr, usb_dev_get_buf_base(pusb, pusb->device.epx_buf_tag), xfer_count))
		    			{
		    				pusb->device.epx_xfer_addr += xfer_count;
		    				pusb->device.epx_xfer_residue -= xfer_count;
		    			}
		    			else
		    			{
		    				__inf("Error: storage to buffer copy error!!\n");
				        	ret = USB_RETVAL_COMPERR;
		    			}
			    		pusb->dma_last_transfer = xfer_count;
			    	}
			    	else
			    	{
			    		pusb->dma_last_transfer = 0;
			    	}
	 			}
	 			else
	 			{
	 				pusb->device.epx_xfer_tranferred = byte_count;
			    	maxpkt = usb_get_eptx_maxpkt(pusb);
			    	maxpkt = (maxpkt&0x7ff)*(((maxpkt&0xf800)>>11)+1);
			    	if(aw_module(byte_count, maxpkt))
			    	{
				   		usb_set_eptx_csr(pusb, usb_get_eptx_csr(pusb)|USB_TXCSR_TXPKTRDY);
			   	 	}
			    	pusb->eptx_xfer_state[ep_no-1] = USB_EPX_END;
	 			}
		 	}
#else
			if(!(usb_get_eptx_csr(pusb)&USB_TXCSR_TXPKTRDY))
			{
				if(pusb->device.epx_xfer_residue > 0)   //Data is not transfered over
				{
					__u32 xfer_count = min(pusb->device.epx_xfer_residue, maxpkt);

					usb_fifo_accessed_by_cpu(pusb);
					usb_write_ep_fifo(pusb, ep_no, pusb->device.epx_xfer_addr, xfer_count);
					pusb->device.epx_xfer_residue -= xfer_count;
			  		pusb->device.epx_xfer_tranferred += xfer_count;
			  		pusb->device.epx_xfer_addr += xfer_count;
					usb_set_eptx_csr(pusb, USB_TXCSR_TXFIFO|USB_TXCSR_TXPKTRDY);
			    	pusb->eptx_xfer_state[ep_no-1] = USB_EPX_DATA;
				}
				else if(!(usb_get_eptx_csr(pusb)&USB_TXCSR_FIFONOTEMP))
				{

					pusb->eptx_xfer_state[ep_no-1] = USB_EPX_END;
				}
			}
#endif
		}
		break;

		case USB_EPX_END:
		if(!(usb_get_eptx_csr(pusb)&0x3))
		{
			usb_set_eptx_csr(pusb, usb_get_eptx_csr(pusb)&USB_TXCSR_ISO);
			pusb->eptx_xfer_state[ep_no-1] = USB_EPX_SETUP;
			ret = USB_RETVAL_COMPOK;
		}
		break;

		default:
			__inf("Error: Wrong eptx_xfer_state=%d\n", pusb->eptx_xfer_state[ep_no-1]);
			pusb->eptx_xfer_state[ep_no-1] = USB_EPX_SETUP;
	}
	usb_select_ep(pusb, ep_save);

	return ret;
}
/*
************************************************************************************************************
*
*                                             usb_dev_bulk_xfer
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
USB_RETVAL usb_dev_bulk_xfer(pusb_struct pusb)
{
	__u32 rx_count=0;
	pCBWPKG pCBW;
	pCSWPKG pCSW;
	USB_RETVAL ret = USB_RETVAL_NOTCOMP;
	__u32 ep_save = usb_get_active_ep(pusb);
	USB_RETVAL fret = USB_RETVAL_NOTCOMP;

	pCBW = (pCBWPKG)(pusb->buffer);
  	pCSW = (pCSWPKG)(pusb->buffer);

	switch(pusb->device.bo_state)
	{
		case USB_BO_IDLE:
		case USB_BO_CBW:
		 	if(!pusb->eprx_flag[pusb->device.bo_ep_out-1])
  		 	{
  				break;
  		 	}

			pusb->eprx_flag[pusb->device.bo_ep_out-1]--;
	  		usb_select_ep(pusb, pusb->device.bo_ep_out);
	  		if(!(usb_get_eprx_csr(pusb)&USB_RXCSR_RXPKTRDY))
	  		{
	  			break;
	  		}
	  		rx_count = usb_get_eprx_count(pusb);
			if(rx_count != USB_CBW_LEN)
	  		{
	  			usb_eprx_flush_fifo(pusb);
	  			__inf("Error: Not CBW, RX Data Length=%d\n",rx_count);
	  			break;
	  		}
	  		do
	  		{
	  			ret = epx_out_handler_dev(pusb, pusb->device.bo_ep_out, (__u32)pusb->buffer, rx_count, USB_PRTCL_BULK);
	  		}
	  		while(ret == USB_RETVAL_NOTCOMP);

	  		if(ret == USB_RETVAL_COMPERR)
	  		{
	  			__inf("Error: RX CBW Error\n");
	  			break;
	  		}
	  		else
	  		{
	  			ret = USB_RETVAL_NOTCOMP;
	  		}
	  		if(pCBW->dCBWSig != USB_CBW_SIG)
	  		{
	  			__inf("Error: Not CBW, Error Signature=0x%x\n", pCBW->dCBWSig);
	  			break;
	  		}
#if 0
			{
				__u32 i;
	            __inf("dCBWSig = %x\n", pCBW->dCBWSig);
	            __inf("dCBWTag = %x\n", pCBW->dCBWTag);
	            __inf("dCBWDTL = %x\n", pCBW->dCBWDTL);
	            __inf("bmCBWFlg = %x\n", pCBW->bmCBWFlg);
	            __inf("bCBWLUN = %x\n", pCBW->bCBWLUN);
	            __inf("bCBWCBL = %x\n", pCBW->bCBWCBL);
	            for(i=0;i<16;i++)
	            {
	            	__inf("CBWCB[%d] = %x\n", i, pCBW->CBWCB[i]);
				}
			}
#endif
	  		switch(pCBW->CBWCB[0])
	  		{
	  			case 0x00://Test Unit Ready
					pCSW->dCSWSig = USB_CSW_SIG;
					pCSW->dCSWDataRes = 0;
					if(!usb_work_type)
					{
						pCSW->bCSWStatus = 0;
					}
					else
					{
						pCSW->bCSWStatus = 1;
					}
					pusb->device.bo_xfer_addr = (__u32)pCSW;
					pusb->device.bo_xfer_residue = USB_CSW_LEN;
					pusb->device.bo_xfer_tranferred = 0;
					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
						pusb->device.bo_state = USB_BO_CSW;
					}
					else
					{
						if(fret == USB_RETVAL_COMPOK)
						{
						 	pusb->device.bo_xfer_tranferred = pusb->device.bo_xfer_residue;
							pusb->device.bo_xfer_residue = 0;
							pusb->device.bo_state = USB_BO_CBW;
						}
						else
						{
						 	pusb->device.bo_state = USB_BO_CBW;
							__inf("Error: CSW Send Error!!\n");
						}
					}
	  				break;
	  			case 0x1e://Remove Allow/Prevent
					pCSW->dCSWSig = USB_CSW_SIG;
					pCSW->dCSWDataRes = 0;
					pCSW->bCSWStatus = 0;
					pusb->device.bo_xfer_addr = (__u32)pCSW;
					pusb->device.bo_xfer_residue = USB_CSW_LEN;
					pusb->device.bo_xfer_tranferred = 0;
					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
						pusb->device.bo_state = USB_BO_CSW;
					}
					else
					{
						pusb->device.bo_state = USB_BO_CBW;
						__inf("Error: CSW Send Error!!\n");
					}
	  				break;
	  			case 0x2f://Verify(10)
					pCSW->dCSWSig = USB_CSW_SIG;
					pCSW->dCSWDataRes = 0;
					pCSW->bCSWStatus = 0;
					pusb->device.bo_xfer_addr = (__u32)pCSW;
					pusb->device.bo_xfer_residue = USB_CSW_LEN;
					pusb->device.bo_xfer_tranferred = 0;
					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
					 	pusb->device.bo_state = USB_BO_CSW;
					}
					else
					{
						pusb->device.bo_state = USB_BO_CBW;
						__inf("Error: CSW Send Error!!\n");
					}
	  				break;
	  			case 0x12://Inquiry
	  				pusb->device.bo_xfer_addr = (__u32)InquiryData;
	  				pusb->device.bo_xfer_residue = min(pCBW->dCBWDTL, 36);
	  				pusb->device.bo_xfer_tranferred = 0;
	  				fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
						pusb->device.bo_state = USB_BO_TXDATA;
					}
					else
					{
						pusb->device.bo_state = USB_BO_CBW;
						__inf("Error: Data Send Error!!\n");
					}
	  				break;
	  			case 0x23://Read Format Capacity
					{
						__u8 formatcap[12];
						__u32 sec_cnt[2];

						formatcap[0] = 0x00;
						formatcap[1] = 0x00;
						formatcap[2] = 0x00;
						formatcap[3] = 0x08;
						formatcap[8] = 0x02;
						formatcap[9] = 0x00;
						formatcap[10] = 0x02;
						formatcap[11] = 0x00;

						//sec_cnt[0] = (pusb->device.bo_seccnt+1);
						//sec_cnt[0] = (262144);
						sec_cnt[0] = wBoot_part_capacity(pCBW->bCBWLUN);
						//__inf("part index = %d, format capacity = %d\n", pCBW->bCBWLUN, sec_cnt[0]);
						formatcap[4] = sec_cnt[0]>>24;
						formatcap[5] = sec_cnt[0]>>16;
						formatcap[6] = sec_cnt[0]>>8;
						formatcap[7] = sec_cnt[0];

						pusb->device.bo_xfer_addr = (__u32)formatcap;
						if(!usb_work_type)
						{
							pusb->device.bo_xfer_residue = min(pCBW->dCBWDTL, 12);
						}
						else
						{
							pusb->device.csw_fail_flag = 1;
							pusb->device.bo_xfer_residue = 0;
						}
						pusb->device.bo_xfer_tranferred = 0;
						fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
						if(fret == USB_RETVAL_NOTCOMP)
						{
							pusb->device.bo_state = USB_BO_TXDATA;
						}
						else
						{
							pusb->device.bo_state = USB_BO_CBW;
							__inf("Error: Data Send Error!!\n");
						}
					}
	  				break;
	  			case 0x25://Read Capacity
					{
						__u8 capacity[8];
						__u32 sec_cnt[2];

						capacity[4] = 0x00;
						capacity[5] = 0x00;
						capacity[6] = 0x02;
						capacity[7] = 0x00;

						//sec_cnt[0] = (pusb->device.bo_seccnt);
						//sec_cnt[0] = (262144);
						sec_cnt[0] = wBoot_part_capacity(pCBW->bCBWLUN) - 1;
						//__inf("part index = %d, capacity = %d\n", pCBW->bCBWLUN, sec_cnt[0]);

						capacity[0] = sec_cnt[0]>>24;
						capacity[1] = sec_cnt[0]>>16;
						capacity[2] = sec_cnt[0]>>8;
						capacity[3] = sec_cnt[0];

						pusb->device.bo_xfer_addr = (__u32)capacity;
						if(!usb_work_type)
						{
							pusb->device.bo_xfer_residue = min(pCBW->dCBWDTL, 8);
						}
						else
						{
							pusb->device.csw_fail_flag = 1;
							pusb->device.bo_xfer_residue = 0;
						}
						pusb->device.bo_xfer_tranferred = 0;
						fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
						if(fret == USB_RETVAL_NOTCOMP)
						{
							pusb->device.bo_state = USB_BO_TXDATA;
						}
						else
						{
							pusb->device.bo_state = USB_BO_CBW;
							__inf("Error: Data Send Error!!\n");
						}
					}
	  				break;
	  			case 0x28://Read(10)  write to host
					{
						__u32 read_len = 0;
						__u32 read_offset = 0;
						__s32 part_start;

						read_len = pCBW->CBWCB[7];
						read_len <<= 8;
						read_len |= pCBW->CBWCB[8];      //这里 read_len 是扇区数
						//read_len <<= USB_DEV_SEC_BITS;   //这里 read_len 是字节数

						read_offset = pCBW->CBWCB[2];
						read_offset <<= 8;
						read_offset |= pCBW->CBWCB[3];
						read_offset <<= 8;
						read_offset |= pCBW->CBWCB[4];
						read_offset <<= 8;
						read_offset |= pCBW->CBWCB[5];		//这里 read_offset 是偏移的扇区
						//read_offset <<= USB_DEV_SEC_BITS;  //From Blocks to Bytes  //这里 read_offset 是偏移的字节数
						pusb->device.bo_xfer_addr = pusb->device.bo_memory_base;

						//读出flash数据
						if(!usb_work_type)
						{
							part_start = wBoot_part_start(pCBW->bCBWLUN);
							//__inf("part index = %d, start = %d\n", pCBW->bCBWLUN, part_start);
							if(wBoot_block_read(read_offset + part_start, read_len, (void *)pusb->device.bo_memory_base) < 0)
							{
								__inf("part index = %d, start = %d, read_start = %d, len = %d\n", pCBW->bCBWLUN, part_start, read_offset + part_start, read_len);
								pCSW->dCSWSig = USB_CSW_SIG;
								pCSW->dCSWDataRes = pCBW->dCBWDTL;
								pCSW->bCSWStatus = 1;
								pusb->device.bo_xfer_addr = (__u32)pCSW;
								pusb->device.bo_xfer_residue = USB_CSW_LEN;
								pusb->device.bo_xfer_tranferred = 0;
								epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
								pusb->device.bo_state = USB_BO_CSW;

								__inf("Error: Flash Read Fail\n");
								break;
							}
							read_len <<= USB_DEV_SEC_BITS;   //这里 read_len 是字节数
							read_offset <<= USB_DEV_SEC_BITS;  //From Blocks to Bytes  //这里 read_offset 是偏移的字节数

							pusb->device.bo_xfer_residue = min(read_len, MAX_DDMA_SIZE); //Max USB Packet is 64KB    //??

						}
						else
						{
							pusb->device.csw_fail_flag = 1;
							pusb->device.bo_xfer_residue = 0;
						}
						pusb->device.bo_xfer_tranferred = 0;
					}
					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
						pusb->device.bo_state = USB_BO_TXDATA;
					}
					else
					{
						pusb->device.bo_state = USB_BO_CBW;
						__inf("Error: Data Send Error!!\n");
					}
	  				break;
	  			case 0x1A://Mode Sense(6)
					pusb->device.bo_xfer_addr = (__u32)SenseData;
					pusb->device.bo_xfer_residue = min(pCBW->dCBWDTL, 4);
					pusb->device.bo_xfer_tranferred = 0;
					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
					 	pusb->device.bo_state = USB_BO_TXDATA;
					}
					else
					{
						pusb->device.bo_state = USB_BO_CBW;
						__inf("Error: Data Send Error!!\n");
					}
	  				break;
	  		    case 0x03:
					pusb->device.bo_xfer_addr = (__u32)RequestSense;
					pusb->device.bo_xfer_residue = min(pCBW->dCBWDTL, 18);
					pusb->device.bo_xfer_tranferred = 0;
					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					if(fret == USB_RETVAL_NOTCOMP)
					{
					 	pusb->device.bo_state = USB_BO_TXDATA;
					}
					else
					{
						pusb->device.bo_state = USB_BO_CBW;
						__inf("Error: Data Send Error!!\n");
					}
	  				break;
	  			case 0x2A://Write(10)   read from host
	  				{
						write_len = pCBW->CBWCB[7];
						write_len <<= 8;
						write_len |= pCBW->CBWCB[8];			//这里 write_len 是扇区数
						write_len <<= USB_DEV_SEC_BITS;         //这里 write_len 是字节数

						write_offset = pCBW->CBWCB[2];
						write_offset <<= 8;
						write_offset |= pCBW->CBWCB[3];
						write_offset <<= 8;
						write_offset |= pCBW->CBWCB[4];
						write_offset <<= 8;
						write_offset |= pCBW->CBWCB[5];			//这里 write_offset 是偏移的扇区
						write_offset <<= USB_DEV_SEC_BITS;		//这里 write_offset 是偏移的字节数

						pusb->device.bo_xfer_addr = pusb->device.bo_memory_base;

						pusb->device.bo_xfer_residue = min(write_len, MAX_DDMA_SIZE);
						pusb->device.bo_xfer_tranferred = 0;
	  				}
	  				fret = epx_out_handler_dev(pusb, pusb->device.bo_ep_out, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
	  				if(fret==USB_RETVAL_COMPOK)
					{
						__s32 flash_ret, start, nsector;
						//write to flash
						start = (write_offset>>9) + wBoot_part_start(pCBW->bCBWLUN);
						nsector = write_len>>USB_DEV_SEC_BITS;
						flash_ret = wBoot_block_write(start, nsector, (void *)pusb->device.bo_memory_base);
						__inf("part index = %d, start = %d\n", pCBW->bCBWLUN, start);
						if(flash_ret < 0)
						{
							__inf("flash write start %d sector %d failed\n", start, nsector);
							pCSW->dCSWSig = USB_CSW_SIG;
							pCSW->dCSWDataRes = pCBW->dCBWDTL;
							pCSW->bCSWStatus = 1;
							pusb->device.bo_xfer_addr = (__u32)pCSW;
							pusb->device.bo_xfer_residue = USB_CSW_LEN;
							pusb->device.bo_xfer_tranferred = 0;
							epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
							pusb->device.bo_state = USB_BO_CSW;

							__inf("Error: Flash Write Fail\n");
							break;
						}
						pusb->device.bo_xfer_tranferred = pusb->device.bo_xfer_residue;
						pusb->device.bo_xfer_residue = 0;
						pCSW->dCSWSig = USB_CSW_SIG;
						pCSW->dCSWDataRes = pCBW->dCBWDTL - pusb->device.bo_xfer_tranferred;
						pCSW->bCSWStatus = 0;
						pusb->device.bo_xfer_addr = (__u32)pCSW;
						pusb->device.bo_xfer_residue = USB_CSW_LEN;
						pusb->device.bo_xfer_tranferred = 0;
						fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
						if(fret == USB_RETVAL_NOTCOMP)
						{
						 	pusb->device.bo_state = USB_BO_CSW;
						}
						else
						{
							pusb->device.bo_state = USB_BO_CBW;
							__inf("Error: CSW Send Error!!\n");
						}
					}
	  				else
  					{
						if(fret == USB_RETVAL_NOTCOMP)
						{
							pusb->device.bo_state = USB_BO_RXDATA;
						}
						else
						{
							pusb->device.bo_state = USB_BO_CBW;
							__inf("Error: Rx Data Error!!\n");
						}
  					}
	  				break;
	  			case 0xf3:
	  			{
	  				if(!usb_work_type)
	  				{
	  					break;
	  				}
	  				__inf("usb handshake\n");
	  				__inf("usb command = %d\n", pCBW->CBWCB[1]);
	  				switch(pCBW->CBWCB[1])
	  				{
	  					case 0:				//握手
	  					{
	  						__usb_handshake_t  handshake;

	  						__inf("usb command = 0\n");
	  						memset(&handshake, 0, sizeof(__usb_handshake_t));
							strcpy(handshake.magic, "usbhandshake");
							handshake.sizelo = (part_info.len >> 0 ) & 0xffffffff;
							handshake.sizehi = (part_info.len >> 32) & 0xffffffff;
							__inf("part sectors low = %d, high = %d\n", handshake.sizelo, handshake.sizehi);
		  					pusb->device.bo_xfer_residue = sizeof(__usb_handshake_t);
		  					memcpy((char *)pusb->device.bo_memory_base, &handshake, sizeof(__usb_handshake_t));
		  					pusb->device.bo_xfer_addr = pusb->device.bo_memory_base;
		  					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
							if(fret == USB_RETVAL_NOTCOMP)
							{
								pusb->device.bo_state = USB_BO_TXDATA;
							}
							else
							{
								pusb->device.bo_state = USB_BO_CBW;
								__inf("Error: Data Send Error!!\n");
							}
							write_start = part_info.addr;
						}
						break;

						case 1:				//接收数据
						{
							int   start, length;
							__s32 flash_ret, nsector;

							__inf("usb command = 1\n");
							if(!write_start)
							{
								pusb->device.bo_state = USB_BO_CBW;
								__inf("Error: CSW Send Error!!\n");

								break;
							}
							start     = write_start;
							write_len = (*(int *)(pCBW->CBWCB + 8))<<9;
							length    = write_len;
		  					pusb->device.bo_xfer_addr = pusb->device.bo_memory_base;
		  					pusb->device.bo_xfer_residue = length;
							__inf("write size = %d\n", length);
							fret = epx_out_handler_dev(pusb, pusb->device.bo_ep_out, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
							if(fret == USB_RETVAL_COMPOK)
							{
								//回应状态
								nsector = length/512;
								__msg("write start = %d, count = %d\n", start, nsector);
								flash_ret = wBoot_block_write(start, nsector, (void *)pusb->device.bo_memory_base);
								write_start += nsector;
								if(flash_ret < 0)
								{
									__inf("flash write start %d sector %d failed\n", start, nsector);
									pCSW->dCSWSig = USB_CSW_SIG;
									pCSW->dCSWDataRes = pCBW->dCBWDTL;
									pCSW->bCSWStatus = 1;
									pusb->device.bo_xfer_addr = (__u32)pCSW;
									pusb->device.bo_xfer_residue = USB_CSW_LEN;
									pusb->device.bo_xfer_tranferred = 0;
									epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
									pusb->device.bo_state = USB_BO_CSW;

									__inf("Error: Flash Write Fail\n");
									break;
								}
								pusb->device.bo_xfer_tranferred = pusb->device.bo_xfer_residue;
								pusb->device.bo_xfer_residue = 0;
								pCSW->dCSWSig = USB_CSW_SIG;
								pCSW->dCSWDataRes = pCBW->dCBWDTL - pusb->device.bo_xfer_tranferred;
								pCSW->bCSWStatus = 0;
								pusb->device.bo_xfer_addr = (__u32)pCSW;
								pusb->device.bo_xfer_residue = USB_CSW_LEN;
								pusb->device.bo_xfer_tranferred = 0;
								fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
								if(fret == USB_RETVAL_NOTCOMP)
								{
								 	pusb->device.bo_state = USB_BO_CSW;
								}
								else
								{
									pusb->device.bo_state = USB_BO_CBW;
									__inf("Error: CSW Send Error!!\n");
								}
							}
							else
		  					{
								if(fret == USB_RETVAL_NOTCOMP)
								{
									pusb->device.bo_state = USB_BO_RXDATA;
								}
								else
								{
									pusb->device.bo_state = USB_BO_CBW;
									__inf("Error: Rx Data Error!!\n");
								}
		  					}
						}
						break;

						case 2:				//关闭usb
						{
		  					__usb_handshake_t  handshake;

	  						__inf("usb command = 2\n");
	  						memset(&handshake, 0, sizeof(__usb_handshake_t));
							strcpy(handshake.magic, "usb_dataok");
							handshake.sizelo = 0;
							handshake.sizehi = 0;
							pusb->device.bo_xfer_residue = sizeof(__usb_handshake_t);
		  					memcpy((char *)pusb->device.bo_memory_base, &handshake, sizeof(__usb_handshake_t));
		  					pusb->device.bo_xfer_addr = pusb->device.bo_memory_base;

		  					fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
							if(fret == USB_RETVAL_NOTCOMP)
							{
								pusb->device.bo_state = USB_BO_TXDATA;
								usb_status = 1;
							}
							else
							{
								pusb->device.bo_state = USB_BO_CBW;
								__inf("Error: Data Send Error!!\n");
							}
						}
						break;

						default:
						break;
	  				}
	  			}
	  			break;

	  			default:
	  				break;
	  		}
			break;

		case USB_BO_RXDATA:
			fret = epx_out_handler_dev(pusb, pusb->device.bo_ep_out, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
			if(fret==USB_RETVAL_COMPOK)
			{
				__s32 flash_ret, start, nsector;
				//write to flash
				if(!usb_work_type)
				{
					start = (write_offset>>9) + wBoot_part_start(pCBW->bCBWLUN);
				}
				else
				{
					start = write_start;
				}
				nsector = write_len>>USB_DEV_SEC_BITS;
				__msg("write start = %d, count = %d\n", start, nsector);
				if(usb_work_type)
				{
					write_start += nsector;
				}
				flash_ret = wBoot_block_write(start, nsector, (void *)pusb->device.bo_memory_base);
				if(flash_ret < 0)
				{
					__inf("flash write start %d sector %d failed\n", start, nsector);
					pCSW->dCSWSig = USB_CSW_SIG;
					pCSW->dCSWDataRes = pCBW->dCBWDTL;
					pCSW->bCSWStatus = 1;
					pusb->device.bo_xfer_addr = (__u32)pCSW;
					pusb->device.bo_xfer_residue = USB_CSW_LEN;
					pusb->device.bo_xfer_tranferred = 0;
					epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
					pusb->device.bo_state = USB_BO_CSW;

					__inf("Error: Flash Write Fail\n");
					break;
				}


				pusb->device.bo_xfer_tranferred = pusb->device.bo_xfer_residue;
				pusb->device.bo_xfer_residue = 0;
				pCSW->dCSWSig = USB_CSW_SIG;
				pCSW->dCSWDataRes = pCBW->dCBWDTL - pusb->device.bo_xfer_tranferred;
				pCSW->bCSWStatus = 0;
				if((usb_work_type == 1) && (pusb->device.csw_fail_flag == 1))
				{
					pCSW->bCSWStatus = 1;
				}
				pusb->device.bo_xfer_addr = (__u32)pCSW;
				pusb->device.bo_xfer_residue = USB_CSW_LEN;
				pusb->device.bo_xfer_tranferred = 0;
				epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
				pusb->device.bo_state = USB_BO_CSW;
			}
			else if(fret == USB_RETVAL_COMPERR)
			{
				__inf("Error: RxData Error\n");
				pusb->device.bo_state = USB_BO_CBW;
			}
			break;

		case USB_BO_TXDATA:
			fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
			if(fret==USB_RETVAL_COMPOK)
			{
				pusb->device.bo_xfer_tranferred = pusb->device.bo_xfer_residue;
				pusb->device.bo_xfer_residue = 0;
				pCSW->dCSWSig = USB_CSW_SIG;
				pCSW->dCSWDataRes = pCBW->dCBWDTL - pusb->device.bo_xfer_tranferred;
				pCSW->bCSWStatus = 0;
				if((usb_work_type == 1) && (pusb->device.csw_fail_flag == 1))
				{
					pCSW->bCSWStatus = 1;
				}
				pusb->device.bo_xfer_addr = (__u32)pCSW;
				pusb->device.bo_xfer_residue = USB_CSW_LEN;
				pusb->device.bo_xfer_tranferred = 0;
				epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
				pusb->device.bo_state = USB_BO_CSW;
			}
			else if(fret == USB_RETVAL_COMPERR)
			{
				__inf("Error: TxData Error\n");
				pusb->device.bo_state = USB_BO_CBW;
			}
			break;

		case USB_BO_CSW:
			fret = epx_in_handler_dev(pusb, pusb->device.bo_ep_in, pusb->device.bo_xfer_addr, pusb->device.bo_xfer_residue, USB_PRTCL_BULK);
			if(fret==USB_RETVAL_COMPOK)
			{
				pusb->device.bo_xfer_tranferred = pusb->device.bo_xfer_residue;
				pusb->device.bo_xfer_residue = 0; //min(pCBW->dCBWDTL, 36);
				pusb->device.bo_state = USB_BO_CBW;
				if(usb_work_type == 1)
				{
					pusb->device.csw_fail_flag = 0;
					if(usb_status == 1)
					{
						__msg("1\n");
						usb_status = 2;
					}
				}
			}
			else if(fret==USB_RETVAL_COMPERR)
			{
				pusb->device.bo_state = USB_BO_CBW;
			  	__inf("Error: Tx CSW Error!!\n");
			}
			break;

		default:
			break;
	}
	usb_select_ep(pusb, ep_save);

	return ret;
}
///////////////////////////////////////////////////////////////////
//                 usb control transfer
///////////////////////////////////////////////////////////////////

/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static __u32 ep0_set_config_handler_dev(pusb_struct pusb)
{
	pSetupPKG ep0_setup = (pSetupPKG)(pusb->buffer);
	uConfigDes *pconfig;
	uIntfDes *pintf;
	__u32 i;
	__u32 fifo_addr = 1024;    //

	pconfig = (uConfigDes *)(pusb->device.config_desc);
	if(pconfig->bConfigVal != ep0_setup->wValue)
	{
		__inf("Error: Right Configval %d; Error Configval %d\n", pconfig->bConfigVal, ep0_setup->wValue);
		return 0;
	}
	pintf = (uIntfDes *)(&pusb->device.config_desc[USB_CONFIG_DESC_LEN]);
	for(i=0; i<pintf->bNumEP; i++)
	{
		uEPDes *pep;
		__u32 ep_no;
		__u32 ep_attr;
		__u32 maxpktsz;
		//__u32 interval;
		__u32 ep_dir;

		pep = (uEPDes*)(&pusb->device.config_desc[USB_CONFIG_DESC_LEN + USB_INTF_DESC_LEN + USB_ENDP_DESC_LEN*i]);
		ep_no = pep->bEPAddr & 0xf;
		ep_dir = (pep->bEPAddr>>7) & 0x1; //0 for OUT, and 1 for IN
		ep_attr = pep->bmAttrib & 0x3;
		maxpktsz = pep->wMaxPktSize1 & 0x7;
		maxpktsz <<= 8;
		maxpktsz |= pep->wMaxPktSize0 & 0xff;

		usb_select_ep(pusb, ep_no);
		if(ep_dir)
		{
			usb_set_eptx_maxpkt(pusb, maxpktsz, USB_EP_FIFO_SIZE/maxpktsz);
			usb_set_eptx_fifo_addr(pusb, fifo_addr);
			usb_set_eptx_fifo_size(pusb, 1, USB_EP_FIFO_SIZE);
			pusb->device.eptx_prtcl[ep_no-1] = ep_attr;
			pusb->device.eptx_fifo[ep_no-1] = (fifo_addr<<16)|(0x1<<15)|(maxpktsz&0x7fff);
			fifo_addr += ((USB_EP_FIFO_SIZE<<1)+(USB_FIFO_ADDR_BLOCK-1))&(~(USB_FIFO_ADDR_BLOCK-1));  //Align to USB_FIFO_ADDR_BLOCK
			if(pintf->bIntfProtocol == 0x50)  //Bulk Only Device
			{
				pusb->device.bo_ep_in = ep_no;
			}
			usb_eptx_flush_fifo(pusb);
			usb_eptx_flush_fifo(pusb);
		}
		else
		{
			usb_set_eprx_maxpkt(pusb, maxpktsz, USB_EP_FIFO_SIZE/maxpktsz);
			usb_set_eprx_fifo_addr(pusb, fifo_addr);
			usb_set_eprx_fifo_size(pusb, 1, USB_EP_FIFO_SIZE);
			pusb->device.eprx_prtcl[ep_no-1] = ep_attr;
			pusb->device.eprx_fifo[ep_no-1] = (fifo_addr<<16)|(0x1<<15)|(maxpktsz&0x7fff);
			fifo_addr += ((USB_EP_FIFO_SIZE<<1)+(USB_FIFO_ADDR_BLOCK-1))&(~(USB_FIFO_ADDR_BLOCK-1));
			if(pintf->bIntfProtocol == 0x50)  //Bulk Only Device
			{
				pusb->device.bo_ep_out = ep_no;
			}
			usb_eprx_flush_fifo(pusb);
			usb_eprx_flush_fifo(pusb);
		}
	}

	return 1;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static __s32 ep0_in_handler_dev(pusb_struct pusb)
{
	__u32 temp = 0;
	pSetupPKG ep0_setup = (pSetupPKG)(pusb->buffer);

	if((ep0_setup->bmRequest&0x60)==0x00)
	{
		switch(ep0_setup->bRequest)
		{
			case 0x00 :
    			__inf("usb_device: Get Status\n");
			case 0x06 :
				switch((ep0_setup->wValue>>8)&0xff)
				{
					case 0x01:              //Get Device Desc
						pusb->ep0_maxpktsz = *((__u8*)(pusb->device.dev_desc+7));
						pusb->ep0_xfer_srcaddr = (__u32)pusb->device.dev_desc;
						pusb->ep0_xfer_residue = min(*((__u8*)pusb->ep0_xfer_srcaddr), ep0_setup->wLength);
						break;
					case 0x02:              //Get Configuration Desc
						temp = pusb->device.config_desc[3] & 0xff;
						temp = temp << 8;
						temp |= pusb->device.config_desc[2] & 0xff;
						pusb->ep0_xfer_srcaddr = (__u32)pusb->device.config_desc;
						pusb->ep0_xfer_residue = min(temp, ep0_setup->wLength);
						break;
					case 0x03:             //Get String Desc
					   	temp = ep0_setup->wValue&0xff;
					   	if(temp < 4)
						{
							pusb->ep0_xfer_srcaddr = (__u32)pusb->device.str_desc[temp];
							pusb->ep0_xfer_residue = min(*((__u8*)pusb->ep0_xfer_srcaddr), ep0_setup->wLength);
						}
						else
						{
							__inf("Unkown String Desc!!\n");
						}
						break;
					case 0x04:           //Get Interface Desc
					    pusb->ep0_xfer_residue = 0;
					    __inf("usb_device: Get Interface Descriptor\n");
				    	break;
					case 0x05:          //Get Endpoint Desc
					    pusb->ep0_xfer_residue = 0;
					    __inf("usb_device: Get Endpoint Descriptor\n");
				    	break;
					case 0x06:           //Get Device Qualifier
					    pusb->ep0_xfer_srcaddr = (__u32)pusb->device.dev_qual;
					    pusb->ep0_xfer_residue = min(*((__u8*)pusb->ep0_xfer_srcaddr), ep0_setup->wLength);
				    	break;
					case 0x09:
					    pusb->ep0_xfer_srcaddr = (__u32)pusb->device.otg_desc;
					    pusb->ep0_xfer_residue = min(*((__u8*)pusb->ep0_xfer_srcaddr), ep0_setup->wLength);
				    	break;
					default  :
					    pusb->ep0_xfer_residue = 0;
					    __inf("usb_device: Get Unkown Descriptor\n");
				}
		      	break;
	     	case 0x08 :
				pusb->ep0_xfer_residue = 0;
				__inf("usb_device: Get Configuration\n");
				break;
			case 0x0A :
				pusb->ep0_xfer_residue = 0;
				__inf("usb_device: Get Interface\n");
				break;
			case 0x0C :
				pusb->ep0_xfer_residue = 0;
				__inf("usb_device: Sync Frame\n");
				break;
	      	default   :
	        	pusb->ep0_xfer_residue = 0;
	        	__inf("usb_device: Unkown Standard Request = 0x%x\n", ep0_setup->bRequest);
		}
	}
	else if((ep0_setup->bmRequest&0x60)==0x20)
	{
		switch(ep0_setup->bRequest)
		{
			case 0x00FE :
				pusb->ep0_xfer_srcaddr = (__u32)&(pusb->device.MaxLUN);
				pusb->ep0_xfer_residue = 1;
				__inf("usb_device: Get MaxLUN\n");
				break;
			default     :
				pusb->ep0_xfer_residue = 0;
				__inf("usb_device: Unkown Class-Specific Request = 0x%x\n", ep0_setup->bRequest);
		}
	}
	else
	{
		pusb->ep0_xfer_residue = 0;
		__inf("usb_device: Unkown EP0 IN!!\n");
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static __s32 ep0_out_handler_dev(pusb_struct pusb)
{
	pSetupPKG ep0_setup = (pSetupPKG)(pusb->buffer);

	switch(ep0_setup->bRequest)
	{
		case 0x01 :
			if(ep0_setup->wIndex&0x80)
			{

			}
			else
			{

			}
		  	break;

		case 0x03 :
			 switch(ep0_setup->wValue)
			 {
			 	case 0x0002:
	          		switch(ep0_setup->wIndex)
	          		{
	           			case 0x0100:
	             			usb_set_test_mode(pusb, 0x02);
	             			__inf("usb_device: Send Test J Now...\n");
	            			break;
						case 0x0200:
							usb_set_test_mode(pusb, 0x04);
							__inf("usb_device: Send Test K Now...\n");
							break;
						case 0x0300:
							usb_set_test_mode(pusb, 0x01);
							__inf("usb_device: Test SE0_NAK Now...\n");
							break;
						case 0x0400:
							usb_write_ep_fifo(pusb, 0, (__u32)TestPkt, 53);
							usb_set_ep0_csr(pusb, 0x02);
							usb_set_test_mode(pusb, 0x08);

	          				__inf("usb_device: Send Test Packet Now...\n");
	         				break;
				        	default:
				          	__inf("usb_device: Unkown Test Mode: 0x%x\n", ep0_setup->wIndex);
				    }
	        		break;

				case 0x0003:
				case 0x0004:
				case 0x0005:
					__inf("usb_device: HNP Enable...\n");
					break;

				default:
				__inf("usb_device: Unkown SetFeature Value: 0x%x\n", ep0_setup->wValue);
			}
			break;

		case 0x05 :
			usb_set_dev_addr(pusb, ep0_setup->wValue);
       		pusb->device.func_addr = ep0_setup->wValue;
       		__inf("usb_device: Set Address 0x%x\n", ep0_setup->wValue);
			break;

		case 0x07 :
       		__inf("usb_device: Set Descriptor\n");
      		break;
    	case 0x09 :
      		ep0_set_config_handler_dev(pusb);
    		break;
    	case 0x0B :
	       	__inf("usb_device: Set Interface\n");
	      	break;
    	default   :
      		__inf("usb_device: Unkown EP0 OUT: 0x%x!!\n", ep0_setup->bRequest);
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 usb_dev_ep0xfer(pusb_struct pusb)
{
	//__u32 i=0;
	__u32 temp=0;
	pSetupPKG ep0_setup = (pSetupPKG)(pusb->buffer);
	__u32 is_last;
	__u32 byte_trans;
	//__u32 src_addr;

	if(pusb->role != USB_ROLE_DEV) return 0;
	if(!pusb->ep0_flag) return 0;
	pusb->ep0_flag--;

	usb_select_ep(pusb, 0);
  	temp = usb_get_ep0_csr(pusb);

	if(pusb->ep0_xfer_state == USB_EP0_DATA)  //Control IN Data Stage or Stage Status
	{
		if(temp&0x1)
		{
			pusb->ep0_xfer_state = USB_EP0_SETUP;
		}
		else if(temp&(0x1<<4))
		{
			usb_set_ep0_csr(pusb, 0x80);
			__inf("WRN: EP0 Setup End!!\n");
		}
		else if(!(temp&(0x1<<1)))
		{
			if(pusb->ep0_xfer_residue)
		 	{
		 		pusb->ep0_xfer_srcaddr += pusb->ep0_maxpktsz;
		 		if(pusb->ep0_xfer_residue == 0xffffffff)
			  	{
				  	is_last = 1;
				  	byte_trans = 0;
				  	pusb->ep0_xfer_residue = 0;
			   	}
			  	else
			   	{
			   		if(pusb->ep0_xfer_residue < pusb->ep0_maxpktsz)
			    	{
						is_last = 1;
						byte_trans = pusb->ep0_xfer_residue;
						pusb->ep0_xfer_residue = 0;
			    	}
			   		else if(pusb->ep0_xfer_residue == pusb->ep0_maxpktsz)
					{
						is_last = 0;
						byte_trans = pusb->ep0_xfer_residue;
						pusb->ep0_xfer_residue = 0xffffffff;  //Send a zero packet next time
					}
					else
					{
						is_last = 0;
						byte_trans = pusb->ep0_maxpktsz;
						pusb->ep0_xfer_residue -= pusb->ep0_maxpktsz;
			   		}
			 	}
			 	usb_write_ep_fifo(pusb, 0, pusb->ep0_xfer_srcaddr, byte_trans);
			 	if(is_last || (!byte_trans))
			 	{
			 		usb_set_ep0_csr(pusb, 0x0a);
			 	}
			 	else
			 	{
			 		usb_set_ep0_csr(pusb, 0x02);
				}

			 	if(usb_get_ep0_count(pusb))
			 	{
		  			__inf("Error: COUNT0 = 0x%x\n", usb_get_ep0_count(pusb));
		 		}
		 	}
		}
		else
		{
			__inf("WRN: Unkown EP0 Interrupt, CSR=0x%x!!\n", temp);
		}
	}
	if(pusb->ep0_xfer_state == USB_EP0_SETUP)  //Setup or Control OUT Status Stage
	{
		if(temp&0x1)
		{
			__u32 ep0_count = usb_get_ep0_count(pusb);

			if(ep0_count==8)
			{
				pusb->ep0_flag = 0;
				usb_read_ep_fifo(pusb, 0, (__u32)pusb->buffer, 8);

				if(ep0_setup->bmRequest&0x80)//in
				{
					usb_set_ep0_csr(pusb, 0x40);
					ep0_in_handler_dev(pusb);

					if(pusb->ep0_xfer_residue<pusb->ep0_maxpktsz)
					{
						is_last = 1;
						byte_trans = pusb->ep0_xfer_residue;
						pusb->ep0_xfer_residue = 0;
					}
				 	else if(pusb->ep0_xfer_residue==pusb->ep0_maxpktsz)
					{
						is_last = 0;
						byte_trans = pusb->ep0_xfer_residue;
						pusb->ep0_xfer_residue = 0xffffffff;
					}
					else
					{
						is_last = 0;
						byte_trans = pusb->ep0_maxpktsz;
						pusb->ep0_xfer_residue -= pusb->ep0_maxpktsz;
					}

				 	usb_write_ep_fifo(pusb, 0, pusb->ep0_xfer_srcaddr, byte_trans);
				 	if(is_last || (!byte_trans))
				 	{
				 		usb_set_ep0_csr(pusb, 0x0a);
				   	}
				   	else
				   	{
				   		usb_set_ep0_csr(pusb, 0x02);
				   	}

				   	pusb->ep0_xfer_state = USB_EP0_DATA;
				}
				else                         //out
				{
					usb_set_ep0_csr(pusb, 0x48);
					pusb->ep0_xfer_state = USB_EP0_SETUP;
				}
			}
			else
			{
			  	usb_ep0_flush_fifo(pusb);
		    	__inf("Error: EP0 Rx Error Length = 0x%x\n", ep0_count);
			}
		}
		else
		{
			ep0_out_handler_dev(pusb);
		}
	}

	return 1;
}
///////////////////////////////////////////////////////////////////
//                usb storage function
///////////////////////////////////////////////////////////////////
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 usb_bus_irq_handler_dev(pusb_struct pusb)
{
	__u32 busirq_en;
	__u32 i;
	__u32 ep_save = usb_get_active_ep(pusb);

	if(pusb->role != USB_ROLE_DEV) return 0;
  	if(!pusb->busirq_flag) return 0;
  	usb_select_ep(pusb, 0);
  	pusb->busirq_flag--;
  	busirq_en = usb_get_bus_interrupt_enable(pusb);

  	if(pusb->busirq_status & busirq_en & USB_BUSINT_SUSPEND)
  	{
	  	pusb->busirq_status &= ~USB_BUSINT_SUSPEND;
	  	//Suspend Service Subroutine
	  	pusb->suspend = 1;

		if(wBoot_dma_QueryState(pusb->dma))
		{
			__inf("Error: DMA for EP is not finished after Bus Suspend\n");
		}
		wBoot_dma_stop(pusb->dma);
	  	__inf("uSuspend\n");
  	}

	if(pusb->busirq_status & busirq_en & USB_BUSINT_RESUME)
	{
		pusb->busirq_status &= ~USB_BUSINT_RESUME;
		//Resume Service Subroutine
		pusb->suspend = 0;
		__inf("uResume\n");
	}

	if(pusb->busirq_status & busirq_en & USB_BUSINT_RESET)
	{
		pusb->busirq_status &= ~USB_BUSINT_RESET;
		//Device Reset Service Subroutine
		pusb->connect = 1;
		pusb->reset = 1;
		pusb->suspend = 0;
		pusb->rst_cnt ++;
		for(i=0; i<USB_MAX_EP_NO; i++)
		{
			pusb->eptx_flag[i] = 0;
			pusb->eprx_flag[i] = 0;
			pusb->eptx_xfer_state[i] = USB_EPX_SETUP;
			pusb->eprx_xfer_state[i] = USB_EPX_SETUP;
		}
		pusb->device.func_addr = 0x00;
		pusb->device.bo_state = USB_BO_IDLE;
		pusb->timer = USB_IDLE_TIMER;

		//Reset Function Address
		usb_set_dev_addr(pusb, 0x00);
		//Bus Reset may disable all interrupt enable, re-enable the interrupts need
		usb_set_bus_interrupt_enable(pusb, 0xf7);
		usb_set_eptx_interrupt_enable(pusb, 0xffff);
		usb_set_eprx_interrupt_enable(pusb, 0xfffe);

		if(wBoot_dma_QueryState(pusb->dma))
		{
			__inf("Error: DMA for EP is not finished after Bus Reset\n");
		}
		wBoot_dma_stop(pusb->dma);
	  	__inf("uSuspend\n");
	}

  	if(pusb->busirq_status & busirq_en & USB_BUSINT_SESSEND)
  	{
  		pusb->busirq_status &= ~USB_BUSINT_SESSEND;
  		//Device Reset Service Subroutine
  		pusb->connect = 0;
		pusb->reset = 0;
		pusb->suspend = 1;
		__inf("uSessend\n");
  	}

  	usb_select_ep(pusb, ep_save);

  	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void usb_power_polling_dev(pusb_struct pusb)
{
	if(pusb->role != USB_ROLE_DEV) return;
  	if(pusb->connect) return;

  	if(usb_get_vbus_level(pusb) == USB_VBUS_VBUSVLD)
  	{
		if(pusb->timer != USB_DEVICE_VBUS_DET_TIMER)
		{
			if(pusb->timer != USB_IDLE_TIMER)  //timer should not occupied by any other ones at this time
			{
				__inf("Error: Timer Ocuppied\n");
			}
		    pusb->timer = USB_DEVICE_VBUS_DET_TIMER;

		   	pusb->loop = 0;
		   	pusb->power_debouce = 0x50;

			return;
		}
		else if(pusb->power_debouce > 0)
		{
			pusb->loop ++;
		   	pusb->power_debouce --;

			return;
		}
	}
  	else
  	{
    	pusb->timer = USB_IDLE_TIMER;  //Release Timer and return
  		pusb->loop = 0;
  		return;
  	}

	pusb->loop ++;
	pusb->timer = USB_IDLE_TIMER;
	pusb->loop = 0;
	usb_soft_connect(pusb);
	pusb->connect = 1;
	__inf("USB Connect!!\n");

	return;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 usb_device_function(pusb_struct pusb)
{
	if(pusb->role != USB_ROLE_DEV)
	{
		return 0;
	}

	usb_power_polling_dev(pusb);
	usb_bus_irq_handler_dev(pusb);
	usb_dev_ep0xfer(pusb);

	usb_dev_bulk_xfer(pusb);

	return 1;
}

























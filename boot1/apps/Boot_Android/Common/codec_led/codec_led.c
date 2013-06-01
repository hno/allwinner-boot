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
#include "include.h"

#define AC_DAC_BASE      0x01c22c00
#define AC_DAC_DPC   	__REG(AC_DAC_BASE + 0x00)
#define AC_DAC_FIFOC 	__REG(AC_DAC_BASE + 0x04)
#define AC_DAC_FIFOS 	__REG(AC_DAC_BASE + 0x08)
#define AC_DAC_TXDATA	__REG(AC_DAC_BASE + 0x0c)
#define AC_DAC_ACTL   	__REG(AC_DAC_BASE + 0x10)
#define AC_DAC_TUNE   	__REG(AC_DAC_BASE + 0x14)
#define AC_DAC_DG   	__REG(AC_DAC_BASE + 0x18)
#define AC_ADC_FIFOC   	__REG(AC_DAC_BASE + 0x1c)
#define AC_ADC_FIOS   	__REG(AC_DAC_BASE + 0x20)
#define AC_ADC_RXDATA   __REG(AC_DAC_BASE + 0x24)
#define AC_ADC_ACTL   	__REG(AC_DAC_BASE + 0x28)
#define AC_ADC_DG   	__REG(AC_DAC_BASE + 0x2c)
#define AC_DAC_CNT   	__REG(AC_DAC_BASE + 0x30)
#define AC_ADC_CNT   	__REG(AC_DAC_BASE + 0x34)
#define AC_DG       	__REG(AC_DAC_BASE + 0x38)

#define AC_INT       	30

#define AC_DMA_BASE     0x01c02000
#define AC_DMA_CFG      (AC_DMA_BASE + 0x100)
#define AC_DMA_IRQ      (AC_DMA_BASE + 0x00)
#define AC_DMA_SRC      (AC_DMA_BASE + 0x104)
#define AC_DMA_DES      (AC_DMA_BASE + 0x108)
#define AC_DMA_CNT      (AC_DMA_BASE + 0x10c)

__u32  codec_dma_hd = 0;
__u16  pbuf_play[2] = {0xBFDA, 0x7DF7};
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
int codec_init(void)
{
	int i;
	__u32 reg_val;

	//申请一路DMA，用于CODEC
	codec_dma_hd = wBoot_dma_request(DMAC_DMATYPE_NORMAL);
	if(!codec_dma_hd)
	{
		__inf("codec led error: unable to request one dma channal\n");

		return -1;
	}
#if 1
	reg_val = *(volatile __u32 *)(0x1c20800 + 0x100);
	reg_val &= ~(0x7U << 28);
	reg_val |=  (0x01 << 28);
	*(volatile __u32 *)(0x1c20800 + 0x100) = reg_val;

	reg_val = *(volatile __u32 *)(0x1c20800 + 0x110);
	reg_val &= ~(0x2U << 30);
	reg_val |=  (0x01 << 30);
	*(volatile __u32 *)(0x1c20800 + 0x110) = reg_val;

	*(volatile __u32 *)(0x1c20800 + 0x10C) |= (1<<15);
#endif
	//PLL SETING
	//put_wvalue(CCM_AUDIO_PLL_REG,reg_val);
	//version A
	//put_wvalue(CCM_AUDIO_PLL_REG,0x80105309);
	//VERSION B
	CCMU_REG_PLL2_CTRL = 0x90105615;
	for(i=0;i< 0x2fff;i++);
	CCMU_REG_APB_MOD0 |= 1<<0;

	*(volatile unsigned int *)(0x01c20000 + 0x140) |= 1U<<31;

	return 0;
}
//*****************************************************************************
//	uint32 ac_setup_dma()
//  Description:
//			Setup normal DMA for Audio Codec
//	Arguments:
//			uint32 dma_id
//						Normal (Bus) DMA ID number
//						0--3: Normal (Bus) DMA ID number 0, 1, 2, 3
//						Others: Reserved
//      uint32 dma_bus_width
//            Bus width (16 or 32) for normal DMA
//			uint32 rw_flag
//						Read/ Write flag (0: Read, 1: Write)
//			uint32 buf_adr
//						The start address of data buffer (WORD aligned)
//			uint32 bcnt
//						DMA byte number
//
//	Return Value:	None
//*****************************************************************************
void ac_setup_dma(unsigned int saddr, unsigned int daddr, unsigned int  bcnt)
{
	__dma_setting_t  dma_config;

	dma_config.cfg.src_drq_type = 0x16;		//DRAM
	dma_config.cfg.src_addr_type = 0;		//源端地址不变
	dma_config.cfg.src_burst_length = 0;	//burst = 1
	dma_config.cfg.src_data_width   = 1;    //一次传输16bit
	dma_config.cfg.dst_drq_type     = 0x13;	//D/A
	dma_config.cfg.dst_addr_type    = 1;    //目的端地址不变
	dma_config.cfg.dst_burst_length = 0;	//burst = 1
	dma_config.cfg.dst_data_width   = 1;    //一次接收16bit
	dma_config.cfg.continuous_mode  = 1;    //dma loop模式

	dma_config.pgsz = 0;
	dma_config.pgstp = 0;
	dma_config.cmt_blk_cnt = 0;

	wBoot_dma_Setting(codec_dma_hd, (void *)&dma_config);

	wlibc_CleanFlushDCacheRegion((void *)saddr, bcnt);
	wBoot_dma_start(codec_dma_hd, saddr, daddr, bcnt);

	return ;
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
__u32 codec_play(void)
{
	int i;

	AC_DAC_DPC = 0x80000000;
	AC_DAC_FIFOC = 0x11400f11;

	AC_DAC_ACTL |= (0x3U<<30)|(0x1<<8)|(0x1<<6)|(0x3d);
	AC_ADC_ACTL |= (0x1<<4);

	ac_setup_dma((unsigned int)pbuf_play, AC_DAC_BASE + 0x0c, 4);

	for(i=0;i< 0xfff;i++);
	AC_DAC_ACTL |= 0x02;

	return codec_dma_hd;
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
void codec_wink(int status)
{
	int i;

	wBoot_dma_stop(codec_dma_hd);
	if(status)
	{
		pbuf_play[0] = 0xBFDA;
		pbuf_play[1] = 0x7DF7;
	}
	else
	{
		pbuf_play[0] = 0;
		pbuf_play[1] = 0;
	}
	AC_DAC_FIFOC |= 0x01;
	for(i=0;i<0xffff;i++)
	{
		if(!(AC_DAC_FIFOC & 0x01))
		{
			break;
		}
	}
	wBoot_dma_Restart(codec_dma_hd);

	return ;
}


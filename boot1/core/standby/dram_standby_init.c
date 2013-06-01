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
#include "dram_i.h"


/*
*********************************************************************************************************
*                                   DRAM INIT
*
* Description: dram init function
*
* Arguments  : para     dram config parameter
*
*
* Returns    : result
*
* Note       :
*********************************************************************************************************
*/
void mctl_ddr3_reset(void)
{
	__u32 reg_val;

	reg_val = mctl_read_w(SDR_CR);
	reg_val &= ~(0x1<<12);
	mctl_write_w(SDR_CR, reg_val);
	standby_delay(0x100);
	reg_val = mctl_read_w(SDR_CR);
	reg_val |= (0x1<<12);
	mctl_write_w(SDR_CR, reg_val);
}

void mctl_set_drive(void)
{
    __u32 reg_val;

    reg_val = mctl_read_w(SDR_CR);
    reg_val |= (0x6<<12);
		reg_val |= 0xFFC;
    reg_val &= ~0x3;
    reg_val &= ~(0x3<<28);
    mctl_write_w(SDR_CR, reg_val);
}

void mctl_itm_disable(void)
{
    __u32 reg_val = 0x0;

	reg_val = mctl_read_w(SDR_CCR);
	reg_val |= 0x1<<28;
	reg_val &= ~(0x1U<<31);          //danielwang, 2012-06-12
	mctl_write_w(SDR_CCR, reg_val);
}

void mctl_itm_enable(void)
{
    __u32 reg_val = 0x0;

    reg_val = mctl_read_w(SDR_CCR);
    reg_val &= ~(0x1<<28);
    mctl_write_w(SDR_CCR, reg_val);
}

void mctl_enable_dll0(void)
{
    __u32 i = 0;

    mctl_write_w(SDR_DLLCR0, mctl_read_w(SDR_DLLCR0) & ~0x40000000 | 0x80000000);
	standby_delay(0x100);

    mctl_write_w(SDR_DLLCR0, mctl_read_w(SDR_DLLCR0) & ~0xC0000000);
	standby_delay(0x1000);

    mctl_write_w(SDR_DLLCR0, mctl_read_w(SDR_DLLCR0) & ~0x80000000 | 0x40000000);
    standby_delay(0x1000);
}

void mctl_enable_dllx(__u32 phase)
{
    __u32 i = 0;
    __u32 reg_val;
    __u32 dll_num;
    __u32	dqs_phase = phase;

	reg_val = mctl_read_w(SDR_DCR);
	reg_val >>=6;
	reg_val &= 0x7;
	if(reg_val == 3)
		dll_num = 5;
	else
		dll_num = 3;

    for(i=1; i<dll_num; i++)
	{
		mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~(0xf<<14) | ((dqs_phase&0xf)<<14));
		mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~0x40000000 | 0x80000000);
		dqs_phase = dqs_phase>>4;
	}

	standby_delay(0x100);

    for(i=1; i<dll_num; i++)
    {
        mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~0xC0000000);
    }

	standby_delay(0x1000);

    for(i=1; i<dll_num; i++)
    {
        mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~0x80000000 | 0x40000000);
    }
    standby_delay(0x1000);
}

void mctl_disable_dll(void)
{
	__u32 reg_val;

	reg_val = mctl_read_w(SDR_DLLCR0);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR0, reg_val);

	reg_val = mctl_read_w(SDR_DLLCR1);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR1, reg_val);

	reg_val = mctl_read_w(SDR_DLLCR2);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR2, reg_val);

	reg_val = mctl_read_w(SDR_DLLCR3);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR3, reg_val);

	reg_val = mctl_read_w(SDR_DLLCR4);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR4, reg_val);
}



void mctl_configure_hostport(void)
{
    __u32 i;
	__u32 hpcr_value[32] = {
			0x00000301,0x00000301,0x00000301,0x00000301,
			0x00000301,0x00000301,0x00000301,0x00000301,
			0x0,       0x0,       0x0,       0x0,
			0x0,       0x0,       0x0,       0x0,
			0x00001031,0x00001031,0x00000735,0x00001035,
			0x00001035,0x00000731,0x00001031,0x00000735,
			0x00001035,0x00001031,0x00000731,0x00001035,
			0x00001031,0x00000301,0x00000301,0x00000731,	
	};
 	for(i=0; i<8; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), hpcr_value[i]);
	}
	
	for(i=16; i<28; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), hpcr_value[i]);
	}	
	
	mctl_write_w(SDR_HPCR + (29<<2), hpcr_value[i]);
	mctl_write_w(SDR_HPCR + (31<<2), hpcr_value[i]);
}

/*
*********************************************************************************************************
*                                   DRAM EXIT
*
* Description: dram exit;
*
* Arguments  : none;
*
* Returns    : result;
*
* Note       :
*********************************************************************************************************
*/
__s32 DRAMC_exit(void)
{
    return 0;
}

/*
*********************************************************************************************************
*                                   CHECK DDR READPIPE
*
* Description: check ddr readpipe;
*
* Arguments  : none
*
* Returns    : result, 0:fail, 1:success;
*
* Note       :
*********************************************************************************************************
*/
__s32 DRAMC_scan_readpipe(void)
{
    __u32 reg_val;

    //data training trigger
    reg_val = mctl_read_w(SDR_CCR);
    reg_val |= 0x1<<30;
    mctl_write_w(SDR_CCR, reg_val);

    //check whether data training process is end
    while(mctl_read_w(SDR_CCR) & (0x1<<30)) {};

    //check data training result
    reg_val = mctl_read_w(SDR_CSR);
    if(reg_val & (0x1<<20))
    {
        return -1;
    }

    return (0);
}


/*
*********************************************************************************************************
*                                   DRAM SCAN READ PIPE
*
* Description: dram scan read pipe
*
* Arguments  : none
*
* Returns    : result, 0:fail, 1:success;
*
* Note       :
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                   DRAM CLOCK CONTROL
*
* Description: dram get clock
*
* Arguments  : on   dram clock output (0: disable, 1: enable)
*
* Returns    : none
*
* Note       :
*********************************************************************************************************
*/
void DRAMC_clock_output_en(__u32 on)
{
    __u32 reg_val;

    reg_val = mctl_read_w(SDR_CR);

    if(on)
        reg_val |= 0x1<<16;
    else
        reg_val &= ~(0x1<<16);

    mctl_write_w(SDR_CR, reg_val);
}

__s32 dram_scan_readpipe(void)
{
    return DRAMC_scan_readpipe();
}



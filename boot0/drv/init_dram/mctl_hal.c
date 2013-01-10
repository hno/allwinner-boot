//*****************************************************************************
//	Allwinner Technology, All Right Reserved. 2006-2010 Copyright (c)
//
//	File: 				mctl_hal.c
//
//	Description:  This file implements basic functions for AW1633 DRAM controller
//
//	History:
//              2012/02/06      Berg Xing       0.10    Initial version
//              2012/02/24      Berg Xing       0.20    Support 2 channel
//              2012/02/27      Berg Xing       0.30    modify mode register access
//				2012/03/01		Berg Xing       0.40    add LPDDR2
//				2012/03/10		Berg Xing       0.50    add mctl_dll_init() function
//				2012/04/26		Berg Xing       0.60    add deep sleep
//				2012/06/19		Berg Xing       0.70    add 2T mode
//				2012/11/07		CPL				0.80	FPGA version based on berg's code
//				2012/11/14		CPL				0.90	add SID and regulate the parameters order
//				2012/11/21		CPL				0.91	modify parameters error
//*****************************************************************************
//#include "include.h"
#include "mctl_reg.h"
#include "mctl_hal.h"

//structure & macro definition
__dram_para_t *dram_para;
//#define __FPGA__

//#define DDR2_FPGA_S2C
//#define DDR2_FPGA_S2C_2C
//#define LPDDR2_FPGA_S2C_2CS_2CH
#define DDR3_32B

static void aw_delay (unsigned int n)
{
	while(n--);
}

//--------------------------------------------external function definition-------------------------------------------
//*****************************************************************************
//	unsigned int mctl_init()
//  Description:	DRAM Controller Initialize Procession
//
//	Arguments:		None
//
//	Return Value:	0: Fail    others: dram size
//*****************************************************************************
unsigned int DRAMC_init(__dram_para_t *para)
{
	unsigned int i, mem_val;
	//check input dram parameter structure
//  if(!para)
//	{
//   	//dram parameter is invalid
//    	return 0;
//	}

   //***********************************************
   // dram system init
   //***********************************************
  	mctl_sys_init();

    //***********************************************
    // dram dll init
    //***********************************************
	if(((para->dram_para2>>8)&0xF) == 2)
	{
		mctl_dll_init(0, para);
		mctl_dll_init(1, para);
	}
	else
	{
		mctl_dll_init(0, para);
	}

	//release mctl reset
	mctl_reset_release();

   //***********************************************
   // dram mctl & phy init
   //***********************************************
	if(((para->dram_para2>>8)&0xF) == 2)
	{
		mctl_channel_init(0, para);
		mctl_channel_init(1, para);
	}
	else
	{
		mctl_channel_init(0, para);
	}
   //***********************************************
   // dram com init
   //***********************************************
	mctl_com_init(para);

   //***********************************************
   // dram port configure
   //***********************************************
	mctl_port_cfg();

	return (para->dram_para1&0xFFFF);
}
//*****************************************************************************
//	unsigned int DRAMC_init_auto(__dram_para_t *para)
//  Description:	DRAM auto detect Initialize Procession
//
//	Arguments:		None
//
//	Return Value:	0: Fail    others: dram size
//*****************************************************************************
unsigned int DRAMC_init_auto(__dram_para_t *para)
{
	unsigned int i, j, row;
	unsigned int err_flag = 0;
	unsigned int id = 0;
	unsigned int ch_lock = 0;
	unsigned int bus_lock = 0;

	//config the init parameters according different IC
	//common detect init

	//para->dram_bus_width = 16;
	paraconfig(&(para->dram_para2), 0xF<<0, 0<<0);
	//para->dram_page_size = 2;
	paraconfig(&(para->dram_para1), 0xF<<16, 2<<16);
	//para->dram_rank_num = 1;
	paraconfig(&(para->dram_para2), 0xF<<12, 1<<12);
	//para->dram_size = 0;
	paraconfig(&(para->dram_para1), 0xFFFF<<0, 0<<0);
	//para->dram_row_num = 14;
	paraconfig(&(para->dram_para1), 0xFF<<20, 14<<20);
	//para->dram_bank_size = 8;
	paraconfig(&(para->dram_para1), 0xF<<28, 1<<28);

#ifndef __FPGA__
	id = ss_bonding_id();

	if(id == 0)//A31
	{
		paraconfig(&(para->dram_tpr13), 0x3<<3, 0<<3);
		if((para->dram_tpr13 & (0x1<<2)) == 1)//bus width lock
		{
			bus_lock = 1;
			//para->dram_bus_width = 32;
			paraconfig(&(para->dram_para2), 0xF<<0, 1<<0);
			//para->dram_page_size = 4;
			paraconfig(&(para->dram_para1), 0xF<<16, 4<<16);
		}

		if((para->dram_tpr13 & (0x1<<1)) == 1)//channel lock
		{
			ch_lock = 1;
			//para->dram_ch_num = 2;
			paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);
		}
	}else if(id == 1)//A31S
	{
		paraconfig(&(para->dram_tpr13), 0x3<<3, 1<<3);
		if((para->dram_tpr13 & (0x1<<2)) == 1)//bus width lock
		{
			bus_lock = 1;
			//para->dram_bus_width = 32;
			paraconfig(&(para->dram_para2), 0xF<<0, 1<<0);
			//para->dram_page_size = 4;
			paraconfig(&(para->dram_para1), 0xF<<16, 4<<16);
		}

		if((para->dram_tpr13 & (0x1<<1)) == 1)//channel lock
		{
			ch_lock = 1;
			//para->dram_ch_num = 1;
			paraconfig(&(para->dram_para2), 0xF<<8, 1<<8);
		}
	}else if(id == 2)//A3X PHONE
	{
		paraconfig(&(para->dram_tpr13), 0x3<<3, 2<<3);
	}else
	{
		paraconfig(&(para->dram_tpr13), 0x3<<3, 3<<3);
	}
#endif

	//channel number detect
	if(ch_lock == 0)// channel num auto detect
	{
		//para->dram_ch_num = 2;
		paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);

		//dram init
		DRAMC_init(para);

		for(i=0;i<32;i++)
		{
			mctl_write_w(0x40000000 + i*4, i*4 | (i*4+1)<<8 | (i*4+2)<<16 | (i*4+3)<<24);
		}

		for(i=0;i<16;i++)
		{
			if(mctl_read_w(0x40000000 + i*4) != (i*4 | (i*4+1)<<8 | (i*4+2)<<16 | (i*4+3)<<24))
			{
				//dram init fail
				return 0;
			}
		}
		for(i=16;i<32;i++)
		{
			if(mctl_read_w(0x40000000 + i*4) != (i*4 | (i*4+1)<<8 | (i*4+2)<<16 | (i*4+3)<<24))
			{
				//para->dram_ch_num = 1;
				paraconfig(&(para->dram_para2), 0xF<<8, 1<<8);
				break;
			}
		}
	}

	//bus width detect
	if(bus_lock == 0)//bus width auto detect
	{
		//para->dram_bus_width = 32;
		paraconfig(&(para->dram_para2), 0xF<<0, 1<<0);
		//para->dram_page_size = 2;
		paraconfig(&(para->dram_para1), 0xF<<16, 2<<16);
		//dram init
		DRAMC_init(para);

		//para->dram_page_size = 4;
		paraconfig(&(para->dram_para1), 0xF<<16, 4<<16);
		for(i=0;i<32;i++)
		{
			mctl_write_w(0x40000000 + i*4, i*4 | (i*4+1)<<8 | (i*4+2)<<16 | (i*4+3)<<24);
		}

		for(i=0;i<32;i++)
		{
			if(mctl_read_w(0x40000000 + i*4) != (i*4 | (i*4+1)<<8 | (i*4+2)<<16 | (i*4+3)<<24))
			{
				err_flag |= 1<<31;
				//para->dram_bus_width = 16;
				paraconfig(&(para->dram_para2), 0xF<<0, 0<<0);
				//para->dram_page_size = 2;
				paraconfig(&(para->dram_para1), 0xF<<16, 2<<16);
				break;
			}
		}
	}


	//row width detect
	for(row=16;row>11;row--)
	{
		//para->dram_row_num = row;
		paraconfig(&(para->dram_para1), 0xFF<<20, row<<20);

		//dram init
		DRAMC_init(para);

		for(i=0;i<32;i++)
		{
			mctl_write_w(0x40000000 + i*4, 0x40000000 + i*4);
		}

		for(i=0x8000000;i<=0x80000000;i+=0x8000000)
		{
			for(j=0;j<32;j++)
			{
				if((i<0x80000000) &&
				   (mctl_read_w(0x40000000 + i + j*4) != mctl_read_w(0x40000000 + j*4)))
				{
					break;
				}else if((i == 0x80000000) && (mctl_read_w(0x40000000 + i + j*4) == 0))
				{
					continue;
				}else if((i == 0x80000000) && (mctl_read_w(0x40000000 + i + j*4) != 0))
				{
					break;
				}
			}
			if(j == 32)
			{
				//if((i>>20) < para->dram_size)
				if((i>>20) < ((para->dram_para1)&0xFFFF))
				{
					//para->dram_row_num = row + 1;
					paraconfig(&(para->dram_para1), 0xFF<<20, (row+1)<<20);
					break;
				}else
				{
					//para->dram_size = i>>20;
					paraconfig(&(para->dram_para1), 0xFFFF<<0, (i>>20)<<0);
					//para->dram_row_num = row;
					paraconfig(&(para->dram_para1), 0xFF<<20, row<<20);
					break;
				}
			}
		}

		if((i>>20)< ((para->dram_para1)&0xFFFF))
		{
			break;
		}
	}

	//rank number detect
	//if(para->dram_ch_num == 2)
	if(((para->dram_para2>>8)&0xF) == 2)
	{
		if(((para->dram_para1)&0xFFFF) < 2048)
		{
			//para->dram_ch_num = 1;
			paraconfig(&(para->dram_para2), 0xF<<8, 1<<8);
			//para->dram_rank_num = 2;
			paraconfig(&(para->dram_para2), 0xF<<12, 2<<12);

			//dram init
			DRAMC_init(para);

			for(i=0;i<32;i++)
			{
				mctl_write_w(0x40000000 + i*4, 0x40000000 + i*4);
				mctl_write_w(0x40000000 + (((para->dram_para1)&0xFFFF)<<19) + i*4, 0x40000000 + (((para->dram_para1)&0xFFFF)<<19) + i*4);
			}
			for(i=0;i<32;i++)
			{
				if((mctl_read_w(0x40000000 + i*4) != mctl_read_w(0x40000000 + (((para->dram_para1)&0xFFFF)<<20) + i*4)) )
				{
					break;
				}
			}
			if(i != 32)
			{
				//para->dram_ch_num = 2;
				paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);
				//para->dram_rank_num = 1;
				paraconfig(&(para->dram_para2), 0xF<<12, 1<<12);
			}else if(i == 32)
			{
				for(i=0;i<32;i++)
				{
					if((mctl_read_w(0x40000000 + (((para->dram_para1)&0xFFFF)<<19) + i*4) != (0x40000000 + (((para->dram_para1)&0xFFFF)<<19) + i*4)) )
					{
						break;
					}
				}
				if(i == 32)
				{
					//para->dram_ch_num = 2;
					paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);
					//para->dram_rank_num = 2;
					paraconfig(&(para->dram_para2), 0xF<<12, 2<<12);
					//para->dram_size <<= 1;
					paraconfig(&(para->dram_para1), 0xFFFF<<0, (((para->dram_para1)&0xFFFF)<<1)<<0);
				}else
				{
					//para->dram_ch_num = 2;
					paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);
					//para->dram_rank_num = 1;
					paraconfig(&(para->dram_para2), 0xF<<12, 1<<12);
				}
			}
			//dram init
			DRAMC_init(para);
		}

	}else if(((para->dram_para2>>8)&0xF) == 1)
	{
		if(((para->dram_para1)&0xFFFF) < 2048)
		{
			//para->dram_rank_num = 2;
			paraconfig(&(para->dram_para2), 0xF<<12, 2<<12);
			//dram init
			DRAMC_init(para);
			for(i=0;i<32;i++)
			{
				mctl_write_w(0x40000000 + i*4, 0x40000000 + i*4);
				mctl_write_w(0x40000000 + (((para->dram_para1)&0xFFFF)<<20) + i*4, 0x40000000 + (((para->dram_para1)&0xFFFF)<<20) + i*4);
			}
			for(i=0;i<32;i++)
			{
				if((mctl_read_w(0x40000000 + i*4) != mctl_read_w(0x40000000 + (((para->dram_para1)&0xFFFF)<<20) + i*4)) &&
				   (mctl_read_w(0x40000000 + (((para->dram_para1)&0xFFFF)<<20) + i*4) == (0x40000000 + (((para->dram_para1)&0xFFFF)<<20) + i*4)))
				{
					break;
				}
			}
			if(i == 32)
			{
				//para->dram_rank_num = 1;
				paraconfig(&(para->dram_para2), 0xF<<12, 1<<12);
			}else
			{
				//para->dram_size <<= 1;
				paraconfig(&(para->dram_para1), 0xFFFF<<0, (((para->dram_para1)&0xFFFF)<<1)<<0);
			}
			//dram init
			DRAMC_init(para);
		}
	}

	return ((para->dram_para1)&0xFFFF);
}


unsigned int mctl_sys_init(void)
{
	unsigned int reg_val;

	//PLL5 disable
	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
  	reg_val &= ~(0x1U<<31);
  	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

	//config PLL5 DRAM CLOCK: PLL5 = (24*N*K)/M
	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
	reg_val &= ~((0x3<<0) | (0x3<<4) | (0x1F<<8));
	reg_val |= ((0x1<<0) | (0x1<<4));	//K = M = 2;
	reg_val |= ((dram_para->dram_clk/24-1)<<0x8);//N
	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

  	//PLL5 enable
	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
  	reg_val |= 0x1U<<31;
  	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

  	//PLL5 configuration update(validate PLL5)
  	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
  	reg_val |= 0x1U<<20;
  	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

#ifndef SYSTEM_SIMULATION
  	aw_delay(0x20);
#endif

  	//MDFS clock enable
	reg_val = mctl_read_w(CCM_MDFS_CLK_CTRL);
	reg_val |= 0x1U<<31;
  	mctl_write_w(CCM_MDFS_CLK_CTRL, reg_val);

  	//select DRAM clock
  	reg_val = mctl_read_w(CCM_DRAMCLK_CFG_CTRL);
	reg_val |= 0x1U<<16;
  	mctl_write_w(CCM_DRAMCLK_CFG_CTRL, reg_val);

  	//release DRAMC register reset
  	reg_val = mctl_read_w(CCM_AHB1_RST_REG0);
  	reg_val |= 0x1<<14;
  	mctl_write_w(CCM_AHB1_RST_REG0, reg_val);

  	//DRAMC AHB clock on
  	reg_val = mctl_read_w(CCM_AHB1_GATE0_CTRL);
  	reg_val |= 0x1<<14;
  	mctl_write_w(CCM_AHB1_GATE0_CTRL, reg_val);

  	return (1);
}

unsigned int mctl_reset_release(void)
{
	unsigned int reg_val;

  	reg_val = mctl_read_w(CCM_DRAMCLK_CFG_CTRL);
  	reg_val |= 0x1U<<31;
  	mctl_write_w(CCM_DRAMCLK_CFG_CTRL, reg_val);

#ifndef SYSTEM_SIMULATION
  	aw_delay(0x20);
#endif

  	return (1);
}

unsigned int mctl_dll_init(unsigned int ch_index, __dram_para_t *para)
{
	unsigned int ch_id;

	if(ch_index == 1)
		ch_id = 0x1000;
	else
		ch_id = 0x0;

   //***********************************************
   // set dram PHY register
   //***********************************************
	//reset dll
	mctl_write_w(ch_id + SDR_ACDLLCR,0x80000000);
	mctl_write_w(ch_id + SDR_DX0DLLCR,0x80000000);
	mctl_write_w(ch_id + SDR_DX1DLLCR,0x80000000);
	//if(para->dram_bus_width == 32)
	if(((para->dram_para2>>0)&0xF) == 1)
	{
		mctl_write_w(ch_id + SDR_DX2DLLCR,0x80000000);
		mctl_write_w(ch_id + SDR_DX3DLLCR,0x80000000);
	}

#ifndef SYSTEM_SIMULATION
	aw_delay(0x10);
#endif

	//enable dll
	mctl_write_w(ch_id + SDR_ACDLLCR,0x0);
	mctl_write_w(ch_id + SDR_DX0DLLCR,0x0);
	mctl_write_w(ch_id + SDR_DX1DLLCR,0x0);
	//if(para->dram_bus_width == 32)
	if(((para->dram_para2>>0)&0xF) == 1)
	{
		mctl_write_w(ch_id + SDR_DX2DLLCR,0x0);
		mctl_write_w(ch_id + SDR_DX3DLLCR,0x0);
	}

#ifndef SYSTEM_SIMULATION
	aw_delay(0x10);
#endif

	//release reset dll
	mctl_write_w(ch_id + SDR_ACDLLCR,0x40000000);
	mctl_write_w(ch_id + SDR_DX0DLLCR,0x40000000);
	mctl_write_w(ch_id + SDR_DX1DLLCR,0x40000000);
	//if(para->dram_bus_width == 32)
	if(((para->dram_para2>>0)&0xF) == 1)
	{
		mctl_write_w(ch_id + SDR_DX2DLLCR,0x40000000);
		mctl_write_w(ch_id + SDR_DX3DLLCR,0x40000000);
	}
#ifndef SYSTEM_SIMULATION
	aw_delay(0x10);
#endif

	return (1);
}

unsigned int mctl_channel_init(unsigned int ch_index, __dram_para_t *para)
{
	unsigned int reg_val;
	unsigned int clkmhz;
	unsigned int ch_id;
	unsigned int hold_flag;

	//get flag of pad hold status
#ifdef __FPGA__
	reg_val = 0;
#else
	reg_val = mctl_read_w(R_VDD_SYS_PWROFF_GATE);
#endif

	if(ch_index == 1)
	{
		ch_id = 0x1000;
		hold_flag = (reg_val>>1)&0x1;
	}
	else
	{
		ch_id = 0x0;
		hold_flag = reg_val&0x1;
	}

	//set COM sclk enable register
	reg_val = mctl_read_w(SDR_COM_CCR);
	reg_val |= (0x4 | (0x1<<ch_index));
	mctl_write_w(SDR_COM_CCR, reg_val);

	//send NOP command to active CKE
	reg_val = 0x80000000;
	mctl_write_w(ch_id + SDR_MCMD, reg_val);

   //set PHY genereral configuration register
   reg_val = 0x01042202;
   if(((para->dram_para2>>12)&0xF) == 2)
   		reg_val |= (0x1<<19);
   mctl_write_w(ch_id + SDR_PGCR, reg_val);

   //set mode register
	mctl_write_w(ch_id + SDR_MR0, para->dram_mr0);
	mctl_write_w(ch_id + SDR_MR1, para->dram_mr1);
	mctl_write_w(ch_id + SDR_MR2, para->dram_mr2);
	mctl_write_w(ch_id + SDR_MR3, para->dram_mr3);

	//set PHY DDR mode
	if(para->dram_type == 2)		//DDR2
		reg_val = 0xa;
	else if(para->dram_type == 3)	//DDR3
		reg_val = 0xb;
	else if(para->dram_type == 5)	//LPDDR
		reg_val = 0x8;
	else							//LPDDR2
		reg_val = 0xc;
	mctl_write_w(ch_id + SDR_DCR, reg_val);

#ifndef __FPGA__
	//set DDR system general configuration register
	reg_val = 0xd200013b;
	mctl_write_w(ch_id + SDR_DSGCR, reg_val);

	//set DATX8 common configuration register
	reg_val = 0x910;
	mctl_write_w(ch_id + SDR_DXCCR, reg_val);
#endif

   //***********************************************
   // check dram PHY status
   //***********************************************
	while( (mctl_read_w(ch_id + SDR_PGSR)&0x3)!= 0x3 ) {};

	//init external dram
#ifdef __FPGA__
	reg_val = 0x69;
#else
	if(hold_flag)
	{
		reg_val = 0x41;
	}
	else
	{
		reg_val = 0xe9;
	}
#endif
	mctl_write_w(ch_id + SDR_PIR, reg_val);

#ifndef SYSTEM_SIMULATION
	aw_delay(0x10);
#endif

	//wait init done
	while( (mctl_read_w(ch_id + SDR_PGSR)&0x1) == 0x0) {};

   //***********************************************
   // set dram MCTL register
   //***********************************************
	//move to configure state
	reg_val = 0x1;
	mctl_write_w(ch_id + SDR_SCTL, reg_val);
	while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x1 ) {};

	//set memory timing regitsers
	clkmhz = para->dram_clk;
	//clkmhz = clkmhz/1000000;
	reg_val = clkmhz;
	mctl_write_w(ch_id + SDR_TOGCNT1U, reg_val);		//1us
	reg_val = clkmhz/10;
	mctl_write_w(ch_id + SDR_TOGCNT100N, reg_val);		//100ns
	mctl_write_w(ch_id + SDR_TREFI	,((para->dram_tpr2)>>15)&0xFF);
	mctl_write_w(ch_id + SDR_TMRD	,((para->dram_tpr3)>>0)&0x7);
	mctl_write_w(ch_id + SDR_TRFC	,((para->dram_tpr2)>>23)&0x1FF);
	mctl_write_w(ch_id + SDR_TRP	,(((para->dram_tpr3)>>3)&0xF) | ((((para->dram_tpr1)>>0)&0x3)<<16));
	mctl_write_w(ch_id + SDR_TRTW	,((para->dram_tpr4)>>8)&0xF);
	mctl_write_w(ch_id + SDR_TAL	,((para->dram_tpr4)>>4)&0xF);
	mctl_write_w(ch_id + SDR_TCL	,((para->dram_tpr4)>>0)&0xF);
	mctl_write_w(ch_id + SDR_TCWL	,((para->dram_tpr5)>>28)&0xF);
	mctl_write_w(ch_id + SDR_TRAS	,((para->dram_tpr3)>>19)&0x3F);
	mctl_write_w(ch_id + SDR_TRC	,((para->dram_tpr3)>>13)&0x3F);
	mctl_write_w(ch_id + SDR_TRCD	,((para->dram_tpr5)>>24)&0xF);
	mctl_write_w(ch_id + SDR_TRRD	,((para->dram_tpr5)>>20)&0xF);
	mctl_write_w(ch_id + SDR_TRTP	,((para->dram_tpr5)>>16)&0xF);
	mctl_write_w(ch_id + SDR_TWR	,((para->dram_tpr5)>>12)&0xF);
	mctl_write_w(ch_id + SDR_TWTR	,((para->dram_tpr5)>>8)&0xF);
	mctl_write_w(ch_id + SDR_TEXSR	,((para->dram_tpr1)>>22)&0x3FF);
	mctl_write_w(ch_id + SDR_TXP	,((para->dram_tpr5)>>1)&0x7);
	mctl_write_w(ch_id + SDR_TXPDLL ,((para->dram_tpr3)>>7)&0x3F);
	mctl_write_w(ch_id + SDR_TZQCS	,((para->dram_tpr2)>>0)&0x7F);
	mctl_write_w(ch_id + SDR_TZQCSI ,(para->dram_tpr0));
	mctl_write_w(ch_id + SDR_TDQS   ,((para->dram_tpr6)>>29)&0x7);
	mctl_write_w(ch_id + SDR_TCKSRE ,((para->dram_tpr4)>>27)&0x1F);
	mctl_write_w(ch_id + SDR_TCKSRX ,((para->dram_tpr4)>>22)&0x1F);
	mctl_write_w(ch_id + SDR_TCKE 	,((para->dram_tpr4)>>17)&0x1F);
	mctl_write_w(ch_id + SDR_TMOD 	,((para->dram_tpr4)>>12)&0x1F);
	mctl_write_w(ch_id + SDR_TRSTL  ,((para->dram_tpr3)>>25)&0x7F);
	mctl_write_w(ch_id + SDR_TZQCL  ,((para->dram_tpr1)>>2)&0x2FF);
	mctl_write_w(ch_id + SDR_TMRR 	,((para->dram_tpr2)>>7)&0xFF);
	mctl_write_w(ch_id + SDR_TCKESR ,((para->dram_tpr5)>>4)&0xF);
	mctl_write_w(ch_id + SDR_TDPD 	,((para->dram_tpr1)>>12)&0x3FF);

	//select 16/32-bits mode for MCTL
	reg_val = 0x0;
	//if(para->dram_bus_width == 16)
	if(((para->dram_para2>>0)&0xF) == 0)
		reg_val = 0x1;
	mctl_write_w(ch_id + SDR_PPCFG, reg_val);

	//set DFI timing registers
//	mctl_write_w(ch_id + SDR_DFITPHYWRL, 1);
	//if((para->dram_timing.dram_2t_mode == 1) && (para->dram_type != 6))
	if((((para->dram_tpr13>>5)&0x1) == 1) && (para->dram_type != 6))
	{
		reg_val = ((para->dram_tpr5)>>28)&0xF - 0;
		mctl_write_w(ch_id + SDR_DFITPHYWRL, reg_val);
		reg_val = ((para->dram_tpr4)>>0)&0xF - 1;
		mctl_write_w(ch_id + SDR_DFITRDDEN, reg_val);
	}
	else
	{
		reg_val = ((para->dram_tpr5)>>28)&0xF - 1;
		mctl_write_w(ch_id + SDR_DFITPHYWRL, reg_val);
		reg_val = ((para->dram_tpr4)>>0)&0xF - 2;
		mctl_write_w(ch_id + SDR_DFITRDDEN, reg_val);
	}
	mctl_write_w(ch_id + SDR_DFITPHYRDL, 15);

	reg_val = 0x5;
	mctl_write_w(ch_id + SDR_DFISTCFG0, reg_val);

	//configure memory related attributes of mctl
	if(para->dram_type == 2)			//DDR2
		reg_val = 0x70040;
	else if(para->dram_type == 3)		//DDR3
		reg_val = 0x70061;
	else if(para->dram_type == 5)		//LPDDR
		reg_val = 0x970040;
	else								//LPDDR2
		reg_val = 0xd70040;
	mctl_write_w(ch_id + SDR_MCFG, reg_val);

	//DFI update configuration register
	reg_val = 0x2;
	mctl_write_w(ch_id + SDR_DFIUPDCFG, reg_val);

	//move to access state
	reg_val = 0x2;
	mctl_write_w(ch_id + SDR_SCTL, reg_val);

	while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x3 ) {};

	if(hold_flag)
	{
		//move to sleep state
		reg_val = 0x3;
		mctl_write_w(ch_id + SDR_SCTL, reg_val);
		while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x5 ) {};

		//close pad hold function
		reg_val = mctl_read_w(R_VDD_SYS_PWROFF_GATE);
		if(ch_index == 1)
			reg_val &= ~(0x1<<1);
		else
			reg_val &= ~(0x1);
		mctl_write_w(R_VDD_SYS_PWROFF_GATE, reg_val);

		//set WAKEUP command
		reg_val = 0x4;
		mctl_write_w(ch_id + SDR_SCTL, reg_val);
		while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x3 ) {};

		//calibration and dqs training
		reg_val = 0x89;
		mctl_write_w(ch_id + SDR_PIR, reg_val);
		while( (mctl_read_w(ch_id + SDR_PGSR)&0x1) == 0x0) {};
	}

	//set power down period
	reg_val = mctl_read_w(ch_id + SDR_MCFG);
	reg_val |= 0x10 << 8;
	mctl_write_w(ch_id + SDR_MCFG, reg_val);

	return (1);
}

unsigned int mctl_com_init(__dram_para_t *para)
{
	unsigned int reg_val;

	//set COM memory organization register
	reg_val = 0;
	//if(para->dram_rank_num == 2)
	if(((para->dram_para2>>12)&0xF) == 2)
		reg_val |= 0x1;

	//if(para->dram_bank_size == 8)
	if(((para->dram_para1>>28)&0xF) == 1)
		reg_val |= 0x1<<2;

	//reg_val |= ((para->dram_row_num -1)&0xf)<<4;
	reg_val |= ((((para->dram_para1>>20)&0xFF) -1)&0xf)<<4;

/*
	if(para->dram_page_size == 8)
		reg_val |= 0xa<<8;
	else if(para->dram_page_size == 4)
		reg_val |= 0x9<<8;
	else if(para->dram_page_size == 2)
		reg_val |= 0x8<<8;
	else if(para->dram_page_size == 1)
		reg_val |= 0x7<<8;
	else
		reg_val |= 0x6<<8;
*/
	if(((para->dram_para1>>16)&0xF) == 8)
		reg_val |= 0xa<<8;
	else if(((para->dram_para1>>16)&0xF) == 4)
		reg_val |= 0x9<<8;
	else if(((para->dram_para1>>16)&0xF) == 2)
		reg_val |= 0x8<<8;
	else if(((para->dram_para1>>16)&0xF) == 1)
		reg_val |= 0x7<<8;
	else
		reg_val |= 0x6<<8;

/*
	if(para->dram_bus_width == 32)
		reg_val |= 0x3<<12;
	else
		reg_val |= 0x1<<12;
*/
	if(((para->dram_para2>>0)&0xF) == 1)
		reg_val |= 0x3<<12;
	else
		reg_val |= 0x1<<12;

/*
	if(para->dram_access_mode == 0)
		reg_val |= 0x1<<15;
*/
	if(((para->dram_para2>>4)&0xF) == 0)
		reg_val |= 0x1<<15;

	reg_val |= (para->dram_type)<<16;

/*
	if(para->dram_ch_num == 2)
		reg_val |= 0x1<<19;
*/
	if(((para->dram_para2>>8)&0xF) == 2)
		reg_val |= 0x1<<19;

	reg_val |= 0x1<<20;
#ifdef __FPGA__
	if(para->dram_type != 6)
	{
		reg_val |= 0x1<<22;
	}else
	{
		reg_val |= 0x319<<20;
	}
#else
	reg_val |= 0x1<<22;
#endif
	mctl_write_w(SDR_COM_CR, reg_val);

	if((((para->dram_tpr13>>5)&0x1) == 1) && (para->dram_type != 6))
	{
		reg_val = mctl_read_w(SDR_COM_DBGCR);
		reg_val |= 1U << 6;
		mctl_write_w(SDR_COM_DBGCR, reg_val);
	}

#ifdef __FPGA__
	//set preset readpipe value
	if(para->dram_type == 2)
	{// for DDR2
		reg_val = 0x9;
		reg_val |= 0x9<<8;
	}
	else
	{// for LPDDR2
		//reg_val = 0x6;
		//reg_val |= 0x6<<8;
	}
	mctl_write_w(SDR_COM_DBGCR1, reg_val);

#endif

	//set COM sclk enable register
//	reg_val = 0x7;
//	mctl_write_w(SDR_COM_CCR, reg_val);

	return (1);
}

unsigned int mctl_port_cfg(void)
{
	unsigned int reg_val;

	//enable DRAM AXI clock for CPU access
	reg_val = mctl_read_w(CCM_AXI_GATE_CTRL);
	reg_val |= 0x1;
	mctl_write_w(CCM_AXI_GATE_CTRL, reg_val);

  	return (1);
}


//*****************************************************************************
//	signed int init_DRAM(int type)
//  Description:	System init dram
//
//	Arguments:		type:	0: auto detect init		1: get the parameters and init
//
//	Return Value:	0: fail
//					others: pass
//*****************************************************************************
signed int init_DRAM(int type, void *para)
{
	unsigned int i;
	signed int ret_val;
	unsigned int id = 0;
	dram_para = (__dram_para_t *)para;

	//DDR2 128Mx8 (128M Byte), total 1GB
#ifdef DDR2_FPGA_S2C
	dram_para->dram_clk		=	24;
	dram_para->dram_type		=	2;
	dram_para->dram_zq		=	0;
	dram_para->dram_odt_en	=	0;
	dram_para->dram_para1	=	0x10E40400;
	dram_para->dram_para2	=	0x2111;
	dram_para->dram_mr0		=	0xA62;
	dram_para->dram_mr1		=	0;
	dram_para->dram_mr2		=	0;
	dram_para->dram_mr3		=	0;
	dram_para->dram_tpr0		=	0;
	dram_para->dram_tpr1		=	0x32000001;
	dram_para->dram_tpr2		=	0x1A270100;
	dram_para->dram_tpr3		=   0x930332;
	dram_para->dram_tpr4		=	0x60206;
	dram_para->dram_tpr5		=	0x56436422;
	dram_para->dram_tpr6		=	0x20000000;
	dram_para->dram_tpr7		=	0;
	dram_para->dram_tpr8		=	0;
	dram_para->dram_tpr9		=	0;
	dram_para->dram_tpr10	=	0;
	dram_para->dram_tpr11	=	0;
	dram_para->dram_tpr12	=	0;
	dram_para->dram_tpr13	=	0x7;

	//DDR2 128Mx8 (128M Byte), total 512MB
#elif defined DDR2_FPGA_S2C_2C
	dram_para->dram_clk		=	24;
	dram_para->dram_type		=	2;
	dram_para->dram_zq		=	0;
	dram_para->dram_odt_en	=	0;
	dram_para->dram_para1	=	0x10E20200;
	dram_para->dram_para2	=	0x1210;
	dram_para->dram_mr0		=	0xA62;
	dram_para->dram_mr1		=	0;
	dram_para->dram_mr2		=	0;
	dram_para->dram_mr3		=	0;
	dram_para->dram_tpr0		=	0;
	dram_para->dram_tpr1		=	0x32000001;
	dram_para->dram_tpr2		=	0x1A270100;
	dram_para->dram_tpr3		=   0x930332;
	dram_para->dram_tpr4		=	0x60206;
	dram_para->dram_tpr5		=	0x56436422;
	dram_para->dram_tpr6		=	0x20000000;
	dram_para->dram_tpr7		=	0;
	dram_para->dram_tpr8		=	0;
	dram_para->dram_tpr9		=	0;
	dram_para->dram_tpr10	=	0;
	dram_para->dram_tpr11	=	0;
	dram_para->dram_tpr12	=	0;
	dram_para->dram_tpr13	=	0x7;

	//LPDDR2 128Mx32 (512M Byte), total 2GB
#elif defined LPDDR2_FPGA_S2C_2CS_2CH
	dram_para->dram_clk		=	120;
	dram_para->dram_type		=	6;
	dram_para->dram_zq		=	0;
	dram_para->dram_odt_en	=	0;
	dram_para->dram_para1	=	0x10E40800;
	dram_para->dram_para2	=	0x2211;
	dram_para->dram_mr0		=	0;
	dram_para->dram_mr1		=	0x92;
	dram_para->dram_mr2		=	0x4;
	dram_para->dram_mr3		=	0x2;
	dram_para->dram_tpr0		=	0;
	dram_para->dram_tpr1		=	0x32000000;
	dram_para->dram_tpr2		=	0x1A270100;
	dram_para->dram_tpr3		=	0x936342;
	dram_para->dram_tpr4		=	0x29460206;
	dram_para->dram_tpr5		=	0x48436363;
	dram_para->dram_tpr6		=	0x20000000;
	dram_para->dram_tpr7		=	0;
	dram_para->dram_tpr8		=	0;
	dram_para->dram_tpr9		=	0;
	dram_para->dram_tpr10	=	0;
	dram_para->dram_tpr11	=	0;
	dram_para->dram_tpr12	=	0;
	dram_para->dram_tpr13	=	0x7;

	//DDR3 256Mx8 (256M Byte)
#elif defined DDR3_32B
	dram_para->dram_clk		=	528;
	dram_para->dram_type		=	3;
	dram_para->dram_zq		=	0;
	dram_para->dram_odt_en	=	0;
	dram_para->dram_para1	=	0x10F40800;
	dram_para->dram_para2	=	0x1211;
	dram_para->dram_mr0		=	0x1A50;
	dram_para->dram_mr1		=	0;
	dram_para->dram_mr2		=	0x18;
	dram_para->dram_mr3		=	0;
	dram_para->dram_tpr0		=	0;
	dram_para->dram_tpr1		=	0x80000000;
	dram_para->dram_tpr2		=	0x35A70140;
	dram_para->dram_tpr3		=	0xA0C4284C;
	dram_para->dram_tpr4		=	0x39c8c209;
	dram_para->dram_tpr5		=	0x8945A555;
	dram_para->dram_tpr6		=	0x20000000;
	dram_para->dram_tpr7		=	0;
	dram_para->dram_tpr8		=	0;
	dram_para->dram_tpr9		=	0;
	dram_para->dram_tpr10	=	0;
	dram_para->dram_tpr11	=	0;
	dram_para->dram_tpr12	=	0;
	dram_para->dram_tpr13	=	0x7;

#else

#endif

	//bonding ID
	//0: A31	1: A31S		2: A3X PHONE
#ifndef __FPGA__
	id = ss_bonding_id();

	if(id == 0)
	{
		//dram_para->dram_ch_num = 2;
		paraconfig(&dram_para->dram_para2, 0xF<<8, 2<<8);
		//dram_para->dram_bus_width = 32;
		paraconfig(&dram_para->dram_para2, 0xF<<0, 1<<0);
	}else if(id == 1)
	{
		//dram_para->dram_ch_num = 1;
		paraconfig(&dram_para->dram_para2, 0xF<<8, 1<<8);
		//dram_para->dram_bus_width = 32;
		paraconfig(&dram_para->dram_para2, 0xF<<0, 1<<0);
	}else if(id == 2)
	{

	}
#endif

	if(!type)
	{
		dram_para->dram_tpr13 = 0x1;
		ret_val = DRAMC_init_auto(dram_para);
	}else
	{
		dram_para->dram_tpr13 = 0x7;
		ret_val = DRAMC_init(dram_para);
	}

	return ret_val;
}

unsigned int ss_bonding_id(void)
{
	unsigned int reg_val;
	unsigned int id;
	//enable SS working clock
	reg_val = mctl_read_w(0x01c20000 + 0x9C); //CCM_SS_SCLK_CTRL
	//24MHz
	reg_val &= ~(0x3<<24);
	reg_val &= ~(0x3<<16);
	reg_val &= ~(0xf);
	reg_val |= 0x0<<16;
	reg_val |= 0;
	reg_val |= 0x1U<<31;
	mctl_write_w(0x01c20000 + 0x9C, reg_val);

//	for(i=0; i<0x1000; i++){};
	//enable SS AHB clock
	reg_val = mctl_read_w(0x01c20000 + 0x60); //CCM_AHB1_GATE0_CTRL
	reg_val |= 0x1<<5;		//SS AHB clock on
	mctl_write_w(0x01c20000 + 0x60, reg_val);

	reg_val = mctl_read_w(0x01C15000 + 0x00); //SS_CTL
	reg_val >>=16;
	reg_val &=0x3;
	mctl_write_w(0x01C15000 + 0x00,reg_val);

	id = reg_val;

	reg_val = mctl_read_w(0x01C15000 + 0x00); //SS_CTL
	reg_val &= ~0x1;
	mctl_write_w(0x01C15000 + 0x00,reg_val);

	//0: A31	1: A31S		2: A3X PHONE
	return id;
}

void paraconfig(unsigned int *para, unsigned int mask, unsigned int value)
{
	unsigned int reg_val = *para;

	reg_val &= ~(mask);
	reg_val |= value;

	*para = reg_val;
}







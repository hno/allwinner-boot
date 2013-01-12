/*
*********************************************************************************************************
* File    : dram_init.c
* By      : Berg.Xing
* Date    : 2011-06-01
* Descript: dram for AW1623 chipset;
* Update  : date                auther      ver     notes
*           2011-06-01      			Berg        1.0     create file
*           2011-06-15      			Berg        1.1     change pad drive mode to dynamic mode0;
*															change to automatic IDDQ mode when entry into
*															self-refresh or power down for saving power
*           2011-07-01      			Berg        1.2     add mctl_disable_dll() function
*															add odt control bit and odt impendance
*															increase dram pll delay time
*           2011-07-27      			Berg        1.3     change pad to high speed mode and
*															select DQS passive window mode
*															tune port priority level
*			2011-08-05					Berg		1.4		change mctl_ddr3_reset() for different die
*			2011-09-16					Berg		1.5		disable dqs drfit compensation for low tempature
*			2011-11-28					Berg        1.6     change output impendence by exteranl configure file
*           2012-02-06            		Daniel      1.7     change output impedance setting sequence to fix a bug
*                                                     and change bit12 of emr0 to enable DLL Fast Exit of DRAM
*           2012-05-17                  Daniel      1.8     change itm-disable function to add delay to CKE after
*                                                           Clock Stable
*     2012-06-15      Daniel      1.9     Adjust Initial Delay(including relation among RST/CKE/CLK)
*     2012-06-21      Daniel      2.0     Add Three Times DRAM Initial for CARD Boot (DRAMC_init_EX)
*********************************************************************************************************
*/
#include "boot0_i.h"
#include "dram_i.h"
#include "dram_para.h"
typedef  boot_dram_para_t		__dram_para_t;

extern   __u32 odt_status;

#pragma arm section rwdata="RW_KRNL_CLK",code="CODE_KRNL_CLK",rodata="RO_KRNL_CLK",zidata="ZI_KRNL_CLK"


void mctl_delay(__u32 dly)
{
	__u32	i;
	for(i=0; i<dly; i++){};
}
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

#ifndef FPGA_51_TEST
	mctl_write_w(TIMER_CPU_CFG_REG, 0);
	reg_val = mctl_read_w(TIMER_CPU_CFG_REG);
	reg_val >>=6;
	reg_val &=0x3;
	if(reg_val == 0)
	{
		reg_val = mctl_read_w(SDR_CR);
		reg_val &= ~(0x1<<12);
		mctl_write_w(SDR_CR, reg_val);
		mctl_delay(0x100);
		reg_val = mctl_read_w(SDR_CR);
		reg_val |= (0x1<<12);
		mctl_write_w(SDR_CR, reg_val);
	}
	else
	{
		reg_val = mctl_read_w(SDR_CR);
		reg_val |= (0x1<<12);
		mctl_write_w(SDR_CR, reg_val);
		mctl_delay(0x100);
		reg_val = mctl_read_w(SDR_CR);
		reg_val &= ~(0x1<<12);
		mctl_write_w(SDR_CR, reg_val);
	}
#else
	reg_val = mctl_read_w(SDR_CR);
	reg_val &= ~(0x1<<12);
	mctl_write_w(SDR_CR, reg_val);
	
	mctl_delay(0x100);
	
	reg_val = mctl_read_w(SDR_CR);
	reg_val |= (0x1<<12);
	mctl_write_w(SDR_CR, reg_val);

#endif
}

void mctl_set_drive(void)
{
	__u32 reg_val;

#ifndef FPGA_51_TEST
	reg_val = mctl_read_w(SDR_CR);
	reg_val |= (0x6<<12);
	reg_val |= 0xFFC;
	reg_val &= ~0x3;
	mctl_write_w(SDR_CR, reg_val);
#else
	reg_val = mctl_read_w(SDR_CR);
	reg_val |= (0x6<<12);
	reg_val |= 0xF00;	
	reg_val &= ~0x3;
	mctl_write_w(SDR_CR, reg_val);	
#endif

}

void mctl_itm_disable(void)
{
	__u32 reg_val = 0x0;

	reg_val = mctl_read_w(SDR_CCR);
	reg_val |= 0x1<<28;
	reg_val &= ~(0x1U<<31);    //danielwang, 2012/5/17
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
	mctl_write_w(SDR_DLLCR0, mctl_read_w(SDR_DLLCR0) & ~0x40000000 | 0x80000000);

	mctl_delay(0x100);

	mctl_write_w(SDR_DLLCR0, mctl_read_w(SDR_DLLCR0) & ~0xC0000000);

	mctl_delay(0x1000);

	mctl_write_w(SDR_DLLCR0, mctl_read_w(SDR_DLLCR0) & ~0x80000000 | 0x40000000);
	mctl_delay(0x1000);
}

void mctl_enable_dllx(void)
{
	__u32 i = 0;

	for(i=1; i<5; i++)
	{
		mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~0x40000000 | 0x80000000);
	}

	mctl_delay(0x100);

	for(i=1; i<5; i++)
	{
		mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~0xC0000000);
	}

	mctl_delay(0x1000);

	for(i=1; i<5; i++)
	{
		mctl_write_w(SDR_DLLCR0+(i<<2), mctl_read_w(SDR_DLLCR0+(i<<2)) & ~0x80000000 | 0x40000000);
	}
	mctl_delay(0x1000);
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
		0x00000301,0x00000301,0x0,       0x0,
		0x0,       0x0,       0x0,       0x0,
		0x0,       0x0,       0x0,       0x0,
		0x00001031,0x00001031,0x00000735,0x00001035,
		0x00001035,0x00000731,0x00001031,0x00000735,
		0x00001035,0x00001031,0x00000731,0x00001035,
		0x00001031,0x00000301,0x00000301,0x00000731,
	};

	for(i=0; i<32; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), hpcr_value[i]);
	}
}

void mctl_setup_dram_clock(__u32 clk)
{
    __u32 reg_val;

    //setup DRAM PLL
    reg_val = mctl_read_w(DRAM_CCM_SDRAM_PLL_REG);
    reg_val &= ~0x3;
    reg_val |= 0x1;                                             //m factor
    reg_val &= ~(0x3<<4);
    reg_val |= 0x1<<4;                                          //k factor
    reg_val &= ~(0x1f<<8);
    reg_val |= ((clk/24)&0x1f)<<8;                              //n factor
    reg_val &= ~(0x3<<16);
    reg_val |= 0x1<<16;                                         //p factor
    reg_val &= ~(0x1<<29);                                      //clock output disable
    reg_val |= (__u32)0x1<<31;                                  //PLL En
    mctl_write_w(DRAM_CCM_SDRAM_PLL_REG, reg_val);
	mctl_delay(0x100000);
    reg_val = mctl_read_w(DRAM_CCM_SDRAM_PLL_REG);
	reg_val |= 0x1<<29;
    mctl_write_w(DRAM_CCM_SDRAM_PLL_REG, reg_val);

    //reset GPS
    reg_val = mctl_read_w(DRAM_CCM_GPS_CLK_REG);
    reg_val &= ~0x3;
    mctl_write_w(DRAM_CCM_GPS_CLK_REG, reg_val);
    reg_val = mctl_read_w(DRAM_CCM_AHB_GATE_REG);
    reg_val |= (0x1<<26);
    mctl_write_w(DRAM_CCM_AHB_GATE_REG, reg_val);
	mctl_delay(0x100);                                      	//delay for a moment
    reg_val = mctl_read_w(DRAM_CCM_AHB_GATE_REG);
    reg_val &= ~(0x1<<26);
    mctl_write_w(DRAM_CCM_AHB_GATE_REG, reg_val);

    //open DRAMC AHB clock
    //close it first
    reg_val = mctl_read_w(DRAM_CCM_AHB_GATE_REG);
    reg_val &= ~(0x1<<14);
    mctl_write_w(DRAM_CCM_AHB_GATE_REG, reg_val);
	mctl_delay(0x1000);
    //then open it
    reg_val |= 0x1<<14;
    mctl_write_w(DRAM_CCM_AHB_GATE_REG, reg_val);
	mctl_delay(0x1000);
}

__u32 mctl_ahb_reset(void)
{
	__u32 reg_val;
	
	reg_val = mctl_read_w(0x01c20060);
	reg_val &=~(0x3<<14);
	mctl_write_w(0x01c20060,reg_val);
	
	mctl_delay(0x10);
	
	reg_val = mctl_read_w(0x01c20060);
	reg_val |=(0x3<<14);
	mctl_write_w(0x01c20060,reg_val);
	
	return 0;
}

__s32 DRAMC_init(__dram_para_t *para)
{
	__u32 reg_val;
	__s32 ret_val;
	__u32 hold_flag;

	//check input dram parameter structure
    if(!para)
	{
    	//dram parameter is invalid
    	return -1;
	}

	//memc reset
	mctl_ahb_reset();

	//get super standby flag
	hold_flag = mctl_read_w(SDR_DPCR)&0x1;

	//init dll
	mctl_enable_dll0();
	mctl_enable_dllx();
	
	mctl_delay(0x10);

	//memc clock on
	reg_val = mctl_read_w(SDR_CR);
	reg_val |= 0x1<<16;
	mctl_write_w(SDR_CR, reg_val);
	
	mctl_delay(0x10);

	//set DQS gate method
	reg_val = mctl_read_w(SDR_CCR);
	reg_val &= ~(0x1<<28);
	reg_val &= ~(0x1<<17);
	reg_val |= 0x1<<14;
	reg_val |= 0x1<<1;
	mctl_write_w(SDR_CCR, reg_val);

	//configure external DRAM
	reg_val = 0;
	if(MCTL_DDR_TYPE == 3)
		reg_val |= 0x1;
	reg_val |= (MCTL_IO_WIDTH>>3) <<1;
	if(MCTL_CHIP_SIZE == 256)
		reg_val |= 0x0<<3;
	else if(MCTL_CHIP_SIZE == 512)
		reg_val |= 0x1<<3;
	else if(MCTL_CHIP_SIZE == 1024)
		reg_val |= 0x2<<3;
	else if(MCTL_CHIP_SIZE == 2048)
		reg_val |= 0x3<<3;
	else if(MCTL_CHIP_SIZE == 4096)
		reg_val |= 0x4<<3;
	else if(MCTL_CHIP_SIZE == 8192)
		reg_val |= 0x5<<3;
	else
		reg_val |= 0x0<<3;
	reg_val |= ((MCTL_BUS_WIDTH>>3) - 1)<<6;
	reg_val |= (2 -1)<<10;
	reg_val |= 0x1<<12;
	reg_val |= ((1)&0x3)<<13;
	mctl_write_w(SDR_DCR, reg_val);

	//set refresh period
	DRAMC_set_autorefresh_cycle(para->dram_clk);

	//set timing parameters
	reg_val = 0x36d47793;
	mctl_write_w(SDR_TPR0, reg_val);
	reg_val = 0xa090;
	mctl_write_w(SDR_TPR1, reg_val);
	reg_val = 0x1aa00;
	mctl_write_w(SDR_TPR2, reg_val);

	//set mode register
	if(MCTL_DDR_TYPE==3)				//ddr3
	{
		reg_val = 0x0;
		reg_val |= (MCTL_CAS - 4)<<4;
		reg_val |= 0x5<<9;
	}
	else if(MCTL_DDR_TYPE==2)			//ddr2
	{
		reg_val = 0x2;
		reg_val |= MCTL_CAS<<4;
		reg_val |= 0x5<<9;
	}
	mctl_write_w(SDR_MR, reg_val);
	reg_val = 0x0;
	mctl_write_w(SDR_EMR, reg_val);
	reg_val = 0x8;
	mctl_write_w(SDR_EMR2, reg_val);
	reg_val = 0x0;
	mctl_write_w(SDR_EMR3, reg_val);

	//start memc ZQ process
	{
		reg_val = 0x87b00000;
		mctl_write_w(SDR_ZQCR0, reg_val);
		while( !(mctl_read_w(SDR_ZQSR) & (0x1U<<31))) {};
	}

	//release reset
	mctl_ddr3_reset();

	//setup pad mode
	reg_val = (0x7<<24)|(0x7<<20)|(0x1<<16)|(0x1<<12)|(0x7<<8)|(0xf<<2);
	mctl_write_w(SDR_CR, reg_val);

	//start init external dram
	reg_val = mctl_read_w(SDR_CCR);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_CCR, reg_val);
	while(mctl_read_w(SDR_CCR) & (0x1U<<31)) {};

	ret_val = DRAMC_scan_readpipe();

	return DRAMC_get_dram_size();
}

__s32 DRAMC_init_EX(__dram_para_t *para)
{
	__u32 i = 0;
	__s32 ret_val;
	
	for(i=0; i<3; i++)
	{
	  ret_val = DRAMC_init(para);
	  if(ret_val) break;
	}
	
	return ret_val;
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
#ifndef FPGA_51_TEST
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
#else
__s32 DRAMC_scan_readpipe(void)
{
	__u32 reg_val;
	
	reg_val = 0x9;//0x8;
	//if(MCTL_CS_NUM==2)
	reg_val |= 0x1<<9;
	mctl_write_w(SDR_APR, reg_val);	

	return (0);	
}

#endif


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

    reg_val = mctl_read_w(DRAM_CCM_SDRAM_CLK_REG);

    if(on)
        reg_val |= 0x1<<15;
    else
        reg_val &= ~(0x1<<15);

    mctl_write_w(DRAM_CCM_SDRAM_CLK_REG, reg_val);
}
/*
*********************************************************************************************************
* Description: Set autorefresh cycle
*
* Arguments  : clock value in MHz unit
*
* Returns    : none
*
* Note       :
*********************************************************************************************************
*/
void DRAMC_set_autorefresh_cycle(__u32 clk)
{
	__u32 reg_val;
	__u32 tmp_val;
	
	if(MCTL_CHIP_SIZE<=1024)		
		tmp_val = (131*clk)>>10;
	else
		tmp_val = (336*clk)>>10;
	reg_val = tmp_val;					
	tmp_val = (7987*clk)>>10;
	tmp_val = tmp_val*9 - 200;
	reg_val |= tmp_val<<8;
	reg_val |= 0x8<<24;
	mctl_write_w(SDR_DRR, reg_val);

}


/*
**********************************************************************************************************************
*                                               GET DRAM SIZE
*
* Description: Get DRAM Size in MB unit;
*
* Arguments  : None
*
* Returns    : 32/64/128
*
* Notes      :
*
**********************************************************************************************************************
*/
__u32 DRAMC_get_dram_size(void)
{
    __u32 reg_val;
    __u32 dram_size;
    __u32 chip_den;

    reg_val = mctl_read_w(SDR_DCR);
    chip_den = (reg_val>>3)&0x7;
    if(chip_den == 0)
    	dram_size = 32;
    else if(chip_den == 1)
    	dram_size = 64;
    else if(chip_den == 2)
    	dram_size = 128;
    else if(chip_den == 3)
    	dram_size = 256;
    else if(chip_den == 4)
    	dram_size = 512;
    else
    	dram_size = 1024;

    if( ((reg_val>>1)&0x3) == 0x1)
    	dram_size<<=1;
    if( ((reg_val>>6)&0x7) == 0x3)
    	dram_size<<=1;
    if( ((reg_val>>10)&0x3) == 0x1)
    	dram_size<<=1;

    return dram_size;
}

#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
__s32 DRAMC_to_card_init(__dram_para_t *para)
{
	__u32 i, m;
	__u32 dram_den; //half_den;
	__u32 err_flag;
	__u32 start_adr, end_adr;

	//try 16/32 bus width
	para->dram_type = 3;
	para->dram_rank_num = 1;
	para->dram_chip_density = 4096;
	para->dram_io_width	= 16;
	para->dram_bus_width = 32;
	if(!DRAMC_init_EX(para))
	{
		para->dram_bus_width = 16;
	}

	//try 1/2 rank
	para->dram_rank_num = 2;
	if(!DRAMC_init_EX(para))
	{
		para->dram_rank_num = 1;
	}
	//try 2048/4096 chip density
	para->dram_chip_density = 4096;
	dram_den = para->dram_chip_density<<17;
	if(para->dram_rank_num == 2)
		dram_den<<=1;
	if(para->dram_bus_width == 32)
		dram_den<<=1;
//	half_den = dram_den>>1;
	if(!DRAMC_init_EX(para))
	{
		return 0;
	}
	//write preset value at special address
	start_adr = 0x40000000;
	end_adr = 0x40000000 + dram_den;
	for(m = start_adr; m< end_adr; m+=0x4000000)
	{
		for(i=0; i<16; i++)
		{
			*((__u32 *)(m + i*4)) = m + i;
		}
	}
	//read and check value at special address
	err_flag = 0;
	for(m = start_adr; m< end_adr; m+=0x4000000)
	{
		for(i=0; i<16; i++)
		{
			if( *((__u32 *)(m + i*4)) != (m + i) )
			{
				err_flag = 1;
				break;
			}

		}
	}
	if(!err_flag)
	{
	    para->dram_size = DRAMC_get_dram_size();

		return para->dram_size;
	}

	para->dram_chip_density = 2048;
	if(!DRAMC_init_EX(para))
	{
		return 0;
	}

	para->dram_size = DRAMC_get_dram_size();

	return (para->dram_size);
}
#endif

__s32 init_DRAM(int type)
{
	__u32 i;
	__s32 ret_val;
	boot_dram_para_t  boot0_para;

	get_boot0_dram_para( &boot0_para );
	if(boot0_para.dram_clk > 2000)
	{
		boot0_para.dram_clk /= 1000000;
	}

	ret_val = DRAMC_init( &boot0_para );

/*
	ret_val = 0;
	i = 0;
	while( (ret_val == 0) && (i<4) )
	{
#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND_FLASH
		ret_val = DRAMC_init( &boot0_para );
#elif SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
		if(!type)
		{
			ret_val = DRAMC_to_card_init( &boot0_para );
		}
		else
		{
			ret_val = DRAMC_init( &boot0_para );
		}
		set_boot0_dram_para( &boot0_para );
#else
#error  The storage media of Boot1 has not been defined.
#endif

		i++;
	}
*/

	return ret_val;
}

#pragma arm section


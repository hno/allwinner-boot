/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : Boot0_C_part.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.12.18
*
* Description :
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.11.09      1.1.0        build the file
*
* Gary.Wang       2007.12.18      1.1.0        remove "BT0_self_rcv"
*
************************************************************************************************************************
*/
#include "boot0_i.h"

#include <string.h>

extern const boot0_file_head_t  BT0_head;

__u32 odt_status;

//static void move_RW( void );
static void clear_ZI( void );
static void timer_init(void);
static void print_version(void);
static __u32 check_odt(int ms);
static void bias_calibration(void);
static void pll_reset( void );
void __msdelay(__u32 ms);
//void dram_para_display(void);
/*******************************************************************************
*函数名称: Boot0_C_part
*函数原型：void Boot0_C_part( void )
*函数功能: Boot0中用C语言编写的部分的主流程
*入口参数: void
*返 回 值: void
*备    注:
*******************************************************************************/
void Boot0_C_part( void )
{
	__u32 status;
	__s32 dram_size;
	int	index = 0;
	int   ddr_aotu_scan = 0;
	volatile unsigned int *reg_addr = 0;

//	move_RW( );
	clear_ZI( );

	bias_calibration();

    timer_init();
    UART_open( BT0_head.prvt_head.uart_port, (void *)BT0_head.prvt_head.uart_ctrl, 24*1000*1000 );
	//odt_status = check_odt(5);
    if( BT0_head.prvt_head.enable_jtag )
    {
		jtag_init( (normal_gpio_cfg *)BT0_head.prvt_head.jtag_gpio );
    }
	msg("HELLO! BOOT0 is starting!\n");
	print_version();

	{
		__u32 reg_val;
		__u32 fel_flag;

		fel_flag = *(volatile unsigned int *)(0x01f00000 + 0x108);
		//print smp status.
		index = 0;
		while(index < 0x18)
		{
			reg_addr = (volatile unsigned int *)(0x01f00000 + 0x100 + index);
			reg_val = *reg_addr;
	    		*reg_addr = 0;
	    		msg("reg_addr %x =%x\n", reg_addr, reg_val);
	    		index+=0x4;
		}


//		reg_val = *(volatile unsigned int *)(0x01f00000 + 0x108);
//		*(volatile unsigned int *)(0x01f00000 + 0x108) = 0;
//		msg("fel_flag=%x\n", fel_flag);
		if(fel_flag == 0x5AA5A55A)
		{
			msg("eraly jump fel\n");

			pll_reset();
			__msdelay(10);

			jump_to( FEL_BASE );
		}

	}

	mmu_system_init(EGON2_DRAM_BASE, 1 * 1024, EGON2_MMU_BASE);
	mmu_enable();

	//dram_size = init_DRAM(BT0_head.boot_head.platform[7]);                                // 初始化DRAM
//#ifdef  CONFIG_SUN6I_FPGA
//	ddr_aotu_scan = 1;
//	msg("config fpga\n");
//#else
//	ddr_aotu_scan = BT0_head.boot_head.platform[7];
//	msg("not config fpga\n");
//#endif
    ddr_aotu_scan = 0;
#ifdef DEBUG
	{
		int k;

		for(k=0;k<16;k++)
		{
			msg("%x\n", BT0_head.prvt_head.dram_para[k]);
		}
	}
#endif
//	msg("------------before------------\n");
//	dram_para_display();

	dram_size = init_DRAM(ddr_aotu_scan, (void *)BT0_head.prvt_head.dram_para);
	if(dram_size)
	{
		mdfs_save_value((void *)BT0_head.prvt_head.dram_para);
		msg("dram size =%d\n", dram_size);
	}
	else
	{
		msg("initializing SDRAM Fail.\n");
		mmu_disable( );

		pll_reset();
		__msdelay(10);

		jump_to( FEL_BASE );
	}

//	{
//		__u32 reg_val;
//
//		reg_val = *(volatile __u32 *)(0x1c20d20);
//		*(volatile __u32 *)(0x1c20d20) = 0;
//		msg("reg_val=%x, %x\n", reg_val, *(volatile __u32 *)(0x1c20d24));
//		if(reg_val & 0x01)
//		{
//			mmu_disable( );
//			jump_to( 0x40100000 );
//		}
//	}
//	msg("------------end------------\n");
//	dram_para_display();
	#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND_FLASH
		status = load_Boot1_from_nand( );         // 载入Boot1
	#elif SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SPI_NOR_FLASH
		status = load_boot1_from_spinor( );         // 载入Boot1
	#elif SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
		status = load_boot1_from_sdmmc( (char *)BT0_head.prvt_head.storage_data );  // 载入boot1
	#else
		#error The storage media of Boot1 has not been defined.
	#endif


	msg("Ready to disable icache.\n");

	mmu_disable( );                               // disable instruction cache

	if( status == OK )
	{
//		restart_watch_dog( );                     // restart watch dog
		//跳转boot1之前，把dram的大小写进去
		//set_dram_size(dram_size );
		//跳转之前，把所有的dram参数写到boot1中
		set_dram_para((void *)&BT0_head.prvt_head.dram_para, dram_size);
		msg("Succeed in loading Boot1.\n"
		    "Jump to Boot1.\n");
		jump_to( BOOT1_BASE );                    // 如果载入Boot1成功，跳转到Boot1处执行
	}
	else
	{
//		disable_watch_dog( );                     // disable watch dog

		msg("Fail in loading Boot1.\n"
		    "Jump to Fel.\n");

		pll_reset();
		__msdelay(10);

		jump_to( FEL_BASE );                      // 如果载入Boot1失败，将控制权交给Fel
	}
}
/*******************************************************************************
*函数名称: set_pll
*函数原型：void set_pll( void )
*函数功能: Boot0中用C语言编写的 调整CPU频率
*入口参数: void
*返 回 值: void
*备    注:
*******************************************************************************/
void set_pll( void )
{
	__u32 reg_val;

	//切换到24M，设置AXI分频为2
	CCMU_REG_AXI_MOD = 0x00010001;
	//设置PLL1到408M
	reg_val = (0x00011011) | (0x80000000);
	CCMU_REG_PLL1_CTRL = reg_val;
	//等待lock
#ifndef CONFIG_SUN6I_FPGA
	do
	{
		reg_val = CCMU_REG_PLL1_CTRL;
	}
	while(!(reg_val & (0x1 << 28)));
#endif
	//设置CPU:AXI:AHB:APB分频 4:2:2:1
	CCMU_REG_AHB1_APB1 = 0x02 << 12;
	//设置ATP分频为2
	reg_val = CCMU_REG_AXI_MOD;
	reg_val &= ~(3 << 8);
	reg_val |=  (1 << 8);
	CCMU_REG_AXI_MOD = reg_val;
	//切换到PLL1
	reg_val = CCMU_REG_AXI_MOD;
	reg_val &= ~(3 << 16);
	reg_val |=  (2 << 16);
	CCMU_REG_AXI_MOD = reg_val;
	//打开DMA
	*(volatile unsigned int *)(0x01c20000 + 0x60)  |= 1 << 6;
	*(volatile unsigned int *)(0x01c20000 + 0x2C0) |= 1 << 6;

	return ;
}

static void pll_reset( void )
{
	CCMU_REG_AXI_MOD = 0x00010000;
	CCMU_REG_PLL1_CTRL = 0x00001000;
	CCMU_REG_AHB1_APB1 = 0x00001010;

}

static __u32 check_odt(int ms)
{
#if 0
	__u32 value, tmp_cfg, tmp_pull;
	__u32 t1, t2;

	value = *(volatile __u32 *)(0x01c20828);
	tmp_cfg = value;

	value &= ~(0x07 << 24);
	*(volatile __u32 *)(0x01c20828) = value;

	value = *(volatile __u32 *)(0x01c20840);
	tmp_pull = value;
	value &= ~(0x03 << 28);
	value |=  (0x01 << 28);
	*(volatile __u32 *)(0x01c20840) = value;

	t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	t2 = t1 + ms;
	do
	{
		t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	}
	while(t2 >= t1);

	value = (*(volatile __u32 *)(0x01c20834)) & (1 << 14);

	*(volatile __u32 *)(0x01c20840) = tmp_pull;
	*(volatile __u32 *)(0x01c20828) = tmp_cfg;

	return value;
#endif
	return 0;
}

static void timer_init(void)
{
	*(volatile unsigned int *)(0x01c20000 + 0x144) |= (1U << 31);
	*(volatile unsigned int *)(0x01c20C00 + 0x80 )  = 1;
	*(volatile unsigned int *)(0x01c20C00 + 0x8C )  = 0x2EE0;
	*(volatile unsigned int *)(0x01c20C00 + 0x84 )  = 0;
}

static void print_version(void)
{
	msg("boot0 version : %s\n", BT0_head.boot_head.platform + 2);

	return;
}

//static void move_RW( void )
//{
//	__u8    *psrc8;
//	__u8    *pdst8;
//	__u32   *psrc32;
//	__u32   *pdst32;
//	__u32   size;
//	__u32   N;
//
//	extern unsigned char Load$$Boot0_RW_ZI$$Base;
//	extern unsigned char Image$$Boot0_RW_ZI$$Base;
//	extern unsigned char Image$$Boot0_RW_ZI$$Length;
//
//	size = (__u32) &Image$$Boot0_RW_ZI$$Length;
//	psrc32  = (__u32 *)&Load$$Boot0_RW_ZI$$Base;
//	pdst32  = (__u32 *)&Image$$Boot0_RW_ZI$$Base;
//
//	N = size >> 4;
//	while( N-- > 0 )
//	{
//		*pdst32++ = *psrc32++;
//		*pdst32++ = *psrc32++;
//		*pdst32++ = *psrc32++;
//		*pdst32++ = *psrc32++;
//	}
//
//	N = size & ( ( 1 << 4 ) - 1 );
//	psrc8 = (__u8 *)psrc32;
//	pdst8 = (__u8 *)pdst32;
//	while( N--)
//	{
//		*pdst8++ = *psrc8++;
//	}
//}



static void clear_ZI( void )
{
//	__u8  *p8;
	__u32 *p32;
	__u32 size;
//	__u32 N;

	extern unsigned char Image$$Boot0_RW_ZI$$ZI$$Base;
	extern unsigned char Image$$Boot0_RW_ZI$$ZI$$Length;

	size = (__u32)  &Image$$Boot0_RW_ZI$$ZI$$Length;
	p32  = (__u32 *)&Image$$Boot0_RW_ZI$$ZI$$Base;

	memset(p32, 0, size);
//	N = size >> 4;
//	while( N-- > 0 )
//	{
//		*p32++ = 0;
//		*p32++ = 0;
//		*p32++ = 0;
//		*p32++ = 0;
//	}
//
//	N = size & ( ( 1 << 4 ) - 1 );
//	p8 = (__u8 *)p32;
//	while( N--)
//	{
//		*p8++ = 0;
//	}
}

static void bias_calibration(void)
{
	//open codec apb gate
	*(volatile unsigned int *)(0x1c20000 + 0x68) |= 1;
	//disable codec soft reset
	*(volatile unsigned int *)(0x1c20000 + 0x2D0) |= 1;
	//enable HBIASADCEN
	*(volatile unsigned int *)(0x1c22C00 + 0x28) |= (1 << 29);
}

void disbale_cpus(void)
{
	//disable watchdog
	*(volatile unsigned int *)(0x01f01000 + 0x00) = 0;
	*(volatile unsigned int *)(0x01f01000 + 0x04) = 1;
	*(volatile unsigned int *)(0x01f01000 + 0x18) &= ~1;
	//assert cups
	*(volatile unsigned int *)(0x01f01C00 + 0x00) = 0;
	//disable cpus module gating
	*(volatile unsigned int *)(0x01f01400 + 0x28) = 0;
	//disable cpus module assert
	*(volatile unsigned int *)(0x01f01400 + 0xb0) = 0;
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
void __msdelay(__u32 ms)
{
	__u32 t1, t2;

	t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	t2 = t1 + ms;
	do
	{
		t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	}
	while(t2 >= t1);

	return ;
}

void config_pll1_para(void)
{
	volatile unsigned int value;

	//by sunny at 2013-1-20 17:53:21.
	value = *(volatile unsigned int *)(0x1c20250);
	value &= ~(1 << 26);
	value |= (1 << 26);
	value &= ~(0x7 << 23);
	value |= (0x7 << 23);
	*(volatile unsigned int *)(0x1c20250) = value;

	value = *(volatile unsigned int *)(0x1c20220);
	value &= ~(0xf << 24);
	value |= (0xf << 24);
	*(volatile unsigned int *)(0x1c20220) = value;
}

//void dram_para_display(void)
//{
//	int i;
//	__u32 *value = (void *)BT0_head.prvt_head.dram_para;
//
//	for(i=0;i<32;i++)
//	{
//		msg("para %d value %x\n", i, value[i]);
//	}
//
//	return ;
//}

void  set_vldo_for_pll(void)
{
	volatile unsigned int reg_val;
	/* set voltage and ldo for pll */

	reg_val = *(volatile unsigned int *)(0X01F01400+0x44);
	reg_val &= ~(0xffU << 24);
  	reg_val |= 0xa7U << 24;
  	*(volatile unsigned int *)(0X01F01400+0x44) = reg_val;

	reg_val = *(volatile unsigned int *)(0X01F01400+0x44);
	reg_val &= ~(0x1 << 15);
	reg_val &= ~(0x7 << 16);
	reg_val |= 0x7 << 16;
	reg_val |= 0xa7U << 24;
  	*(volatile unsigned int *)(0X01F01400+0x44) = reg_val;

	return ;
}

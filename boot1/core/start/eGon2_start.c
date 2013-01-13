/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON start sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : eGon2_start.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "include.h"
#include "arm_base.h"


static void  print_version(void);
static __s32 reserved_init(void);
static __s32 reserved_exit(void);
static void eGon2_storage_type_set(void);
/*******************************************************************************
*函数名称: eGon2_start
*函数原型：void Boot1_C_part( void )
*函数功能: Boot1中用C语言编写的部分的主流程
*入口参数: void
*返 回 值: void
*备    注:
*******************************************************************************/
void eGon2_start( void )
{
	__s32   i;
	__u32   default_clock;
	__s32   exception;
	__u32   boot_heap_base;
	__s32   force_to_card0 = 0;
	void    *script_buf = NULL;
	H_FILE  hfile = NULL;
	FS_PART_OPTS_t   fs_ops;

	/* init enviroment for running app */
//	move_RW( );
	reposition_boot_standby();
	clear_ZI( );

    // 做两次调频，第一次先到384M ???
    //default_clock = eGon2_clock_set(0, 240);
    default_clock = 384;
    //起始地址在0x4X400000，保证堆起始地址在0x4Y000000(Y = X + 1)
    boot_heap_base = BOOT1_BASE + 0x01200000;
    //设置堆的大小为16M
	eGon2_create_heap( boot_heap_base, SZ_16M );

	/* use mmu */
	mmu_system_init(EGON2_DRAM_BASE, 4 * 1024, EGON2_MMU_BASE);                 // init mmu
	mmu_enable( );                      // enable mmu

	eGon2_timer_init( );				// timer 初始化
	eGon2_Int_Init( );                  // 对中断系统进行初始化
	reposition_arm_start( );            // reposition vect table
	set_vect_low_addr( );               // set interrupt vector low address
    open_sys_int( );                    // open system interrupt

	//初始化UART
    serial_init(BT1_head.prvt_head.uart_port, (normal_gpio_cfg *)BT1_head.prvt_head.uart_ctrl, 115200, 24000000);
    print_version();
    //初始化IIC, 现在还没有调整过频率，运行在384M
    eGon2_twi_init(BT1_head.prvt_head.twi_port, (normal_gpio_cfg *)BT1_head.prvt_head.twi_ctrl, 24000000, 400000);
#ifndef CONFIG_SUN7I_FPGA
    //初始化POWER，调整核心电压
    if(!eGon2_power_init((void *)&BT1_head.prvt_head.core_para))
    {
        //开始调整频率，电压已经调整完毕
        if(default_clock != BT1_head.prvt_head.core_para.user_set_clock)
        {
        	default_clock = eGon2_clock_set_ext(BT1_head.prvt_head.core_para.user_set_clock, BT1_head.prvt_head.core_para.user_set_core_vol);
    		//default_clock = eGon2_clock_set_ext(k, BT1_head.prvt_head.core_para.user_set_core_vol);
    		eGon2_printf("set dcdc2=%d, clock=%d successed\n", BT1_head.prvt_head.core_para.user_set_core_vol, default_clock);
        }
        else
        {
        	eGon2_printf("set default clock=384\n");
        }
    }
    else
    {
    	eGon2_printf("set dcdc2 failed, set default clock 384M\n");
    }
#else
    eGon2_power_init((void *)&BT1_head.prvt_head.core_para);
    eGon2_printf("set pll1 %d\n", BT1_head.prvt_head.core_para.user_set_clock);
    default_clock = eGon2_clock_set_ext(BT1_head.prvt_head.core_para.user_set_clock, BT1_head.prvt_head.core_para.user_set_core_vol);
    eGon2_printf("set dcdc2=%d, clock=%d successed\n", BT1_head.prvt_head.core_para.user_set_core_vol, default_clock);
#endif

    eGon2_key_init();
    //检查是否需要直接进入fel，通常用于异常出现的情况
    exception = eGon2_boot_detect();
    if(!exception)
    {
        eGon2_printf("key found, jump to fel\n");

        eGon2_simple_jump_Fel();
    }
    else if(exception > 0)
    {
		boot1_file_head_t  *boot1_buf;

		boot1_buf = (boot1_file_head_t *)BOOT1_BASE;
    	boot1_buf->prvt_head.uart_port = exception;
    }
    else if(exception == -2)
    {
    	eGon2_printf("key found, try to debug mode\n");

    	force_to_card0 = 1;
    }
	eGon2_printf("flash init start\n");
    if(!eGon2_block_device_init())
    {
    	eGon2_printf("flash init finish\n");
    }
    else
    {
    	eGon2_printf("flash init failed\n");

    	eGon2_jump_Fel( );
    }
    fs_ops.Write = eGon2_block_device_write;
    fs_ops.Read  = eGon2_block_device_read;
    fs_ops.Init  = reserved_init;
    fs_ops.Exit  = reserved_exit;

    FS_regpartopts(&fs_ops);
    if(FS_init())
    {
    	eGon2_printf("fs init fail, jump to fel\n");

    	eGon2_jump_Fel( );
    }
    eGon2_printf("fs init ok\n");
    if(FSMount('c'))
    {
    	eGon2_printf("fs mount fail, jump to fel\n");

    	eGon2_jump_Fel( );
    }
    eGon2_printf("fs mount ok\n");
	//加载配置文件
	script_buf = (void *)SCRIPT_BASE;
	hfile = FS_fopen("c:\\script.bin", "r");
	if(!hfile)
	{
		hfile = FS_fopen("c:\\script0.bin", "r");
	}
	if(hfile)
	{
		__u32 length;

		length = FS_filelen(hfile);
		FS_fread(script_buf, length, 1, hfile);
		FS_fclose(hfile);
		eGon2_script_parser_init((char *)script_buf);
	}
	else
	{
		eGon2_printf("unable to open script file, check it carefully\n");
	}
	eGon2_printf("script finish\n");
#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND
//	eGon2_dynamic();
//	{
//		char  buf[32];
//
//		eGon2_printf("test for dynamic\n");
//		if(!eGon2_script_parser_fetch("dynamic", "MAC", (int *)buf, 32/4))
//		{
//			eGon2_printf("mac addr= %s\n", buf);
//		}
//	}
#endif
	//设置电压
	eGon2_set_power_on_vol();
	eGon2_power_set_vol();
	eGon2_config_charge_current(0);
	//设置nand参数到脚本中
	eGon2_printf("power finish\n");
//	{
//		while((*(int *)(0x40000000)) != 0x55);
//	}
#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND
	eGon2_block_ratio();
	eGon2_storage_type_set();
#endif
	if(force_to_card0 == 1)
	{
		eGon2_force_to_debug();
	}
    {
    	char  *str_pointer_array[1];
#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND
		char  str_array0[32] = "c:\\boot.axf";

		str_pointer_array[0] = str_array0;
#elif SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
		char  str_array0[32] = "c:\\boot.axf";
		char  str_array1[32] = "c:\\sprite.axf";

		str_pointer_array[0] = str_array0;

		if(!BT1_head.boot_head.platform[7])
		{
			str_pointer_array[0] = str_array1;
		}
		else
		{
			eGon2_storage_type_set();
		}
#endif
        eGon2_run_app(1, str_pointer_array);
    }

    for(;;)
    {
        //eGon2_printf("wait here\n");
        for(i=0;i<10000;i++);
    }
}

static void print_version(void)
{
	eGon2_printf("boot1 version : %s\n", BT1_head.boot_head.platform);

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
static __s32 reserved_init(void)
{
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
static __s32 reserved_exit(void)
{
    return 0;
}
static void eGon2_storage_type_set(void)
{
	boot_file_head_t  *bfh;
	int  type;

	bfh = (boot_file_head_t *)BOOT1_BASE;
	type = bfh->eGON_vsn[2];

	if(!eGon2_script_parser_patch("target", "storage_type", type))
	{
		eGon2_printf("storage_type=%d\n", type);
	}

	return ;
}

/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : BootMain.c
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
#include "BootOS.h"
#include "Board.h"
#include "common_res.h"
#include "boot_img.h"
#include "boot0_v2.h"
#include <math.h>

boot1_private_head_t  boot1_priv_para;
boot_hardware_res     board_res;
__u32				  layer_hd;
__u32                 disp_hd;
int boot_recovery_status = 0;

extern  __u32 usb_pc_type;

static  __s32 key_value_test(void);
static int __detect_card_mode(void);
/*
*******************************************************************************
*                     BootMain
*
* Description:
*    BOOT主应用程序
*
* Parameters:
*    void
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
int BootMain(int argc, char **argv)
{
	__u32                 kernal_addr;
	__u32				  para_addr;
	__s32                 ret;
    __s32                 logo_status = 0;
//    boot_global_info_t   *global_info;
//    char				  product[64];

//	DMSG_INFO("BootMain start\n");
	while( 1 )
	{
		char ch;

		ch = wBoot_getc_delay( 1 );  // 27000000
	    __inf("%d\n", ch);
	    if( ch == '2' )
	    {
	    	__inf("Jump to fel\n");
			{
				__s32 tick;

				for(tick = 10000; tick>0; tick --);
			}
	    	wBoot_jump_fel( );
	    }
	    else if(ch == '-')
	    {
	        __u32 dbg = 0x55;

            __inf("for debug\n");
	        while(dbg == 0x55);
	        break;
	    }
	    else if(ch == '1')
	    {
	    	usb_start(0);
    		usb_run();
    		break;
	    }
	    else if(ch == '3')
	    {
            key_value_test();
	        break;
	    }
	    else
	    {
			break;
	    }
	}

	//while((*(volatile __u32 *)0x40000000) != 0x12345678);
	//数据初始化
	memset(&board_res, 0, sizeof(boot_hardware_res));
	memset(&boot1_priv_para, 0, sizeof(boot1_private_head_t));
	//获取参数
	ret = wBoot_get_para(WBOOT_PARA_GLOBAL, &boot1_priv_para);
	if(ret != 0)
	{
		DMSG_PANIC("ERR: wBoot_get_para failed\n");
		goto jump_to_parameters_fail;
	}
    //填充自定义数据
    //strcpy(board_res.fel_file_name, OS_FOR_USB_UPDATE_FILE_NAME);
    //检查，用户是否有进入fel的需求
    //首先检查文件形式升级
//    if(!check_file_to_fel(board_res.fel_file_name))
//    {
//        __inf("jump to fel because of update file found\n");
//        goto jump_to_fel;
//    }
//    //如果是量产完成，直接关机
//    if(!check_natch_time("c:\\natch.ini", boot1_priv_para.work_mode))
//    {
//        goto jump_to_power_off;
//    }
    //power_set_init();
//    memset(product, 0, 64);
//    ret = wBoot_script_parser_fetch("target", "product", (int *)product, 64/4);
//    if(!ret)
//    {
//    	if(!strcmp(product, "homlet"))
//    	{
//	    	user_gpio_set_t     *gpio_init;
//	    	__u32			    gpio_count;
//
//			__inf("gpio config\n");
//	    	gpio_count = wBoot_script_parser_mainkey_get_gpio_count("gpio_init");
//	    	if(gpio_count)
//	    	{
//	    		gpio_init = wBoot_malloc(gpio_count * sizeof(user_gpio_set_t));
//	    		if(gpio_init)
//	    		{
//	    			wBoot_script_parser_mainkey_get_gpio_cfg("gpio_init", (void *)gpio_init, gpio_count);
//	    			wBoot_GPIO_Request(gpio_init, gpio_count);
//	    			__inf("gpio ok\n");
//	    		}
//	    	}
//	    	__inf("gpio finish\n");
//	    }
//	}
#if 0
	{
		user_gpio_set_t     gpio_recovery;
	    __u32			    gpio_hd = NULL;
	    int					gpio_value;
		ret = wBoot_script_parser_fetch("system", "recovery_key", (int *)&gpio_recovery, sizeof(user_gpio_set_t)/4);
		if(!ret)
		{
			gpio_recovery.mul_sel = 0;		//强制设置成输入
			gpio_hd = wBoot_GPIO_Request(&gpio_recovery, 1);
			if(gpio_hd)
			{
				int k;

				gpio_value = 0;
				for(k=0;k<4;k++)
				{
					gpio_value += wBoot_GPIO_Read_One_PIN_Value(gpio_hd, 0);
					wBoot_timer_delay(25);
				}
			}
			if(!gpio_value)
			{
				__inf("set to recovery\n");
				boot_recovery_status = 1;
			}
		}
	}
#endif
	{
		__inf("try card mode\n");
		__detect_card_mode();
	}
	{
		user_gpio_set_t     gpio_recovery[2];
		int ret1, ret2;

		ret1 = wBoot_script_parser_fetch("uart_para", "uart_debug_tx", (int *)&gpio_recovery[0], sizeof(user_gpio_set_t)/4);
		ret2 = wBoot_script_parser_fetch("uart_para", "uart_debug_rx", (int *)&gpio_recovery[1], sizeof(user_gpio_set_t)/4);
		__inf("ret1=%d,ret2=%d\n",ret1,ret2);
		if((!ret1) && (!ret2))
		{
			__inf("all gpio get ok\n");
			__inf("%x\n", wBoot_GPIO_Request(gpio_recovery, 2));
		}
	}
	{
		user_gpio_set_t     gpio_recovery[4];
		int ret[4];

		ret[0] = wBoot_script_parser_fetch("jtag_para", "jtag_ms", (int *)&gpio_recovery[0], sizeof(user_gpio_set_t)/4);
		ret[1] = wBoot_script_parser_fetch("jtag_para", "jtag_ck", (int *)&gpio_recovery[1], sizeof(user_gpio_set_t)/4);
		ret[2] = wBoot_script_parser_fetch("jtag_para", "jtag_do", (int *)&gpio_recovery[2], sizeof(user_gpio_set_t)/4);
		ret[3] = wBoot_script_parser_fetch("jtag_para", "jtag_di", (int *)&gpio_recovery[3], sizeof(user_gpio_set_t)/4);

		if((!ret[0]) && (!ret[1]) && (!ret[2]) && (!ret[3]))
		{
			__inf("all jtag gpio get ok\n");
			__inf("%x\n", wBoot_GPIO_Request(gpio_recovery, 4));
		}
	}
//	__inf("init to usb pc\n");
	//power_set_usbpc();
    //初始化显示设备
    //BoardInit_Display(global_info->display_device, global_info->display_mode);
    BoardInit_Display(0, 0);
    //开始准备系统数据
    //检测电压状态
    if(check_power_status())
    {
    	ret = -1;
    }
    else
    {
    	boot_show_logo("c:\\os_show\\bootlogo.bmp", 1, BOOT_LOGO_MEM_BASE);
    	//power_int_reg();	//启用中断检测vbus
    	if(BootOS_detect_os_type(&para_addr, &kernal_addr, 0, &logo_status))
    	{
    		ret = -2;
    	}
    }
	BoardExit(0, ret);

    if(!ret)
    {
        BootOS(para_addr, kernal_addr);
    }
    else if(ret == -1)
    {
    	goto jump_to_power_off;
    }
    else if(ret == -2)
    {
    	goto jump_to_fel;
    }

jump_to_fel:
	__inf("try to fel in 1 secend\n");
	wBoot_timer_delay(50);
    wBoot_jump_fel();

	//显示一个提示信息，等待用户自己处理
jump_to_parameters_fail:

jump_to_power_off:
    __inf("try to power off\n");
    wBoot_power_set_off();

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
static  __s32 key_value_test(void)
{
	__s32 key_value;

	__inf("welcome to key value test\n");
	__inf("press any key, and the value would be printed\n");
	__inf("press power key to exit\n\n");
	while(1)
	{
		key_value = wBoot_key_get_value();
		if(key_value > 0)
			__inf("key value = %d\n", key_value);
		key_value = 0;
	}
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
#define  BOOT0_MAX_SIZE  (32 * 1024)

static int __detect_card_mode(void)
{
	int ret;
	char  buffer[BOOT0_MAX_SIZE];
	boot0_file_head_t  *boot0_head;
	__u32 src_sum, cal_sum;
	boot_file_head_t  *bfh = (boot_file_head_t *)BOOT1_BASE;
	int  storage_type = bfh->eGON_vsn[2];

	__inf("storage_type=%d\n",storage_type);
	if(storage_type == 1)
	{
		return -1;
	}

	ret = SDMMC_LogicalInit(0, 20 * 1024 * 1024/512, 4); //初始化不成功
	if(ret < 0)
	{
		__msg("card is not exist\n");

		return -1;
	}
	memset(buffer, 0, BOOT0_MAX_SIZE);
	ret = SDMMC_LogicalRead1(16, BOOT0_MAX_SIZE/512, buffer, 0);
	if(ret < 0)
	{
		__msg("read card boot0 failed\n");

		return -1;
	}

	boot0_head = (boot0_file_head_t *)buffer;
	__msg("boot0 magic = %s\n", boot0_head->boot_head.magic);
	if(strncmp((const char *)boot0_head->boot_head.magic, BOOT0_MAGIC, 8))
	{
		__msg("boot0 magic invalid\n");

		return -1;
	}

	src_sum = boot0_head->boot_head.check_sum;
	boot0_head->boot_head.check_sum = STAMP_VALUE;
	cal_sum = verify_sum(buffer, boot0_head->boot_head.length);
	if(src_sum != cal_sum)
	{
		__msg("boot0 addsum error\n");

		return -1;
	}

	wBoot_timer_delay(50);
    wBoot_jump_fel();

	return 0;
}

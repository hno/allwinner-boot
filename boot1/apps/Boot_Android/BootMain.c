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
#include "BootOS.h"
#include "Board.h"
#include "common_res.h"
#include "boot_img.h"

boot1_private_head_t  boot1_priv_para;
boot_hardware_res     board_res;
int boot_recovery_status = 0;

extern  __u32 usb_pc_type;

static  __s32 key_value_test(void);
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
    boot_global_info_t   *global_info;
//    char				  product[64];
#ifndef SPEED_UP_BOOT
	DMSG_INFO("BootMain start\n");
#endif
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
//    strcpy(board_res.fel_file_name, OS_FOR_USB_UPDATE_FILE_NAME);
    //检查，用户是否有进入fel的需求
    //首先检查文件形式升级
//    if(!check_file_to_fel(board_res.fel_file_name))
//    {
//        __inf("jump to fel because of update file found\n");
//        goto jump_to_fel;
//    }
    //如果是量产完成，直接关机
//    if(!check_natch_time("c:\\natch.ini", boot1_priv_para.work_mode))
//    {
//        goto jump_to_power_off;
//    }
    
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
#ifdef CONFIG_AW_HOMELET_PRODUCT
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
//	check_private_part(boot1_priv_para.uart_port);
//	check_private_part(11);
#ifndef CONFIG_AW_HOMELET_PRODUCT
    power_set_init();
	__inf("init to usb pc\n");
	power_set_usbpc();
#endif

#ifndef SPEED_UP_BOOT

    //申请内存，填充第一个启动脚本
    global_info = (boot_global_info_t *)wBoot_malloc(sizeof(boot_global_info_t));
    if(!global_info)
    {
        __inf("unable to malloc memory for bootini\n");

        return -1;
    }
    //填充启动脚本
    memset(global_info, 0, sizeof(boot_global_info_t));
    ret = script_patch("c:\\boot.ini", global_info, 0);
    if(ret < 0)
    {
        __inf("unable to parser boot.ini\n");

        goto jump_to_fel;
    }

    //初始化显示设备
    BoardInit_Display(global_info->display_device, global_info->display_mode);
#else

    //初始化显示设备
    BoardInit_Display(0,0);
#endif

    //开始准备系统数据
    //检测电压状态
    if(check_power_status())
    {
    	ret = -1;
    }
    else
    {
//    	power_int_reg();	//启用中断检测vbus
    	if(BootOS_detect_os_type(&para_addr, &kernal_addr, (void *)global_info, &logo_status))
    	{
    		ret = -2;
    	}
    	else
    	{
    		ret = 0;
    	}
    }
    BoardExit(logo_status, ret);
    
#ifndef CONFIG_AW_HOMELET_PRODUCT
	power_int_rel();
	usb_detect_exit();
#endif
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
    wBoot_timer_delay(50);
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
	while(1)
	{
		key_value = wBoot_key_get_value();
		if(key_value > 0)
			__inf("key value = %d\n", key_value);
		key_value = 0;
	}
}



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
#include "card_sprite.h"
#include "part_info.h"
#include "common_res.h"
#include "card_sprite_res.h"


boot_hardware_res     board_res;
int					  force_debug = 0;
int					  cue;


extern  __u32 usb_start(void);
extern  __u32 usb_run(void);

static void SIMD_HW_enable(void)
{
#ifdef __NEON_SIMD__
__asm__ __volatile__ (
"push {r0 ,r1}\n\t"
"MRC p15, #0, r1, c1, c0, #2 \n\t"/* r1 = Access Control Register*/
"ORR r1, r1, #(0xf << 20)\n\t" /* enable full access for p10,11*/
"MCR p15, #0, r1, c1, c0, #2\n\t" /*Access Control Register = r1*/
"MOV r1, #0\n\t"
"MCR p15, #0, r1, c7, c5, #4\n\t" /*flush prefetch buffer because of FMXR below*/
"MOV r0,#0x40000000\n\t" /*and CP 10 & 11 were only just enabled,Enable VFP itself*/
"FMXR FPEXC, r0\n\t" /*FPEXC = r0*/
"pop {r0 ,r1}\n\t"
"isb sy\n\t"
"dsb sy\n\t"
);
#endif
}

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
	__s32                 ret;
    int                   erase_flash;
	MBR					  mbr_info;
    boot_global_info_t   *global_info;

    SIMD_HW_enable();
    DMSG_INFO("big firmware! here we go !\n");
	DMSG_INFO("Sprite start\n");
	{
		char ch;

		ch = wBoot_getc_delay( 1 );  // 27000000
	    __inf("%d\n", ch);

	    switch(ch)
	   	{
	   		case '1':
	   		{
	   			usb_start();
	    		usb_run();
	    		break;
	   		}
	   		case '2':
	   		{
	   			__inf("Jump to fel\n");
	   			wBoot_jump_fel( );
	   			break;
	   		}
	   		case '-':
	   		{
	   			__u32 dbg = 0x55;

	            __inf("hold\n");
		        while(dbg == 0x55);
		        break;
	   		}
	   		case '+':
	   		{
	   			force_debug = 1;
	        	break;
	   		}
	   		default:
	   			break;
	   	}
	}
	*(volatile unsigned int *)(0x1c20C00 + 0x118) = 0;
	*(volatile unsigned int *)(0x1c20C00 + 0x11C) = 3;
	//数据初始化
	memset(&board_res, 0, sizeof(boot_hardware_res));
    //申请内存，填充第一个启动脚本
    global_info = (boot_global_info_t *)wBoot_malloc(sizeof(boot_global_info_t));
    if(!global_info)
    {
        __wrn("unable to malloc memory for bootini\n");

        return -1;
    }
    //填充启动脚本
    memset(global_info, 0, sizeof(boot_global_info_t));
    ret = script_patch("c:\\boot.ini", global_info, 0);
    if(ret < 0)
    {
        __wrn("unable to parser boot.ini\n");

        return -1;
    }
    //初始化显示设备
    __inf("display init\n");
    cue = -1;
    if(boot_ui_init(global_info->display_device, global_info->display_mode) >= 0)
    {
    	//开启字符显示
    	__inf("char init\n");
    	boot_ui_char_init(0);
    	cue = 0;
    }
    sprite_led_init();
    sprite_wrn_init();
    //获取MBR信息
    __inf("mbr fetch\n");
	ret = card_mbr_info((void *)&mbr_info);
	if(ret < 0)
	{
		__wrn("unable to get mbr info\n");

        return -1;
	}
	boot_ui_check_device_open();
    
	ret = wBoot_script_parser_fetch("platform", "eraseflag", &erase_flash, 1);
	if((!ret) && (erase_flash))
	{
		erase_flash = 1;
	}
	else
	{
		erase_flash = 0;
	}
    
    //开始准备系统数据
    ret = card_sprite((void *)&mbr_info,erase_flash, cue);

	sprite_wrn_exit();
	sprite_led_exit(ret);
	if(!ret)
	{
		char	buffer[512];

		board_res.led_status[0] = CARD_SPRITE_SUCCESSED;
		sprite_wrn("CARD OK\n");

		__inf("get work mode\n");
		memset(buffer, 0, 512);
		wBoot_block_dread(15, 1, buffer);
		if(strcmp(buffer, "1234567890abcdefghijklmnopqrstuvwxyz"))
		{
			__inf("try to close\n");
			wBoot_timer_delay(3000);

			wBoot_power_set_off();
		}
		else
		{
			int ret;

			__inf("try to format\n");
			wBoot_timer_delay(500);
			ret = FormatDisk();
			__inf("format %d\n", ret);
			WATCHDOG_REG_MODE = 0x0;
			wBoot_timer_delay(500);
			WATCHDOG_REG_MODE = 0x3;
		}
	}
	else
	{
		board_res.led_status[0] = CARD_SPRITE_FAIL;
	}
	{
		__u32 dbg = 0x55;

		while(dbg == 0x55);
	}

    return 0;
}


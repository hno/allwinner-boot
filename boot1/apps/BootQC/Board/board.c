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
#include "common_res.h"

/*
*******************************************************************************
*                     BoardInit_Display
*
* Description:
*    加载 display 驱动
*
* Parameters:
*    void
*
* Return value:
*    0 : 成功
*   !0 : 失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 BoardInit_Display(__s32 source, __s32 mode)
{
    __s32 ret = 0;
    __s32 source_type, tv_mode, dislpay_device;

    board_res.display_source = source;
    if((source < 0) || (mode < 0))
    {
        board_res.display_source = -1;
        return 0;
    }
    ret = wBoot_driver_install("c:\\drv_de.drv");
    if(ret != 0)
    {
        DMSG_PANIC("ERR: wBoot_driver_install display driver failed\n");
        return -1;
    }
	//提前打开显示设备
	board_res.disp_hd = wBoot_driver_open(EGON2_EMOD_TYPE_DRV_DISP, NULL);
    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: open display driver failed\n");
        return -1;
    }
    source_type = 0;
    tv_mode     = 0;
    switch(source)
    {
        case 1:                                 //CVBS
            source_type = DISP_OUTPUT_TYPE_TV;
            tv_mode = DISP_TV_CVBS;
            break;
        case 2:                                 //YPBPR
            source_type = DISP_OUTPUT_TYPE_TV;
            tv_mode = DISP_TV_YPBPR;
            break;
//        case 3:                                 //SVIDEO
//            source_type = DISP_OUTPUT_TYPE_TV;
//            tv_mode = DISP_TV_SVIDEO;
//            break;
        case 3:                                 //HDMI
            source_type = DISP_OUTPUT_TYPE_HDMI;
            break;
        case 4:                                 //VGA
            source_type = DISP_OUTPUT_TYPE_VGA;
            break;
        default:
            source_type = DISP_OUTPUT_TYPE_LCD;
            break;
    }
    dislpay_device = (source_type << 16) | (tv_mode << 8) | ((mode << 0));
    //打开显示屏幕
    ret = De_OpenDevice(dislpay_device);
    if(ret != 0)
    {
        DMSG_PANIC("ERR: De Open LCD or TV failed\n");
        return -1;
    }
	board_res.layer_hd = De_RequestLayer(DISP_LAYER_WORK_MODE_NORMAL);
	if(board_res.layer_hd == NULL)
	{
        DMSG_PANIC("ERR: De_RequestLayer failed\n");
        return -1;
	}

	//wBoot_block_init();
	LCD_DisplayInit();

    return 0;
}

/*
*******************************************************************************
*                     BoardExit_Display
*
* Description:
*    卸载 display 驱动
*
* Parameters:
*    void
*
* Return value:
*    0 : 成功
*   !0 : 失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 BoardExit_Display(void)
{
    if(board_res.display_source < 0)
    {
        return 0;
    }
    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");
    }
	else
	{
	    //退出显示驱动，只关闭中断，但是不关闭屏幕
		wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_SET_EXIT_MODE, DISP_EXIT_MODE_CLEAN_PARTLY, 0);
		//wBoot_driver_ioctl(display_hd, DISP_CMD_LCD_OFF, 0, 0);
		wBoot_driver_close(board_res.disp_hd);
	}

    return wBoot_driver_uninstall(EGON2_EMOD_TYPE_DRV_DISP);
}

/*
*******************************************************************************
*                     BoardInit
*
* Description:
*    硬件初始化。
*
* Parameters:
*    :  input.  Boot阶段的参数。
*
* Return value:
*    返回硬件初始化的结果，成功或者失败。
*
* note:
*    无
*
*******************************************************************************
*/
__s32 BoardInit(void)
{
    /* 初始化 WatchDog */

    /* 初始化 PIO */

    /* 初始化 Jtag */

    /* 初始化 Uart */
    /* 初始化 Key */
    /* 初始化 IR */

    /* 初始化 FM */

    /* 初始化 显示输出 */
    //BoardInit_Display();

    return 0;
}

/*
*******************************************************************************
*                     BoardExit
*
* Description:
*    关闭所有的硬件资源。
*
* Parameters:
*    :  input.  Boot阶段的参数。
*
* Return value:
*    返回成功或者失败。
*
* note:
*    无
*
*******************************************************************************
*/
__s32 BoardExit(void)
{
    /* 关闭 显示输出 */
    BoardExit_Display();

    /* 关闭 FM */

    /* 关闭 IR */

    /* 关闭 Key */
    /* 关闭 Uart */

    /* 关闭 Jtag */

    /* 关闭 PIO */

    /* 关闭 WatchDog */
    return 0;
}



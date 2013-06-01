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
#include "egon2.h"
#include "ui_main.h"
#include "ui_main_i.h"
#include "display_interface.h"
#include "string.h"

ui_main_source_t  ui_source;

static  int boot_ui_fetchscn_info(void);
static  int boot_ui_showLayer(void);
/*
*******************************************************************************
*                     boot_ui_init
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
__s32 boot_ui_init(__s32 display_source, __s32 display_mode)
{
    __s32 ret = 0;
    __s32 source, tv_mode;

	if(display_source < 0)
	{
		return BOOT_UI_DISPLAY_SOURCE_INVALID;
	}
    if(display_mode < 0)
    {
        return BOOT_UI_DISPLAY_MODE_INVALID;
    }
    ret = wBoot_driver_install("c:\\drv_de.drv");
    if(ret < 0)
    {
        return BOOT_UI_DISPLAY_DRIVER_NOT_EXIST;
    }
    memset(&ui_source, 0, sizeof(ui_main_source_t));
	//提前打开显示设备，用于调用显示提供的命令
	ui_source.disp_hd = wBoot_driver_open(EGON2_EMOD_TYPE_DRV_DISP, NULL);
    if(ui_source.disp_hd == NULL)
    {
        return BOOT_UI_DISPLAY_DRIVER_OPEN_FAIL;
    }
    tv_mode     = 0;
    switch(display_source)
    {
        case 1:                                 //CVBS
            source  = DISP_OUTPUT_TYPE_TV;
            tv_mode = DISP_TV_CVBS;
            break;
        case 2:                                 //YPBPR
            source = DISP_OUTPUT_TYPE_TV;
            tv_mode = DISP_TV_YPBPR;
            break;
//        case 3:                                 //SVIDEO
//            source_type = DISP_OUTPUT_TYPE_TV;
//            tv_mode = DISP_TV_SVIDEO;
//            break;
        case 3:                                 //HDMI
            source = DISP_OUTPUT_TYPE_HDMI;
            break;
        case 4:                                 //VGA
            source = DISP_OUTPUT_TYPE_VGA;
            break;
        default:
            source = DISP_OUTPUT_TYPE_LCD;
            break;
    }
    //打开显示屏幕
    ret = De_OpenDevice(source, tv_mode, display_mode);
    if(ret != 0)
    {
        return BOOT_UI_DISPLAY_DEVICE_OPEN_FAIL;
    }
    //申请图层
	ui_source.layer_hdl = De_RequestLayer(DISP_LAYER_WORK_MODE_NORMAL);
	if(ui_source.layer_hdl == NULL)
	{
        return BOOT_UI_DISPLAY_LAYER_REQUEST_FAIL;
	}
	//获取屏幕参数，描绘背景
	if(boot_ui_fetchscn_info())
	{
		return BOOT_UI_SCREEN_INFO_FAIL;
	}
	//现在除了屏幕还没有打开，其它都已经准备好
	//发现如果是LCD，则不打开屏幕，否则都打开屏幕
	if(source != DISP_OUTPUT_TYPE_LCD)
	{
		boot_ui_showLayer();
	}
	ui_source.color = UI_BOOT_GUI_GREEN;

    return BOOT_UI_INIT_SUCCESSED;
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
__s32 boot_ui_exit(int ui_exit_mode)
{
	if(!ui_source.disp_hd)
	{
		return -1;
	}

	if(ui_exit_mode)
	{
		wBoot_driver_ioctl(ui_source.disp_hd, DISP_CMD_LCD_OFF, 0, 0);
	}
	else
	{
		wBoot_driver_ioctl(ui_source.disp_hd, DISP_CMD_SET_EXIT_MODE, DISP_EXIT_MODE_CLEAN_PARTLY, 0);
	}

    return wBoot_driver_uninstall(EGON2_EMOD_TYPE_DRV_DISP);
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
__s32 boot_ui_check_device_open(void)
{
	int count = 5000;
	int query = 20;
	int once  = count/query;
	int ret;

	while(1)
	{
		ret = De_IsLCDOpen();
		if(ret == 1)
		{
			break;
		}
		else if(ret == -1)
		{
			return -1;
		}
		wBoot_timer_delay(query);
		once --;
		if(once <= 0)
		{
			break;
		}
	}
	if(ret < 0)
	{
		return -1;
	}
	boot_ui_showLayer();

	return 0;
}
/*
*******************************************************************************
*                     ui_AllocLayerPara
*
* Description:
*    初始化一个图层参数
*
* Parameters:
*    PictureInfo  :  input. 图片信息
*
* Return value:
*    返回图层参数
*
* note:
*    void
*
*******************************************************************************
*/
static  int boot_ui_fetchscn_info(void)
{
	ui_source.layer_para = (__disp_layer_info_t *)wBoot_malloc(sizeof(__disp_layer_info_t));
	if(ui_source.layer_para == NULL)
	{
		return -1;
	}
	memset(ui_source.layer_para, 0, sizeof(__disp_layer_info_t));
    /* 初始化图形参数 */
	ui_source.screen_width  = De_GetSceenWidth();
	ui_source.screen_height = De_GetSceenHeight();
	ui_source.screen_size   = ui_source.screen_width * ui_source.screen_height * 4;
    ui_source.screen_buf 	= wBoot_malloc(ui_source.screen_size);
    if(!ui_source.screen_buf)
    {
        wBoot_free(ui_source.layer_para);
		ui_source.layer_para = NULL;

        return -1;
    }
    memset(ui_source.screen_buf, 0, ui_source.screen_size);

    ui_source.layer_para->fb.size.width 	= ui_source.screen_width;
    ui_source.layer_para->fb.addr[0] 		= (__u32)ui_source.screen_buf;
    ui_source.layer_para->fb.mode 			= DISP_MOD_INTERLEAVED;
    ui_source.layer_para->fb.format 		= DISP_FORMAT_ARGB8888;
    ui_source.layer_para->fb.br_swap 		= 0;
    ui_source.layer_para->fb.seq 			= DISP_SEQ_ARGB;
    ui_source.layer_para->ck_enable       	= 0;
	ui_source.layer_para->mode           	= DISP_LAYER_WORK_MODE_NORMAL;
	ui_source.layer_para->alpha_en       	= 0;
	ui_source.layer_para->alpha_val      	= 0xff;
	ui_source.layer_para->pipe           	= 0;
	ui_source.layer_para->src_win.x      	= 0;
	ui_source.layer_para->src_win.y      	= 0;
	ui_source.layer_para->src_win.width  	= ui_source.screen_width;
	ui_source.layer_para->src_win.height 	= ui_source.screen_height;
	ui_source.layer_para->scn_win.x      	= 0;
	ui_source.layer_para->scn_win.y      	= 0;
	ui_source.layer_para->scn_win.width  	= ui_source.screen_width;
	ui_source.layer_para->scn_win.height 	= ui_source.screen_height;

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
static int boot_ui_showLayer(void)
{
	__s32 ret = 0;

	ret = De_SetLayerPara(ui_source.layer_hdl, ui_source.layer_para);
	if(ret != 0)
	{
		return -1;
	}
	/* 显示 */
	De_OpenLayer(ui_source.layer_hdl);

	return 0;
}




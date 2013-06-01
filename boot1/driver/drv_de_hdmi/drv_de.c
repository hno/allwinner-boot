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

#include "drv_de.h"

static __u32    de_ext_hd;
static __hdle   lcd_timer[2] = {0};
static __u32    lcd_flow_cnt[2] = {0};
static __bool   lcd_op_finished[2] = {0};
static __u32    exit_mode = DISP_EXIT_MODE_CLEAN_ALL;//0:clean all  1:disable interrupt

void xDelayMS(__u32 Milliseconds)
{
    __u32 cnt = 1;
    __u32 delay_unit = 0;
    __u32 CoreClk = 120;

    delay_unit = (CoreClk * 1000 * 1000) / 1000;
    cnt = Milliseconds * delay_unit;
    while(cnt--);
}

// [before][step_0][delay_0][step_1][delay_1]......[step_n-2][delay_n-2][step_n-1][delay_n-1][after]
void DRV_lcd_open_callback(void *parg)
{
    __lcd_flow_t *flow;
    __u32 sel = (__u32)parg;
    __s32 i = lcd_flow_cnt[sel]++;

    flow = BSP_disp_lcd_get_open_flow(sel);

	if(i < flow->func_num)
    {
    	flow->func[i].func(sel);
        if(flow->func[i].delay == 0)
        {
            DRV_lcd_open_callback((void*)sel);
        }
        else
        {
            wBoot_timer_start(lcd_timer[sel], flow->func[i].delay, 0);
    	}
    }
    else if(i == flow->func_num)
    {
        BSP_disp_lcd_open_after(sel);
        lcd_op_finished[sel] = 1;
    }
}

__s32 DRV_lcd_open(__u32 sel)
{
    lcd_flow_cnt[sel] = 0;
    lcd_op_finished[sel] = 0;
    lcd_timer[sel] = wBoot_timer_request((void*)DRV_lcd_open_callback,(void*)sel);

    BSP_disp_lcd_open_before(sel);
    DRV_lcd_open_callback((void*)sel);


    return 0;
}

__bool DRV_lcd_check_open_finished(__u32 sel)
{
    if(lcd_op_finished[sel] && lcd_timer[sel] != NULL)
    {
        wBoot_timer_release(lcd_timer[sel]);
        lcd_timer[sel] = NULL;
    }
    return lcd_op_finished[sel];
}

// [before][step_0][delay_0][step_1][delay_1]......[step_n-2][delay_n-2][step_n-1][delay_n-1][after]
void DRV_lcd_close_callback(void *parg)
{
    __lcd_flow_t *flow;
    __u32 sel = (__u32)parg;
    __s32 i = lcd_flow_cnt[sel]++;

    flow = BSP_disp_lcd_get_close_flow(sel);

    if(i < flow->func_num)
    {
    	flow->func[i].func(sel);
        if(flow->func[i].delay == 0)
        {
            DRV_lcd_close_callback((void*)sel);
        }
        else
        {
            wBoot_timer_start(lcd_timer[sel], flow->func[i].delay, 0);
        }
    }
    else if(i == flow->func_num)
    {
        BSP_disp_lcd_close_after(sel);
        lcd_op_finished[sel] = 1;
    }
}

__s32 DRV_lcd_close(__u32 sel)
{
    lcd_flow_cnt[sel] = 0;
    lcd_op_finished[sel] = 0;
    lcd_timer[sel] = wBoot_timer_request((void*)DRV_lcd_close_callback,(void*)sel);

    BSP_disp_lcd_close_befor(sel);
    DRV_lcd_close_callback((void*)sel);

    return 0;
}

__bool DRV_lcd_check_close_finished(__u32 sel)
{
    if(lcd_op_finished[sel] && lcd_timer[sel] != NULL)
    {
    	wBoot_timer_release(lcd_timer[sel]);
        lcd_timer[sel] = NULL;
    }
    return lcd_op_finished[sel];
}

void hdmi_delay_ms(__u32 t)
{
    wBoot_timer_delay(t);//ms
}

__s32 DRV_hdmi_open(void)
{
    __inf("DRV_hdmi_open\n");

    Hdmi_hal_video_enable_sync(1);

	return 0;
}

__s32 DRV_hdmi_close(void)
{
    __inf("DRV_hdmi_close\n");
    
	Hdmi_hal_video_enable_sync(0); 

	return 0;
}
__s32 DRV_hdmi_set_display_mode(__disp_tv_mode_t mode)
{
	__u32 hdmi_mode;

	__inf("DRV_hdmi_set_display_mode,mode:%d\n",mode);
	
	switch(mode)
	{
	case DISP_TV_MOD_480I:
		hdmi_mode = HDMI1440_480I;
		break;
		
	case DISP_TV_MOD_576I:
		hdmi_mode = HDMI1440_576I;
		break;
		
	case DISP_TV_MOD_480P:
		hdmi_mode = HDMI480P;
		break;
		
	case DISP_TV_MOD_576P:
		hdmi_mode = HDMI576P;
		break;  
		
	case DISP_TV_MOD_720P_50HZ:
		hdmi_mode = HDMI720P_50;
		break;
		
	case DISP_TV_MOD_720P_60HZ:
		hdmi_mode = HDMI720P_60;
		break;
		
	case DISP_TV_MOD_1080I_50HZ:
		hdmi_mode = HDMI1080I_50;
		break;
		
	case DISP_TV_MOD_1080I_60HZ:
		hdmi_mode = HDMI1080I_60;
		break;         
		
	case DISP_TV_MOD_1080P_24HZ:
		hdmi_mode = HDMI1080P_24;
		break;    
		
	case DISP_TV_MOD_1080P_50HZ:
		hdmi_mode = HDMI1080P_50;
		break;
		
	case DISP_TV_MOD_1080P_60HZ:
		hdmi_mode = HDMI1080P_60;
		break;  

	case DISP_TV_MOD_1080P_24HZ_3D_FP:
		hdmi_mode = HDMI1080P_24_3D_FP;
		break;  
		
    case DISP_TV_MOD_720P_50HZ_3D_FP:
        hdmi_mode = HDMI720P_50_3D_FP;
        break;

    case DISP_TV_MOD_720P_60HZ_3D_FP:
        hdmi_mode = HDMI720P_60_3D_FP;
        break;

	default:
	    __wrn("unsupported video mode %d when set display mode\n", mode);
		return -1;
	}

	return Hdmi_hal_set_display_mode(hdmi_mode);
}
__s32 DRV_hdmi_mode_support(__disp_tv_mode_t mode)
{
	__u32 hdmi_mode;
	
	switch(mode)
	{
	case DISP_TV_MOD_480I:
		hdmi_mode = HDMI1440_480I;
		break;
		
	case DISP_TV_MOD_576I:
		hdmi_mode = HDMI1440_576I;
		break;
		
	case DISP_TV_MOD_480P:
		hdmi_mode = HDMI480P;
		break;
		
	case DISP_TV_MOD_576P:
		hdmi_mode = HDMI576P;
		break;  
		
	case DISP_TV_MOD_720P_50HZ:
		hdmi_mode = HDMI720P_50;
		break;
		
	case DISP_TV_MOD_720P_60HZ:
		hdmi_mode = HDMI720P_60;
		break;
		
	case DISP_TV_MOD_1080I_50HZ:
		hdmi_mode = HDMI1080I_50;
		break;
		
	case DISP_TV_MOD_1080I_60HZ:
		hdmi_mode = HDMI1080I_60;
		break;         
		
	case DISP_TV_MOD_1080P_24HZ:
		hdmi_mode = HDMI1080P_24;
		break;    
		
	case DISP_TV_MOD_1080P_50HZ:
		hdmi_mode = HDMI1080P_50;
		break;
		
	case DISP_TV_MOD_1080P_60HZ:
		hdmi_mode = HDMI1080P_60;
		break;  

	case DISP_TV_MOD_1080P_24HZ_3D_FP:
	    hdmi_mode = HDMI1080P_24_3D_FP;
	    break;

    case DISP_TV_MOD_720P_50HZ_3D_FP:
        hdmi_mode = HDMI720P_50_3D_FP;
        break;

    case DISP_TV_MOD_720P_60HZ_3D_FP:
        hdmi_mode = HDMI720P_60_3D_FP;
        break;

	default:
		hdmi_mode = HDMI720P_50;
		break;
	}

	return Hdmi_hal_mode_support(hdmi_mode);
}
__s32 DRV_hdmi_get_HPD_status(void)
{
	return Hdmi_hal_get_HPD();
}


__s32 DRV_hdmi_set_pll(__u32 pll, __u32 clk)
{
    Hdmi_hal_set_pll(pll, clk);
    return 0;
}
__s32 DRV_set_exit_mode(__u32 mode)
{
    exit_mode = mode;
    return 0;
}

__s32 disp_int_process(__u32 sel)
{
    return 0;
}

/*
****************************************************************************************************
*
*             DRV_DE_INIT
*
*  Description: 初始化函数，通常把只会执行一次的操作放在这里
*               当驱动第一次安装的时候会调用这个函数
*               此函数没有参数，需要参数的时候需要从配置脚本去获取
*  Parameters:
*
*  Return value:
*       0
*       -1
****************************************************************************************************
*/
__s32 DRV_DE_INIT(void)
{
    __disp_bsp_init_para para;

    para.base_image0    = 0x01e60000;
    para.base_image1    = 0x01e40000;
    para.base_scaler0   = 0x01e00000;
    para.base_scaler1   = 0x01e20000;
    para.base_lcdc0     = 0x01c0c000;
    para.base_lcdc1     = 0x01c17000;
    para.base_tvec0     = 0x01c0a000;
    para.base_tvec1     = 0x01c1b000;
    para.base_ccmu      = 0x01c20000;
    para.base_sdram     = 0x01c01000;
    para.base_pioc      = 0x01c20800;
    para.base_pwm       = 0x01c20c00;
    para.disp_int_process = disp_int_process;
    para.Hdmi_open           = DRV_hdmi_open;
    para.Hdmi_close          = DRV_hdmi_close;
    para.hdmi_set_mode       = DRV_hdmi_set_display_mode;
    para.hdmi_mode_support   = DRV_hdmi_mode_support;
    para.hdmi_get_HPD_status = DRV_hdmi_get_HPD_status;
    para.hdmi_set_pll        = DRV_hdmi_set_pll;

    BSP_disp_init(&para);
    Hdmi_set_reg_base(0x01c16000);
    Hdmi_hal_init();
    BSP_disp_open();

    return 0;
}


/*
****************************************************************************************************
*
*             DRV_DE_EXIT
*
*  Description: 卸载驱动，通常把只会执行一次的操作放在这里
*               当驱动退出的时候会调用这个这个函数
*               卸载函数没有参数
*  Parameters:
*
*  Return value:
*       0
*       -1
****************************************************************************************************
*/
__s32 DRV_DE_EXIT(void)
{
    if(lcd_op_finished[0] && lcd_timer[0] != NULL)
    {
        wBoot_timer_release(lcd_timer[0]);
        lcd_timer[0] = NULL;
    }
    if(lcd_op_finished[1] && lcd_timer[1] != NULL)
    {
        wBoot_timer_release(lcd_timer[1]);
        lcd_timer[1] = NULL;
    }

    BSP_disp_exit(exit_mode);

    return 0;
}


/*
****************************************************************************************************
*
*             DRV_DE_OPEN
*
*  Description:驱动打开操作，必须考虑到最终用户多次打开的情况
*
*
*  Parameters: mid: 用户驱动所对应的模块编号，系统提供，用户不需要关心
*                   很可能驱动编写者不需要这个参数
*  Return value:
*       0
*       -1
****************************************************************************************************
*/
__u32 DRV_DE_OPEN(__u32 mid, void *p_arg)
{
    return (__u32)&de_ext_hd;
}


/*
****************************************************************************************************
*
*             DRV_DE_CLOSE
*
*  Description:驱动关闭操作，必须考虑到最终用户多次关闭的情况
*
*
*  Parameters: mid: 最终用户通过驱动模块编号来关闭驱动
*                   很可能驱动编写者不需要这个参数
*  Return value:
*       eGON2_OK
*       eGON2_FAIL
****************************************************************************************************
*/
__s32 DRV_DE_CLOSE(__u32 hd)
{
    return 0;
}


/*
****************************************************************************************************
*
*             DRV_DE_READ
*
*  Description: 驱动提供的读操作函数
*
*
*  Parameters:  pdata: 用户传进的数据地址，通常用于读回的数据所存放的地址
*               size : 要读的数据块的大小
*               n    : 要读的次数，实际上真正要读的数据大小应该是size * n
*               mid  : 模块编号，驱动编写者可以不用关心
*  Return value:
*       读成功的时候，应该返回实际读回的数据大小
*       读失败的时候，应该返回0，表示读回了0字节数据
*
****************************************************************************************************
*/
__u32 DRV_DE_READ(void *pdata, __u32 size, __u32 n, __u32 hd)
{
    return 0;
}


/*
****************************************************************************************************
*
*             DRV_DE_WRITE
*
*  Description: 驱动提供的写操作函数
*
*
*  Parameters:  pdata: 用户传进的数据地址，通常用于写下的数据所存放的地址
*               size : 要写的数据块的大小
*               n    : 要写的次数，实际上真正要写的数据大小应该是size * n
*               mid  : 模块编号，驱动编写者可以不用关心
*  Return value:
*       写成功的时候，应该返回实际写下的数据大小
*       写失败的时候，应该返回0，表示写下了0字节数据
*
****************************************************************************************************
*/
__u32 DRV_DE_WRITE(const void *pdata, __u32 size, __u32 n, __u32 hd)
{
    return 0;
}


/*
****************************************************************************************************
*
*             DRV_DE_IOCTRL
*
*  Description: 驱动提供的io操作函数，通常用于扩展操作
*
*
*  Parameters:  mid: 模块编号，驱动编写者可以不用关心
*               cmd: 驱动提供的操作命令
*               aux: 用户传进的整型参数
*               pbuffer: 用户传进的指针型参数
*
*  Return value:
*       0
*       -1
****************************************************************************************************
*/
__s32 DRV_DE_IOCTRL(__u32 hd, __u32 cmd, __s32 aux, void *pbuffer)
{
    __u32 para0=0, para1=0, para2=0;

    if(pbuffer != NULL)
    {
        para0 = *((__u32*)pbuffer);
        para1 = *((__u32*)((__u32)pbuffer+4));
        para2 = *((__u32*)((__u32)pbuffer+8));
    }

    switch(cmd)
	{
//----disp global----
	case DISP_CMD_SET_BKCOLOR:
		return BSP_disp_set_bk_color(aux, (__disp_color_t*)para0);

	case DISP_CMD_SET_COLORKEY:
		return BSP_disp_set_color_key(aux, (__disp_colorkey_t *)para0);

	case DISP_CMD_SET_PALETTE_TBL:
		return BSP_disp_set_palette_table(aux, (__u32 *)para0, para1, para2);

	case DISP_CMD_GET_PALETTE_TBL:
		return BSP_disp_get_palette_table(aux, (__u32 *)para0, para1, para2);

	case DISP_CMD_START_CMD_CACHE:
		return BSP_disp_cmd_cache(aux);

	case DISP_CMD_EXECUTE_CMD_AND_STOP_CACHE:
		return BSP_disp_cmd_submit(aux);

	case DISP_CMD_GET_OUTPUT_TYPE:
		return  BSP_disp_get_output_type(aux);

	case DISP_CMD_SCN_GET_WIDTH:
		return BSP_disp_get_screen_width(aux);

	case DISP_CMD_SCN_GET_HEIGHT:
		return BSP_disp_get_screen_height(aux);

	case DISP_CMD_SET_EXIT_MODE:
	    return DRV_set_exit_mode(aux);

	case DISP_CMD_SET_GAMMA_TABLE:
		return BSP_disp_set_gamma_table(aux, (__u32 *)para0, para1);

	case DISP_CMD_GAMMA_CORRECTION_ON:
		return BSP_disp_gamma_correction_enable(aux);

	case DISP_CMD_GAMMA_CORRECTION_OFF:
		return BSP_disp_gamma_correction_disable(aux);


//----layer----
	case DISP_CMD_LAYER_REQUEST:
		return BSP_disp_layer_request(aux, (__disp_layer_work_mode_t)para0);

	case DISP_CMD_LAYER_RELEASE:
		return BSP_disp_layer_release(aux, para0);

	case DISP_CMD_LAYER_OPEN:
		return BSP_disp_layer_open(aux, para0);

	case DISP_CMD_LAYER_CLOSE:
		return BSP_disp_layer_close(aux, para0);

	case DISP_CMD_LAYER_SET_FB:
		return BSP_disp_layer_set_framebuffer(aux, para0, (__disp_fb_t*)para1);

	case DISP_CMD_LAYER_GET_FB:
		return BSP_disp_layer_get_framebuffer(aux, para0, (__disp_fb_t*)para1);

	case DISP_CMD_LAYER_SET_SRC_WINDOW:
		return BSP_disp_layer_set_src_window(aux,para0, (__disp_rect_t*)para1);

	case DISP_CMD_LAYER_GET_SRC_WINDOW:
		return BSP_disp_layer_get_src_window(aux,para0, (__disp_rect_t*)para1);

	case DISP_CMD_LAYER_SET_SCN_WINDOW:
		return BSP_disp_layer_set_screen_window(aux,para0, (__disp_rect_t*)para1);

	case DISP_CMD_LAYER_GET_SCN_WINDOW:
		return BSP_disp_layer_get_screen_window(aux,para0, (__disp_rect_t*)para1);

	case DISP_CMD_LAYER_SET_PARA:
		return BSP_disp_layer_set_para(aux, para0, (__disp_layer_info_t *)para1);

	case DISP_CMD_LAYER_GET_PARA:
		return BSP_disp_layer_get_para(aux, para0, (__disp_layer_info_t *)para1);

	case DISP_CMD_LAYER_TOP:
		return BSP_disp_layer_set_top(aux, para0);

	case DISP_CMD_LAYER_BOTTOM:
		return BSP_disp_layer_set_bottom(aux, para0);

	case DISP_CMD_LAYER_ALPHA_ON:
		return BSP_disp_layer_alpha_enable(aux, para0, 1);

	case DISP_CMD_LAYER_ALPHA_OFF:
		return BSP_disp_layer_alpha_enable(aux, para0, 0);

	case DISP_CMD_LAYER_SET_ALPHA_VALUE:
		return BSP_disp_layer_set_alpha_value(aux, para0, para1);

	case DISP_CMD_LAYER_CK_ON:
		return BSP_disp_layer_colorkey_enable(aux, para0, 1);

	case DISP_CMD_LAYER_CK_OFF:
		return BSP_disp_layer_colorkey_enable(aux, para0, 0);

	case DISP_CMD_LAYER_SET_PIPE:
		return BSP_disp_layer_set_pipe(aux, para0, para1);

	case DISP_CMD_LAYER_GET_ALPHA_VALUE:
		return BSP_disp_layer_get_alpha_value(aux, para0);

	case DISP_CMD_LAYER_GET_ALPHA_EN:
		return BSP_disp_layer_get_alpha_enable(aux, para0);

	case DISP_CMD_LAYER_GET_CK_EN:
		return BSP_disp_layer_get_colorkey_enable(aux, para0);

	case DISP_CMD_LAYER_GET_PRIO:
		return BSP_disp_layer_get_piro(aux, para0);

	case DISP_CMD_LAYER_GET_PIPE:
		return BSP_disp_layer_get_pipe(aux, para0);

    case DISP_CMD_LAYER_SET_SMOOTH:
        return BSP_disp_layer_set_smooth(aux, para0,(__disp_video_smooth_t) para1);

    case DISP_CMD_LAYER_GET_SMOOTH:
        return BSP_disp_layer_get_smooth(aux, para0);


//----scaler----
	case DISP_CMD_SCALER_REQUEST:
		return BSP_disp_scaler_request();

	case DISP_CMD_SCALER_RELEASE:
		return BSP_disp_scaler_release(para0);

	case DISP_CMD_SCALER_EXECUTE:
		return BSP_disp_scaler_start(para0,(__disp_scaler_para_t *)para1);

//----hwc----
	case DISP_CMD_HWC_OPEN:
		return  BSP_disp_hwc_enable(aux, 1);

	case DISP_CMD_HWC_CLOSE:
		return  BSP_disp_hwc_enable(aux, 0);

	case DISP_CMD_HWC_SET_POS:
		return BSP_disp_hwc_set_pos(aux, (__disp_pos_t*)para0);

	case DISP_CMD_HWC_GET_POS:
		return BSP_disp_hwc_get_pos(aux, (__disp_pos_t *)para0);

	case DISP_CMD_HWC_SET_FB:
		return BSP_disp_hwc_set_framebuffer(aux, (__disp_hwc_pattern_t *)para0);

	case DISP_CMD_HWC_SET_PALETTE_TABLE:
		return BSP_disp_hwc_set_palette(aux, (void*)para0, para1, para2);


//----video----
	case DISP_CMD_VIDEO_START:
		return BSP_disp_video_start(aux, para0);

	case DISP_CMD_VIDEO_STOP:
		return BSP_disp_video_stop(aux, para0);

	case DISP_CMD_VIDEO_SET_FB:
		return BSP_disp_video_set_fb(aux, para0, (__disp_video_fb_t *)para1);

    case DISP_CMD_VIDEO_GET_FRAME_ID:
        return BSP_disp_video_get_frame_id(aux, para0);

    case DISP_CMD_VIDEO_GET_DIT_INFO:
        return BSP_disp_video_get_dit_info(aux, para0,(__disp_dit_info_t *)para1);

//----lcd----
	case DISP_CMD_LCD_ON:
		return DRV_lcd_open(aux);

	case DISP_CMD_LCD_OFF:
		return DRV_lcd_close(aux);

	case DISP_CMD_LCD_SET_BRIGHTNESS:
		return BSP_disp_lcd_set_bright(aux, para0);

	case DISP_CMD_LCD_GET_BRIGHTNESS:
		return BSP_disp_lcd_get_bright(aux);

	case DISP_CMD_LCD_CPUIF_XY_SWITCH:
		return BSP_disp_lcd_xy_switch(aux, para0);

	case DISP_CMD_LCD_SET_SRC:
		return BSP_disp_lcd_set_src(aux, (__disp_lcdc_src_t)para0);

	case DISP_CMD_LCD_CHECK_OPEN_FINISH:
		return DRV_lcd_check_open_finished(aux);

	case DISP_CMD_LCD_CHECK_CLOSE_FINISH:
		return DRV_lcd_check_close_finished(aux);


//----tv----
	case DISP_CMD_TV_ON:
		return BSP_disp_tv_open(aux);

	case DISP_CMD_TV_OFF:
		return BSP_disp_tv_close(aux);

	case DISP_CMD_TV_SET_MODE:
		return BSP_disp_tv_set_mode(aux, (__disp_tv_mode_t)para0);

	case DISP_CMD_TV_GET_MODE:
		return BSP_disp_tv_get_mode(aux);

	case DISP_CMD_TV_AUTOCHECK_ON:
		return BSP_disp_tv_auto_check_enable(aux);

	case DISP_CMD_TV_AUTOCHECK_OFF:
		return BSP_disp_tv_auto_check_disable(aux);

	case   DISP_CMD_TV_GET_INTERFACE:
		return BSP_disp_tv_get_interface(aux);

	case DISP_CMD_TV_SET_SRC:
		return BSP_disp_tv_set_src(aux, (__disp_lcdc_src_t)para0);

    case DISP_CMD_TV_GET_DAC_STATUS:
        return BSP_disp_tv_get_dac_status(aux, para0);

    case DISP_CMD_TV_SET_DAC_SOURCE:
        return BSP_disp_tv_set_dac_source(aux, para0, (__disp_tv_dac_source)para1);

    case DISP_CMD_TV_GET_DAC_SOURCE:
        return BSP_disp_tv_get_dac_source(aux, para0);

//----hdmi----
	case DISP_CMD_HDMI_ON:
		return BSP_disp_hdmi_open(aux);

	case DISP_CMD_HDMI_OFF:
		return BSP_disp_hdmi_close(aux);

	case DISP_CMD_HDMI_SET_MODE:
		return BSP_disp_hdmi_set_mode(aux, (__disp_tv_mode_t)para0);

	case DISP_CMD_HDMI_GET_MODE:
		return BSP_disp_hdmi_get_mode(aux);

	case DISP_CMD_HDMI_GET_HPD_STATUS:
	    return BSP_disp_hdmi_get_hpd_status(aux);

	case DISP_CMD_HDMI_SUPPORT_MODE:
		return BSP_disp_hdmi_check_support_mode(aux, para0);

	case DISP_CMD_HDMI_SET_SRC:
		return BSP_disp_hdmi_set_src(aux, (__disp_lcdc_src_t)para0);


//----vga----
	case DISP_CMD_VGA_ON:
		return BSP_disp_vga_open(aux);

	case DISP_CMD_VGA_OFF:
		return BSP_disp_vga_close(aux);

	case DISP_CMD_VGA_SET_MODE:
		return BSP_disp_vga_set_mode(aux, (__disp_vga_mode_t)para0);

	case DISP_CMD_VGA_GET_MODE:
		return BSP_disp_vga_get_mode(aux);

	case DISP_CMD_VGA_SET_SRC:
		return BSP_disp_vga_set_src(aux, (__disp_lcdc_src_t)para0);

//----sprite----
	case DISP_CMD_SPRITE_OPEN:
		return BSP_disp_sprite_open(aux);

	case DISP_CMD_SPRITE_CLOSE:
		return BSP_disp_sprite_close(aux);

	case DISP_CMD_SPRITE_SET_FORMAT:
		return BSP_disp_sprite_set_format(aux, (__disp_pixel_fmt_t)para0, (__disp_pixel_seq_t)para1);

	case DISP_CMD_SPRITE_GLOBAL_ALPHA_ENABLE:
		return BSP_disp_sprite_alpha_enable(aux);

	case DISP_CMD_SPRITE_GLOBAL_ALPHA_DISABLE:
		return BSP_disp_sprite_alpha_disable(aux);

	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_ENABLE:
		return BSP_disp_sprite_get_alpha_enable(aux);

	case DISP_CMD_SPRITE_SET_GLOBAL_ALPHA_VALUE:
		return BSP_disp_sprite_set_alpha_vale(aux, para0);

	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_VALUE:
		return BSP_disp_sprite_get_alpha_value(aux);

	case DISP_CMD_SPRITE_SET_ORDER:
		return BSP_disp_sprite_set_order(aux, para0,para1);

	case DISP_CMD_SPRITE_GET_TOP_BLOCK:
		return BSP_disp_sprite_get_top_block(aux);

	case DISP_CMD_SPRITE_GET_BOTTOM_BLOCK:
		return BSP_disp_sprite_get_bottom_block(aux);

	case DISP_CMD_SPRITE_SET_PALETTE_TBL:
		return  BSP_disp_sprite_set_palette_table(aux, (__u32 * )para0,para1,para2);

	case DISP_CMD_SPRITE_GET_BLOCK_NUM:
		return BSP_disp_sprite_get_block_number(aux);

	case DISP_CMD_SPRITE_BLOCK_REQUEST:
		return BSP_disp_sprite_block_request(aux, (__disp_sprite_block_para_t*)para0);

	case DISP_CMD_SPRITE_BLOCK_RELEASE:
		return BSP_disp_sprite_block_release(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_SET_SCREEN_WINDOW:
		return BSP_disp_sprite_block_set_screen_win(aux, para0,(__disp_rect_t *)para1);

	case DISP_CMD_SPRITE_BLOCK_GET_SCREEN_WINDOW:
		return BSP_disp_sprite_block_get_srceen_win(aux, para0,(__disp_rect_t *)para1);

	case DISP_CMD_SPRITE_BLOCK_SET_SOURCE_WINDOW:
		return BSP_disp_sprite_block_set_src_win(aux, para0,(__disp_rect_t *)para1);

	case DISP_CMD_SPRITE_BLOCK_GET_SOURCE_WINDOW:
		return BSP_disp_sprite_block_get_src_win(aux, para0,(__disp_rect_t *)para1);

	case DISP_CMD_SPRITE_BLOCK_SET_FB:
		return BSP_disp_sprite_block_set_framebuffer(aux, para0,(__disp_fb_t *)para1);

	case DISP_CMD_SPRITE_BLOCK_GET_FB:
		return BSP_disp_sprite_block_get_framebufer(aux, para0,(__disp_fb_t *)para1);

	case DISP_CMD_SPRITE_BLOCK_SET_TOP:
		return BSP_disp_sprite_block_set_top(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_SET_BOTTOM:
		return BSP_disp_sprite_block_set_bottom(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_GET_PREV_BLOCK:
		return BSP_disp_sprite_block_get_pre_block(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_GET_NEXT_BLOCK:
		return BSP_disp_sprite_block_get_next_block(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_GET_PRIO:
		return BSP_disp_sprite_block_get_prio(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_OPEN:
		return BSP_disp_sprite_block_open(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_CLOSE:
		return BSP_disp_sprite_block_close(aux, para0);

	case DISP_CMD_SPRITE_BLOCK_SET_PARA:
		return BSP_disp_sprite_block_set_para(aux, para0,(__disp_sprite_block_para_t*)para1);

	case DISP_CMD_SPRITE_BLOCK_GET_PARA:
		return BSP_disp_sprite_block_get_para(aux, para0,(__disp_sprite_block_para_t*)para1);

	default:
	    __wrn("not supported display cmd:%x\n",cmd);
	    return eGON2_FAIL;
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
#define  DELAY_ONCE_TIME   (50)

__s32 DRV_DE_Standby(__u32 cmd, void *pArg)
{
	__s32 ret;
	__s32 timedly = 5000;
	__s32 check_time = timedly/DELAY_ONCE_TIME;

	if(cmd == EGON2_MOD_ENTER_STANDBY)
	{
		DRV_lcd_close(0);
		do
		{
			ret = DRV_lcd_check_close_finished(0);
			if(ret == 1)
			{
				break;
			}
			else if(ret == -1)
			{
				return -1;
			}
			wBoot_timer_delay(DELAY_ONCE_TIME);
			check_time --;
			if(check_time <= 0)
			{
				return -1;
			}
		}
		while(1);
		BSP_disp_clk_off(3);

		return 0;
	}
	else if(cmd == EGON2_MOD_EXIT_STANDBY)
	{
		BSP_disp_clk_on(3);
		DRV_lcd_open(0);
		do
		{
			ret = DRV_lcd_check_open_finished(0);
			if(ret == 1)
			{
				break;
			}
			else if(ret == -1)
			{
				return -1;
			}
			wBoot_timer_delay(DELAY_ONCE_TIME);
			check_time --;
			if(check_time <= 0)
			{
				return -1;
			}
		}
		while(1);

		return 0;
	}

	return -1;
}



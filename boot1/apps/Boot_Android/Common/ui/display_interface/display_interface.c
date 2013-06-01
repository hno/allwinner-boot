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

#include  "include.h"
#include  "common_res.h"

/*
*******************************************************************************
*                     De_SetBlkColor
*
* Description:
*    设置背景颜色
*
* Parameters:
*    Color  :  input. 颜色
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_SetBlkColor(__disp_color_t *Color)
{
    __s32 ret;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)Color;
    arg[1] = 0;
    arg[2] = 0;
    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_SET_BKCOLOR, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_SET_BKCOLOR failed\n");
    }

    return ret;
}


/*
*******************************************************************************
*                     De_GetProperHDMIMode
*
* Description:
*    获取hdmi所支持的最高分辨率模式
*
* Parameters:
*    void
*
* Return value:
*    mode  :  最高分辨率模式
*   -1 :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_GetProperHDMIMode(void)
{
    __u32 arg[3];
    __u32 i;
    __s32 ret;
    __u32 tv_mode[13]={
    DISP_TV_MOD_1080P_60HZ,      
    DISP_TV_MOD_1080P_50HZ,    
    DISP_TV_MOD_1080P_24HZ,
    DISP_TV_MOD_1080I_60HZ,           
    DISP_TV_MOD_1080I_60HZ,        
    DISP_TV_MOD_1080I_50HZ,        
    DISP_TV_MOD_720P_60HZ ,        
    DISP_TV_MOD_720P_50HZ ,        
    DISP_TV_MOD_576P      ,        
    DISP_TV_MOD_480P      ,        
    DISP_TV_MOD_576I      ,        
    DISP_TV_MOD_480I      ,
    };

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }
    
    for(i=0; i<13; i++)
    {
        arg[0] = tv_mode[i];
        arg[1] = 0;
        arg[2] = 0;
        ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_SUPPORT_MODE, 0, arg);

        if(ret == 1)
        {
            break;
        }
    }

    if(i == 13)
    {
        ret = -1;
    }else
    {
        ret = tv_mode[i];
    }

    return ret;
}

/*
*******************************************************************************
*                     De_OpenDevice
*
* Description:
*    打开TV
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_OpenDevice(__s32 display_source)
{
    __s32 ret = 0;
    __s32 aux;
    __s32 high_flag, low_flag;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }
    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;

    #ifndef CONFIG_AW_HOMELET_PRODUCT
    high_flag = (display_source >> 16) & 0xffff;
    low_flag  = (display_source >>  0) & 0xffff;

    if(DISP_OUTPUT_TYPE_LCD == high_flag)   //TV输出
    {
        ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LCD_ON, 0, (void*)arg);
    }
    else if(DISP_OUTPUT_TYPE_TV == high_flag)
    {
        if(DISP_TV_CVBS == low_flag)
        {
            aux = DISP_TV_MOD_720P_50HZ;
        }
        else if(DISP_TV_YPBPR == low_flag)
        {
            aux = DISP_TV_MOD_PAL;
        }
        else
        {
            return -1;
        }
        arg[0] = aux;
        wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_TV_SET_MODE, 0, arg);
        ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_TV_ON, 0, NULL);
    }
    else if(DISP_OUTPUT_TYPE_HDMI == high_flag)
    {
    	arg[0] = low_flag&0xff;
        wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_SET_MODE, 0, arg);
        ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_ON, 0, NULL);;
    }
    else
    {
        return -1;
    }

    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_HDMI_ON failed\n");
    }
#else
    {
        int  value = 1;
        int  ret;
        __u32 output_type = 0;
        __u32 output_mode = 0;
        __u32 auto_hpd = 0;
        __u32 err_count = 0;
        __u32 arg[4];
        int i;



    //screen0_output_type
        if(wBoot_script_parser_fetch("boot_disp", "output_type", &value, 1) < 0)
        {
            __wrn("fetch script data boot_disp.output_type fail\n");
            err_count ++;
            value = 0;
        }else
        {
            __inf("boot_disp.output_type=%d\n", value);
        }

        if(value == 0)
        {
            output_type = DISP_OUTPUT_TYPE_NONE;
        }
        else if(value == 1)
        {
            output_type = DISP_OUTPUT_TYPE_LCD;
        }
        else if(value == 2)
        {
            output_type = DISP_OUTPUT_TYPE_TV;
        }
        else if(value == 3)
        {
            output_type = DISP_OUTPUT_TYPE_HDMI;
        }
        else if(value == 4)
        {
            output_type = DISP_OUTPUT_TYPE_VGA;
        }
        else
        {
            __wrn("invalid screen0_output_type %d\n", value);
            return -1;
        }
    //screen0_output_mode    
        if(wBoot_script_parser_fetch("boot_disp", "output_mode", &value, 1) < 0)
        {
            __wrn("fetch script data boot_disp.output_mode fail\n");
            err_count ++;
            value = 0;
        }else
        {
            __inf("boot_disp.output_mode=%d\n", value);
        }
        
        if(output_type == DISP_OUTPUT_TYPE_TV || output_type == DISP_OUTPUT_TYPE_HDMI)
        {
            output_mode = (__disp_tv_mode_t)value;
        }
        else if(output_type == DISP_OUTPUT_TYPE_VGA)
        {
            output_mode = (__disp_vga_mode_t)value;
        }

    //auto hot plug detect    
        if(wBoot_script_parser_fetch("boot_disp", "auto_hpd", &value, 1) < 0)
        {
            __wrn("fetch script data boot_disp.auto_hpd fail\n");
            err_count ++;
            value = 0;
        }else
        {    
            __inf("boot_disp.auto_hpd=%d\n", value);
        }


        if(err_count == 3)//no boot_disp config
        {
            if(wBoot_script_parser_fetch("lcd0_para", "lcd_used", &value, 1) < 0)
            {
                __wrn("fetch script data lcd0_para.lcd_used fail\n");
                value = 0;
            }else
            {    
                __inf("lcd0_para.lcd_used=%d\n", value);
            }

            value = 0;
            if(value == 1) //lcd available
            {
                output_type = DISP_OUTPUT_TYPE_LCD;
            }else
            {
                arg[0] = 0;
                arg[1] = 0;
                arg[2] = 0;
                ret = 0;
                for(i=0; (i<3)&&(ret==0); i++)
                {
                    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_GET_HPD_STATUS, 0, (void*)arg);
                }
                if(ret == 1)
                {
                    output_type = DISP_OUTPUT_TYPE_HDMI;
                  //  output_mode = De_GetProperHDMIMode();
                  //  output_mode = (output_mode == -1)? DISP_TV_MOD_720P_50HZ:output_mode;
                    output_mode = DISP_TV_MOD_720P_60HZ;
                }else
                {
      
                    ret = 0;
                    arg[0] = 0;
                    arg[1] = 0;
                    arg[2] = 0;
                    for(i=0; (i<6)&&(ret==0); i++)
                    {
                        __inf("%d\n",i);
                        ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_TV_GET_INTERFACE, 0, (void*)arg);
                        wBoot_timer_delay(200);
                    }
                    __inf("tv detect, ret = %d\n", ret);
                    if((ret & DISP_TV_CVBS) == DISP_TV_CVBS)
                    {
                        output_type = DISP_OUTPUT_TYPE_TV;
                        output_mode = DISP_TV_MOD_PAL;
                        __inf("cvbs plug\n");
                    }else if((ret & DISP_TV_YPBPR) == DISP_TV_YPBPR)
                    {
                        output_type = DISP_OUTPUT_TYPE_TV;
                        output_mode = DISP_TV_MOD_720P_50HZ;
                        __inf("ypbpr plug\n");
                    }else
                    {
                        __inf("no device plug\n");
                        output_type = DISP_OUTPUT_TYPE_NONE;

                    }
                }
            }
            
        }else//has boot_disp config
        {
            if(output_type == DISP_OUTPUT_TYPE_LCD)
            {

            }else if(auto_hpd == 1)
            {
                if(wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_GET_HPD_STATUS, 0, (void*)arg) == 1)
                {
                    output_type = DISP_OUTPUT_TYPE_HDMI;
                  //  output_mode = De_GetProperHDMIMode();
                  //  output_mode = (output_mode == -1)? DISP_TV_MOD_720P_50HZ:output_mode;
                  output_mode = DISP_TV_MOD_720P_60HZ;
                }else
                {
                    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_TV_GET_INTERFACE, 0, (void*)arg);
                    if((ret & DISP_TV_CVBS) == DISP_TV_CVBS)
                    {
                        output_type = DISP_OUTPUT_TYPE_TV;
                        output_mode = DISP_TV_MOD_PAL;
                    }else if((ret & DISP_TV_YPBPR) == DISP_TV_YPBPR)
                    {
                        if((output_type == DISP_OUTPUT_TYPE_VGA) && (output_type != DISP_OUTPUT_TYPE_TV))
                        {
                            output_type = DISP_OUTPUT_TYPE_VGA;
                            output_mode = DISP_VGA_H1024_V768;
                        }else
                        {
                            output_type = DISP_OUTPUT_TYPE_TV;
                            output_mode = DISP_TV_MOD_720P_50HZ;
                        }
                    }else
                    {
                        output_type = DISP_OUTPUT_TYPE_NONE;
                    }
                }
            }
        }
        
        if(output_type == DISP_OUTPUT_TYPE_LCD)
        {
            __inf("lcd open\n");
            arg[0] = 0;
            arg[1] = 0;
            arg[2] = 0;
            ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LCD_ON, 0, (void*)arg);
            __inf("lcd open,ret=%d\n",ret);
        }
        else if(output_type == DISP_OUTPUT_TYPE_TV)
        {
            __inf("tv open\n");
            arg[0] = output_mode;
            arg[1] = 0;
            arg[2] = 0;
            wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_TV_SET_MODE, 0, arg);
            ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_TV_ON, 0, NULL);
        }
        else if(output_type == DISP_OUTPUT_TYPE_HDMI)
        {
            __inf("hdmi open\n");
            arg[0] = output_mode;
            arg[1] = 0;
            arg[2] = 0;
            wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_SET_MODE, 0, arg);
            ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_HDMI_ON, 0, NULL);
        }
        else if(output_type == DISP_OUTPUT_TYPE_VGA)
        {
            __inf("vga open\n");
            arg[0] = output_mode;
            arg[1] = 0;
            arg[2] = 0;
            wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_VGA_SET_MODE, 0, arg);
            ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_VGA_ON, 0, NULL);
        }
    }
#endif

    return ret;
}

/*
*******************************************************************************
*                     De_CloseDevice
*
* Description:
*    关闭TV
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_CloseDevice(__s32 display_source)
{
    __s32 ret = 0;
    __u32 arg[3], cmd;
    __s32 high_flag;

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }
    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;
    high_flag = (display_source >> 16) & 0xffff;

    if(DISP_OUTPUT_TYPE_LCD == high_flag)   //TV输出
    {
        cmd = DISP_CMD_LCD_OFF;
    }
    else if(DISP_OUTPUT_TYPE_TV == high_flag)
    {
        cmd = DISP_CMD_TV_OFF;
    }
    else if(DISP_OUTPUT_TYPE_HDMI == high_flag)
    {
        cmd = DISP_CMD_HDMI_OFF;
    }
    else
    {
        return -1;
    }

    ret = wBoot_driver_ioctl(board_res.disp_hd, cmd, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: display driver turn off display device failed\n");
    }

    return ret;
}
/*
*******************************************************************************
*                     De_IsLCDOpen
*
* Description:
*    LCD是否打开 ？
*
* Parameters:
*    void
*
* Return value:
*    <0	:  成功
*    0 	:  LCD正在初始化
*    >0	:  驱动打开失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_IsLCDOpen(void)
{
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;
    return wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LCD_CHECK_OPEN_FINISH, 0, (void*)arg);
}

/*
*******************************************************************************
*                     De_IsLCDClose
*
* Description:
*    LCD是否关闭 ？
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_IsLCDClose(void)
{
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;
    return wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LCD_CHECK_CLOSE_FINISH, 0, (void*)arg);
}
/*
*******************************************************************************
*                     De_RequestLayer
*
* Description:
*    申请图层
*
* Parameters:
*    layer_para  :  input.  图层参数
*    Layer_hd    :  output. 图层句柄
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__hdle De_RequestLayer(__disp_layer_work_mode_t mode)
{
    __hdle Layer_hd = NULL;
    __u32  arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return (__hdle)0;
    }

    arg[0] = mode;
    arg[1] = 0;
    arg[2] = 0;
    Layer_hd = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);
    if(Layer_hd == NULL)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_REQUEST failed\n");
    }

    return Layer_hd;
}


/*
*******************************************************************************
*                     De_ReleaseLayer
*
* Description:
*    释放图层
*
* Parameters:
*    Layer_hd    :  input. 图层句柄
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_ReleaseLayer(__s32 layer_hdl)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = 0;
    arg[2] = 0;
    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_RELEASE, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_RELEASE failed\n");
    }

    return ret;
}


/*
*******************************************************************************
*                     De_OpenLayer
*
* Description:
*    打开图层
*
* Parameters:
*    Layer_hd    :  input. 图层句柄
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_OpenLayer(__s32 layer_hdl)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = 0;
    arg[2] = 0;
    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_OPEN, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_OPEN failed\n");
    }

    return ret;
}


/*
*******************************************************************************
*                     De_CloseLayer
*
* Description:
*    关闭图层
*
* Parameters:
*    Layer_hd    :  input. 图层句柄
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_CloseLayer(__s32 layer_hdl)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = 0;
    arg[2] = 0;
    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_CLOSE, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_CLOSE failed\n");
    }

    return ret;
}

/*
*******************************************************************************
*                     De_GetFrameBuffer
*
* Description:
*    获得 FrameBuffer
*
* Parameters:
*    Layer_hd    :  input.  图层句柄
*    FrameBuffer :  output. 图层缓冲区
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_GetFrameBuffer(__s32 layer_hdl, __disp_fb_t *FrameBuffer)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = (__u32)FrameBuffer;
    arg[2] = 0;
    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_GET_FB, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_GET_FB failed\n");
    }

    return ret;
}

/*
*******************************************************************************
*                     De_SetFrameBuffer
*
* Description:
*    设置FrameBuffer
*
* Parameters:
*    Layer_hd    :  input.  图层句柄
*    FrameBuffer :  input.  图层缓冲区
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_SetFrameBuffer(__s32 layer_hdl, __disp_fb_t *FrameBuffer)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = (__u32)FrameBuffer;
    arg[2] = 0;

    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_SET_FB, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_SET_FB failed\n");
    }

    return ret;
}

/*
*******************************************************************************
*                     De_GetSceenWidth
*
* Description:
*    获得屏幕的宽度
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__u32 De_GetSceenWidth(void)
{
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return 0;
    }

    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;

    return wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_SCN_GET_WIDTH, 0, (void*)arg);
}



/*
*******************************************************************************
*                     De_GetSceenWidth
*
* Description:
*    获得屏幕的高度
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__u32 De_GetSceenHeight(void)
{
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return 0;
    }

    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;

    return wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_SCN_GET_HEIGHT, 0, (void*)arg);
}

/*
*******************************************************************************
*                     De_GetLayerPara
*
* Description:
*    获得图层参数
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_GetLayerPara(__s32 layer_hdl, __disp_layer_info_t * layer_para)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = (__u32)layer_para;
    arg[2] = 0;
    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_GET_PARA, 0, (void*)arg);

    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_GET_PARA failed\n");
    }

    return ret;
}

/*
*******************************************************************************
*                     De_SetLayerPara
*
* Description:
*    设置图层参数
*
* Parameters:
*    void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 De_SetLayerPara(__s32 layer_hdl, __disp_layer_info_t * layer_para)
{
    __s32 ret = 0;
    __u32 arg[3];

    if(board_res.disp_hd == NULL)
    {
        DMSG_PANIC("ERR: display driver not open yet\n");

        return -1;
    }

    arg[0] = (__u32)layer_hdl;
    arg[1] = (__u32)layer_para;
    arg[2] = 0;

    ret = wBoot_driver_ioctl(board_res.disp_hd, DISP_CMD_LAYER_SET_PARA, 0, (void*)arg);
    if(ret)
    {
        DMSG_PANIC("ERR: wBoot_driver_ioctl DISP_CMD_LAYER_SET_PARA failed\n");
    }

    return ret;
}



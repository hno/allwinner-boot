/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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

#include  "include.h"
#include  "ui.h"
#include  "display_interface/display_interface.h"
#include  "common_res.h"
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
display_layer_info_t *ui_AllocLayerPara(Picture_t *PictureInfo)
{
	display_layer_info_t *layer_para = NULL;
	__u32 screen_width = 0;
	__u32 screen_height = 0;

	layer_para = (display_layer_info_t *)wBoot_malloc(sizeof(display_layer_info_t));
	if(layer_para == NULL)
	{
		DMSG_PANIC("ERR: wBoot_malloc failed\n");
		return NULL;
	}
	memset(layer_para, 0, sizeof(display_layer_info_t));
    /* 初始化图形参数 */
	screen_width  = De_GetSceenWidth();
	screen_height = De_GetSceenHeight();

    layer_para->fb.size.width 		= (PictureInfo->RowSize * 8) / PictureInfo->BitCount;
    layer_para->fb.addr[0] 			= (__u32)PictureInfo->Buffer;
    layer_para->fb.mode = DISP_MOD_INTERLEAVED;
    layer_para->fb.format = DISP_FORMAT_ARGB8888;
    layer_para->fb.br_swap = 0;
    layer_para->fb.seq = DISP_SEQ_ARGB;
    layer_para->ck_enable        = 0;
	layer_para->mode           = DISP_LAYER_WORK_MODE_NORMAL;
	layer_para->alpha_en       = 1;
	layer_para->alpha_val      = 0xff;
	layer_para->pipe           = 0;
	layer_para->src_win.x      = 0;
	layer_para->src_win.y      = 0;
	layer_para->src_win.width  = PictureInfo->Width;
	layer_para->src_win.height = PictureInfo->Height;
	layer_para->scn_win.x      = (screen_width - PictureInfo->Width) / 2;  	/* 居中 */
	layer_para->scn_win.y      = (screen_height - PictureInfo->Height) / 2;	/* 居中 */
	layer_para->scn_win.width  = PictureInfo->Width;
	layer_para->scn_win.height = PictureInfo->Height;

    return layer_para;
}

/*
*******************************************************************************
*                     ui_FreeLayerPara
*
* Description:
*    释放一个图层参数
*
* Parameters:
*    layer_para  :  input. 图层参数
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
__s32 ui_FreeLayerPara(display_layer_info_t *layer_para)
{
    if(layer_para){
		wBoot_free(layer_para);
	}

	return 0;
}

/*
*******************************************************************************
*                     ShowLayer
*
* Description:
*    显示图层
*
* Parameters:
*    layer_hdl   :  input. 图层句柄
*    layer_para  :  input. 图层参数
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
__s32 ShowLayer(__hdle layer_hdl, display_layer_info_t *layer_para, __s32 layer_source)
{
	__s32 ret = 0;

    if(!layer_source)
    {
	    WaitForDeInitFinish();
    }
	ret = De_SetLayerPara(layer_hdl, layer_para);
	if(ret != 0){
		DMSG_PANIC("ERR: De_SetLayerPara failed\n");
		return -1;
	}
	/* 显示 */
	De_OpenLayer(layer_hdl);

	return 0;
}
/*
*******************************************************************************
*                     ShowLayer
*
* Description:
*    显示图层
*
* Parameters:
*    layer_hdl   :  input. 图层句柄
*    layer_para  :  input. 图层参数
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
__s32 ShowLayer1(__hdle layer_hdl, display_layer_info_t *layer_para, __s32 layer_source)
{
	__s32 ret = 0;

    //if(!layer_source)
    //{
	//    WaitForDeInitFinish();
    //}
	ret = De_SetLayerPara(layer_hdl, layer_para);
	if(ret != 0){
		DMSG_PANIC("ERR: De_SetLayerPara failed\n");
		return -1;
	}
	/* 显示 */
	De_OpenLayer(layer_hdl);

	return 0;
}
/*
*******************************************************************************
*                     WaitForDeInitFinish
*
* Description:
*   等待de初始化完毕
*
* Parameters:
*   void
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*   void
*
*******************************************************************************
*/
__s32 WaitForDeInitFinish(void)
{
	__s32 ret;
	__s32 timedly = 2000;
	__s32 check_time = timedly/50;

	do
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
		wBoot_timer_delay(50);
		check_time --;
		if(check_time <= 0)
		{
			return -1;
		}
	}
	while(1);

	return 0;
}

/*
*******************************************************************************
*                     ShowPictureEx
*
* Description:
*   把图片数据解析到指定的地址中，并且显示出来.
* 如果指定的地址为NULL, 则可以存放在任何地址。
*
* Parameters:
*   Para  	:  input.  Boot阶段的参数。
*   Path 	:  input.  图片存放在介质中的位置，如“c:\logo.bmp”
*   Addr	:  input.  存放解析后的图片,
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*   void
*
*******************************************************************************
*/
__u32 ShowPictureEx(char *Path, __u32 address)
{
	Picture_t PictureInfo;
	__s32 ret = 0;
	display_layer_info_t *layer_para = NULL;

	/* 参数初始化 */
	if(!board_res.layer_hd)
    {
        return 0;
    }
	memset(&PictureInfo, 0, sizeof(Picture_t));

	ret = Parse_Pic_BMP_ByPath(Path, &PictureInfo, address);
	if(ret != 0)
	{
		DMSG_PANIC("ERR: Parse_Pic_BMP failed\n");
		goto error;
	}

	/* 显示图片 */
	layer_para = ui_AllocLayerPara(&PictureInfo);
	ShowLayer(board_res.layer_hd, layer_para, board_res.display_source);
	//wBoot_timer_delay(50);

    return (__u32)layer_para;

error:

	return 0;
}

/*
*******************************************************************************
*                     ShowPictureEx
*
* Description:
*   把图片数据解析到指定的地址中，并且显示出来.
* 如果指定的地址为NULL, 则可以存放在任何地址。
*
* Parameters:
*   Para  	:  input.  Boot阶段的参数。
*   Path 	:  input.  图片存放在介质中的位置，如“c:\logo.bmp”
*   Addr	:  input.  存放解析后的图片,
*
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*   void
*
*******************************************************************************
*/
__u32 ShowPictureEx1(char *Path, __u32 address)
{
	Picture_t PictureInfo;
	__s32 ret = 0;
	display_layer_info_t *layer_para = NULL;

	/* 参数初始化 */
	if(!board_res.layer_hd)
    {
        return 0;
    }
	memset(&PictureInfo, 0, sizeof(Picture_t));
	ret = Parse_Pic_BMP_ByPath(Path, &PictureInfo, address);
	if(ret != 0)
	{
		DMSG_PANIC("ERR: Parse_Pic_BMP failed\n");
		goto error;
	}
	layer_para = ui_AllocLayerPara(&PictureInfo);
	ShowLayer1(board_res.layer_hd, layer_para, board_res.display_source);

    return (__u32)layer_para;

error:

	return 0;
}

__s32 ShutPictureEx(__u32 pic_hd)
{
	display_layer_info_t *layer_para = NULL;

	if(!pic_hd)
	{
		return 0;
	}

	layer_para = (display_layer_info_t *)pic_hd;
	De_CloseLayer(board_res.layer_hd);
	ui_FreeLayerPara(layer_para);

	return 0;
}



__u32 ShowBatteryCharge_init(int rate)
{
	bat_charge_show *battery_info = NULL;
	__s32 			 ret = 0, i;
	char             pic_name[32];

	/* 参数初始化 */
	if(!board_res.layer_hd)
    {
        return 0;
    }
    battery_info = wBoot_malloc(sizeof(bat_charge_show));
    if(!battery_info)
    {
    	return 0;
    }
    memset(battery_info, 0, sizeof(bat_charge_show));
    memset(pic_name, 0, 32);
    strcpy(pic_name, "c:\\os_show\\bat0.bmp");

	for(i=0;i<10;i++)
	{
		pic_name[14] = '0' + i;
		ret = Parse_Pic_BMP_ByPath(pic_name, &battery_info->battery[i], 0);
		if(ret != 0)
		{
			__inf("parser bmp file %s failed\n", pic_name);
			return 0;
		}
	}
	ret = Parse_Pic_BMP_ByPath("c:\\os_show\\bat10.bmp", &battery_info->battery[10], 0);
	if(ret != 0)
	{
		__inf("parser bmp file c:\\os_show\\bat10.bmp failed\n");
		return 0;
	}

	battery_info->PictureInfo.BitCount = battery_info->battery[0].BitCount;
	battery_info->PictureInfo.Height = battery_info->battery[0].Height;
	battery_info->PictureInfo.Width  = battery_info->battery[0].Width;
	battery_info->PictureInfo.BufferSize = battery_info->PictureInfo.Width * battery_info->PictureInfo.Height * 4;
	battery_info->PictureInfo.Buffer = wBoot_malloc(battery_info->PictureInfo.BufferSize);
	battery_info->PictureInfo.RowSize = battery_info->PictureInfo.Width * 4;
	battery_info->buffersize = battery_info->PictureInfo.BufferSize;
	if(!battery_info->PictureInfo.Buffer)
	{
		return 0;
	}
	memcpy(battery_info->PictureInfo.Buffer, battery_info->battery[0].Buffer, battery_info->PictureInfo.BufferSize);
	/* 显示图片 */
	battery_info->layer_para = ui_AllocLayerPara(&battery_info->PictureInfo);
	ShowLayer(board_res.layer_hd, battery_info->layer_para, board_res.display_source);

    return (__u32)battery_info;
}


__s32 ShowBatteryCharge_exit(__u32 pic_hd)
{
	bat_charge_show *battery_info = NULL;

	/* 参数初始化 */
	if(!pic_hd)
    {
        return -1;
    }
    battery_info = (bat_charge_show *)pic_hd;

	De_CloseLayer(board_res.layer_hd);
	ui_FreeLayerPara(battery_info->layer_para);

    return 0;
}

int ShowBatteryCharge_reset(__u32 pic_hd)
{
	bat_charge_show *battery_info = NULL;

	if(!pic_hd)
    {
        return -1;
    }

	battery_info = (bat_charge_show *)pic_hd;

    battery_info->layer_para->alpha_en = 1;
    battery_info->layer_para->alpha_val = 255;
    wBoot_timer_delay(50);
    De_SetLayerPara(board_res.layer_hd, battery_info->layer_para);
}

__s32 ShowBatteryCharge_degrade(__u32 pic_hd, int step_time)
{
	bat_charge_show *battery_info = NULL;
	int  alpha_step;
	int  alpha, delay_time;
	//int  i;

	if(!pic_hd)
    {
        return -1;
    }

    battery_info = (bat_charge_show *)pic_hd;
	alpha_step = 5;
	delay_time = 50;
	alpha = battery_info->layer_para->alpha_val;

	battery_info->layer_para->alpha_en = 1;
	alpha -= alpha_step;
	if(alpha > 0)
	{
		De_SetLayerPara(board_res.layer_hd, battery_info->layer_para);
		wBoot_timer_delay(delay_time);
		battery_info->layer_para->alpha_val = alpha;
	}
	else
	{
		return -1;
	}
//	else
//	{
//		break;
//	}
//	}

	return 0;
}

__s32 ShowBatteryCharge_rate(__u32 pic_hd, int rate)
{
	bat_charge_show 	*battery_info = NULL;

	/* 参数初始化 */
	if(!pic_hd)
    {
        return -1;
    }
    battery_info = (bat_charge_show *)pic_hd;

	if(rate != 100)
	{
		memcpy(battery_info->PictureInfo.Buffer, battery_info->battery[rate/10].Buffer, battery_info->buffersize);
	}
	else
	{
		memcpy(battery_info->PictureInfo.Buffer, battery_info->battery[10].Buffer, battery_info->buffersize);
	}

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
display_layer_info_t *ui_multi_AllocLayerPara(display_lcd_set_t  *lcd_info, __u32 address)
{
	display_layer_info_t *layer_para = NULL;
	__u32 screen_width = 0;
	__u32 screen_height = 0;
	__u32 buffer_size;

	layer_para = (display_layer_info_t *)wBoot_malloc(sizeof(display_layer_info_t));
	if(layer_para == NULL)
	{
		DMSG_PANIC("ERR: wBoot_malloc failed\n");
		return NULL;
	}
	memset(layer_para, 0, sizeof(display_layer_info_t));
    /* 初始化图形参数 */
	screen_width  = De_GetSceenWidth();
	screen_height = De_GetSceenHeight();
	//根据屏幕的大小，申请framebuffer的大小
	lcd_info->lcd_width  = screen_width;
	lcd_info->lcd_height = screen_height;
	buffer_size = screen_width * screen_height * 4;
	if(!address)
	{
        lcd_info->display_buffer = wBoot_malloc(buffer_size);
        if(!lcd_info->display_buffer)
        {
            __inf("multi displya fail: malloc memory for display buffer failed\n");

            wBoot_free(layer_para);
            return NULL;
        }
    }
    else
    {
        lcd_info->display_buffer = (__u32 *)address;
    }
    memset(lcd_info->display_buffer, 0, buffer_size);

    layer_para->fb.size.width 		= screen_width;
    layer_para->fb.size.height 		= screen_height;
    layer_para->fb.addr[0] 			= (__u32)lcd_info->display_buffer;
    layer_para->fb.mode = DISP_MOD_INTERLEAVED;
    layer_para->fb.format = DISP_FORMAT_ARGB8888;
    layer_para->fb.br_swap = 0;
    layer_para->fb.seq = DISP_SEQ_ARGB;
    layer_para->ck_enable        = 0;
	layer_para->mode           = DISP_LAYER_WORK_MODE_NORMAL;
	layer_para->alpha_en       = 1;
	layer_para->alpha_val      = 0xff;
	layer_para->pipe           = 0;
	layer_para->src_win.x      = 0;
	layer_para->src_win.y      = 0;
	layer_para->src_win.width  = screen_width;
	layer_para->src_win.height = screen_height;
	layer_para->scn_win.x      = 0;
	layer_para->scn_win.y      = 0;
	layer_para->scn_win.width  = screen_width;
	layer_para->scn_win.height = screen_height;

    return layer_para;
}
/*
*******************************************************************************
*                     ShowPicture
*
* Description:
*   以图片的存储路径来显示一张图片
*
* Parameters:
*   Path    :  需要显示的图片路径，只支持BMP格
*   picture_group : 图片个数，最多同时有8个
*   show_way:  显示方式       参数保留，暂时不支持
*              要求所有图片的大小一致
* Return value:
*    0  :  成功
*   !0  :  失败
*
* note:
*   void
*
*******************************************************************************
*/
__hdle ShowMultiPicture(pic_name_info_t *pic_name_info, __u32 picture_group, __u32 show_way)
{
    __s32     ret;
    __u32     i, j = 0;
    __u32     tmp_left, tmp_top, tmp_width;
    char     *tmp_addr, *base_tmp_addr;
    char     *tmp_pic_addr;
    __u32     pic_width, pic_height;
    ui_show_multi_set_t  *pic_info = NULL;

    /* 参数初始化 */
    if(!board_res.layer_hd)
    {
        return (__hdle)0;
    }
	//寻找出所有的图片格式
	if(picture_group > 8)
	{
	    picture_group = 8;
	}
	pic_info = (ui_show_multi_set_t *)wBoot_malloc(sizeof(ui_show_multi_set_t));
	if(!pic_info)
	{
	    return (__hdle)0;
	}
	memset((void *)pic_info, 0, sizeof(ui_show_multi_set_t));
	pic_info->pic_total = picture_group;
	for(i=0;i<picture_group;i++)
	{
	    for(j=0;j<pic_name_info[i].valid_num;j++)
	    {
	        ret = Parse_Pic_BMP_ByPath(pic_name_info[i].name[j], &(pic_info->pic[i].PictureInfo[pic_info->pic[i].valid_num]), 0);
    	    if(ret)
            {
                __inf("decode %s bmp file failed\n", pic_name_info[i].name[j]);
            }
            else
            {
                pic_info->pic[i].valid_num ++;
            }
        }
	}
	//申请一个图层用于显示，并获取display buffer的地址
    pic_info->layer_para = ui_multi_AllocLayerPara((void *)&(pic_info->lcd_info), 0);
    //根据获取的用户数据，开始填充参数
    //暂时只做左右排列

    //按照从左到右摆放的方式，寻找每个编组的图应该在的位置
    pic_width    = pic_info->pic[0].PictureInfo[0].Width;
    pic_height   = pic_info->pic[0].PictureInfo[0].Height;

    tmp_top  = ( pic_info->lcd_info.lcd_height - pic_height)>>1;
    tmp_width= pic_info->lcd_info.lcd_width/picture_group;
    tmp_left = (tmp_width - pic_width)>>1;
    for(i=0;i<picture_group;i++)
    {
        pic_info->pic[i].left = tmp_left + i * tmp_width;
        pic_info->pic[i].top  = tmp_top;
    }
    //开始贴图，第一次
    base_tmp_addr = (char *)pic_info->lcd_info.display_buffer;
    base_tmp_addr += tmp_top * pic_info->lcd_info.lcd_width * 4;
    //开始一个图一个图的开始贴图
    for(i=0;i<picture_group;i++)
    {
        tmp_addr = base_tmp_addr + (pic_info->pic[i].left<<2);
        tmp_pic_addr = (char *)pic_info->pic[i].PictureInfo[0].Buffer;
        for(j=0;j<pic_height;j++)
        {
            memcpy(tmp_addr, tmp_pic_addr, pic_width << 2);
            tmp_addr += pic_info->lcd_info.lcd_width * 4;
            tmp_pic_addr += pic_width << 2;
        }
    }
	/* 显示图片 */
	ShowLayer(board_res.layer_hd, pic_info->layer_para, board_res.display_source);
    wBoot_timer_delay(50);

	return (__hdle)pic_info;
}
//重置图形，使所有显示的图形回复到第0副图
__s32 ui_reset_pic(__hdle show_hd)
{
    ui_show_multi_set_t  *pic_info = (ui_show_multi_set_t *)show_hd;
    char  *base_tmp_addr, *tmp_addr, *tmp_pic_addr;
    __u32  tmp_top, pic_width, pic_height;
    __u32  i, j;

    if(!show_hd)
    {
        return -1;
    }
    pic_width  = pic_info->pic[0].PictureInfo[0].Width;
    pic_height = pic_info->pic[0].PictureInfo[0].Height;

    tmp_top  = ( pic_info->lcd_info.lcd_height - pic_height)>>1;
    base_tmp_addr = (char *)pic_info->lcd_info.display_buffer;
    base_tmp_addr += tmp_top * pic_info->lcd_info.lcd_width * 4;
    //开始贴图
    for(i=0;i<pic_info->pic_total;i++)
    {
        tmp_addr = base_tmp_addr + (pic_info->pic[i].left<<2);
        tmp_pic_addr = (char *)pic_info->pic[i].PictureInfo[0].Buffer;
        for(j=0;j<pic_height;j++)
        {
            memcpy(tmp_addr, tmp_pic_addr, pic_width << 2);
            tmp_addr += pic_info->lcd_info.lcd_width * 4;
            tmp_pic_addr += pic_width << 2;
        }
        pic_info->pic[i].pic_turn = 0;
    }

    wBoot_timer_delay(75);

    return 0;
}
//每个OS可以使用最多4个贴图，当选中任意一个OS的时候，使用
//当前OS的贴图轮流贴，呈现闪烁的效果
__s32 ui_coruscate_pic(__hdle show_hd, __u32 sel_num)
{
    ui_show_multi_set_t  *pic_info = (ui_show_multi_set_t *)show_hd;
    __u32 tmp_pic_index;
    char  *base_tmp_addr, *tmp_addr, *tmp_pic_addr;
    __u32  tmp_top, pic_width, pic_height;
    __u32  j;

    if(!show_hd)
    {
        return -1;
    }
    if(sel_num >= pic_info->pic_total)
    {
        return -1;
    }
    //获取当前显示图片的顺序
    tmp_pic_index = pic_info->pic[sel_num].pic_turn;

    pic_width  = pic_info->pic[0].PictureInfo[0].Width;
    pic_height = pic_info->pic[0].PictureInfo[0].Height;

    tmp_top  = ( pic_info->lcd_info.lcd_height - pic_height)>>1;
    base_tmp_addr = (char *)pic_info->lcd_info.display_buffer;
    base_tmp_addr += tmp_top * pic_info->lcd_info.lcd_width * 4;
    //开始贴图
    tmp_addr = base_tmp_addr + (pic_info->pic[sel_num].left<<2);
    tmp_pic_addr = (char *)pic_info->pic[sel_num].PictureInfo[tmp_pic_index].Buffer;
    for(j=0;j<pic_height;j++)
    {
        memcpy(tmp_addr, tmp_pic_addr, pic_width << 2);
        tmp_addr += pic_info->lcd_info.lcd_width * 4;
        tmp_pic_addr += pic_width << 2;
    }
    //调整下次的顺序
    tmp_pic_index ++;
    if(tmp_pic_index >= pic_info->pic[sel_num].valid_num)
    {
        tmp_pic_index = 0;
    }
    pic_info->pic[sel_num].pic_turn = tmp_pic_index;

    wBoot_timer_delay(250);

    return 0;
}

__s32 ui_release_resource(__hdle show_hd)
{
    ui_show_multi_set_t *pic_info = (ui_show_multi_set_t *)show_hd;
    __u32  i, j;
    __s32  *addr;

    if(!show_hd)
    {
        return -1;
    }
    De_CloseLayer(board_res.layer_hd);
    addr = (__s32 *)pic_info->lcd_info.display_buffer;
    for(i=0;i<pic_info->lcd_info.lcd_height;i++)
    {
        for(j=0;j<pic_info->lcd_info.lcd_width;j++)
        {
            *addr = 0xff000000;
            addr ++;
        }
    }
    wBoot_free(pic_info->layer_para);
    wBoot_free(pic_info);
    wBoot_timer_delay(50);

    return 0;
}


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
#include  "common_res.h"
#include  "../sfte/FontEngine.h"

display_info_set_t  display_info;

__u32  WORD_SIZE;
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
	__u32 time = 0;

	if(De_IsLCDOpen() < 0){
		DMSG_PANIC("ERR: Not need to wait LCD open\n");
		return -1;
	}

    /* 最大超时时间为 5s */
	time = 500;
	while(!De_IsLCDOpen() && time--)
	{
//		DMSG_INFO("[MSG]: Wait for lcd open finish\n");
		wBoot_timer_delay(10);
	}

//	if(!time){
//		DMSG_PANIC("ERR: Wait for lcd open finish timeout\n");
//	}

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
__s32 ShowLayer(__hdle layer_hdl, display_layer_info_t *layer_para)
{
	__s32 ret = 0;

	WaitForDeInitFinish();
	ret = De_SetLayerPara(layer_hdl, layer_para);
	if(ret != 0)
	{
		DMSG_PANIC("ERR: De_SetLayerPara failed\n");
		return -1;
	}
	/* 显示 */
	De_OpenLayer(layer_hdl);

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
display_layer_info_t *LCD_DisplayAllocLayerPara(void)
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
	display_info.screen_width  = screen_width;
	display_info.screen_height = screen_height;
	buffer_size                = screen_width * screen_height * 4 * 2;

    display_info.screen_buf = wBoot_malloc(buffer_size);
    if(!display_info.screen_buf)
    {
        __wrn("LCD_DisplayAllocLayerPara: malloc memory for display buffer failed\n");

        wBoot_free(layer_para);

        return NULL;
    }

    memset(display_info.screen_buf, 0, buffer_size);

    layer_para->fb.size.width 		= screen_width;
    layer_para->fb.size.height 		= screen_height;
    layer_para->fb.addr[0] 			= (__u32)display_info.screen_buf;
    layer_para->fb.mode = DISP_MOD_INTERLEAVED;
    layer_para->fb.format = DISP_FORMAT_ARGB8888;
    layer_para->fb.br_swap = 0;
    layer_para->fb.seq = DISP_SEQ_ARGB;
    layer_para->ck_enable        = 0;
	layer_para->mode           = DISP_LAYER_WORK_MODE_NORMAL;
	layer_para->alpha_en       = 0;
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

    display_info.crt_addr = display_info.screen_buf;

    display_info.total_height = ((buffer_size / 4) / (screen_width)) / (WORD_SIZE);   //总的高度，可以显示的行数

    display_info.rest_screen_height  = display_info.total_height - 1;          //记录屏幕的剩余高度，行数, 剩余1行不用
    display_info.rest_display_height = screen_height/WORD_SIZE - 1;    //记录显示的剩余高度，行数，剩余1行不用
    display_info.rest_screen_width   = screen_width;                        //剩余宽度等于显示宽度, 像素单位
    display_info.current_height = 0;
    display_info.fb_addr        = display_info.screen_buf;
    display_info.x              = 0;
    display_info.y              = 0;

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
__s32 LCD_DisplayInit(void)
{
    __u32 	   screen_width = 0;
    char       font_name[16];

    display_layer_info_t *layer_para = NULL;
    /* 参数初始化 */
    memset(&display_info, 0, sizeof(display_info_set_t));
    memset(font_name, 0, 16);
	//获取字体大小
	screen_width = De_GetSceenWidth();
	if(screen_width > 480)
	{
		WORD_SIZE = 32;
		strcpy(font_name, "c:\\font32.sft");
	}
	else
	{
		WORD_SIZE = 24;
		strcpy(font_name, "c:\\font24.sft");
    }
    //申请一个图层用于显示，并获取display buffer的地址
    layer_para = LCD_DisplayAllocLayerPara();
    if(!layer_para)
    {
        return -1;
    }
    open_font(font_name, WORD_SIZE, display_info.screen_width,  display_info.screen_buf);   //打开字库
    /* 显示图片 */
	ShowLayer(board_res.layer_hd, layer_para);
    wBoot_timer_delay(50);

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
__s32 LCD_Change_frame_buffer(__u8 *addr)
{
    __disp_fb_t  fb_buffer;

    De_GetFrameBuffer(board_res.layer_hd, &fb_buffer);
    fb_buffer.addr[0] = (__u32)addr;
    De_SetFrameBuffer(board_res.layer_hd, &fb_buffer);

    return 0;
}






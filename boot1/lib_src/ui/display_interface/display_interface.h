/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: Display.h
*
* Author 		: javen
*
* Description 	: 显示输出操作函数集
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/
#ifndef  __DISPLAY_H__
#define  __DISPLAY_H__

#include "types.h"

__s32 De_OpenDevice(__s32 display_source, __s32 tv_mode, __s32 display_mode);
__s32 De_CloseDevice(__s32 display_source);

__s32 De_IsLCDOpen(void);
__s32 De_IsLCDClose(void);
__s32 De_ReleaseLayer(__s32 layer_hdl);
__s32 De_OpenLayer(__s32 layer_hdl);
__s32 De_CloseLayer(__s32 layer_hdl);


__s32 De_SetBlkColor(__disp_color_t *Color);
__hdle De_RequestLayer(__disp_layer_work_mode_t mode);
__s32 De_GetFrameBuffer(__s32 layer_hdl, __disp_fb_t *FrameBuffer);
__s32 De_SetFrameBuffer(__s32 layer_hdl, __disp_fb_t *FrameBuffer);
__s32 De_GetLayerPara(__s32 layer_hdl, __disp_layer_info_t * layer_para);
__s32 De_SetLayerPara(__s32 layer_hdl, __disp_layer_info_t * layer_para);


__u32 De_GetSceenWidth(void);
__u32 De_GetSceenHeight(void);


#endif   //__DISPLAY_H__


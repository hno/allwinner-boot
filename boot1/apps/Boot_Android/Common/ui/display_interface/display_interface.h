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
#ifndef  __DISPLAY_H__
#define  __DISPLAY_H__

#include "types.h"

__s32 De_OpenDevice(__s32 display_source);
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


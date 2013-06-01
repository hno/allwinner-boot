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
#ifndef  __UI_H__
#define  __UI_H__

#include  "types.h"
#include  "Picture//picture.h"
#include  "display_interface//display_interface.h"

#define    BMP_SHOW_COUNT_MAX       (16)

typedef  __disp_layer_info_t   display_layer_info_t;

typedef  struct display_lcd
{
    void  *display_buffer;
    __u32  lcd_width;
    __u32  lcd_height;
    __u32  reserved;
}
display_lcd_set_t;

typedef struct pic_name_info
{
    __u32  valid_num;
    char   name[4][32];
}
pic_name_info_t;

typedef struct pic_show_info
{
    __u32       left;
    __u32       top;
    __u32       valid_num;
    __u32       pic_turn;
    Picture_t   PictureInfo[4];
}
pic_show_info_t;

typedef  struct  _ui_show_multi
{
    __u32                 pic_total;
    display_lcd_set_t     lcd_info;
    display_layer_info_t *layer_para;
    pic_show_info_t       pic[8];
}
ui_show_multi_set_t;


typedef struct _bat_charge_show
{
	display_layer_info_t  *layer_para;
	Picture_t			   battery[11];
	Picture_t 			   PictureInfo;
	__u32				   buffersize;
}
bat_charge_show;
//---------------------------------------------------------------
//  ∫Í ∂®“Â
//---------------------------------------------------------------
#define  BOOT_PICTURE_TYPE_NULL         0x00
#define  BOOT_PICTURE_TYPE_BMP          0x01
#define  BOOT_PICTURE_TYPE_JPG          0x02

extern  __s32 WaitForDeInitFinish(void);

extern  display_layer_info_t *ui_AllocLayerPara(Picture_t *PictureInfo);
extern  __s32                 ui_FreeLayerPara(display_layer_info_t *layer_para);
extern  __s32                 ShowLayer(__hdle layer_hdl, display_layer_info_t *layer_para, __s32 layer_source);

extern  __u32                 ShowPictureEx(char *Path, __u32 address);
extern  __s32 				  ShutPictureEx(__u32 pic_hd);

extern  display_layer_info_t *ui_multi_AllocLayerPara(display_lcd_set_t  *lcd_info, __u32 address);
extern  __hdle                ShowMultiPicture(pic_name_info_t *pic_name_info, __u32 picture_group, __u32 show_way);
extern  __s32                 ui_coruscate_pic(__hdle show_hd, __u32 sel_num);
extern  __s32                 ui_release_resource(__hdle show_hd);
extern  __s32                 ui_reset_pic(__hdle show_hd);


extern  __u32 				  ShowBatteryCharge_init(int rate);
extern  __s32 				  ShowBatteryCharge_exit(__u32 pic_hd);
extern  __s32 				  ShowBatteryCharge_rate(__u32 pic_hd, int rate);
extern  __s32 				  ShowBatteryCharge_degrade(__u32 pic_hd, int step_time);

#endif   //__UI_H__


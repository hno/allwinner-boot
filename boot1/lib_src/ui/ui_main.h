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
#ifndef  _ui_main_h_
#define  _ui_main_h_

#include  "types.h"
#include  "ui_color.h"
#include  "ui_progressbar/ui_progressbar.h"
#include  "ui_char/ui_char.h"

#define  BOOT_UI_INIT_SUCCESSED					(0)

#define  BOOT_UI_DISPLAY_SOURCE_INVALID         (-1)
#define  BOOT_UI_DISPLAY_MODE_INVALID			(-2)
#define  BOOT_UI_DISPLAY_DRIVER_NOT_EXIST		(-3)
#define  BOOT_UI_DISPLAY_DRIVER_OPEN_FAIL		(-4)
#define  BOOT_UI_DISPLAY_DEVICE_OPEN_FAIL		(-5)
#define  BOOT_UI_DISPLAY_LAYER_REQUEST_FAIL		(-6)
#define  BOOT_UI_SCREEN_INFO_FAIL				(-7)


extern  __s32 boot_ui_init(__s32 display_source, __s32 display_mode);
extern  __s32 boot_ui_exit(int ui_exit_mode);
extern  __s32 boot_ui_check_device_open(void);


extern  __s32 boot_ui_draw_hollowrectangle(__s32 x1, __s32 y1, __s32 x2, __s32 y2);
extern  __s32 boot_ui_draw_solidrectangle (__s32 x1, __s32 y1, __s32 x2, __s32 y2);
extern  __s32 boot_ui_draw_line(__s32 x1, __s32 y1, __s32 x2, __s32 y2);
extern  __s32 boot_ui_draw_lineto(__s32 x, __s32 y);
extern  __s32 boot_ui_draw_point(__s32 x, __s32 y);
extern  __s32 boot_ui_set_color(__s32 color);
extern  __s32 boot_ui_get_color(void);
extern  __s32 boot_ui_moveto(int x, int y);
extern  __s32 boot_ui_clear(void);

#endif  /* _ui_main_h_ */

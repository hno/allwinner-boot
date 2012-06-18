/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod demo sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : drv_demo.h
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

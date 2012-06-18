/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: ui.c
*
* Author 		: javen
*
* Description 	: 图形显示输出操作函数集
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/
#ifndef  __UI_DRAW_H__
#define  __UI_DRAW_H__

extern  __s32 boot_ui_draw_hollowrectangle(__s32 x1, __s32 y1, __s32 x2, __s32 y2);
extern  __s32 boot_ui_draw_solidrectangle (__s32 x1, __s32 y1, __s32 x2, __s32 y2);
extern  __s32 boot_ui_draw_line(__s32 x1, __s32 y1, __s32 x2, __s32 y2);
extern  __s32 boot_ui_draw_lineto(__s32 x, __s32 y);
extern  __s32 boot_ui_draw_point(__s32 x, __s32 y);
extern  __s32 boot_ui_set_color(__s32 color);
extern  __s32 boot_ui_get_color(void);
extern  __s32 boot_ui_moveto(int x, int y);


#endif   //__UI_DRAW_H__


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
#ifndef  __UI_CONTROL_H__
#define  __UI_CONTROL_H__


extern  unsigned	boot_ui_progressbar_create(int x1, int y1, int x2, int y2);
extern  int       	boot_ui_progressbar_config(unsigned p, int frame_color, int progress_color, int thickness);

extern  int 		boot_ui_progressbar_active(unsigned p);
extern  int 		boot_ui_progressbar_destroy(unsigned p);
extern  int 		boot_ui_progressbar_upgrate(unsigned p, int rate);



#endif   //__UI_CONTROL_H__


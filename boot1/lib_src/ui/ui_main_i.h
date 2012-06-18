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
#ifndef  _ui_main_I_H_
#define  _ui_main_I_H_

#include "egon2.h"
#include "string.h"

typedef struct _ui_main_source_t
{
	__u32   disp_hd;
	__u32   layer_hdl;
	int 	screen_width;
	int 	screen_height;
	int 	screen_size;
	__u32   color;
	int     this_x;
	int     this_y;
	char 	*screen_buf;
	__disp_layer_info_t  *layer_para;
}
ui_main_source_t;


extern  ui_main_source_t  ui_source;


#endif  /* _ui_main_I_H_ */

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
#ifndef  __UI_H__
#define  __UI_H__

#include  "types.h"
#include  "display_interface/display_interface.h"

#define BAS_SCREEN_SIZE                (3072000)
#define DECOMPRESS_UI_PIC_MAX           8
#define FILE_NAME_MAX                   16

typedef struct __display_info_set
{
    __u8   *screen_buf;                       //存储屏幕地址
    __u32   screen_height;                    //存储屏幕高度，字符单位，行
    __u32   screen_width;                     //存储屏幕宽度
    __u8   *crt_addr;                         //当前用于显示的地址
    __u8   *fb_addr;                          //图层地址
    __u32   rest_screen_height;               //剩余的存储屏幕高度，剩余总高度, 字符单位，行
    __u32   rest_screen_width;                //剩余的存储屏幕宽度, 剩余总宽度, 字符单位，行
    __u32   rest_display_height;              //剩余的显示高度
    __u32   total_height;                     //用于显示总的高度
    __u32   current_height;                   //当前已经使用的高度
    __u32   x;                                //显示位置的x坐标
    __u32   y;                                //显示位置的y坐标
}
display_info_set_t;


typedef struct _shell_char_t
{
    char    name;                             //字符，使用了32位描述，实际是一个char类型
    __u32   x;                                //显示字符的x坐标
    __u32   y;                                //显示字符的y坐标
    __u32   width;                            //字符的显示宽度
    __u32   *addr;                            //字符显示时候的起始地址
}
_shell_char_set_t;

typedef struct _debug_shell_t
{
    __u32                index;               //记录当前用于打印的字符的序号
    __u32                tail;                //末尾字符编号，不存在的字符
    _shell_char_set_t    str[260];            //保存用于打印的字符
}
_debug_shell_set_t;



typedef struct _cursor_t
{
    volatile __u32       x;                   //光标的x坐标
    volatile __u32       y;                   //光标的y坐标

}
_cursor_set_t;


typedef  __disp_layer_info_t   display_layer_info_t;


extern   __u32   WORD_SIZE;
//---------------------------------------------------------------
//  宏 定义
//---------------------------------------------------------------
#define  BOOT_PICTURE_TYPE_NULL         0x00
#define  BOOT_PICTURE_TYPE_BMP          0x01
#define  BOOT_PICTURE_TYPE_JPG          0x02

extern   int  LCD_DisplayInit(void);
extern   int  LCD_Change_frame_buffer(__u8 *addr);
extern   void debug_printf( const char * str, ...);
extern   int  debug_setdisplay_color(__u32 user_set_color);
extern   int  debug_setdisplay_clear(void);

#endif   //__UI_H__


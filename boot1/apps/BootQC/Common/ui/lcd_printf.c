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
#include <string.h>
#include <stdarg.h>
#include  "include.h"
#include  "ui.h"
#include  "format/format.h"
#include  "../sfte/FontEngine.h"


extern  display_info_set_t  display_info;


static  __u32 change_line = 0;
static  __u32 color = 0x00ff00;            //默认绿色

/*
**********************************************************************************************************************
*                                               _change_to_new_line
*
* Description:
*
* Arguments  :
*
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void  _change_to_new_line(void)
{
    __u32  tmp_size;

    wlibc_int_disable();
    display_info.rest_screen_width   = display_info.screen_width;   //作为新的一行的长度,像素单位

    display_info.rest_screen_height  -= 1;           //剩余的屏幕高度, 行数
    display_info.rest_display_height -= 1;           //剩余的显示高度，行数
    display_info.current_height      += 1;           //当前高度变多一行
    display_info.x                    = 0;           //
    display_info.y                    = display_info.current_height * WORD_SIZE;

    if(display_info.rest_display_height <= 0)               //剩余宽度不够了，需要切换回到第一个屏幕
    {
        tmp_size = (4 * display_info.screen_width) * (display_info.screen_height - WORD_SIZE);

        display_info.rest_screen_height = display_info.total_height - (display_info.screen_height/WORD_SIZE);          //剩余高度行数
        memcpy(display_info.screen_buf, display_info.fb_addr + 4 * display_info.screen_width * WORD_SIZE,  tmp_size);
        display_info.crt_addr = display_info.screen_buf + tmp_size;
        memset(display_info.screen_buf + tmp_size,  0,  4 * 2 * display_info.screen_width * WORD_SIZE);
        display_info.current_height = display_info.screen_height/WORD_SIZE - 2;
        display_info.rest_display_height  = 1;
        display_info.x                    = 0;           //
        display_info.y                    = display_info.current_height * WORD_SIZE;

        display_info.fb_addr = display_info.screen_buf;    //屏幕显示buffer地址
        LCD_Change_frame_buffer(display_info.fb_addr);
    }
    else if(display_info.rest_display_height <= 0)                         //当前窗口显示不下，需要向下滑动
    {
        tmp_size = 4 * display_info.screen_width * display_info.current_height * WORD_SIZE;

        memset(display_info.screen_buf + tmp_size,  0,  8 * display_info.screen_width * WORD_SIZE);       //清除掉下一行的数据
        display_info.crt_addr = display_info.screen_buf + tmp_size;                           //当前显示内容地址变换
        display_info.rest_display_height = 1;                                               //记录显示的剩余高度，行数

        display_info.fb_addr += 4 * display_info.screen_width * WORD_SIZE;                              //切换到下一行
        LCD_Change_frame_buffer(display_info.fb_addr);
    }
    else
    {
        tmp_size = 4 * display_info.screen_width * display_info.y;
        memset(display_info.screen_buf + tmp_size,  0,  4 * display_info.screen_width * WORD_SIZE);       //清除掉下一行的数据
        display_info.crt_addr = display_info.screen_buf + tmp_size;                           //当前显示内容地址变换
    }

    wlibc_int_enable();

    return;
}

/*
**********************************************************************************************************************
*                                               _debug_display_putchar
*
* Description:
*
* Arguments  :  ch         :  需要打印的字符
*               rest_width :  当前行剩余的宽度
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 _debug_display_putchar(__u8 ch)
{
    __s32 ret, width;

    wlibc_int_disable();

    ret = check_change_line(display_info.x, ch);
    if(ret == -1)             //访问失败，当前字符不处理
    {
        wlibc_int_enable();
        return -1;
    }
    else if(ret == 0)        //访问成功，当前字符处理，但是不需要换行
    {
        ;
    }
    else if(ret == 1)        //访问成功，当前字符处理，需要换行
    {
        _change_to_new_line( );
    }

    width = draw_bmp_ulc(display_info.x, display_info.y, color);    //显示字符,返回当前显示字符的宽度，像素单位
    display_info.x += width;
    display_info.rest_screen_width -= width;                        //记录当前行剩余的像素
    wlibc_int_enable();
    //调用打印函数

	return 0;
}
/*
**********************************************************************************************************************
*                                               debug_display_putstr
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void debug_display_putstr(const char * str)
{
	while( *str != '\0' )
	{
		if(*str == '\n')
		{
            //需要换行的时候，自动切换到下一行开始进行显示
            //调用换行函数
            __wrn("here\n");
            _change_to_new_line();
		}
		else
		{
			_debug_display_putchar(*str);
		}
		str++;
	}
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
__s32 debug_setdisplay_clear(void)
{
    memset((__u8 *)display_info.screen_buf, 0x0, display_info.screen_width * display_info.screen_height * 4 * 2);//全部初始化成背景色(0x0 黑色)
    display_info.crt_addr = display_info.screen_buf;

    display_info.rest_screen_height  = display_info.total_height - 1;            //记录屏幕的剩余高度，行数, 剩余1行不用
    display_info.rest_display_height = display_info.screen_height/WORD_SIZE - 1; //记录显示的剩余高度，行数，剩余1行不用
    display_info.rest_screen_width   = display_info.screen_width;                //剩余宽度等于显示宽度, 像素单位
    display_info.current_height = 0;
    display_info.fb_addr        = display_info.screen_buf;
    display_info.x              = 0;
    display_info.y              = 0;

    change_line    = 0;
    wBoot_timer_delay( 20 );

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
__s32 debug_setdisplay_color(__u32 user_set_color)
{
    color = user_set_color;

    return 0;
}
/*
******************************************************************************************************************
*
*Function Name : UART_printf
*
*Description : This function is to formatedly output through UART, similar to ANSI C function printf().
*                This function can support and only support the following Conversion Specifiers:
*              %d		Signed decimal integer.
*              %u		Unsigned decimal integer.
*              %x		Unsigned hexadecimal integer, using hex digits 0f.
*              %X		Unsigned hexadecimal integer, using hex digits 0F.
*              %c		Single character.
*              %s		Character string.
*              %p		A pointer.
*
*Input : refer to ANSI C function printf().
*
*Output : void, different form ANSI C function printf().
*
*call for : void int_to_string_dec( __s32 input, char * str ), defined in format_transformed.c.
*           void int_to_string_hex( __s32 input, char * str );  defined in format_transformed.c.
*           void Uint_to_string_dec( __u32 input, char * str );  defined in format_transformed.c.
*           void UART_putchar( __s32 ch); defined in boot loader.
*           void UART_puts( const char * string ); defined in boot loader.
*
*Others : None at present.
*
*******************************************************************************************************************
*/
void debug_printf( const char * str, ...)
{
	char string[13];
	char *p;
	__s32 hex_flag ;
	va_list argp;

	va_start( argp, str );

    if(change_line == 1)
    {
        _change_to_new_line();
        wBoot_timer_delay(30);
        change_line = 0;
    }

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			hex_flag = HEX_X;
			switch( *str )
			{
			    case 'u':
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          debug_display_putstr( p );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  debug_display_putstr( p );
                          ++str;
						  break;
				case 'c': _debug_display_putchar( va_arg( argp,  __s32 ) );
						  ++str;
						  break;
				case 's': debug_display_putstr( va_arg( argp, char * ) );
						  ++str;
						  break;
				case 'l':
					{
						__u32 high;
						__u32 low;

						if( str[1] == 'l' && ( str[2] == 'x' || str[2] == 'X' ) )
						{
							low = va_arg( argp, __u32 );
							high = va_arg( argp, __u32 );

							if( str[2] == 'x' )
								hex_flag = 'x';
							else
								hex_flag = 'X';
							int_to_string_hex( high, string, hex_flag );
							debug_display_putstr( p );
							int_to_string_hex( low, string, hex_flag );
							debug_display_putstr( p+2 );
							str += 3;
						  	break;
						}
						else
						{
							int_to_string_dec( va_arg( argp,  __s32 ), string );
                          	debug_display_putstr( p );
						  	++str;
						  	break;
						}
					}
				default : _debug_display_putchar( '%' );          // if current character is not Conversion Specifiers 'dxpXucs',
						  _debug_display_putchar( *str );         // output directly '%' and current character, and then
						  ++str;                        // let 'str' point to next character.
			}
		}

		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				change_line = 1;
				//_change_to_new_line();

			_debug_display_putchar( *str++ );
		}
	}

	va_end( argp );
}




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
#include <stdarg.h>
#include "format_transformed.h"
#include "egon2.h"


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
static __u32 mem_puts(const char *str, char *p )
{
    __u32 len = 0;

	while( *str != '\0' )
	{
		if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
		{
		    *p++ = '\r';
		    len ++;
        }
        *p++ = *str++;
        len ++;
	}

	return len;
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
void wlibc_uprintf( const char * str, ...)
{
	char string[16], str_store[256];
	char *p, *q = str_store;
	__s32 hex_flag ;
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			hex_flag = HEX_X;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          q += mem_puts( p, q );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  q += mem_puts( p, q );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  q += mem_puts( p, q );
						  ++str;
						  break;
				case 'c': *q++ = va_arg( argp,  __s32 );
						  ++str;
						  break;
				case 's': q += mem_puts( va_arg( argp, char * ), q );
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
							q += mem_puts( p, q );
							int_to_string_hex( low, string, hex_flag );
							q += mem_puts( p+2, q );
							str += 3;
						  	break;
						}
						else
						{
							int_to_string_dec( va_arg( argp,  __s32 ), string );
                          	q += mem_puts( p, q );
						  	++str;
						  	break;
						}
					}
				default : *q++ = '%';                                    // if current character is not Conversion Specifiers 'dxpXucs',
						  *q++ = *str++;                                 // output directly '%' and current character, and then
						                                                 // let 'str' point to next character.
			}
		}
		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				*q++ = '\r';

            *q++ = *str++;
		}
	}

	va_end( argp );

    *q = 0;

	wBoot_uprintf((void *)str_store);

	return ;
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
void wlibc_ntprintf( const char * str, ...)
{
	char string[16], str_store[256];
	char *p, *q = str_store;
	__s32 hex_flag ;
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			hex_flag = HEX_X;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          q += mem_puts( p, q );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  q += mem_puts( p, q );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  q += mem_puts( p, q );
						  ++str;
						  break;
				case 'c': *q++ = va_arg( argp,  __s32 );
						  ++str;
						  break;
				case 's': q += mem_puts( va_arg( argp, char * ), q );
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
							q += mem_puts( p, q );
							int_to_string_hex( low, string, hex_flag );
							q += mem_puts( p+2, q );
							str += 3;
						  	break;
						}
						else
						{
							int_to_string_dec( va_arg( argp,  __s32 ), string );
                          	q += mem_puts( p, q );
						  	++str;
						  	break;
						}
					}
				default : *q++ = '%';                                    // if current character is not Conversion Specifiers 'dxpXucs',
						  *q++ = *str++;                                 // output directly '%' and current character, and then
						                                                 // let 'str' point to next character.
			}
		}
		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				*q++ = '\r';

            *q++ = *str++;
		}
	}

	va_end( argp );

    *q = 0;

	wBoot_ntprintf((void *)str_store);

	return ;
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
void wlibc_sprintf( char *str_store, const char * str, ...)
{
	char string[16];
	char *p, *q = str_store;
	__s32 hex_flag;
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			hex_flag = HEX_X;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          q += mem_puts( p, q );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  q += mem_puts( p, q );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  q += mem_puts( p, q );
						  ++str;
						  break;
				case 'c': *q++ = va_arg( argp,  __s32 );
						  ++str;
						  break;
				case 's': q += mem_puts( va_arg( argp, char * ), q );
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
							q += mem_puts( p, q );
							int_to_string_hex( low, string, hex_flag );
							q += mem_puts( p+2, q );
							str += 3;
						  	break;
						}
						else
						{
							int_to_string_dec( va_arg( argp,  __s32 ), string );
                          	q += mem_puts( p, q );
						  	++str;
						  	break;
						}
					}
				default : *q++ = '%';                                    // if current character is not Conversion Specifiers 'dxpXucs',
						  *q++ = *str++;                                 // output directly '%' and current character, and then
						                                                 // let 'str' point to next character.
			}
		}
		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				*q++ = '\r';

            *q++ = *str++;
		}
	}

	va_end( argp );

    *q = 0;

	return ;
}



/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON libc sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : eGon2_common.c
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
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          q += mem_puts( p, q );
						  ++str;
						  break;
				case 'x':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string );
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
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          q += mem_puts( p, q );
						  ++str;
						  break;
				case 'x':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string );
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
	va_list argp;

	va_start( argp, str );

	while( *str )
	{
		if( *str == '%' )
		{
			++str;
			p = string;
			switch( *str )
			{
				case 'd': int_to_string_dec( va_arg( argp,  __s32 ), string );
                          q += mem_puts( p, q );
						  ++str;
						  break;
				case 'x':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string );
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



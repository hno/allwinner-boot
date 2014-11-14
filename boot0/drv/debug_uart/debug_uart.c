/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
*File Name : debug_UART.c
*
*Author : Gary.Wang
*
*Version : 1.1.0
*
*Date : 2008.04.29
*
*Description :
*
*Others : None at present.
*
*
* History :
*
*  <Author>         <time>      <version>     <description>
*
* Wangyugang      2008.04.29      1.1.0        build the file
*
*******************************************************************************************************************
*/
#include "boot0_drv_i.h"
#include <stdarg.h>
#include "format_transformed.h"
#include "uart_hd.h"


//char UART_getchar_with_display( __u32 timeout )
//{
//	char c;
//
//	c = UART_getchar( timeout );
//
//	if( c != 0)
//		UART_putchar( c );
//
//	return c;
//}



//void UART_puts( const char * str )
//{
//	while( *str != '\0' )
//	{
//		UART_putchar( *str++ );
//	}
//
//	UART_putchar('\r');
//	UART_putchar('\n');
//}



void UART_puts_no_newline( const char * str )
{
	while( *str != '\0' )
	{
		if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
			UART_putchar( '\r' );

		UART_putchar( *str++ );
	}
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
void UART_printf2( const char * str, ...)
{
	char string[13];
	char *p;
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
                          UART_puts_no_newline( p );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  UART_puts_no_newline( p );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  UART_puts_no_newline( p );
						  ++str;
						  break;
				case 'c': UART_putchar( va_arg( argp,  __s32 ) );
						  ++str;
						  break;
				case 's': UART_puts_no_newline( va_arg( argp, char * ) );
						  ++str;
						  break;
				default : UART_putchar( '%' );       // if current character is not Conversion Specifiers 'dxpXucs',
						  UART_putchar( *str );         // output directly '%' and current character, and then
						  ++str;                        // let 'str' point to next character.
			}
		}

		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				UART_putchar( '\r' );

			UART_putchar( *str++ );
		}
	}

	va_end( argp );
}



void eFG_printf( const char * str, ...)
{
}



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
#include "include.h"
#include <stdarg.h>
#include "format_transformed.h"
#include "sw_uart.h"

#define   SERIAL_PORT    serial_ports[serial_index]

static sw_uart_t serial_ports[8] = {
	(sw_uart_t)CFG_SW_SERIAL_COM0,
	(sw_uart_t)CFG_SW_SERIAL_COM1,
	(sw_uart_t)CFG_SW_SERIAL_COM2,
	(sw_uart_t)CFG_SW_SERIAL_COM3,
	(sw_uart_t)CFG_SW_SERIAL_COM4,
	(sw_uart_t)CFG_SW_SERIAL_COM5,
	(sw_uart_t)CFG_SW_SERIAL_COM6,
	(sw_uart_t)CFG_SW_SERIAL_COM7
};

static int serial_index = 0;

void eFG_printf( const char * str, ...);
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
char eGon2_getc( __u32 timeout )
{
	__u32  i;
	char   c;

	if( timeout == 0 )           // infinite wait
	{
	    do
	    {
	        c = sw_uart_getc(SERIAL_PORT);
	    }
		while(!c);

		return c;
	}
	else
	{
		for( i = 0; i < timeout; i++ )
		{
			c = sw_uart_getc(SERIAL_PORT);
			if(c)
			{
			    return c;
			}
		}
		if( i == timeout )
			return 0;
		else
			return c;
	}
}

char eGon2_getc_ext( void )
{
    return sw_uart_getc(SERIAL_PORT);
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
__u32 eGon2_getall( char *str )
{
	__u32  i;
	char   c;

    i = 0;
    do
    {
        c = sw_uart_getc(SERIAL_PORT);
        if(c)
        {
            str[i++] = c;
        }
        else
        {
            return i;
        }
    }
    while(1);
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
void eGon2_putc(char str)
{
    sw_uart_putc( SERIAL_PORT, str );
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
static void serial_puts( const char * str )
{
	while( *str != '\0' )
	{
		if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
			sw_uart_putc( SERIAL_PORT, '\r' );

		sw_uart_putc( SERIAL_PORT, *str++ );
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
__s32 serial_init(__s32 uart_port, void  *uart_ctrl, __s32 baudrate, __u32 sys_clock)
{
    __u32 apb_clk = sys_clock;
    __u32 df;

    df =  (( apb_clk) + (baudrate << 3)) / (baudrate << 4) ;            // (UART_APBCLK )/(16*UART_BAUD)
    if(sw_uart_init(serial_ports[uart_port], uart_port, uart_ctrl, df) != -1)
    {
        serial_index = uart_port;

        return 0;
    }

    return -1;
}

__s32 serial_exit(__s32 uart_port)
{
	return sw_uart_exit(uart_port);
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
void eGon2_printf( const char * str, ...)
{
	char string[16];
	char *p;
	__s32 hex_flag ;
	__u32 time, time_rest, time_sec;
	va_list argp;

	time = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	time_sec = time/1000;
	time_rest = time%1000;
	eFG_printf("[%8d.%3d] ",time_sec, time_rest);
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
                          serial_puts( p );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  serial_puts( p );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  serial_puts( p );
						  ++str;
						  break;
				case 'c': sw_uart_putc( SERIAL_PORT, va_arg( argp,  __s32 ) );
						  ++str;
						  break;
				case 's': serial_puts( va_arg( argp, char * ) );
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
							serial_puts( p );
							int_to_string_hex( low, string, hex_flag );
							serial_puts( p+2 );
							str += 3;
						  	break;
						}
						else
						{
							int_to_string_dec( va_arg( argp,  __s32 ), string );
                          	serial_puts( p );
						  	++str;
						  	break;
						}
					}
				default : sw_uart_putc( SERIAL_PORT, '%' );       // if current character is not Conversion Specifiers 'dxpXucs',
						  sw_uart_putc( SERIAL_PORT, *str );      // output directly '%' and current character, and then
						  ++str;                                         // let 'str' point to next character.
			}
		}
		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				sw_uart_putc( SERIAL_PORT, '\r' );

			sw_uart_putc( SERIAL_PORT, *str++ );
		}
	}

	va_end( argp );

	return;
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
void eFG_printf( const char * str, ...)
{
	char string[16];
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
                          serial_puts( p );
						  ++str;
						  break;
				case 'x': hex_flag = HEX_x;	         // jump to " case 'X' "
				case 'p':
				case 'X': int_to_string_hex( va_arg( argp,  __s32 ), string, hex_flag );
						  serial_puts( p );
                          ++str;
						  break;
				case 'u': Uint_to_string_dec( va_arg( argp,  __s32 ), string );
						  serial_puts( p );
						  ++str;
						  break;
				case 'c': sw_uart_putc( SERIAL_PORT, va_arg( argp,  __s32 ) );
						  ++str;
						  break;
				case 's': serial_puts( va_arg( argp, char * ) );
						  ++str;
						  break;
				case '8':
					{
						__u32 value;

						value = va_arg( argp,  __u32);
						memset(string, '0', 16);
						int_to_string_dec_ex(value, string, 16);
						serial_puts( p + 7 );
						str += 2;
					}
					break;
				case '3':
					{
						__u32 value;

						value = va_arg( argp,  __u32);
						string[3] = '\0';
						string[2] = (value%10) + '0';
						value/= 10;
						string[1] = value%10 + '0';
						value/= 10;
						string[0] = value%10 + '0';
						serial_puts( p);
						str += 2;
					}
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
							serial_puts( p );
							int_to_string_hex( low, string, hex_flag );
							serial_puts( p+2 );
							str += 3;
						  	break;
						}
						else
						{
							int_to_string_dec( va_arg( argp,  __s32 ), string );
                          	serial_puts( p );
						  	++str;
						  	break;
						}
					}
				default : sw_uart_putc( SERIAL_PORT, '%' );       // if current character is not Conversion Specifiers 'dxpXucs',
						  sw_uart_putc( SERIAL_PORT, *str );      // output directly '%' and current character, and then
						  ++str;                                         // let 'str' point to next character.
			}
		}
		else
		{
			if( *str == '\n' )                      // if current character is '\n', insert and output '\r'
				sw_uart_putc( SERIAL_PORT, '\r' );

			sw_uart_putc( SERIAL_PORT, *str++ );
		}
	}

	va_end( argp );

	return;
}


/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : misc.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.04.03
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.04.03       1.1.0        build the file
*
************************************************************************************************************************
*/
#include "types.h"


/*******************************************************************************
*函数名称: g_mod
*函数原型：uint32 g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p )
*函数功能: 从nand flash的某一块中找到一个完好备份将其载入到RAM中。如果成功，返
*          回OK；否则，返回ERROR。
*入口参数: dividend          输入。被除数
*          divisor           输入。除数
*          quot_p            输出。商
*返 回 值: 余数
*******************************************************************************/
__u32 g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p )
{
	if( divisor == 0 )
	{
		*quot_p = 0;
		return 0;
	}
	if( divisor == 1 )
	{
		*quot_p = dividend;
		return 0;
	}

	for( *quot_p = 0; dividend >= divisor; ++(*quot_p) )
		dividend -= divisor;
	return dividend;
}



static __s32 check_valid_hex_char( char ch, __s32 *number_p )
{
	if( '0' <= ch  &&  ch <= '9' )
	{
		*number_p = ch - '0';
		return 1;
	}
	else if( 'a' <= ch  &&  ch <= 'f' )
	{
		*number_p = ch - 'a' + 10;
		return 1;
	}
	else if( 'A' <= ch  &&  ch <= 'F' )
	{
		*number_p = ch - 'A' + 10;
		return 1;
	}
	else
		return 0;
}



__s32 str2int( const char *str, const char **err )
{
	__s32 i;

	if( str[0] == '0' && ( str[1] == 'x' || str[1] == 'X' ) )        // hex
	{
		__s32  number;
		__u32 unsum;

		unsum = 0;
		for( i = 2; check_valid_hex_char( str[i], &number );  i++ )
		{
			unsum = ( unsum << 4 ) + number;
		}

		if( i == 2 )              // fail
		{
			if( err != NULL )
				*err = str;
			return 0;
		}
		if( err != NULL )
			*err = &str[i];
		return unsum;
	}
	else if( str[0] == '0' )                                          // oct
	{
		__u32 unsum;

		unsum = 0;
		for( i = 0;  '0' <= str[i] && str[i] <= '7';  i++ )
			unsum = ( unsum << 3 ) + str[i] - '0';

		if( err != NULL )
			*err = &str[i];

		return unsum;
	}
	else                                                              // dec
	{
		#define POS    1
		#define NEG    0
		__s32 index;
		__s32 pre;
		__s32 sum;

		index = 0;
		pre = POS;
		if( str[0] == '-' )
		{
			index++;
			pre = NEG;
		}

		sum = 0;
		for( i = index;  '0' <= str[i] && str[i] <= '9'; i++ )
		{
			sum = sum * 10 + str[i] - '0';
		}

		if( i == index )              // fail
		{
			if( err != NULL )
				*err = str;
			return 0;
		}
		if( pre == NEG )
			sum = -sum;
		if( err != NULL )
			*err = &str[i];
		return sum;
	}
}


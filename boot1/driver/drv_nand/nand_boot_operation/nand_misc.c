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
__u32 nand_g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p )
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




/* end of misc.c */

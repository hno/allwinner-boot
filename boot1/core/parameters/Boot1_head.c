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
/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : Boot1_head.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.11.06
*
* Description : This file defines the file head part of Boot1, which contains some important
*             infomations such as magic, platform infomation and so on, and MUST be allocted in the
*             head of Boot1.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.11.06      1.1.0        build the file
*
************************************************************************************************************************
*/
#include "include.h"



const boot1_file_head_t  BT1_head = {
	                                  {
	      /* jump_instruction */          ( 0xEA000000 | ( ( ( sizeof( boot1_file_head_t ) + sizeof( int ) - 1 ) / sizeof( int ) - 2 ) & 0x00FFFFFF ) ),
							   		      BOOT1_MAGIC,
							   		      STAMP_VALUE,
							   		      0,
							   		      0,
							   		      0x4000,
							   		      BOOT1_FILE_HEAD_VERSION,
							   		      BOOT1_VERSION,
							   		      EGON_VERSION,
							   		      {
							   		      	'2','.','0','.','0',0,0,0
							   		      },
							 	      },
							 	      {
							 	      	  sizeof( boot1_file_head_t ),
							 	      	  BOOT1_PRVT_HEAD_VERSION,
							 	      	  0,
							 	      	  { 0 },
							 	      	  { 0 },
							 	      	  { 0 },
							 	      	  { 0 },
							 	      	  0,
							 	      	  { 0 },
							 	      	  0,
							 	      	  0,
							 	      	  0,
							 	      	  0,
							 	      	  0,
							 	      	  { 0 },
							 	      	  { 0 }
							 	      }
							 	  };








/*******************************************************************************
*
*                  关于Boot_file_head中的jump_instruction字段
*
*  jump_instruction字段存放的是一条跳转指令：( B  BACK_OF_Boot_file_head )，此跳
*转指令被执行后，程序将跳转到Boot_file_head后面第一条指令。
*
*  ARM指令中的B指令编码如下：
*          +--------+---------+------------------------------+
*          | 31--28 | 27--24  |            23--0             |
*          +--------+---------+------------------------------+
*          |  cond  | 1 0 1 0 |        signed_immed_24       |
*          +--------+---------+------------------------------+
*  《ARM Architecture Reference Manual》对于此指令有如下解释：
*  Syntax :
*  B{<cond>}  <target_address>
*    <cond>    Is the condition under which the instruction is executed. If the
*              <cond> is ommitted, the AL(always,its code is 0b1110 )is used.
*    <target_address>
*              Specified the address to branch to. The branch target address is
*              calculated by:
*              1.  Sign-extending the 24-bit signed(wro's complement)immediate
*                  to 32 bits.
*              2.  Shifting the result left two bits.
*              3.  Adding to the contents of the PC, which contains the address
*                  of the branch instruction plus 8.
*
*  由此可知，此指令编码的最高8位为：0b11101010，低24位根据Boot_file_head的大小动
*态生成，所以指令的组装过程如下：
*  ( sizeof( boot_file_head_t ) + sizeof( int ) - 1 ) / sizeof( int )    求出文件头
*                                              占用的“字”的个数
*  - 2                                         减去PC预取的指令条数
*  & 0x00FFFFFF                                求出signed-immed-24
*  | 0xEA000000                                组装成B指令
*
*******************************************************************************/

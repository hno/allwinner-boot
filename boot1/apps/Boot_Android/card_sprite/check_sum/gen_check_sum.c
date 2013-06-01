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
******************************************************************************************************************
*                                            Aone project ---- eGON module
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : gen_check_sum.c
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.09.23
*
* Description : This file provides a function gen_check_sum( ) to generate a check_sum code for a
*             source file and create a destination file with the check_sum code and aligning 4KB.
*
* Functions list : int gen_check_sum( const char * source_file, const char * dest_file )
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <desricption>
*
* Gary.Wang       2007.08.09      1.1.0        build the file
*
* Gary.Wang       2007.09.23      1.1.0        make many modifications:
*                                            create a destination file.
*                                            make destination file align 4KB.
*
* Gary.Wang       2007.10.13      1.1.0        ignore "stamp value"
*                                              "check sum" generated overlays "stamp value"
*                                              "destination file size" are inputed by outside
*******************************************************************************************************************
*/

#include  "check_sum.h"

__s32 gen_check_sum( void *boot_buf )
{
	boot_file_head_t  *head_p;
	__u32           length;
	__u32           *buf;
	__u32            loop;
	__u32            i;
	__u32            sum;

	head_p = (boot_file_head_t *)boot_buf;
	length = head_p->length;
	if( ( length & 0x3 ) != 0 )                   // must 4-byte-aligned
		return -1;
	buf = (__u32 *)boot_buf;
	head_p->check_sum = STAMP_VALUE;              // fill stamp
	loop = length >> 2;
    /* 计算当前文件内容的“校验和”*/
    for( i = 0, sum = 0;  i < loop;  i++ )
    	sum += buf[i];

    /* write back check sum */
    head_p->check_sum = sum;

    return 0;
}

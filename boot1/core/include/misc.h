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
#ifndef __MISC_H_
#define __MISC_H_	1


#define BOOT1_PRVT_HEAD_VERSION         "1230"    // X.X.XX
#define BOOT1_FILE_HEAD_VERSION         "1230"    // X.X.XX

#define BOOT1_VERSION                   "1230"    // X.X.XX

#define BOOT1_MAX_SIZE                  SZ_2M

#define BOOT1_STACK_SIZE                SZ_1M
#define BOOT1_STACK_BOTTOM              ( BOOT1_BASE + BOOT1_MAX_SIZE + BOOT1_STACK_SIZE )

#define MMU_PAGE_TABLE_BASE             BOOT1_STACK_BOTTOM
#define MMU_PAGE_TABLE_SIZE             ( SZ_16K * 2 )
#define BOOT1_BIG_BUFFER_BASE           ( MMU_PAGE_TABLE_BASE + MMU_PAGE_TABLE_SIZE )

#define NFB_BUF_SIZE                    SZ_1M
#define COMPRESS_PACKAGE_SIZE_MAX       SZ_4M     // add for spi nor. by Gary.2009-12-8 15:19:23


#define GOOD_BOOT0_BASE                 EGON_DRAM_BASE
#define TEMP_BOOT0_BASE                 ( GOOD_BOOT0_BASE + BOOT1_STACK_SIZE )
#define GOOD_BOOT1_BASE                 EGON_DRAM_BASE
#define TEMP_BOOT1_BASE                 ( GOOD_BOOT1_BASE + BOOT1_MAX_SIZE )
#if BOOT1_BASE < ( TEMP_BOOT1_BASE + BOOT1_MAX_SIZE )
	#error Boot1 is overlaped by temp boot1 backup.
#endif

#endif  /*#ifndef __MISC_H_*/



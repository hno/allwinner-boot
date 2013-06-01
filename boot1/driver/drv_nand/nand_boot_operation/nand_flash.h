/*
* (C) Copyright 2007-2012
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Neil Peng<penggang@allwinnertech.com>
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

#ifndef  __nand_flash_h
#define  __nand_flash_h

#include "egon2.h"
#include "string.h"
#include "boot0_v2.h"
#include "boot1_v2.h"
#include "nand_for_boot1.h"

extern __u32 NF_BLOCK_SIZE;
extern __u32 NF_BLK_SZ_WIDTH;
extern __u32 NF_PAGE_SIZE;
extern __u32 NF_PG_SZ_WIDTH;
extern __u32 BOOT1_LAST_BLK_NUM;

#define ADV_NF_OK                       0x0
#define ADV_NF_FIND_OK                  0x0
#define ADV_NF_NO_NEW_BAD_BLOCK         0x0
#define ADV_NF_ERROR                    0x1
#define ADV_NF_NO_FIND_ERR              0x2
#define ADV_NF_OVERTIME_ERR             0x3
#define ADV_NF_LACK_BLKS                0x4
#define ADV_NF_NEW_BAD_BLOCK            0x5


#define NF_SECTOR_SIZE                  512U
#define NF_SCT_SZ_WIDTH                 9U
#define OOB_BUF_SIZE_PER_SECTOR         4


enum
{
	NF_OK         = 0,
	NF_GOOD_BLOCK = 0,
	NF_OVERTIME_ERR  ,
	NF_ECC_ERR       ,
	NF_BAD_BLOCK     ,
	NF_ERASE_ERR     ,
	NF_PROG_ERR      ,
	NF_NEW_BAD_BLOCK ,
	NF_LACK_BLKS     ,
	NF_ERROR     = -1,
	NF_ERR_COUNT              // always the last one
};


#define MAX_PAGE_SIZE         SZ_16K
#define BAD_BLK_FLAG          0


extern __u32 page_with_bad_block;

extern __u32 nand_g_mod( __u32 dividend, __u32 divisor, __u32 *quot_p );

extern __s32  NF_open ( void );
extern __s32  NF_close( void );
extern __s32  NF_read ( __u32 sector_num, void *buffer, __u32 N );
extern __s32  NF_write( __u32 sector_num, void *buffer, __u32 N );
extern __s32  NF_write_Seq( __u32 sector_num, void *buffer, __u32 N );
extern __s32  NF_erase( __u32 blk_num );
extern __s32  NF_read_status ( __u32 blk_num );
extern __s32  NF_mark_bad_block( __u32 blk_num );
extern __s32  NF_verify_block( __u32 blk_num );




#endif     //  ifndef __nand_flash_h


/* end of nand_flash.h  */

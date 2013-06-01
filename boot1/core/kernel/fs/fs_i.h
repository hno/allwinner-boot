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
#ifndef __FS_I_H__
#define __FS_I_H__

#define FSYS_FILE_MAXOPEN              16    /* Maximum number of file handles */
#define FSYS_DIR_MAXOPEN                8    /* Maximum number of directory handles */
#define MAX_PARTS_NUM                  16

#define FSYS_ERR_OK                     (__s16)0x0000
#define FSYS_ERR_EOF                    (__s16)0xfff0
#define FSYS_ERR_DISKFULL               (__s16)0xffe0
#define FSYS_ERR_INVALIDPAR             (__s16)0xffd0
#define FSYS_ERR_WRITEONLY              (__s16)0xffc0
#define FSYS_ERR_READONLY               (__s16)0xffb0
#define FSYS_ERR_READERROR              (__s16)0xffa0
#define FSYS_ERR_WRITEERROR             (__s16)0xff90
#define FSYS_ERR_DISKCHANGED            (__s16)0xff80
#define FSYS_ERR_CLOSE                  (__s16)0xff70

#define FSYS_CMD_FLUSH_CACHE            1000L
#define FSYS_CMD_CHK_DSKCHANGE          1010L
#define FSYS_CMD_READ_SECTOR            1100L
#define FSYS_CMD_WRITE_SECTOR           1110L
#define FSYS_CMD_FORMAT_MEDIA           2222L
#define FSYS_CMD_FORMAT_AUTO            2333L
#define FSYS_CMD_INC_BUSYCNT            3001L
#define FSYS_CMD_DEC_BUSYCNT            3002L
#define FSYS_CMD_GET_DISKFREE           4000L
#define FSYS_CMD_GET_DEVINFO            4011L
#define FSYS_CMD_FLASH_ERASE_CHIP       9001L

#define EPDK_OK			0                   /* 代表成功                                                */
#define EPDK_FAIL		(-1)                /* 代表失败                                                */
#define EPDK_TRUE  		1                   /* 真                                                      */
#define EPDK_FALSE		0                   /* 假                                                      */

#define  EPDK_DISABLED                                      0
#define  EPDK_ENABLED                                       1
#define  EPDK_NO                                            0
#define  EPDK_YES                                           1
#define  EPDK_OFF                                           0
#define  EPDK_ON                                            1
#define  EPDK_CLR                                           0
#define  EPDK_SET                                           1
#define  NULL            0                   /* 指针空                                                  */

#include "include.h"
#include "fs.h"
#include "inode.h"
#include "part.h"
#include "eGon2_sys.h"

#define   fsprintf                  eGon2_printf

#endif


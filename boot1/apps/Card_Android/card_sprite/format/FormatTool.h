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

#ifndef		__FORMAT_TOOL_H__
#define 	__FORMAT_TOOL_H__

/* Part size limit */
#define		FSYS_MIN_PART_SIZE		(64 * 1024 * 1024)	//sd卡最小容量64M bytes

typedef struct FormatParameter {
    int  ClusterSize;           //cluster size (default 0)
    int  FSSize;                //file system size (default 0)
    int  Quick;                 //quick format (default 0)
    int  Subtype;               //sub fs type like fat12/fat16 (default 0)
    int  RsvdSecCnt;            //reserved space, 0x20~0x8000  (保留扇区设置)
    char Label[12];             //volume label, max len is 11 bytes
    int  UserDataSec;           //user data start block (用户数据区开始扇区数)
} FormatParameter;

extern int FATFS_Format(FormatParameter *para);
extern int FormatDisk(void);

#endif		/* __FORMAT_TOOL_H__ */

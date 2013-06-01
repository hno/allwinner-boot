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
#ifndef	_WBOOT_FS_H_
#define	_WBOOT_FS_H_

#define     H_FILE                      void*
#define     HDIR                        void*

//for main fs type define
#define     FSTYPE_FAT                  1
#define     FSTYPE_NTFS                 2
//for sub fs type define
#define     FS_FAT12                    0
#define     FS_FAT16                    1
#define     FS_FAT32                    2
#define     FS_DONTCARE                 0xff

#define 	SEEK_SET                    0		//from begin of the file
#define 	SEEK_CUR                    1		//current position
#define 	SEEK_END                    2		//from end of the file

#define 	FSYS_ENT_ATTR_READ_ONLY     0x0001
#define 	FSYS_ENT_ATTR_HIDDEN        0x0002
#define 	FSYS_ENT_ATTR_SYSTEM        0x0004
#define 	FSYS_ENT_VOLUME_ID          0x0008
#define 	FSYS_ENT_ATTR_DIRECTORY     0x0010
#define 	FSYS_ENT_ATTR_ARCHIVE       0x0020

#define 	FSYS_dirnamemax             260

typedef struct __entry
{
  unsigned char      d_name[FSYS_dirnamemax];
  int                f_size;
  unsigned int       f_attr;
}ENTRY;

#endif	//_WBOOT_FS_H_

/* end of _WBOOT_FS_H_ */


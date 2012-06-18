/*
****************************************************************************************************
*											PC FS Format Tool
*										Just for FAT file System
*
*						        (c) Copyright 2009-2010, Sunny China
*											All	Rights Reserved
*
* File   	: FormatTool.h
* Version	: V1.0
* By     	: Sunny
* Date		: 2009-11-6 14:58:41
****************************************************************************************************
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

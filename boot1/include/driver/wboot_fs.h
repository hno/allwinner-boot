/*
*********************************************************************************************************
*											        eBase
*
*
*
*						        (c) Copyright 2006-2010, AW China
*											All	Rights Reserved
*
* File    	: 	egon_drv_common_inc.h
* Date	:	2010-09-25
* By      	: 	holigun
* Version 	: 	V1.00
*********************************************************************************************************
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


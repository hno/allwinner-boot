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

#ifndef _FSYS_FAT_PRIVATE_H_
#define _FSYS_FAT_PRIVATE_H_

#include "fs_i.h"
#include <string.h>

/*
*********************************************************************************************************
*             #define constants
*********************************************************************************************************
*/

#define FSYS_FAT_ATTR_READ_ONLY   0x01
#define FSYS_FAT_ATTR_HIDDEN      0x02
#define FSYS_FAT_ATTR_SYSTEM      0x04
#define FSYS_FAT_VOLUME_ID        0x08
#define FSYS_FAT_ATTR_ARCHIVE     0x20
#define FSYS_FAT_ATTR_DIRECTORY   0x10

#define FSYS_FAT_DENTRY_SIZE      0x20

#define FSYS_FAT_SEC_SIZE       	    0x200   /* do not change for FAT */

#define FSYS_FAT_MAXUNIT                2         /* 每个设备支持多少个分区                   */
#define FSYS_FAT_NOFAT32                0         /* 1 disables FAT32 support                 */
#define FSYS_FAT_NOFORMAT               0         /* 1 disables code for formatting a media   */
#define FSYS_FAT_DISKINFO               1         /* 0 disables command FS_CMD_GET_DISKFREE   */
#define FSYS_FAT_FWRITE_UPDATE_DIR      1

#define FSYS_FAT_SEEK_SET                   0		//from begin of the file
#define FSYS_FAT_SEEK_CUR                   1		//current position
#define FSYS_FAT_SEEK_END                   2		//from end of the file

/*
*********************************************************************************************************
*             Global data types
*********************************************************************************************************
*/

/* BIOS parameter block (FAT12/FAT16) */
typedef struct __FSYS_FAT_BPB
{
    __u32               TotSec32;         /* RSVD + FAT + ROOT + FATA (>=64k) */
    __u32               FATSz32;          /* number of FAT sectors          */
    __u32               RootClus;         /* root dir clus for FAT32        */
    __u16               BytesPerSec;      /* _512_,1024,2048,4096           */
    __u16               RootEntCnt;       /* number of root dir entries     */
    __u16               TotSec16;         /* RSVD + FAT + ROOT + FATA (<64k) */
    __u16               FATSz16;          /* number of FAT sectors          */
    __u16               ExtFlags;         /* mirroring info                 */
    __u16               FSInfo;           /* position of FSInfo structure   */
    __u16               Signature;        /* 0xAA55 Signature               */
    __u16               RsvdSecCnt;       /* 1 for FAT12 & FAT16            */
    unsigned char       SecPerClus;       /* sec in allocation unit         */
    unsigned char       NumFATs;          /* 2                              */
    unsigned int		fattype;
} __fsys_fat_BPB_t;


/* FAT directory entry */
typedef struct __FSYS_FAT_DENTRY
{
    unsigned char   data[32];
} __fsys_fat_dentry_t;

typedef struct __FSYS_FAT_FILE_P
{
    __u32                       fileid_lo;          /* unique id for file (lo)                                      */
    __u32                       fileid_hi;          /* unique id for file (hi)                                      */
    __u32                       fileid_ex;          /* unique id for file (ex)                                      */
    __s32                       EOFClust;           /* used by FAT FSL only                                         */
    __u32                       CurClust;           /* used by FAT FSL only                                         */

}__fsys_fat_file_p_t;

//typedef struct __FSYS_FAT_PART_P
//{
//    __fsys_fat_BPB_t	BPB;
//
//}__fsys_fat_part_p_t;

/*
*********************************************************************************************************
*             Externals
*********************************************************************************************************
*/

//extern __fsys_fat_BPB_t fsys_fat_aBPBUnit;

#define fsys_fat_aBPBUnit 	 ((__fsys_fat_BPB_t*)pPart->supper_data)
#define f_fsys_fat_aBPBUnit  ((__fsys_fat_BPB_t*) pFile->pPart->supper_data)
#define d_fsys_fat_aBPBUnit  ((__fsys_fat_BPB_t*) pDir->pPart->supper_data)

/*
*********************************************************************************************************
*             Global function prototypes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*             fat_misc
*/

void                fsys_fat_block_init(void);
char               *fsys_fat_malloc(unsigned int Size);
void                fsys_fat_free(void *pBuffer);
void                fsys_fat_bpb_block_init(void);
char               *fsys_fat_bpb_malloc(void);
void                fsys_fat_bpb_free(void *pBuffer);
__s32               fsys_fat_diskclust(__fsys_part_t *pPart, __s32 StrtClst, __s32 ClstNum);
__s32               fsys_fat_FAT_alloc(__fsys_part_t *pPart, __s32 LastClust);
__s32               fsys_fat_FAT_find_eof(__fsys_part_t *pPart, __s32 StrtClst, __u32 *pClstCnt);
int                 fsys_fat_which_type(__fsys_part_t *pPart);
int                 fsys_fat_checkunit(__fsys_part_t *pPart);

/*
*********************************************************************************************************
*
*             fat_in
*/

__u32	            fsys_fat_fread(void *pData, __u32 Size, __u32 N, __fsys_file_t *pFile);


/*
*********************************************************************************************************
*
*             fat_out
*/

__u32	            fsys_fat_fwrite(const void *pData, __u32 Size, __u32 N, __fsys_file_t *pFile);
__s32               fsys_fat_fclose(__fsys_file_t *pFile);


/*
*********************************************************************************************************
*
*             fat_open
*/

__fsys_file_t      *fsys_fat_fopen(const char *pFileName, const char *pMode, __fsys_file_t *pFile);
__u32               fsys_fat_dir_size(__fsys_part_t *pPart, __u32 DirStart);
__u32               fsys_fat_dir_realsec(__fsys_part_t *pPart, __u32 DirStart, __u32 DirSec);
void                fsys_fat_make_realname(char *pEntryName, const char *pOrgName);
__u32               fsys_fat_find_dir(__fsys_part_t *pPart, char *pDirName, __u32 DirStart, __u32 DirSize);
__u32               fsys_fat_findpath(__fsys_part_t *pPart, const char *pFullName, char**pFileName, __u32 *pDirStart);
__s32               fsys_fat_DeleteFileOrDir(__fsys_part_t *pPart,  const char *pName, __u32 DirStart, __u32 DirSize, char RmFile);


/*
*********************************************************************************************************
*
*             fat_ioctl
*/
__s32 				fsys_fat_fseek (__fsys_file_t * pFile, __s32 Offset, __s32 Whence);
int                 fsys_fat_fioctl(__fsys_file_t * pPart, __s32 Cmd, 	 __s32 Aux, void *pBuffer);


/*
*********************************************************************************************************
*
*             fat_dir
*/

__fsys_dir_t       *fsys_fat_opendir(const char *pDirName, __fsys_dir_t *pDir);
int                 fsys_fat_closedir(__fsys_dir_t *pDir);
__fsys_dirent_t    *fsys_fat_readdir(__fsys_dir_t *pDir);
int                 fsys_fat_MkRmDir(const char *pDirName, __fsys_part_t *pPart, char MkDir);


/*
*********************************************************************************************************
*
*             fat_mount
*/
int 			    fsys_fat_ReadBPB(__fsys_part_t * pPart);
__s32 				fsys_fat_mount(__fsys_part_t * pPart);
__s32 				fsys_fat_unmount(__fsys_part_t *pPart);
__bool            	fsys_fatfs_identify (__fsys_part_t * pPart);
__s32            	fsys_fatfs_format   (__fsys_part_t * pPart, int force_type);
__s32            	fsys_fatfs_checkdisk(__fsys_part_t * pPart);


/* known media */
#define FSYS_MEDIA_SMC_1MB          1010L
#define FSYS_MEDIA_SMC_2MB          1020L
#define FSYS_MEDIA_SMC_4MB          1030L
#define FSYS_MEDIA_SMC_8MB          1040L
#define FSYS_MEDIA_SMC_16MB         1050L
#define FSYS_MEDIA_SMC_32MB         1060L
#define FSYS_MEDIA_SMC_64MB         1070L
#define FSYS_MEDIA_SMC_128MB        1080L

#define FSYS_MEDIA_RAM_16KB         2010L
#define FSYS_MEDIA_RAM_64KB         2020L
#define FSYS_MEDIA_RAM_128KB        2030L
#define FSYS_MEDIA_RAM_256KB        2040L
#define FSYS_MEDIA_RAM_512KB        2050L
#define FSYS_MEDIA_RAM_1MB			2060L
#define FSYS_MEDIA_RAM_2MB			2070L
#define FSYS_MEDIA_RAM_4MB			2080L
#define FSYS_MEDIA_RAM_8MB			2090L
#define FSYS_MEDIA_RAM_16MB			2100L
#define FSYS_MEDIA_RAM_32MB			2110L

#define FSYS_MEDIA_FD_144MB         3040L

#define FSYS_MEDIA_MMC_32MB         4060L
#define FSYS_MEDIA_MMC_64MB         4070L
#define FSYS_MEDIA_MMC_128MB        4080L

#define FSYS_MEDIA_SD_16MB          5050L
#define FSYS_MEDIA_SD_64MB          5070L
#define FSYS_MEDIA_SD_128MB         5080L

#define FSYS_MEDIA_CF_32MB          6060L
#define FSYS_MEDIA_CF_64MB          6070L
#endif  /* _FSYS_FAT_PRIVATE_H_ */



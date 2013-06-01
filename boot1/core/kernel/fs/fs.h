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
#ifndef __FS_H__
#define __FS_H__

#include "inode.h"
#include "part.h"

//typedef struct __FSYS_FILE              __fsys_file_t;          /* 文件                                             */
//typedef struct __FSYS_DIR               __fsys_dir_t;           /* 目录                                             */
//typedef struct __FSYS_DIRENT            __fsys_dirent_t;        /* 目录项                                           */

//typedef struct __FSYS_PART       	    __fsys_part_t;          /* 分区                                             */

typedef struct __FSYS_FSOPS             __fsys_fsops_t;         /* 文件系统操作                                     */
//typedef struct __FSYS_FS                __fsys_fs_t;

struct __FSYS_FSOPS
{
    __fsys_file_t*   (*fopen    ) (const char * /*FileName*/, const char * /*pMode*/, __fsys_file_t * /*pFile*/ );
    int              (*fclose   ) (__fsys_file_t *pFile                                                         );
    unsigned int     (*fread    ) (void *pData,          unsigned int Size, unsigned int N, __fsys_file_t *pFile);
    unsigned int     (*fwrite   ) (const void *pData,    unsigned int Size, unsigned int N, __fsys_file_t *pFile);
    long             (*ftell    ) (__fsys_file_t *pFile                                                         );
    int              (*fseek    ) (__fsys_file_t *pFile, int Offset,                 int Whence                 );
    int              (*fioctl   ) (__fsys_file_t *pFile, int Cmd,                    int Aux,      void *pBuffer);
    __fsys_dir_t*    (*opendir  ) (const char *pDirName, __fsys_dir_t *pDir                                     );
    int              (*closedir ) (__fsys_dir_t *pDir                                                           );
    __fsys_dirent_t *(*readdir  ) (__fsys_dir_t *pDir                                                           );
    void             (*rewinddir) (__fsys_dir_t *pDir                                                           );
    int              (*mkdir    ) (const char *pDirName, __fsys_part_t* pPart,      char Aux                    );
    int              (*rmdir    ) (const char *pDirName, __fsys_part_t* pPart,      char Aux                    );

    int              (*mount    ) (__fsys_part_t * pPart                                                        );
    int              (*unmount  ) (__fsys_part_t * pPart                                                        );
    __bool           (*identify ) (__fsys_part_t * pPart                                                        );
    int              (*format   ) (__fsys_part_t * pPart, int force_type                                        );
    int              (*checkdisk) (__fsys_part_t * pPart                                                        );
    int              (*tellfstype)(__fsys_part_t * pPart                                                        );
};

struct __FSYS_FS
{
	const char		          * name;
	int                         type;

	__fsys_fsops_t            * ops;                    /* 文件系统函数集                                       */
    int                       (*fs_init)(void);
    int                       (*fs_exit)(void);
};

#endif

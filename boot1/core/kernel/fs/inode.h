/*
**********************************************************************************************************************
*											        eGon
*						           the Easy Portable/Player Operation System
*									       eGON fsys sub-system
*
*						         (c) Copyright 2010-2012, Jerry China
*											All	Rights Reserved
*
* File    :
* By      : Jerry
* Version : V2.00
* Date	  :
**********************************************************************************************************************
*/
#ifndef _SYS_FSYS_INTER_H_
#define _SYS_FSYS_INTER_H_

#include "part.h"

typedef struct __FSYS_FILE      __fsys_file_t;          /* 文件                                             */
typedef struct __FSYS_DIRENT    __fsys_dirent_t;        /* 目录项                                           */
typedef struct __FSYS_DIR       __fsys_dir_t;           /* 目录                                             */

struct __FSYS_FILE
{
    __fsys_part_t             * pPart;              /* 分区                                                         */
    __fsys_fs_t               * pFS;

    unsigned int                fileid_lo;          /* unique id for file (lo)                                      */
    unsigned int                fileid_hi;          /* unique id for file (hi)                                      */
    unsigned int                fileid_ex;          /* unique id for file (ex)                                      */
    int                         EOFClust;           /* used by FAT FSL only                                         */
    unsigned int                CurClust;           /* used by FAT FSL only                                         */

    int                         filepos;            /* current position in file                                     */
    int                         size;               /* size of file                                                 */
    short                       error;              /* error code                                                   */
    unsigned char               inuse;              /* handle in use mark                                           */
    unsigned char               mode_r;             /* mode READ                                                    */
    unsigned char               mode_w;             /* mode WRITE                                                   */
    unsigned char               mode_a;             /* mode APPEND                                                  */
    unsigned char               mode_c;             /* mode CREATE                                                  */
    unsigned char               mode_b;             /* mode BINARY                                                  */
};

#define  FSYS_dirnamemax 260
struct __FSYS_DIRENT
{
  unsigned int     d_ino;                            /* to be POSIX conform                                          */
  unsigned char    d_name[FSYS_dirnamemax];
  unsigned char    fatdirattr;                       /* FAT only. Contains the "DIR_Attr" field of an entry.         */
};

struct __FSYS_DIR
{
    __fsys_part_t             * pPart;              /* 分区                                                         */
    __fsys_fs_t               * pFS;

    __fsys_dirent_t             dirent;             /* current directory entry                                      */

    unsigned int                dirid_lo;           /* unique id for file (lo)                                      */
    unsigned int                dirid_hi;           /* unique id for file (hi)                                      */
    unsigned int                dirid_ex;           /* unique id for file (ex)                                      */

    int                         dirpos;             /* current position in file                                     */
    int                         size;               /* size of file                                                 */
    short                       error;              /* error code                                                   */
    unsigned char               inuse;              /* handle in use mark                                           */
    unsigned char               mode;               /* mode READ                                                    */
};

#endif  //#define _FSYS_VFS_H_



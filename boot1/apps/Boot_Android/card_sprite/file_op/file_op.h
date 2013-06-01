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
#ifndef __FILE_OP_H__
#define __FILE_OP_H__

#include  "../card_sprite_i.h"

/* typedef */
typedef int 			SRes;
typedef int 			WRes;
typedef int 			Int32;
typedef unsigned int 	UInt32;
typedef unsigned int 	Sector_t;

typedef long long       Int64;
typedef unsigned long long UInt64;
typedef enum __ESzSeek
{
  SZ_SEEK_SET = 0,
  SZ_SEEK_CUR = 1,
  SZ_SEEK_END = 2
}
ESzSeek;

/*---------buffer state---------*/
#define   DEC_BUFFER_CLEAR      0
#define   DEC_BUFFER_DIRTY      1

#define   DEC_SECTOR_SIZE       512
#define   DEC_SECTOR_BITS       9

typedef  struct
{
    UInt32   State;
    UInt32   Pos;
    void    *Buffer;
} DecBuffer_t;

/* ---------- File ---------- */
typedef struct  __CSzFile
{
  int   		writeMode;		/* write open mode falg 			*/
  Sector_t 		SectorNr;		/* begin sector number 				*/
  Sector_t 		CurSec;			/* current sector number 			*/
  __s64   	CurPos;			/* file current postion 			*/
  __s64   	Size;			/* total size (just for read open) 	*/
  DecBuffer_t	Buffer_t;		/* data buffer 						*/
} CSzFile;

extern  CSzFile *File_Open(Sector_t sector, UInt32 size_low, UInt32 size_high, __u32 writeMode);
extern  WRes File_Close (CSzFile *p);

extern  WRes File_Read  (void *data, __u32 size, __u32 size_cnt, CSzFile *p);
extern  WRes File_Write (const void *data, __u32 size, __u32 size_cnt, CSzFile *p);
extern  WRes File_Seek  (CSzFile *p, __s64 pos, ESzSeek origin);

#endif	/* __FILE_OP_H__ */

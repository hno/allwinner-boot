/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									         Decompression For Boot
*
*						        (c) Copyright 2009-2010, Sunny China
*											All	Rights Reserved
*
* File    : DFB_File.h
* By      : sunny
* Version : V2.0
* Date	  : 2009-11-4 10:35:33
*********************************************************************************************************
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

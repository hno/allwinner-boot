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

#include "fat_priv.h"
/*
**********************************************************************************************************************
*             Global functions
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
*             fsys_fat_fread
*
  Description:
  FS internal function. Read data from a file.

  Parameters:
  pData       - Pointer to a data buffer for storing data transferred
                from file.
  Size        - Size of an element to be transferred from file to data
                buffer
  N           - Number of elements to be transferred from the file.
  pFile       - Pointer to a FS_FILE data structure.

  Return value:
  Number of elements read.
**********************************************************************************************************************
*/

#define     FAT_ENTRY_CACHE_NUM     4

typedef struct __FAT_ENTRY_SCT
{
    int     sector;
    char    buffer[512];
} __fat_entry_sct_t;

typedef struct __FAT_ENTRY_CACHE
{
    int                 cachemaxnum;
    int                 cacheindex;
    __fat_entry_sct_t   entrysectors[FAT_ENTRY_CACHE_NUM];
} __fat_entry_cache_t;

__fat_entry_cache_t fat_entry_cache;

/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int fat_sector_cache_init(void)
{
    int index = 0;
    while (index < FAT_ENTRY_CACHE_NUM)
    {
        fat_entry_cache.entrysectors[index].sector = -1;/* 无效扇区 */
        index++;
    }
    fat_entry_cache.cacheindex  = 0;
    fat_entry_cache.cachemaxnum = FAT_ENTRY_CACHE_NUM;
    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int fat_sector_cache_exit(void)
{
    int index = 0;
    while (index < FAT_ENTRY_CACHE_NUM)
    {
        fat_entry_cache.entrysectors[index].sector = -1;/* 无效扇区 */
        index++;
    }
    fat_entry_cache.cacheindex  = 0;
    fat_entry_cache.cachemaxnum = 0;
    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void *fat_sector_read(__fsys_part_t *pPart, __u32 fatsec)
{
    int     index;
    void   *tmpbuffer;
    int     err;

    /* 在cache中直接查找 */
    index = 0;
    while (index < fat_entry_cache.cachemaxnum)
    {
        if (fat_entry_cache.entrysectors[index].sector == fatsec)
        {
            /* 命中 */
            return (void *)(fat_entry_cache.entrysectors[index].buffer);
        }
        index++;
    }
    /* 替换一个FAT表cache项 */
    index     = fat_entry_cache.cacheindex;
    tmpbuffer = (void *)(fat_entry_cache.entrysectors[index].buffer);
    fat_entry_cache.cacheindex++;
    if (fat_entry_cache.cacheindex >= fat_entry_cache.cachemaxnum)
    {
        fat_entry_cache.cacheindex = 0;
    }

    /* 从块设备中读取FAT表 */
    err = pPart->pdops.read((void *)tmpbuffer, fatsec, 1, pPart);
    if (err < 0)
    {
        return NULL;
    }
    fat_entry_cache.entrysectors[index].sector = fatsec;

    /* 为了更好的读性能，直接将cache项的buffer返回，返回的buffer不可修改.
     * sunny at 2010-8-27 16:11:14.
     */
    return tmpbuffer;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 fsys_fat_clustmap(__fsys_file_t *pFile, int fclst, int *maxnum)
{
    __u32   fatsize;
    __s32   fatindex;
    __s32   fatsec;
    __s32   fatoffs;
    __s32   lastsec;
    __s32   curclst;
    __s32   prevclst;
    __s32   startclst;
    __s32   seachcnt;
    __s32   todo;
    __s32   bytespersec;
    int     fattype;
    char   *buffer = NULL;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif /* FS_FAT_NOFAT32==0 */
    __fsys_part_t *pPart = pFile->pPart;

    fattype = fsys_fat_which_type(pPart);
#if (FSYS_FAT_NOFAT32!=0)
    if (fattype == 2)
    {
        return 0;
    }
#endif  /* FS_FAT_NOFAT32!=0 */

    fatsize = fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        fatsize = fsys_fat_aBPBUnit->FATSz32;
    }
    bytespersec = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
    todo        = *maxnum;
    lastsec     = -1;

    /* walk to target cluster location,
     * always from file start cluster.
     */
    curclst   = pFile->fileid_hi;
    startclst = 0;
    seachcnt  = 0;
    while (seachcnt < fclst)
    {
        if (fattype == 1)
        {
            fatindex = curclst + (curclst / 2);  /* FAT12 */
        }
#if (FSYS_FAT_NOFAT32==0)
        else if (fattype == 2)
        {
            fatindex = curclst * 4;               /* FAT32 */
        }
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            fatindex = curclst * 2;               /* FAT16 */
        }
        fatsec  = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / bytespersec);
        fatoffs = fatindex % bytespersec;
        if (fatsec != lastsec)
        {
            buffer = fat_sector_read(pFile->pPart, fatsec);
            if (buffer == NULL)
            {
                /* fat1 read failed, try fat2 */
                buffer = fat_sector_read(pFile->pPart, fatsize + fatsec);
                if (buffer == NULL)
                {
                    return 0;
                }
            }
            lastsec = fatsec;
        }
        if (fattype == 1)
        {
            if (fatoffs == (bytespersec - 1))
            {
                a = buffer[fatoffs];
                buffer = fat_sector_read(pFile->pPart, fatsec);
                if (buffer == NULL)
                {
                    /* fat1 read failed, try fat2 */
                    buffer = fat_sector_read(pFile->pPart, fatsize + fatsec);
                    if (buffer == NULL)
                    {
                        return 0;
                    }
                }
                lastsec = fatsec + 1;
                b = buffer[0];
            }
            else
            {
                a = buffer[fatoffs];
                b = buffer[fatoffs + 1];
            }
            if (curclst & 1)
            {
                curclst = ((a & 0xf0) >> 4) + 16 * b;
            }
            else
            {
                curclst = a + 256 * (b & 0x0f);
            }
            curclst &= 0x0fffL;
            if (curclst >= 0x0ff8L)
            {
                return 0;
            }
        }
#if (FSYS_FAT_NOFAT32==0)
        else if (fattype == 2)
        {
            a = buffer[fatoffs];
            b = buffer[fatoffs + 1];
            c = buffer[fatoffs + 2];
            d = buffer[fatoffs + 3];
            curclst = a + 0x100L * b + 0x10000L * c + 0x1000000L * d;
            curclst &= 0x0fffffffL;
            if (curclst >= (__s32)0x0ffffff8L)
            {
                return 0;
            }
        }
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            a = buffer[fatoffs];
            b = buffer[fatoffs + 1];
            curclst  = a + 256 * b;
            curclst &= 0xffffL;
            if (curclst >= (__s32)0xfff8L)
            {
                return 0;
            }
        }
        seachcnt++;
    }
    /* 扫描从startclst开始的连续簇个数 */
    startclst = curclst;
    prevclst  = curclst;
    while (todo)
    {
        if (fattype == 1)
        {
            fatindex = curclst + (curclst / 2);  /* FAT12 */
        }
#if (FSYS_FAT_NOFAT32==0)
        else if (fattype == 2)
        {
            fatindex = curclst * 4;               /* FAT32 */
        }
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            fatindex = curclst * 2;               /* FAT16 */
        }
        fatsec  = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / bytespersec);
        fatoffs = fatindex % bytespersec;
        if (fatsec != lastsec)
        {
            buffer = fat_sector_read(pFile->pPart, fatsec);
            if (buffer == NULL)
            {
                /* fat1 read failed, try fat2 */
                buffer = fat_sector_read(pFile->pPart, fatsize + fatsec);
                if (buffer == NULL)
                {
                    return 0;
                }
            }
            lastsec = fatsec;
        }
        if (fattype == 1)
        {
            if (fatoffs == (bytespersec - 1))
            {
                a = buffer[fatoffs];
                buffer = fat_sector_read(pFile->pPart, fatsec);
                if (buffer == NULL)
                {
                    /* fat1 read failed, try fat2 */
                    buffer = fat_sector_read(pFile->pPart, fatsize + fatsec);
                    if (buffer == NULL)
                    {
                        return 0;
                    }
                }
                lastsec = fatsec + 1;
                b = buffer[0];
            }
            else
            {
                a = buffer[fatoffs];
                b = buffer[fatoffs + 1];
            }
            if (curclst & 1)
            {
                curclst = ((a & 0xf0) >> 4) + 16 * b;
            }
            else
            {
                curclst = a + 256 * (b & 0x0f);
            }
            curclst &= 0x0fffL;
            if (curclst == (__s32)0x0fffL)
            {
                /* 最后一簇 */
                todo--;
                break;
            }
            if (curclst != (prevclst + 1))
            {
                /* 非连续簇 */
                todo--;
                break;
            }
        }
#if (FSYS_FAT_NOFAT32==0)
        else if (fattype == 2)
        {
            a = buffer[fatoffs];
            b = buffer[fatoffs + 1];
            c = buffer[fatoffs + 2];
            d = buffer[fatoffs + 3];
            curclst = a + 0x100L * b + 0x10000L * c + 0x1000000L * d;
            curclst &= 0x0fffffffL;
            if (curclst == (__s32)0x0fffffffL)
            {
                /* 最后一簇 */
                todo--;
                break;
            }
            if (curclst != (prevclst + 1))
            {
                /* 非连续簇 */
                todo--;
                break;
            }
        }
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            a = buffer[fatoffs];
            b = buffer[fatoffs + 1];
            curclst  = a + 256 * b;
            curclst &= 0xffffL;
            if (curclst == (__s32)0xffffL)
            {
                /* 最后一簇 */
                todo--;
                break;
            }
            if (curclst != (prevclst + 1))
            {
                /* 非连续簇 */
                todo--;
                break;
            }
        }
        prevclst = curclst;
        todo--;
    }

    *maxnum = *maxnum - todo;
    return startclst;   /* 返回首簇号地址 */
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 fsys_fat_bmap(__fsys_file_t *pFile, __s32 fpos, __s32 *maxsct)
{
    __u32   dstart;
	__u32   dsize;
	__u32   datastart;
	__u32   sectornum;
	__u32   fatsize;
	__s32   maxclust;
	__s32   startclst;
	__u32   clstsize;
	__s32   maxtmp;
	__s32   sctoffset;

	/* 扫描FAT表，查找连续簇的个数能否满足最大个数请求 */
	maxclust  = (*maxsct + f_fsys_fat_aBPBUnit->SecPerClus - 1) / f_fsys_fat_aBPBUnit->SecPerClus;
	clstsize  = (f_fsys_fat_aBPBUnit->SecPerClus * FSYS_FAT_SEC_SIZE);
	startclst = fsys_fat_clustmap(pFile, fpos / clstsize, &maxclust);
	if (startclst == 0)
	{
	    //printf("scan cluster failed\n");
	    return 0;
	}
	fatsize = f_fsys_fat_aBPBUnit->FATSz16;
	if (fatsize == 0)
	{
    	/* FAT32 */
	    fatsize = f_fsys_fat_aBPBUnit->FATSz32;
	}
    dstart    = f_fsys_fat_aBPBUnit->RsvdSecCnt + f_fsys_fat_aBPBUnit->NumFATs * fatsize;
    dsize     = ((__u32)((__u32)f_fsys_fat_aBPBUnit->RootEntCnt) * FSYS_FAT_DENTRY_SIZE) / FSYS_FAT_SEC_SIZE;
  	datastart = dstart + dsize;

  	sctoffset = (fpos / FSYS_FAT_SEC_SIZE) % f_fsys_fat_aBPBUnit->SecPerClus;
    sectornum = datastart + (f_fsys_fat_aBPBUnit->SecPerClus * (startclst - 2)) + sctoffset;
    maxtmp    = (maxclust * f_fsys_fat_aBPBUnit->SecPerClus);
    *maxsct = *maxsct <  maxtmp ? *maxsct : maxtmp;
    return sectornum;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__u32 fsys_fat_fread(void *pData, __u32 Size, __u32 N, __fsys_file_t *pFile)
{
	int     err;
	__u8	*buffer;
	__u32	chunck_size;
	__u32	sector_nr;
	__u32   offset;
	__u32   len;
	__u32   pos;
	__u8   *tmpbuf = NULL;
	__s32   maxsct;

	if (!pFile || !pFile->pPart)
	{
		return 0;
    }

    if (pFile->filepos >= pFile->size)
    {
    	/* 已到文件结尾 */
    	pFile->error = FSYS_ERR_EOF;
    	return 0;
    }
    pos     = pFile->filepos;
  	len     = N * Size;
    len     = len > (pFile->size - pos) ? (pFile->size - pos) : len;
	buffer  = (__u8 *)pData;
	offset  = pos % FSYS_FAT_SEC_SIZE;

	/* 处理扇区不对齐的头部部分 */
	if (offset)
	{
	    tmpbuf    = (__u8 *)fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    	maxsct    = 1;
    	sector_nr = fsys_fat_bmap(pFile, pos, &maxsct);
    	if (sector_nr == 0)
    	{
    	    //printf("map sector number failed\n");
    	    return 0;
    	}
    	err = pFile->pPart->pdops.read((void*)tmpbuf, sector_nr, 1, pFile->pPart);
      	if (err <= 0)
      	{
        	pFile->error = FSYS_ERR_READERROR;
        	return 0;
      	}
	    chunck_size = len <= (FSYS_FAT_SEC_SIZE - offset) ? len : (FSYS_FAT_SEC_SIZE - offset);
	    memcpy(buffer, tmpbuf + offset, chunck_size);
        buffer  += chunck_size;
        len     -= chunck_size;
        pos     += chunck_size;
	}
	/* 处理扇区对齐的大块 */
	if (len / FSYS_FAT_SEC_SIZE)
	{
	    while ((maxsct = (len / FSYS_FAT_SEC_SIZE)) != 0)
	    {
	        /* 一次读取最大的连续扇区数                 */
	        sector_nr = fsys_fat_bmap(pFile, pos, &maxsct);
	        if (sector_nr == 0)
    	    {
    	        //printf("map sector number failed\n");
    	        return 0;
    	    }

	        /* 为了提高性能, 直接使用用户的buffer读取大块 */
    	    err = pFile->pPart->pdops.read((void*)buffer, sector_nr, maxsct, pFile->pPart);
      	    if (err <= 0)
      	    {
      	        if (tmpbuf)
  	            {
                    fsys_fat_free(tmpbuf);
  	            }
            	pFile->error = FSYS_ERR_READERROR;
            	return 0;
      	    }
	        chunck_size = maxsct * FSYS_FAT_SEC_SIZE;
            buffer  += chunck_size;
            len     -= chunck_size;
            pos     += chunck_size;
        }
	}
	/* 处理尾部剩余部分 */
	if (len)
	{
	    if (tmpbuf == NULL)
	    {
	        tmpbuf = (__u8 *)fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
	    }
	    maxsct = 1;
	    sector_nr = fsys_fat_bmap(pFile, pos, &maxsct);
	    if (sector_nr == 0)
    	{
    	    //printf("map sector number failed\n");
    	    return 0;
    	}
    	err = pFile->pPart->pdops.read((void*)tmpbuf, sector_nr, 1, pFile->pPart);
      	if (err <= 0)
      	{
      	    if (tmpbuf)
  	        {
                fsys_fat_free(tmpbuf);
  	        }
        	pFile->error = FSYS_ERR_READERROR;
        	return 0;
      	}
	    chunck_size = len;

	    memcpy(buffer, tmpbuf, chunck_size);
        buffer  += chunck_size;
        len     -= chunck_size;
        pos     += chunck_size;
	}
  	if (tmpbuf)
  	{
        fsys_fat_free(tmpbuf);
  	}

  	chunck_size = pos - pFile->filepos;
  	pFile->filepos = pos;
	return chunck_size;
}

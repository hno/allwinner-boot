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
*             #define constants
**********************************************************************************************************************
*/

#ifndef FSYS_FAT_NOFAT32
  #define FSYS_FAT_NOFAT32        0
#endif /* FSYS_FAT_NOFAT32 */


/*
**********************************************************************************************************************
*             Local functions
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
*
*             _FS_fat_find_file
*
  Description:
  FS internal function. Find the file with name pFileName in directory
  DirStart. Copy its directory entry to pDirEntry.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  pFileName   - File name.
  pDirEntry   - Pointer to an fsys_fat_dentry_type data structure.
  DirStart    - 1st cluster of the directory.
  DirSize     - Sector (not cluster) size of the directory.

  Return value:
  >=0         - File found. Value is the first cluster of the file.
  <0          - An error has occured.
**********************************************************************************************************************
*/
static __s32 _fsys_fat_find_file   (__fsys_part_t       *pPart,
                                    const char          *pFileName,
                                    __fsys_fat_dentry_t *pDirEntry,
                                    __u32               DirStart,
                                    __u32               DirSize)
{
    __fsys_fat_dentry_t *s;
    __u32 i;
    __u32 dsec;
    int len;
    int err;
    int c;
    char *buffer;

    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    len = strlen(pFileName);
    if (len > 11)
    {
        len = 11;
    }
    /* Read directory */
    for (i = 0; i < DirSize; i++)
    {
        dsec = fsys_fat_dir_realsec(pPart, DirStart, i);
        if (dsec == 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        err = pPart->pdops.read((void *)buffer, dsec, 1, pPart);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        s = (__fsys_fat_dentry_t *)buffer;
        while (1)
        {
            if (s >= (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
            {
                break;  /* End of sector reached */
            }
            c = strncmp((char*)s->data, pFileName, len);
            if (c == 0)
            {  /* Name does match */
                if (!(s->data[11] & FSYS_FAT_ATTR_DIRECTORY))
                {
                    break;  /* Entry found */
                }
            }
            s++;
        }
        if (s < (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
        {
            /* Entry found. Return number of 1st block of the file */
            if (pDirEntry)
            {
                memcpy(pDirEntry, s, sizeof(__fsys_fat_dentry_t));
            }
            fsys_fat_free(buffer);
            dsec  = (__u32)s->data[26];
            dsec += (__u32)s->data[27] * 0x100UL;
            dsec += (__u32)s->data[20] * 0x10000UL;
            dsec += (__u32)s->data[21] * 0x1000000UL;
            return ((__s32)dsec);
        }
    }
    fsys_fat_free(buffer);
    return -1;
}


/*
**********************************************************************************************************************
*
*             _FS_fat_IncDir
*
  Description:
  FS internal function. Increase directory starting at DirStart.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  DirStart    - 1st cluster of the directory.
  pDirSize    - Pointer to an __u32, which is used to return the new
                sector (not cluster) size of the directory.

  Return value:
  ==1         - Success.
  ==-1        - An error has occured.
**********************************************************************************************************************
*/

static int _fsys_fat_IncDir(__fsys_part_t *pPart, __u32 DirStart, __u32 *pDirSize)
{
    __u32 i;
    __u32 dsec;
    __s32 last;
    char *buffer;
    int err;

    if (DirStart == 0)
    {
        /* Increase root directory only, if not FAT12/16  */
        i = fsys_fat_aBPBUnit->RootEntCnt;
        if (i != 0)
        {
            return -1;  /* Not FAT32 */
        }
    }
    last = fsys_fat_FAT_find_eof(pPart, DirStart, 0);
    if (last < 0)
    {
        return -1;  /* No EOF marker found */
    }
    last = fsys_fat_FAT_alloc(pPart, last);  /* Allocate new cluster */
    if (last < 0)
    {
        return -1;
    }
    *pDirSize = *pDirSize + fsys_fat_aBPBUnit->SecPerClus;
    /* Clean new directory cluster */
    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    memset(buffer, 0x00, (__u32)FSYS_FAT_SEC_SIZE);
    for (i = *pDirSize - fsys_fat_aBPBUnit->SecPerClus; i < *pDirSize; i++)
    {
        dsec = fsys_fat_dir_realsec(pPart, DirStart, i);
        if (dsec == 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        err = pPart->pdops.write((void *)buffer, dsec, 1, pPart);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
    }
    fsys_fat_free(buffer);
    return 1;
}


/*
**********************************************************************************************************************
*
*             _FS_fat_create_file
*
  Description:
  FS internal function. Create a file in the directory specified
  with DirStart. Do not call, if you have not checked before for
  existing file with name pFileName.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number, which is passed to the device driver.
  pFileName   - File name.
  DirStart    - Start of directory, where to create pDirName.
  DirSize     - Sector size of the directory starting at DirStart.

  Return value:
  >=0         - 1st cluster of the new file.
  ==-1        - An error has occured.
  ==-2        - Cannot create, because directory is full.
**********************************************************************************************************************
*/

static __s32 _fsys_fat_create_file(__fsys_part_t    *pPart,
                                   const char       *pFileName,
                                   __u32             DirStart,
                                   __u32             DirSize)
{
    __fsys_fat_dentry_t  *s;
    __u32 i;
    __u32 dsec;
    __s32 cluster;
    int len;
    int err;
    __u32 val;
    char *buffer;

    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    len = strlen(pFileName);
    if (len > 11)
    {
        len = 11;
    }
    /* Read directory */
    for (i = 0; i < DirSize; i++)
    {
        dsec = fsys_fat_dir_realsec(pPart, DirStart, i);
        if (dsec == 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        err = pPart->pdops.read((void *)buffer, dsec, 1, pPart);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        s = (__fsys_fat_dentry_t *)buffer;
        while (1)
        {
            if (s >= (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
            {
                break;  /* End of sector reached */
            }
            if (s->data[0] == 0x00)
            {
                break;  /* Empty entry found */
            }
            if (s->data[0] == (unsigned char)0xe5)
            {
                break;  /* Deleted entry found */
            }
            s++;
        }
        if (s < (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
        {
            /* Free entry found. Make entry and return 1st block of the file */
            strncpy((char*)s->data, pFileName, len);
            s->data[11] = FSYS_FAT_ATTR_ARCHIVE;
            /* Alloc block in FAT */
            cluster = fsys_fat_FAT_alloc(pPart, -1);
            if (cluster >= 0)
            {
                s->data[12]     = 0x00;                           /* Res */
                s->data[13]     = 0x00;                           /* CrtTimeTenth (optional, not supported) */
                s->data[14]     = 0x00;                           /* CrtTime (optional, not supported) */
                s->data[15]     = 0x00;
                s->data[16]     = 0x00;                           /* CrtDate (optional, not supported) */
                s->data[17]     = 0x00;
                s->data[18]     = 0x00;                           /* LstAccDate (optional, not supported) */
                s->data[19]     = 0x00;
#ifdef OS_WINXP
                val             = time(NULL);
#else
                val             = 0;
#endif
                s->data[22]     = (unsigned char)(val & 0xff);   /* WrtTime */
                s->data[23]     = (unsigned char)((val >> 8) & 0xff);
                s->data[24]     = (unsigned char)((val >> 16) & 0xff);   /* WrtDate */
                s->data[25]     = (unsigned char)((val >> 24) & 0xff);
                s->data[26]     = (unsigned char)(cluster & 0xff);    /* FstClusLo / FstClusHi */
                s->data[27]     = (unsigned char)((cluster / 256) & 0xff);
                s->data[20]     = (unsigned char)((cluster / 0x10000L) & 0xff);
                s->data[21]     = (unsigned char)((cluster / 0x1000000L) & 0xff);
                s->data[28]     = 0x00;                           /* FileSize */
                s->data[29]     = 0x00;
                s->data[30]     = 0x00;
                s->data[31]     = 0x00;
                err = pPart->pdops.write((void *)buffer, dsec, 1, pPart);
                if (err < 0)
                {
                    fsys_fat_free(buffer);
                    return -1;
                }
            }
            fsys_fat_free(buffer);
            return cluster;
        }
    }
    fsys_fat_free(buffer);
    return -2;      /* Directory is full */
}


/*
**********************************************************************************************************************
*             Global functions section 1
**********************************************************************************************************************
  Functions in this section are global, but are used inside the FAT
  File System Layer only.
*/

/*
**********************************************************************************************************************
*
*             fsys_fat_DeleteFileOrDir
*
  Description:
  FS internal function. Delete a file or directory.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number, which is passed to the device driver.
  pName       - File or directory name.
  DirStart    - Start of directory, where to create pDirName.
  DirSize     - Sector size of the directory starting at DirStart.
  RmFile      - 1 => remove a file
                0 => remove a directory

  Return value:
  >=0         - Success.
  <0          - An error has occured.
**********************************************************************************************************************
*/

__s32 fsys_fat_DeleteFileOrDir(__fsys_part_t * pPart,  const char *pName,
                                    __u32 DirStart, __u32 DirSize, char RmFile)
{
    __fsys_fat_dentry_t *s;
    __u32 dsec;
    __u32 i;
    __u32 value;
    __u32 fatsize;
    __u32 filesize;
    __s32 len;
    __s32 bytespersec;
    __s32 fatindex;
    __s32 fatsec;
    __s32 fatoffs;
    __s32 lastsec;
    __s32 curclst = 0;
    __s32 todo;
    char *buffer;
    int fattype;
    int err;
    int err2;
    int lexp;
    int x;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif /* FS_FAT_NOFAT32==0 */

    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return 0;
    }
    fattype = fsys_fat_which_type(pPart);
#if (FSYS_FAT_NOFAT32!=0)
    if (fattype == 2)
    {
        fsys_fat_free(buffer);
        return -1;
    }
#endif  /* FS_FAT_NOFAT32!=0 */
    fatsize = fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        fatsize = fsys_fat_aBPBUnit->FATSz32;
    }
    bytespersec = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
    len = strlen(pName);
    if (len > 11)
    {
        len = 11;
    }
    /* Read directory */
    for (i = 0; i < DirSize; i++)
    {
        curclst = -1;
        dsec = fsys_fat_dir_realsec(pPart, DirStart, i);
        if (dsec == 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        err = pPart->pdops.read((void *)buffer, dsec, 1, pPart);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        /* Scan for pName in the directory sector */
        s = (__fsys_fat_dentry_t *) buffer;
        while (1)
        {
            if (s >= (__fsys_fat_dentry_t *)(buffer + bytespersec))
            {
                break;  /* End of sector reached */
            }
            x = strncmp((char*)s->data, pName, len);
            if (x == 0)
            { /* Name does match */
                if (s->data[11] != 0)
                {
                    break;  /* Entry found */
                }
            }
            s++;
        }
        if (s < (__fsys_fat_dentry_t *)(buffer + bytespersec))
        {
            /* Entry has been found, delete directory entry */
            s->data[0]  = 0xe5;
            s->data[11] = 0;
            err = pPart->pdops.write((void *)buffer, dsec, 1, pPart);
            if (err < 0)
            {
                fsys_fat_free(buffer);
                return -1;
            }
            /* Free blocks in FAT */
            /*
               For normal files, there are no more clusters freed than the entrie's filesize
               does indicate. That avoids corruption of the complete media in case there is
               no EOF mark found for the file (FAT is corrupt!!!).
               If the function should remove a directory, filesize if always 0 and cannot
               be used for that purpose. To avoid running into endless loop, todo is set
               to 0x0ffffff8L, which is the maximum number of clusters for FAT32.
            */
            if (RmFile)
            {
                filesize  = s->data[28] + 0x100UL * s->data[29] + 0x10000UL * s->data[30] + 0x1000000UL * s->data[31];
                todo      = filesize / (fsys_fat_aBPBUnit->SecPerClus * bytespersec);
                value     = filesize % (fsys_fat_aBPBUnit->SecPerClus * bytespersec);
                if (value != 0)
                {
                    todo++;
                }
            }
            else
            {
                todo = (__s32)0x0ffffff8L;
            }
            curclst = s->data[26] + 0x100L * s->data[27] + 0x10000L * s->data[20] + 0x1000000L * s->data[21];
            lastsec = -1;
            /* Free cluster loop */
            while (todo)
            {
                if (fattype == 1)
                {
                    fatindex = curclst + (curclst / 2);    /* FAT12 */
                }
#if (FSYS_FAT_NOFAT32==0)
                else if (fattype == 2)
                {
                    fatindex = curclst * 4;               /* FAT32 */
                }
#endif  /* FS_FAT_NOFAT32==0 */
                else
                {
                    fatindex = curclst * 2;               /* FAT16 */
                }
                fatsec = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / bytespersec);
                fatoffs = fatindex % bytespersec;
                if (fatsec != lastsec)
                {
                    err = pPart->pdops.read((void *)buffer, fatsec, 1, pPart);
                    if (err < 0)
                    {
                        err = pPart->pdops.read((void *)buffer, fatsize + fatsec, 1, pPart);
                        if (err < 0)
                        {
                            fsys_fat_free(buffer);
                            return -1;
                        }
                        /* Try to repair original FAT sector with contents of copy */
                        pPart->pdops.write((void *)buffer, fatsec, 1, pPart);
                    }
                    lastsec = fatsec;
                }
                if (fattype == 1)
                {
                    if (fatoffs == (bytespersec - 1))
                    {
                        a = buffer[fatoffs];
                        if (curclst & 1)
                        {
                            buffer[fatoffs] &= 0x0f;
                        }
                        else
                        {
                            buffer[fatoffs]  = 0x00;
                        }
                        err  = pPart->pdops.write(pPart, fatsec, 1, pPart);
                        err2 = pPart->pdops.write(pPart, fatsize + fatsec, 1, pPart);
                        lexp = (err < 0);
                        lexp = lexp || (err2 < 0);
                        if (lexp)
                        {
                            fsys_fat_free(buffer);
                            return -1;
                        }
                        err = pPart->pdops.read(pPart, fatsec + 1, 1, pPart);
                        if (err < 0)
                        {
                            err = pPart->pdops.read(pPart, fatsize + fatsec + 1, 1, pPart);
                            if (err < 0)
                            {
                                fsys_fat_free(buffer);
                                return -1;
                            }
                            /* Try to repair original FAT sector with contents of copy */
                            pPart->pdops.write((void*)buffer, fatsec + 1, 1, pPart);
                        }
                        lastsec = fatsec + 1;
                        b = buffer[0];
                        if (curclst & 1)
                        {
                            buffer[0]  = 0x00;;
                        }
                        else
                        {
                            buffer[0] &= 0xf0;
                        }
                        err  = pPart->pdops.write((void *)buffer, fatsec + 1, 1, pPart);
                        err2 = pPart->pdops.write((void *)buffer, fatsize + fatsec + 1, 1, pPart);
                        lexp = (err < 0);
                        lexp = lexp || (err2 < 0);
                        if (lexp)
                        {
                            fsys_fat_free(buffer);
                            return -1;
                        }
                    }
                    else
                    {
                        a = buffer[fatoffs];
                        b = buffer[fatoffs + 1];
                        if (curclst & 1)
                        {
                            buffer[fatoffs]     &= 0x0f;
                            buffer[fatoffs + 1]  = 0x00;
                        }
                        else
                        {
                            buffer[fatoffs]      = 0x00;
                            buffer[fatoffs + 1] &= 0xf0;
                        }
                        err  = pPart->pdops.write((void *)buffer, fatsec, 1, pPart);
                        err2 = pPart->pdops.write((void *)buffer, fatsize + fatsec, 1, pPart);
                        lexp = (err < 0);
                        lexp = lexp || (err2 < 0);
                        if (lexp)
                        {
                            fsys_fat_free(buffer);
                            return -1;
                        }
                    }
                    if (curclst & 1)
                    {
                        curclst = ((a & 0xf0) >> 4) + 16 * b;
                    }
                    else
                    {
                        curclst = a + 256 * (b & 0x0f);
                    }
                    curclst &= 0x0fff;
                    if (curclst >= 0x0ff8)
                    {
                        fsys_fat_free(buffer);
                        return 0;
                    }
                }
#if (FSYS_FAT_NOFAT32==0)
                else if (fattype == 2)
                { /* FAT32 */
                    a = buffer[fatoffs];
                    b = buffer[fatoffs + 1];
                    c = buffer[fatoffs + 2];
                    d = buffer[fatoffs + 3] & 0x0f;
                    buffer[fatoffs]      = 0x00;
                    buffer[fatoffs + 1]  = 0x00;
                    buffer[fatoffs + 2]  = 0x00;
                    buffer[fatoffs + 3]  = 0x00;
                    err  = pPart->pdops.write((void *)buffer, fatsec, 1, pPart);
                    err2 = pPart->pdops.write((void *)buffer, fatsize + fatsec, 1, pPart);
                    lexp = (err < 0);
                    lexp = lexp || (err2 < 0);
                    if (lexp)
                    {
                        fsys_fat_free(buffer);
                        return -1;
                    }
                    curclst = a + 0x100 * b + 0x10000L * c + 0x1000000L * d;
                    curclst &= 0x0fffffffL;
                    if (curclst >= (__s32)0x0ffffff8L)
                    {
                        fsys_fat_free(buffer);
                        return 0;
                    }
                }
#endif /* FS_FAT_NOFAT32==0 */
                else
                {
                    a = buffer[fatoffs];
                    b = buffer[fatoffs + 1];
                    buffer[fatoffs]     = 0x00;
                    buffer[fatoffs + 1] = 0x00;
                    err  = pPart->pdops.write((void *)buffer, fatsec, 1, pPart);
                    err2 = pPart->pdops.write((void *)buffer, fatsize + fatsec, 1, pPart);
                    lexp = (err < 0);
                    lexp = lexp || (err2 < 0);
                    if (lexp)
                    {
                        fsys_fat_free(buffer);
                        return -1;
                    }
                    curclst  = a + 256 * b;
                    curclst &= 0xffff;
                    if (curclst >= (__s32)0xfff8)
                    {
                        fsys_fat_free(buffer);
                        return 0;
                    }
                }
                todo--;
            } /* Free cluster loop */
        } /*  Delete entry */
        if (curclst > 0)
        {
            fsys_fat_free(buffer);
            return curclst;
        }
    } /* for */
    fsys_fat_free(buffer);
    return curclst;
}
//如果是小写字符，则转换成大写字符
//如果是大写字符，则原样输出
//自己写这个函数是发现c库的函数输出没有判断是大写字符，而是直接减去('a'-'A')输出
static char _toupper(char c)
{
	if((c <= 'z') && (c >= 'a'))
	{
		return c - ('a' - 'A');
	}

	return c;
}

/*
**********************************************************************************************************************
*
*             fsys_fat_make_realname
*
  Description:
  FS internal function. Convert a given name to the format, which is
  used in the FAT directory.

  Parameters:
  pOrgName    - Pointer to name to be translated
  pEntryName  - Pointer to a buffer for storing the real name used
                in a directory.

  Return value:
  None.
**********************************************************************************************************************
*/
void fsys_fat_make_realname(char *pEntryName, const char *pOrgName)
{
    char	*ext;
    char	*s;
    int i;

    s = (char *)pOrgName;
    ext = (char *) strchr(s, '.');
    if (!ext)
    {
        ext = &s[strlen(s)];
    }
    i=0;
    while (1)
    {
        if (s >= ext)
        {
            break;  /* '.' reached */
        }
        if (i >= 8)
        {
            break;  /* If there is no '.', this is the end of the name */
        }
        if (*s == (char)0xe5)
        {
            pEntryName[i] = 0x05;
        }
        else
        {
            pEntryName[i] = (char)_toupper(*s);
        }
        i++;
        s++;
    }
    while (i < 8)
    {
        /* Fill name with spaces*/
        pEntryName[i] = ' ';
        i++;
    }
    if (*s == '.')
    {
        s++;
    }
    while (i < 11)
    {
        if (*s != 0)
        {
            if (*s == (char)0xe5)
            {
                pEntryName[i] = 0x05;
            }
            else
            {
                pEntryName[i] = (char)_toupper(*s);
            }
            s++;
        }
        else
        {
            pEntryName[i] = ' ';
        }
        i++;
    }
    pEntryName[11]=0;
}

/*
**********************************************************************************************************************
*
*             fsys_fat_find_dir
*
  Description:
  FS internal function. Find the directory with name pDirName in directory
  DirStart.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  pDirName    - Directory name; if zero, return the root directory.
  DirStart    - 1st cluster of the directory.
  DirSize     - Sector (not cluster) size of the directory.

  Return value:
  >0          - Directory found. Value is the first cluster of the file.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

__u32 fsys_fat_find_dir(__fsys_part_t *pPart, char *pDirName, __u32 DirStart, __u32 DirSize)
{
    __fsys_fat_dentry_t  *s;
    __u32 dstart;
    __u32 i;
    __u32 dsec;
    __u32 fatsize;
    int len;
    int err;
    int c;
    char *buffer;

    if (pDirName == 0)
    {
        /* Return root directory */
        if (fsys_fat_aBPBUnit->FATSz16)
        {
            fatsize = fsys_fat_aBPBUnit->FATSz16;
            dstart  = fsys_fat_aBPBUnit->RsvdSecCnt + fsys_fat_aBPBUnit->NumFATs * fatsize;
        }
        else
        {
            fatsize = fsys_fat_aBPBUnit->FATSz32;
            dstart  = fsys_fat_aBPBUnit->RsvdSecCnt + fsys_fat_aBPBUnit->NumFATs * fatsize
                    + (fsys_fat_aBPBUnit->RootClus - 2) * fsys_fat_aBPBUnit->SecPerClus;
        }
    }
    else
    {
        /* Find directory */
        buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
        if (!buffer)
        {
            return 0;
        }
        len = strlen(pDirName);
        if (len > 11)
        {
            len = 11;
        }
        /* Read directory */
        for (i = 0; i < DirSize; i++)
        {
            dsec = fsys_fat_dir_realsec(pPart, DirStart, i);
            if (dsec == 0)
            {
                fsys_fat_free(buffer);
                return 0;
            }
            err = pPart->pdops.read((void *)buffer, dsec, 1, pPart);
            if (err < 0)
            {
                fsys_fat_free(buffer);
                return 0;
            }
            s = (__fsys_fat_dentry_t *)buffer;
            while (1)
            {
                if (s >= (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
                {
                    break;  /* End of sector reached */
                }
                c = strncmp((char*)s->data, pDirName, len);
                if (c == 0)
                { /* Name does match */
                    if (s->data[11] & FSYS_FAT_ATTR_DIRECTORY)
                    {
                        break;  /* Entry found */
                    }
                }
                s++;
            }
            if (s < (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
            {
                /* Entry found. Return number of 1st block of the directory */
                fsys_fat_free(buffer);
                dstart  = (__u32)s->data[26];
                dstart += (__u32)0x100UL * s->data[27];
                dstart += (__u32)0x10000UL * s->data[20];
                dstart += (__u32)0x1000000UL * s->data[21];
                return dstart;
            }
        }
        dstart = 0;
        fsys_fat_free(buffer);
    }
    return dstart;
}

/*
**********************************************************************************************************************
*
*             fsys_fat_dir_realsec
*
  Description:
  FS internal function. Translate a directory relative sector number
  to a real sector number on the media.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  DirStart    - 1st cluster of the directory. This is zero to address
                the root directory.
  DirSec      - Sector in the directory.

  Return value:
  >0          - Directory found. Value is the sector number on the media.
  ==0         - An error has occured.
**********************************************************************************************************************
*/
__u32 fsys_fat_dir_realsec(__fsys_part_t *pPart, __u32 DirStart, __u32 DirSec)
{
    __u32 rootdir;
    __u32 rsec;
    __u32 dclust;
    __u32 fatsize;
    int fattype;
    int lexp;
    unsigned char secperclus;

    fattype = fsys_fat_which_type(pPart);
    lexp = (0 == DirStart);
    lexp = lexp && (fattype != 2);
    if (lexp)
    {
        /* Sector in FAT12/FAT16 root directory */
        rootdir = fsys_fat_find_dir(pPart, 0, 0, 0);
        rsec = rootdir + DirSec;
    }
    else
    {
        fatsize = fsys_fat_aBPBUnit->FATSz16;
        if (fatsize == 0)
        {
            fatsize = fsys_fat_aBPBUnit->FATSz32;
        }
        secperclus = fsys_fat_aBPBUnit->SecPerClus;
        dclust = DirSec / secperclus;
        if (0 == DirStart)
        {
            /* FAT32 root directory */
            rsec = fsys_fat_aBPBUnit->RootClus;
        }
        else
        {
            rsec = fsys_fat_diskclust(pPart, DirStart, dclust);
            if (rsec == 0)
            {
                return 0;
            }
        }
        rsec -= 2;
        rsec *= secperclus;
        rsec += fsys_fat_aBPBUnit->RsvdSecCnt + fsys_fat_aBPBUnit->NumFATs * fatsize;
        rsec += ((__u32)((__u32)fsys_fat_aBPBUnit->RootEntCnt) * FSYS_FAT_DENTRY_SIZE) / FSYS_FAT_SEC_SIZE;
        rsec += (DirSec % secperclus);
    }
    return rsec;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_dirsize
*
  Description:
  FS internal function. Return the sector size of the directory
  starting at DirStart.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  DirStart    - 1st cluster of the directory. This is zero to address
                the root directory.

  Return value:
  >0          - Sector (not cluster) size of the directory.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

__u32 fsys_fat_dir_size(__fsys_part_t *pPart, __u32 DirStart)
{
    __u32 dsize;
    __s32 value;

    if (DirStart == 0)
    {
        /* For FAT12/FAT16 root directory, the size can be found in BPB */
        dsize = ((__u32)((__u32)fsys_fat_aBPBUnit->RootEntCnt)
                * FSYS_FAT_DENTRY_SIZE) / ((__u32)fsys_fat_aBPBUnit->BytesPerSec);
        if (dsize == 0)
        {
            /* Size in BPB is 0, so it is a FAT32 (FAT32 does not have a real root dir) */
            value = fsys_fat_FAT_find_eof(pPart, fsys_fat_aBPBUnit->RootClus, &dsize);
            if (value < 0)
            {
                dsize = 0;
            }
            else
            {
                dsize *= fsys_fat_aBPBUnit->SecPerClus;
            }
        }
    }
    else
    {
        /* Calc size of a sub-dir */
        value = fsys_fat_FAT_find_eof(pPart, DirStart, &dsize);
        if (value < 0)
        {
            dsize = 0;
        }
        else
        {
            dsize *= fsys_fat_aBPBUnit->SecPerClus;
        }
    }
    return dsize;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_findpath
*
  Description:
  FS internal function. Return start cluster and size of the directory
  of the file name in pFileName.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  pFullName   - Fully qualified file name w/o device name.
  pFileName   - Pointer to a pointer, which is modified to point to the
                file name part of pFullName.
  pUnit       - Pointer to an __u32 for returning the unit number.
  pDirStart   - Pointer to an __u32 for returning the start cluster of
                the directory.

  Return value:
  >0          - Sector (not cluster) size of the directory.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

__u32 fsys_fat_findpath(__fsys_part_t *pPart, const char *pFullName, char **pFileName, __u32 *pDirStart)
{
    __u32 dsize;
    __s32 i;
    __s32 j;
    char *dname_start;
    char *dname_stop;
    char *chprt;
    int x;
    char dname[12];
    char realname[12];

	*pFileName = (char *)pFullName;
    /* Check volume */
    x = !fsys_fat_checkunit(pPart);
    if (x)
    {
        return 0;
    }
    /* Setup pDirStart/dsize for root directory */
    *pDirStart = 0;
    dsize      = fsys_fat_dir_size(pPart, 0);
    /* Find correct directory */
    do
    {
        dname_start = (char *)strchr(*pFileName, '\\');
        if (dname_start)
        {
            dname_start++;
            *pFileName = dname_start;
            dname_stop = (char *)strchr(dname_start, '\\');
        }
        else
        {
            dname_stop = 0;
        }
        if (dname_stop)
        {
            i = dname_stop-dname_start;
            if (i >= 12)
            {
                j = 0;
                for (chprt = dname_start; chprt < dname_stop; chprt++)
                {
                    if (*chprt == '.')
                    {
                        i--;
                    }
                    else if (j < 12)
                    {
                        realname[j] = *chprt;
                        j++;
                    }
                }
                if (i >= 12)
                {
                    return 0;
                }
            }
            else
            {
                strncpy(realname, dname_start, i);
            }
            realname[i] = 0;
            fsys_fat_make_realname(dname, realname);
            *pDirStart =  fsys_fat_find_dir(pPart, dname, *pDirStart, dsize);
            if (*pDirStart)
            {
                dsize  =  fsys_fat_dir_size(pPart, *pDirStart);
            }
            else
            {
                dsize = 0;    /* Directory NOT found */
            }
        }
    } while (dname_start);
    return dsize;
}


/*
**********************************************************************************************************************
*             Global functions section 2
**********************************************************************************************************************
  These are real global functions, which are used by the API Layer
  of the file system.

*/

/*
**********************************************************************************************************************
*
*             fsys_fat_fopen
*
  Description:
  FS internal function. Open an existing file or create a new one.

  Parameters:
  pFileName   - File name.
  pMode       - Mode for opening the file.
  pFile       - Pointer to an FS_FILE data structure.

  Return value:
  ==0         - Unable to open the file.
  !=0         - Address of the FS_FILE data structure.
**********************************************************************************************************************
*/

__fsys_file_t *fsys_fat_fopen(const char *pFileName, const char *pMode, __fsys_file_t *pFile)
{
    __u32 dstart;
    __u32 dsize;
    __s32 i;
    char 	*fname;
    __fsys_fat_dentry_t  s;
    char realname[12];
    int lexp_a;
    int lexp_b;

    if (!pFile)
    {
        return 0;  /* Not a valid pointer to an __fsys_file_t structure*/
    }
    dsize = fsys_fat_findpath(pFile->pPart, pFileName, &fname, &dstart);
    if (dsize == 0)
    {
        return 0;  /* Directory not found */
    }
    //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_INC_BUSYCNT, 0, (void*)0);  /* Turn on busy signal */
    fsys_fat_make_realname(realname, fname);  /* Convert name to FAT real name */
    /* FileSize = 0 */
    s.data[28] = 0x00;
    s.data[29] = 0x00;
    s.data[30] = 0x00;
    s.data[31] = 0x00;
    i = _fsys_fat_find_file(pFile->pPart, realname, &s, dstart, dsize);
    /* Delete file */
    lexp_b = (strcmp(pMode, "del") == 0);      /* Delete file request */
    lexp_a = lexp_b && (i >= 0);                      /* File does exist */
    if (lexp_a)
    {
        i = fsys_fat_DeleteFileOrDir(pFile->pPart, realname, dstart, dsize, 1);
        if (i != 0)
        {
            pFile->error = -1;
        }
        else
        {
            pFile->error = 0;
        }
        //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_FLUSH_CACHE, 2, (void*)0);
        //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
        return 0;
    }
    else if (lexp_b)
    {
        //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
        pFile->error = -1;
        return 0;
    }
    /* Check read only */
    lexp_a = ((i >= 0) && ((s.data[11] & FSYS_FAT_ATTR_READ_ONLY) != 0)) &&
            ((pFile->mode_w) || (pFile->mode_a) || (pFile->mode_c));
    if (lexp_a)
    {
        /* Files is RO and we try to create, write or append */
        //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
        return 0;
    }
    lexp_a = ( i>= 0) && (!pFile->mode_a) && (((pFile->mode_w) && (!pFile->mode_r)) ||
              ((pFile->mode_w) && (pFile->mode_c) && (pFile->mode_r)) );
    if (lexp_a)
    {
        /* Delete old file */
        i = fsys_fat_DeleteFileOrDir(pFile->pPart, realname, dstart, dsize, 1);
        /* FileSize = 0 */
        s.data[28] = 0x00;
        s.data[29] = 0x00;
        s.data[30] = 0x00;
        s.data[31] = 0x00;
        i=-1;
    }
    if ((!pFile->mode_c) && (i < 0))
    {
        /* File does not exist and we must not create */
        //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
        return 0;
    }
    else if ((pFile->mode_c) && (i < 0))
    {
        /* Create new file */
        i = _fsys_fat_create_file(pFile->pPart, realname, dstart, dsize);
        if (i < 0)
        {
            /* Could not create file */
            if (i == -2)
            {
                /* Directory is full, try to increase */
                i = _fsys_fat_IncDir(pFile->pPart, dstart, &dsize);
                if (i > 0)
                {
                    i = _fsys_fat_create_file(pFile->pPart, realname, dstart, dsize);
                }
            }
            if (i < 0)
            {
                //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
                return 0;
            }
        }
    }

    pFile->fileid_hi  = i;
    pFile->fileid_ex  = dstart;
    pFile->EOFClust   = -1;
    pFile->CurClust   = 0;
    pFile->error      = 0;
    pFile->size       = (__u32)s.data[28];   /* FileSize */
    pFile->size      += (__u32)0x100UL * s.data[29];
    pFile->size      += (__u32)0x10000UL * s.data[30];
    pFile->size      += (__u32)0x1000000UL * s.data[31];
    if (pFile->mode_a)
    {
        pFile->filepos   = pFile->size;
    }
    else
    {
        pFile->filepos   = 0;
    }
    return pFile;
}


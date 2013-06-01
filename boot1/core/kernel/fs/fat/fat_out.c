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
*             Local functions
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
*
*             _FS_fat_write_dentry
*
  Description:
  FS internal function. Write a directory entry.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  FirstClust  - First cluster of the file, which's directory entry
                will be written.
  pDirEntry   - Pointer to an fsys_fat_dentry_type structure, which
                contains the new directory entry.
  DirSec      - Sector, which contains the directory entry.
  pBuffer     - Pointer to a buffer, which contains the sector with
                the old directory entry.

  Return value:
  ==1         - Directory entry has been written.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

static int _fsys_fat_write_dentry(  __fsys_part_t       * pPart,
                                    __u32                 FirstClust,
                                    __fsys_fat_dentry_t * pDirEntry,
                                    __u32                 DirSec,
                                    char                * pBuffer)
{
    __fsys_fat_dentry_t *s;
    __u32 value;
    int err;

    if (DirSec == 0)
    {
        return 0;  /* Not a valid directory sector */
    }
    if (pBuffer == 0)
    {
        return 0;  /* No buffer */
    }
    /* Scan for the directory entry with FirstClust in the directory sector */
    s = (__fsys_fat_dentry_t *)pBuffer;
    while (1)
    {
        if (s >= (__fsys_fat_dentry_t *)(pBuffer + FSYS_FAT_SEC_SIZE))
        {
            break;  /* End of sector reached */
        }
        value = (__u32)s->data[26] + 0x100UL * s->data[27] + 0x10000UL * s->data[20] + 0x1000000UL * s->data[21];
        if (value == FirstClust)
        {
            break;  /* Entry found */
        }
        s++;
    }
    if (s < (__fsys_fat_dentry_t *)(pBuffer + FSYS_FAT_SEC_SIZE))
    {
        if (pDirEntry)
        {
            memcpy(s, pDirEntry, sizeof(__fsys_fat_dentry_t));
            err = (pPart->pdops.write)((void*)pBuffer, DirSec, 1, pPart);
            if (err < 0)
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}


/*
**********************************************************************************************************************
*
*             _FS_fat_read_dentry
*
  Description:
  FS internal function. Read a directory entry.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  FirstClust  - First cluster of the file, which's directory entry
                will be read.
  DirStart    - Start of directory, where to read the entry.
  pDirEntry   - Pointer to an fsys_fat_dentry_type structure, which is
                used to read the directory entry.
  pDirSec     - Pointer to an __u32, which is used to store the sector
                number, in which the directory entry has been read.
  pBuffer     - Pointer to a buffer, which is used for reading the
                directory.

  Return value:
  ==1         - Directory entry has been read.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

static int _fsys_fat_read_dentry(   __fsys_part_t       *pPart,
                                    __u32                FirstClust,
                                    __u32                DirStart,
                                    __fsys_fat_dentry_t *pDirEntry,
                                    __u32               *pDirSec,
                                    char                *pBuffer)
{
    __u32 i;
    __u32 dsize;
    __u32 value;
    __fsys_fat_dentry_t *s;
    int err;

    if (pBuffer == 0)
    {
        return 0;
    }
    dsize  =  fsys_fat_dir_size(pPart, DirStart);
    /* Read the directory */
    for (i = 0; i < dsize; i++)
    {
        *pDirSec = fsys_fat_dir_realsec(pPart, DirStart, i);
        if (*pDirSec == 0)
        {
            return 0;  /* Unable to translate relative directory sector to absolute setor */
        }
        err = (pPart->pdops.read)((void*)pBuffer, *pDirSec, 1, pPart);
        if (err < 0)
        {
            return 0;
        }
        /* Scan for entry with FirstClus in the sector */
        s = (__fsys_fat_dentry_t *)pBuffer;
        while (1)
        {
            if (s >= (__fsys_fat_dentry_t *)(pBuffer + FSYS_FAT_SEC_SIZE))
            {
                break;  /* End of sector reached */
            }
            value = (__u32)s->data[26] + 0x100UL * s->data[27] + 0x10000UL * s->data[20] + 0x1000000UL * s->data[21];
            if (value == FirstClust)
            {
                break;  /* Entry found */
            }
            s++;
        }
        if (s < (__fsys_fat_dentry_t *)(pBuffer + FSYS_FAT_SEC_SIZE))
        {
            if (pDirEntry)
            {
                /* Read the complete directory entry from the buffer */
                memcpy(pDirEntry, s, sizeof(__fsys_fat_dentry_t));
            }
            return 1;
        }
    }
    return 0;
}


/*********************************************************************
*             Global functions
**********************************************************************
*/

/*********************************************************************
*
*             fsys_fat_fwrite
*
  Description:
  FS internal function. Write data to a file.

  Parameters:
  pData       - Pointer to data, which will be written to the file.
  Size        - Size of an element to be transferred to a file.
  N           - Number of elements to be transferred to the file.
  pFile       - Pointer to a FS_FILE data structure.

  Return value:
  Number of elements written.
*/

__u32 fsys_fat_fwrite(const void *pData, __u32 Size, __u32 N, __fsys_file_t *pFile)
{
    __u32 todo;
    __u32 dstart;
    __u32 dsize;
    __u32 bytesperclus;
    __u32 datastart;
    __u32 fatsize;
    __u32 fileclustnum;
    __u32 diskclustnum;
    __u32 prevclust;
    __s32 last;
    __s32 i;
    __s32 j;
#if (FSYS_FAT_FWRITE_UPDATE_DIR)
    __fsys_fat_dentry_t  s;
    __u32 dsec;
    __u32 val;
#endif /* FS_FAT_FWRITE_UPDATE_DIR */
    int err;
    int lexp;
    char *buffer;

    if (!pFile)
    {
        return 0;
    }
    /* Check if media is OK */
    err = pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_PART_CMD_GET_STATUS, 0, 0);
    if (err == FSYS_PART_MEDIACHANGED)
    {
        pFile->error = FSYS_ERR_DISKCHANGED;
        return 0;
    }
    else if (err < 0)
    {
        pFile->error = FSYS_ERR_WRITEERROR;
        return 0;
    }

    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return 0;
    }
    fatsize = f_fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        /* FAT32 */
        fatsize = f_fsys_fat_aBPBUnit->FATSz32;
    }
    todo = N * Size;  /* Number of bytes to be written */
    if (!todo)
    {
        fsys_fat_free(buffer);
        return 0;
    }
    /* Alloc new clusters if required */
    bytesperclus = (__u32)f_fsys_fat_aBPBUnit->SecPerClus * ((__u32)f_fsys_fat_aBPBUnit->BytesPerSec);

    /* Calculate number of clusters required */
    i = (pFile->filepos + todo) / bytesperclus;
    if ((pFile->filepos + todo) % bytesperclus)
    {
        i++;
    }
    /* Calculate clusters already allocated */
    j = pFile->size / bytesperclus;
    lexp = (pFile->size % bytesperclus);
    lexp = lexp || (pFile->size == 0);
    if (lexp)
    {
        j++;
    }
    i -= j;
    if (i > 0)
    {
        /* Alloc new clusters */
        last = pFile->EOFClust;
        if (last < 0)
        {
            /* Position of EOF is unknown, so we scan the whole file to find it */
            last = fsys_fat_FAT_find_eof(pFile->pPart, pFile->fileid_hi, 0);
        }
        if (last < 0)
        {
            /* No EOF found */
            fsys_fat_free(buffer);
            return 0;
        }
        while (i)
        {
            last = fsys_fat_FAT_alloc(pFile->pPart, last);  /* Allocate new cluster */
            pFile->EOFClust = last;
            if (last < 0)
            {
                /* Cluster allocation failed */
                pFile->size += (N * Size - todo);
                pFile->error = FSYS_ERR_DISKFULL;
                fsys_fat_free(buffer);
                return ((N * Size - todo) / Size);
            }
            i--;
        }
    }
    /* Get absolute postion of data area on the media */
    dstart    = (__u32)f_fsys_fat_aBPBUnit->RsvdSecCnt + f_fsys_fat_aBPBUnit->NumFATs * fatsize;
    dsize     = ((__u32)((__u32)f_fsys_fat_aBPBUnit->RootEntCnt) * FSYS_FAT_DENTRY_SIZE) / FSYS_FAT_SEC_SIZE;
    datastart = dstart + dsize;
    /* Write data to clusters */
    prevclust = 0;
    while (todo)
    {  /* Write data loop */
        /* Translate file ppinter position to cluster position*/
        fileclustnum = pFile->filepos / bytesperclus;
        /*
            Translate the file relative cluster position to an absolute cluster
            position on the media. To avoid scanning the whole FAT of the file,
            we remember the current cluster position in the FS_FILE data structure.
        */
        if (prevclust == 0)
        {
            diskclustnum = pFile->CurClust;
            if (diskclustnum == 0)
            {
                /* No known current cluster position, we have to scan from the file's start cluster */
                diskclustnum = fsys_fat_diskclust(pFile->pPart, pFile->fileid_hi, fileclustnum);
            }
        }
        else
        {
            /* Get next cluster of the file starting at the current cluster */
            diskclustnum = fsys_fat_diskclust(pFile->pPart, prevclust, 1);
        }
        prevclust        = diskclustnum;
        pFile->CurClust  = diskclustnum;
        if (diskclustnum == 0)
        {
            /* Translation to absolute cluster failed */
            pFile->error = FSYS_ERR_WRITEERROR;
            fsys_fat_free(buffer);
            return ((N * Size - todo) / Size);
        }
        diskclustnum -= 2;
        j = (pFile->filepos % bytesperclus) / FSYS_FAT_SEC_SIZE;
        while (1)
        {  /* Cluster loop */
            if (!todo)
            {
                break;  /* Nothing more to write */
            }
            if (j >= f_fsys_fat_aBPBUnit->SecPerClus)
            {
                break; /* End of cluster reached */
            }
            i = pFile->filepos % FSYS_FAT_SEC_SIZE;
            /*
                We only have to read the sector from the media, if we do not
                modify the whole sector. That is the case if

                a) Writing starts not at the first byte of the sector
                b) Less data than the sector contains is written
            */
            lexp = (i != 0);
            lexp = lexp || (todo < FSYS_FAT_SEC_SIZE);
            if (lexp)
            {
                /* We have to read the old sector */
                err = pFile->pPart->pdops.read((void*)buffer,
                                datastart + diskclustnum * f_fsys_fat_aBPBUnit->SecPerClus + j,
                                1,
                                pFile->pPart);
                if (err < 0)
                {
                    pFile->error = FSYS_ERR_WRITEERROR;
                    fsys_fat_free(buffer);
                    return ((N * Size - todo) / Size);
                }
            }
            while (1)
            {  /* Sector loop */
                if (!todo)
                {
                    break;  /* Nothing more to write */
                }
                if (i >= FSYS_FAT_SEC_SIZE)
                {
                    break;  /* End of sector reached */
                }
                buffer[i] = *((char *)(((char *)pData) + N * Size - todo));
                i++;
                pFile->filepos++;
                if (pFile->filepos > pFile->size)
                {
                    pFile->size = pFile->filepos;
                }
                todo--;
            }  /* Sector loop */
            /* Write the modified sector */
            err = pFile->pPart->pdops.write((void*)buffer,
                          datastart + diskclustnum * f_fsys_fat_aBPBUnit->SecPerClus + j,
                          1,
                          pFile->pPart);
            if (err < 0)
            {
                pFile->error = FSYS_ERR_WRITEERROR;
                fsys_fat_free(buffer);
                return ((N * Size - todo) / Size);
            }
            j++;
        }  /* Cluster loop */
    } /* Write data loop */
    if (i >= FSYS_FAT_SEC_SIZE)
    {
        if (j >= f_fsys_fat_aBPBUnit->SecPerClus)
        {
            /* File pointer is already in the next cluster */
            pFile->CurClust = fsys_fat_diskclust(pFile->pPart, prevclust, 1);
        }
    }
#if (FSYS_FAT_FWRITE_UPDATE_DIR)
    /* Modify directory entry */
    err = _fsys_fat_read_dentry(pFile->pPart, pFile->fileid_hi, pFile->fileid_ex, &s, &dsec, buffer);
    if (err == 0)
    {
        pFile->error = FSYS_ERR_WRITEERROR;
        fsys_fat_free(buffer);
        return ((N * Size - todo) / Size);
    }
    s.data[28] = (unsigned char)(pFile->size & 0xff);   /* FileSize */
    s.data[29] = (unsigned char)((pFile->size / 0x100UL) & 0xff);
    s.data[30] = (unsigned char)((pFile->size / 0x10000UL) & 0xff);
    s.data[31] = (unsigned char)((pFile->size / 0x1000000UL) & 0xff);
#ifdef OS_WINXP
    val             = time(NULL);
#else
    val             = 0;
#endif
    s.data[22]     = (unsigned char)(val & 0xff);   /* WrtTime */
    s.data[23]     = (unsigned char)((val >> 8) & 0xff);
    s.data[24]     = (unsigned char)((val >> 16) & 0xff);   /* WrtDate */
    s.data[25]     = (unsigned char)((val >> 24) & 0xff);
    err = _fsys_fat_write_dentry(pFile->pPart, pFile->fileid_hi, &s, dsec, buffer);
    if (err == 0)
    {
        pFile->error = FSYS_ERR_WRITEERROR;
    }
#endif /* FS_FAT_FWRITE_UPDATE_DIR */
    fsys_fat_free(buffer);
    return ((N * Size - todo) / Size);
}


/*
**********************************************************************************************************************
*
*             fsys_fat_fclose
*
  Description:
  FS internal function. Close a file referred by pFile.

  Parameters:
  pFile       - Pointer to a FS_FILE data structure.

  Return value:
  None.
**********************************************************************************************************************
*/

__s32 fsys_fat_fclose(__fsys_file_t *pFile)
{
	__s32	ret = EPDK_OK;

#if (FSYS_FAT_FWRITE_UPDATE_DIR == 0)
  	__fsys_fat_dentry_t s;
	char *buffer;
	__u32 dsec;
	__u32 val;
#endif /* FS_FAT_FWRITE_UPDATE_DIR */
	int err;

	if (!pFile)
	{
      	return EPDK_FAIL;
  	}
 	/* Check if media is OK */
  	err = pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_PART_CMD_GET_STATUS, 0, 0);
  	if (err == FSYS_PART_MEDIACHANGED)
    {
    	pFile->error = FSYS_ERR_DISKCHANGED;
    	//pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
	    pFile->inuse = 0;
	    return EPDK_FAIL;
	}
	else if (err < 0)
	{
    	pFile->error = FSYS_ERR_CLOSE;
	    //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
    	pFile->inuse = 0;
	    return EPDK_FAIL;
	}
#if (FSYS_FAT_FWRITE_UPDATE_DIR == 0)
	/* Modify directory entry */
	buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
	if (!buffer)
	{
	    pFile->inuse = 0;
    	pFile->error = FSYS_ERR_CLOSE;
	    return EPDK_FAIL;
	}
	err = _fsys_fat_read_dentry(pFile->pPart, pFile->fileid_hi, pFile->fileid_ex, &s, &dsec, buffer);
  	if (err == 0)
  	{
    	pFile->inuse = 0;
	    pFile->error = FSYS_ERR_CLOSE;
    	fsys_fat_free(buffer);
	    return EPDK_FAIL;
	}
	s.data[28] = (unsigned char)(pFile->size & 0xff);   /* FileSize */
  	s.data[29] = (unsigned char)((pFile->size / 0x100UL) & 0xff);
  	s.data[30] = (unsigned char)((pFile->size / 0x10000UL) & 0xff);
  	s.data[31] = (unsigned char)((pFile->size / 0x1000000UL) & 0xff);
#ifdef OS_WINXP
    val             = time(NULL);
#else
    val             = 0;
#endif
    s.data[22]     = (unsigned char)(val & 0xff);   /* WrtTime */
    s.data[23]     = (unsigned char)((val >> 8) & 0xff);
    s.data[24]     = (unsigned char)((val >> 16) & 0xff);   /* WrtDate */
    s.data[25]     = (unsigned char)((val >> 24) & 0xff);
    err = _fsys_fat_write_dentry(pFile->pPart, pFile->fileid_hi, &s, dsec, buffer);
    if (err == 0)
    {
        pFile->error = FSYS_ERR_CLOSE;
        ret = EPDK_FAIL;
    }
    fsys_fat_free(buffer);
#endif /* FS_FAT_FWRITE_UPDATE_DIR */
//    err = pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_FLUSH_CACHE, 2, (void*)0);
    if (err < 0)
    {
        pFile->error = FSYS_ERR_WRITEERROR;
        ret = EPDK_FAIL;
    }
    pFile->inuse = 0;
    //pFile->pPart->pdops.ioctl(pFile->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
    return ret;
}



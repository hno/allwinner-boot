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
*
*             _fsys_fat_create_directory
*
  Description:
  FS internal function. Create a directory in the directory specified
  with DirStart. Do not call, if you have not checked before for
  existing directory with name pDirName.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number, which is passed to the device driver.
  pDirName    - Directory name.
  DirStart    - Start of directory, where to create pDirName.
  DirSize     - Size of the directory starting at DirStart.

  Return value:
  >=0         - Directory has been created.
  <0          - An error has occured.
**********************************************************************************************************************
*/

static int _fsys_fat_create_directory(__fsys_part_t *pPart,
									const char 	    *pDirName,
                                    __u32 		    DirStart,
                                    __u32 		    DirSize)
{
    char                *buffer;
    __fsys_fat_dentry_t *s;
    __u32               dirindex;
    __u32               dsec;
    __s32               cluster;
    __u32               val;
    int                 err;
    int                 len;
    int                 j;

    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    len = strlen(pDirName);
    if (len > 11)
    {
        len = 11;
    }
    /* Read directory */
    for (dirindex = 0; dirindex < DirSize; dirindex++)
    {
        dsec = fsys_fat_dir_realsec(pPart, DirStart, dirindex);
        if (dsec == 0)
        {
            /* Translation of relativ directory sector to an absolute sector failed */
            fsys_fat_free(buffer);
            return -1;
        }
        err = pPart->pdops.read((void*)buffer, dsec, 1, pPart); /* Read directory sector */
        if (err < 0)
        {
            /* Read error */
            fsys_fat_free(buffer);
            return -1;
        }
        /* Scan the directory sector for a free or deleted entry */
        s = (__fsys_fat_dentry_t *)buffer;
        while (1)
        {
            if (s >= (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
            {
                break;  /* End of sector reached */
            }
            if (s->data[0] == 0x00)
            {
                break;  /* Found a free entry */
            }
            if (s->data[0] == (unsigned char)0xe5)
            {
                break;  /* Found a deleted entry */
            }
            s++;
        }
        if (s < (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
        {
            /* Free entry found. Make entry and return 1st block of the file. */
            strncpy((char*)s->data, pDirName, len);
            s->data[11] = FSYS_FAT_ATTR_DIRECTORY;
            cluster = fsys_fat_FAT_alloc(pPart, -1);                   /* Alloc block in FAT */
            if (cluster >= 0)
            {
                s->data[12]     = 0x00;                                /* Res */
                s->data[13]     = 0x00;                                /* CrtTimeTenth (optional, not supported) */
                s->data[14]     = 0x00;                                /* CrtTime (optional, not supported) */
                s->data[15]     = 0x00;
                s->data[16]     = 0x00;                                /* CrtDate (optional, not supported) */
                s->data[17]     = 0x00;
                s->data[18]     = 0x00;                                /* LstAccDate (optional, not supported) */
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
                s->data[28]     = 0x00;                                /* FileSize */
                s->data[29]     = 0x00;
                s->data[30]     = 0x00;
                s->data[31]     = 0x00;
                err = pPart->pdops.write((void*)buffer, dsec, 1, pPart);      /* Write the modified directory sector      */
                if (err < 0)
                {
                     fsys_fat_free(buffer);
                     return -1;
                }
                /* Clear new directory and make '.' and '..' entries */
                /* Make "." entry */
                memset(buffer, 0x00, (__u32)FSYS_FAT_SEC_SIZE);
                s = (__fsys_fat_dentry_t *)buffer;
                strncpy((char*)s->data, ".          ", 11);
                s->data[11]     = FSYS_FAT_ATTR_DIRECTORY;
                s->data[22]     = (unsigned char)(val & 0xff);   /* WrtTime */
                s->data[23]     = (unsigned char)((val >> 8) & 0xff);
                s->data[24]     = (unsigned char)((val >> 16) & 0xff);   /* WrtDate */
                s->data[25]     = (unsigned char)((val >> 24) & 0xff);
                s->data[26]     = (unsigned char)(cluster & 0xff);    /* FstClusLo / FstClusHi */
                s->data[27]     = (unsigned char)((cluster / 256) & 0xff);
                s->data[20]     = (unsigned char)((cluster / 0x10000L) & 0xff);
                s->data[21]     = (unsigned char)((cluster / 0x1000000L) & 0xff);
                /* Make entry ".." */
                s++;
                strncpy((char*)s->data, "..         ", 11);
                s->data[11]     = FSYS_FAT_ATTR_DIRECTORY;
                s->data[22]     = (unsigned char)(val & 0xff);   /* WrtTime */
                s->data[23]     = (unsigned char)((val >> 8) & 0xff);
                s->data[24]     = (unsigned char)((val >> 16) & 0xff);   /* WrtDate */
                s->data[25]     = (unsigned char)((val >> 24) & 0xff);
                s->data[26]     = (unsigned char)(DirStart & 0xff);    /* FstClusLo / FstClusHi */
                s->data[27]     = (unsigned char)((DirStart / 256) & 0xff);
                s->data[20]     = (unsigned char)((DirStart / 0x10000L) & 0xff);
                s->data[21]     = (unsigned char)((DirStart / 0x1000000L) & 0xff);
                dsec = fsys_fat_dir_realsec(pPart, cluster, 0); /* Find 1st absolute sector of the new directory */
                if (dsec == 0)
                {
                    fsys_fat_free(buffer);
                    return -1;
                }
                /* Write "." & ".." entries into the new directory */
                err = pPart->pdops.write((void*)buffer, dsec, 1, pPart);
                if (err < 0)
                {
                    fsys_fat_free(buffer);
                    return -1;
                }
                /* Clear rest of the directory cluster */
                memset(buffer, 0x00, (__u32)FSYS_FAT_SEC_SIZE);
                for (j = 1; j < fsys_fat_aBPBUnit->SecPerClus; j++)
                {
                    dsec = fsys_fat_dir_realsec(pPart, cluster, j);
                    err = pPart->pdops.write((void*)buffer, dsec, 1, pPart);
                    if (err < 0)
                    {
                        fsys_fat_free(buffer);
                        return -1;
                    }
                }
                fsys_fat_free(buffer);
                return 1;

            }
            fsys_fat_free(buffer);
            return -1;
        }
    }
    fsys_fat_free(buffer);
    return -2;  /* Directory is full */
}


/*
**********************************************************************************************************************
*             Global functions
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
*
*             fsys_fat_opendir
*
  Description:
  FS internal function. Open an existing directory for reading.

  Parameters:
  pDirName    - Directory name.
  pDir        - Pointer to a FS_DIR data structure.

  Return value:
  ==0         - Unable to open the directory.
  !=0         - Address of an FS_DIR data structure.
**********************************************************************************************************************
*/

__fsys_dir_t *fsys_fat_opendir(const char *pDirName, __fsys_dir_t *pDir)
{
    __u32 len;
    //__u32 unit;
    __u32 dstart;
    __u32 dsize;
    __s32 i;
    char realname[12];
    char *filename;

    if (!pDir)
    {
        return 0;  /* No valid pointer to a FS_DIR structure */
    }

    /* Find path on the media and return file name part of the complete path */
    dsize = fsys_fat_findpath(pDir->pPart, pDirName, &filename, &dstart);
    if (dsize == 0)
        return 0;  /* Directory not found */

    pDir->pPart->pdops.ioctl(pDir->pPart, FSYS_CMD_INC_BUSYCNT, 0, (void*)0); /* Turn on busy signal */
    len = strlen(filename);
    if (len != 0)
    {
        /* There is a name in the complete path (it does not end with a '\') */
        fsys_fat_make_realname(realname, filename);  /* Convert name to FAT real name */
        i =  fsys_fat_find_dir(pDir->pPart, realname, dstart, dsize);  /* Search name in the directory */
        if (i == 0)
        {
            /* Directory not found */
            pDir->pPart->pdops.ioctl(pDir->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
            return 0;
        }
    }
    else
    {
        /*
            There is no name in the complete path (it does end with a '\'). In that
            case, fsys_fat_findpath returns already start of the directory.
        */
        i = dstart;  /* Use 'current' path */
    }
    if (i)
    {
        dsize  =  fsys_fat_dir_size(pDir->pPart, i);  /* Get size of the directory */
    }
    if (dsize == 0)
    {
        /* Directory not found */
        pDir->pPart->pdops.ioctl(pDir->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
        return 0;
    }
    //pDir->dirid_lo  = unit;
    pDir->dirid_hi  = i;
    pDir->dirid_ex  = dstart;
    pDir->error     = 0;
    pDir->size      = dsize;
    pDir->dirpos    = 0;
    return pDir;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_closedir
*
  Description:
  FS internal function. Close a directory referred by pDir.

  Parameters:
  pDir        - Pointer to a FS_DIR data structure.

  Return value:
  ==0         - Directory has been closed.
  ==-1        - Unable to close directory.
**********************************************************************************************************************
*/

int fsys_fat_closedir(__fsys_dir_t *pDir)
{
  	if (!pDir)
  	{
    	return -1;  /* No valid pointer to a FS_DIR structure */
	}
	pDir->pPart->pdops.ioctl(pDir->pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
//	pDir->inuse = 0;
	return 0;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_readdir
*
  Description:
  FS internal function. Read next directory entry in directory
  specified by pDir.

  Parameters:
  pDir        - Pointer to a FS_DIR data structure.

  Return value:
  ==0         - No more directory entries or error.
  !=0         - Pointer to a directory entry.
**********************************************************************************************************************
*/

__fsys_dirent_t *fsys_fat_readdir(__fsys_dir_t *pDir)
{
    __fsys_fat_dentry_t *s;
    __u32 dirindex;
    __u32 dsec;
    __u16 bytespersec;
    char *buffer;
    int err;

    if (!pDir)
    {
        return 0;  /* No valid pointer to a FS_DIR structure */
    }
    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return 0;
    }
    bytespersec = d_fsys_fat_aBPBUnit->BytesPerSec;
    dirindex = pDir->dirpos / bytespersec;
    while (dirindex < (__u32)pDir->size)
    {
        dsec = fsys_fat_dir_realsec(pDir->pPart, pDir->dirid_hi, dirindex);
        if (dsec == 0)
        {
            /* Cannot convert logical sector */
            fsys_fat_free(buffer);
            return 0;
        }
        /* Read directory sector */
        err = pDir->pPart->pdops.read((void*)buffer, dsec, 1, pDir->pPart);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return 0;
        }
        /* Scan for valid directory entry */
        s = (__fsys_fat_dentry_t *)&buffer[pDir->dirpos % bytespersec];
        while (1)
        {
            if (s >= (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
            {
                break;  /* End of sector reached */
            }
            if (s->data[11] != 0x00)
            { /* not an empty entry */
                if (s->data[0] != (unsigned char)0xe5)
                { /* not a deleted file */
                    if (s->data[11] != (FSYS_FAT_ATTR_READ_ONLY | FSYS_FAT_ATTR_HIDDEN | FSYS_FAT_ATTR_SYSTEM | FSYS_FAT_VOLUME_ID))
                    {
                        break;  /* Also not a long entry, so it is a valid entry */
                    }
                }
            }
            s++;
            pDir->dirpos += 32;
        }
        if (s < (__fsys_fat_dentry_t *)(buffer + FSYS_FAT_SEC_SIZE))
        {
            /* Valid entry found, copy it.*/
            pDir->dirpos += 32;
            memcpy(pDir->dirent.d_name, s->data, 8);
            pDir->dirent.d_name[8] = '.';
            memcpy(&pDir->dirent.d_name[9], &s->data[8], 3);
            pDir->dirent.d_name[12] = 0;
            pDir->dirent.fatdirattr = s->data[11];
            fsys_fat_free(buffer);
            return &pDir->dirent;
        }
        dirindex++;
    }
    fsys_fat_free(buffer);
    return 0;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_MkRmDir
*
  Description:
  FS internal function. Create or remove a directory. If you call this
  function to remove a directory (MkDir==0), you must make sure, that
  it is already empty.

  Parameters:
  pDirName    - Directory name.
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  MkDir       - ==0 => Remove directory.
                !=0 => Create directory.

  Return value:
  ==0         - Directory has been created.
  ==-1        - An error has occured.
**********************************************************************************************************************
*/
int  fsys_fat_MkRmDir(const char *pDirName, __fsys_part_t* pPart, char MkDir)
{
    __u32       len;
    __u32       dstart;
    __u32       dsize;
    __s32       i;
    int         lexp_a;
    int         lexp_b;
    char        realname[12];
    char       *filename;

    if (pPart == 0)
    {
        return -1; /* Not a valid index */
    }

    dsize = fsys_fat_findpath(pPart, pDirName, &filename, &dstart);
    if (dsize == 0)
    {
        return -1;  /* Path not found */
    }
    pPart->pdops.ioctl(pPart, FSYS_CMD_INC_BUSYCNT, 0, (void*)0); /* Turn on busy signal */
    len = strlen(filename);
    if (len != 0)
    {
        fsys_fat_make_realname(realname, filename);  /* Convert name to FAT real name */
        i =  fsys_fat_find_dir(pPart, realname, dstart, dsize);
        lexp_a = (i!=0) && (MkDir);  /* We want to create a direcory , but it does already exist */
        lexp_b = (i==0) && (!MkDir); /* We want to remove a direcory , but it does not exist */
        lexp_a = lexp_a || lexp_b;
        if (lexp_a)
        {
            /* We want to create, but dir does already exist or we want to remove, but dir is not there */
            /* turn off busy signal */
            pPart->pdops.ioctl(pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);
            return -1;
        }
    }
    else
    {
        pPart->pdops.ioctl(pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn off busy signal */
        return -1;
    }
    /*
        When you get here, variables have following values:
        dstart="current"
        dsize="size of current"
        realname="real dir name to create"
    */
    if (MkDir)
    {
        i = _fsys_fat_create_directory(pPart, realname, dstart, dsize);  /* Create the directory */
    }
    else
    {
        i = fsys_fat_DeleteFileOrDir(pPart, realname, dstart, dsize, 0);  /* Remove the directory */
    }
    if (i >= 0)
    {
        /* If the operation has been successfull, flush the cache.*/
        i = pPart->pdops.ioctl(pPart, FSYS_PART_CMD_FLUSH_CACHE, 2, (void*)0);
        pPart->pdops.ioctl(pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn of busy signal */
        if (i < 0)
        {
            return -1;
        }
        return 0;
    }
    pPart->pdops.ioctl(pPart, FSYS_CMD_DEC_BUSYCNT, 0, (void*)0);  /* Turn of busy signal */
    return -1;
}

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
#endif /* FS_FAT_NOFAT32 */

#ifndef FSYS_DIR_MAXOPEN
    #define FSYS_DIR_MAXOPEN        0
#endif /* FS_DIR_MAXOPEN */

#define FSYS_MEMBLOCK_NUM   (FSYS_FILE_MAXOPEN+FSYS_DIR_MAXOPEN)*2


/*
**********************************************************************************************************************
*             Local data types
**********************************************************************************************************************
*/

typedef struct __FSYS_FAT_BLK
{
    int status;
    char memory[FSYS_FAT_SEC_SIZE];
} __fsys_fat_blk_t;

typedef struct __fsys_fat_BPB_BLK
{
    int status;
    char memory[sizeof(__fsys_fat_BPB_t)];
}__fsys_fat_BPB_BLK_t;

/*
**********************************************************************************************************************
*             Local Variables
**********************************************************************************************************************
*/

static __fsys_fat_blk_t     _fsys_fat_memblk[FSYS_MEMBLOCK_NUM];
static __fsys_fat_BPB_BLK_t  _fsys_fat_bpb_memblk[MAX_PARTS_NUM];

/*
**********************************************************************************************************************
*             Local functions section
**********************************************************************************************************************
*/




/*
**********************************************************************************************************************
*             _fsys_fat_FindFreeCluster
*
  Description:
  FS internal function. Find the next free entry in the FAT.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  pFATSector  - Returns the sector number of the free entry.
  pLastSector - Returns the sector number of the sector in pBuffer.
  pFATOffset  - Returns the offset of the free FAT entry within the
                sector pFATSector.
  LastClust   - Cluster, which will be used to link the new allocated
                cluster to. Here it is used at hint for where to start
                in the FAT.
  pBuffer     - Pointer to a sector buffer.
  FSysType    - ==1 => FAT12
                ==0 => FAT16
                ==2 => FAT32
  FATSize     - Size of one FAT ind sectors.
  BytesPerSec - Number of bytes in each sector.

  Return value:
  >=0         - Number of the free cluster.
  <0          - An error has occured.
**********************************************************************************************************************
*/

static __s32 _fsys_fat_FindFreeCluster(__fsys_part_t    *pPart,
                                       __s32            *pFATSector,
                                       __s32            *pLastSector,
                                       __s32            *pFATOffset,
                                       __s32             LastClust,
                                       unsigned char    *pBuffer,
                                       int               FSysType,
                                       __u32             FATSize,
                                       __s32             BytesPerSec)
{
    __u32 totclst;
    __u32 rootdirsize;
    __s32 curclst;
    __s32 fatindex;
    int err;
    int scan;
    unsigned char fatentry;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif

    if (LastClust > 0)
    {
        curclst = LastClust + 1;  /* Start scan after the previous allocated sector */
    }
    else
    {
        curclst = 0;  /*  Start scan at the beginning of the media */
    }
    scan          = 0;
    *pFATSector   = 0;
    *pLastSector  = -1;
    fatentry      = 0xff;
    /* Calculate total number of data clusters on the media */
    totclst = fsys_fat_aBPBUnit->TotSec16;
    if (totclst == 0)
    {
        totclst = fsys_fat_aBPBUnit->TotSec32;
    }
    rootdirsize = ((__u32)((__u32)fsys_fat_aBPBUnit->RootEntCnt) * FSYS_FAT_DENTRY_SIZE) / BytesPerSec;
    totclst     = totclst - (fsys_fat_aBPBUnit->RsvdSecCnt + fsys_fat_aBPBUnit->NumFATs * FATSize + rootdirsize);
    totclst    /= fsys_fat_aBPBUnit->SecPerClus;
    while (1)
    {
        if (curclst >= (__s32)totclst)
        {
            scan++;
            if (scan > 1)
            {
                break;  /* End of clusters reached after 2nd scan */
            }
            if (LastClust <= 0)
            {
                break;  /* 1st scan started already at zero */
            }
            curclst   = 0;  /* Try again starting at the beginning of the FAT */
            fatentry  = 0xff;
        }
    if (fatentry == 0)
    {
        break;  /* Free entry found */
    }
    if (FSysType == 1)
    {
        fatindex = curclst + (curclst / 2);    /* FAT12 */
    }
    else if (FSysType == 2)
    {
        fatindex = curclst * 4;               /* FAT32 */
    }
    else
    {
        fatindex = curclst * 2;               /* FAT16 */
    }
    *pFATSector = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / BytesPerSec);
    *pFATOffset = fatindex % BytesPerSec;
    if (*pFATSector != *pLastSector)
    {
        err = pPart->pdops.read((void*)pBuffer, *pFATSector, 1, pPart);
        if (err < 0)
        {
            err = pPart->pdops.read((void*)pBuffer, FATSize + *pFATSector, 1, pPart);
            if (err < 0)
            {
                return -1;
            }
            /* Try to repair original FAT sector with contents of copy */
            pPart->pdops.write((void*)pBuffer, *pFATSector, 1, pPart);
        }
        *pLastSector = *pFATSector;
    }
    if (FSysType == 1)
    {
        if (*pFATOffset == (BytesPerSec - 1))
        {
            a = pBuffer[*pFATOffset];
            err = pPart->pdops.read( (void*)pBuffer, *pFATSector + 1, 1, pPart);
            if (err < 0)
            {
          err = pPart->pdops.read((void*)pBuffer, FATSize + *pFATSector + 1, 1, pPart);
          if (err < 0) {
            return -1;
          }
          /* Try to repair original FAT sector with contents of copy */
          pPart->pdops.write((void*)pBuffer, *pFATSector + 1, 1, pPart);
        }
        *pLastSector = *pFATSector + 1;
        b = pBuffer[0];
      }
      else {
        a = pBuffer[*pFATOffset];
        b = pBuffer[*pFATOffset + 1];
      }
      if (curclst & 1) {
        fatentry = ((a & 0xf0) >> 4 ) | b;
      }
      else {
        fatentry = a | (b & 0x0f);
      }
    }
#if (FSYS_FAT_NOFAT32==0)
    else if (FSysType == 2) {
      a = pBuffer[*pFATOffset];
      b = pBuffer[*pFATOffset + 1];
      c = pBuffer[*pFATOffset + 2];
      d = pBuffer[*pFATOffset + 3];
      fatentry = a | b | c | d;
    }
#endif /* FS_FAT_NOFAT32==0 */
    else {
      a = pBuffer[*pFATOffset];
      b = pBuffer[*pFATOffset + 1];
      fatentry = a | b;
    }
    if (fatentry != 0) {
      curclst++;  /* Cluster is in use or defect, so try the next one */
    }
  }
  if (fatentry == 0) {
    return curclst;  /* Free cluster found */
  }
  return -1;
}


/*
**********************************************************************************************************************
*             _fsys_fat_SetEOFMark
*
  Description:
  FS internal function. Set the EOF mark in the FAT for a cluster.
  The function does not write the FAT sector. An exception is FAT12,
  if the FAT entry is in two sectors.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  FATSector   - FAT sector, where the cluster is located.
  pLastSector - Pointer to an __s32, which contains the number of the
                sector in pBuffer.
  FATOffset   - Offset of the cluster in the FAT sector.
  Cluster     - Cluster number, where to set the EOF mark.
  pBuffer     - Pointer to a sector buffer.
  FSysType    - ==1 => FAT12
                ==0 => FAT16
                ==2 => FAT32
  FATSize     - Size of one FAT ind sectors.
  BytesPerSec - Number of bytes in each sector.

  Return value:
  >=0         - EOF mark set.
  <0          - An error has occured.
**********************************************************************************************************************
*/

static int _fsys_fat_SetEOFMark(__fsys_part_t  *pPart,
                                __s32           FATSector,
                                __s32          *pLastSector,
                                __s32           FATOffset,
                                __s32           Cluster,
                                unsigned char  *pBuffer,
                                int             FSysType,
                                __u32           FATSize,
                                __s32           BytesPerSec)
{
    int     err1;
    int     err2;
    int     lexp;

    if (FSysType == 1)
    {
        if (FATOffset == (BytesPerSec - 1))
        {
            /* Entry in 2 sectors (we have 2nd sector in buffer) */
            if (Cluster & 1)
            {
                pBuffer[0]  = (char)0xff;
            }
            else
            {
                pBuffer[0] |= (char)0x0f;
            }
            err1 = pPart->pdops.write((void*)pBuffer, FATSector + 1, 1, pPart);
            err2 = pPart->pdops.write((void*)pBuffer, FATSize + FATSector + 1, 1, pPart);
            lexp = (err1 < 0);
            lexp = lexp || (err2 < 0);
            if (lexp)
            {
                return -1;
            }
            err1 = pPart->pdops.read((void*)pBuffer, FATSector, 1, pPart);
            if (err1 < 0)
            {
                err1 = pPart->pdops.read((void*)pBuffer, FATSize + FATSector, 1, pPart);
                if (err1 < 0)
                {
                    return -1;
                }
                /* Try to repair original FAT sector with contents of copy */
                pPart->pdops.write((void*)pBuffer, FATSector, 1, pPart);
            }
            *pLastSector = FATSector;
            if (Cluster & 1)
            {
                pBuffer[FATOffset] |= (char)0xf0;
            }
            else
            {
                pBuffer[FATOffset]  = (char)0xff;
            }
        }
        else
        {
        if(Cluster & 1)
        {
            pBuffer[FATOffset]   |= (char)0xf0;
            pBuffer[FATOffset+1]  = (char)0xff;
        }
        else
        {
            pBuffer[FATOffset]    = (char)0xff;
            pBuffer[FATOffset+1] |= (char)0x0f;
        }
    }
}
#if (FSYS_FAT_NOFAT32==0)
    else if (FSysType == 2)
    { /* FAT32 */
        pBuffer[FATOffset]      = (char)0xff;
        pBuffer[FATOffset + 1]  = (char)0xff;
        pBuffer[FATOffset + 2]  = (char)0xff;
        pBuffer[FATOffset + 3]  = (char)0x0f;
    }
#endif /* FS_FAT_NOFAT32==0 */
    else
    { /* FAT16 */
        pBuffer[FATOffset]      = (char)0xff;
        pBuffer[FATOffset + 1]  = (char)0xff;
    }
    return 0;
}


/*
**********************************************************************************************************************
*             _fsys_fat_LinkCluster
*
  Description:
  FS internal function. Link the new cluster with the EOF mark to the
  cluster list.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  pLastSector - Pointer to an __s32, which contains the number of the
                sector in pBuffer.
  Cluster     - Cluster number of the new cluster with the EOF mark.
  LastClust   - Number of cluster, to which the new allocated cluster
                is linked to.
  pBuffer     - Pointer to a sector buffer.
  FSysType    - ==1 => FAT12
                ==0 => FAT16
                ==2 => FAT32
  FATSize     - Size of one FAT ind sectors.
  BytesPerSec - Number of bytes in each sector.

  Return value:
  >=0         - Link has been made.
  <0          - An error has occured.
**********************************************************************************************************************
*/

static int _fsys_fat_LinkCluster(__fsys_part_t     *pPart,
                                 __s32             *pLastSector,
                                 __s32              Cluster,
                                 __s32              LastClust,
                                 unsigned char     *pBuffer,
                                 int                FSysType,
                                 __u32              FATSize,
                                 __s32              BytesPerSec)
{
    __s32 		fatindex;
    __s32 		fatoffs;
    __s32 		fatsec;
    int 		lexp;
    int 		err;
    int 		err2;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif

    /* Link old last cluster to this one */
    if (FSysType == 1)
    {
        fatindex = LastClust + (LastClust / 2); /* FAT12 */
    }
    else if (FSysType == 2)
    {
        fatindex = LastClust * 4;               /* FAT32 */
    }
    else
    {
        fatindex = LastClust * 2;               /* FAT16 */
    }
    fatsec = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / BytesPerSec);
    fatoffs = fatindex % BytesPerSec;
    if (fatsec != *pLastSector)
    {
        /*
        FAT entry, which has to be modified is not in the same FAT sector, which is
        currently in the buffer. So write it to the media now.
        */
        err  = pPart->pdops.write((void*)pBuffer, *pLastSector, 1, pPart);
        err2 = pPart->pdops.write((void*)pBuffer, *pLastSector + FATSize, 1, pPart);
        lexp = (err < 0);
        lexp = lexp || (err2 < 0);
        if (lexp)
        {
            return -1;
        }
        err = pPart->pdops.read((void*)pBuffer, fatsec, 1,pPart);
        if (err < 0)
        {
            err = pPart->pdops.read((void*)pBuffer, FATSize + fatsec, 1, pPart);
            if (err<0)
            {
                return -1;
            }
            /* Try to repair original FAT sector with contents of copy */
            pPart->pdops.write((void*)pBuffer, fatsec, 1, pPart);
        }
        *pLastSector = fatsec;
    }
    a = Cluster & 0xff;
    b = (Cluster / 0x100L) & 0xff;
#if (FSYS_FAT_NOFAT32==0)
    c = (Cluster / 0x10000L) & 0xff;
    d = (Cluster / 0x1000000L) & 0x0f;
#endif
    if (FSysType == 1)
    {
        if (fatoffs == (BytesPerSec - 1))
        {
            /* Entry in 2 sectors (we have 2nd sector in buffer) */
            if (LastClust & 1)
            {
                pBuffer[fatoffs]   &= (char)0x0f;
                pBuffer[fatoffs]   |= (char)((a << 4) & 0xf0);
            }
            else
            {
                pBuffer[fatoffs]    = (char)(a & 0xff);
            }
            err  = pPart->pdops.write((void*)pBuffer, fatsec, 1, pPart);
            err2 = pPart->pdops.write((void*)pBuffer, FATSize + fatsec, 1, pPart);
            lexp = (err < 0);
            lexp = lexp || (err2 < 0);
            if (lexp)
            {
                return -1;
            }
            err = pPart->pdops.read((void*)pBuffer, fatsec + 1, 1, pPart);
            if (err < 0)
            {
                return -1;
            }
            *pLastSector = fatsec + 1;
            if (LastClust & 1)
            {
                pBuffer[0]  = (char)(((a >> 4) & 0x0f) | ((b << 4) & 0xf0));
            }
            else
            {
                pBuffer[0] &= (char)0xf0;
                pBuffer[0] |= (char)(b & 0x0f);
            }
            err  = pPart->pdops.write((void*)pBuffer, fatsec + 1, 1, pPart);
            err2 = pPart->pdops.write((void*)pBuffer, FATSize + fatsec + 1, 1, pPart);
            lexp = (err < 0);
            lexp = lexp || (err2 < 0);
            if (lexp)
            {
                return -1;
            }
        }
        else
        {
            if (LastClust & 1)
            {
                pBuffer[fatoffs]     &= (char)0x0f;
                pBuffer[fatoffs]     |= (char)((a << 4) & 0xf0);
                pBuffer[fatoffs + 1]  = (char)(((a >> 4) & 0x0f) | ((b << 4) & 0xf0));
            }
            else
            {
                pBuffer[fatoffs]      = (char)(a & 0xff);
                pBuffer[fatoffs + 1] &= (char)0xf0;
                pBuffer[fatoffs + 1] |= (char)(b & 0x0f);
            }
            err  = pPart->pdops.write((void*)pBuffer, fatsec, 1, pPart);
            err2 = pPart->pdops.write((void*)pBuffer, FATSize + fatsec, 1, pPart);
            lexp = (err < 0);
            lexp = lexp || (err2 < 0);
            if (lexp)
            {
                return -1;
            }
        }
    }
#if (FSYS_FAT_NOFAT32==0)
    else if (FSysType == 2)
    { /* FAT32 */
        pBuffer[fatoffs]      = a;
        pBuffer[fatoffs + 1]  = b;
        pBuffer[fatoffs + 2]  = c;
        pBuffer[fatoffs + 3]  = d;
        err  = pPart->pdops.write((void*)pBuffer, fatsec, 1, pPart);
        err2 = pPart->pdops.write((void*)pBuffer, FATSize + fatsec, 1, pPart);
        lexp = (err < 0) ;
        lexp = lexp || (err2 < 0);
        if (lexp)
        {
            return -1;
        }
    }
#endif /* FS_FAT_NOFAT32==0 */
    else
    { /* FAT16 */
        pBuffer[fatoffs]      = a;
        pBuffer[fatoffs + 1]  = b;
        err  = pPart->pdops.write((void*)pBuffer, fatsec, 1, pPart);
        err2 = pPart->pdops.write((void*)pBuffer, FATSize + fatsec, 1, pPart);
        lexp = (err < 0);
        lexp = lexp || (err2 < 0);
        if (lexp)
        {
            return -1;
        }
    }
    return 0;
}


/*
**********************************************************************************************************************
*             Global functions section
**********************************************************************************************************************

  Functions in this section are used by FAT File System layer only

*/

/*
**********************************************************************************************************************
*
*             fsys_fat_block_init
*
  Description:
  FS internal function. Init FAT block memory management.

  Parameters:
  None.

  Return value:
  None.
**********************************************************************************************************************
*/

void fsys_fat_block_init(void)
{
  	int i;

  	for (i = 0; i < FSYS_MEMBLOCK_NUM; i++)
	{
		_fsys_fat_memblk[i].status = 0;
  	}
}


/*
**********************************************************************************************************************
*
*             fsys_fat_malloc
*
  Description:
  FS internal function. Allocate a sector buffer.

  Parameters:
  Size        - Size of the sector buffer. Normally this is 512.
                Parameter is for future extension.

  Return value:
  ==0         - Cannot allocate a buffer.
  !=0         - Address of a buffer.
**********************************************************************************************************************
*/


char *fsys_fat_malloc(unsigned int Size)
{
    int i;

    if (Size <= FSYS_FAT_SEC_SIZE)
    {
        for (i = 0; i < FSYS_MEMBLOCK_NUM; i++)
        {
            if (_fsys_fat_memblk[i].status == 0)
            {
                _fsys_fat_memblk[i].status = 1;
                memset((void*)_fsys_fat_memblk[i].memory, 0, (__u32)FSYS_FAT_SEC_SIZE);
                return ((void*)_fsys_fat_memblk[i].memory);
            }
        }
    }
    return 0;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_free
*
  Description:
  FS internal function. Free sector buffer.

  Parameters:
  pBuffer     - Pointer to a buffer, which has to be set free.

  Return value:
  None.
**********************************************************************************************************************
*/

void fsys_fat_free(void *pBuffer)
{
    int i;

    for (i = 0; i < FSYS_MEMBLOCK_NUM; i++)
    {
        if (((void*)_fsys_fat_memblk[i].memory) == pBuffer)
        {
            _fsys_fat_memblk[i].status = 0;
            return;
        }
    }
}

/*
**********************************************************************************************************************
*
*             fsys_fat_bpb_block_init
*
  Description:
  FS internal function. Init FAT block memory management.

  Parameters:
  None.

  Return value:
  None.
**********************************************************************************************************************
*/

void fsys_fat_bpb_block_init(void)
{
  	int i;

  	for (i = 0; i < MAX_PARTS_NUM; i++)
	{
		_fsys_fat_bpb_memblk[i].status = 0;
  	}
}


/*
**********************************************************************************************************************
*
*             fsys_fat_bpb_malloc
*
  Description:
  FS internal function. Allocate a sector buffer.

  Parameters:
  Size        - Size of the sector buffer. Normally this is 512.
                Parameter is for future extension.

  Return value:
  ==0         - Cannot allocate a buffer.
  !=0         - Address of a buffer.
**********************************************************************************************************************
*/


char *fsys_fat_bpb_malloc(void)
{
    int i;

    for (i = 0; i < MAX_PARTS_NUM; i++)
    {
        if (_fsys_fat_bpb_memblk[i].status == 0)
        {
            _fsys_fat_bpb_memblk[i].status = 1;
            memset((void*)_fsys_fat_bpb_memblk[i].memory, 0, sizeof(__fsys_fat_BPB_BLK_t));
            return ((void*)_fsys_fat_bpb_memblk[i].memory);
        }
    }
    return 0;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_bpb_free
*
  Description:
  FS internal function. Free sector buffer.

  Parameters:
  pBuffer     - Pointer to a buffer, which has to be set free.

  Return value:
  None.
**********************************************************************************************************************
*/

void fsys_fat_bpb_free(void *pBuffer)
{
    int i;

    for (i = 0; i < MAX_PARTS_NUM; i++)
    {
        if (((void*)_fsys_fat_bpb_memblk[i].memory) == pBuffer)
        {
            _fsys_fat_bpb_memblk[i].status = 0;
            return;
        }
    }
}

/*
**********************************************************************************************************************
*
*             fsys_fat_checkunit
*
  Description:
  FS internal function. Read Bios-Parameter-Block from a device and
  check, if it contains valid data.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.

  Return value:
  ==1         - BPB is okay.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

int fsys_fat_checkunit(__fsys_part_t * pPart)
{
    int err;
    int status;
    int lexp;

    status = pPart->pdops.ioctl(pPart, FSYS_PART_CMD_GET_STATUS, 0, 0);
    if (status < 0)
    {
        return 0;
    }
    if (status == FSYS_PART_MEDIACHANGED)
    {
        /* Mount new volume */
        err = fsys_fat_ReadBPB(pPart);
        if (err < 0)
        {
            return 0;
        }
    }
    if (fsys_fat_aBPBUnit->Signature != 0xaa55)
    {
        err = fsys_fat_ReadBPB(pPart);
        lexp = (err < 0);
        lexp = lexp || (fsys_fat_aBPBUnit->Signature != 0xaa55);
        if (lexp)
        {
            return 0;
        }
    }
    if ( fsys_fat_aBPBUnit->NumFATs != 2)
    {
        return 0;  /* Only 2 FATs are supported */
    }
    if (fsys_fat_aBPBUnit->FATSz16 == 0)
    {
        if (fsys_fat_aBPBUnit->ExtFlags & 0x0080)
        {
            return 0;  /* Only mirroring at runtime supported */
        }
    }
    return 1;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_which_type
*
  Description:
  FS internal function. Determine FAT type used on a media. This
  function is following the MS specification very closely.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.

  Return value:
  ==0         - FAT16.
  ==1         - FAT12.
  ==2         - FAT32
**********************************************************************************************************************
*/

int fsys_fat_which_type(__fsys_part_t *pPart)
{
//    __u32 coc;
//    __u32 fatsize;
//    __u32 totsec;
//    __u32 datasec;
//    __u32 bytespersec;
//    __u32 dsize;
//    __u32 fat1216;
//
//    bytespersec   = (__u32)fsys_fat_aBPBUnit->BytesPerSec;
//    dsize         = ((__u32)((__u32)fsys_fat_aBPBUnit->RootEntCnt) * FSYS_FAT_DENTRY_SIZE) / bytespersec;
//    fatsize       = fsys_fat_aBPBUnit->FATSz16;
//    fat1216       = 0;
//    if (fatsize == 0)
//    {
//    	fat1216 = 1;
//        fatsize = fsys_fat_aBPBUnit->FATSz32;
//    }
//    totsec = (__u32)fsys_fat_aBPBUnit->TotSec16;
//    if (totsec == 0)
//    {
//    	fat1216 = 1;
//        totsec = fsys_fat_aBPBUnit->TotSec32;
//    }
//    datasec = totsec - (fsys_fat_aBPBUnit->RsvdSecCnt +
//                          fsys_fat_aBPBUnit->NumFATs * fatsize + dsize);
//    coc     = datasec / fsys_fat_aBPBUnit->SecPerClus;
//    if(fat1216)
//    {
//    	return 2; /* FAT32 */
//    }
//    else
//    {
//    	if (coc < 4085)
//	    {
//	        return 1;  /* FAT12 */
//	    }
//	    else  // if (coc < 65525)
//	    {
//	        return 0;  /* FAT16 */
//	    }
//    }
	return fsys_fat_aBPBUnit->fattype;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_FAT_find_eof
*
  Description:
  FS internal function. Find the next EOF mark in the FAT.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  StrtClst    - Starting cluster in FAT.
  pClstCnt    - If not zero, this is a pointer to an __u32, which
                is used to return the number of clusters found
                between StrtClst and the next EOF mark.

  Return value:
  >=0         - Cluster, which contains the EOF mark.
  <0          - An error has occured.
**********************************************************************************************************************
*/

__s32 fsys_fat_FAT_find_eof(__fsys_part_t *pPart, __s32 StrtClst, __u32 *pClstCnt)
{
    __u32   clstcount;
    __u32   fatsize;
    __u32   maxclst;
    __s32   fatindex;
    __s32   fatsec;
    __s32   fatoffs;
    __s32   lastsec;
    __s32   curclst;
    __s32   bytespersec;
    __s32   eofclst;
    int     fattype;
    int     err;
    char    *buffer;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif /* FS_FAT_NOFAT32==0 */

    fattype = fsys_fat_which_type(pPart);
    if (fattype == 1)
    {
        maxclst = 4085UL;       /* FAT12 */
    }
    else if (fattype == 2)
    {
#if (FSYS_FAT_NOFAT32 == 0)
        maxclst = 0x0ffffff0UL; /* FAT32 */
#else
        return -1;
#endif /* (FS_FAT_NOFAT32==0) */
    }
    else
    {
        maxclst = 65525UL;      /* FAT16 */
    }
    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    fatsize = fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        fatsize = fsys_fat_aBPBUnit->FATSz32;
    }
    bytespersec   = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
    curclst       = StrtClst;
    lastsec       = -1;
    clstcount     = 0;
    while (clstcount < maxclst)
    {
        eofclst = curclst;
        clstcount++;
        if (fattype == 1)
        {
            fatindex = curclst + (curclst / 2);   /* FAT12 */
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
            err = pPart->pdops.read((void*)buffer, fatsec, 1, pPart);
            if (err < 0)
            {
                err = pPart->pdops.read((void*)buffer, fatsize + fatsec, 1, pPart);
                if (err < 0)
                {
                    fsys_fat_free(buffer);
                    return -1;
                }
                /* Try to repair original FAT sector with contents of copy */
                pPart->pdops.write((void*)buffer, fatsec, 1, pPart);
            }
            lastsec = fatsec;
        }
        if (fattype == 1)
        {
            if (fatoffs == (bytespersec - 1))
            {
                a   = buffer[fatoffs];
                err = pPart->pdops.read((void*)buffer, fatsec + 1, 1, pPart);
                if (err < 0)
                {
                    err = pPart->pdops.read((void*)buffer, fatsize + fatsec + 1, 1, pPart);
                    if (err < 0)
                    {
                        fsys_fat_free(buffer);
                        return -1;
                    }
                    /* Try to repair original FAT sector with contents of copy */
                    pPart->pdops.write((void*)buffer, fatsec + 1, 1, pPart);

                }
                lastsec = fatsec + 1;
                b       = buffer[0];
            }
            else
            {
                a = buffer[fatoffs];
                b = buffer[fatoffs + 1];
            }
            if (curclst & 1)
            {
                curclst = ((a & 0xf0) >> 4 ) + 16 * b;
            }
            else
            {
                curclst = a + 256 * (b & 0x0f);
            }
            curclst &= 0x0fffL;
            if (curclst >= 0x0ff8L)
            {
                /* EOF found */
                fsys_fat_free(buffer);
                if (pClstCnt)
                {
                    *pClstCnt = clstcount;
                }
                return eofclst;
            }
        }
#if (FSYS_FAT_NOFAT32==0)
        else if (fattype == 2)
        {
            a         = buffer[fatoffs];
            b         = buffer[fatoffs + 1];
            c         = buffer[fatoffs + 2];
            d         = buffer[fatoffs + 3];
            curclst   = a + 0x100L * b + 0x10000L * c + 0x1000000L * d;
            curclst  &= 0x0fffffffL;
            if (curclst >= (__s32)0x0ffffff8L)
            {
                /* EOF found */
                fsys_fat_free(buffer);
                if (pClstCnt)
                {
                    *pClstCnt = clstcount;
                }
                return eofclst;
            }
        }
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            a         = buffer[fatoffs];
            b         = buffer[fatoffs + 1];
            curclst   = a + 256 * b;
            curclst  &= 0xffffL;
            if (curclst >= (__s32)0xfff8L)
            {
                /* EOF found */
                fsys_fat_free(buffer);
                if (pClstCnt)
                {
                    *pClstCnt = clstcount;
                }
                return eofclst;
            }
        }
    } /* while (clstcount<maxclst) */
    fsys_fat_free(buffer);
    return -1;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_FAT_alloc
*
  Description:
  FS internal function. Allocate a new cluster in the FAT and link it
  to LastClust. Assign an EOF mark to the new allocated cluster.
  The function has grown a lot, since it supports all FAT types (FAT12,
  FAT16 & FAT32). There is also room for performance improvement, when
  makeing the new FAT entry and the old entry is within the same FAT
  sector.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  LastClust   - Number of cluster, to which the new allocated cluster
                is linked to. If this is negative, the new cluster is
                not linked to anything and only the EOF mark is set.

  Return value:
  >=0         - Number of new allocated cluster, which contains the
                EOF mark.
  <0          - An error has occured.
**********************************************************************************************************************
*/

__s32 fsys_fat_FAT_alloc(__fsys_part_t *pPart, __s32 LastClust)
{
    __u32           fatsize;
    __s32           fatoffs;
    __s32           bytespersec;
    __s32           curclst;
    __s32           fatsec;
    __s32           lastsec;
    unsigned char  *buffer;
    int             fattype;
    int             err;
    int             err2;
    int             lexp;

    buffer = (unsigned char*)fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    fattype = fsys_fat_which_type(pPart);
#if (FSYS_FAT_NOFAT32!=0)
    if (fattype == 2)
    {
        fsys_fat_free(buffer);
        return -1;
    }
#endif /* FSYS_FAT_NOFAT32!=0 */
    fatsize = fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        fatsize = fsys_fat_aBPBUnit->FATSz32;
    }
    bytespersec   = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
    /* Find a free cluster in the FAT */
    curclst       = _fsys_fat_FindFreeCluster(pPart, &fatsec, &lastsec, &fatoffs, LastClust, buffer, fattype, fatsize, bytespersec);
    if (curclst < 0)
    {
        fsys_fat_free(buffer);   /* No free cluster found. */
        return -1;
    }
    /* Make an EOF entry for the new cluster */
    err = _fsys_fat_SetEOFMark(pPart, fatsec, &lastsec, fatoffs, curclst, buffer, fattype, fatsize, bytespersec);
    if (err < 0)
    {
        fsys_fat_free(buffer);
        return -1;
    }
    /* Link the new cluster to the cluster list */
    if (LastClust < 0)
    {
        err  = pPart->pdops.write((void*)buffer, lastsec, 1, pPart);
        err2 = pPart->pdops.write((void*)buffer, fatsize + lastsec, 1, pPart);
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
        err = _fsys_fat_LinkCluster(pPart, &lastsec, curclst, LastClust, buffer, fattype, fatsize, bytespersec);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
    }

#if (FSYS_FAT_NOFAT32==0)
    /* Update the FSInfo structure */
    if (fattype == 2)   /* FAT32 */
    {
        /* Modify FSInfo */
        err = pPart->pdops.read((void*)buffer, fsys_fat_aBPBUnit->FSInfo, 1, pPart);
        if (err < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
        /* Check for FSInfo structure in buffer */
        if (buffer[0] == (char)0x52) {
          if (buffer[1] == (char)0x52) {
            if (buffer[2] == (char)0x61) {
              if (buffer[3] == (char)0x41) {
                if (buffer[484] == (char)0x72) {
                  if (buffer[485] == (char)0x72) {
                    if (buffer[486] == (char)0x41) {
                      if (buffer[487] == (char)0x61) {
                        if (buffer[508] == (char)0x00) {
                          if (buffer[509] == (char)0x00) {
                            if (buffer[510] == (char)0x55) {
                              if (buffer[511] == (char)0xaa) {
                                /* Invalidate last known free cluster count */
                                buffer[488] = (char)0xff;
                                buffer[489] = (char)0xff;
                                buffer[490] = (char)0xff;
                                buffer[491] = (char)0xff;
                                /* Give hint for free cluster search */
                                buffer[492] = curclst & 0xff;
                                buffer[493] = (curclst / 0x100L) & 0xff;
                                buffer[494] = (curclst / 0x10000L) & 0xff;
                                buffer[495] = (curclst / 0x1000000L) & 0x0f;
                                err  = pPart->pdops.write((void*)buffer, fsys_fat_aBPBUnit->FSInfo, 1, pPart);
                                if (err < 0)
                                {
                                    fsys_fat_free(buffer);
                                    return -1;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        } /* buffer contains FSInfo structure */
    } /* FS_Info modification for fattype==2 */
#endif /* FS_FAT_NOFAT32==0 */
    fsys_fat_free(buffer);
    return curclst;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_diskclust
*
  Description:
  FS internal function. Walk through the FAT starting at StrtClst for
  ClstNum times. Return the found cluster number of the media. This is
  very similar to fsys_fat_FAT_find_eof.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  StrtClst    - Starting point for FAT walk.
  ClstNum     - Number of steps.

  Return value:
  > 0         - Number of cluster found after ClstNum steps.
  ==0         - An error has occured.
**********************************************************************************************************************
*/

__s32 fsys_fat_diskclust(__fsys_part_t *pPart, __s32 StrtClst, __s32 ClstNum)
{
    __u32   fatsize;
    __s32   fatindex;
    __s32   fatsec;
    __s32   fatoffs;
    __s32   lastsec;
    __s32   curclst;
    __s32   todo;
    __s32   bytespersec;
    int     err;
    int     fattype;
    char   *buffer;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif /* FS_FAT_NOFAT32==0 */

    fattype = fsys_fat_which_type(pPart);
#if (FSYS_FAT_NOFAT32!=0)
    if (fattype == 2)
    {
        return 0;
    }
#endif  /* FS_FAT_NOFAT32!=0 */
    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer) {
      return 0;
    }
    fatsize = fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        fatsize = fsys_fat_aBPBUnit->FATSz32;
    }
    bytespersec = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
    todo        = ClstNum;
    curclst     = StrtClst;
    lastsec     = -1;
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
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            fatindex = curclst * 2;               /* FAT16 */
        }
        fatsec  = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / bytespersec);
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
                    return 0;
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
                err = pPart->pdops.read((void *)buffer, fatsec + 1, 1, pPart);
                if (err < 0)
                {
                    err = pPart->pdops.read((void *)buffer, fatsize + fatsec + 1, 1, pPart);
                    if (err < 0)
                    {
                        fsys_fat_free(buffer);
                        return 0;
                    }
                    /* Try to repair original FAT sector with contents of copy */
                    pPart->pdops.write((void *)buffer, fatsec + 1, 1, pPart);
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
                fsys_fat_free(buffer);
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
                fsys_fat_free(buffer);
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
                fsys_fat_free(buffer);
                return 0;
            }
        }
        todo--;
    }
    fsys_fat_free(buffer);
    return curclst;
}

__s32 fsys_fat_diskclust0(__fsys_part_t *pPart, __s32 StrtClst, __s32 ClstNum)
{
    __u32   fatsize;
    __s32   fatindex;
    __s32   fatsec;
    __s32   fatoffs;
    __s32   lastsec;
    __s32   curclst;
    __s32   todo;
    __s32   bytespersec;
    int     err;
    int     fattype;
    char   *buffer;
    unsigned char a;
    unsigned char b;
#if (FSYS_FAT_NOFAT32==0)
    unsigned char c;
    unsigned char d;
#endif /* FS_FAT_NOFAT32==0 */

    fattype = fsys_fat_which_type(pPart);
#if (FSYS_FAT_NOFAT32!=0)
    if (fattype == 2)
    {
        return 0;
    }
#endif  /* FS_FAT_NOFAT32!=0 */
    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer) {
      return 0;
    }
    fatsize = fsys_fat_aBPBUnit->FATSz16;
    if (fatsize == 0)
    {
        fatsize = fsys_fat_aBPBUnit->FATSz32;
    }
    bytespersec = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
    todo        = ClstNum;
    curclst     = StrtClst;
    lastsec     = -1;
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
#endif /* FS_FAT_NOFAT32==0 */
        else
        {
            fatindex = curclst * 2;               /* FAT16 */
        }
        fatsec  = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / bytespersec);
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
                    return 0;
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
                err = pPart->pdops.read((void *)buffer, fatsec + 1, 1, pPart);
                if (err < 0)
                {
                    err = pPart->pdops.read((void *)buffer, fatsize + fatsec + 1, 1, pPart);
                    if (err < 0)
                    {
                        fsys_fat_free(buffer);
                        return 0;
                    }
                    /* Try to repair original FAT sector with contents of copy */
                    pPart->pdops.write((void *)buffer, fatsec + 1, 1, pPart);
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
                fsys_fat_free(buffer);
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
                fsys_fat_free(buffer);
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
            	if (curclst == 0xffffL)
            	{
            		return StrtClst + (ClstNum - todo) + 1;
            	}
                fsys_fat_free(buffer);
                return 0;
            }
        }
        todo--;
    }
    fsys_fat_free(buffer);
    return curclst;
}

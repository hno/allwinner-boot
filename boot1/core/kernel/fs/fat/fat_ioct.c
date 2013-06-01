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
#define FSYS_KNOWNMEDIA_NUM    sizeof(_fsys_wd_format_media_table) / sizeof(__fsys_wd_format_media_t)

#ifndef FSYS_FAT_NOFORMAT
    #define FSYS_FAT_NOFORMAT       0
#endif

#ifndef FSYS_FAT_DISKINFO
    #define FS_FAT_DISKINFO       1
#endif

#ifndef FSYS_SUPPORT_SEC_ACCESS
    #define FSYS_SUPPORT_SEC_ACCESS 1
#endif
/*
**********************************************************************************************************************
*             Local data types
**********************************************************************************************************************
*/

#if (FSYS_FAT_NOFORMAT==0)

typedef struct {
    __s32   media_id;
    __u32   totsec32;
    __u32   hiddsec;
    __u16   totsec16;
    __u16   rootentcnt;
    __u16   fatsz16;
    __u16   secpertrk;
    __u16   numheads;
    char    secperclus;
    char    media;
    char    fsystype;
} __fsys_wd_format_media_t;


typedef struct
{
    __u32   SecNum;
    __u32   Num;
} _FS_FAT_ROOTENTCNT ;


typedef struct
{
    __u32 SecNum;
    __u16 Num;
} _FS_FAT_SECPERCLUST;


/*
**********************************************************************************************************************
*             Local Variables
**********************************************************************************************************************
*/

///*                totsec32       hidsec    totsec16   rootent  fatsz16  secpertrk  numheads secperclus  media   fstype */
//static const __fsys_wd_format_media_t _fsys_wd_format_media_table[] =
//{
//    { FSYS_MEDIA_RAM_16KB,  0x00000000UL, 0x00000000UL, 0x0020,   0x0040,  0x0001,  0x0004,     0x0004,   0x01, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_64KB,  0x00000000UL, 0x00000000UL, 0x0080,   0x0040,  0x0001,  0x0004,     0x0004,   0x01, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_128KB, 0x00000000UL, 0x00000000UL, 0x0100,   0x0080,  0x0001,  0x0004,     0x0004,   0x01, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_256KB, 0x00000000UL, 0x00000000UL, 0x0200,   0x0080,  0x0002,  0x0004,     0x0004,   0x01, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_512KB, 0x00000000UL, 0x00000000UL, 0x0400,   0x00e0,  0x0003,  0x0004,     0x0004,   0x01, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_1MB,   0x00000000UL, 0x0000000DUL, 0x07c3,   0x0100,  0x0001,  0x0004,     0x0004,   0x08, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_2MB,   0x00000000UL, 0x0000000bUL, 0x0f95,   0x0100,  0x0002,  0x0008,     0x0004,   0x08, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_4MB,   0x00000000UL, 0x0000001bUL, 0x1f25,   0x0100,  0x0002,  0x0008,     0x0004,   0x10, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_8MB,   0x00000000UL, 0x00000019UL, 0x3e67,   0x0100,  0x0003,  0x0010,     0x0004,   0x10, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_16MB,  0x00000000UL, 0x00000029UL, 0x7cd7,   0x0100,  0x0003,  0x0010,     0x0004,   0x20, (char) 0xf8, 0 },
//    { FSYS_MEDIA_RAM_32MB,  0x00000000UL, 0x00000020UL, 0xf760,   0x0200,  0x007c,  0x0020,     0x0004,   0x02, (char) 0xf8, 1 },
//    { FSYS_MEDIA_FD_144MB,  0x00000000UL, 0x00000000UL, 0x0b40,   0x00e0,  0x0009,  0x0012,     0x0002,   0x01, (char) 0xf0, 0 },
//    { FSYS_MEDIA_SMC_1MB,   0x00000000UL, 0x0000000DUL, 0x07c3,   0x0100,  0x0001,  0x0004,     0x0004,   0x08, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_2MB,   0x00000000UL, 0x0000000bUL, 0x0f95,   0x0100,  0x0002,  0x0008,     0x0004,   0x08, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_4MB,   0x00000000UL, 0x0000001bUL, 0x1f25,   0x0100,  0x0002,  0x0008,     0x0004,   0x10, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_8MB,   0x00000000UL, 0x00000019UL, 0x3e67,   0x0100,  0x0003,  0x0010,     0x0004,   0x10, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_16MB,  0x00000000UL, 0x00000029UL, 0x7cd7,   0x0100,  0x0003,  0x0010,     0x0004,   0x20, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_32MB,  0x00000000UL, 0x00000023UL, 0xf9dd,   0x0100,  0x0006,  0x0010,     0x0008,   0x20, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_64MB,  0x0001f3c9UL, 0x00000037UL, 0x0000,   0x0100,  0x000c,  0x0020,     0x0008,   0x20, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SMC_128MB, 0x0003e7d1UL, 0x0000002fUL, 0x0000,   0x0100,  0x0020,  0x0020,     0x0010,   0x20, (char) 0xf8, 1 },
//    { FSYS_MEDIA_MMC_32MB,  0x00000000UL, 0x00000020UL, 0xf460,   0x0200,  0x003d,  0x0020,     0x0004,   0x04, (char) 0xf8, 1 },
//    { FSYS_MEDIA_MMC_64MB,  0x0001e8e0UL, 0x00000020UL, 0x0000,   0x0200,  0x007a,  0x0020,     0x0008,   0x04, (char) 0xf8, 1 },
//    { FSYS_MEDIA_MMC_128MB, 0x0003d2e0UL, 0x00000020UL, 0x0000,   0x0200,  0x00f5,  0x0020,     0x0008,   0x04, (char) 0xf8, 1 },
//    { FSYS_MEDIA_SD_16MB,   0x00000000UL, 0x00000039UL, 0x7187,   0x0200,  0x0003,  0x0020,     0x0002,   0x20, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SD_64MB,   0x0001dbd9UL, 0x00000027UL, 0x0000,   0x0200,  0x000c,  0x0020,     0x0008,   0x20, (char) 0xf8, 0 },
//    { FSYS_MEDIA_SD_128MB,  0x0003c09fUL, 0x00000061UL, 0x0000,   0x0200,  0x001f,  0x0020,     0x0008,   0x20, (char) 0xf8, 1 },
//    { FSYS_MEDIA_CF_32MB,   0x00000000UL, 0x00000020UL, 0xf760,   0x0200,  0x007c,  0x0020,     0x0004,   0x02, (char) 0xf8, 1 },
//    { FSYS_MEDIA_CF_64MB,   0x0001e860UL, 0x00000020UL, 0x0000,   0x0200,  0x007b,  0x0020,     0x0004,   0x04, (char) 0xf8, 1 }
//};


/* table for getting number of root entries for a given media size */

static const _FS_FAT_ROOTENTCNT _FS_auto_rootcnt[] =
{
    {         0x100,     0x40 },
    {         0x200,     0x80 },
    {      0x0b40UL,     0xe0 },
    {  0x0001f3c9UL,    0x100 },
    {  0xffffffffUL,    0x200 }
};

/* table for calculating cluster size */
static const _FS_FAT_SECPERCLUST _FS_auto_secperclust[] =
{
    /* medias up to 512MB are formatted with FAT16 */
    {     0x0b40UL, 0x0001 },
    {      32680UL, 0x0002 },
    {     262144UL, 0x0004 },
    {     524288UL, 0x0008 },
    {    1048576UL, 0x0010 },
    /* medias bigger than 512MB are formatted with FAT32 */
    {   16777216UL, 0x0008 },
    {   33554432UL, 0x0010 },
    {   67108864UL, 0x0020 },
    { 0xffffffffUL, 0x0040 }
};

#endif /* FS_FAT_NOFORMAT==0 */

/*
**********************************************************************************************************************
*             Local functions
**********************************************************************************************************************
*/

#if (FSYS_FAT_NOFORMAT == 0)

/*
**********************************************************************************************************************
*
*             _FS_fat_format
*
  Description:
  FS internal function. Format a media using specified parameters.
  Currently this function needs many parameters. The function will be
  improved.

  Parameters:
  pDriver     - Pointer to a device driver function table.
  Unit        - Unit number.
  SecPerClus  - Number of sector per allocation unit.
  RootEntCnt  - For FAT12/FAT16, this is the number of 32 byte root
                directory entries. 0 for FAT32.
  TotSec16    - 16-bit total count of sectors. If zero, TotSec32 must
                be none-zero.
  TotSec32    - 32-bit total count of sectors. If zero, TotSec16 must
                be none-zero.
  Media       - Media byte.
  FATSz16     - 16-bit count of sectors occupied by one FAT. 0 for
                FAT32 volumes, which use FATSz32.
  FATSz32     - 32-bit count of sectors occupied by one FAT. This is
                valid for FAT32 medias only.
  SecPerTrk   - Sectors per track.
  NumHeads    - Number of heads.
  HiddSec     - Count of hidden sectors preceding the partition.
  FSysType    - ==0 => FAT12
                ==1 => FAT16
                ==2 => FAT32

  Return value:
  >=0         - Media has been formatted.
  <0          - An error has occured.
**********************************************************************************************************************
*/
static int _fsys_fat_format(__fsys_part_t *pPart,
                            char    SecPerClus,
                            __u16   RootEntCnt,
                            __u16   TotSec16,
                            __u32   TotSec32,
                            char    Media,
                            __u16   FATSz16,
                            __u32   FATSz32,
                            __u16   SecPerTrk,
                            __u16   NumHeads,
                            __u32   HiddSec,
                            char FSysType)
 {
    __fsys_fat_BPB_t bpb;
    __u32 sector;
    __u32 value;
    char *buffer;
    int i;
    int j;
    int n;

    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return -1;
    }
    memset(buffer, 0x00, (__u32)FSYS_FAT_SEC_SIZE);
    /* Setup BPB */
    memset(&bpb, 0x00, (__u32)sizeof(bpb));
    bpb.BytesPerSec = 0x0200;       /* _512_,1024,2048,4096           */
    bpb.SecPerClus  = SecPerClus;   /* sec in allocation unit         */
    if (FSysType != 2)
    {
        bpb.RsvdSecCnt  = 0x0001;       /* 1 for FAT12 & FAT16            */
    }
#if (FSYS_FAT_NOFAT32==0)
    else
    {
        bpb.RsvdSecCnt  = 0x0020;       /* 32 for FAT32                   */
    }
#else
    /* FAT32 disabled */
    else
    {
        fsys_fat_free(buffer);
        return -1;
    }
#endif /* FS_FAT_NOFAT32==0 */
    bpb.NumFATs     = 0x02;         /* 2                              */
    bpb.RootEntCnt  = RootEntCnt;   /* number of root dir entries     */
    bpb.TotSec16    = TotSec16;     /* RSVD+FAT+ROOT+DATA (<64k)      */
    bpb.FATSz16     = FATSz16;      /* number of FAT sectors          */
    bpb.TotSec32    = TotSec32;     /* RSVD+FAT+ROOT+FATA (>=64k)     */
    bpb.Signature   = 0xaa55;       /* 0xAA55 Signature               */

    /* setup BPB specifics for FAT32 */
    bpb.FATSz32     = FATSz32;      /* number of FAT sectors          */
    bpb.ExtFlags    = 0x0000;       /* mirroring info                 */
    bpb.RootClus    = 0x00000002UL; /* root dir clus for FAT32        */
    bpb.FSInfo      = 0x0001;       /* position of FSInfo structure   */

    /*
        Prepare buffer with information of the BPB
        offset 0 - 35 is same for FAT12/FAT16 and FAT32
    */

    /* jmpBoot = 0xe9 0x0000 */
    buffer[0]   = (char)0xe9;
    buffer[1]   = (char)0x00;
    buffer[2]   = (char)0x00;
    /* OEMName = '        ' */
    for (i = 3; i < 11; i++)
    {
        buffer[i] = (char)0x20;
    }
    /* BytesPerSec */
    buffer[11]  = (char)(bpb.BytesPerSec & 0xff);
    buffer[12]  = (char)((unsigned int)(bpb.BytesPerSec & 0xff00) >> 8);
    /* SecPerClus */
    buffer[13]  = (char)bpb.SecPerClus;
    /* RsvdSecCnt */
    buffer[14]  = (char)(bpb.RsvdSecCnt & 0xff);
    buffer[15]  = (char)((unsigned int)(bpb.RsvdSecCnt & 0xff00) >> 8);
    /* NumFATs */
    buffer[16]  = (char)bpb.NumFATs;
    /* RootEntCnt */
    buffer[17]  = (char)(bpb.RootEntCnt & 0xff);
    buffer[18]  = (char)((unsigned int)(bpb.RootEntCnt & 0xff00) >> 8);
    /* TotSec16 */
    buffer[19]  = (char)(bpb.TotSec16 & 0xff);
    buffer[20]  = (char)((unsigned int)(bpb.TotSec16 & 0xff00) >> 8);
    /* Media */
    buffer[21]  = Media;
    /* FATSz16 */
    buffer[22]  = (char)(bpb.FATSz16 & 0xff);
    buffer[23]  = (char)((unsigned int)(bpb.FATSz16 & 0xff00) >> 8);
    /* SecPerTrk */
    buffer[24]  = (char)(SecPerTrk & 0xff);
    buffer[25]  = (char)((unsigned int)(SecPerTrk & 0xff00) >> 8);
    /* NumHeads */
    buffer[26]  = (char)(NumHeads & 0xff);
    buffer[27]  = (char)((unsigned int)(NumHeads & 0xff00) >> 8);
    /* HiddSec */
    buffer[28]  = (char)(HiddSec & 0xff);
    buffer[29]  = (char)((__u32)(HiddSec & 0x0000ff00UL) >> 8);
    buffer[30]  = (char)((__u32)(HiddSec & 0x00ff0000UL) >> 16);
    buffer[31]  = (char)((__u32)(HiddSec & 0xff000000UL) >> 24);
    /* TotSec32 */
    buffer[32]  = (char)(bpb.TotSec32 & 0xff);
    buffer[33]  = (char)((__u32)(bpb.TotSec32 & 0x0000ff00UL) >> 8);
    buffer[34]  = (char)((__u32)(bpb.TotSec32 & 0x00ff0000UL) >> 16);
    buffer[35]  = (char)((__u32)(bpb.TotSec32 & 0xff000000UL) >> 24);

    /* Offset 36 and above have different meanings for FAT12/FAT16 and FAT32 */
    if (FSysType != 2)
    {
        /* FAT12/FAT16 */
        /* DrvNum = 0x00 (floppy) */
        buffer[36]  = (char)0x00;
        /* Reserved1 = 0x00 (floppy) */
        buffer[37]  = (char)0x00;
        /* BootSig = 0x00 (next three fields are not 'present') */
        buffer[38]  = (char)0x00;
        /* VolID = 0x00000000 (serial number, e.g. date/time) */
        for (i = 39; i < 43; i++)
        {
            buffer[i] = (char)0x00;
        }
        /* VolLab = ' ' */
        for (i = 43; i < 54; i++)
        {
            buffer[i] = (char)0x20;
        }
        /* FilSysType = 'FAT12' */
        if (FSysType == 0)
        {
            strncpy(&buffer[54], "FAT12   ", 8);
        }
        else
        {
            strncpy(&buffer[54], "FAT16   ", 8);
        }
    }
#if (FSYS_FAT_NOFAT32==0)
    else
    {
        /* FAT32 */
        /* FATSz32 */
        buffer[36]  = (char)(bpb.FATSz32 & 0xff);
        buffer[37]  = (char)((__u32)(bpb.FATSz32 & 0x0000ff00UL) >> 8);
        buffer[38]  = (char)((__u32)(bpb.FATSz32 & 0x00ff0000UL) >> 16);
        buffer[39]  = (char)((__u32)(bpb.FATSz32 & 0xff000000UL) >> 24);
        /* EXTFlags */
        buffer[40]  = (char)(bpb.ExtFlags & 0xff);
        buffer[41]  = (char)((unsigned int)(bpb.ExtFlags & 0xff00) >> 8);
        /* FSVer = 0:0 */
        buffer[42]  = 0x00;
        buffer[43]  = 0x00;
        /* RootClus */
        buffer[44]  = (char)(bpb.RootClus & 0xff);
        buffer[45]  = (char)((__u32)(bpb.RootClus & 0x0000ff00UL) >> 8);
        buffer[46]  = (char)((__u32)(bpb.RootClus & 0x00ff0000UL) >> 16);
        buffer[47]  = (char)((__u32)(bpb.RootClus & 0xff000000UL) >> 24);
        /* FSInfo */
        buffer[48]  = (char)(bpb.FSInfo & 0xff);
        buffer[49]  = (char)((unsigned int)(bpb.FSInfo & 0xff00) >> 8);
        /* BkBootSec = 0x0006; */
        buffer[50]  = 0x06;
        buffer[51]  = 0x00;
        /* Reserved */
        for (i = 52; i < 64; i++)
        {
            buffer[i] = (char)0x00;
        }
        /* DrvNum = 0x00 (floppy) */
        buffer[64]  = (char)0x00;
        /* Reserved1 = 0x00 (floppy) */
        buffer[65]  = (char)0x00;
        /* BootSig = 0x00 (next three fields are not 'present') */
        buffer[66]  = (char)0x00;
        /* VolID = 0x00000000 (serial number, e.g. date/time) */
        for (i = 67; i < 71; i++)
        {
            buffer[i] = (char)0x00;
        }
        /* VolLab = ' ' */
        for (i = 71; i < 82; i++)
        {
            buffer[i] = (char)0x20;
        }
        /* FilSysType = 'FAT12' */
        strncpy(&buffer[82], "FAT32   ", 8);
  }
#endif /* FS_FAT_NOFAT32==0 */
    /* Signature = 0xAA55 */
    buffer[510] = (char)0x55;
    buffer[511] = (char)0xaa;
    /* Write BPB to media */
    i = pPart->pdops.write((void*)buffer, 0, 1, pPart);
    if (i < 0)
    {
        fsys_fat_free(buffer);
        return -1;
    }
    if (FSysType == 2)
    {
        /* Write backup BPB */
        i = pPart->pdops.write((void*)buffer, 6, 1, pPart);
        if (i<0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
    }
    /*  Init FAT 1 & 2   */
    memset(buffer, 0x00, FSYS_FAT_SEC_SIZE);
    for (sector = 0; sector < 2 * (bpb.FATSz16 + bpb.FATSz32); sector++)
    {
        value = sector % (bpb.FATSz16 + bpb.FATSz32);
        if (value != 0)
        {
            i = pPart->pdops.write((void*)buffer, bpb.RsvdSecCnt + sector, 1, pPart);
            if (i<0)
            {
                fsys_fat_free(buffer);
                return -1;
            }
        }
    }
    buffer[0] = (char)Media;
    buffer[1] = (char)0xff;
    buffer[2] = (char)0xff;
    if (FSysType != 0)
    {
        buffer[3] = (char)0xff;
    }
#if (FSYS_FAT_NOFAT32==0)
    if (FSysType == 2)
    {
        buffer[4]   = (char)0xff;
        buffer[5]   = (char)0xff;
        buffer[6]   = (char)0xff;
        buffer[7]   = (char)0x0f;
        buffer[8]   = (char)0xff;
        buffer[9]   = (char)0xff;
        buffer[10]  = (char)0xff;
        buffer[11]  = (char)0x0f;
    }
#endif /* FS_FAT_NOFAT32==0 */
    for (i = 0; i < 2; i++)
    {
        j = pPart->pdops.write((void*)buffer, (__u32)bpb.RsvdSecCnt + i * ((__u32)bpb.FATSz16+bpb.FATSz32), 1, pPart);
        if (j < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
    }
    /* Init root directory area */
    memset(buffer, 0x00, (__u32)FSYS_FAT_SEC_SIZE);
    if (bpb.RootEntCnt != 0)
    {
        /* FAT12/FAT16 */
        n = (((__u32)bpb.RootEntCnt * 32) / (__u32)512);
        for (i = 0; i < n; i++)
        {
            j = pPart->pdops.write((void*)buffer, bpb.RsvdSecCnt + 2 * bpb.FATSz16 + i, 1, pPart);
            if (j < 0)
            {
                fsys_fat_free(buffer);
                return -1;
            }
        }
    }
#if (FSYS_FAT_NOFAT32==0)
    else
    {
        /* FAT32 */
        n = bpb.SecPerClus;
        for (i = 0; i < n; i++)
        {
            j = pPart->pdops.write((void*)buffer, bpb.RsvdSecCnt + 2 * bpb.FATSz32 + (bpb.RootClus - 2) * n + i, 1, pPart);
            if (j < 0)
            {
                fsys_fat_free(buffer);
                return -1;
            }
        }
    }
#endif /* FS_FAT_NOFAT32==0 */
#if (FSYS_FAT_NOFAT32==0)
    if (FSysType == 2)
    {
        /* Init FSInfo */
        memset(buffer, 0x00, FSYS_FAT_SEC_SIZE);
        /* LeadSig = 0x41615252 */
        buffer[0]     = (char)0x52;
        buffer[1]     = (char)0x52;
        buffer[2]     = (char)0x61;
        buffer[3]     = (char)0x41;
        /* StructSig = 0x61417272 */
        buffer[484]   = (char)0x72;
        buffer[485]   = (char)0x72;
        buffer[486]   = (char)0x41;
        buffer[487]   = (char)0x61;
        /* Invalidate last known free cluster count */
        buffer[488]   = (char)0xff;
        buffer[489]   = (char)0xff;
        buffer[490]   = (char)0xff;
        buffer[491]   = (char)0xff;
        /* Give hint for free cluster search */
        buffer[492]   = (char)0xff;
        buffer[493]   = (char)0xff;
        buffer[494]   = (char)0xff;
        buffer[495]   = (char)0xff;
        /* TrailSig = 0xaa550000 */
        buffer[508]   = (char)0x00;
        buffer[509]   = (char)0x00;
        buffer[510]   = (char)0x55;
        buffer[511]   = (char)0xaa;
        i = pPart->pdops.write((void*)buffer, bpb.FSInfo, 1, pPart);
        if (i < 0)
        {
            fsys_fat_free(buffer);
            return -1;
        }
    }
#endif /* FS_FAT_NOFAT32==0 */
    fsys_fat_free(buffer);
    return 0;
}

/*
**********************************************************************************************************************
*
*             _FS_FAT_AutoFormat
*
  Description:
  FS internal function. Get information about the media from the
  device driver. Based on that informaton, calculate parameters for
  formatting that media and call the format routine.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.

  Return value:
  >=0         - Media has been formatted.
  <0          - An error has occured.
**********************************************************************************************************************
*/
int _FS_FAT_AutoFormat(__fsys_part_t * pPart, int force_type)
{
    struct
    {
        __u32 hiddennum;
        __u32 headnum;
        __u32 secnum;
        __u32 partsize;
    } devinfo;
    __u32 rootentcnt;
    __u32 fatsz;
    __u32 rootdirsectors;
    __u32 tmpval1;
    __u32 tmpval2;
    __u32 fatsz32;
    __u32 totsec32;
    __u32 resvdseccnt;
    __u16 totsec16;
    __u16 fatsz16;
    int i;
    unsigned char secperclust;
    char fsystype;

    /* Get info from device */
    devinfo.hiddennum = 0;
    devinfo.headnum   = 0;
    devinfo.secnum    = 0;
    devinfo.partsize  = pPart->len >> 9;

    if (force_type ==0xff )
    {
        if (devinfo.partsize <= 1048576UL)
        {
            fsystype = 1;   /* FAT16 */
        }
#if (FSYS_FAT_NOFAT32==0)
        else
        {
            fsystype = 2;   /* FAT32 */
        }
#else
        else
        {
            /* FAT32 disabled */
            return -1;
        }
#endif /* FS_FAT_NOFAT32==0 */
    }
    else if (force_type == 1 || force_type == 2)
    {
        fsystype = force_type;
    }
    else return -1;

    /* Set correct RootEntCnt & ResvdSecCnt */
    if (fsystype != 2)
    {
        /* FAT16 */
        i = 0;
        while (_FS_auto_rootcnt[i].SecNum < devinfo.partsize)
        {
            i++;
        }
        rootentcnt = _FS_auto_rootcnt[i].Num;
        resvdseccnt = 1;
    }
#if (FSYS_FAT_NOFAT32==0)
    else
    {
        /* FAT32 */
        rootentcnt = 0;
        resvdseccnt = 0x20;
    }
#endif /* FS_FAT_NOFAT32==0 */

    /* Determinate SecPerClust */
    i = 0;
    while (_FS_auto_secperclust[i].SecNum < devinfo.partsize)
    {
        i++;
    }
    secperclust = (unsigned char) _FS_auto_secperclust[i].Num;
    /*
        FAT16/FAT32 FAT size calculation
        The formula used is following the Microsoft specification
        version 1.03 very closely. Therefore the calculated FAT
        size can be up to 8 sectors bigger than required for the
        media. This is a waste of up to 8 sectors, but not a problem
        regarding correctness of the media's data.
    */
    rootdirsectors = ((rootentcnt * 32 ) + 511) / 512;
    tmpval1 = devinfo.partsize - (resvdseccnt + rootdirsectors);
    tmpval2 = (256 * secperclust) + 2;
#if (FSYS_FAT_NOFAT32==0)
    if (fsystype == 2)
    {
        tmpval2 = tmpval2 / 2;
    }
#endif /* FS_FAT_NOFAT32==0 */
    fatsz = (tmpval1 + (tmpval2 - 1)) / tmpval2;
    if (fsystype != 2)
    {
        fatsz16 = fatsz;
        fatsz32 = 0;
    }
#if (FSYS_FAT_NOFAT32==0)
    else
    {
        fatsz16 = 0;
        fatsz32 = fatsz;
    }
#endif /* FS_FAT_NOFAT32==0 */

    /* Number of total sectors (512 byte units) of the media
         This is independent of FAT type (FAT12/FAT16/FAT32) */
    if (devinfo.partsize < 0x10000UL)
    {
        totsec16 = devinfo.partsize;
        totsec32 = 0;
    }
    else
    {
        totsec16 = 0;
        totsec32 = devinfo.partsize;
    }
    /* Format media using calculated values */
    i = _fsys_fat_format(pPart,
                        secperclust,
                        (__u16)rootentcnt,
                        totsec16,
                        totsec32,
                        (char)0xf8,
                        fatsz16,
                        fatsz32,
                        (__u16)devinfo.secnum,
                        (__u16)devinfo.headnum,
                        devinfo.hiddennum,
                        fsystype);
    return i;
}

#endif /* FS_FAT_NOFORMAT==0 */

#if FSYS_FAT_DISKINFO

///*
//**********************************************************************************************************************
//*
//*             _fsys_fat_GetTotalFree
//*
//  Description:
//  FS internal function. Store information about used/unused clusters
//  in a FS_DISKFREE_T data structure.
//
//  Parameters:
//  Idx         - Index of device in the device information table
//                referred by fsys_pDevInfo.
//  Unit        - Unit number.
//  pDiskData   - Pointer to a FS_DISKFREE_T data structure.
//
//  Return value:
//  ==0         - Information is stored in pDiskData.
//  <0          - An error has occured.
//**********************************************************************************************************************
//*/
//static int _fsys_fat_GetTotalFree(__fsys_part_t * pPart, __fsys_diskfree_t *pDiskData)
//{
//    __u32 	freeclust;
//    __u32 	usedclust;
//    __u32 	totclust;
//    __u32 	fatentry;
//    __u32 	fatsize;
//    __s32 	fatoffs;
//    __s32 	bytespersec;
//    __s32 	cursec;
//    __s32	fatsec;
//    __s32 	lastsec;
//    __s32 	fatindex;
//    int 	fattype;
//    int 	err;
//    char 	*buffer;
//    unsigned char a;
//    unsigned char b;
//#if (FSYS_FAT_NOFAT32==0)
//    unsigned char c;
//    unsigned char d;
//#endif
//
//    if (!pDiskData)
//    {
//      return -1;  /* No pointer to a FS_DISKFREE_T structure */
//    }
//    buffer = fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
//    if (!buffer)
//    {
//      return -1;
//    }
//    fattype = fsys_fat_which_type(pPart);
//#if (FSYS_FAT_NOFAT32!=0)
//    if (fattype == 2)
//    {
//        fsys_fat_free(buffer);
//        return -1;
//    }
//#endif /* FS_FAT_NOFAT32!=0 */
//    fatsize = fsys_fat_aBPBUnit->FATSz16;
//    if (fatsize == 0)
//    {
//        fatsize = fsys_fat_aBPBUnit->FATSz32;
//    }
//    bytespersec = (__s32)fsys_fat_aBPBUnit->BytesPerSec;
//    /* Calculate total allocation units on disk */
//    totclust = fsys_fat_aBPBUnit->TotSec16;
//    if (!totclust)
//    {
//        totclust = fsys_fat_aBPBUnit->TotSec32;
//    }
//    totclust  -= fsys_fat_aBPBUnit->RsvdSecCnt;
//    totclust  -= 2*fatsize;
//    usedclust  = fsys_fat_aBPBUnit->RootEntCnt;
//    usedclust *= 0x20;
//    usedclust /= bytespersec;
//    totclust  -= usedclust;
//    totclust  /= fsys_fat_aBPBUnit->SecPerClus;
//    /* Scan FAT for free and used entries */
//    cursec     = 0;
//    fatsec     = 0;
//    lastsec    = -1;
//    fatentry   = 0xffffUL;
//    freeclust  = 0;
//    usedclust  = 0;
//    while (1)
//    {
//        if (cursec >= (__s32)totclust)
//        {
//            break;  /* Last cluster reached */
//        }
//        if (fatsec >= (__s32)fatsize + fsys_fat_aBPBUnit->RsvdSecCnt)
//        {
//            break;  /* End of FAT reached */
//        }
//        if (fattype == 1)
//        {
//            fatindex = (cursec + 2) + ((cursec + 2) / 2);    /* FAT12 */
//        }
//        else if (fattype == 2)
//        {
//            fatindex = (cursec + 2) * 4;               /* FAT32 */
//        }
//        else
//        {
//            fatindex = (cursec + 2) * 2;               /* FAT16 */
//        }
//        fatsec = fsys_fat_aBPBUnit->RsvdSecCnt + (fatindex / bytespersec);
//        fatoffs = fatindex % bytespersec;
//        if (fatsec != lastsec)
//        {
//            err = pPart->pdops.read((void*)buffer, fatsec, 1, pPart);
//            if (err < 0)
//            {
//                err = pPart->pdops.read((void*)buffer, fatsize + fatsec, 1, pPart);
//                if (err < 0)
//                {
//                    fsys_fat_free(buffer);
//                    return -1;
//                }
//                /* Try to repair original FAT sector with contents of copy */
//                pPart->pdops.write((void*)buffer, fatsec, 1, pPart);
//            }
//            lastsec = fatsec;
//        }
//        if (fattype == 1)
//        {
//            if (fatoffs == (bytespersec - 1))
//            {
//                a = buffer[fatoffs];
//                err = pPart->pdops.read((void*)buffer, fatsec+1, 1, pPart);
//                if (err < 0)
//                {
//                    err = pPart->pdops.read((void*)buffer, fatsize + fatsec + 1, 1, pPart);
//                    if (err < 0)
//                    {
//                        fsys_fat_free(buffer);
//                        return -1;
//                    }
//                    /* Try to repair original FAT sector with contents of copy */
//                    pPart->pdops.write((void*)buffer, fatsec + 1, 1, pPart);
//                }
//                lastsec = fatsec + 1;
//                b = buffer[0];
//            }
//            else
//            {
//                a = buffer[fatoffs];
//                b = buffer[fatoffs + 1];
//            }
//            if (cursec & 1)
//            {
//                fatentry = ((a & 0xf0) >> 4 ) + 16 * b;
//            }
//            else
//            {
//                fatentry = a + 256 * (b & 0x0f);
//            }
//            fatentry &= 0x0fff;
//        }
//#if (FSYS_FAT_NOFAT32==0)
//        else if (fattype == 2)
//        {
//            a = buffer[fatoffs];
//            b = buffer[fatoffs + 1];
//            c = buffer[fatoffs + 2];
//            d = buffer[fatoffs + 3];
//            fatentry = a + 0x100UL * b + 0x10000UL * c + 0x1000000UL * d;
//            fatentry &= 0x0fffffffUL;
//        }
//#endif /* FS_FAT_NOFAT32==0 */
//        else
//        {
//            a = buffer[fatoffs];
//            b = buffer[fatoffs + 1];
//            fatentry = a + 256 * b;
//            fatentry &= 0xffffUL;
//        }
//        cursec++;
//        if (fatentry == 0)
//        {
//            freeclust++;
//        }
//        else
//        {
//            usedclust++;
//        }
//    }
//    fsys_fat_free(buffer);
//    pDiskData->total_clusters      = totclust;
//    pDiskData->avail_clusters      = freeclust;
//    pDiskData->sectors_per_cluster = fsys_fat_aBPBUnit->SecPerClus;
//    pDiskData->bytes_per_sector    = (__u16)bytespersec;
//    return 0;
//}

#endif /* FS_FAT_DISKINFO */

/*
**********************************************************************************************************************
*             Global functions
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
*
*             fsys_fat_ioctl
*
  Description:
  FS internal function. Execute device command. The FAT layer checks
  first, if it has to process the command (e.g. format). Any other
  command is passed to the device driver.

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.
  Cmd         - Command to be executed.
  Aux         - Parameter depending on command.
  pBuffer     - Pointer to a buffer used for the command.

  Return value:
  Command specific. In general a negative value means an error.
**********************************************************************************************************************
*/
int fsys_fat_fioctl(__fsys_file_t * pFile, __s32 Cmd, __s32 Aux, void * pBuffer)
{

	return EPDK_OK;
}



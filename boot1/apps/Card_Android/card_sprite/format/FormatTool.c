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
#include    "card_sprite_i.h"
#include    "FormatTool.h"
#include 	"FormatToolPrivate.h"


static int DeviceWriteSector(void *buffer, int start, int count)
{
	int ret;

	ret = wBoot_block_write(start, count , buffer);
	if(!ret)
	{
		return count;
	}

	return 0;
}
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
static int _fsys_fat_format(char    SecPerClus,
                            __u16   RootEntCnt,
                            __u16   TotSec16,
                            __u32   TotSec32,
                            char    Media,
                            __u16   FATSz16,
                            __u32   FATSz32,
                            __u16   SecPerTrk,
                            __u16   NumHeads,
                            __u32   HiddSec,
                            char    FSysType,
                            char*   VolLab,
                            FormatParameter *para)
 {
    __fsys_fat_BPB_t bpb;
    char *buffer;
    int i;
    int j;
    int n;

    buffer = (char *)sprite_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
    	__wrn("malloc failed\n");
        return -1;
    }
    memset(buffer, 0, (size_t)FSYS_FAT_SEC_SIZE);
    /* Setup BPB */
    memset(&bpb, 0x00, (__u32)sizeof(bpb));
    bpb.BytesPerSec = 0x0200;       	/* _512_,1024,2048,4096           */
    bpb.SecPerClus  = SecPerClus;   	/* sec in allocation unit         */
    if (FSysType != 2)
    {
        bpb.RsvdSecCnt  = 0x0001;       /* 1 for FAT12 & FAT16            */
//		bpb.RsvdSecCnt = para->RsvdSecCnt;
    }
    else
    {
        bpb.RsvdSecCnt  = 0x0020;        /* 32 for FAT32                   */
//        bpb.RsvdSecCnt  = para->RsvdSecCnt;/* 32 for FAT32                   */
    }
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
        if(VolLab)strncpy(&buffer[43], VolLab, 11);
        /* FilSysType = 'FAT12' */
        if (FSysType == 0)
        {
            strncpy(&buffer[54], "FAT12   ", 8);
			__inf("fs type is FAT12\n");
			__inf("start format...\n");
        }
        else
        {
            strncpy(&buffer[54], "FAT16   ", 8);
			__inf("fs type is FAT16\n");
			__inf("start format...\n");
        }
    }
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
        if(VolLab)strncpy(&buffer[71], VolLab, 11);
        /* FilSysType = 'FAT12' */
        strncpy(&buffer[82], "FAT32   ", 8);
		__inf("fs type is FAT32\n");
		__inf("start format...\n");
  }
    /* Signature = 0xAA55 */
    buffer[510] = (char)0x55;
    buffer[511] = (char)0xaa;
    /* Write BPB to media */
    i = DeviceWriteSector((void*)buffer, 0, 1);
    if (i < 0)
    {
        sprite_free(buffer);
        return -1;
    }
    if (FSysType == 2)
    {
        /* Write backup BPB */
        i = DeviceWriteSector((void*)buffer, 6, 1);
        if (i<0)
        {
            sprite_free(buffer);
            return -1;
        }
    }
    /*  Init FAT 1 & 2   */
	{
#define PACK_WRITE_SEC_CNT		64

		int left;
		int sec_cnt;
		int sector_num;
		char *FATBuffer;
//		int   PACK_WRITE_SEC_CNT = 1;

//		if(FSysType == 2)
		{
//			PACK_WRITE_SEC_CNT = 32;
		}
		FATBuffer = (char *)sprite_malloc(FSYS_FAT_SEC_SIZE * PACK_WRITE_SEC_CNT);
		if (FATBuffer == NULL) {
			__inf("Malloc failed\n");
			return -1;
		}

		memset(FATBuffer, 0, FSYS_FAT_SEC_SIZE * PACK_WRITE_SEC_CNT);

		left = 2 * (bpb.FATSz16 + bpb.FATSz32);
		sector_num = 0;
		while (left) {
			sec_cnt = left < PACK_WRITE_SEC_CNT ? left : PACK_WRITE_SEC_CNT;
			if(!DeviceWriteSector((void*)FATBuffer, bpb.RsvdSecCnt + sector_num, sec_cnt))
			{
				__inf("write sdcard on sector %d, counter %d failed\n", bpb.RsvdSecCnt + sector_num, sec_cnt);
				return -1;
			}
			left -= sec_cnt;
			sector_num += sec_cnt;
		}
		sprite_free(FATBuffer);

#undef  PACK_WRITE_SEC_CNT
	}

	memset(buffer, 0, FSYS_FAT_SEC_SIZE);
    buffer[0] = (char)Media;
    buffer[1] = (char)0xff;
    buffer[2] = (char)0xff;
    if (FSysType != 0)
    {
        buffer[3] = (char)0xff;
    }
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
    for (i = 0; i < 2; i++)
    {
        j = DeviceWriteSector((void*)buffer, (__u32)bpb.RsvdSecCnt + i * ((__u32)bpb.FATSz16+bpb.FATSz32), 1);
        if (j < 0)
        {
            sprite_free(buffer);
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
            j = DeviceWriteSector((void*)buffer, bpb.RsvdSecCnt + 2 * bpb.FATSz16 + i, 1);
            if (j < 0)
            {
                sprite_free(buffer);
                return -1;
            }
        }
        /* add vol label FDT */
        if(VolLab){
            strncpy(buffer, VolLab, 11);
            for (i = strlen(VolLab); i < 11; i++)
            {
                buffer[i] = (char)0x20;
            }
            buffer[11]=0x08;
            if(DeviceWriteSector((void*)buffer,
                        		 bpb.RsvdSecCnt + 2 * bpb.FATSz16, 1) < 0){
                sprite_free(buffer);
                return -1;
            }
        }
    }
    else
    {
        /* FAT32 */
        n = bpb.SecPerClus;
        for (i = 0; i < n; i++)
        {
            j = DeviceWriteSector((void*)buffer, bpb.RsvdSecCnt + 2 * bpb.FATSz32 + (bpb.RootClus - 2) * n + i, 1);
            if (j < 0)
            {
                sprite_free(buffer);
                return -1;
            }
        }

        /* add vol label FDT */
        if(VolLab){
            strncpy(buffer, VolLab, 11);
            for (i = strlen(VolLab); i < 11; i++)
            {
                buffer[i] = (char)0x20;
            }
            buffer[11]=0x08;
            if(DeviceWriteSector((void*)buffer, bpb.RsvdSecCnt +
                    2 * bpb.FATSz32 + (bpb.RootClus - 2) * n, 1) < 0){
                sprite_free(buffer);
                return -1;
            }
        }
    }
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
        /* Invalidate last known sprite_free cluster count */
        buffer[488]   = (char)0xff;
        buffer[489]   = (char)0xff;
        buffer[490]   = (char)0xff;
        buffer[491]   = (char)0xff;
        /* Give hint for sprite_free cluster search */
        buffer[492]   = (char)0xff;
        buffer[493]   = (char)0xff;
        buffer[494]   = (char)0xff;
        buffer[495]   = (char)0xff;
        /* TrailSig = 0xaa550000 */
        buffer[508]   = (char)0x00;
        buffer[509]   = (char)0x00;
        buffer[510]   = (char)0x55;
        buffer[511]   = (char)0xaa;
        i = DeviceWriteSector((void*)buffer, bpb.FSInfo, 1);
        if (i < 0)
        {
            sprite_free(buffer);
            return -1;
        }
    }
    sprite_free(buffer);

	/* uesr data start Sec: RsvdSecCnt + FAT + Root directory  			   */
	para->UserDataSec = bpb.RsvdSecCnt +
						bpb.NumFATs * bpb.FATSz32 +
						bpb.RootEntCnt * 32 +
						bpb.RootClus * bpb.SecPerClus;
    return 0;
}

/*
**********************************************************************************************************************
*
*             FATFS_Format
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
int FATFS_Format(FormatParameter *para)
{
    struct
    {
        __u32 hiddennum;
        __u32 headnum;
        __u32 secnum;
        __u32 partseccnt;
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
    int force_type = 0xff;
    char* VolLab = NULL;
    int i;
    unsigned char secperclust;
    char fsystype;
    int partseccnt;

    if(para)
	{
        VolLab = para->Label;
    }
    /* Get info from device */
    devinfo.hiddennum = 0;
    devinfo.headnum   = 0;
    devinfo.secnum    = 0;

	/* get part size here */
    partseccnt = wBoot_block_size();
//	partseccnt = 495 << 11;
	if (partseccnt < (((__u32)FSYS_MIN_PART_SIZE)>>FSYS_FAT_SEC_BITS)) {
		__inf("part too small\n");
		return -1;
	}
    devinfo.partseccnt = partseccnt;

	format_log_info("part size is %d mbytes\n", (int)(partseccnt>>11));
    if (force_type ==0xff )
    {
    	/* part size smaller then 512M (512 * 1M) */
        if (devinfo.partseccnt <= 1048576UL)
        {
            fsystype = 1;   /* FAT16 */
        }
        else
        {
            fsystype = 2;   /* FAT32 */
        }
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
        while (_FS_auto_rootcnt[i].SecNum < devinfo.partseccnt)
        {
            i++;
        }
        rootentcnt = _FS_auto_rootcnt[i].Num;
        resvdseccnt = para->RsvdSecCnt;
    }
    else
    {
        /* FAT32 */
        rootentcnt = 0;
//        resvdseccnt = 0x20;
        resvdseccnt = para->RsvdSecCnt;
    }

    /* Determinate SecPerClust */
    i = 0;
    while (_FS_auto_secperclust[i].SecNum < devinfo.partseccnt)
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
    tmpval1 = devinfo.partseccnt - (resvdseccnt + rootdirsectors);
    tmpval2 = (256 * secperclust) + 2;
    if (fsystype == 2)
    {
        tmpval2 = tmpval2 / 2;
    }
    fatsz = (tmpval1 + (tmpval2 - 1)) / tmpval2;
    if (fsystype != 2)
    {
        fatsz16 = fatsz;
        fatsz32 = 0;
    }
    else
    {
        fatsz16 = 0;
        fatsz32 = fatsz;
    }
    /* Number of total sectors (512 byte units) of the media
       This is independent of FAT type (FAT12/FAT16/FAT32) */
    if (devinfo.partseccnt < 0x10000UL)
    {
        totsec16 = devinfo.partseccnt;
        totsec32 = 0;
    }
    else
    {
        totsec16 = 0;
        totsec32 = devinfo.partseccnt;
    }
    /* Format media using calculated values */
    i = _fsys_fat_format(secperclust,
                        (__u16)rootentcnt,
                        totsec16,
                        totsec32,
                        (char)0xf8,
                        fatsz16,
                        fatsz32,
                        (__u16)devinfo.secnum,
                        (__u16)devinfo.headnum,
                        devinfo.hiddennum,
                        fsystype,
                        VolLab,
                        para);
    return i;
}


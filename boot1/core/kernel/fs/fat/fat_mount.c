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

extern int _FS_FAT_AutoFormat(__fsys_part_t * pPart, int force_type);
/*
**********************************************************************************************************************
*             Local functions
**********************************************************************************************************************
*/



/*
**********************************************************************************************************************
*             Global functions
**********************************************************************************************************************
*/
/*
**********************************************************************************************************************
*
*             fsys_fat_ReadBPB
*
  Description:
  FSYS internal function. Read Bios-Parameter-Block from a device and
  copy the relevant data to fsys_fat_aBPBUnit->

  Parameters:
  Idx         - Index of device in the device information table
                referred by fsys_pDevInfo.
  Unit        - Unit number.

  Return value:
  ==0         - BPB successfully read.
  <0          - An error has occured.
*/

int fsys_fat_ReadBPB(__fsys_part_t * pPart)
{
    int 			err;
    unsigned char * buffer;

    buffer = (unsigned char*)fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return EPDK_FAIL;
    }

    err = pPart->pdops.read((void*)buffer, 0, 1, pPart);
    if (err < 0)
    {
        fsys_fat_free(buffer);
        return -1;
    }
    /* Assign fsys_fat_aBPBUnit */
    fsys_fat_aBPBUnit->BytesPerSec   = buffer[11] + 256 * buffer[12];      /* _512_,1024,2048,4096           */
    fsys_fat_aBPBUnit->SecPerClus    = buffer[13];                         /* sec in allocation unit         */
    fsys_fat_aBPBUnit->RsvdSecCnt    = buffer[14] + 256 * buffer[15];      /* 1 for FAT12 & FAT16            */
    fsys_fat_aBPBUnit->NumFATs       = buffer[16];                         /* 2                              */
    fsys_fat_aBPBUnit->RootEntCnt    = buffer[17] + 256 * buffer[18];      /* number of root dir entries     */
    fsys_fat_aBPBUnit->TotSec16      = buffer[19] + 256 * buffer[20];      /* RSVD + FAT + ROOT + FATA(<64k) */
    fsys_fat_aBPBUnit->FATSz16       = buffer[22] + 256 * buffer[23];      /* number of FAT sectors          */
    fsys_fat_aBPBUnit->TotSec32      = buffer[32] + 0x100UL * buffer[33]   /* RSVD + FAT + ROOT + FATA(>=64k)*/
                                                 + 0x10000UL * buffer[34]
                                                 + 0x1000000UL * buffer[35];

	if(!memcmp(&buffer[0x36], "FAT16   ", 8))
	{
		eGon2_printf("fattype FAT16\n");
		fsys_fat_aBPBUnit->fattype   = 0;
	}
	else if(!memcmp(&buffer[0x36], "FAT12   ", 8))
	{
		eGon2_printf("fattype FAT12\n");
		fsys_fat_aBPBUnit->fattype   = 1;
	}
	else
	{
		eGon2_printf("fattype FAT32\n");
		fsys_fat_aBPBUnit->fattype   = 2;
	}

    if (fsys_fat_aBPBUnit->FATSz16 == 0)
    {
        fsys_fat_aBPBUnit->FATSz32   = buffer[36] + 0x100UL * buffer[37]   /* number of FAT sectors          */
                                                 + 0x10000UL * buffer[38]
                                                 + 0x1000000UL * buffer[39];
        fsys_fat_aBPBUnit->ExtFlags  = buffer[40] + 256 * buffer[41];      /* mirroring info                 */
        fsys_fat_aBPBUnit->RootClus  = buffer[44] + 0x100UL * buffer[45]   /* root dir clus for FAT32        */
                                                 + 0x10000UL * buffer[46]
                                                 + 0x1000000UL * buffer[47];
        fsys_fat_aBPBUnit->FSInfo    = buffer[48] + 256 * buffer[49];      /* position of FSInfo structure   */
    }
    else
    {
        fsys_fat_aBPBUnit->FATSz32   = 0;
        fsys_fat_aBPBUnit->ExtFlags  = 0;
        fsys_fat_aBPBUnit->RootClus  = 0;
        fsys_fat_aBPBUnit->FSInfo    = 0;
    }
    fsys_fat_aBPBUnit->Signature     = buffer[FSYS_FAT_SEC_SIZE-2] + 256 * buffer[FSYS_FAT_SEC_SIZE-1];
    fsys_fat_free(buffer);
    return err;
}
/*
**********************************************************************************************************************
*
*             fsys_fat_mount
*
  Description:
    将一个具体的分区挂接到文件系统上

  Parameters:
    pPart       - Pointer to partition handle.

  Return value:
    成功
    失败：分区已经被挂载，或者文件系统不能支持，或者分区被破坏
*/

__s32 fsys_fat_mount(__fsys_part_t * pPart)
{
    int err;
    int lexp;

//	if(pPart->supper_data)
//	{
//		return EPDK_FAIL;
//	}

	pPart->supper_data = (__hdle)fsys_fat_bpb_malloc();
	if(!pPart->supper_data)
	{
		return EPDK_FAIL;
	}

	err = fsys_fat_ReadBPB(pPart);
	if(err < 0)
	{
		fsys_fat_bpb_free((void*)pPart->supper_data);
		pPart->supper_data = 0;
		return EPDK_FAIL;
	}

    if (fsys_fat_aBPBUnit->Signature != 0xaa55)
    {
        err = fsys_fat_ReadBPB(pPart);
        lexp = (err < 0);
        lexp = lexp || (fsys_fat_aBPBUnit->Signature != 0xaa55);
        if (lexp)
        {
			fsys_fat_bpb_free( (void*)pPart->supper_data);
			pPart->supper_data = 0;
            return EPDK_FAIL;
        }
    }
    if (fsys_fat_aBPBUnit->NumFATs != 2)
    {
		fsys_fat_bpb_free( (void*)pPart->supper_data);
		pPart->supper_data = 0;
        return EPDK_FAIL;  /* Only 2 FATs are supported */
    }
    if (fsys_fat_aBPBUnit->FATSz16 == 0)
    {
        if (fsys_fat_aBPBUnit->ExtFlags & 0x0080)
        {
			fsys_fat_bpb_free( (void*)pPart->supper_data);
			pPart->supper_data = 0;
            return EPDK_FAIL;  /* Only mirroring at runtime supported */
        }
    }

	return EPDK_OK;
}


/*
**********************************************************************************************************************
*
*             fsys_fat_unmount
*
  Description:
    将一个具体的分区脱离文件系统

  Parameters:
    pPart       - Pointer to partition handle.

  Return value:
    成功
    失败：分区已经被卸掉
*/

__s32 fsys_fat_unmount(__fsys_part_t *pPart)
{
    /* 清除文件系统附着在分区上的私有的信息			*/
	fsys_fat_bpb_free( (void*)pPart->supper_data);
    pPart->supper_data = 0;
    return EPDK_OK;
}



/*
**********************************************************************************************************************
*
*             fsys_fatfs_identify
*
  Description:
	识别分区是否为fatfs文件系统

  Parameters:


  Return value:

*/

__bool fsys_fatfs_identify(__fsys_part_t * pPart)
{
    __u32 err;

    unsigned char * buffer;

    buffer = (unsigned char*)fsys_fat_malloc(FSYS_FAT_SEC_SIZE);
    if (!buffer)
    {
        return EPDK_FAIL;
    }

    err = pPart->pdops.read((void*)buffer, 0, 1, pPart);
    if (err <= 0)
    {
        fsys_fat_free(buffer);
        return -1;
    }

    if(buffer[16] != 2)
	{
		fsys_fat_free(buffer);
        return EPDK_FALSE;
	}
    if(buffer[FSYS_FAT_SEC_SIZE-2] + 256 * buffer[FSYS_FAT_SEC_SIZE-1] != 0xaa55)
	{
		fsys_fat_free(buffer);
        return EPDK_FALSE;
	}

	fsys_fat_free(buffer);
    return EPDK_TRUE;
}
/*
**********************************************************************************************************************
*
*             fsys_fatfs_format
*
  Description:


  Parameters:


  Return value:

*/
__s32 fsys_fatfs_format(__fsys_part_t * pPart, int force_type)
{
    return _FS_FAT_AutoFormat(pPart, force_type);
}
/*
**********************************************************************************************************************
*
*             fsys_fatfs_checkdisk
*
  Description:


  Parameters:


  Return value:

*/
__s32 fsys_fatfs_checkdisk(__fsys_part_t * pPart)
{
    return EPDK_OK;
}

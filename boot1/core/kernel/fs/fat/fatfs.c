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

#include  "fat_priv.h"

#define     FSTYPE_FAT            1

__fsys_fsops_t fsys_fatfs_ops =
{
    fsys_fat_fopen,                                 /* open                                                         */
    fsys_fat_fclose,                                /* close                                                        */
    fsys_fat_fread,                                 /* read                                                         */
    fsys_fat_fwrite,                                /* write                                                        */
    0,                                              /* tell                                                         */
    fsys_fat_fseek,                                 /* seek                                                         */
    fsys_fat_fioctl,                                /* ioctl                                                        */

    fsys_fat_opendir,                               /* opendir                                                      */
    fsys_fat_closedir,                              /* closedir                                                     */
    fsys_fat_readdir,                               /* readdir                                                      */
    0,                                              /* rewinddir                                                    */
    fsys_fat_MkRmDir,                               /* mkdir                                                        */
    fsys_fat_MkRmDir,                               /* rmdir                                                        */

    fsys_fat_mount,
    fsys_fat_unmount,
    fsys_fatfs_identify,
    fsys_fatfs_format,
    fsys_fatfs_checkdisk,
    fsys_fat_which_type,
};

extern int fat_sector_cache_init(void);
/*
*********************************************************************************************************
*                                         Initialize OS Resources
*********************************************************************************************************
*/
__s32 fsys_fatfs_init (void)
{
    fsys_fat_block_init();
    fsys_fat_bpb_block_init();
  	fat_sector_cache_init();
	return EPDK_OK;
}




/*
*********************************************************************************************************
*                                         Reclaim OS Resources
*********************************************************************************************************
*/

int  fsys_fatfs_exit (void)
{
	return EPDK_OK;
}
__fsys_fs_t fatfs = {
    "fat",
    FSTYPE_FAT,
    &fsys_fatfs_ops,
    fsys_fatfs_init,
    fsys_fatfs_exit,
};

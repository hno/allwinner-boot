/*
*********************************************************************************************************
*											        ePOS
*								the Easy Portable/Player Operation System
*									       file system sub-system
*												fat12/16/32
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : fsys_fatfs.c
* By      : steven.ZGJ
* Version : V1.00
* Descriptions:
*           为了避免重入问题，在这里建立信号量
*********************************************************************************************************
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

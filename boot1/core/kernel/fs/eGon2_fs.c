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
#include "fs_i.h"
#include "eGon2_fs_i.h"


extern __fsys_pdops_t vPDops;
extern __fsys_fs_t    fatfs;
extern __s32          part_init(void);
extern __s32          part_exit(void);


#define 	 MAX_FS_NUM 8
__fsys_fs_t* fs_array[MAX_FS_NUM] = {&fatfs, 0};

__fsys_part_t part_array[MAX_PARTS_NUM]; //= { 0 };

//#define MAX_SUPERBLOCK_NUM MAX_PARTS_NUM

static __fsys_file_t      _fsys_filehandle[FSYS_FILE_MAXOPEN];
static __fsys_dir_t       _fsys_dirhandle[FSYS_DIR_MAXOPEN];

#define FSYS_VALID_MODE_NUM     (sizeof(_fsys_valid_modes) / sizeof(__fsys_mode_t))
typedef struct __FSYS_MODE
{
    char         *mode;
    unsigned char mode_r;                           /* mode READ                                                    */
    unsigned char mode_w;                           /* mode WRITE                                                   */
    unsigned char mode_a;                           /* mode APPEND                                                  */
    unsigned char mode_c;                           /* mode CREATE                                                  */
    unsigned char mode_b;                           /* mode BINARY                                                  */
} __fsys_mode_t;

static const __fsys_mode_t _fsys_valid_modes[] =
{
    /*       READ  WRITE  APPEND  CREATE  BINARY */
    { "r"   ,  1,    0,     0,       0,     0 },
    { "w"   ,  0,    1,     0,       1,     0 },
    { "a"   ,  0,    1,     1,       1,     0 },
    { "rb"  ,  1,    0,     0,       0,     1 },
    { "wb"  ,  0,    1,     0,       1,     1 },
    { "ab"  ,  0,    1,     1,       1,     1 },
    { "r+"  ,  1,    1,     0,       0,     0 },
    { "w+"  ,  1,    1,     0,       1,     0 },
    { "a+"  ,  1,    1,     1,       1,     0 },
    { "r+b" ,  1,    1,     0,       0,     1 },
    { "rb+" ,  1,    1,     0,       0,     1 },
    { "w+b" ,  1,    1,     0,       1,     1 },
    { "wb+" ,  1,    1,     0,       1,     1 },
    { "a+b" ,  1,    1,     1,       1,     1 },
    { "ab+" ,  1,    1,     1,       1,     1 }
};
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
int set_file_mode(char* pMode, __fsys_file_t* fp)
{
    int j, c;

    j = 0;
    while (1)
    {
        if (j >= FSYS_VALID_MODE_NUM)
            break;                          /* Not in list of valid modes                                   */
        c = strcmp(pMode, _fsys_valid_modes[j].mode);
        if (c == 0)
            break;                          /* Mode found in list                                           */
        j++;
    }
    if (j < FSYS_VALID_MODE_NUM)
    {
        /* Set mode flags according to the mode string */
        fp->mode_r = _fsys_valid_modes[j].mode_r;
        fp->mode_w = _fsys_valid_modes[j].mode_w;
        fp->mode_a = _fsys_valid_modes[j].mode_a;
        fp->mode_c = _fsys_valid_modes[j].mode_c;
        fp->mode_b = _fsys_valid_modes[j].mode_b;
        return FS_OK;
    }
    else
    {
        return FS_FAIL;
    }
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
__fsys_file_t* file_h_alloc(void)
{
    int i = 0;
    for  (i=0; i < FSYS_FILE_MAXOPEN; i++)
    {
        if (!_fsys_filehandle[i].inuse)
        {
           _fsys_filehandle[i].inuse = 1;
           break;                              /* Unused entry found                                           */
        }
    }
    if (i >= FSYS_FILE_MAXOPEN)
       return NULL;
    else
       return &_fsys_filehandle[i];
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
void file_h_free(__fsys_file_t* hdle)
{
    hdle->inuse = 0;
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
__fsys_dir_t* dir_h_alloc(void)
{
    int i = 0;
    for  (i=0; i < FSYS_DIR_MAXOPEN; i++)
    {
        if (!_fsys_dirhandle[i].inuse)
        {
           _fsys_dirhandle[i].inuse = 1;
           break;                              /* Unused entry found                                           */
        }
    }
    if (i >= FSYS_DIR_MAXOPEN)
       return NULL;
    else
       return &_fsys_dirhandle[i];
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
void dir_h_free(__fsys_dir_t* hdle)
{
    hdle->inuse = 0;
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
__fsys_part_t* part_h_alloc(void)
{
    int i = 0;

    for  (i=0; i < MAX_PARTS_NUM; i++)
    {
        if (!part_array[i].inuse)
        {
           part_array[i].inuse = 1;
           break;                              /* Unused entry found                                           */
        }
    }
    if (i >= MAX_PARTS_NUM)
       return NULL;
    else
       return &part_array[i];
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
void part_h_free(__fsys_part_t* hdle)
{
    hdle->inuse = 0;
}

//scan and check the part array for special disk symble
//return --  -1(FS_FAIL) FS_FAIL
//           >=0      fs array index


void check_if_change_disk_part(char *disk)
{
	if((*disk == 'z') || (*disk == 'Z'))
	{
		*disk = 'c' + FS_getpart_count(2) - 1;
	}

	return;
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
__fsys_part_t* locate_part_handle(char disk)
{
	int i;

    if ( disk >= 'C' && disk <= 'Z' )
        disk += 'a' - 'A';
    if (disk < 'c' || disk > 'z' )
	{
		fsprintf("disk symble is invalid, FS_FAIL!\n");

		return NULL;
	}

//    for (i= 0; i < MAX_PARTS_NUM && part_array[i].inuse != 0; i++)
    for (i= 0; i < MAX_PARTS_NUM; i++)
	{
		if ( part_array[i].disk_symble == disk )
		{
			return &part_array[i];
		}
	}

	return NULL;
}

//scan and check the fs array for fs_formatinfo
//return --  -1(FS_FAIL) FS_FAIL
//           >=0      fs array index
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
int get_fs_item(int fs_type)
{
	int fs_index;
	int i;

	for (fs_index = 0xff, i= 0; i < MAX_FS_NUM && fs_array[i] != 0; i++)
	{
		if ( fs_array[i]->type == fs_type )
		{
			fs_index = i;
			break;
		}
	}
	if ( fs_index == 0xff )
	{
		fsprintf("the filesystem type is not support, FS_FAIL!\n");

		return FS_FAIL;
	}
	return fs_index;
}

//find out fs type within the part and return the fs array index
//return --  -1(FS_FAIL) FS_FAIL
//           >=0      fs array index
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
static int identify_fs_item(__fsys_part_t* part)
{
	int fs_index;
	int i;

	for (fs_index = 0xff, i= 0; i < MAX_FS_NUM && fs_array[i] != 0; i++)
	{
		if ( fs_array[i]->ops->identify(part) == EPDK_TRUE )
		{
			fs_index = i;
			break;
		}
	}
	if ( fs_index == 0xff )
	{
		fsprintf("unkown fs type in the input file, FS_FAIL!\n");

		return FS_FAIL;
	}
	return fs_index;
}

//extern __fsys_part_t 	*bootpart_array[];
FS_PART_OPTS_t   pCurPartOpts;
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
__s32 FSMount(char disk)
{
	int fs_index;
	__fsys_part_t *part; //= bootpart_array[0]; /* now only support 1 part*/
	int i;

	check_if_change_disk_part(&disk);
    //check if 'disk' is already been used
	if ( locate_part_handle(disk) != NULL )
	{
		fsprintf("disk ""%c"" has been used! FS_FAIL\n", disk);

		return FS_FAIL;
	}
	if ( disk >= 'C' && disk <= 'Z' )
	{
	    part = &part_array[disk - 'C'];
	}
	else
	{
	    part = &part_array[disk - 'c'];
    }
	part->disk_symble = disk;

	//identify the fs for the part
	fs_index = identify_fs_item(part);
	if (fs_index == FS_FAIL)
	{
		//part_h_free(part);
		part->inuse = 0;

		return FS_FAIL;
	}

	//mount the part
	i = fs_array[fs_index]->ops->mount(part);
	if ( i == FS_OK )
	{
		part->inuse = 1;
		part->pFS= fs_array[fs_index];

		return FS_OK;
	}
	else
	{
		fsprintf("mount error, FS_FAIL!\n");
		//part_h_free(part);
		part->inuse = 0;

		return FS_FAIL;
	}
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
__s32 FSUnmount(char disk)
{
	__fsys_part_t *part;
	int res = FS_OK;

	check_if_change_disk_part(&disk);
    part = locate_part_handle(disk);
	if (part == NULL)
	{
		return FS_FAIL;
	}

    part->pFS->ops->unmount(part);

	//part_h_free(part);
	part->inuse = 0;

	return res;
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
HDIR FS_opendir(const char * dir)
{
	__fsys_part_t *part;
	__fsys_dir_t *res;

	check_if_change_disk_part((char *)dir);
	part = locate_part_handle(*dir);
	if ( part == NULL )
	{
		return NULL;
	}

    res = dir_h_alloc();
    res->pPart = part;
    res->pFS = part->pFS;

	dir+=2;
	if ( part->pFS->ops->opendir(dir, res) > 0 )
	{
		return (HDIR)res;
    }
	else
    {
        dir_h_free(res);

        return NULL;
    }
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
int  FS_readdir(HDIR hdir, ENTRY* ent)
{
    __fsys_dirent_t *et;

    et = ((__fsys_dir_t*)hdir)->pFS->ops->readdir((__fsys_dir_t*)hdir);
	if (et > 0)
	{
		strncpy((char*)ent->d_name, (char*)et->d_name, FSYS_dirnamemax);
		ent->f_attr = et->fatdirattr;
	}

	return (int)et==0 ? FS_FAIL : FS_OK;
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
__s32   FS_closedir(HDIR hdir)
{
	((__fsys_dir_t*)hdir)->pFS->ops->closedir((__fsys_dir_t*)hdir);
    dir_h_free((__fsys_dir_t*)hdir);

    return FS_OK;
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
__s32 	FS_mkdir(char * dir)		//存在同名的dir就不要重复创建dir
{
	__fsys_part_t *part;

	check_if_change_disk_part(dir);
	part = locate_part_handle(*dir);
	if ( part == NULL )
	{
		return FS_FAIL;
	}

	dir+=2;

	return part->pFS->ops->mkdir(dir, part, 1);
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
__s32 	FS_rmdir(char * dir)		//dir目录非空就不删除目录,需要检测dir不存在的情况
{
	__fsys_part_t *part;

	check_if_change_disk_part(dir);
	part = locate_part_handle(*dir);
	if ( part == NULL ) return FS_FAIL;

	dir+=2;

	return part->pFS->ops->rmdir(dir,part, 0);
}

//------------------------------------------------------------------------------------------------------------
//文件操作
//------------------------------------------------------------------------------------------------------------
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
H_FILE  FS_fopen(const char * filename, char * mode)			//文件打开/创建
{
	__fsys_part_t *part;
    __fsys_file_t* res;

	check_if_change_disk_part((char *)filename);
	part = locate_part_handle(*filename);
	if ( part == NULL ) return NULL;

    res = file_h_alloc();
    res->pPart = part;
    res->pFS = part->pFS;
	set_file_mode( mode, res);

	filename += 2;
	if ( part->pFS->ops->fopen(filename, mode, res) > 0)
	{
		return res;
    }
	else
    {
        if (mode[0]=='d' && mode[1]=='e' && mode[2]=='l')
        {
            int i;

            i = res->error;
            file_h_free(res);

            return (H_FILE)i;
        }
        file_h_free(res);

        return NULL;
    }

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
__s32   FS_fclose(H_FILE hfile)						//文件关闭
{
	((__fsys_file_t*)hfile)->pFS->ops->fclose((__fsys_file_t*)hfile);
    file_h_free((__fsys_file_t*)hfile);
    return FS_OK;
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
__s32 	FS_fread(void * buffer, __u32 block_size, __u32 count, H_FILE hfile)	//读文件
{
	return ((__fsys_file_t*)hfile)->pFS->ops->fread(buffer, block_size, count, (__fsys_file_t*)hfile);
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
__s32 	FS_fwrite(void * buffer, __u32 block_size, __u32 count, H_FILE hfile) //写文件
{
	return ((__fsys_file_t*)hfile)->pFS->ops->fwrite(buffer, block_size, count, (__fsys_file_t*)hfile);
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
__s32  FS_rmfile(const char * filename)
{
	return (__s32)FS_fopen(filename, "del");
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
__s32 	FS_fseek(H_FILE hfile, __s32 offset, __s32 whence)//文件指针偏移
{
	return ((__fsys_file_t*)hfile)->pFS->ops->fseek((__fsys_file_t*)hfile, offset, whence);
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
__s32 	FS_feof(H_FILE hfile)							//判断是否到文件末尾
{
	return (((__fsys_file_t*)hfile)->filepos == ((__fsys_file_t*)hfile)->size);
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
__u32 	FS_ftell(H_FILE hfile)							//查看当前文件指针偏移
{
	return ((__fsys_file_t*)hfile)->filepos;
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
__u32 	FS_filelen(H_FILE hfile)						//查看文件的长度
{
	return ((__fsys_file_t*)hfile)->size;
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
__s32 FS_regpartopts(FS_PART_OPTS_t *pPartOpts)
{
	if (pPartOpts)
	{
		pCurPartOpts.Write = pPartOpts->Write;
		pCurPartOpts.Read  = pPartOpts->Read;
		pCurPartOpts.Init  = pPartOpts->Init;
		pCurPartOpts.Exit  = pPartOpts->Exit;

		return FS_OK;
	}

	return FS_FAIL;
}


extern __s32 boot_part_init(void);
extern __s32 boot_part_exit(void);

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
__s32  FS_init(void)
{
    int i;

	memset(part_array, 0, MAX_PARTS_NUM * sizeof(__fsys_part_t));
	if(!boot_part_init())
	{
		eGon2_printf("fs init fail\n");
		return -1;
	}
    for(i=0; i<MAX_FS_NUM; i++)
	{
        if (fs_array[i])
        {
            fs_array[i]->fs_init();
    	}
	}

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
__s32 FS_exit(void)
{
    int i;

	boot_part_exit();
    for(i=0; i<MAX_FS_NUM; i++)
	{
        if (fs_array[i])
        {
            fs_array[i]->fs_exit();
        }
	}

    return 0;
}




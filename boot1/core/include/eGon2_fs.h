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
#ifndef     __EGON2_FS__H__
#define     __EGON2_FS__H__             1

typedef struct FS_FORMAT_INFO
{
    int m_type;
	int s_type;
    int size;
}FS_FORMAT_INFO_t;

typedef struct FS_PART_OPTS
{
	int (*Write)(unsigned int nSectNum, unsigned int nSectorCnt, void * pBuf);
	int (*Read )(unsigned int nSectNum, unsigned int nSectorCnt, void * pBuf);
	int (*Init )(void);
	int (*Exit )(void);
}FS_PART_OPTS_t;


/**
**********************************************************************************************************************
*
* @fn - int FS_regpartopts(void)
*
* @brief - 注册分区操作函数，需在文件系统初始化之前完成
* @return - OK OR FAIL
**********************************************************************************************************************
*/
int FS_regpartopts(FS_PART_OPTS_t *pPartOpts);

/**
**********************************************************************************************************************
*
* @fn - int FSMount(void)
*
* @brief - 挂载flash盘中的系统分区
* @return - OK OR FAIL
**********************************************************************************************************************
*/
int FSMount(char disk);

/**
**********************************************************************************************************************
*
* @fn - int FSUnmount(void)
*
* @brief - 卸载flash盘的系统分区
* @return - OK OR FAIL
**********************************************************************************************************************
*/
int FSUnmount(char disk);
/**
**********************************************************************************************************************
*
* @fn - HDIR FS_opendir(char * dir)
*
* @brief - 打开镜像缓存中的一个目录
* @param - dir : 镜像中目录名(必须是绝对路径)
* @return - NULL
*          > 0  - 目录句柄
**********************************************************************************************************************
*/
HDIR FS_opendir(const char * dir);

/**
**********************************************************************************************************************
*
* @fn - int  FS_readdir(HDIR hdir, ENTRY* ent)
*
* @brief - 读取镜像缓存中的目录的目录项
* @param - hdir : 目录句柄
* @param - ent : input entry prt for store the result
* @return - OK or FAIL
**********************************************************************************************************************
*/
int  FS_readdir(HDIR hdir, ENTRY* ent);

/**
**********************************************************************************************************************
*
* @fn - int   FS_closedir(HDIR hdir)
*
* @brief - 关闭镜像缓存中的目录的目录项
* @param - hdir : 目录句柄
* @return - OK or FAIL
**********************************************************************************************************************
*/
int   FS_closedir(HDIR hdir);

/**
**********************************************************************************************************************
*
* @fn - int 	FS_mkdir(char * dir)
*
* @brief - 在镜像缓存中创建一个目录，存在同名的dir报错退出
* @param - dir : 镜像中目录名(必须是绝对路径)
* @return - OK or FAIL
**********************************************************************************************************************
*/
int 	FS_mkdir(char * dir);

/**
**********************************************************************************************************************
*
* @fn - int 	FS_rmdir(char * dir)
*
* @brief - 在镜像缓存中删除一个目录，目录必须为空且存在，否则报错
* @param - dir : 镜像中目录名(必须是绝对路径)
* @return - OK or FAIL
**********************************************************************************************************************
*/
int 	FS_rmdir(char * dir);

/**
**********************************************************************************************************************
*
* @fn - H_FILE FS_fopen(char * filename, char * mode)
*
* @brief - 在镜像缓存中 打开 or 创建一个文件
* @param - filename : 镜像中文件名(必须是绝对路径)
* @param - mode : 模式（同标准fopen函数）
* @return - NULL 失败
*           >  0 文件句柄
**********************************************************************************************************************
*/
H_FILE FS_fopen(const char * filename, char * mode);

/**
**********************************************************************************************************************
*
* @fn - int FS_fclose(H_FILE hfile)
*
* @brief - 在镜像缓存中关闭一个文件
* @param - filename : 镜像中文件名(必须是绝对路径)
* @return - OK or FAIL
**********************************************************************************************************************
*/
int FS_fclose(H_FILE hfile);

/**
**********************************************************************************************************************
*
* @fn - int FS_fread(void * buffer, unsigned int block_size, unsigned int count, H_FILE hfile)
*
* @brief - 读取镜像缓存中一个文件的内容
* @param - hfile : 打开的文件句柄
* @param - buffer : 结果buffer
* @param - block_size : 读取操作的单目大小
* @param - count : 读取操作的单目数
* @return - 读取出来的单目数目
**********************************************************************************************************************
*/
int FS_fread(void * buffer, unsigned int block_size, unsigned int count, H_FILE hfile);

/**
**********************************************************************************************************************
*
* @fn - int FS_fwrite(void * buffer, unsigned int block_size, unsigned int count, H_FILE hfile)
*
* @brief - 写镜像缓存中一个文件
* @param - hfile : 打开的文件句柄
* @param - buffer : 数据源buffer
* @param - block_size : 写操作的单目大小
* @param - count : 写操作的单目数
* @return - 成功写入的单目数目
**********************************************************************************************************************
*/
int FS_fwrite(void * buffer, unsigned int block_size, unsigned int count, H_FILE hfile);

/**
**********************************************************************************************************************
*
* @fn - int FS_rmfile(char * filename)
*
* @brief - 在镜像缓存中删除 一个文件
* @param - filename : 镜像中文件名(必须是绝对路径)
* @return - OK or FAIL
**********************************************************************************************************************
*/
int FS_rmfile(const char * filename);

/**
**********************************************************************************************************************
*
* @fn - int FS_fseek(H_FILE hfile, int offset, int whence)
*
* @brief - seek
* @param - hfile : 打开的文件句柄
* @param - offset :
* @param - whence :
* @return - OK or FAIL
**********************************************************************************************************************
*/
int FS_fseek(H_FILE hfile, int offset, int whence);

/**
**********************************************************************************************************************
*
* @fn - int 	FS_feof(H_FILE hfile)
*
* @brief - test eof
* @param - hfile : 打开的文件句柄
* @return - 1 : eof
*           0 : not eof
**********************************************************************************************************************
*/
int 	FS_feof(H_FILE hfile);

//PLUGINAPI FSTAT*  FS_fstat(H_FILE hfile);

/**
**********************************************************************************************************************
*
* @fn - int 	FS_filelen(H_FILE hfile)
*
* @brief - filelen
* @param - hfile : 打开的文件句柄
* @return - file length
*
**********************************************************************************************************************
*/
unsigned int 	FS_filelen(H_FILE hfile);

/**
**********************************************************************************************************************
*
* @fn - int 	FS_ftell(H_FILE hfile)
*
* @brief - ftell
* @param - hfile : 打开的文件句柄
* @return - file length
*
**********************************************************************************************************************
*/
unsigned int 	FS_ftell(H_FILE hfile);
/**
**********************************************************************************************************************
*
* @fn - int 	FS_getpart_info(const char *class_name, const char *name, void *part_info)
*
* @brief - ftell
* @param - hfile : class_name: 用于匹配的次名称
*                  name      : 用于匹配的主名称
*                  part_info : 用于获取参数的数据结构地址
* @return - 0： 成功
*           -1: 失败
*
**********************************************************************************************************************
*/
int FS_getpart_info(const char *class_name, const char *name, part_info_t *part_info);
int FS_getpart_count(int part_attribute);
int FS_getpart_start(int part_index);
int FS_getpart_capacity(int part_index);
/**
**********************************************************************************************************************
*
* @fn - int 	FS_init(void)
*
* @brief - test eof
* @param - hfile : 初始化文件系统的内部环境
* @return - 1 : eof
*           0 : not eof
**********************************************************************************************************************
*/
int 	FS_init(void);

/**
**********************************************************************************************************************
*
* @fn - int 	FS_exit(void)
*
* @brief - test eof
* @param - hfile :
* @return - 1 : eof
*           0 : not eof
**********************************************************************************************************************
*/
int 	FS_exit(void);

#endif  /* __EGON2_FS__H__ */



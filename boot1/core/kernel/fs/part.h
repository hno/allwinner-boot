/*
**********************************************************************************************************************
*											        eGon
*						           the Easy Portable/Player Operation System
*									       eGON fsys sub-system
*
*						         (c) Copyright 2010-2012, Jerry China
*											All	Rights Reserved
*
* File    :
* By      : Jerry
* Version : V2.00
* Date	  :
**********************************************************************************************************************
*/
#ifndef __PART_H__
#define __PART_H__

#define FSYS_MAX_PARTS          8
#define FSYS_PART_CMD_GET_PARTSIZE      1000L       /* 获取part大小，byte为单位                                     */
#define FSYS_PART_CMD_FLUSH_CACHE       1001L       /* flash cache                                                  */

#define FSYS_PART_CMD_INC_BUSYCNT       1100L       /* 增加busy count                                               */
#define FSYS_PART_CMD_DEC_BUSYCNT       1101L       /* 减少busy count                                               */
#define FSYS_PART_CMD_GET_STATUS        1102L       /* 获取状态                                                     */
#define FSYS_PART_CMD_GET_INFO          1103L       /* 获取info                                                     */
#define FSYS_PART_CMD_FORMAT            1104L       /* 格式化                                                       */

#define FSYS_PART_MODE_CACHE_USED       1002L       /* 启动cache机制                                                */
#define FSYS_PART_MODE_CACHE_UNUSED     1003L       /* 关闭cache机制                                                */

#define FSYS_PART_MEDIACHANGED          0x0001

typedef struct __FSYS_PART       	    __fsys_part_t;          /* 分区                                             */
typedef struct __FSYS_PDOPS             __fsys_pdops_t;         /* 分区操作                                         */
typedef struct __FSYS_DISKFREE          __fsys_diskfree_t;      /*                                                  */
typedef struct __FSYS_FS                __fsys_fs_t;

struct __FSYS_PDOPS
{
      signed int            (*read )(void *pData, unsigned int Sector, unsigned int N, __fsys_part_t * pPart  );
      signed int            (*write)(void *pData, unsigned int Sector, unsigned int N, __fsys_part_t * pPart  );
      signed int            (*ioctl)(__fsys_part_t * pPart,int Cmd,             int Aux,       void  * pBuffer);
};

struct __FSYS_PART
{
    char                    * name;                   /* 分区名                                                       */
    char					disk_symble;              /* just like "c" "d"... */
	__fsys_fs_t				* pFS;
    __hdle                  supper_data;
    int                     inuse;

    int                     start;
    int                     len;

	__fsys_pdops_t          pdops;
	void*					hPDPrivate;

    int                     error;                  /* 分区操作出错的纪录                                           */
};

struct __FSYS_DISKFREE
{
    unsigned int     total_clusters;
    unsigned int     avail_clusters;
    unsigned short   sectors_per_cluster;
    unsigned short   bytes_per_sector;
};

#endif      /* __PART_H__ */

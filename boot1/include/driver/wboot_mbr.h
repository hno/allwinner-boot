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
#ifndef	_WBOOT_MBR_H_
#define	_WBOOT_MBR_H_


#define     DOWNLOAD_MAP_NAME   "dlinfo.fex"
#define     SUNXI_MBR_NAME      "sunxi_mbr.fex"
/* MBR       */
#define     MBR_SIZE			    (16 * 1024)
#define     DL_SIZE					(16 * 1024)
#define   	MBR_MAGIC			    "softw411"
#define     MBR_MAX_PART_COUNT		120
#define     MBR_COPY_NUM          	4    //mbr的备份数量
#define     MBR_RESERVED          	(MBR_SIZE - 32 - (MBR_MAX_PART_COUNT * sizeof(PARTITION)))   //mbr保留的空间
#define     DL_RESERVED           	(DL_SIZE - 32 - (MBR_MAX_PART_COUNT * sizeof(dl_one_part_info)))

/* partition information */
typedef struct sunxi_partition_t
{
	unsigned  int       addrhi;				//起始地址, 以扇区为单位
	unsigned  int       addrlo;				//
	unsigned  int       lenhi;				//长度
	unsigned  int       lenlo;				//
	unsigned  char      classname[16];		//次设备名
	unsigned  char      name[16];			//主设备名
	unsigned  int       user_type;          //用户类型
	unsigned  int       keydata;            //关键数据，要求量产不丢失
	unsigned  int       ro;                 //读写属性
	unsigned  char      reserved[68];		//保留数据，匹配分区信息128字节
}__attribute__ ((packed))PARTITION;

/* mbr information */
typedef struct sunxi_mbr
{
	unsigned  int       crc32;				        // crc 1k - 4
	unsigned  int       version;			        // 版本信息， 0x00000100
	unsigned  char 	    magic[8];			        //"softw411"
	unsigned  int 	    copy;				        //分数
	unsigned  int 	    index;				        //第几个MBR备份
	unsigned  int       PartCount;			        //分区个数
	unsigned  int       stamp[1];					//对齐
	PARTITION           array[MBR_MAX_PART_COUNT];	//
	unsigned  char      res[MBR_RESERVED];
}__attribute__ ((packed)) MBR;

typedef struct tag_one_part_info
{
	unsigned  char      name[16];           //所烧写分区的主设备名
	unsigned  int       addrhi;             //所烧写分区的高地址，扇区单位
	unsigned  int       addrlo;             //所烧写分区的低地址，扇区单位
	unsigned  int       lenhi;				//所烧写分区的长度，高32位，扇区单位
	unsigned  int       lenlo;				//所烧写分区的长度，低32位，扇区单位
	unsigned  char      dl_filename[16];    //所烧写分区的文件名称，长度固定16字节
	unsigned  char      vf_filename[16];    //所烧写分区的校验文件名称，长度固定16字节
	unsigned  int       encrypt;            //所烧写分区的数据是否进行加密 0:加密   1：不加密
	unsigned  int       verify;             //所烧写分区的数据是否进行校验 0:不校验 1：校验
}__attribute__ ((packed)) dl_one_part_info;

//分区烧写信息
typedef struct tag_download_info
{
	unsigned  int       crc32;				        		        //crc
	unsigned  int       version;                                    //版本号  0x00000101
	unsigned  char 	    magic[8];			        		        //"softw311"
	unsigned  int       download_count;             		        //需要烧写的分区个数
	unsigned  int       stamp[3];									//对齐
	dl_one_part_info	one_part_info[MBR_MAX_PART_COUNT];	//烧写分区的信息
	unsigned  char      res[DL_RESERVED];
}
__attribute__ ((packed)) download_info;


//标准分区信息
#pragma pack(push, 1)
typedef struct tag_part_info_stand
{
	char 				indicator;			//表示该分区是否是活动分区
	char				start_head;			//分区开始的磁头
	short				start_sector:6;		//分区开始的扇区
	short				start_cylinder:10;	//分区开始的柱面
	char				part_type;			//分区类型			00H:没有指明  01H:DOS12	02H:xenix	04H:DOS16 05H:扩展分区	06H:FAT16
											//					07H:NTFS	  0BH:FAT32
	char				end_head;			//分区结束的磁头
	short				end_sector:6;		//分区结束的扇区
	short				end_cylinder:10;	//分区结束的柱面
	//int					start_sectors;		//起始扇区，前面说的扇区是硬盘的概念，这里的扇区是逻辑概念
	//int					total_sectors;		//分区中的扇区总数
	short				start_sectorl;
	short				start_sectorh;
	short				total_sectorsl;
	short				total_sectorsh;
}
part_info_stand;
#pragma pack(pop)
//标准MBR
#pragma pack(push, 1)
typedef struct tag_mbr_stand
{
	char				mbr[0x89];			//主引导记录
	char				err_info[0x135];	//出错信息
	part_info_stand		part_info[4];		//分区表项
	short				end_flag;			//固定值 0x55aa
}
mbr_stand;
#pragma pack(pop)


//动态数据存放形式
typedef struct tag_dynamic_data_form
{
	char  name[16];
	int   datasize;
	int   usedsize;
}
dynamic_data_form;

typedef struct tag_dynamic_boot_head
{
	char  magic[8];
	int   count;
	int   size;
}
dynamic_boot_head;

#endif	//_WBOOT_MBR_H_

/* end of _WBOOT_MBR_H_ */



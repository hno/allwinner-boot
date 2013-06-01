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
#include "fspart.h"

struct __FSYS_PDOPS     vBootFsPDops;
extern __fsys_part_t    *part_h_alloc(void);

#define         MAX_BOOT_PART   MAX_PARTS_NUM

extern  __fsys_part_t  part_array[MAX_PARTS_NUM];
//__fsys_part_t  *bootpart_array[MAX_BOOT_PART];

#define FSYS_CACHE_MAXNUM		0x08
#define FSYS_SECTOR_SIZE        512

char    BOOTFSMBR_buf[MBR_SIZE];

typedef struct tag_CRC32_DATA
{
	unsigned int CRC;				//int的大小是32位
	unsigned int CRC_32_Tbl[256];	//用来保存码表
}CRC32_DATA_t;

extern  FS_PART_OPTS_t 	pCurPartOpts;
/*
************************************************************************************************************
*
*                                             _calc_crc32
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
static __u32 _calc_crc32(void * buffer, __u32 length)
{
	__u32 i, j;
	CRC32_DATA_t crc32;		//
	__u32 CRC32 = 0xffffffff; //设置初始值

	crc32.CRC = 0;

	for( i = 0; i < 256; ++i)//用++i以提高效率
	{
		crc32.CRC = i;
		for( j = 0; j < 8 ; ++j)
		{
			//这个循环实际上就是用"计算法"来求取CRC的校验码
			if(crc32.CRC & 1)
				crc32.CRC = (crc32.CRC >> 1) ^ 0xEDB88320;
			else //0xEDB88320就是CRC-32多项表达式的值
				crc32.CRC >>= 1;
		}
		crc32.CRC_32_Tbl[i] = crc32.CRC;
	}

	CRC32 = 0xffffffff; //设置初始值
    for( i = 0; i < length; ++i)
    {
        CRC32 = crc32.CRC_32_Tbl[(CRC32^((unsigned char*)buffer)[i]) & 0xff] ^ (CRC32>>8);
    }

    //return CRC32;
	return CRC32^0xffffffff;
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
int FS_getpart_info(const char *class_name, const char *name, part_info_t *part_info)
{
    MBR        *mbr  = (MBR*)BOOTFSMBR_buf;
    PARTITION  *pe   = mbr->array;
    __u32      i;
    long long  len, addr;

    for(i=0; i<mbr->PartCount && i < MAX_BOOT_PART; i++, pe++)
    {
        /* now just scan out the 'disk' classname partition */
        if(strcmp((const char *)pe->classname, class_name))
        {
            continue;
        }
        if(strcmp((const char *)pe->name, name))
        {
            continue;
        }

        addr = pe->addrhi;
        addr = (addr << 32) | pe->addrlo;
        len  = pe->lenhi;
        len  = (len << 32) | pe->lenlo;

        part_info->addr = addr;
        part_info->len  = len;

        return FS_OK;
    }

    return FS_FAIL;
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
int FS_getpart_count(int part_attribute)
{
	MBR        *mbr  = (MBR*)BOOTFSMBR_buf;
	int		    i;
	int			count = 2;

	if(part_attribute == 1)
	{
		for(i=1;i<mbr->PartCount-1;i++)
		{
			if(mbr->array[i].ro)
			{
				count ++;
			}
		}
	}
	else if(part_attribute == 2)
	{
		count = 0;
		for(i=0;i<mbr->PartCount;i++)
		{
			PARTITION* pe = &mbr->array[i];

			if((!strcmp((const char *)pe->classname, "DISK")) || (!strcmp((const char *)pe->classname, "disk")))
        	{
            	count ++;
        	}
        }
	}
	else
	{
		count = mbr->PartCount;
	}
	return count;
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
int FS_getpart_start(int part_index)
{
	MBR        *mbr  = (MBR*)BOOTFSMBR_buf;
	int         i;
	int			count = 0;

	if(part_index >= mbr->PartCount)
	{
		return -1;
	}

	if(!part_index)
	{
		for(i=0;i<mbr->PartCount;i++)
		{
			PARTITION* pe = &mbr->array[i];

			if((!strcmp((const char *)pe->classname, "DISK")) || (!strcmp((const char *)pe->classname, "disk")))
        	{
            	return mbr->array[i].addrlo;
        	}
        }
		return -1;
	}
	for(i=1;i<mbr->PartCount;i++)
	{
		if(mbr->array[i].ro)
		{
			count ++;
		}
		if(part_index == count)
		{
			break;
		}
	}
	if(i == mbr->PartCount)
	{
		i --;
	}

	return mbr->array[i].addrlo;
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
int FS_getpart_capacity(int part_index)
{
	MBR        *mbr  = (MBR*)BOOTFSMBR_buf;
	int         i;
	int			count = 0;

	if(part_index >= mbr->PartCount)
	{
		return -1;
	}
	if(!part_index)
	{
		for(i=0;i<mbr->PartCount;i++)
		{
			PARTITION* pe = &mbr->array[i];

			if((!strcmp((const char *)pe->classname, "DISK")) || (!strcmp((const char *)pe->classname, "disk")))
        	{
            	return mbr->array[i].lenlo;;
        	}
        }
		return -1;
	}
	for(i=1;i<mbr->PartCount;i++)
	{
		if(mbr->array[i].ro)
		{
			count ++;
		}
		if(part_index == count)
		{
			break;
		}
	}
	if(i == mbr->PartCount)
	{
		i --;
	}

	return mbr->array[i].lenlo;
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
int bootfs_scan_part(void)
{
    int i, part_index = 0;
    MBR    *mbr;

    pCurPartOpts.Read(0, MBR_SIZE >> 9 , BOOTFSMBR_buf);

    mbr = (MBR*)BOOTFSMBR_buf;
    if(!strncmp((const char*)mbr->magic, MBR_MAGIC, 8))
    {
        int crc = 0;

        crc = _calc_crc32((void *)&mbr->version, MBR_SIZE-4);
        if(crc == mbr->crc32)
        {
            if(mbr->PartCount < 32)
            {
                PARTITION* pe = mbr->array;

                for(i=0; i<mbr->PartCount && i < MAX_BOOT_PART; i++, pe++)
                {
                    long long  len, addr;

                    /* now just scan out the 'disk' classname partition */
                    if((strcmp((const char *)pe->classname, "DISK")) && (strcmp((const char *)pe->classname, "disk")))
                    {
                        continue;
                    }
                    addr = pe->addrhi;
                    addr = (addr << 32) | pe->addrlo;
                    len  = pe->lenhi;
                    len  = (len << 32) | pe->lenlo;

					if(!len)
					{
#if SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_SD_CARD
						__s32  ret;
						mbr_stand    *smbr;
						char   tmp_data[512];
						int    start, startl;
						int    size, sizel;

						ret = eGon2_block_phyread(0, 1 , tmp_data);
						if(ret)
						{
							eGon2_printf("card read sector 0 fail\n");
							return -1;
						}

						smbr = (mbr_stand*)tmp_data;

					    start = (smbr->part_info[0].start_sectorh) & 0xffff;
					    startl = (smbr->part_info[0].start_sectorl) & 0xffff;
					    start <<= 16;
					    start |= startl;
					    addr   = start - 40960;

					    size = (smbr->part_info[0].total_sectorsh) & 0xffff;
					    sizel = (smbr->part_info[0].total_sectorsl) & 0xffff;
					    size <<= 16;
					    size |= sizel;
					    len   = size;
#elif SYS_STORAGE_MEDIA_TYPE == SYS_STORAGE_MEDIA_NAND
						len = eGon2_block_size() - addr;
						pe->lenlo = len & 0xffffffff;
#endif
//						eGon2_printf("flash_size = %d, addr = %d, len = %d\n", flash_size, addr, len);
					}
                    //alloc part item and link image cache with it
//                	parray[part_index] = part_h_alloc();
//                	if ( parray[part_index] == NULL )
//                	{
//                		return part_index;
//                	}
//                    parray[part_index]->hPDPrivate  = NULL;
//                    parray[part_index]->disk_symble = 0;
//                    parray[part_index]->start       = addr;
//                    parray[part_index]->len         = len;
//                    parray[part_index]->pdops       = vBootFsPDops;
					part_array[part_index].hPDPrivate  = NULL;
                    part_array[part_index].disk_symble = 0;
                    part_array[part_index].start       = addr;
                    part_array[part_index].len         = len;
                    part_array[part_index].pdops       = vBootFsPDops;

                    part_index ++;
                }
            }
        }
    }
    else
    {
    	eGon2_printf("mbr magic %s is not correct\n", mbr->magic);
    }

	return part_index;
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
__s32 boot_part_init(void)
{
    //int i;

    pCurPartOpts.Init();

//    for(i=0; i<MAX_BOOT_PART; i++)
//    {
//        bootpart_array[i] = NULL;
//    }

    return bootfs_scan_part();
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
__s32 boot_part_exit(void)
{
    pCurPartOpts.Exit();

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
__s32 boot_part_read(void * pData, __u32 Sector, __u32 N, __fsys_part_t * pPart)
{
    if(!pCurPartOpts.Read(pPart->start + Sector, N , pData))
    {
        return N;
    }
    else
    {
        return -1;
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
__s32 boot_part_write(void * pData, __u32 Sector, __u32 N, __fsys_part_t * pPart)
{
    if(!pCurPartOpts.Write(pPart->start + Sector, N , pData))
    {
        return N;
    }
    else
    {
        return -1;
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
__s32 boot_part_ioctl(__fsys_part_t * pPart, __s32 Cmd, __s32 Aux, void * pBuffer)
{
    if ( Cmd == FSYS_PART_CMD_GET_PARTSIZE )
    {
        *(long long *)pBuffer = (long long)pPart->len;
    }

    return 0;
}

struct __FSYS_PDOPS vBootFsPDops =
{
   	boot_part_read,
   	boot_part_write,
   	boot_part_ioctl,
};



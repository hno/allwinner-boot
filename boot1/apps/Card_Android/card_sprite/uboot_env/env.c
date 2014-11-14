/*
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * (C) Copyright 2011
 * Texas Instruments, <www.ti.com>
 * Author: Vikram Pandita <vikram.pandita@ti.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include  "update_data.h"
#include  "update_data/card_sprite_flash/card_sprite_flash.h"
#include  "string.h"

#define  ENV_DATA_DRAM_ADDRESS         0x58000000
#define  ENV_FLASH_DRAM_ADDRESS		   0x56000000
#define  ENV_SIZE                      (128 * 1024)
#define  PRIVATE_FLASH_DRAM_ADDRESS	   0x5E000000

char  *env_data_dram_base;
char  *env_data_dram_addr;
int    env_data_dram_size;

int    env_flash_flash_start;
char  *env_flash_dram_base;

int    env_exist = 0;
char   mac_addr_store[32];

int    private_flash_flash_size;
char  *private_flash_dram_base;

int    private_exist = 0;
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
/*
int env_fetch_from_boot1(void)
{
	char *tmp_boot1 = NULL;
	int   ret = 0;

	tmp_boot1 = wBoot_malloc(512 * 1024);
	if(!tmp_boot1)
	{
		__inf("not enough memory to read boot1!\n");

		return 0;
	}
	else
	{
		if(Nand_Load_Boot1(tmp_boot1, 512 * 1024))
		{
			__inf("the boot1 in flash is bad!\n");

			return 0;
		}
		else
		{
			boot1_file_head_t  *tmp_boot1_head = (boot1_file_head_t *)tmp_boot1;
			char 			   *base = (char *)tmp_boot1_head->prvt_head.script_buf;
			int   			   i;
			dynamic_data_form  *dform;
			dynamic_boot_head  *dhead;

			dhead = (dynamic_boot_head *)base;
			if(strncmp(dhead->magic, "dynamic", sizeof("dynamic")))
			{
				__inf("dynamic data magic error\n");

				return 0;
			}
			base += sizeof(dynamic_boot_head);
			for(i=0;i<dhead->count;i++)
			{
				dform = (dynamic_data_form *)base;
				if(!strncmp(dform->name, "MAC", sizeof("MAC")))
				{
					char *buf0 = env_flash_dram_base+4;
					char *buf1 = base + sizeof(dynamic_data_form);
					int   j, k, l;

					__inf("dynamic data form1 find\n");
					strcpy(env_flash_dram_base, "mac=");

					k = 0;
					l = 0;
					for(j=0;j<5;j++)
					{
						buf0[k++] = buf1[l++];
						buf0[k++] = buf1[l++];
						buf0[k++] = ':';
					}
					buf0[k++] = buf1[l++];
					buf0[k++] = buf1[l++];
					buf0[k]   = '\0';
					ret = 1;
					env_exist = 2;

					break;
				}
				else if(!strncmp(dform->name, "mac_addr", sizeof("mac_addr")))
				{
					__inf("dynamic data form2 find\n");
					__inf("usedsize=%d\n", dform->usedsize>>2);

					{
						char *tm = base + sizeof(dynamic_data_form);
						int   p;

						for(p=0;p<4;p++)
						{
							__inf("%c %c %c %c %c %c %c %c\n", *(tm + 0), *(tm + 1), *(tm + 2), *(tm + 3), *(tm + 4), *(tm + 5), *(tm + 6), *(tm + 7));
							tm += 8;
						}
						__inf("\n");
					}

					memcpy(env_flash_dram_base, base + sizeof(dynamic_data_form), dform->usedsize);
					ret = 1;
					env_exist = 2;
				}
				base += dform->usedsize;
			}
		}
		wBoot_free(tmp_boot1);
	}

	return ret;
}
*/
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
int private_fetch_from_flash(void)
{
	char mbr_buf[MBR_SIZE];
	MBR  *mbr_info;
	int  crc, i;
	int  size, start;
	int  ret = -1;

	memset(mac_addr_store, 0, 32);
	env_flash_dram_base = (char *)ENV_FLASH_DRAM_ADDRESS;
	private_flash_dram_base = (char *)PRIVATE_FLASH_DRAM_ADDRESS;
#if 1
	if(sprite_flash_init())
	{
		__inf("update flash env err: flash init\n");

		return -1;
	}
	if(sprite_flash_read(0, MBR_SIZE/512, mbr_buf))
	{
		__inf("update flash env err: read flash error\n");

		goto update_flash_env_err;
	}
	for(i=0;i<4;i++)
	{
		mbr_info = (MBR *)(mbr_buf + i * MBR_SIZE);
		crc = calc_crc32((void *)&mbr_info->version, sizeof(MBR) - 4);
		if(crc == mbr_info->crc32)
		{
			break;
		}
	}
	if(4 == i)
	{
		ret = 0;
		__inf("update flash env err: cant find good flash mbr\n");

		goto update_flash_env_err;
	}
	for(i=0;i<mbr_info->PartCount;i++)
	{
		if(!strcmp("env", (const char *)mbr_info->array[i].name))
		{
			__inf("env part found\n");
			start = mbr_info->array[i].addrlo;
			size  = mbr_info->array[i].lenlo;
			memset(env_flash_dram_base, 0, size<<9);
			if(sprite_flash_read(start, size, env_flash_dram_base))
			{
				__inf("update flash env err: read env data error\n");

				goto update_flash_env_err;
			}
			env_exist = 1;
		}
		else if(!strcmp("private", (const char *)mbr_info->array[i].name))
		{
			__inf("private part found\n");
			start = mbr_info->array[i].addrlo;
			size  = mbr_info->array[i].lenlo;
			memset(private_flash_dram_base, 0, size<<9);
			if(sprite_flash_read(start, size, private_flash_dram_base))
			{
				__inf("update flash private err: read private data error\n");

				goto update_flash_env_err;
			}
			private_flash_flash_size = size<<9;
			private_exist = 1;
		}
	}
	ret = 0;

update_flash_env_err:
	sprite_flash_exit(0);
#endif
//	if(!env_exist)		//如果在旧的环境变量中没有找到动态数据，则去boot1中寻找
//	{
//		ret = env_fetch_from_boot1();
//	}

	return ret;
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
int private_date_restore(int part_size)
{
	__inf("private part status=%d\n", private_exist);
	if(private_exist)
	{
		if(private_flash_flash_size <= part_size)
		{
			if(!update_flash_write_ext(private_flash_dram_base, private_flash_flash_size))
			{
				return 0;
			}
			else
			{
				__inf("update flash private err: write private data error\n");
			}
		}
		else
		{
			__inf("update flash private err: data size %d is larger than part size %d\n", private_flash_flash_size, part_size);
		}
	}
	else
	{
		__inf("sprite update info: no part named private need to be restore\n");
	}

	return -1;
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
int env_init(int flash_start)
{
	env_flash_flash_start = flash_start;
	env_data_dram_base = (char *)ENV_DATA_DRAM_ADDRESS;
	env_data_dram_addr = env_data_dram_base;
	env_data_dram_size = 0;

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
__s32 env_dram_write(void *pbuf, __u32 len)
{
	memcpy(env_data_dram_addr, pbuf, len);
	env_data_dram_addr += len;
	env_data_dram_size += len;

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
__s32 env_length_get(const char *str)
{
	int i = 0;
	char *buf = (char *)str;

	do
	{
		if(!buf[i])
		{
			if(!buf[i + 1])
			{
				return i;
			}
		}
		i ++;
	}
	while(i <= 512 * 1024);

	return i + 1;
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
char *env_read(char *env_buf, const char *str)
{
	char *buf;
	int  crc, length;
	int  env_len, str_len;
	int  one_env_len;

	//获取原来的数据
	buf = (char *)env_buf;
	//获取env数据的有效长度
	env_len = env_length_get(buf);
	//校验crc
	crc = calc_crc32((void *)(buf + 4), ENV_SIZE - 4);
	if(crc != *(int *)buf)
	{
		__msg("*********************crc error*********************\n");
		return 0;
	}
	str_len = strlen(str);
	buf += 4;
	length = 4;
	//依次比较字符串
	while(length < env_len)
	{
		one_env_len = strlen(buf);
		if(!strncmp(buf, str, str_len))
		{
			return buf;
		}
		length += one_env_len + 1;
		buf += one_env_len + 1;
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
int env_write(char *env_buf, char *databuf, const char *str)
{
	char *buf, *buf_next;
	int  i;
	int  crc, length;
	int  env_len, str_len;
	int  one_env_len;

	//获取原来的数据
	buf = (char *)env_buf + 4;
	//获取env数据的有效长度
	env_len = env_length_get(env_buf);
	//校验crc
	crc = calc_crc32((void *)buf, ENV_SIZE - 4);
	if(crc != *(int *)env_buf)
	{
		__msg("*********************crc error*********************\n");
		return -1;
	}
	str_len = strlen(str);
	length = 4;
	//依次比较字符串
	while(length < env_len)
	{
		one_env_len = strlen(buf);
		if(!strncmp(buf, str, str_len))
		{
			if(buf[str_len] == '=')
			{
				//找到原有的环境变量
				buf_next = buf + one_env_len + 1;
				//清除原有的环境变量
				for(i=0;i<env_len - length - one_env_len;i++)
				{
					buf[i] = buf_next[i];
				}
				buf += i;
				for(i=0;i<one_env_len;i++)
				{
					buf[i] = 0;
				}
				break;
			}
		}
		length += one_env_len + 1;
		buf += one_env_len + 1;
	}
	buf = (char *)env_buf + 4;
	env_len = env_length_get(buf);
	strcpy(buf + env_len + 1, databuf);
	//重新计算环境变量crc
	crc = calc_crc32((void *)buf, ENV_SIZE - 4);
	*(int *)env_buf = crc;

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
static void env_restore(const char *env_name)
{
	char *env_src;

	env_src = env_read(env_flash_dram_base, env_name);
	if(!env_src)
	{
		__inf("env error: no env need be rewrite\n");
	}
	else
	{
		memset(mac_addr_store, 0, 32);
		strcpy(mac_addr_store, env_src);
		env_write(env_data_dram_base, env_src, env_name);
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
int env_exit(void)
{
	char  env_name[48];
	int   env_count;
	int   i, tmp;

	__inf("env exit = %d\n", env_exist);
	if(env_exist == 1)
	{
		env_count = wBoot_script_parser_subkey_count("env_restore");
		if(env_count <= 0)
		{
			env_restore("mac");
		}
		else
		{
			char env_sub_name[32];
			i = 0;

			do
			{
				tmp = i;

				memset(env_name, 0, 48);
				memset(env_sub_name, 0, 32);
				strcpy(env_sub_name, "env_sub00");
				env_sub_name[7] = '0' + (tmp/10);
				tmp /= 10;
				env_sub_name[8] = '0' + (tmp);
				wBoot_script_parser_fetch("env_restore", env_sub_name, (int *)env_name, 48/4);
				env_restore(env_name);
				i ++;
			}
			while(i < env_count);
		}
	}
/*
	else if(env_exist == 2)
	{
		__inf("env_flash_dram_base = %s\n", env_flash_dram_base);
		strcpy(mac_addr_store, env_flash_dram_base);
		__inf("mac_addr_store = %s\n", mac_addr_store);
		env_write(env_data_dram_base, env_flash_dram_base, "mac");
	}
*/
	sprite_flash_write(env_flash_flash_start, env_data_dram_size>>9, env_data_dram_base);

	return 0;
}


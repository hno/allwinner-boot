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
#include  "sparse.h"
#include  "sparse_format.h"

#define  SPARSE_HEADER_MAJOR_VER 1
#define  ANDROID_FORMAT_DRAM_ADDRESS         0x48000000

int    android_format_flash_start;
char  *android_format_dram_base;
char  *android_format_dram_addr;
int    android_format_size;
/*
************************************************************************************************************
*
*                                             unsparse_probe
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
int unsparse_probe(char *source, __u32 length, __u32 flash_start)
{
	sparse_header_t *header = (sparse_header_t*) source;

	if (header->magic != SPARSE_HEADER_MAGIC)
	{
		__inf("sparse: bad magic\n");

		return -1;
	}

	if ((header->major_version != SPARSE_HEADER_MAJOR_VER) ||
	    (header->file_hdr_sz != sizeof(sparse_header_t)) ||
	    (header->chunk_hdr_sz != sizeof(chunk_header_t)))
	{
		__inf("sparse: incompatible format\n");

		return -1;
	}
	android_format_dram_base = (char *)ANDROID_FORMAT_DRAM_ADDRESS;
	android_format_flash_start = flash_start;
	android_format_dram_addr = android_format_dram_base;
	android_format_size      = 0;

	return 0;
}
/*
************************************************************************************************************
*
*                                             DRAM_Write
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
int  unsparse_dram_write(void *pbuf, int length)
{
	memcpy(android_format_dram_addr, pbuf, length);
	android_format_dram_addr += length;
	android_format_size += length;

	return 0;
}
/*
************************************************************************************************************
*
*                                             unsparse_deal
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
int unsparse_deal(void)
{
	sparse_header_t *header;
	unsigned i, len = 0;
	char *source;
	chunk_header_t *chunk;
	int  flash_start;
	int  ratio0, ratio1;

	flash_start = android_format_flash_start;
	header = (sparse_header_t *)android_format_dram_base;
	source = android_format_dram_base + header->file_hdr_sz;

	ratio0 = ratio1= 0;
	__inf("sparse ratio=%d\n", ratio0);
	for (i=0; i < header->total_chunks; i++)
	{
		chunk = (chunk_header_t *)source;

		/* move to next chunk */
		source += sizeof(chunk_header_t);
		len = chunk->chunk_sz * header->blk_sz;

		switch (chunk->chunk_type)
		{
			case CHUNK_TYPE_RAW:

				if (chunk->total_sz != (len + sizeof(chunk_header_t)))
				{
					__inf("sparse: bad chunk size for chunk %d, type Raw\n", i);

					return -1;
				}

				if(sprite_flash_write(flash_start, len>>9, source))
				{
					__inf("sparse: flash write failed\n");

					return -1;
				}
				flash_start += (len>>9);
				source += len;

				break;

			case CHUNK_TYPE_DONT_CARE:
				if (chunk->total_sz != sizeof(chunk_header_t))
				{
					__inf("sparse: bogus DONT CARE chunk\n");

					return -1;
				}

				flash_start += (len>>9);

				break;

			default:
				__inf("sparse: unknown chunk ID %x\n", chunk->chunk_type);

				return -1;
		}
		ratio1 = i*10 / header->total_chunks;
		if(ratio0 != ratio1)
		{
			ratio0 = ratio1;
			__inf("sparse ratio=%d\n", ratio0);
		}

	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             unsparse_checksum
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
__u32 unsparse_checksum(void)
{
	return verify_sum(android_format_dram_base, android_format_size);
}



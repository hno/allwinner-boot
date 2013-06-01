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
#include  "card_sprite_i.h"

#define   TEST_BLK_BYTES      (512 * 1024)

#define   CARD_SPRITE_START    		0
#define   CARD_SPRITE_FLASH_INFO    3
#define   CARD_SPRITE_GET_MEM  		6
#define   CARD_SPRITE_OPEN_IMG 		9
#define   CARD_SPRITE_GET_MBR  		12
#define   CARD_SPRITE_GET_MAP  		15
#define   CARD_SPRITE_DOWN_PART     80
#define   CARD_SPRITE_DOWN_BOOT1    87
#define   CARD_SPRITE_DOWN_BOOT0    95
#define   CARD_SPRITE_FINISH   		100

//int       dynamic_data = 0;
int		  private_type = 0;
__hdle    progressbar_hd;

extern    int		android_format;

static  void  sprite_show(int ratio)
{
	//boot_ui_progressbar_upgrate(progressbar_hd, ratio);
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
int  card_part_info(__u32 *part_start, __u32 *part_size, const char *str)
{
    char   *buffer;
    MBR    *mbr;
    int    i;
    
    buffer=wBoot_malloc(MBR_SIZE);
    if (mbr==NULL)
    {
        __inf("malloc mbr buffer fail!\n");
        return 0;
    }
    if(wBoot_block_read(0, MBR_SIZE, buffer))
    {
    	__inf("read mbr failed\n");
        goto _card_part_info_error;
    	return 0;
    }
    mbr = (MBR *)buffer;
    for(i=0;i<mbr->PartCount;i++)
    {
        __inf("part name  = %s\n", mbr->array[i].name);
    	__inf("part start = %d\n", mbr->array[i].lenlo);
    	__inf("part size  = %d\n", mbr->array[i].addrlo);
        if(!strcmp(str, (char *)mbr->array[i].name))
        {
            *part_start = mbr->array[i].addrlo;
            *part_size  = mbr->array[i].lenlo;
            wBoot_free(buffer);
            return 0;
        }
    }
_card_part_info_error:
    
    wBoot_free(buffer);
    return -1;
}

/*
************************************************************************************************************
*
*                                             card_sprite
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
__s32 card_sprite(void)
{
    HIMAGEITEM  imghd = 0, imgitemhd = 0;
    __u64      item_original_size = 0;
    __u64       item_rest_size = 0;
    __u64		item_verify_size = 0;
    __u32       part_sector, file_sector, flash_sector;
    int         crc, mbr_count, mbr_buf_size;
    __u32		img_start;
    MBR         *tmp_mbr_cfg, *mbr_info;
    download_info  *dl_info  = NULL;
    char        *tmp_mbr_buf = NULL;
    char        *src_buf = NULL, *dest_buf = NULL;
    __u32       actual_buf_addr;
    int         ret = -1;
    int         aver_rage, sprite_ratio, pre_ratio, this_ratio;          //进度标志
    unsigned    i, sprite_type;
    int         s_type;
    __u32       part_size;

	//读取原有的uboot环境变量
	private_fetch_from_flash();
  
    src_buf =  (char *)(0x48000000);
    dest_buf = (char *)(0x4C000000);
    dl_info = (download_info *)wBoot_malloc(sizeof(download_info));
    if(!dl_info)
    {
        __wrn("sprite update error: fail to get memory for download map\n");

        goto _update_error_;
    }
    memset(dl_info, 0, sizeof(download_info));
     /*实现擦除data分区*/
    ret = card_part_info(&img_start, &part_size, "data");
    if(ret)
	{
		__wrn("sprite update error: no data part found\n");
	}
	else
	{
		__u32 tmp_size;
		__u32 tmp_start;

		tmp_start = img_start;
		tmp_size = part_size;
		__inf("data part size=%d\n", tmp_size);
		__inf("begin erase part data\n");
		memset(src_buf, 0xff, 1024 * 1024);
        #if 0
		while(tmp_size >= 1024 * 1024)
		{
			wBoot_block_write(tmp_start, 1024 * 1024/512, src_buf);
			tmp_start += 1024 * 1024/512;
			tmp_size  -= 1024 * 1024/512;
		}
		if(tmp_size)
		{
			wBoot_block_write(tmp_start, tmp_size/512, src_buf);
		}
        #else
        wBoot_block_write(tmp_start, 1024 * 1024/512, src_buf);
        #endif
		__inf("finish erase part data\n");
	}
    
    ret = card_part_info(&img_start, &part_size, "sysrecovery");
	if(ret)
	{
		__wrn("sprite update error: read image start error\n");

        goto _update_error_;
	}
	/*开始对压缩包进行操作*/
//*************************************************************************************
//*************************************************************************************
	__inf("part start = %d\n", img_start);
    imghd = Img_Open(img_start, TEST_BLK_BYTES);
    if(!imghd)       //初始化
    {
        __wrn("sprite update error: fail to open img\n");

        goto _update_error_;
    }
//*************************************************************************************
//*************************************************************************************
    //获取MBR
    imgitemhd = Img_OpenItem(imghd, "12345678", "1234567890___MBR");
    if(!imgitemhd)
    {
        __wrn("sprite update error: fail to open item for mbr\n");
        goto _update_error_;
    }
    //MBR长度
    item_original_size = Img_GetItemSize(imghd, imgitemhd);
    //获取空间用于存放MBR
    if(!item_original_size)
    {
        __wrn("sprite update error: get mbr size fail\n");
        goto _update_error_;
    }
    mbr_buf_size = (int)item_original_size;
    tmp_mbr_buf = (char *)wBoot_malloc((__u32)item_original_size);
    if(!tmp_mbr_buf)
    {
        __wrn("sprite update error: fail to get memory for mbr\n");

        goto _update_error_;
    }
    if(!Img_ReadItemData(imghd, imgitemhd, (void *)tmp_mbr_buf, item_original_size))   //读出MBR数据
    {
        __wrn("sprite update error: fail to read data for mbr\n");

        goto _update_error_;
    }
    Img_CloseItem(imghd, imgitemhd);
    imgitemhd = NULL;
    //检查MBR的合法性
    mbr_count = ((MBR *)tmp_mbr_buf)->copy;
    for(i=0;i<mbr_count;i++)
    {
        tmp_mbr_cfg = (MBR *)(tmp_mbr_buf + i * sizeof(MBR));
        crc = calc_crc32((void *)&tmp_mbr_cfg->version, sizeof(MBR) - 4);
        if(crc != tmp_mbr_cfg->crc32)
        {
            __wrn("sprite update warning: check mbr %d part correct fail\n", i);
            __wrn("now fix it automatically\n");
            //自动修正CRC
            tmp_mbr_cfg->crc32 = crc;
        }
    }
    mbr_info=(MBR *)tmp_mbr_buf;

//*************************************************************************************
//*************************************************************************************
	//获取 DOWNLOAD MAP
	imgitemhd = Img_OpenItem(imghd, "12345678", "1234567890DLINFO");
	if(!imgitemhd)
	{
		__wrn("sprite update error: fail to open item for download map\n");

        goto _update_error_;
	}
	//DOWNLOAD MAP 长度
	item_original_size = Img_GetItemSize(imghd, imgitemhd);
	if(!item_original_size)
    {
        __wrn("sprite update error: get download map size fail\n");
        goto _update_error_;
    }
	if(!Img_ReadItemData(imghd, imgitemhd, (void *)dl_info, item_original_size))   //读出 DOWNLOAD MAP 数据
    {
        __wrn("sprite update error: fail to read data for download map\n");

        goto _update_error_;
    }
    Img_CloseItem(imghd, imgitemhd);
    imgitemhd = NULL;
    //检查 DOWNLOAD MAP的合法性
	crc = calc_crc32(&dl_info->version, sizeof(download_info) - 4);
	if(crc != dl_info->crc32)
	{
		__wrn("sprite update error: download map file is not correct\n");

		goto _update_error_;
	}
	sprite_show(CARD_SPRITE_GET_MAP);
/*****************************************************************************
*
*   根据烧写的map表，找到每个分区应该写的位置
*
*****************************************************************************/
	for(i=0;i<dl_info->download_count;i++)
	{
		__inf("dl name = %s\n", dl_info->one_part_info[i].name);
	    imgitemhd = Img_OpenItem(imghd, "RFSFAT16", (char *)dl_info->one_part_info[i].dl_filename);
	    if(!imgitemhd)
	    {
	        __wrn("sprite update error: fail to open part file %s\n", dl_info->one_part_info[i].dl_filename);

	        goto _update_error_;
	    }
	    private_type = 0;
	    item_original_size = Img_GetItemSize(imghd, imgitemhd);
   
	    __inf("item_original_size high =%d\n", (__u32)((item_original_size>>32)&0xffffffff));
        __inf("item_original_size low =%d\n", (__u32)(item_original_size));
	    if(!item_original_size)
	    {
	        __wrn("sprite update error: get part file %s size failed\n", dl_info->one_part_info[i].dl_filename);

	        goto _update_error_;
	    }
        if(!strcmp( (const char *)dl_info->one_part_info[i].name,"sysrecovery"))
	    {
	    	__inf("THIS_IMG_SELF_00 do not need to rewrite\n");
	    	continue;
	    }
	    if(!strcmp((char *)dl_info->one_part_info[i].name, "env"))
	    {
	    	private_type = 1;
	    }
	    else if(!strcmp((char *)dl_info->one_part_info[i].name, "private"))
	    {
	    	continue;
	    }
	    else if(!strcmp((char *)dl_info->one_part_info[i].name, "UDISK"))
	    {
	    	__inf("UDISK do not need to rewrite\n");
	    }
	    //__wrn("get part file %s size = %d\n", dl_info->one_part_info[i].dl_filename, item_original_size);
	    item_rest_size = item_original_size;
	    //检查文件大小是否小于等于分区大小
		file_sector = (__u32)(item_original_size>>9);
		part_sector = dl_info->one_part_info[i].lenlo;
		if(file_sector > part_sector)
		{
			__wrn("sprite update error: file size is %d, part size is %d, cant burn part %s\n", file_sector, part_sector, dl_info->one_part_info[i].dl_filename);

			goto _update_error_;
		}
	    update_flash_open(dl_info->one_part_info[i].addrlo, dl_info->one_part_info[i].addrhi);
	    //初始化解密函数
	    init_code(dl_info->one_part_info[i].encrypt);
	    while(item_rest_size >= TEST_BLK_BYTES)      //当超过32k的时候
	    {
	    	if(!Img_ReadItemData(imghd, imgitemhd, (void *)src_buf, TEST_BLK_BYTES))   //读出32k数据
	        {
	            __wrn("sprite update error: fail to read data from %s\n", dl_info->one_part_info[i].dl_filename);

	            goto _update_error_;
	        }
	        item_rest_size -= TEST_BLK_BYTES;
	        decode((__u32)src_buf, (__u32)dest_buf, TEST_BLK_BYTES, &actual_buf_addr);
	        if(update_flash_write((void *)actual_buf_addr, TEST_BLK_BYTES))
	        {
	        	__wrn("sprite update error: fail to write data in %s\n", dl_info->one_part_info[i].dl_filename);

	            goto _update_error_;
	        }
	        this_ratio = (int)(((item_original_size>>9) - (item_rest_size>>9))*aver_rage)/((__u32)(item_original_size>>9));
	        if(this_ratio)
	        {
	        	if(pre_ratio != this_ratio)
	        	{
	        		sprite_show(sprite_ratio + this_ratio);
	    			pre_ratio = this_ratio;
	    			__inf("this ratio = %d\n", this_ratio);
	    		}
	    	}
	    }
	    if(item_rest_size)
	    {
	        if(!Img_ReadItemData(imghd, imgitemhd, (void *)src_buf, item_rest_size))   //读出32k数据
	        {
	            __wrn("sprite update error: fail to read data from %s\n", dl_info->one_part_info[i].dl_filename);

	            goto _update_error_;
	        }
	        decode((__u32)src_buf, (__u32)dest_buf, (__u32)item_rest_size, &actual_buf_addr);
	        if(update_flash_write((void *)actual_buf_addr, (__u32)item_rest_size))
	        {
	        	__wrn("sprite update error: fail to write last data in %s\n", dl_info->one_part_info[i].dl_filename);

	            goto _update_error_;
	        }
	    }
	    exit_code();
	    __inf("data deal finish\n");
	    update_force_to_flash();
	    sprite_ratio += aver_rage;
	    __msg("sprite_ratio = %d\n", sprite_ratio);
	    sprite_show(sprite_ratio);
	    //此时开始做校验
	    if(private_type)
	    {
	    	continue;
	    }
	    if(dl_info->one_part_info[i].verify)
	    {
	    	imgitemhd = Img_OpenItem(imghd, "RFSFAT16", (char *)dl_info->one_part_info[i].vf_filename);
	    	if(!imgitemhd)
	    	{
	        	__inf("sprite update warning: cant open verify file %s\n", dl_info->one_part_info[i].vf_filename);
	    	}
	    	else
	    	{
	    		item_verify_size = Img_GetItemSize(imghd, imgitemhd);
	    		if(!item_verify_size)
			    {
			        __inf("sprite update warning: cant get verify file %s size\n", dl_info->one_part_info[i].vf_filename);
				}
				else
				{
					init_code(0);
					if(!Img_ReadItemData(imghd, imgitemhd, (void *)src_buf, item_verify_size))   //读出数据
			        {
			            __inf("sprite update warning: fail to read data from %s\n", dl_info->one_part_info[i].vf_filename);
			        }
			        else
			        {
			        	__u32 check_sum = *(__u32 *)src_buf;
			        	__u32 sum = 0;

						if(android_format == -1)
						{
				        	//读出flash上的数据
				        	__inf("normal type verify\n");
				        	item_rest_size = item_original_size;
				        	update_flash_open(dl_info->one_part_info[i].addrlo, dl_info->one_part_info[i].addrhi);
				        	while(item_rest_size >= 512 * 1024)
				        	{
				        		if(update_flash_read_ext(dest_buf, 512 * 1024))
				        		{
				        			__inf("sprite update warning: fail to read data in %s\n", dl_info->one_part_info[i].vf_filename);

				        			goto __download_part_data__;
				        		}
								sum += verify_sum(dest_buf, 512 * 1024);
								item_rest_size -= 512 * 1024;
				        	}
				        	if(item_rest_size)
				        	{
				        		if(update_flash_read_ext(dest_buf, item_rest_size))
				        		{
				        			__inf("sprite update warning: fail to read data in %s\n", dl_info->one_part_info[i].vf_filename);

				        			goto __download_part_data__;
				        		}
				        		sum += verify_sum(dest_buf, (__u32)item_rest_size);
				        	}
				        }
				        else
				        {
				        	__inf("sparse type verify\n");
				        	sum = unsparse_checksum();
				        }
				        __inf("pc check sum=%x, local check sum=%x\n", check_sum, sum);
				        if(sum != check_sum)
			        	{
			        		__wrn("sprite update error: checksum is error %s\n", dl_info->one_part_info[i].dl_filename);

			        		goto _update_error_;
			        	}
				    }
				}
	    	}
	    }
	    else
	    {
	    	__inf("part %s not need verify\n", dl_info->one_part_info[i].dl_filename);
	    }
__download_part_data__:
		;
	}
	sprite_show(CARD_SPRITE_DOWN_PART);
/*****************************************************************************
*
*   还原保留的数据
*
*   private分区数据
*
*
*****************************************************************************/
	__inf("try to deal private part\n");
	for(i=0;i<mbr_info->PartCount;i++)
	{
		if(!strcmp((const char *)mbr_info->array[i].name, "private"))
		{
			__u32 private_start, private_size;

			private_start = mbr_info->array[i].addrlo;
			private_size  = mbr_info->array[i].lenlo;

			update_flash_open(private_start, 0);
			if(private_date_restore(private_size<<9))
			{
				__wrn("sprite update error: fail to write private date\n");
			}
			update_flash_close();

        	break;
    	}
	}
	__inf("dela private part finish\n");
/*****************************************************************************
*
*   关闭imghd和nand句柄
*
*
*****************************************************************************/
    Img_Close(imghd);

    ret = 0;
    
    if(src_buf)
    {
        wBoot_free(src_buf);
    }
//    if(buf0)
//    {
//        sprite_free(buf0);
//    }
//    if(buf1)
//    {
//        sprite_free(buf1);
//    }
    if(tmp_mbr_buf)
    {
        wBoot_free(tmp_mbr_buf);
    }
    if(sprite_type)
    {
        update_flash_exit(1);
    }
    sprite_show(CARD_SPRITE_FINISH);
    return ret;
    
_update_error_:
    
    ret =-1;
    __wrn("sprite update error: current card sprite failed\n");
    __wrn("now hold the machine\n");
    return ret;

}


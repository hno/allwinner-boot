/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: Board.c
*
* Author 		: javen
*
* Description 	: Æô¶¯Òýµ¼
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/

#include "include.h"

part_info_t           part_info;

static int usb_decect_key(int keyvalue)
{
	__s32 ret1, ret2;
	__s32 key_high, key_low;

    ret1 = wBoot_script_parser_fetch("private_key", "key_max", &key_high, 1);
	ret2 = wBoot_script_parser_fetch("private_key", "key_min", &key_low, 1);
	if(ret1)
	{
		__inf("unable to find private_key key_max value\n");
		return -1;
	}
	if(ret2)
	{
		__inf("unable to find private_key key_min value\n");
		return -1;
	}
	__inf("key valye %d, usb key high %d, low %d\n", keyvalue, key_high, key_low);
	if((keyvalue >= key_low) && (keyvalue <= key_high))
	{
		__inf("key found, try private mode\n");
		return 0;
	}

	return -1;
}


int check_private_part(int keyvalue)
{
	int ret;

	ret = wBoot_part_info("DISK", "private", &part_info);
	if(!ret)
	{
//		int  buffer[1024/4];
//		int  i;

		__inf("find private part\n");
//		if(!wBoot_block_read(part_info.addr + 1024 * 1024/512, 2, buffer))
//		{
//			for(i=0;i<1024/4;i++)
//			{
//				if(!(buffer[i] == -1))
//				{
//					__inf("value is bad\n");
//					return -1;
//				}
//			}
//		}
		if(!usb_decect_key(keyvalue))
		{
			usb_start(1);
    		usb_run();
		}
	}

	return 0;
}



/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: BootMain_Private.h
*
* Author 		: javen
*
* Description 	: 启动引导私有数据结构
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/
#ifndef  __SPRITE_RES_IMG__
#define  __SPRITE_RES_IMG__



typedef struct boot_global_info
{
    __s32              display_device;       //显示在LCD，TV，HDMI上
    __s32              display_mode;         //显示模式
    __s32			   erase_flash;          //是否需要擦除nand flash
    char               user_reserved[512];   //用户保留参数，可以自定义
}
boot_global_info_t;


#endif   //__SPRITE_RES_IMG__


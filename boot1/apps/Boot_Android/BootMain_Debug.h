/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: BootMain_Debug.h
*
* Author 		: javen
*
* Description 	: boot1 µ÷ÊÔ´òÓ¡
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/
#ifndef  __BOOTMAIN_DEBUG_H__
#define  __BOOTMAIN_DEBUG_H__

#define  BOOT_CONFIG_DEBUG_LEVEL   3

#if(BOOT_CONFIG_DEBUG_LEVEL == 0)
#define  DMSG_TEST(...)
#define  DMSG_INFO(...)
#define  DMSG_PANIC(...)
#elif(BOOT_CONFIG_DEBUG_LEVEL == 1)
#define  DMSG_TEST(...)
#define  DMSG_INFO(...)
#define  DMSG_PANIC			__inf
#elif(BOOT_CONFIG_DEBUG_LEVEL == 2)
#define  DMSG_TEST(...)
#define  DMSG_INFO			__inf
#define  DMSG_PANIC			__inf
#elif(BOOT_CONFIG_DEBUG_LEVEL == 3)
#define  DMSG_TEST			__inf
#define  DMSG_INFO			__inf
#define  DMSG_PANIC			__inf
#endif

#endif   //__BOOTMAIN_DEBUG_H__



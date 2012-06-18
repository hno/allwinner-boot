/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: BootOS_i.h
*
* Author 		: javen
*
* Description 	: 操作系统引导
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/
#ifndef  __BOOTOS_I_H__
#define  __BOOTOS_I_H__

typedef  struct  bootloader_msg_t
{
    char command[32];
    char status[32];
    char recovery[1024];
}bootloader_msg;

#endif   //__BOOTOS_I_H__


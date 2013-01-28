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

int  card_mbr_info(void *buf)
{
	return wBoot_block_read(0, MBR_SIZE/512, buf);
}


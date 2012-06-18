/*
*************************************************************************************
*                         			eGon
*					        Application Of eGon2.0
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: Parse_Picture.h
*
* Author 		: javen
*
* Description 	: 图片解析
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-10          1.0            create this file
*
*************************************************************************************
*/
#ifndef  __PARSE_PICTURE_H__
#define  __PARSE_PICTURE_H__

#include "types.h"

typedef struct tag_Picture
{
    void *Buffer;   		/* 存放图片数据     */
    __u32 BufferSize;   	/* buffer长度       */

    __u32 BitCount; 		/* 一个像素的bit数  */
    __u32 Width;    		/* 图片宽度         */
    __u32 Height;   		/* 图片高度         */
    __u32 RowSize;  		/* 图片一行的大小   */
}Picture_t;


int Parse_Pic_BMP_ByPath(char *Path, Picture_t *PictureInfo, unsigned address);

#endif   //__PARSE_PICTURE_H__


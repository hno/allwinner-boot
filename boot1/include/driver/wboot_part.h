/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON include sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : eGon2_part.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#ifndef	_WBOOT_PART_H_
#define	_WBOOT_PART_H_

//获取分区信息的数据结构
typedef struct _part_info
{
	long long       addr;     //起始地址，扇区表示
	long long       len;	    //长度
}
part_info_t;



#endif  /* _WBOOT_PART_H_ */



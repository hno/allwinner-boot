/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON axf sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : elf_parser.h
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
#ifndef	_ELF_PARSER_H_
#define	_ELF_PARSER_H_

#define	eGON2_SUB_TYPE_MAGIC	0x55aa0000
#define	eGON2_SUB_TYPE_APP	    (eGON2_SUB_TYPE_MAGIC + 1)
#define	eGON2_SUB_TYPE_DRV	    (eGON2_SUB_TYPE_MAGIC + 2)

//这个宏和eGon2的特殊段段名对应，系统寻找到这个段，然后进行解析
#define     eGon2_HEADER_COMM_MAGIC     "EGON2_MAGIC"

#define 	eGON2_HEADER_PRIV_FED_MAGIC0	'F'
#define 	eGON2_HEADER_PRIV_FED_MAGIC1	'E'
#define 	eGON2_HEADER_PRIV_FED_MAGIC2	'D'
#define 	eGON2_HEADER_PRIV_FED_MAGIC3	'M'

#define 	eGON2_HEADER_PRIV_FET_MAGIC0	'F'
#define 	eGON2_HEADER_PRIV_FET_MAGIC1	'E'
#define 	eGON2_HEADER_PRIV_FET_MAGIC2	'T'
#define 	eGON2_HEADER_PRIV_FET_MAGIC3	'M'


extern      int elf_loader(void * elf_img, unsigned int *entry);

#endif   /* _ELF_PARSER_H_ */


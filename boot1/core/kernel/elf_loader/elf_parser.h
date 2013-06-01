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


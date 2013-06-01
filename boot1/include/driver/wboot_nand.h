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
#ifndef	_WBOOT_NAND_H_
#define	_WBOOT_NAND_H_

typedef enum  wboot_nand_cmd_set
{
    EGON2_NAND_PHY_READ,                   			/* 读NAND 物理块，aux = wboot_nand_rw_operation_t */
    EGON2_NAND_PHY_WRITE,							/* 写NAND 物理块，aux = wboot_nand_rw_operation_t */
    EGON2_NAND_GET_VERSION,							/* 获取NAND版本 */
    EGON2_NAND_ERASE,                        		/* 擦除NAND FLASH 的某一个块*/
    EGON2_NAND_BAD_BLOCK_CHECK,						/* 获取 good_block_ratio */
    EGON2_NAND_HARDWARE_SCAN_START,                 /* 扫描NAND信息开始 */
    EGON2_NAND_HARDWARE_SCAN_STOP,                  /* 扫描NAND信息结束 */
    EGON2_NAND_INIT,								/* NAND 物理逻辑层初始化 */
    EGON2_NAND_EXIT,								/* NAND 物理逻辑层关闭 */
    EGON2_NAND_GET_INFO,							/* NAND 获取信息 */
    EGON2_NAND_
}
wboot_nand_cmd_set_t;

typedef enum wboot_nand_rw_operation
{
	EGON2_NAND_RW_NORMAL,                           // NAND读写方式，按照普通(标准) 方式进行
	EGON2_NAND_RW_SEQUNECE,                         // NAND读写方式按照 SEQUENCE 方式进行
	EGON2_NAND_RW_1K,                               // NAND读写方式按照 页面1K大小 方式进行
	EGON2_NAND_RW_
}
wboot_nand_rw_operation_t;

typedef enum wboot_nand_erase_operation
{
	EGON2_NAND_ERASE_BLOCK,                         // NAND擦除一个块
	EGON2_NAND_ERASE_BOOT_BLOCK,                    // NAND擦除Boot用到的块
	EGON2_NAND_ERASE_CHIP,							// NAND擦除整片
	EGON2_NAND_ERASE_
}
wboot_nand_erase_operation_t;


#endif	//_WBOOT_NAND_H_

/* end of _WBOOT_NAND_H_ */


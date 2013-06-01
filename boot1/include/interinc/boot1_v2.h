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
#ifndef  __boo1_h
#define  __boo1_h


#include "egon_def.h"


#define BOOT1_MAGIC                     "eGON.BT1"
#define MAGIC_FLAG	                    "PHOENIX_CARD_IMG"
/****************************************************************************
*                                                                           *
*     下面的宏定是给 NAND 使用                                              *
*                                                                           *
*                                                                           *
*****************************************************************************/
#define BOOT1_START_BLK_NUM             BLKS_FOR_BOOT0

#define BLKS_FOR_BOOT1_IN_16K_BLK_NF    32
#define BLKS_FOR_BOOT1_IN_32K_BLK_NF    8
#define BLKS_FOR_BOOT1_IN_128K_BLK_NF   5
#define BLKS_FOR_BOOT1_IN_256K_BLK_NF   5
#define BLKS_FOR_BOOT1_IN_512K_BLK_NF   5
#define BLKS_FOR_BOOT1_IN_1M_BLK_NF     5
#define BLKS_FOR_BOOT1_IN_2M_BLK_NF     5
#define BLKS_FOR_BOOT1_IN_4M_BLK_NF     5
/****************************************************************************
*                                                                           *
*     下面的宏定是给 SDMMC 使用                                             *
*                                                                           *
*                                                                           *
*****************************************************************************/
/* 用于卡启动和量产的宏定义 */
#define BOOT1_START_SECTOR_IN_SDMMC    38192
#define BOOTIMG_SECTOR_IN_SDMMC        40240         //epdk100.img包在卡上的存放位置(扇区)

//固定不可改变
#define BOOT1_SDMMC_START_ADDR         38192
/****************************************************************************
*                                                                           *
*     下面的宏定是给 NOR 使用                                               *
*                                                                           *
*                                                                           *
*****************************************************************************/
#define BOOT0_START_SECTOR_IN_SPINOR      0          //由brom指定
#define BOOT1_START_SECTOR_IN_SPINOR     128          //空出64k给boot0使用，同时节约nor空间
#define BOOT1_SPI_NOR_START_ADDR        ( BOOT0_SPI_NOR_START_ADDR + SPI_NOR_SIZE_FOR_BOOT0 )       // add for spi nor. by Gary, 2009-12-8 11:47:17
#define SPI_NOR_SIZE_FOR_BOOT1          ( SZ_64K * 6 )                                              // add for spi nor. by Gary, 2009-12-8 11:47:17
/****************************************************************************
*                                                                           *
*     下面的宏定是给 COMMON 使用                                            *
*                                                                           *
*                                                                           *
*****************************************************************************/

/* work mode */
#define WORK_MODE_DEVELOP	0x01	//用于开发
#define WORK_MODE_TEST		0x02	//用于测试
#define WORK_MODE_PRODUCT	0x04	//用于量产
#define WORK_MODE_UPDATE	0x08	//用于升级

#define BOOT_SCRIPT_BUFFER_SIZE           (32 * 1024)
//定义存储类型
#define   START_FROM_NAND_FLASH       0
#define   START_FROM_SDCARD           1
#define   START_FROM_NOR_FLASH        2

//NAND相关数据结构
typedef struct boot1_nand_para_set
{
    __u32  nand_good_block_ratio;
    __u32  reserved[15];
}boot1_nand_para_set_t;

typedef struct _boot_nand_connect_info_t
{
	__u8  id[8];
	__u8  chip_cnt;
	__u8  chip_connect;
	__u8  rb_cnt;
	__u8  rb_connect;
	__u32 good_block_ratio;
}boot_nand_connect_info_t;


//SD卡相关数据结构
typedef struct _boot_sdcard_info_t
{
	__s32               card_ctrl_num;                //总共的卡的个数
	__s32				boot_offset;                  //指定卡启动之后，逻辑和物理分区的管理
	__s32 				card_no[4];                   //当前启动的卡号, 16-31:GPIO编号，0-15:实际卡控制器编号
	__s32 				speed_mode[4];                //卡的速度模式，0：低速，其它：高速
	__s32				line_sel[4];                  //卡的线制，0: 1线，其它，4线
	__s32				line_count[4];                //卡使用线的个数
}
boot_sdcard_info_t;


typedef struct _boot_core_para_t
{
    __u32  user_set_clock;                 // 运行频率 M单位
    __u32  user_set_core_vol;              // 核心电压 mV单位
    __u32  vol_threshold;                  // 开机门限电压
}
boot_core_para_t;
/******************************************************************************/
/*                              file head of Boot1                            */
/******************************************************************************/
typedef struct _boot1_private_head_t
{
	__u32              prvt_head_size;
	__u8               prvt_head_vsn[4];                // the version of Boot1_private_hea
	__s32						uart_port;              // UART控制器编号
	normal_gpio_cfg             uart_ctrl[2];           // UART控制器(调试打印口)GPIO信息
	boot_dram_para_t      		dram_para;              // dram init para
	char                        script_buf[32 * 1024];  // 脚本参数
	boot_core_para_t            core_para;              // 关键参数
	__s32						twi_port;               // TWI控制器编号
	normal_gpio_cfg             twi_ctrl[2];            // TWI控制器GPIO信息，用于控制TWI
	__s32						debug_enable;           // debug使能参数
    __s32                       hold_key_min;           // hold key最小值
    __s32                       hold_key_max;           // hold key最大值
    __u32                       work_mode;              // 模式，区分量产还是升级
    __u32                       storage_type;           // 存储介质类型  0：nand   1：sdcard    2: spinor
    normal_gpio_cfg             storage_gpio[32];       // 存储设备 GPIO信息
    char                        storage_data[512 - sizeof(normal_gpio_cfg) * 32];      // 用户保留数据信息
    //boot_nand_connect_info_t    nand_connect_info;    // nand 参数
}boot1_private_head_t;

typedef struct _boot1_file_head_t
{
	boot_file_head_t      boot_head;
	boot1_private_head_t  prvt_head;
}boot1_file_head_t;


#endif     //  ifndef __boo1_h

/* end of boo1.h */

/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : service.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.09.14
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.09.14       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __service_h
#define  __service_h

#include "..\\lib_i.h"

extern __u8  *get_page_buf( void );
extern __s32  BOOT_GetPid( void );
extern __bool STATE_SingleChipOp( void );
extern void   get_boot0_dram_para( boot_dram_para_t *boot_dram_para_p );
extern void   delay( __u32 ms );
extern void   set_dram_para(void *dram_addr, __u32 dram_size);

extern void   set_boot0_dram_para( boot_dram_para_t *boot0_dram_para_p );
extern void set_nand_good_block_ratio_para(void *nand_info_boot0);

extern __s32  boot_set_gpio(void  *gpio_list, __u32 group_count_max, __s32 set_gpio);

#endif     //  ifndef __service_h

/* end of service.h */

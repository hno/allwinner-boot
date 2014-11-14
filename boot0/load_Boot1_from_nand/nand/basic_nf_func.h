/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : basic_nf_func.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2009.09.13
*
* Description :
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.09.13       1.1.0        build the file
*
************************************************************************************************************************
*/

#ifndef  __basic_nf_func_h
#define  __basic_nf_func_h



extern __u32 NF_BLOCK_SIZE;
extern __u32 NF_BLK_SZ_WIDTH;
extern __u32 NF_PAGE_SIZE;
extern __u32 NF_PG_SZ_WIDTH;
extern __u32 BOOT1_LAST_BLK_NUM;
extern __u32 page_with_bad_block;


#define NF_SECTOR_SIZE                  512U
#define NF_SCT_SZ_WIDTH                 9U
#define OOB_BUF_SIZE_PER_SECTOR         4


enum 
{
	NF_OK         = 0,
	NF_GOOD_BLOCK = 0, 
	NF_OVERTIME_ERR  ,
	NF_ECC_ERR       ,
	NF_BAD_BLOCK     ,
	NF_ERASE_ERR     ,
	NF_PROG_ERR      ,
	NF_NEW_BAD_BLOCK ,
	NF_LACK_BLKS     ,
	NF_ERROR     = -1,
	NF_ERR_COUNT              // always the last one
};


#define MAX_PAGE_SIZE         SZ_8K
#define BAD_BLK_FLAG          0



extern __s32  NF_open ( void );
extern __s32  NF_close( void );
extern __s32  NF_read ( __u32 sector_num, void *buffer, __u32 N );
extern __s32  NF_write( __u32 sector_num, void *buffer, __u32 N );
extern __s32  NF_erase( __u32 blk_num );
extern __s32  NF_read_status ( __u32 blk_num );
extern __s32  NF_mark_bad_block( __u32 blk_num );
extern __s32  NF_verify_block( __u32 blk_num );




#endif     //  ifndef __basic_nf_func_h


/* end of basic_nf_func.h  */

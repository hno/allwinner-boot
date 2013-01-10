/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : service.c
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
#include "..\lib_i.h"
#include "string.h"


extern const boot0_file_head_t  BT0_head;


__bool STATE_SingleChipOp( void )
{
	return 1;
}



__u8  *get_page_buf( void )
{
	return (__u8 *)PAGE_BUF_FOR_BOOT0;
}


void   get_boot0_dram_para( boot_dram_para_t *boot0_dram_para_p )
{
	memcpy( boot0_dram_para_p, &(BT0_head.prvt_head.dram_para), sizeof(boot_dram_para_t) );
}

void   set_boot0_dram_para( boot_dram_para_t *boot0_dram_para_p )
{
	boot0_file_head_t  *boot0_buf = (boot0_file_head_t *)BOOT0_BASE;

	memcpy( &(boot0_buf->prvt_head.dram_para), boot0_dram_para_p, sizeof(boot_dram_para_t) );
}

void set_dram_para(void *dram_addr )
{
	boot1_file_head_t  *boot1_buf = (boot1_file_head_t *)BOOT1_BASE;
	memcpy((void *)&boot1_buf->prvt_head.dram_para, dram_addr, sizeof(boot_dram_para_t));

	return;
}

//void  delay( __u32 ms )       // CPU frequence is 120Mhz
//{
//	__u32 count;
//	__u32 i;
//
//	for( i = 0, count = ms * 2000;  i < count;  i++ )
//		;
//}

__s32 BOOT_NandGetPara(boot_nand_para_t *param)
{
#if 1
	memcpy( (void *)param, BT0_head.prvt_head.storage_data, sizeof(boot_nand_para_t));
#else
	param->ChipCnt = 2;
	param->ChipConnectInfo = 5;
	param->RbCnt = 1;
	param->RbConnectInfo = 1;
	param->RbConnectMode = 3;
	param->BankCntPerChip = 1;
	param->DieCntPerChip = 1;
	param->PlaneCntPerDie = 2;
	param->SectorCntPerPage = 0x10;
	param->PageCntPerPhyBlk = 0x100;
	param->BlkCntPerDie = 0x800;
	param->OperationOpt = 0x48;
	param->FrequencePar = 0x1f;
	param->EccMode = 2;
	memset(param->NandChipId, 0xff, 8);
	param->NandChipId[0] = 0xad;
	param->NandChipId[1] = 0xd7;
	param->NandChipId[2] = 0x94;
	param->NandChipId[3] = 0x9a;
	param->ValidBlkRatio = 900;
	param->good_block_ratio = 960;
	param->ReadRetryType = 0;
	param->DDRType = 0;
#endif
	return 0;
}



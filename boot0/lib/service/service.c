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
void   print_boot0_dram_para( boot_dram_para_t *boot0_dram_para_p )
{   
    msg("boot0_para->dram_clk			= %d\n",boot0_dram_para_p->dram_clk);
    msg("boot0_para->dram_type			= %d\n",boot0_dram_para_p->dram_type);
    msg("boot0_para->dram_rank_num		= %d\n",boot0_dram_para_p->dram_rank_num);
    msg("boot0_para->dram_chip_density	= %d\n",boot0_dram_para_p->dram_chip_density);
    msg("boot0_para->dram_io_width		= %d\n",boot0_dram_para_p->dram_io_width);
    msg("boot0_para->dram_bus_width		= %d\n",boot0_dram_para_p->dram_bus_width);
    
	msg("boot0_para->dram_cas		    = %d\n",boot0_dram_para_p->dram_cas);
    msg("boot0_para->dram_zq           = %x\n",boot0_dram_para_p->dram_zq);
    msg("boot0_para->dram_odt_en       = %d\n",boot0_dram_para_p->dram_odt_en);
    msg("boot0_para->dram_size         = %d\n",boot0_dram_para_p->dram_size);
    
	
	msg("boot0_para->dram_tpr0			= %x\n",boot0_dram_para_p->dram_tpr0);
	msg("boot0_para.dram_tpr1			= %x\n",boot0_dram_para_p->dram_tpr1);
	msg("boot0_para->dram_tpr2			= %x\n",boot0_dram_para_p->dram_tpr2);
	msg("boot0_para->dram_emr1			= %x\n",boot0_dram_para_p->dram_emr1);
	msg("boot0_para->dram_emr2			= %x\n",boot0_dram_para_p->dram_emr2);
	msg("boot0_para->dram_emr2			= %x\n",boot0_dram_para_p->dram_emr2);
	
}

void   set_boot0_dram_para( boot_dram_para_t *boot0_dram_para_p )
{
	boot0_file_head_t  *boot0_buf = (boot0_file_head_t *)BOOT0_BASE;

	memcpy( &(boot0_buf->prvt_head.dram_para), boot0_dram_para_p, sizeof(boot_dram_para_t) );
}

void set_dram_para(void *dram_addr, __u32 dram_size)
{
    boot_dram_para_t  *dram_para = (boot_dram_para_t *)dram_addr;
	boot1_file_head_t  *boot1_buf = (boot1_file_head_t *)BOOT1_BASE;
    dram_para->dram_size = dram_size;
	memcpy((void *)&boot1_buf->prvt_head.dram_para, dram_addr, sizeof(boot_dram_para_t));

	return;
}

void set_nand_good_block_ratio_para(void *nand_info_boot0)
{
    boot1_file_head_t  *boot1_buf = (boot1_file_head_t *)BOOT1_BASE;
    boot_nand_para_t *nand_info_boot1 = (boot_nand_para_t *)(boot1_buf->prvt_head.storage_data);
	nand_info_boot1->good_block_ratio = ((boot_nand_para_t *)(nand_info_boot0))->good_block_ratio;
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
	param->OperationOpt = 0x1c8;
	param->FrequencePar = 0x1f;
	param->EccMode = 3;
	memset(param->NandChipId, 0xff, 8);
	param->NandChipId[0] = 0xad;
	param->NandChipId[1] = 0xd7;
	param->NandChipId[2] = 0x94;
	param->NandChipId[3] = 0xda;
	param->NandChipId[4] = 0x74;
	param->NandChipId[5] = 0xc3;
	param->ValidBlkRatio = 900;
	param->good_block_ratio = 960;
	param->ReadRetryType = 0x00010604;
	param->DDRType = 0;
#endif
	return 0;
}



#ifndef __NAND_FOR_BOOT1__
#define __NAND_FOR_BOOT1__

#include  "D:\winners\eBase\include\platform\aw_ic_cfg.h"

#define SUCCESS	0
#define FAIL	-1
#define BADBLOCK -2

#if (AW_IC_CFG_GLOBAL == AW_IC_CFG_SUN_20)
typedef struct
{
	__u8 id[8];
	__u8 chip_cnt;
	__u8 chip_connect;
	__u8 rb_cnt;
	__u8 rb_connect;
	__u32 good_block_ratio;
}boot_nand_para_t;

#elif (AW_IC_CFG_GLOBAL == AW_IC_CFG_SUN_18)
typedef struct _boot_nand_para_t
{
    __u8        ChipCnt;                            //the count of the total nand flash chips are currently connecting on the CE pin
    __u16       ChipConnectInfo;                    //chip connect information, bit == 1 means there is a chip connecting on the CE pin
	__u8		RbCnt;
	__u8		RbConnectInfo;						//the connect  information of the all rb  chips are connected
    __u8        RbConnectMode;						//the rb connect  mode
	__u8        BankCntPerChip;                     //the count of the banks in one nand chip, multiple banks can support Inter-Leave
    __u8        DieCntPerChip;                      //the count of the dies in one nand chip, block management is based on Die
    __u8        PlaneCntPerDie;                     //the count of planes in one die, multiple planes can support multi-plane operation
    __u8        SectorCntPerPage;                   //the count of sectors in one single physic page, one sector is 0.5k
    __u16       PageCntPerPhyBlk;                   //the count of physic pages in one physic block
    __u16       BlkCntPerDie;                       //the count of the physic blocks in one die, include valid block and invalid block
    __u16       OperationOpt;                       //the mask of the operation types which current nand flash can support support
    __u8        FrequencePar;                       //the parameter of the hardware access clock, based on 'MHz'
    __u8        EccMode;                            //the Ecc Mode for the nand flash chip, 0: bch-16, 1:bch-28, 2:bch_32
    __u8        NandChipId[5];                      //the nand chip id of current connecting nand chip
    __u16       ValidBlkRatio;                      //the ratio of the valid physical blocks, based on 1024
	__u32 		good_block_ratio;					//good block ratio get from hwscan
	__u32		ReadRetryType;						//the read retry type
	__u32       Reserved[32];
}boot_nand_para_t;

#endif
//typedef struct
//{
//	__u8 id[8];
//	__u8 chip_cnt;
//	__u8 chip_connect;
//	__u8 rb_cnt;
//	__u8 rb_connect;
//	__u32 good_block_ratio;
//}boot_nand_para_t;
//typedef struct
//{
//	__u8        ChipCnt;                            //the count of the total nand flash chips are currently connecting on the CE pin
//    __u16       ChipConnectInfo;                    //chip connect information, bit == 1 means there is a chip connecting on the CE pin
//	__u8		RbCnt;
//	__u8		RbConnectInfo;						//the connect  information of the all rb  chips are connected
//    __u8        RbConnectMode;						//the rb connect  mode
//	__u8        BankCntPerChip;                     //the count of the banks in one nand chip, multiple banks can support Inter-Leave
//    __u8        DieCntPerChip;                      //the count of the dies in one nand chip, block management is based on Die
//    __u8        PlaneCntPerDie;                     //the count of planes in one die, multiple planes can support multi-plane operation
//    __u8        SectorCntPerPage;                   //the count of sectors in one single physic page, one sector is 0.5k
//    __u16       PageCntPerPhyBlk;                   //the count of physic pages in one physic block
//    __u16       BlkCntPerDie;                       //the count of the physic blocks in one die, include valid block and invalid block
//    __u16       OperationOpt;                       //the mask of the operation types which current nand flash can support support
//    __u8        FrequencePar;                       //the parameter of the hardware access clock, based on 'MHz'
//    __u8        EccMode;                            //the Ecc Mode for the nand flash chip, 0: bch-16, 1:bch-28, 2:bch_32
//    __u8        NandChipId[5];                      //the nand chip id of current connecting nand chip
//    __u16       ValidBlkRatio;                      //the ratio of the valid physical blocks, based on 1024
//	__u32 		good_block_ratio;					//good block ratio get from hwscan
//	__u32		ReadRetryType;						//the read retry type
//	__u32       Reserved[32];
//}
//boot_nand_para_t;

typedef struct boot_physical_param
{
	__u8   chip; //chip no
	__u16  block; // block no within chip
	__u16  page; // page no within block
	__u16  sectorbitmap; //done't care
	void   *mainbuf; //data buf
	void   *oobbuf; //oob buf
}
boot_physical_param_t;

typedef struct boot_flash_info
{
	__u32 chip_cnt;
	__u32 blk_cnt_per_chip;
	__u32 blocksize; //unit by sector
	__u32 pagesize; //unit by sector
	__u32 pagewithbadflag; /*bad block flag was written at the first byte of spare area of this page*/
}
boot_flash_info_t;

typedef struct _boot_nand_logical_t
{
	__u32 start_sector;           //起始扇区
	__u32 nsector;				  //总扇区数
	void  *pbuffer;				  //数据地址
}
boot_nand_logical_t;
/*
************************************************************************************************************************
*                       READ ONE SINGLE PAGE
*
*Description: read one page data from nand based on single plane;
*
*Arguments  : *readop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  read ok;
*               = FAIL    read fail.
************************************************************************************************************************
*/
__s32 NFB_PhyRead (struct boot_physical_param *readop);
__s32 NFB_PhyRead_Seq (struct boot_physical_param *readop);
__s32 NFB_PhyRead_1K (struct boot_physical_param *readop);
/*
************************************************************************************************************************
*                       READ ONE SINGLE PAGE 1K PAGE_SIZE
*
*Description: read one page data from nand based on single plane;
*
*Arguments  : *readop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  read ok;
*               = FAIL    read fail.
************************************************************************************************************************
*/
__s32 NAND_PhyRead (struct boot_physical_param *readop);
__s32 NAND_PhyRead_Seq (struct boot_physical_param *readop);
__s32 NAND_PhyRead_1K (struct boot_physical_param *readop);
/*
************************************************************************************************************************
*                       WRITE ONE SINGLE PAGE
*
*Description: write one page data to nand based on single plane;
*
*Arguments  : *writeop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  	write ok;
*               = FAIL    	write fail.
*				= BADBLOCK	write fail and bad block made by program
************************************************************************************************************************
*/
__s32 NFB_PhyWrite (struct boot_physical_param *writeop);
__s32 NFB_PhyWrite_Seq (struct boot_physical_param *writeop);
__s32 NFB_PhyWrite_1K (struct boot_physical_param *writeop);
/*
************************************************************************************************************************
*                       WRITE ONE SINGLE PAGE WITH 1K PAGESIZE
*
*Description: write one page data to nand based on single plane, pagesize set to 1k bytes;
*
*Arguments  : *writeop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  	write ok;
*               = FAIL    	write fail.
*				= BADBLOCK	write fail and bad block made by program
************************************************************************************************************************
*/
__s32 NAND_PhyWrite (struct boot_physical_param *writeop);
__s32 NAND_PhyWrite_1K (struct boot_physical_param *writeop);
__s32 NAND_PhyWrite_Seq (struct boot_physical_param *writeop);
/*
************************************************************************************************************************
*                       ERASE ONE SINGLE BLOCK
*
*Description: erase one block in nand based on single plane;
*
*Arguments  : *eraseop - the structure with physical address in nand and data buffer
*
*Return     :   = SUCESS  	erase ok;
*               = FAIL    	erase fail.
*				= BADBLOCK	erase fail and bad block made by erase
************************************************************************************************************************
*/
__s32 NFB_PhyErase(struct boot_physical_param *eraseop);
__s32 NAND_PhyErase(struct boot_physical_param *eraseop);
/*
************************************************************************************************************************
*                       GET FLASH INFO
*
*Description: get some info about nand flash;
*
*Arguments  : *param     the stucture with info.
*
*Return     : the result of chip reset;
*               = SUCESS  get ok;
*               = FAIL    get fail.
************************************************************************************************************************
*/
__s32 NFB_GetFlashInfo(struct boot_flash_info *param);
__s32 NAND_GetFlashInfo(struct boot_flash_info *param);
/*
************************************************************************************************************************
*                       GET NAND VERSION INFO
*
*Description: get some info about nand flash;
*
*Arguments  : *param     the stucture with info.
*
*Return     :   >0     get ok;
*               =0    get fail.
************************************************************************************************************************
*/
__u32 NFB_GetNandVersion(void);

/*
************************************************************************************************************************
*                       INIT NAND FLASH
*
*Description: initial nand flash,request hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  initial ok;
*               = FAIL    initial fail.
************************************************************************************************************************
*/
__s32 NFB_PhyInit(void);
/*
************************************************************************************************************************
*                       RELEASE NAND FLASH
*
*Description: release  nand flash and free hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  release ok;
*               = FAIL    release fail.
************************************************************************************************************************
*/
__s32 NFB_PhyExit(void);
/*
************************************************************************************************************************
*                       WRITE SOME LOGICAl SECTORS
*
*Description: write some logic area;
*
*Arguments  : nSectnum   - start sector number
*			  nSectorCnt - sector count
*			  *pBuf		 - data buffer
*Return     :   = SUCESS  	write ok;
*               = FAIL    	write fail.

************************************************************************************************************************
*/
__s32 NFB_LogicWrite(__u32 nSectNum, __u32 nSectorCnt, void * pBuf);
__s32 NAND_LogicWrite(__u32 nSectNum, __u32 nSectorCnt, void * pBuf);
/*
************************************************************************************************************************
*                       READ SOME LOGICAl SECTORS
*
*Description: read some logic area;
*
*Arguments  : nSectnum   - start sector number
*			  nSectorCnt - sector count
*			  *pBuf		 - data buffer
*Return     :   = SUCESS  	read ok;
*               = FAIL    	read fail.

************************************************************************************************************************
*/
__s32 NFB_LogicRead(__u32 nSectNum, __u32 nSectorCnt, void * pBuf);
__s32 NAND_LogicRead(__u32 nSectNum, __u32 nSectorCnt, void * pBuf);


/*
************************************************************************************************************************
*                       INIT NAND DRIEVER
*
*Description: configure nand flash driver, include physic init and logic init;
*
*Arguments  : void
*Return     :   = SUCESS  	init ok;
*               = other    	init fail.

************************************************************************************************************************
*/
__s32 NFB_Init(void);
__s32 NAND_Init(void);
/*
************************************************************************************************************************
*                       EXIT NAND DRIVER
*
*Description: exit physic layer and logic layer;
*
*Arguments  : void
*Return     :   = SUCESS  	exit ok;
*               = FAIL    	exit fail.

************************************************************************************************************************
*/
__s32 NFB_Exit(void);
__s32 NAND_Exit(void);

/*
************************************************************************************************************************
*                       SCAN NAND HW
*
*Description: initial nand flash,request hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  initial ok;
*               = FAIL    initial fail.
************************************************************************************************************************
*/
__s32 NAND_HWScanStart(boot_nand_para_t *nand_connect);


/************************************************************************************************************************
*                       NAND_HWScanStop
*
*Description: release  nand flash and free hardware resources;
*
*Arguments  : void.
*
*Return     :   = SUCESS  release ok;
*               = FAIL    release fail.
************************************************************************************************************************
*/
__s32 NAND_HWScanStop(void);

/*
************************************************************************************************************************
*                       NAND_VersionCheck
*
*Description: Check the nand flash driver is current version
*
*Arguments  : void.
*
*Return     :   = 0  version match;
*                 1  version not match
*                -1  can't get invalid version info
************************************************************************************************************************
*/
__s32 NAND_VersionCheck(void);
__s32 NAND_VersionGet(__u8 *version);

/*
************************************************************************************************************************
*                       NAND_EraseBootBlocks
*
*Description: Check the nand flash driver is current version
*
*Arguments  : connecnt info.
*
*Return     :   = 0     OK;
*               other  Fail.
************************************************************************************************************************
*/
__s32  NAND_EraseBootBlocks( const boot_nand_para_t *connect_info_p);


/*
************************************************************************************************************************
*                       NAND_EraseChip
*
*Description: Erase chip, only erase the all 0x0 blocks
*
*Arguments  : connecnt info.
*
*Return     :   = 0     OK;
*               other  Fail.
************************************************************************************************************************
*/
__s32  NAND_EraseChip( const boot_nand_para_t *connect_info_p);

/*******************************************************************************
*函数名称: NAND_BadBlockScan
*函数原型：bad_blcok_scan(const boot_nand_para_t *connect_info_p)
*函数功能: 标记blk_num指定的块为坏块。
*入口参数: connect_info_p
*返 回 值: >0              编程操作成功
*          -1              编程操作失败
*
*备    注:
*******************************************************************************/
__s32 NAND_BadBlockScan(const boot_nand_para_t *connect_info_p);


/*
************************************************************************************************************************
*                       Nand Get Parameter
*
*Description: get some parameter about nand after hwscan;
*
*Arguments  : *param     the stucture with info.
*
*Return     :   = 0     get ok;
*               = -1    get fail.
************************************************************************************************************************
*/
__s32 BOOT_NandGetPara(boot_nand_para_t *param);

#endif

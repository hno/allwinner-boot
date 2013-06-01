/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               DRAM CSP Module
*
*                                    (c) Copyright 2006-2010, Berg.Xing China
*                                             All Rights Reserved
*
* File    : csp_dram_para.h
* By      : Berg.Xing
* Version : v1.0
* Date    : 2010-12-2 13:24
* Descript:
* Update  : date                auther      ver     notes
*           2010-12-2 13:24     Berg.Xing   1.0     build the file;
*********************************************************************************************************
*/
#ifndef __CSP_DRAM_PARA_H__
#define __CSP_DRAM_PARA_H__

#define DRAM_PIN_DEV_ID     (0)
#define DRAM_PIN_LIST       ((__u32 *)0)
#define DRAM_PIN_NUMBER     (0)

//#define FPGA_51_TEST

/*
#define T_WR							0x9
#define T_WTR							0x7
#define T_RTP							0x3
#define T_RTW							0x6
#define T_RAS							0xf
#define T_RRD							0x4
#define T_RC							0x14
#define T_RCD							0x5
#define T_CKE							0x3
#define T_APD							0x8
#define T_XP							0x2
#define T_FAW							0xf
#define T_XSR							0xc8
#define T_RP							0x5
*/

//==============================================================================
// dram configuration parameter reference value
//==============================================================================
//DDR1_32B				DDR1_32B_32Mx16 (DDR 32-bits bus by two 32Mx16 SDRAM chip)
//DDR1_16B				DDR1_16B_32Mx16
//DDR2_32B				DDR2_32B_64Mx16
//DDR2_16B				DDR2_16B_64Mx16
//DDR3_32B				DDR3_32B_128Mx16
//DDR3_16B				DDR3_32B_128Mx16


//#ifdef DDR2_FPGA_S2C
	//DDR2 64Mx16 (128M Byte)
	#define MCTL_DDR_TYPE			2				//1: DDR, 2: DDR2, 3: DDR3
	#define MCTL_IO_WIDTH			8
	#define MCTL_CHIP_SIZE			1024			//unit in Mb
	#define MCTL_CAS				5
	#define MCTL_BANK_SIZE			8
	#define MCTL_COL_WIDTH			10
	#define MCTL_ROW_WIDTH			14
	#define MCTL_BUS_WIDTH			32
    #define MCTL_RANK_NUM			2
	#define	DRAMC_ID				1				//0: DRAMC0, 1: DRAMC1
//#endif


#endif  //__CSP_DRAM_PARA_H__


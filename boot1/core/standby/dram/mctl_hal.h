//*****************************************************************************
//	Allwinner Technology, All Right Reserved. 2006-2010 Copyright (c)
//
//	File: 				mctl_hal.h
//
//	Description:  This file implements basic functions for AW1633 DRAM controller
//
//	History:
//              2012/02/06      Berg Xing       0.10    Initial version
//              2012/02/24      Berg Xing       0.20    Support 2 channel
//              2012/02/27      Berg Xing       0.30    modify mode register access
//				2012/03/01		Berg Xing       0.40    add LPDDR2
//				2012/03/10		Berg Xing       0.50    add mctl_dll_init() function
//				2012/04/26		Berg Xing       0.60    add deep sleep
//				2012/06/19		Berg Xing       0.70    add 2T mode
//				2012/11/07		CPL				0.80	FPGA version based on berg's code
//				2012/11/14		CPL				0.90	add SID and regulate the parameters order
//				2012/11/21		CPL				0.91	modify parameters error
//				2012/11/25		CPL				0.92	modify for IC test
//				2012/11/27		CPL				0.93	add master configuration
//				2012/11/28		CPL				0.94	modify for boot and burn interface compatible
//				2012/11/29		CPL				0.95	modify lock parameters configuration
//				2012/12/3		CPL				0.96	add dll&pll delay and simple test
//*****************************************************************************

#ifndef   _MCTL_HAL_H
#define   _MCTL_HAL_H

#include "include.h"
//#define LINUX_CONFIG
//#define PW2I_PRINK

#ifdef PW2I_PRINK
//	#define msg printk
#endif

typedef struct __DRAM_PARA
{
	//normal configuration
	unsigned int        dram_clk;
	unsigned int        dram_type;		//dram_type			DDR2: 2				DDR3: 3				LPDDR2: 6	DDR3L: 31
    unsigned int        dram_zq;
    unsigned int		dram_odt_en;

	//control configuration
	unsigned int		dram_para1;
    unsigned int		dram_para2;

	//timing configuration
	unsigned int		dram_mr0;
    unsigned int		dram_mr1;
    unsigned int		dram_mr2;
    unsigned int		dram_mr3;
    unsigned int		dram_tpr0;
    unsigned int		dram_tpr1;
    unsigned int		dram_tpr2;
    unsigned int		dram_tpr3;
    unsigned int		dram_tpr4;
    unsigned int		dram_tpr5;
   	unsigned int		dram_tpr6;

    //reserved for future use
    unsigned int		dram_tpr7;
    unsigned int		dram_tpr8;
    unsigned int		dram_tpr9;
    unsigned int		dram_tpr10;
    unsigned int		dram_tpr11;
    unsigned int		dram_tpr12;
    unsigned int		dram_tpr13;

}__dram_para_t;

//#define FPGA_PLATFORM
//#define LPDDR2_FPGA_S2C_2CS_2CH
#define DDR3_32B
#define TEST_MEM 0x40000000

extern unsigned int DRAMC_init(__dram_para_t *para);
extern unsigned int DRAMC_init_auto(__dram_para_t *para);
static unsigned int mctl_sys_init(__dram_para_t *para);
static unsigned int mctl_reset_release(void);
static unsigned int mctl_dll_init(unsigned int ch_index, __dram_para_t *para);
static unsigned int mctl_channel_init(unsigned int ch_index, __dram_para_t *para);
static unsigned int mctl_com_init(__dram_para_t *para);
static unsigned int mctl_port_cfg(void);
extern signed int init_DRAM(int type, void *para);
static unsigned int ss_bonding_id(void);
static void paraconfig(unsigned int *para, unsigned int mask, unsigned int value);
//extern uint32 mctl_basic_test(void);
//extern uint32 mctl_stable_test(void);
extern int p2wi_read(unsigned int addr, unsigned int *val);
extern  int p2wi_write(unsigned int addr, unsigned int val);

#endif  //_MCTL_HAL_H











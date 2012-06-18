/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : sys_op.h
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
#ifndef  __arm_cpu_h
#define  __arm_cpu_h


/* icache operation */
extern void   enable_icache( void );
extern void   disable_icache( void );
extern void   clean_dcache( void );
extern void   flush_dcache( void );
extern void   flush_icache( void );
extern void   enable_dcache( void );
extern void   disable_dcache( void );

/* icache operation */
extern void   set_vect_low_addr( void );
extern void   set_vect_high_addr( void );

/* int operation */
extern void   open_sys_int( void );
extern void   close_sys_int( void );

/* mmu operation */
extern void   mmu_system_init(__u32 dram_base, __u32 dram_size, __u32 mmu_base);
extern void   mmu_enable( void );
extern void   mmu_disable( void );
extern void   mmu_flush_id_cache( void );


#endif     //  ifndef __arm_cpu_h

/* end of arm_cpu.h */

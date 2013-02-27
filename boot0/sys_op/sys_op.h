/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : sys_op_i.h
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
#ifndef  __sys_op_h
#define  __sys_op_h

/*
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
 */
#define typecheck(type,x) \
({	type __dummy; \
	typeof(x) __dummy2; \
	(void)(&__dummy == &__dummy2); \
	1; \
})

/*
 * if return true, means a is after b;
 */	
#define counter_after(a,b) \
(typecheck(__u32, a) && \
typecheck(__u32, b) && \
((__s32)(b) - (__s32)(a) < 0))
#define counter_before(a,b) counter_after(b,a)

#define counter_after_eq(a,b) \
(((__s32)(a) - (__s32)(b) >= 0))
#define counter_before_eq(a,b) counter_after_eq(b,a)


extern   void   mmu_enable( void );
extern   void   mmu_disable( void );
extern   void   mmu_system_init(__u32 dram_base, __u32 dram_size, __u32 mmu_base);
extern	void	change_runtime_env(__u32 mmu_flag);
extern	void	delay_us(__u32 us);
extern	__u32	get_cyclecount (void);
extern	void	init_perfcounters (__u32 do_reset, __u32 enable_divider);



#endif     //  ifndef __sys_op_h

/* end of __sys_op_h.h */

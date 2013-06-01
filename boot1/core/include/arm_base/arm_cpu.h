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

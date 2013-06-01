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
#ifndef __CACHE_H_
#define __CACHE_H_

#include "cache_const.h"

#if EBSP_CPU == EBSP_CPU_ARM920T
#define CSIZE                   14  // cache size as 2**CSIZE (16 K assumed)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     6  // set associativity = 2**NWAY (64 way)
#define I7SET                    5  // CP15 c7 set incrementer as 2**ISET
#define I7WAY                   26  // CP15 c7 way incrementer as 2**SSET
#define I9WAY                   26  // CP15 c9 way incrementer as 2**SSET
#endif

#if EBSP_CPU == EBSP_CPU_ARM922T
#define CSIZE                   14  // cache size as 2**CSIZE (16 K assumed)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     6  // set associativity = 2**NWAY (64 way)
#define I7SET                    5  // CP15 c7 set incrementer as 2**ISET
#define I7WAY                   26  // CP15 c7 way incrementer as 2**SSET
#define I9WAY                   26  // CP15 c9 way incrementer as 2**SSET
#endif

#if EBSP_CPU == EBSP_CPU_AC320
#define CSIZE                   14  // cache size as 2**CSIZE (16 K assumed)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     2  // set associativity = 2**NWAY (4 way)
#define I7SET                    4  // CP15 c7 set incrementer as 2**ISET
#define I7WAY                   30  // CP15 c7 way incrementer as 2**IWAY
#endif

#if EBSP_CPU == EBSP_CPU_ARM926EJ_S
#define CSIZE                   14  // cache size as 2**CSIZE (16 K assumed)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     2  // set associativity = 2**NWAY (4 way)
#define I7SET                    4  // CP15 c7 set incrementer as 2**ISET
#define I7WAY                   30  // CP15 c7 way incrementer as 2**IWAY
#endif

#if EBSP_CPU == EBSP_CPU_ARM940T
#define CSIZE                   12  // cache size as 2**CSIZE (4K)
#define CLINE                    4  // cache line size in bytes as 2**CLINE
#define NWAY                     6  // set associativity = 2**NWAY (64 way)
#define I7SET                    4  // CP15 c7 set incrementer = 2**ISET
#define I7WAY                   26  // CP15 c7 way incrementer = 2**IWAY
#define I9WAY                   0   // CP15 c9 way incrementer = 2**IWAY
#endif

#if EBSP_CPU == EBSP_CPU_ARM946E_S
#define CSIZE                   12  // cache size as 2**CSIZE (4 K assumed)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     2  // set associativity = 2**NWAY (4 way)
#define I7SET                    4  // CP15 c7 set incrementer = 2**ISET
#define I7WAY                   30  // CP15 c7 way incrementer = 2**IWAY
#define I9WAY                    0  // CP15 c7 way incrementer = 2**IWAY
#endif

#if EBSP_CPU == EBSP_CPU_ARM1022E
#define CSIZE                   14  // cache size as 2**CSIZE (16 K)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     6  // set associativity = 2**NWAY (64 way)
#define I7SET                    5  // CP15 c7 set incrementer as 2**ISET
#define I7WAY                   26  // CP15 c7 way incrementer as 2**SSET
#define I9WAY                   26  // CP15 c7 way incrementer = 2**IWAY
#endif

#if EBSP_CPU == EBSP_CPU_ARM1026EJ_S
#define CSIZE                   14  // cache size as 2**CSIZE (16 K assumed)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     2  // set associativity = 2**NWAY (4 way)
#define I7SET                    5  // CP15 c7 set incrementer as 2**ISET
#define I7WAY                   30  // CP15 c7 way incrementer as 2**IWAY
#endif

#if EBSP_CPU == EBSP_CPU_SA_110
#define CSIZE                   14  // cache size as 2**CSIZE (16 K)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     5  // set associativity = 2**NWAY (4 way)
#define CleanAddressDcache      0x8000
#endif

#if EBSP_CPU == EBSP_CPU_XSCALE
#define CSIZE                   15  // cache size as 2**CSIZE (32 K)
#define CLINE                    5  // cache line size in bytes as 2**CLINE
#define NWAY                     5  // set associativity = 2**NWAY (32 way)
#define MNWAY                    1  // set assoc mini D-cache = 2**NWAY (2 way)
#define MCSIZE                  11  // mini cache size  as 2**CSIZE (2 K)
#define CleanAddressDcache      0x8000  //(32K block 0x8000-0x10000)
#define CleanAddressMiniDcache  0x10000 //(2K block 0x10000-0x10800)
#endif


#define SWAY                    (CSIZE-NWAY)            // size of way = 2**SWAY
#define NSET                    (CSIZE-NWAY-CLINE)      // cachelines per way = 2**NSET

#endif


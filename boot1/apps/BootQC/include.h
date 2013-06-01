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
#ifndef  __INCLUDE_H__
#define  __INCLUDE_H__

#include "egon2.h"
#include "string.h"
#include "BootMain_Debug.h"
#include "common.h"

extern  void  debug_printf( const char * str, ...);
extern  int   debug_setdisplay_color(__u32 user_set_color);
extern  int   debug_setdisplay_clear(void);
extern  int   BoardInit_Display(int source, int mode);
extern  void  delay_240_no_cache( __u32 ms );
extern  void  delay_240_icache( __u32 ms );
extern  void  CleanFlushDCacheRegion(__u32 addr, __u32 cnt);

#define __dinf(...)         (debug_printf(__VA_ARGS__))

#define __dmsg(...)    		(debug_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     debug_printf(__VA_ARGS__))

#define __dwrn(...)         (debug_printf("WRN:L%d(%s):", __LINE__, __FILE__),                 \
    						     debug_printf(__VA_ARGS__))



int qc_test(void);

#endif   //__INCLUDE_H__


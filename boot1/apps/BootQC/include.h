/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : include.h
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
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


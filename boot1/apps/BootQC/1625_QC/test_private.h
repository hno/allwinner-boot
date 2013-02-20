/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : lib_i.h
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
#ifndef  __test_lib_i_h
#define  __test_lib_i_h


//#include "operations\\int_handle\\int_service_entry.h"
#include  "include.h"
/*
#include  "include\\ccu.h"
#include  "include\\comm.h"
#include  "include\\common.h"
#include  "include\\dmac.h"
#include  "include\\intc.h"
#include  "include\\irq.h"
#include  "include\\keypad.h"
#include  "include\\type.h"
#include  "include\\TPG_Functions.h"
*/

//#define TC_WHITE       { 255, 255, 255, 255 }
//#define TC_BLACK       {   0,   0,   0, 255 }
//#define TC_RED         {   0,   0, 255, 255 }
//#define TC_GREEN       {   0, 255,   0, 255 }
//#define TC_BLUE        { 255,   0,   0, 255 }

#define   DISPLAY_WHITE          (0xffffff)
#define   DISPLAY_BLACK          (0x000000)
#define   DISPLAY_RED            (0xff0000)
#define   DISPLAY_BLUE           (0x0000ff)
#define   DISPLAY_GREEN          (0x00ff00)

//extern   int InsINT_Func(unsigned char irq_no, int *func_addr);
//extern   int UnsInt_Func(unsigned char irq_no);

//extern   int EnableInt (unsigned char irq_no);
//extern   int DisableInt(unsigned char irq_no);


//extern    void debug_clear_screen(void);
//extern    void debug_display_set_color(__u32 tmpcolor);
extern    void __msdelay( __u32 ms );
extern    unsigned int timer_start(void);
extern    unsigned int timer_stop(void);         //∑µªÿ÷µ «ms

//extern    void *FS_fopen(const char * filename, char * mode);
//extern    int FS_fclose(void *hfile);
//extern    int FS_fread(void * buffer, unsigned int block_size, unsigned int count, void * hfile);
//extern    int FS_fseek(void * hfile, int offset, int whence);
#define   start_display()                     BoardInit_Display(0, 0)

#define   InsINT_Func(irq_no, func_addr)      wBoot_InsINT_Func(irq_no, func_addr, 0)
#define   UnsInt_Func(irq_no)                 wBoot_UnsInt_Func(irq_no)

#define   EnableInt(irq_no)                   wBoot_EnableInt(irq_no)
#define   DisableInt(irq_no)                  wBoot_DisableInt(irq_no)

#define   debug_display_set_color         debug_setdisplay_color
#define   debug_clear_screen              debug_setdisplay_clear


#define   FS_fopen                        wBoot_fopen
#define   FS_fclose                       wBoot_fclose
#define   FS_fread                        wBoot_fread
#define   FS_fseek                        wBoot_fseek

#ifndef NDEBUG
	#define  umsg                         __inf
	#define  dmsg                         __dinf
	#define  dclr()                       debug_clear_screen()
	#define  dcolor(color)                debug_display_set_color(color)
	#define  delay_icache(ms)             xDelayMS(ms)
	#define  delay_ncache(ms)             delay_240_no_cache(ms)
#else
	#define  umsg(...)
	#define  dmsg(...)
	#define  dclr()
	#define  dcolor(color)
	#define  delay_icache(ms)
	#define  delay_ncache(ms)
#endif




#endif     //  ifndef __test_lib_i_h

/* end of __test_lib_i_h.h */

/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : script.c
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
#ifndef  _STANDBY_I_H_
#define  _STANDBY_I_H_


#include  "include.h"


extern  int standby_int_init(void);
extern  int standby_int_exit(void);
extern  int standby_int_query(void);

extern  int  standby_clock_store(void);
extern  int  standby_clock_restore(void);
extern  int  standby_clock_to_source(int clock_source);
extern  void standby_clock_plldisable(void);
extern  void standby_clock_divsetto0(void);
extern  void standby_clock_divsetback(void);
extern  void standby_clock_drampll_ouput(int op);

extern  void standby_clock_apb1_to_source(int clock);
extern  void standby_clock_24m_op(int op);

extern  void  eGon2_store_sp(void);
extern  void  eGon2_restore_sp(void);
extern  void  eGon2_set_sp(void);
extern  void  eGon2_halt(void);


extern  void  dram_power_save_process(void);
extern  __u32 dram_power_up_process(void);

extern  void standby_tmr_enable_watchdog(void);
extern  void standby_tmr_disable_watchdog(void);

#endif  // _STANDBY_I_H_



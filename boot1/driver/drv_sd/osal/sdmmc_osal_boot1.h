/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : sdmmc_osal_boot0.h
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
#ifndef  _SDMMC_OSAL_BOOT0_H_
#define  _SDMMC_OSAL_BOOT0_H_

#include "types.h"

extern  void  SDMMC_OpenAHBClock (__u32 card_no);
extern  void  SDMMC_CloseAHBClock(__u32 card_no);
extern  __u32 SDMMC_GetCmuClk    (void);
extern  __s32 SDMMC_OpenClock    (__u32 sdmmc_max_clock, __u32 cmu_clk, __u32 card_no);
extern  __s32 SDMMC_CloseClock   (__u32 card_no);
extern  void  SDMMC_SetPinOn     (__u32 gpio_no, __u32 line_sel);
extern  void  SDMMC_SetPinOff    (__u32 gpio_no, __u32 line_sel);

extern  __s32 OSAL_printf(const char * str, ...);

#endif  /* _SDMMC_OSAL_BOOT0_H_ */

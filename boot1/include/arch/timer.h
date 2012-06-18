/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eBIOS Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : ebios
* File    : timer.h
*
* By      : Steven
* Version : v1.0
* Date    : 2008-9-5 14:31:03
**********************************************************************************************************************
*/
#ifndef _TIMER_H_
#define _TIMER_H_

  /* base */
#define bTMRC_CTL(Nb)	__REG(TMRC_REGS_BASE + (Nb))

/* Registers */
#define TMRC_REG_CTRL           bTMRC_CTL(0x00)
#define TMRC_REG_STATUS         bTMRC_CTL(0x04)
#define TMRC_REG_TM0CTRL        bTMRC_CTL(0x10)
#define TMRC_REG_TM0IVALUE      bTMRC_CTL(0x14)
#define TMRC_REG_TM0CVALUE      bTMRC_CTL(0x18)

#define RTC_YMD_REG             bTMRC_CTL(0xC4)
#define RTC_HMS_REG             bTMRC_CTL(0xC8)

#define WATCHDOG_REG_CTRL       bTMRC_CTL(0x90)
#define WATCHDOG_REG_MODE       bTMRC_CTL(0x94)




#endif  /* _TMRC_H_ */


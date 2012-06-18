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
* File    : keyc.h
*
* By      : Steven
* Version : v1.0
* Date    : 2008-9-5 14:31:03
**********************************************************************************************************************
*/
#ifndef _LRADCK_C_H_
#define _LRADCK_C_H_

#include "hd_scrpt.h"


#define   LRADCK_READ_REG(reg)            (reg)
#define   LRADCK_WRITE_REG(reg, val)      ((reg) = (val))
/*offset*/

#define  KEYC_o_CTRL               0x00
#define  KEYC_o_INT                0x04
#define  KEYC_o_INT_STATUS         0x08
#define  KEYC_o_DATA0              0x0c
#define  KEYC_o_DATA1              0x10

#define  KEYC_REG_CTRL            __REG(LRAC_REGS_BASE + KEYC_o_CTRL)
#define  KEYC_REG_INT             __REG(LRAC_REGS_BASE + KEYC_o_INT )
#define  KEYC_REG_INT_STATUS      __REG(LRAC_REGS_BASE + KEYC_o_INT_STATUS )
#define  KEYC_REG_DATA0           __REG(LRAC_REGS_BASE + KEYC_o_DATA0)
#define  KEYC_REG_DATA1           __REG(LRAC_REGS_BASE + KEYC_o_DATA1)


#endif  /* _LRADCK_C_H_ */

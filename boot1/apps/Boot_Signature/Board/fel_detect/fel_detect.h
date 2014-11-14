/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#ifndef  __FEL_DETECT_H__
#define  __FEL_DETECT_H__

extern int check_key_to_fel(void);
extern int check_file_to_fel(char *name);
extern int check_vol_to_fel(unsigned vol_threshold);
extern int check_natch_time(char *file_name, unsigned work_mode);
extern int check_power_status(void);


#endif  //__FEL_DETECT_H__

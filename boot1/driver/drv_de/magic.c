/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod de sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : magic.c
*
* By      : danling
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "drv_de.h"


const eGon2_mod_t modinfo =
{
    {'e','G','o','n','2','d','r','v'},		//.magic
    0x01000000,                				//.version
    EGON2_EMOD_TYPE_DRV_DISP,                //.mod id
    0,                                      //.入口地址，驱动(模块)应该填空
	{                                       //.mif
	    &DRV_DE_INIT,
	    &DRV_DE_EXIT,
	    &DRV_DE_OPEN,
	    &DRV_DE_CLOSE,
	    &DRV_DE_READ,
    	&DRV_DE_WRITE,
	    &DRV_DE_IOCTRL,
	    &DRV_DE_Standby
	}
};

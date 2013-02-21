/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : BootMain.c
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
#include "include.h"
/*
*******************************************************************************
*                     BootMain
*
* Description:
*    BOOT主应用程序
*
* Parameters:
*    void
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
int BootMain(int argc, char **argv)
{
	__inf("BootBurn start\n");
	while( 1 )
	{
		char ch;

		ch = wBoot_getc_delay( 1 );  // 27000000
	    __inf("%d\n", ch);
	    if(ch == '-')
	    {
	        __u32 dbg = 0x55;

            __inf("for debug\n");
	        while(dbg == 0x55);
	        break;
	    }
	    else
	    {
			break;
	    }
	}
    check_private_part(0);
    __inf("try to power off\n");
    wBoot_timer_delay(50);
    wBoot_power_set_off();

	return 0;
}



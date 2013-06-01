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
#include "include.h"
#include "board.h"
#include "common_res.h"
#include <math.h>

boot_hardware_res     board_res;
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

void mathlib_test(void)
{
    double x,y;
    __inf("begain mathlib_test!\n");
    __s32 print_x,print_y;
    for(x=0;x<6.28;x+=0.1)
    {
        y=sin(x);
        print_x=x*1000;
        print_y=y*1000;
        __inf("sin(%d)=%d\n",print_x,print_y);
    }
    __inf("mathlib test done!\n");
}

int BootMain(int argc, char **argv)
{
	DMSG_INFO("\n\n\n[MSG]: BootMain start\n");
	while( 1 )
	{
		char ch;

		ch = wBoot_getc_delay( 1 );  // 27000000
	    __inf("%d\n", ch);
	    if( ch == '2' )
	    {
	    	__inf("Jump to fel\n");
			{
				__s32 tick;

				for(tick = 10000; tick>0; tick --);
			}
	    	wBoot_jump_fel( );
	    }
	    else if(ch == '-')
	    {
	        __u32 dbg = 0x55;

            __inf("for debug\n");
	        while(dbg == 0x55);
	        break;
	    }
	    else if(ch == '1')
	    {
	    	usb_start(0);
    		usb_run();
    		break;
	    }
	    else
	    {
			break;
	    }
	}
	//关闭DCACHE
    wlibc_DCacheDisable();
	BoardInit_Display(0, 0);           //初始化显示
		mathlib_test();
//    if(Amain() == 1)
//    	__dinf("test ok\n");
//    else
//    	__dinf("test fail\n");
//    if(qc_test() == 1)
	if(1)
    	__dinf("test ok\n");
    else
    	__dinf("test fail\n");
    //开始准备系统数据
    return 0;
}




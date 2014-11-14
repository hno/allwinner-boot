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
#include "include.h"

extern  volatile __u32  boot_standby_action;
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static void power_int_irq(void *p_arg)
{
	__u8 power_int_status[8];
	__u32 dcin, bat_exist;
//	int i;

	wBoot_power_int_query(power_int_status);
//	for(i=0;i<5;i++)
//	{
//		__inf("int status %d %x\n", i, power_int_status[i]);
//	}
	if(power_int_status[0] & 0x48)	//dc插入或者usb插入中断
	{
		wBoot_power_get_dcin_battery_exist(&dcin, &bat_exist);
		if(dcin)
		{
			__inf("power found\n");
			boot_standby_action &= ~0x10;
			boot_standby_action |= 4;
		}
	}
	if(power_int_status[0] & 0x8)   //usb 插入中断，启动usb检测
	{
		__inf("usb in\n");
		boot_standby_action |= 8;
		usb_detect_enter();
	}
	if(power_int_status[0] & 0x4)
	{
		__inf("usb out\n");
		boot_standby_action &= ~0x04;
		boot_standby_action |= 0x10;
		usb_detect_exit();
	}
	if(power_int_status[2] & 0x2)	//短按键
	{
		__inf("short key\n");
		boot_standby_action |= 2;

	}
	if(power_int_status[2] & 0x1)	//长按键
	{
		__inf("long key\n");
		boot_standby_action |= 1;
	}

	return;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void power_limit_detect_enter(void)
{
	__u8  power_int_enable[8];

	power_int_enable[0] = 0x4C;  //dc in, usb in/out
	power_int_enable[1] = 0;
	power_int_enable[2] = 3;
	power_int_enable[4] = 0;
	power_int_enable[5] = 0;

	__inf("power_limit_detect_enter\n");

	wBoot_power_int_enable(power_int_enable);
	wBoot_InsINT_Func(AW_IRQ_NMI, (int *)power_int_irq, 0);
	wBoot_EnableInt(AW_IRQ_NMI);
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
void power_limit_detect_exit(void)
{
	usb_detect_exit();
	wBoot_power_int_disable();
	wBoot_DisableInt(AW_IRQ_NMI);
}


/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON libc sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : eGon2_common.c
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
#include "egon2.h"
#include "syscall_i.h"

//__swi(EGON2_SWI_POWER_GET_SOURCE)       int   wBoot_power_get_dcin_battery_exist(unsigned int * /*dcin_exist*/, unsigned int */*battery_exist*/);
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
int wBoot_power_get_dcin_battery_exist(unsigned int * dcin_exist, unsigned int *battery_exist)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_GET_SOURCE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_GET_BAT_VOL)      int   wBoot_power_get_battery_vol       (unsigned int * /*battery_vol*/);
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
int wBoot_power_get_battery_vol(unsigned int *battery_vol)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_GET_BAT_VOL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_GET_KEY)          int   wBoot_power_get_key               (void);
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
int wBoot_power_get_key(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_GET_KEY));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_CHECK_STARTUP)    int   wBoot_power_check_startup         (void);
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
int wBoot_power_check_startup(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_CHECK_STARTUP));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_SET_SW1)          int   wBoot_power_set_sw1               (unsigned int on_off);
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
int wBoot_power_set_sw1(unsigned int on_off)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_SET_SW1));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_SET_OFF)          int   wBoot_power_set_off               (void);
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
int wBoot_power_set_off(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_SET_OFF));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_GET_LEVEL_STATUS) int   wBoot_power_get_level				(void);
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
int wBoot_power_get_level(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_GET_LEVEL_STATUS));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_SET_OFF_VOL)		int	  wBoot_power_set_off_vol			(void);
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
int wBoot_power_set_off_vol(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_SET_OFF_VOL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_BATTERY_CAL)		int   wBoot_power_get_cal				(void);
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
int wBoot_power_get_cal(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_BATTERY_CAL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_CUR_LIMIT)		int	  wBoot_power_cur_limit				(int current);
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
int wBoot_power_cur_limit(int current)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_CUR_LIMIT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_VOL_LIMIT)		int   wBoot_power_vol_limit				(int vol);
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
int wBoot_power_vol_limit(int vol)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_VOL_LIMIT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_POWER_TYPE)				int   wBoot_power_type					(void);
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
int wBoot_power_type(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_POWER_TYPE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}





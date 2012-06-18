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

//__swi(EGON2_SWI_SCRIPT_FETCH)              int  wBoot_script_parser_fetch(char *main_name, char *sub_name, int value[], int count);
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
int wBoot_script_parser_fetch(char *main_name, char *sub_name, int value[], int count)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_SCRIPT_FETCH));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_SCRIPT_PATCH)              int  wBoot_script_parser_patch(char *main_name, char *sub_name, int value);
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
int wBoot_script_parser_patch(char *main_name, char *sub_name, int value)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_SCRIPT_PATCH));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_SCRIPT_SUBKEY_COUNT)       int  wBoot_script_parser_subkey_count(char *main_name);
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
int wBoot_script_parser_subkey_count(char *main_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_SCRIPT_SUBKEY_COUNT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_SCRIPT_MAINKEY_COUNT)      int  wBoot_script_parser_mainkey_count(void);
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
int wBoot_script_parser_mainkey_count(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_SCRIPT_MAINKEY_COUNT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_SCRIPT_MAINKEY_GPIO_COUNT) int  wBoot_script_parser_mainkey_get_gpio_count(char *main_name);
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
int wBoot_script_parser_mainkey_get_gpio_count(char *main_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_SCRIPT_MAINKEY_GPIO_COUNT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_SCRIPT_MAINKEY_GPIO_DATA)  int  wBoot_script_parser_mainkey_get_gpio_cfg(char *main_name, void *gpio_cfg, int gpio_count);
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
int wBoot_script_parser_mainkey_get_gpio_cfg(char *main_name, void *gpio_cfg, int gpio_count)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_SCRIPT_MAINKEY_GPIO_DATA));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}



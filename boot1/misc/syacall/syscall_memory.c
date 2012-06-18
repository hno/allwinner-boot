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

//__swi(EGON2_SWI_MALLOC)                 void *wBoot_malloc          (unsigned int /* num_bytes */                              );
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
void *wBoot_malloc(unsigned int num_bytes)
{
	__u32 ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_MALLOC));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return (void *)ret;
}
//__swi(EGON2_SWI_CALLOC)                 void *wBoot_calloc          (unsigned int /* size      */, unsigned int /* num_bytes */);
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
void *wBoot_calloc(unsigned int size, unsigned int num_bytes)
{
	__u32 ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_CALLOC));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return (void *)ret;
}
//__swi(EGON2_SWI_REALLOC)                void *wBoot_realloc         (void *       /* p         */, unsigned int /* num_bytes */);
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
void *wBoot_realloc(void *p, unsigned int num_bytes)
{
	__u32 ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_REALLOC));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return (void *)ret;
}
//__swi(EGON2_SWI_FREE)                   void  wBoot_free            (void *       /* p         */                              );
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
void wBoot_free(void *p)
{
	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_FREE));
	asm("ldmfd sp!, {lr}");
}




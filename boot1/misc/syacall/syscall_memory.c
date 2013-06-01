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




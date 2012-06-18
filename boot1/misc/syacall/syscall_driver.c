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
int wBoot_driver_install(void *class_header)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_REG));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_driver_uninstall(unsigned int mod_id)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_UNREG));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
unsigned int wBoot_driver_open(unsigned int mod_id, void *open_arg)
{
	unsigned int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_OPEN));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_driver_close(unsigned int id)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_CLOSE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_driver_read(void *pBuffer, unsigned int blk, unsigned int n, unsigned int hd)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_READ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_driver_write(void *pBuffer, unsigned int blk, unsigned int n, unsigned int hd)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_WRITE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_driver_ioctl(unsigned int hd, unsigned int cmd, int aux, void *pBuffer)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DRV_IOCTL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_run_app(int argc, char **argv)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_RUN_APP));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_size(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_SIZE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_init(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_DEV_INIT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_read(unsigned int blk_start, unsigned int nblock, void *pBuffer)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_DEV_READ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_write(unsigned int blk_start, unsigned int nblock, void *pBuffer)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_DEV_WRITE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_dread(unsigned int blk_start, unsigned int nblock, void *pBuffer)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_DDEV_READ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_dwrite(unsigned int blk_start, unsigned int nblock, void *pBuffer)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_DDEV_WRITE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
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
int wBoot_block_exit(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_BLOCK_DEV_EXIT));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}




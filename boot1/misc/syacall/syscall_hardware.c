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

//__swi(EGON2_SWI_TWI_READ   )            int   wBoot_twi_read   (void  *      /* twi_arg   */);
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
int wBoot_twi_read(void *twi_arg)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_TWI_READ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_TWI_WRITE  )            int   wBoot_twi_write  (void  *      /* twi_arg   */);
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
int wBoot_twi_write(void *twi_arg)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_TWI_WRITE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
////__swi(EGON2_SWI_TWI_INIT   )            int   wBoot_twi_init   (unsigned int /* twi_index */, unsigned int /* clock  */);
///*
//************************************************************************************************************
//*
//*                                             function
//*
//*    函数名称：
//*
//*    参数列表：
//*
//*    返回值  ：
//*
//*    说明    ：
//*
//*
//************************************************************************************************************
//*/
//int wBoot_twi_init(unsigned int twi_index, unsigned int clock)
//{
//	int ret;
//
//  asm("stmfd sp!, {lr}");
//	asm("swi %0"::"Ir"(EGON2_SWI_TWI_INIT));
//	asm("mov %0, r0":"=r"(ret));
//	asm("ldmfd sp!, {lr}");
//
//	return ret;
//}
////__swi(EGON2_SWI_TWI_EXIT   )            int   wBoot_twi_exit   (void                        );
///*
//************************************************************************************************************
//*
//*                                             function
//*
//*    函数名称：
//*
//*    参数列表：
//*
//*    返回值  ：
//*
//*    说明    ：
//*
//*
//************************************************************************************************************
//*/
//int wBoot_twi_exit(void)
//{
//	int ret;
//
//  asm("stmfd sp!, {lr}");
//	asm("swi %0"::"Ir"(EGON2_SWI_TWI_EXIT));
//	asm("mov %0, r0"::"r"(ret));
//	asm("mov %0, r0":"=r"(ret));
//	asm("ldmfd sp!, {lr}");
//
//	return ret;
//}
////__swi(EGON2_SWI_TWI_SETFREQ)            int   wBoot_twi_setfreq(unsigned int /* twi run clock*/);
///*
//************************************************************************************************************
//*
//*                                             function
//*
//*    函数名称：
//*
//*    参数列表：
//*
//*    返回值  ：
//*
//*    说明    ：
//*
//*
//************************************************************************************************************
//*/
//int wBoot_twi_setfreq(unsigned int twi_run_clock)
//{
//	int ret;
//
//  asm("stmfd sp!, {lr}");
//	asm("swi %0"::"Ir"(EGON2_SWI_TWI_SETFREQ));
//	asm("mov %0, r0":"=r"(ret));
//	asm("ldmfd sp!, {lr}");
//
//	return ret;
//}
//__swi(EGON2_SWI_TIMER_REQ)              unsigned int wBoot_timer_request(void *       /* func_addr */, void * /* p_arg */);
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
unsigned int wBoot_timer_request(void *func_addr, void *p_arg)
{
	unsigned int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_TIMER_REQ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_TIMER_REL)              int          wBoot_timer_release(unsigned int /* hd        */);
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
int wBoot_timer_release(unsigned int  hd)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_TIMER_REL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_TIMER_START)            int          wBoot_timer_start  (unsigned int /* hd        */, int /* delay_time*/, int /* auto_restart */);
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
int wBoot_timer_start(unsigned int hd, int delay_time, int auto_restart)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_TIMER_START));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_TIMER_STOP)             int          wBoot_timer_stop   (unsigned int /* hd        */);
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
int wBoot_timer_stop(unsigned int hd)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_TIMER_STOP));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_REQ)               unsigned int wBoot_dma_request  (unsigned int /* dmatype */                                );
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
unsigned int wBoot_dma_request(unsigned int dma_type)
{
	unsigned int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_REQ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_REL)               int          wBoot_dma_release  (unsigned int /* hDma    */                                );
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
int wBoot_dma_release(unsigned int hdma)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_REL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_START)             int          wBoot_dma_start    (unsigned int /* hDMA    */, unsigned int /* saddr */, unsigned int /* daddr */, unsigned int /* bytes */);
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
int wBoot_dma_start(unsigned int hDMA, unsigned int saddr, unsigned int daddr, unsigned int bytes)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_START));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_STOP)              int          wBoot_dma_stop     (unsigned int /* hDMA    */                                );
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
int wBoot_dma_stop(unsigned int hDMA)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_STOP));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_SETTING)           unsigned int wBoot_dma_Setting  (unsigned int /* hDMA    */, void *       /* cfg    */     );
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
int wBoot_dma_Setting(unsigned int hDMA, void *cfg)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_SETTING));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_RESTART)           int          wBoot_dma_Restart  (unsigned int /* hDMA    */                                );
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
int wBoot_dma_Restart(unsigned int hDMA)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_RESTART));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_QUERYSTATE)        int          wBoot_dma_QueryState(unsigned int /* hDMA    */                               );
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
int wBoot_dma_QueryState(unsigned int hDMA)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_QUERYSTATE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_DMA_QUERYCHAN)         int          wBoot_dma_QueryChan(unsigned int /* hDMA    */                                );
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
int wBoot_dma_QueryChan(unsigned int hDMA)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DMA_QUERYCHAN));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_IRQ_REG)			    int wBoot_InsINT_Func     (unsigned int /* irq_no */, int * /* func_addr */, void * /* p_arg */);
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
int wBoot_InsINT_Func(unsigned int irq_no, int *func_addr, void *p_arg)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_IRQ_REG));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_IRQ_UNREG)		        int wBoot_UnsInt_Func     (unsigned int /* irq_no */                                          );
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
int wBoot_UnsInt_Func(unsigned int irq_no)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_IRQ_UNREG));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_ENABLE_IRQ)            int wBoot_EnableInt       (unsigned int /* irq_no */                                          );
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
int wBoot_EnableInt(unsigned int irq_no)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_ENABLE_IRQ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_DISABLE_IRQ)           int wBoot_DisableInt      (unsigned int /* irq_no */                                          );
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
int wBoot_DisableInt(unsigned int irq_no)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_DISABLE_IRQ));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}



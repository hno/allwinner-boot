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

//__swi(EGON2_SWI_KEY_GET_STATUS)               int wBoot_key_get_status               (void);
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
int wBoot_key_get_status(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_KEY_GET_STATUS));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_KEY_GET_VALUE)				  int wBoot_key_get_value				 (void);
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
int wBoot_key_get_value(void)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_KEY_GET_VALUE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_UART_DEBUG)			void         wBoot_uprintf      ( const char * /* str */                    );
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
void wBoot_uprintf(const char *str)
{
	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_UART_DEBUG));
	asm("ldmfd sp!, {lr}");
}
//__swi (EGON2_SWI_UART_NTDEBUG)			void		 wBoot_ntprintf     ( const char * /* str */                    );
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
void wBoot_ntprintf(const char *str)
{
	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_UART_NTDEBUG));
	asm("ldmfd sp!, {lr}");
}
////__swi (EGON2_SWI_DISPLAY_DEBUG)		void wBoot_printf               ( const char * /* str */                    );
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
//void wBoot_printf(void)
//{
//  asm("stmfd sp!, {lr}");
//	asm("swi %0"::"Ir"(EGON2_SWI_DISPLAY_DEBUG));
//  asm("ldmfd sp!, {lr}");
//}
//__swi (EGON2_SWI_UART_GETC )            char         wBoot_getc         ( void                                      );
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
char wBoot_getc(void)
{
	char ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_UART_GETC));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_UART_GETS )            unsigned int wBoot_getall       ( char *       /* str */                    );
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
unsigned int wBoot_getall(void)
{
	unsigned int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_UART_GETS));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi (EGON2_SWI_UART_PUTC )            void         wBoot_putc         ( char         /* str */                    );
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
void wBoot_putc(char *str)
{
	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_UART_PUTC));
	asm("ldmfd sp!, {lr}");
}
//__swi (EGON2_SWI_UART_GETC_DELAY )      char         wBoot_getc_delay   ( unsigned int /* delay */					);
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
char wBoot_getc_delay(unsigned int delay)
{
	char ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_UART_GETC_DELAY));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}


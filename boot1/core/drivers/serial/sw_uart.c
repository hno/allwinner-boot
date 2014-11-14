/*
 * COM1 SW161x support
 * originally from linux source (arch/ppc/boot/ns16550.c)
 * modified to use CFG_ISA_MEM and new defines
 */

#include "include.h"
#include "sw_uart.h"

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
int sw_uart_init (sw_uart_t com_port, int uart_port, void  *uart_ctrl, int baud_divisor)
{
	CCMU_REG_APB2_GATING &= ~(1 << (16 + uart_port));
    eGon2_timer_delay(5);
    CCMU_REG_APB2_GATING |=  (1 << (16 + uart_port));

	(*(volatile unsigned int *)0x01c202D8) |= (1 << (16 + uart_port));

	//eGon2_GPIO_Set_cfg((normal_gpio_cfg *)uart_ctrl, 2, 1);

	com_port->ier = 0x00;
	com_port->lcr = LCR_BKSE | LCRVAL;
	com_port->dll = baud_divisor & 0xff;
	com_port->dlh = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCRVAL;
	com_port->mcr = MCRVAL;
	com_port->fcr = FCRVAL;

	return 0;
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
int sw_uart_exit(int uart_port)
{
	CCMU_REG_APB2_GATING &= ~(1 << (16 + uart_port));

	return 0;
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
void sw_uart_reinit (sw_uart_t com_port, int baud_divisor)
{
	com_port->ier = 0x00;
	com_port->lcr = LCR_BKSE;
	com_port->dll = baud_divisor & 0xff;
	com_port->dlh = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCRVAL;
	com_port->mcr = MCRVAL;
	com_port->fcr = FCRVAL;
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
void sw_uart_putc (sw_uart_t com_port, char c)
{
	while ((com_port->usr & TXFIFO_FULL) == 0);
	com_port->thr = c;
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
char sw_uart_getc (sw_uart_t com_port)
{
    if((com_port->usr & RXFIFO_EMPTY) == 0)
    {
        return 0;
    }

	return (com_port->rbr);
}




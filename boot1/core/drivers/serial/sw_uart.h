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
#ifndef _SW_UART_H_
#define _SW_UART_H_  1

#define CFG_SW_SERIAL_MAX      4
#define CFG_SW_SERIAL_COM0	   UART0_REGS_BASE	      /* uart0 */
#define CFG_SW_SERIAL_COM1	   UART1_REGS_BASE	      /* uart1 */
#define CFG_SW_SERIAL_COM2	   UART2_REGS_BASE	      /* uart2 */
#define CFG_SW_SERIAL_COM3	   UART3_REGS_BASE	      /* uart3 */
#define CFG_SW_SERIAL_COM4	   UART4_REGS_BASE	      /* uart4 */
#define CFG_SW_SERIAL_COM5	   UART5_REGS_BASE	      /* uart5 */
#define CFG_SW_SERIAL_COM6	   UART6_REGS_BASE	      /* uart6 */
#define CFG_SW_SERIAL_COM7	   UART7_REGS_BASE	      /* uart7 */

struct sw_uart
{
    volatile unsigned int rbr;       /* RBR, THR, DLL */
    volatile unsigned int dlh;       /* DLH, IER      */
    volatile unsigned int iir;       /* IIR, FCR      */
    volatile unsigned int lcr;       /* LCR           */
    volatile unsigned int mcr;       /* MCR           */
    volatile unsigned int lsr;       /* LSR           */
    volatile unsigned int msr;       /* MSR           */
    volatile unsigned int sch;       /* SCH           */
    volatile unsigned int dump[23];  /* reserved bytes*/
    volatile unsigned int usr;       /* USR           */
    volatile unsigned int tfl;       /* TFL           */
    volatile unsigned int rfl;       /* RFL           */
};

typedef volatile struct sw_uart *sw_uart_t;

#define thr rbr
#define dll rbr
#define ier dlh
#define fcr iir

#define FCR_FIFO_EN     0x01		/* Fifo enable */
#define FCR_RXSR        0x02		/* Receiver soft reset */
#define FCR_TXSR        0x04		/* Transmitter soft reset */

#define MCR_DTR         0x01
#define MCR_RTS         0x02
#define MCR_DMA_EN      0x04
#define MCR_TX_DFR      0x08

#define LCR_WLS_MSK	    0x03		/* character length select mask */
#define LCR_WLS_5	    0x00		/* 5 bit character length */
#define LCR_WLS_6	    0x01		/* 6 bit character length */
#define LCR_WLS_7	    0x02		/* 7 bit character length */
#define LCR_WLS_8	    0x03		/* 8 bit character length */
#define LCR_STB		    0x04		/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN		    0x08		/* Parity eneble */
#define LCR_EPS		    0x10		/* Even Parity Select */
#define LCR_STKP	    0x20		/* Stick Parity */
#define LCR_SBRK	    0x40		/* Set Break */
#define LCR_BKSE	    0x80		/* Bank select enable */

#define LSR_DR		    0x01		/* Data ready */
#define LSR_OE		    0x02		/* Overrun */
#define LSR_PE		    0x04		/* Parity error */
#define LSR_FE		    0x08		/* Framing error */
#define LSR_BI		    0x10		/* Break */
#define LSR_THRE	    0x20		/* Xmit holding register empty */
#define LSR_TEMT	    0x40		/* Xmitter empty */
#define LSR_ERR		    0x80		/* Error */

#define RXFIFO_FULL     0x10      /* Rx Fifo check full  */
#define RXFIFO_EMPTY    0x08      /* Rx Fifo check empty */
#define TXFIFO_EMPTY    0x04      /* Tx Fifo check empty */
#define TXFIFO_FULL     0x02      /* Tx Fifo check full  */
#define UART_BUSY       0x01      /* uart control busy   */

#define LCRVAL          0x03					                /* 8 data, 1 stop, no parity */
#define MCRVAL          (MCR_DTR | MCR_RTS)			        /* RTS/DTR */
#define FCRVAL          (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* Clear & enable FIFOs */


/* useful defaults for LCR */
#define LCR_8N1		0x03

int 	sw_uart_init   (sw_uart_t com_port, int uart_port, void  *uart_ctrl, int baud_divisor);
int     sw_uart_exit   (int       uart_port);
void	sw_uart_putc   (sw_uart_t com_port, char c);
char	sw_uart_getc   (sw_uart_t com_port);
int		sw_uart_tstc   (sw_uart_t com_port);
void	sw_uart_reinit (sw_uart_t com_port, int baud_divisor);

#endif   /* _SW_UART_H_ */

/* end of sw_uart.h */

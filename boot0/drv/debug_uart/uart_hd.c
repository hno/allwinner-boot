/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
*File Name : uart.c
*
*Author : Gary.Wang
*
*Version : 1.1.0
*
*Date : 2009.09.13
*
*Description :
*
*Others : None at present.
*
*
* History :
*
*  <Author>         <time>      <version>     <description>
*
* Wangyugang      2009.09.13      1.1.0        build the file
*
*******************************************************************************************************************
*/
//#define __FPGA__

#include "boot0_drv_i.h"

#define   UART_OFFSET   0x400
#define   TxFIFOSize    1024
//UART Baudrate Control
#define   UART_BAUD    115200      // Baud rate for UART
                                   // Compute the divisor factor
// UART Line Control Parameter
#define   PARITY       0           // Parity: 0,2 - no parity; 1 - odd parity; 3 - even parity
#define   STOP         0           // Number of Stop Bit: 0 - 1bit; 1 - 2(or 1.5)bits
#define   DLEN         3           // Data Length: 0 - 5bits; 1 - 6bits; 2 - 7bits; 3 - 8bits

//#ifndef __FPGA__
//#define SUART_REGS_BASE   0x01c28000
//#else
//#define SUART_REGS_BASE   0x01f02800
//#endif
#define SUART_REGS_BASE   0x01c28000

#define UART_REG_o_RBR              0x00
#define UART_REG_o_THR              0x00
#define UART_REG_o_DLL              0x00
#define UART_REG_o_DLH              0x04
#define UART_REG_o_IER              0x04
#define UART_REG_o_IIR              0x08
#define UART_REG_o_FCR              0x08
#define UART_REG_o_LCR              0x0C
#define UART_REG_o_MCR              0x10
#define UART_REG_o_LSR              0x14
#define UART_REG_o_MSR              0x18
#define UART_REG_o_SCH              0x1C
#define UART_REG_o_USR              0x7C
#define UART_REG_o_TFL              0x80
#define UART_REG_o_RFL              0x84
#define UART_REG_o_HALT             0xA4

#define UART_REG_RBR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_RBR  )
#define UART_REG_THR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_THR  )
#define UART_REG_DLL(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_DLL  )
#define UART_REG_DLH(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_DLH  )
#define UART_REG_IER(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_IER  )
#define UART_REG_IIR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_IIR  )
#define UART_REG_FCR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_FCR  )
#define UART_REG_LCR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_LCR  )
#define UART_REG_MCR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_MCR  )
#define UART_REG_LSR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_LSR  )
#define UART_REG_MSR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_MSR  )
#define UART_REG_SCH(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_SCH  )
#define UART_REG_USR(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_USR  )
#define UART_REG_TFL(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_TFL  )
#define UART_REG_RFL(port)          __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_RFL  )
#define UART_REG_HALT(port)         __REG( SUART_REGS_BASE + port * UART_OFFSET + UART_REG_o_HALT )

#define SERIAL_CHAR_READY()     ( UART_REG_LSR(port) & ( 1 << 0 ) )
#define SERIAL_READ_CHAR()      UART_REG_RBR(port)
#define SERIAL_READ_STATUS()    ( UART_REG_LSR(port) & 0xFF )
#define SERIAL_WRITE_STATUS()	UART_REG_LSR(port)
#define SERIAL_WRITE_READY()	( UART_REG_LSR(port) & ( 1 << 6 ) )
#define SERIAL_WRITE_CHAR(c)	( ( UART_REG_THR(port) ) = ( c ) )


static  __u32  port = 0;


void UART_open( __s32 uart_port, void  *uart_ctrl, __u32 apb_freq )
{
  	__u32   temp=0, i;
  	__u32   uart_clk;
  	__u32   lcr;
  	volatile unsigned int   *reg;

	port = uart_port;

	// config clock
	if(port > 7)
	{
		return ;
	}
	reg = (volatile unsigned int *)0x01c2006C;
	*reg &= ~(1 << (16 + port));
	for( i = 0; i < 100; i++ );
	*reg |=  (1 << (16 + port));
	// config uart gpio
	// config tx gpio
	boot_set_gpio((normal_gpio_cfg *)uart_ctrl, 2, 1);

    // Set Baudrate
    uart_clk = ( apb_freq + 8*UART_BAUD ) / (16*UART_BAUD);
    lcr = UART_REG_LCR(port);
    UART_REG_HALT(port) = 1;
    UART_REG_LCR(port) = lcr | 0x80;
    UART_REG_DLH(port) = uart_clk>>8;
    UART_REG_DLL(port) = uart_clk&0xff;
    UART_REG_LCR(port) = lcr & (~0x80);
	UART_REG_HALT(port) = 0;
    // Set Lin Control Register
    temp = ((PARITY&0x03)<<3) | ((STOP&0x01)<<2) | (DLEN&0x03);
    UART_REG_LCR(port) = temp;

    // Disable FIFOs
    UART_REG_FCR(port) = 0x06;
}


void UART_close( void )
{
	return ;
}



//char UART_getchar( __u32 timeout )
//{
//	__u32  i;
//
//	if( timeout == 0 )           // infinite wait
//	{
//		while( !SERIAL_CHAR_READY() )
//			;
//		return SERIAL_READ_CHAR();
//	}
//	else
//	{
//		for( i = 0; i < timeout; i++ )
//		{
//			if(SERIAL_CHAR_READY())
//				break;
//		}
//		if( i == timeout )
//			return 0;
//		else
//			return SERIAL_READ_CHAR();
//	}
//}




void UART_putchar(char c)
{
	while (!SERIAL_WRITE_READY())
	  ;                       /* nothing */

	SERIAL_WRITE_CHAR(c);
}

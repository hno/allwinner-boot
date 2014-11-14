/*
************************************************************************************************************************
*                                                  DMAC BSP for sun
*                                 DMAC hardware registers definition and BSP interfaces
*
*                             Copyright(C), 2006-2008, uLIVE Co., Ltd.
*											       All Rights Reserved
*
* File Name : dmac.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2008.05.25
*
* Description : This file provides some definition of DMAC's hardware registers and BSP interfaces.
*             This file is very similar to file "dmac.inc"; the two files should be modified at the
*             same time to keep coherence of information.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.05.25       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef _DMAC_H_
#define _DMAC_H_

/*
*********************************************************************************************************
*   DMA Controller define          < maintained by Steve >
*********************************************************************************************************
*/

#define DMAC_DMATYPE_NORMAL         0
#define DMAC_DMATYPE_DEDICATED      1

/* port */
#define DMA_N0                      0
#define DMA_N1                      1
#define DMA_N2                      2
#define DMA_N3                      3
#define DMA_D0                      0
#define DMA_D1                      1
#define DMA_D2                      2
#define DMA_D3                      3
#define DMA_OFFSET                  0x20

#define  DMA_HDLER_TYPE_CNT                     2

#define  DMA_TRANSFER_HALF_INT       0
#define  DMA_TRANSFER_END_INT        1

#define  DMA_TRANSFER_UNLOOP_MODE   0
#define  DMA_TRANSFER_LOOP_MODE     1


//================================
//======    DMA 配置     =========
//================================

/* DMA 基础配置  */
#define DMAC_CFG_CONTINUOUS_ENABLE              (0x01)	//(0x01<<29)
#define DMAC_CFG_CONTINUOUS_DISABLE             (0x00)	//(0x01<<29)

//* DMA 时钟 */
#define	DMAC_CFG_WAIT_1_DMA_CLOCK				(0x00)	//(0x00<<26)
#define	DMAC_CFG_WAIT_2_DMA_CLOCK				(0x01)	//(0x01<<26)
#define	DMAC_CFG_WAIT_3_DMA_CLOCK				(0x02)	//(0x02<<26)
#define	DMAC_CFG_WAIT_4_DMA_CLOCK				(0x03)	//(0x03<<26)
#define	DMAC_CFG_WAIT_5_DMA_CLOCK				(0x04)	//(0x04<<26)
#define	DMAC_CFG_WAIT_6_DMA_CLOCK				(0x05)	//(0x05<<26)
#define	DMAC_CFG_WAIT_7_DMA_CLOCK				(0x06)	//(0x06<<26)
#define	DMAC_CFG_WAIT_8_DMA_CLOCK				(0x07)	//(0x07<<26)

/* DMA 传输目的端 配置 */
/* DMA 目的端 传输宽度 */
#define	DMAC_CFG_DEST_DATA_WIDTH_8BIT			(0x00)	//(0x00<<24)
#define	DMAC_CFG_DEST_DATA_WIDTH_16BIT			(0x01)	//(0x01<<24)
#define	DMAC_CFG_DEST_DATA_WIDTH_32BIT			(0x02)	//(0x02<<24)

/* DMA 目的端 突发传输模式 */
#define	DMAC_CFG_DEST_1_BURST       			(0x00)	//(0x00<<23)
#define	DMAC_CFG_DEST_4_BURST		    		(0x01)	//(0x01<<23)

/* DMA 目的端 地址变化模式 */
#define	DMAC_CFG_DEST_ADDR_TYPE_LINEAR_MODE		(0x00)	//(0x00<<21)
#define	DMAC_CFG_DEST_ADDR_TYPE_IO_MODE 		(0x01)	//(0x01<<21)
#define	DMAC_CFG_DEST_ADDR_TYPE_HPAGE_MODE 		(0x02)	//(0x02<<21)
#define	DMAC_CFG_DEST_ADDR_TYPE_VPAGE_MODE 		(0x03)	//(0x03<<21)


/* DMA 传输源端 配置 */
/* DMA 源端 传输宽度 */
#define	DMAC_CFG_SRC_DATA_WIDTH_8BIT			(0x00)	//(0x00<<8)
#define	DMAC_CFG_SRC_DATA_WIDTH_16BIT			(0x01)	//(0x01<<8)
#define	DMAC_CFG_SRC_DATA_WIDTH_32BIT			(0x02)	//(0x02<<8)

/* DMA 源端 突发传输模式 */
#define	DMAC_CFG_SRC_1_BURST       				(0x00)	//(0x00<<7)
#define	DMAC_CFG_SRC_4_BURST		    		(0x01)	//(0x01<<7)

/* DMA 源端 地址变化模式 */
#define	DMAC_CFG_SRC_ADDR_TYPE_LINEAR_MODE		(0x00)	//(0x00<<5)
#define	DMAC_CFG_SRC_ADDR_TYPE_IO_MODE 			(0x01)	//(0x01<<5)
#define	DMAC_CFG_SRC_ADDR_TYPE_HPAGE_MODE 		(0x02)	//(0x02<<5)
#define	DMAC_CFG_SRC_ADDR_TYPE_VPAGE_MODE 		(0x03)	//(0x03<<5)


/* DMA 传输目的端 配置 */
/* DMA 传输目的端 N型DMA 目的选择 */
#define	DMAC_CFG_DEST_TYPE_IR					(0x00)	//(0x00<<16)
#define	DMAC_CFG_DEST_TYPE_SPDIF		    	(0x01)	//(0x01<<16)
#define	DMAC_CFG_DEST_TYPE_IIS			    	(0x02)	//(0x02<<16)
#define	DMAC_CFG_DEST_TYPE_AC97			    	(0x03)	//(0x03<<16)
#define	DMAC_CFG_DEST_TYPE_SPI0				    (0x04)	//(0x04<<16)
#define	DMAC_CFG_DEST_TYPE_SPI1				    (0x05)	//(0x05<<16)
#define	DMAC_CFG_DEST_TYPE_SPI2				    (0x06)	//(0x06<<16)
#define	DMAC_CFG_DEST_TYPE_UART0				(0x08)	//(0x08<<16)
#define	DMAC_CFG_DEST_TYPE_UART1				(0x09)	//(0x09<<16)
#define	DMAC_CFG_DEST_TYPE_UART2				(0x0a)	//(0x0a<<16)
#define	DMAC_CFG_DEST_TYPE_UART3				(0x0b)	//(0x0b<<16)
#define	DMAC_CFG_DEST_TYPE_AUDIO_DA				(0x0c)	//(0x0c<<16)

#define	DMAC_CFG_DEST_TYPE_NFC_DEBUG			(0x0f)	//(0x0f<<16)
#define	DMAC_CFG_DEST_TYPE_N_SRAM 				(0x10)	//(0x10<<16)
#define	DMAC_CFG_DEST_TYPE_N_SDRAM				(0x11)	//(0x11<<16)
#define	DMAC_CFG_DEST_TYPE_UART4				(0x12)	//(0x12<<16)
#define	DMAC_CFG_DEST_TYPE_UART5				(0x13)	//(0x13<<16)
#define	DMAC_CFG_DEST_TYPE_UART6				(0x14)	//(0x14<<16)
#define	DMAC_CFG_DEST_TYPE_UART7				(0x15)	//(0x15<<16)

/* DMA 传输目的端 D型DMA 目的选择 */
#define	DMAC_CFG_DEST_TYPE_D_SRAM 				(0x00)	//(0x00<<16)
#define	DMAC_CFG_DEST_TYPE_D_SDRAM				(0x01)	//(0x01<<16)
#define	DMAC_CFG_DEST_TYPE_TCON0				(0x02)	//(0x02<<16)
#define	DMAC_CFG_DEST_TYPE_NFC  		    	(0x03)	//(0x03<<16)
#define	DMAC_CFG_DEST_TYPE_USB0			    	(0x04)	//(0x04<<16)
#define	DMAC_CFG_DEST_TYPE_USB1			    	(0x05)	//(0x05<<16)
#define	DMAC_CFG_DEST_TYPE_SDC1			    	(0x07)	//(0x07<<16)
#define	DMAC_CFG_DEST_TYPE_SDC2 				(0x08)	//(0x08<<16)
#define	DMAC_CFG_DEST_TYPE_SDC3 				(0x09)	//(0x09<<16)
#define	DMAC_CFG_DEST_TYPE_MSC  				(0x0a)	//(0x0a<<16)
#define	DMAC_CFG_DEST_TYPE_EMAC 				(0x0b)	//(0x0b<<16)
#define	DMAC_CFG_DEST_TYPE_SS   				(0x0d)	//(0x0d<<16)
#define	DMAC_CFG_DEST_TYPE_USB2			    	(0x0f)	//(0x0f<<16)
#define	DMAC_CFG_DEST_TYPE_ATA			    	(0x10)	//(0x10<<16)

/* DMA 传输源端 配置 */
/* DMA 传输源端 N型DMA 目的选择 */
#define	DMAC_CFG_SRC_TYPE_IR					(0x00)	//(0x00<<0)
#define	DMAC_CFG_SRC_TYPE_SPDIF		    	   	(0x01)	//(0x01<<0)
#define	DMAC_CFG_SRC_TYPE_IIS			    	(0x02)	//(0x02<<0)
#define	DMAC_CFG_SRC_TYPE_AC97			    	(0x03)	//(0x03<<0)
#define	DMAC_CFG_SRC_TYPE_SPI0				    (0x04)	//(0x04<<0)
#define	DMAC_CFG_SRC_TYPE_SPI1				    (0x05)	//(0x05<<0)
#define	DMAC_CFG_SRC_TYPE_SPI2				    (0x06)	//(0x06<<0)
#define	DMAC_CFG_SRC_TYPE_UART0				    (0x08)	//(0x08<<0)
#define	DMAC_CFG_SRC_TYPE_UART1				    (0x09)	//(0x09<<0)
#define	DMAC_CFG_SRC_TYPE_UART2				    (0x0a)	//(0x0a<<0)
#define	DMAC_CFG_SRC_TYPE_UART3				    (0x0b)	//(0x0b<<0)
#define	DMAC_CFG_SRC_TYPE_AUDIO 				(0x0c)	//(0x0c<<0)
#define	DMAC_CFG_SRC_TYPE_TP     				(0x0d)	//(0x0d<<0)

#define	DMAC_CFG_SRC_TYPE_NFC_DEBUG			    (0x0f)	//(0x0f<<0)
#define	DMAC_CFG_SRC_TYPE_N_SRAM 				(0x10)	//(0x10<<0)
#define	DMAC_CFG_SRC_TYPE_N_SDRAM				(0x11)	//(0x11<<0)
#define	DMAC_CFG_SRC_TYPE_UART4				    (0x12)	//(0x12<<0)
#define	DMAC_CFG_SRC_TYPE_UART5				    (0x13)	//(0x13<<0)
#define	DMAC_CFG_SRC_TYPE_UART6				    (0x14)	//(0x14<<0)
#define	DMAC_CFG_SRC_TYPE_UART7				    (0x15)	//(0x15<<0)

/* DMA 传输源端 D型DMA 目的选择 */
#define	DMAC_CFG_SRC_TYPE_D_SRAM 				(0x00)	//(0x00<<0)
#define	DMAC_CFG_SRC_TYPE_D_SDRAM				(0x01)	//(0x01<<0)
#define	DMAC_CFG_SRC_TYPE_TCON0				    (0x02)	//(0x02<<0)
#define	DMAC_CFG_SRC_TYPE_NFC  		    	   	(0x03)	//(0x03<<0)
#define	DMAC_CFG_SRC_TYPE_USB0			    	(0x04)	//(0x04<<0)
#define	DMAC_CFG_SRC_TYPE_USB1			    	(0x05)	//(0x05<<0)
#define	DMAC_CFG_SRC_TYPE_SDC1			    	(0x07)	//(0x07<<0)
#define	DMAC_CFG_SRC_TYPE_SDC2 				    (0x08)	//(0x08<<0)
#define	DMAC_CFG_SRC_TYPE_SDC3 				    (0x09)	//(0x09<<0)
#define	DMAC_CFG_SRC_TYPE_MSC  				    (0x0a)	//(0x0a<<0)
#define	DMAC_CFG_SRC_TYPE_EMAC 				    (0x0c)	//(0x0c<<0)
#define	DMAC_CFG_SRC_TYPE_SS   				    (0x0e)	//(0x0e<<0)
#define	DMAC_CFG_SRC_TYPE_USB2			    	(0x0f)	//(0x0f<<0)
#define	DMAC_CFG_SRC_TYPE_ATA			    	(0x10)	//(0x10<<0)

  /* offset */
#define DMAC_REG_o_IRQ_EN           0x00
#define DMAC_REG_o_IRQ_PENDING      0x04
#define DMAC_REG_o_SYS_PRI          0x08
#define DMAC_REG_o_CFG              0x00
#define DMAC_REG_o_SRC_ADDR         0x04
#define DMAC_REG_o_DST_ADDR         0x08
#define DMAC_REG_o_BYTE_CNT         0x0C
#define DMAC_REG_o_PAGE_SIZE        0x10
#define DMAC_REG_o_PAGE_STEP        0x14
#define DMAC_REG_o_CMT_BLK          0x18
  /* registers */
#define DMAC_REG_IRQ_EN             __REG( DMAC_REGS_BASE + DMAC_REG_o_IRQ_EN                     )
#define DMAC_REG_IRQ_PENDING        __REG( DMAC_REGS_BASE + DMAC_REG_o_IRQ_PENDING                )
#define DMAC_REG_SYS_PRI            __REG( DMAC_REGS_BASE + DMAC_REG_o_SYS_PRI                    )
#define DMAC_REG_N0_CFG             __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N0_SRC_ADDR        __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N0_DST_ADDR        __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N0_BYTE_CNT        __REG( DMAC_REGS_BASE + 1 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_N1_CFG             __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N1_SRC_ADDR        __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N1_DST_ADDR        __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N1_BYTE_CNT        __REG( DMAC_REGS_BASE + 2 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_N2_CFG             __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N2_SRC_ADDR        __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N2_DST_ADDR        __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N2_BYTE_CNT        __REG( DMAC_REGS_BASE + 3 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_N3_CFG             __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N3_SRC_ADDR        __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N3_DST_ADDR        __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N3_BYTE_CNT        __REG( DMAC_REGS_BASE + 4 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )

#define DMAC_REG_D0_CFG             __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D0_SRC_ADDR        __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D0_DST_ADDR        __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D0_BYTE_CNT        __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D0_PAGE_SIZE       __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D0_PAGE_STEP       __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D0_CMT_BLK         __REG( DMAC_REGS_BASE + 5 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D1_CFG             __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D1_SRC_ADDR        __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D1_DST_ADDR        __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D1_BYTE_CNT        __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D1_PAGE_SIZE       __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D1_PAGE_STEP       __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D1_CMT_BLK         __REG( DMAC_REGS_BASE + 6 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D2_CFG             __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D2_SRC_ADDR        __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D2_DST_ADDR        __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D2_BYTE_CNT        __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D2_PAGE_SIZE       __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D2_PAGE_STEP       __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D2_CMT_BLK         __REG( DMAC_REGS_BASE + 7 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D3_CFG             __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D3_SRC_ADDR        __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D3_DST_ADDR        __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D3_BYTE_CNT        __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D3_PAGE_SIZE       __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D3_PAGE_STEP       __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D3_CMT_BLK         __REG( DMAC_REGS_BASE + 8 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )

#define DMAC_REG_D4_CFG             __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D4_SRC_ADDR        __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D4_DST_ADDR        __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D4_BYTE_CNT        __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D4_PAGE_SIZE       __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D4_PAGE_STEP       __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D4_CMT_BLK         __REG( DMAC_REGS_BASE + 9 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D5_CFG             __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D5_SRC_ADDR        __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D5_DST_ADDR        __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D5_BYTE_CNT        __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D5_PAGE_SIZE       __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D5_PAGE_STEP       __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D5_CMT_BLK         __REG( DMAC_REGS_BASE + 10 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D6_CFG             __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D6_SRC_ADDR        __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D6_DST_ADDR        __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D6_BYTE_CNT        __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D6_PAGE_SIZE       __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D6_PAGE_STEP       __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D6_CMT_BLK         __REG( DMAC_REGS_BASE + 11 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
#define DMAC_REG_D7_CFG             __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D7_SRC_ADDR        __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D7_DST_ADDR        __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D7_BYTE_CNT        __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D7_PAGE_SIZE       __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D7_PAGE_STEP       __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D7_CMT_BLK         __REG( DMAC_REGS_BASE + 12 * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )
  /* registers' another defination */
#define DMAC_REG_N_CFG(port)        __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_N_SRC_ADDR(port)   __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_N_DST_ADDR(port)   __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_N_BYTE_CNT(port)   __REG( DMAC_REGS_BASE + ( 1 + port ) * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D_CFG(port)        __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_CFG       )
#define DMAC_REG_D_SRC_ADDR(port)   __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_SRC_ADDR  )
#define DMAC_REG_D_DST_ADDR(port)   __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_DST_ADDR  )
#define DMAC_REG_D_BYTE_CNT(port)   __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_BYTE_CNT  )
#define DMAC_REG_D_PAGE_SIZE(port)  __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_PAGE_SIZE )
#define DMAC_REG_D_PAGE_STEP(port)  __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_PAGE_STEP )
#define DMAC_REG_D_CMT_BLK(port)    __REG( DMAC_REGS_BASE + ( 5 + port ) * DMA_OFFSET + DMAC_REG_o_CMT_BLK   )


#endif    // #ifndef _DMAC_H_

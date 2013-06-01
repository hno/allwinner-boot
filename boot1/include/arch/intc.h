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
#ifndef _INTC_H_
#define _INTC_H_

/*
*********************************************************************************************************
*   Interrupt controller define
*********************************************************************************************************
*/
/* GIC registers */
#define GICD_CTLR			__REG(ARMV7_GIC_BASE + 0x0000)
#define GICD_TYPE			__REG(ARMV7_GIC_BASE + 0x0004)
#define GICD_IIDR			__REG(ARMV7_GIC_BASE + 0x0008)

#define GICD_IGROUPR		__REG(ARMV7_GIC_BASE + 0x0080)

#define GICD_ISENABLER(n)	__REG(ARMV7_GIC_BASE + 0x100 + 4 * (n))
#define GICD_ICENABLER(n)	__REG(ARMV7_GIC_BASE + 0x180 + 4 * (n))

#define GICD_ISPENDR(n)		__REG(ARMV7_GIC_BASE + 0x200 + 4 * (n))
#define GICD_ICPENDR(n)		__REG(ARMV7_GIC_BASE + 0x280 + 4 * (n))

#define GICD_ISACTIVER(n)	__REG(ARMV7_GIC_BASE + 0x300 + 4 * (n))
#define GICD_ICACTIVER(n)	__REG(ARMV7_GIC_BASE + 0x380 + 4 * (n))

#define GICD_SGI_PRIO(n)	__REG(ARMV7_GIC_BASE + 0x400 + 4 * (n))
#define GICD_PPI_PRIO(n)	__REG(ARMV7_GIC_BASE + 0x410 + 4 * (n))
#define GICD_SPI_PRIO(n)	__REG(ARMV7_GIC_BASE + 0x420 + 4 * (n))

#define GICD_SGI_ITARG(n)	__REG(ARMV7_GIC_BASE + 0x800 + 4 * (n))
#define GICD_PPI_ITARG(n)	__REG(ARMV7_GIC_BASE + 0x810 + 4 * (n))
#define GICD_SPI_ITARG(n)	__REG(ARMV7_GIC_BASE + 0x820 + 4 * (n))

#define GICD_ICFGR(n)		__REG(ARMV7_GIC_BASE + 0xc00 + 4 * (n))

#define GICD_SGIR			__REG(ARMV7_GIC_BASE + 0xf00)	// 0xf00

#define GICC_CTLR		    __REG(ARMV7_CPUIF_BASE + 0x000) // 0x8000
#define GICC_PMR			__REG(ARMV7_CPUIF_BASE + 0x004) // 0x8004
#define GICC_BPR			__REG(ARMV7_CPUIF_BASE + 0x008) // 0x8008
#define GICC_IAR			__REG(ARMV7_CPUIF_BASE + 0x00c) // 0x800c
#define GICC_EOIR			__REG(ARMV7_CPUIF_BASE + 0x010) // 0x8010
#define GICC_RPR			__REG(ARMV7_CPUIF_BASE + 0x014) // 0x8014
#define GICC_HPPIR			__REG(ARMV7_CPUIF_BASE + 0x018) // 0x8018
#define GICC_DIR			__REG(ARMV7_CPUIF_BASE + 0x1000)// 0x1000

/* gic source list */
/* software generated interrupt */
#define GIC_SRC_SGI(_n)		(_n)
#define GIC_SRC_SGI0		GIC_SRC_SGI(0 )  // (0 )
#define GIC_SRC_SGI1		GIC_SRC_SGI(1 )  // (1 )
#define GIC_SRC_SGI2		GIC_SRC_SGI(2 )  // (2 )
#define GIC_SRC_SGI3		GIC_SRC_SGI(3 )  // (3 )
#define GIC_SRC_SGI4		GIC_SRC_SGI(4 )  // (4 )
#define GIC_SRC_SGI5		GIC_SRC_SGI(5 )  // (5 )
#define GIC_SRC_SGI6		GIC_SRC_SGI(6 )  // (6 )
#define GIC_SRC_SGI7		GIC_SRC_SGI(7 )  // (7 )
#define GIC_SRC_SGI8		GIC_SRC_SGI(8 )  // (8 )
#define GIC_SRC_SGI9		GIC_SRC_SGI(9 )  // (9 )
#define GIC_SRC_SGI10		GIC_SRC_SGI(10)  // (10)
#define GIC_SRC_SGI11		GIC_SRC_SGI(11)  // (11)
#define GIC_SRC_SGI12		GIC_SRC_SGI(12)  // (12)
#define GIC_SRC_SGI13		GIC_SRC_SGI(13)  // (13)
#define GIC_SRC_SGI14		GIC_SRC_SGI(14)  // (14)
#define GIC_SRC_SGI15		GIC_SRC_SGI(15)  // (15)
/* private peripheral interrupt */
#define GIC_SRC_PPI(_n)		(16 + (_n))
#define GIC_SRC_PPI0		GIC_SRC_PPI(0 )  // (16)
#define GIC_SRC_PPI1		GIC_SRC_PPI(1 )  // (17)
#define GIC_SRC_PPI2		GIC_SRC_PPI(2 )  // (18)
#define GIC_SRC_PPI3		GIC_SRC_PPI(3 )  // (19)
#define GIC_SRC_PPI4		GIC_SRC_PPI(4 )  // (20)
#define GIC_SRC_PPI5		GIC_SRC_PPI(5 )  // (21)
#define GIC_SRC_PPI6		GIC_SRC_PPI(6 )  // (22)
#define GIC_SRC_PPI7		GIC_SRC_PPI(7 )  // (23)
#define GIC_SRC_PPI8		GIC_SRC_PPI(8 )  // (24)
#define GIC_SRC_PPI9		GIC_SRC_PPI(9 )  // (25)
#define GIC_SRC_PPI10		GIC_SRC_PPI(10)  // (26)
#define GIC_SRC_PPI11		GIC_SRC_PPI(11)  // (27)
#define GIC_SRC_PPI12		GIC_SRC_PPI(12)  // (28)
#define GIC_SRC_PPI13		GIC_SRC_PPI(13)  // (29)
#define GIC_SRC_PPI14		GIC_SRC_PPI(14)  // (30)
#define GIC_SRC_PPI15		GIC_SRC_PPI(15)  // (31)
/* external peripheral interrupt */

#define GIC_SRC_SPI(_n)		(32 + (_n))

#ifndef CONFIG_AW_FPGA_PLATFORM	//chip irq mapping

#define GIC_SRC_NMI			GIC_SRC_SPI(0)   // (32)
#define GIC_SRC_UART0		GIC_SRC_SPI(1)   // (33)
#define GIC_SRC_UART1		GIC_SRC_SPI(2)   // (34)
#define GIC_SRC_UART2		GIC_SRC_SPI(3)   // (35)
#define GIC_SRC_UART3		GIC_SRC_SPI(4)   // (36)
#define GIC_SRC_CIR0		GIC_SRC_SPI(5)   // (37)
#define GIC_SRC_CIR1		GIC_SRC_SPI(6  ) // (38)
#define GIC_SRC_TWI0		GIC_SRC_SPI(7  ) // (39)
#define GIC_SRC_TWI1		GIC_SRC_SPI(8  ) // (40)
#define GIC_SRC_TWI2		GIC_SRC_SPI(9  ) // (41)
#define GIC_SRC_SPI0		GIC_SRC_SPI(10 ) // (42)
#define GIC_SRC_SPI1		GIC_SRC_SPI(11 ) // (43)
#define GIC_SRC_SPI2		GIC_SRC_SPI(12 ) // (44)
#define GIC_SRC_SPDIF		GIC_SRC_SPI(13 ) // (45)
#define GIC_SRC_AC97		GIC_SRC_SPI(14 ) // (46)
#define GIC_SRC_TS			GIC_SRC_SPI(15 ) // (47)
#define GIC_SRC_IIS			GIC_SRC_SPI(16 ) // (48)
#define GIC_SRC_UART4		GIC_SRC_SPI(17 ) // (49)
#define GIC_SRC_UART5		GIC_SRC_SPI(18 ) // (50)
#define GIC_SRC_UART6		GIC_SRC_SPI(19 ) // (51)
#define GIC_SRC_UART7		GIC_SRC_SPI(20 ) // (52)
#define GIC_SRC_KP			GIC_SRC_SPI(21 ) // (53)
#define GIC_SRC_TIMER0		GIC_SRC_SPI(22 ) // (54)
#define GIC_SRC_TIMER1		GIC_SRC_SPI(23 ) // (55)
#define GIC_SRC_TIMER2		GIC_SRC_SPI(24 ) // (56)
#define GIC_SRC_TIMER3		GIC_SRC_SPI(25 ) // (57)
#define GIC_SRC_CAN			GIC_SRC_SPI(26 ) // (58)
#define GIC_SRC_DMA			GIC_SRC_SPI(27 ) // (59)
#define GIC_SRC_PIO_EINT	GIC_SRC_SPI(28 ) // (60)
#define GIC_SRC_TP			GIC_SRC_SPI(29 ) // (61)
#define GIC_SRC_CODEC		GIC_SRC_SPI(30 ) // (62)
#define GIC_SRC_LRADC		GIC_SRC_SPI(31 ) // (63)
#define GIC_SRC_MMC0		GIC_SRC_SPI(32 ) // (64)
#define GIC_SRC_MMC1		GIC_SRC_SPI(33 ) // (65)
#define GIC_SRC_MMC2		GIC_SRC_SPI(34 ) // (66)
#define GIC_SRC_MMC3		GIC_SRC_SPI(35 ) // (67)
#define GIC_SRC_MS			GIC_SRC_SPI(36 ) // (68)
#define GIC_SRC_NAND		GIC_SRC_SPI(37 ) // (69)
#define GIC_SRC_USB0		GIC_SRC_SPI(38 ) // (70)
#define GIC_SRC_USB1		GIC_SRC_SPI(39 ) // (71)
#define GIC_SRC_USB2		GIC_SRC_SPI(40 ) // (72)
#define GIC_SRC_SCR			GIC_SRC_SPI(41 ) // (73)
#define GIC_SRC_CSI0		GIC_SRC_SPI(42 ) // (74)
#define GIC_SRC_CSI1		GIC_SRC_SPI(43 ) // (75)
#define GIC_SRC_LCD0		GIC_SRC_SPI(44 ) // (76)
#define GIC_SRC_LCD1		GIC_SRC_SPI(45 ) // (77)
#define GIC_SRC_MP			GIC_SRC_SPI(46 ) // (78)
#define GIC_SRC_DEBE0_DEFE0	GIC_SRC_SPI(47 ) // (79)
#define GIC_SRC_DEBE1_DEFE1	GIC_SRC_SPI(48 ) // (80)
#define GIC_SRC_PMU			GIC_SRC_SPI(49 ) // (81)
#define GIC_SRC_SPI3		GIC_SRC_SPI(50 ) // (82)
//#define GIC_SRC_			GIC_SRC_SPI(51 ) // (83)
#define GIC_SRC_PATA		GIC_SRC_SPI(52 ) // (84)
#define GIC_SRC_VE			GIC_SRC_SPI(53 ) // (85)
#define GIC_SRC_SS			GIC_SRC_SPI(54 ) // (86)
#define GIC_SRC_EMAC		GIC_SRC_SPI(55 ) // (87)
#define GIC_SRC_SATA		GIC_SRC_SPI(56 ) // (88)
#define GIC_SRC_GPS			GIC_SRC_SPI(57 ) // (89)
#define GIC_SRC_HDMI		GIC_SRC_SPI(58 ) // (90)
#define GIC_SRC_TVE01		GIC_SRC_SPI(59 ) // (91)
#define GIC_SRC_ACE			GIC_SRC_SPI(60 ) // (92)
#define GIC_SRC_TVD			GIC_SRC_SPI(61 ) // (93)
#define GIC_SRC_PS2_0		GIC_SRC_SPI(62 ) // (94)
#define GIC_SRC_PS2_1		GIC_SRC_SPI(63 ) // (95)
#define GIC_SRC_USB3		GIC_SRC_SPI(64 ) // (96)
#define GIC_SRC_USB4		GIC_SRC_SPI(65 ) // (97 )
#define GIC_SRC_PLE_PERFMU	GIC_SRC_SPI(66 ) // (98 )
#define GIC_SRC_TIMER4		GIC_SRC_SPI(67 ) // (99 )
#define GIC_SRC_TIMER5		GIC_SRC_SPI(68 ) // (100)
#define GIC_SRC_GPU_GP		GIC_SRC_SPI(69 ) // (101)
#define GIC_SRC_GPU_GPMMU	GIC_SRC_SPI(70 ) // (102)
#define GIC_SRC_GPU_PP0		GIC_SRC_SPI(71 ) // (103)
#define GIC_SRC_GPU_PPMMU0	GIC_SRC_SPI(72 ) // (104)
#define GIC_SRC_GPU_PMU		GIC_SRC_SPI(73 ) // (105)
#define GIC_SRC_GPU_PP1		GIC_SRC_SPI(74 ) // (106)
#define GIC_SRC_GPU_PPMMU1	GIC_SRC_SPI(75 ) // (107)
#define GIC_SRC_GPU_RSV0	GIC_SRC_SPI(76 ) // (108)
#define GIC_SRC_GPU_RSV1	GIC_SRC_SPI(77 ) // (109)
#define GIC_SRC_GPU_RSV2	GIC_SRC_SPI(78 ) // (110)
#define GIC_SRC_GPU_RSV3	GIC_SRC_SPI(79 ) // (111)
#define GIC_SRC_GPU_RSV4	GIC_SRC_SPI(80 ) // (112)
#define GIC_SRC_HSTMR0		GIC_SRC_SPI(81 ) // (113)
#define GIC_SRC_HSTMR1		GIC_SRC_SPI(82 ) // (114)
#define GIC_SRC_HSTMR2		GIC_SRC_SPI(83 ) // (115)
#define GIC_SRC_HSTMR3		GIC_SRC_SPI(84 ) // (116)
#define GIC_SRC_GMAC		GIC_SRC_SPI(85 ) // (117)
//#define GIC_SRC_	GIC_SRC_SPI(86 ) // (118)
//#define GIC_SRC_	GIC_SRC_SPI(87 ) // (119)
//#define GIC_SRC_	GIC_SRC_SPI(88 ) // (120)
//#define GIC_SRC_	GIC_SRC_SPI(89 ) // (121)
//#define GIC_SRC_	GIC_SRC_SPI(90 ) // (122)
//#define GIC_SRC_	GIC_SRC_SPI(91 ) // (123)
//#define GIC_SRC_	GIC_SRC_SPI(92 ) // (124)
//#define GIC_SRC_	GIC_SRC_SPI(93 ) // (125)
//#define GIC_SRC_	GIC_SRC_SPI(94 ) // (126)
//#define GIC_SRC_	GIC_SRC_SPI(95 ) // (127)
//#define GIC_SRC_	GIC_SRC_SPI(96 ) // (128)
//#define GIC_SRC_	GIC_SRC_SPI(97 ) // (129)
//
//#define GIC_SRC_	GIC_SRC_SPI(108) // (140)
//#define GIC_SRC_	GIC_SRC_SPI(109) // (141)
//#define GIC_SRC_	GIC_SRC_SPI(110) // (142)
//#define GIC_SRC_	GIC_SRC_SPI(111) // (143)
//#define GIC_SRC_	GIC_SRC_SPI(112) // (144)
//#define GIC_SRC_	GIC_SRC_SPI(113) // (145)
//#define GIC_SRC_	GIC_SRC_SPI(114) // (146)
//#define GIC_SRC_	GIC_SRC_SPI(115) // (147)
//#define GIC_SRC_	GIC_SRC_SPI(116) // (148)
//#define GIC_SRC_	GIC_SRC_SPI(117) // (149)
//#define GIC_SRC_	GIC_SRC_SPI(118) // (150)
//#define GIC_SRC_	GIC_SRC_SPI(119) // (151)
//#define GIC_SRC_	GIC_SRC_SPI(120) // (152)
//#define GIC_SRC_	GIC_SRC_SPI(121) // (153)
//#define GIC_SRC_	GIC_SRC_SPI(122) // (154)
//#define GIC_SRC_	GIC_SRC_SPI(123) // (155)
//#define GIC_SRC_	GIC_SRC_SPI(124) // (156)

#define GIC_IRQ_NUM		(GIC_SRC_HSTMR3 + 1)

#else	//fpga irq mapping
#define GIC_SRC_NMI			GIC_SRC_SPI(0) 	// (32)
#define GIC_SRC_UART0		GIC_SRC_SPI(1)	// (33)
#define GIC_SRC_UART1		GIC_SRC_SPI(2)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_UART2		GIC_SRC_SPI(3)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_UART3		GIC_SRC_SPI(4)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_UART4		GIC_SRC_SPI(1)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_UART5		GIC_SRC_SPI(1)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_UART6		GIC_SRC_SPI(1)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_UART7		GIC_SRC_SPI(1)	// (33)	not exist in fpga, just for compiling
#define GIC_SRC_TWI0		GIC_SRC_SPI(2)	// (34)
#define GIC_SRC_TWI1		GIC_SRC_SPI(8)	// (34)	not exist in fpga, just for compiling
#define GIC_SRC_TWI2		GIC_SRC_SPI(9)	// (34)	not exist in fpga, just for compiling
#define GIC_SRC_PIO_EINT	GIC_SRC_SPI(2)	// (34)
#define GIC_SRC_CIR0		GIC_SRC_SPI(2)	// (34)
#define GIC_SRC_CIR1		GIC_SRC_SPI(2)	// (34)	not exist in fpga, just for compiling
#define GIC_SRC_KP			GIC_SRC_SPI(2)	// (34)
#define GIC_SRC_CAN			GIC_SRC_SPI(2)	// (34)
#define GIC_SRC_PS2_0		GIC_SRC_SPI(2)	// (34)
#define GIC_SRC_PS2_1		GIC_SRC_SPI(2)	// (34)	not exist in fpga, just for compiling
#define GIC_SRC_SPDIF		GIC_SRC_SPI(3)  // (35)
#define GIC_SRC_IIS			GIC_SRC_SPI(3)  // (35)
#define GIC_SRC_AC97		GIC_SRC_SPI(3)  // (35)
#define GIC_SRC_TS			GIC_SRC_SPI(3)  // (35)
#define GIC_SRC_PLE_PERFMU	GIC_SRC_SPI(3)  // (35)
#define GIC_SRC_TIMER0		GIC_SRC_SPI(4)  // (36)
#define GIC_SRC_TIMER1		GIC_SRC_SPI(5)  // (37)
#define GIC_SRC_HSTMR0		GIC_SRC_SPI(5)  // (37)
#define GIC_SRC_HSTMR1		GIC_SRC_SPI(5)  // (37)	not exist in fpga, just for compiling
#define GIC_SRC_HSTMR2		GIC_SRC_SPI(5)  // (37)	not exist in fpga, just for compiling
#define GIC_SRC_HSTMR3		GIC_SRC_SPI(5)  // (37)	not exist in fpga, just for compiling
#define GIC_SRC_TIMER2		GIC_SRC_SPI(6)  // (38)
#define GIC_SRC_TIMER3		GIC_SRC_SPI(25) // (38)	not exist in fpga, just for compiling
#define GIC_SRC_TIMER4		GIC_SRC_SPI(6)  // (38)	not exist in fpga, just for compiling
#define GIC_SRC_TIMER5		GIC_SRC_SPI(6)  // (38)	not exist in fpga, just for compiling
#define GIC_SRC_DMA			GIC_SRC_SPI(7)  // (38)
#define GIC_SRC_TP			GIC_SRC_SPI(8)  // (40)
#define GIC_SRC_CODEC		GIC_SRC_SPI(8)  // (40)
#define GIC_SRC_LRADC		GIC_SRC_SPI(8)  // (40)
#define GIC_SRC_MMC0		GIC_SRC_SPI(9)  // (41)
#define GIC_SRC_MMC1		GIC_SRC_SPI(33)  // (41) not exist in fpga, just for compiling
#define GIC_SRC_MMC2		GIC_SRC_SPI(10) // (42)
#define GIC_SRC_MMC3		GIC_SRC_SPI(35) // (42) not exist in fpga, just for compiling
#define GIC_SRC_MS			GIC_SRC_SPI(10) // (42)
#define GIC_SRC_NAND		GIC_SRC_SPI(11) // (43)
#define GIC_SRC_USB0		GIC_SRC_SPI(12) // (44)
#define GIC_SRC_USB1		GIC_SRC_SPI(13) // (45)
#define GIC_SRC_USB2		GIC_SRC_SPI(14) // (46)
#define GIC_SRC_CSI1		GIC_SRC_SPI(14) // (46)
#define GIC_SRC_TVD			GIC_SRC_SPI(14) // (46)
#define GIC_SRC_SCR			GIC_SRC_SPI(15) // (47)
#define GIC_SRC_SPI0		GIC_SRC_SPI(15) // (47)
#define GIC_SRC_SPI1		GIC_SRC_SPI(15) // (47) not exist in fpga, just for compiling
#define GIC_SRC_SPI2		GIC_SRC_SPI(15) // (47) not exist in fpga, just for compiling
#define GIC_SRC_SPI3		GIC_SRC_SPI(15) // (47) not exist in fpga, just for compiling
#define GIC_SRC_TVE01		GIC_SRC_SPI(15) // (47)
#define GIC_SRC_EMAC		GIC_SRC_SPI(15) // (47)
#define GIC_SRC_GMAC		GIC_SRC_SPI(15) // (47)
#define GIC_SRC_USB3		GIC_SRC_SPI(15) // (47)
#define GIC_SRC_CSI0		GIC_SRC_SPI(16) // (48)
#define GIC_SRC_USB4		GIC_SRC_SPI(16) // (48)
#define GIC_SRC_LCD0		GIC_SRC_SPI(17) // (49)
#define GIC_SRC_LCD1		GIC_SRC_SPI(18) // (50)
#define GIC_SRC_MP			GIC_SRC_SPI(19) // (51)
#define GIC_SRC_DEBE0_DEFE0	GIC_SRC_SPI(20) // (52)
#define GIC_SRC_DEBE1_DEFE1	GIC_SRC_SPI(21) // (53)
#define GIC_SRC_PMU			GIC_SRC_SPI(22) // (54)
#define GIC_SRC_SATA		GIC_SRC_SPI(22) // (54)
#define GIC_SRC_PATA		GIC_SRC_SPI(22) // (54)
#define GIC_SRC_HDMI		GIC_SRC_SPI(23) // (55)
#define GIC_SRC_VE			GIC_SRC_SPI(24) // (56)
#define GIC_SRC_SS			GIC_SRC_SPI(24) // (56)
#define GIC_SRC_GPS			GIC_SRC_SPI(24) // (56)
#define GIC_SRC_ACE			GIC_SRC_SPI(24) // (56)
#define GIC_SRC_GPU_GP		GIC_SRC_SPI(25) // (57)
#define GIC_SRC_GPU_GPMMU	GIC_SRC_SPI(26) // (58)
#define GIC_SRC_GPU_PP0		GIC_SRC_SPI(27) // (59)
#define GIC_SRC_GPU_PPMMU0	GIC_SRC_SPI(28) // (60)
#define GIC_SRC_GPU_PMU		GIC_SRC_SPI(29) // (61)
#define GIC_SRC_GPU_PP1		GIC_SRC_SPI(30) // (62)
#define GIC_SRC_GPU_PPMMU1	GIC_SRC_SPI(31) // (63)
#define GIC_IRQ_NUM			(GIC_SRC_GPU_PPMMU1 + 1)

#endif	//fpga irq mapping


#endif




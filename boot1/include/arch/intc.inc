;//****************************************************************************************************************
;//*                                               Aone CPU define
;//*                                       Aone CPU regiesters, memory, interrupts, ... define
;//*
;//*                                         (c) Copyright 2007-2008, esOS
;//*                                               All Rights Reserved
;//*
;//* File    : Aone.h
;//* By      : Steven.ZGJ
;//* Version : V0.1
;//* Histroy : 2007.10.12  modified by Wang Yugang
;//****************************************************************************************************************
	IF :LNOT::DEF:_INTC_INC_
	GBLA  _INTC_INC_

;/*
;*********************************************************************************************************
;*   Interrupt controller define
;*********************************************************************************************************
;*/
;/* Offset */
INTC_REG_o_VCTR               EQU    0x00
INTC_REG_o_VTBLBADDR          EQU    0x04

INTC_REG_o_NMI_CTRL           EQU    0x0C

INTC_REG_o_IRQ_PENDCLR0       EQU    0x10
INTC_REG_o_IRQ_PENDCLR1       EQU    0x14
INTC_REG_o_IRQ_PENDCLR2       EQU    0x18

INTC_REG_o_FIQ_PENDCLR0       EQU    0x20
INTC_REG_o_FIQ_PENDCLR1       EQU    0x24
INTC_REG_o_FIQ_PENDCLR2       EQU    0x28

INTC_REG_o_IRQ_SEL0           EQU    0x30
INTC_REG_o_IRQ_SEL1           EQU    0x34
INTC_REG_o_IRQ_SEL2           EQU    0x38

INTC_REG_o_ENABLE0            EQU    0x40
INTC_REG_o_ENABLE1            EQU    0x44
INTC_REG_o_ENABLE2            EQU    0x48

INTC_REG_o_MASK0              EQU    0x50
INTC_REG_o_MASK1              EQU    0x54
INTC_REG_o_MASK2              EQU    0x58

INTC_REG_o_RESP0              EQU    0x60
INTC_REG_o_RSEP1              EQU    0x64
INTC_REG_o_RESP2              EQU    0x68

INTC_REG_o_FF0                EQU    0x70
INTC_REG_o_FF1                EQU    0x74
INTC_REG_o_FF2                EQU    0x78

INTC_REG_o_PRIO0              EQU    0x80
INTC_REG_o_PRIO1              EQU    0x84
INTC_REG_o_PRIO2              EQU    0x88
INTC_REG_o_PRIO3              EQU    0x8C
INTC_REG_o_PRIO4              EQU    0x90

;//address
INTC_REG_VCTR                 EQU    ( INTC_REGS_BASE +  INTC_REG_o_VCTR         )
INTC_REG_VTBLBADDR            EQU    ( INTC_REGS_BASE +  INTC_REG_o_VTBLBADDR    )

INTC_REG_NMI_CTRL             EQU    ( INTC_REGS_BASE +  INTC_REG_o_NMI_CTRL     )

INTC_REG_IRQ_PENDCLR0         EQU    ( INTC_REGS_BASE +  INTC_REG_o_IRQ_PENDCLR0 )
INTC_REG_IRQ_PENDCLR1         EQU    ( INTC_REGS_BASE +  INTC_REG_o_IRQ_PENDCLR1 )
INTC_REG_IRQ_PENDCLR2         EQU    ( INTC_REGS_BASE +  INTC_REG_o_IRQ_PENDCLR2 )

INTC_REG_FIQ_PENDCLR0         EQU    ( INTC_REGS_BASE +  INTC_REG_o_FIQ_PENDCLR0 )
INTC_REG_FIQ_PENDCLR1         EQU    ( INTC_REGS_BASE +  INTC_REG_o_FIQ_PENDCLR1 )
INTC_REG_FIQ_PENDCLR2         EQU    ( INTC_REGS_BASE +  INTC_REG_o_FIQ_PENDCLR2 )

INTC_REG_IRQ_SEL0             EQU    ( INTC_REGS_BASE +  INTC_REG_o_IRQ_SEL0     )
INTC_REG_IRQ_SEL1             EQU    ( INTC_REGS_BASE +  INTC_REG_o_IRQ_SEL1     )
INTC_REG_IRQ_SEL2             EQU    ( INTC_REGS_BASE +  INTC_REG_o_IRQ_SEL2     )

INTC_REG_ENABLE0              EQU    ( INTC_REGS_BASE +  INTC_REG_o_ENABLE0      )
INTC_REG_ENABLE1              EQU    ( INTC_REGS_BASE +  INTC_REG_o_ENABLE1      )
INTC_REG_ENABLE2              EQU    ( INTC_REGS_BASE +  INTC_REG_o_ENABLE2      )

INTC_REG_MASK0                EQU    ( INTC_REGS_BASE +  INTC_REG_o_MASK0        )
INTC_REG_MASK1                EQU    ( INTC_REGS_BASE +  INTC_REG_o_MASK1        )
INTC_REG_MASK2                EQU    ( INTC_REGS_BASE +  INTC_REG_o_MASK2        )

INTC_REG_RESP0                EQU    ( INTC_REGS_BASE +  INTC_REG_o_RESP0        )
INTC_REG_RSEP1                EQU    ( INTC_REGS_BASE +  INTC_REG_o_RSEP1        )
INTC_REG_RESP2                EQU    ( INTC_REGS_BASE +  INTC_REG_o_RESP2        )

INTC_REG_FF0                  EQU    ( INTC_REGS_BASE +  INTC_REG_o_FF0          )
INTC_REG_FF1                  EQU    ( INTC_REGS_BASE +  INTC_REG_o_FF1          )
INTC_REG_FF2                  EQU    ( INTC_REGS_BASE +  INTC_REG_o_FF2          )

INTC_REG_PRIO0                EQU    ( INTC_REGS_BASE +  INTC_REG_o_PRIO0        )
INTC_REG_PRIO1                EQU    ( INTC_REGS_BASE +  INTC_REG_o_PRIO1        )
INTC_REG_PRIO2                EQU    ( INTC_REGS_BASE +  INTC_REG_o_PRIO2        )
INTC_REG_PRIO3                EQU    ( INTC_REGS_BASE +  INTC_REG_o_PRIO3        )
INTC_REG_PRIO4                EQU    ( INTC_REGS_BASE +  INTC_REG_o_PRIO4        )



;/* mask */
INTC_IRQNO_FIQ                EQU      0
INTC_IRQNO_UART0              EQU      1
INTC_IRQNO_UART1              EQU      2
INTC_IRQNO_UART2              EQU      3
INTC_IRQNO_UART3              EQU      4
INTC_IRQNO_IR0                EQU      5
INTC_IRQNO_IR1                EQU      6
INTC_IRQNO_TWI0               EQU      7
INTC_IRQNO_TWI1               EQU      8
INTC_IRQNO_TWI2               EQU      9

INTC_IRQNO_SPI0               EQU      10
INTC_IRQNO_SPI1               EQU      11
INTC_IRQNO_SPI2               EQU      12
INTC_IRQNO_SPDIF              EQU      13
INTC_IRQNO_AC97               EQU      14
INTC_IRQNO_TS                 EQU      15
INTC_IRQNO_IIS                EQU      16

INTC_IRQNO_UART4              EQU      17
INTC_IRQNO_UART5              EQU      18
INTC_IRQNO_UART6              EQU      19
INTC_IRQNO_UART7              EQU      20

INTC_IRQNO_KEYPAD             EQU      21
INTC_IRQNO_TIMER0             EQU      22
INTC_IRQNO_TIMER1             EQU      23
INTC_IRQNO_TIMER245           EQU      24
INTC_IRQNO_TIMER3             EQU      25
INTC_IRQNO_CAN                EQU      26
INTC_IRQNO_DMA                EQU      27
INTC_IRQNO_PIO                EQU      28
INTC_IRQNO_TP                 EQU      29

INTC_IRQNO_ADDA               EQU      30
INTC_IRQNO_LRADC              EQU      31
INTC_IRQNO_SDMMC0             EQU      32
INTC_IRQNO_SDMMC1             EQU      33
INTC_IRQNO_SDMMC2             EQU      34
INTC_IRQNO_SDMMC3             EQU      35
INTC_IRQNO_MS                 EQU      36
INTC_IRQNO_NAND               EQU      37
INTC_IRQNO_USB0               EQU      38
INTC_IRQNO_USB1               EQU      39
INTC_IRQNO_USB2               EQU      40

INTC_IRQNO_SCR                EQU      41

INTC_IRQNO_CSI0               EQU      42
INTC_IRQNO_CSI1               EQU      43
INTC_IRQNO_LCD0               EQU      44
INTC_IRQNO_LCD1               EQU      45
INTC_IRQNO_MP                 EQU      46
INTC_IRQNO_DE_FE0             EQU      47
INTC_IRQNO_DE_FE1             EQU      48
INTC_IRQNO_PMU                EQU      49

INTC_IRQNO_SPI3               EQU      50
INTC_IRQNO_TZASC              EQU      51
INTC_IRQNO_PATA               EQU      52
INTC_IRQNO_VE                 EQU      53

INTC_IRQNO_SS                 EQU      54
INTC_IRQNO_EMAC               EQU      55
INTC_IRQNO_SATA               EQU      56
INTC_IRQNO_GPS                EQU      57
INTC_IRQNO_HDMI               EQU      58
INTC_IRQNO_TVE01              EQU      59

INTC_IRQNO_ACE                EQU      60
INTC_IRQNO_TVD                EQU      61
INTC_IRQNO_PS0                EQU      62
INTC_IRQNO_PS1                EQU      63
INTC_IRQNO_USB3               EQU      64
INTC_IRQNO_USB4               EQU      65
INTC_IRQNO_PLE                EQU      66
INTC_IRQNO_TIMER4             EQU      67
INTC_IRQNO_TIMER5             EQU      68
INTC_IRQNO_GPU_GP             EQU      69

INTC_IRQNO_GPU_MP             EQU      70
INTC_IRQNO_GPU_PP0            EQU      71
INTC_IRQNO_GPU_PPMMU0         EQU      72
INTC_IRQNO_GPU_PMU            EQU      73
INTC_IRQNO_GPU_RSV0           EQU      74
INTC_IRQNO_GPU_RSV1           EQU      75
INTC_IRQNO_GPU_RSV2           EQU      76
INTC_IRQNO_GPU_RSV3           EQU      77
INTC_IRQNO_GPU_RSV4           EQU      78
INTC_IRQNO_GPU_RSV5           EQU      79
INTC_IRQNO_GPU_RSV6           EQU      80



	ENDIF     ;;; IF :LNOT::DEF:__INTC_INC
	END       ;;; end of intc.inc


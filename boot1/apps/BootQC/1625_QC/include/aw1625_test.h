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

#ifndef AW1603_TEST_H
#define AW1603_TEST_H

#define AC320         1

extern int pio_main (void);
extern void pioirq_main (void);
extern int dma_main (void);
extern void intc_main (void);
extern void ccu_main (void);
extern void idu_main (void);
extern void key_main (void);
extern int ac320_idu (void);
extern int NMI_main (void);
extern void Usb_Wakeup_main (void);
extern void AC320_INST_main (void);
extern void ReadNor_main (void);
extern void norboot_dma_main(void);
extern void DmaPrior_main(void);
extern void stopiduck_main(void);
extern void sram_main(void);
extern void pwm_main(void);
extern int nor_main(void);
extern int timer_main(void);
extern void wake_main(void);
extern void io_main(void);
extern void loop_main(void);
extern int Bist(void);
extern int twi0_qc_test(void);
extern int twi1_qc_test(void);
extern int twi2_qc_test(void);


//#define		SRAM_TEST	1
//#define		stopiduck_TEST	1
//#define		DmaPrior_TEST	1
//#define		norboot_dma_TEST	1
//#define		ReadNor_TEST	1
//#define	Usb_Wakeup_TEST	1
//#define AC320_INST_TEST	1
//#define NMI_TEST	1
#define AC320_IDU_TEST	1

//#define AC320_IDU_TEST	1
//#define PIO_TEST	    1
//#define DMA_TEST	    1
//#define INTC_TEST     1
//#define CLOCK_TEST    1
//#define KEY_TEST      1
//#define COMM_TEST     1

#endif


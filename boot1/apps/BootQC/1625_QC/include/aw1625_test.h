
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


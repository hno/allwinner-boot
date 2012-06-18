/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON driver sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : timre.h
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
#ifndef _SW_TIMER_H_
#define _SW_TIMER_H_

#define SW_TIMER_MAX_TICK       16777

#define CFG_SW_TIMER_MAX       2
#define CFG_SW_TIMER_CTL0	   (TMRC_REGS_BASE + 0x0010)	      /* timer0 */
#define CFG_SW_TIMER_CTL1	   (TMRC_REGS_BASE + 0x0020)          /* timer1 */

#define CFG_SW_TIMER_INT_CTRL   (*(volatile unsigned int *)(TMRC_REGS_BASE + 0x0000))         /* timer int status */
#define CFG_SW_TIMER_INT_STATS  (*(volatile unsigned int *)(TMRC_REGS_BASE + 0x0004))         /* timer int status */
#define CFG_SW_WATCHDOG_CTRL    (*(volatile unsigned int *)(TMRC_REGS_BASE + 0x0030))         /* watchdog control */
#define CFG_SW_WATCHDOG_INTERVAL   (*(volatile unsigned int *)(TMRC_REGS_BASE + 0x0034))      /* watchdog interval */

struct sw_timer
{
    volatile unsigned int control;     /* 控制器参数      */
    volatile unsigned int init_val;    /* 初始定时时间    */
};

typedef volatile struct sw_timer *sw_timer_t;

typedef int (* usr_func)(void *p_arg);

typedef struct  sw_timer_mag
{
    unsigned int     used;
    unsigned int     restart;
    unsigned int     index;
    void             *arg;
    sw_timer_t       timer_ctl;
}
sw_timer_mag_t;

#endif /* _SW_TIMER_H_ */

/*end of sw_timer.h*/

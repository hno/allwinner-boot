/*
 * (C) Copyright 2012
 *     wangflord@allwinnertech.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 */
#include <string.h>
#include "types.h"
#include "arch.h"
#include "mctl_hal.h"
#include "sys_config_para_xi.h"



typedef struct __fes_aide_info{
    __u32 dram_init_flag;       /* Dram��ʼ����ɱ�־       */
    __u32 dram_update_flag;     /* Dram �����Ƿ��޸ı�־  */
    __u8 reserve[8];
}fes_aide_info_t;

#define    SRAM_A_BASS    0x00
#define    SRAM_B_BASS    0x23000
//sram��felδʹ�õ�����, 0x7010, 0x7cf0
#define    SRAM_A_FEL_RESERVATION_START  (SRAM_A_BASS + 0x7010)
#define    SRAM_A_FEL_RESERVATION_END    (SRAM_A_BASS + 0x7d00)

//fel�׶Σ�fes_aide�Ĳ����Ĵ�ŵ�ַ, 0x7010
#define	   SYS_PARA_DOWNLOAD_ADDR		SRAM_A_FEL_RESERVATION_START
#define	   SYS_PARA_DOWNLOAD_ADDR_LEN	0x200     //512

//fel�׶Σ�fes_aide�Ĳ���������ɺ�ı�־��ַ, 0x7210
#define    SYS_PARA_LOG_ADDR    		(SYS_PARA_DOWNLOAD_ADDR + SYS_PARA_DOWNLOAD_ADDR_LEN)
#define    SYS_PARA_LOG_LEN    	    	0x10         //16, 0x7210
#define	   SYS_PARA_LOG         		(0x4d415244)   //dram_log
/*
************************************************************************************
*                          note_dram_log
*
* Description:
*	    ����ɱ�־
* Parameters:
*		void
* Return value:
*    	0: success
*      !0: fail
* History:
*       void
************************************************************************************
*/
static void  note_dram_log(void)
{
    fes_aide_info_t *fes_aide = (fes_aide_info_t *)SYS_PARA_LOG_ADDR;

    memset(fes_aide, 0, sizeof(fes_aide_info_t));
    fes_aide->dram_init_flag    = SYS_PARA_LOG;
    fes_aide->dram_update_flag  = 1;
}

static void timer_init(void)
{
	*(volatile unsigned int *)(0x01c20000 + 0x144) |= (1U << 31);
	*(volatile unsigned int *)(0x01c20C00 + 0x80 )  = 1;
	*(volatile unsigned int *)(0x01c20C00 + 0x8C )  = 0x2EE0;
	*(volatile unsigned int *)(0x01c20C00 + 0x84 )  = 0;
}


void __msdelay(__u32 ms)
{
	__u32 t1, t2;

	t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	t2 = t1 + ms;
	do
	{
		t1 = *(volatile unsigned int *)(0x01c20C00 + 0x84);
	}
	while(t2 >= t1);

	return ;
}

static void timer_exit(void)
{
	*(volatile unsigned int *)(0x01c20000 + 0x144) &= ~(1U << 31);
	*(volatile unsigned int *)(0x01c20C00 + 0x80 )  = 0;
	*(volatile unsigned int *)(0x01c20C00 + 0x8C )  = 0x5DB5DB;
	*(volatile unsigned int *)(0x01c20C00 + 0x84 )  = 0;
}

/*
************************************************************************************
*                          dram_config
*
* Description:
*		dram����
* Parameters:
*		void
* Return value:
*    	0: success
*      !0: fail
* History:
*       void
************************************************************************************
*/
s32 init_dram(__dram_para_t *dram_para)
{
    return init_DRAM(0, dram_para) ? 0 : -1;
}

s32  dram_config(__dram_para_t *dram_para)
{
    if(init_dram(dram_para) == 0){
        note_dram_log();
    }

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
#pragma arm section code="fes1_section"

void fes1_entry(void)
{
	___epos_para_t 	*sys_para = (___epos_para_t *)SYS_PARA_DOWNLOAD_ADDR;
	__dram_para_t  *dram_para = (__dram_para_t *)&(sys_para->dram_para);

	timer_init();
	dram_config(dram_para);
	__msdelay(10);
	timer_exit();
    //��ɱ�־
    note_dram_log();

    return;
}

#pragma arm section




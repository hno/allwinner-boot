/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON driver sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : timer.c
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
#include "include.h"
#include "sw_timer.h"
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
unsigned int mctl_deep_sleep_test(void)
{
	unsigned int i, k;
	unsigned int mem_val;
	unsigned int err_flag = 0;
	unsigned int array[16];

	//fill external memory by sequence number
	for(i=0; i<0x10; i++)
	{
		*((unsigned int *)(0x40000100 + i*4)) = 0x55aaaa55;
	}

	while(1)
	{

		//mctl_deep_sleep_entry();
		dram_power_save_process();
		standby_serial_putc('1');
		k = 0x100;
		for(i=0; i<k; i++){};

		//mctl_deep_sleep_exit();
		dram_power_up_process();
		standby_serial_putc('2');
		//re-check memory content
		for(i=0; i<0x10; i++)
		{
			mem_val = *((unsigned int *)(0x40000100 + i*4));
			if(mem_val != 0x55aaaa55)
			{
				standby_serial_putc('e');
				err_flag = 1;
			}

		}

		if(err_flag == 1)
			return (err_flag);
		standby_serial_putc('4');
		standby_serial_putc('\n');
	}

	return (err_flag);
}







/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : sdmmc_osal_boot0.c
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
#include "sdmmc_for_boot1.h"

__hdle sdcard_gpio_hd;


void SDMMC_OpenAHBClock(__u32 card_no)
{
    *(volatile __u32 *)(CCMU_REGS_BASE + 0x60) |= 1 << (8 + card_no);
}

void SDMMC_CloseAHBClock(__u32 card_no)
{
    *(volatile __u32 *)(CCMU_REGS_BASE + 0x60) &= ~(1 << (8 + card_no));
}

/*
**********************************************************************************************************************
*
*             SDMMC_GetCmuClk
*
*  Description:   获取系统时钟频率
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
__u32 SDMMC_GetCmuClk(void)
{
	__u32 reg_val;
	__u32 div_p, factor_n;
	__u32 factor_k, factor_m;
	__u32 clock;

	reg_val  = *(volatile unsigned int *)(0x01c20000 + 0x20);
	div_p    = (reg_val >> 16) & 0x3;
	factor_n = (reg_val >> 8) & 0x1f;
	factor_k = ((reg_val >> 4) & 0x3) + 1;
	factor_m = ((reg_val >> 0) & 0x3) + 1;

	clock = 24 * factor_n * factor_k/div_p/factor_m;

	return clock;
}
/*
**********************************************************************************************************************
*
*             SDMMC_SetClock
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
__u32 SDMMC_OpenClock(__u32 sdmmc_max_clock, __u32 cmu_clk, __u32 card_no)
{
	__u32 cfg;
	__u32 sdmmc_clk_divid_ratio;

	sdmmc_clk_divid_ratio = cmu_clk / sdmmc_max_clock;
	if (cmu_clk % sdmmc_max_clock)
	{
		sdmmc_clk_divid_ratio ++;
	}
	if (sdmmc_clk_divid_ratio)
	{
		if (sdmmc_clk_divid_ratio > 16)
		{
			sdmmc_clk_divid_ratio = 15;
		}
		else
		{
			sdmmc_clk_divid_ratio --;
	    }
	}
	else
	{
	    return 0;
	}
	cfg = *(volatile __u32 *)(CCMU_REGS_BASE + 0x88 + 0x04 * card_no);

	/*gate on sdmmc clock*/
	cfg |= (1U << 31);
	/*take cmu pll as sdmmc src block*/
	cfg &= ~(0x03 << 24);
	cfg |=  (0x02 << 24);
    /*set ratio*/
	cfg &= ~(0x0f << 0);
	cfg |= (sdmmc_clk_divid_ratio & 0xf);

	*(volatile __u32 *)(CCMU_REGS_BASE + 0x88 + 0x04 * card_no) = cfg;

	return cmu_clk/(sdmmc_clk_divid_ratio + 1);
}
/*
**********************************************************************************************************************
*
*             SDMMC_SetClock
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
__s32 SDMMC_CloseClock(__u32 card_no)
{
	*(volatile __u32 *)(CCMU_REGS_BASE + 0x88 + 0x04 * card_no) &= ~(1U << 31);

	return 0;
}
/*
**********************************************************************************************************************
*
*             SDMMC_SetPin
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
void SDMMC_SetPinOn(__u32 gpio_no, __u32 line_sel)
{
    user_gpio_set_t  gpio_set[8];
	__s32            ret;

	memset(gpio_set, 0, sizeof(user_gpio_set_t)*8);
	ret = wBoot_script_parser_mainkey_get_gpio_cfg("card_boot2_para", (void *)gpio_set, 8);
	if(!ret)
	{
		sdcard_gpio_hd = wBoot_GPIO_Request(gpio_set, 8);
	}
}
/*
**********************************************************************************************************************
*
*             SDMMC_SetPin
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
void SDMMC_SetPinOff(__u32 gpio_no, __u32 line_sel)
{
	if(sdcard_gpio_hd)
    {
        wBoot_GPIO_Release(sdcard_gpio_hd, 0);
    }
}
/*
**********************************************************************************************************************
*
*             OSAL_printf
*
*  Description:  用户可以自行设定是否需要打印
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
__s32 sdmmc_printf(const char * str, ...)
{
    return 0;
}

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
#include "include.h"
#include "types.h"

int usbpc_cur = 0, usbpc_vol = 4000;
int usbdc_cur = 0, usbdc_vol = 4000;
__u32 usb_ac_enable_gpio_hd  = 0;
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
int power_set_init(void)
{
    script_gpio_set_t  gpio;
    int ret;
	__inf("set pc\n");
    ret = wBoot_script_parser_fetch("usbc0", "usb_ac_enable_gpio", &gpio, 1);    
	if(ret >= 0)
	{
       usb_ac_enable_gpio_hd = wBoot_GPIO_Request(&gpio, 1);
    }
	wBoot_script_parser_fetch("pmu_para", "pmu_usbvol", &usbdc_vol, 1);
	wBoot_script_parser_fetch("pmu_para", "pmu_usbcur", &usbdc_cur, 1);
	wBoot_script_parser_fetch("pmu_para", "pmu_usbvol_pc", &usbpc_vol, 1);
	wBoot_script_parser_fetch("pmu_para", "pmu_usbcur_pc", &usbpc_cur, 1);
	__inf("usbdc_vol = %d, usbdc_cur = %d\n", usbdc_vol, usbdc_cur);
	__inf("usbpc_vol = %d, usbpc_cur = %d\n", usbpc_vol, usbpc_cur);

	return 0;
}
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
void power_set_usbpc(void)
{
    __inf("set pc\n");
    if(usb_ac_enable_gpio_hd)
    {
        wBoot_GPIO_Write_One_PIN_Value(usb_ac_enable_gpio_hd, 0, "usb_ac_enable_gpio");
    }
	wBoot_power_cur_limit(usbpc_cur);
	wBoot_power_vol_limit(usbpc_vol);
}
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
void power_set_usbdc(void)
{
    __inf("set dc\n");
	if(usb_ac_enable_gpio_hd)
    {
        wBoot_GPIO_Write_One_PIN_Value(usb_ac_enable_gpio_hd, 1, "usb_ac_enable_gpio");
    }
	wBoot_power_cur_limit(usbdc_cur);
	wBoot_power_vol_limit(usbdc_vol);
}


void system_reset(void)
{
	WATCHDOG_REG_CTRL = 0xA57<<1;
    WATCHDOG_REG_CTRL &= ~1;
    WATCHDOG_REG_CTRL = 0;
    WATCHDOG_REG_MODE = 0x13;
    WATCHDOG_REG_MODE = 0x0;
    //开始重启
	wBoot_block_exit();
	WATCHDOG_REG_MODE = 0x3;
	wBoot_timer_delay(50);
	while(1);
}


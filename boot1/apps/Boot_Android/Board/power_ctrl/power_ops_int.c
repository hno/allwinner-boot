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
#include  "axp_i.h"

#define NMI_CTL_REG            (0x01c00030)
#define NMI_IRG_PENDING_REG    (0x01c00034)
#define NMI_INT_ENABLE_REG     (0x01c00038)

#define writel(v, addr)	(*((volatile unsigned long  *)(addr)) = (unsigned long)(v))

static  __u8  power_int_value[5];
static  __u32 power_int_working = 0;

extern  __u32  tmr_hd;
extern  int    usb_working;
extern  int    power_ops_int_status;

__s32 BOOT_TWI_Read(__u32 arg1, __u8 *arg2, __u8 *arg3)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = 1;
    twi_data.data = arg3;
    twi_data.if_restart = 1;

    return wBoot_twi_read((void *)&twi_data);
}

__s32  BOOT_TWI_Write(__u32 arg1, __u8 *arg2, __u8 *arg3)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = 1;
    twi_data.data = arg3;

    return wBoot_twi_write((void *)&twi_data);
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
void boot_power_nmi_enable(void)
{

    writel(0x00,NMI_CTL_REG); //nagative edge trigger
    writel(0x01,NMI_IRG_PENDING_REG); //clean the NMI pendding
    writel(0x01,NMI_INT_ENABLE_REG); //enable the NMI irq
    
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
void boot_power_nmi_disable(void)
{

    writel(0x00,NMI_CTL_REG); 
    writel(0x01,NMI_IRG_PENDING_REG);
    writel(0x00,NMI_INT_ENABLE_REG);

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

__s32 boot_power_int_enable(void)
{
    __u8  reg_addr;
    __u8  int_enable[5];
    int	  i;

    for(i=0;i<5;i++)
    {
    	reg_addr = BOOT_POWER20_INTEN1 + i;
    	if(BOOT_TWI_Read(AXP20_ADDR, &reg_addr, power_int_value + i))
    	{
    		__inf("twi read err\n");
        	return -1;
    	}
    }

	int_enable[0] = 0x0C;	//开启：VBUS移除，VBUS插入
	int_enable[1] = 0;		//
	int_enable[2] = 0x03;	//开启：电源按键短按，长按
	int_enable[3] = 0;
	int_enable[4] = 0;

	for(i=0;i<5;i++)
	{
		reg_addr = BOOT_POWER20_INTEN1 + i;
    	if(BOOT_TWI_Write(AXP20_ADDR, &reg_addr, int_enable + i))
    	{
    		__inf("twi write err\n");
        	return -1;
    	}
    }
    
    boot_power_nmi_enable();


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
__s32 boot_power_int_disable(void)
{
    __u8  reg_addr;
    int   i;

	for(i=0;i<5;i++)
	{
		reg_addr = BOOT_POWER20_INTEN1 + i;
    	if(BOOT_TWI_Write(AXP20_ADDR, &reg_addr, power_int_value + i))
    	{
    		__inf("twi write err\n");
        	return -1;
    	}
    }
    
    boot_power_nmi_disable();
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
__s32 eGon2_power_int_query(__u8 *int_status)
{
    __u8  reg_addr;
    int	  i;

	for(i=0;i<5;i++)
	{
		reg_addr = BOOT_POWER20_INTSTS1 + i;
    	if(BOOT_TWI_Read(AXP20_ADDR, &reg_addr, int_status + i))
    	{
    		__inf("twi read err\n");
        	return -1;
    	}
        __debug("int_status[%d]=%x\n",i,(int)int_status[i]);
    	if(BOOT_TWI_Write(AXP20_ADDR, &reg_addr, int_status + i))
    	{
    		__inf("twi write err\n");
        	return -1;
    	}
    }

    writel(0x01,NMI_IRG_PENDING_REG);


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
void power_int_handler(void *p_arg)
{
	__u8 status[5];

	status[0] = 0;
    __debug("%s\n",__FUNCTION__);
	eGon2_power_int_query(status);
	if(status[0] & 0x04)		//vbus移除
	{
		__inf("vbus remove\n");
		usb_working = 0;
		usb_detect_exit();
		power_set_usbpc();
	}
	else if(status[0] & 0x08)	//vbus接入，启动usb去检测
	{
		__inf("vbus insert\n");
		power_ops_int_status |= 0x04;
		usb_detect_enter();
	}
	if(status[2] & 0x02)		//短按
	{
		power_ops_int_status |= 0x02;
	}
	else if(status[2] & 0x01)	//长按
	{
		power_ops_int_status |= 0x01;
	}
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
void power_int_reg(void)
{
    __u8 status[5];
	__inf("power start detect\n");
	if(!power_int_working)
	{
		__inf("power enter detect\n");
		power_ops_int_status = 0;
		power_int_working = 1;
		boot_power_int_enable();
		wBoot_InsINT_Func(GIC_SRC_NMI, (int *)power_int_handler, 0);
        wBoot_EnableInt(GIC_SRC_NMI);
        
	}
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
void power_int_rel(void)
{
	__inf("power exit detect\n");
	boot_power_int_disable();
    wBoot_DisableInt(GIC_SRC_NMI);
	power_int_working = 0;
	power_ops_int_status = 0;

}

